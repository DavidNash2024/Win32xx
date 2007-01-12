///////////////////////////////////////
// main.cpp

#include <tchar.h>
#include "DialogApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CDialogApp theApp(hInstance);

	// Process the window messages
	return theApp.MessageLoop();
}

