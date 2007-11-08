///////////////////////////////////////
// main.cpp

#include "MDIFrameApp.h"

INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
    CMDIFrameApp MyApp(hInstance);

	// Run the application
	return MyApp.Run();
}

