///////////////////////////////////////
// main.cpp


#include "ThreadApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CThreadApp theApp(hInstance);

	// Process the window messages for the main thread
	return theApp.MessageLoop();
}


