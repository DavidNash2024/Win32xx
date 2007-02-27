///////////////////////////////////////
// main.cpp

#include <tchar.h>		// Support for _tWinMain
#include "ThreadApp.h"
#include "MainWin.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CThreadApp theApp(hInstance);

	// Process the window messages for the main thread
	return theApp.MessageLoop();
}


