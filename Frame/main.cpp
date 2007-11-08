///////////////////////////////////////
// main.cpp

#include "FrameApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CFrameApp theApp(hInstance);

	// Run the application
	return theApp.Run();
}


