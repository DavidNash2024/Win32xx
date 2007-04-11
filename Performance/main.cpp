///////////////////////////////////////
// main.cpp

#include <tchar.h>		// Support for _tWinMain
#include "PerfApp.h"
#include "MainWnd.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CPerformanceApp theApp(hInstance);

	// Process the window messages for the main thread
	return theApp.MessageLoop();
}


