/////////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "TrayApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // Start Win32++
    CTrayApp MyApp;

	// Run the application
    return MyApp.Run();
}



