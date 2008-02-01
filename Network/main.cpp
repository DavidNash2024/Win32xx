///////////////////////////////////////
// main.cpp

#include "NetworkApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CNetworkApp theApp(hInstance);

	// Run the application
	return theApp.Run();
}


