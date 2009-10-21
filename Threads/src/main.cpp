///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "ThreadApp.h"


INT WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CThreadApp theApp;

	// Run the application
	theApp.Run();
	
	return 0;
}


