///////////////////////////////////////
// main.cpp

#include "PropertySheetApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CPSApp theApp(hInstance);

	// Process the window messages
	return theApp.MessageLoop();
}


