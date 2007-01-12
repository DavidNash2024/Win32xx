///////////////////////////////////////
// main.cpp


#include "tchar.h"
#include "MDIFrameApp.h"

INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
    CMDIFrameApp MyApp(hInstance);

    // Process the window messages
	return MyApp.MessageLoop();
}

