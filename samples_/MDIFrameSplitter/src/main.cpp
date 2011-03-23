///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "MDIFrameSplitterApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Start Win32++
    CMDIFrameSplitterApp MyApp;

	// Run the application
	return MyApp.Run();
}

