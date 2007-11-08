///////////////////////////////////////
// main.cpp

#include "SubApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CSubApp theApp(hInstance);

	// Run the application
	return theApp.Run();
}


