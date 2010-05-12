///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "ContainerApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Start Win32++
	CDockContainerApp theApp;

	// Run the application
	return theApp.Run();
}


