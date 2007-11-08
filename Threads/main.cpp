///////////////////////////////////////
// main.cpp


#include "ThreadApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CThreadApp theApp(hInstance);

	// Run the application
	return theApp.Run();
}


