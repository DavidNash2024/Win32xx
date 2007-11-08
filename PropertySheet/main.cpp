///////////////////////////////////////
// main.cpp

#include "PropertySheetApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CPropertySheetApp theApp(hInstance);

	// Run the application
	return theApp.Run();
}


