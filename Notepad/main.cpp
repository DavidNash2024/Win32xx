///////////////////////////////////////
// main.cpp

#include "TextApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CTextApp theApp(hInstance);

	// Process the window messages
	return theApp.MessageLoop();
}


