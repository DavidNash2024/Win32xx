///////////////////////////////////////
// main.cpp

#include "SplitterApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CSplitterApp theApp(hInstance);

	// Run the application
	return theApp.Run();
}


