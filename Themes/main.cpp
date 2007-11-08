///////////////////////////////////////
// main.cpp

#include "ThemesApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CThemesApp theApp(hInstance);

	// Run the application
	return theApp.Run();
}


