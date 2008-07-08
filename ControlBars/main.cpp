///////////////////////////////////////
// main.cpp

#include <tchar.h>
#include "DialogApp.h"


INT WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	// Start DevWin++
	CDialogApp theApp;

	// Run the application
	return theApp.Run();
}

