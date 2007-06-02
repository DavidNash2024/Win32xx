///////////////////////////////////////
// main.cpp

#include "PerfApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CPerformanceApp theApp(hInstance);

	// Process the window messages
	return theApp.MessageLoop();
}


