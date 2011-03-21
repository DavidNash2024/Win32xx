
#include "stdafx.h"
#include "SimpleApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Start Win32++
    CSimpleRibbonApp MyApp;

	// Run the application
    return MyApp.Run();
}
