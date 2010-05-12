///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "TabbedMDIApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Start Win32++
	CTabbedMDIApp theApp;

	// Run the application
	return theApp.Run();
}


