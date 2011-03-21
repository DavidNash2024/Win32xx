///////////////////////////////////////
// main.cpp


#include "stdafx.h"
#include "DockingApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Start Win32++
    CDockingApp MyApp;

	// Run the application
	return MyApp.Run();
}

