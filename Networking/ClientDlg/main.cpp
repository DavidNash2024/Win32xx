///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "DialogApp.h"


INT WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CDialogApp theApp;

	// Run the application
	return theApp.Run();
}

