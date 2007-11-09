///////////////////////////////////////
// main.cpp

#include "FormApp.h"


INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	// Start Win32++
	CFormApp theApp(hInstance);

	// Run the application
	return theApp.Run();
}


