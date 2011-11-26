////////////////////////////////
// Main.cpp

// Also add the Win32++\include  directory to project's additional include directories
// Also add the Win32++\include  directory to project's additional resource directories

#include "ScribbleApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // Start Win32++
    CScribbleApp MyApp;

	// Run the application
    return MyApp.Run();
}
