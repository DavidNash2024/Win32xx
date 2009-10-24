///////////////////////////////////////
// main.cpp

// Add the Win32++\include  directory to project's additional include directories
#include "ScribbleApp.h"


INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    // Start Win32++
    CScribbleApp MyApp;

	// Run the application
    return MyApp.Run();
}
