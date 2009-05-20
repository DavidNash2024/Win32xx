///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "FrameApp.h"


INT WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CFrameApp theApp;

	// Run the application
	return theApp.Run();
}

// Note:
// _tWinMain is the Unicode variant of WinMain.
