///////////////////////////////////////
// main.cpp


#include "ThreadApp.h"


INT WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CThreadApp theApp;

	// Run the application
	return theApp.Run();
}


