///////////////////////////////////////
// main.cpp

#include <tchar.h>
#include "DialogApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CDialogApp theApp(hInstance);

	// Run the application
	return theApp.Run();
}

