///////////////////////////////////////
// main.cpp

#include "SuperclassApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CSuperclassApp theApp(hInstance);

	// Process the window messages
	return theApp.MessageLoop();
}


