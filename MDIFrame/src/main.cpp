///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "MDIFrameApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Start Win32++
    CMDIFrameApp MyApp;

	// Run the application
	return MyApp.Run();
}

