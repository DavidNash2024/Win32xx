///////////////////////////////////////
// main.cpp

#include "DlgSubclassApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CDlgSubclassApp theApp(hInstance);

	// Run the application
	return theApp.Run();
}

