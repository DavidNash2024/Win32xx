///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "DialogDemoApp.h"


INT WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CDlgSubclassApp theApp;

	// Run the application
	return theApp.Run();
}

