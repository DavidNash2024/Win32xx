///////////////////////////////////////
// main.cpp


#include "stdafx.h"
#include "PropertySheetApp.h"


INT WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CPropertySheetApp theApp;

	// Run the application
	return theApp.Run();
}


