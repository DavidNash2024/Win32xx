///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "TextApp.h"


INT WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CTextApp theApp;

	// Run the application
	return theApp.Run();
}


