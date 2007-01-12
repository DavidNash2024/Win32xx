///////////////////////////////////////
// main.cpp

#include <tchar.h>		// Support for _tWinMain
#include "RebarFrameApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CRebarFrameApp theApp(hInstance);

	// Process the window messages
	return theApp.MessageLoop();
}


