///////////////////////////////////////
// main.cpp

#include "FrameApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CFrameApp theApp(hInstance);

	// Process the window messages
	return theApp.MessageLoop();
}


