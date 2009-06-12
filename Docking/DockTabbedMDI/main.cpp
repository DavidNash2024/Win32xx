///////////////////////////////////////
// main.cpp

#include "TabbedMDIApp.h"


INT WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CTabbedMDIApp theApp;

	// Run the application
	return theApp.Run();
}


