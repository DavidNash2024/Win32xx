///////////////////////////////////////
// main.cpp

#include "ShellApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CShellApp theApp(hInstance);

	// Run the application
	return theApp.Run();
}


