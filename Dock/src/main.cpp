///////////////////////////////////////
// main.cpp


#include "stdafx.h"
#include "DockingApp.h"


INT WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
    CDockingApp MyApp;

	// Run the application
	return MyApp.Run();
}

