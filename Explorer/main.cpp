///////////////////////////////////////
// main.cpp

#include <tchar.h>		// Support for _tWinMain
#include "ShellApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CShellApp theApp(hInstance);

	// Process the window messages
	return theApp.MessageLoop();
}


