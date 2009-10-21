///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "MDIDemoApp.h"


INT WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
    CMDIDemoApp MyApp;

	// Run the application
	return MyApp.Run();
}

