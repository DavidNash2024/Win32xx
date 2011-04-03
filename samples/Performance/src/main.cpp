///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "PerfApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Start Win32++
	CPerformanceApp theApp;

	// Run the application
	return theApp.Run();
}


