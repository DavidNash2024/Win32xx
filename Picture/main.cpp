///////////////////////////////////////
// main.cpp

#include "PictureApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CPictureApp theApp(hInstance);

	// Process the window messages
	return theApp.MessageLoop();
}


