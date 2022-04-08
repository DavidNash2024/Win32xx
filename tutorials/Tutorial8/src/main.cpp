////////////////////////////////
// Main.cpp

// Also add the Win32++\include  directory to project's additional include directories
// Also add the Win32++\include  directory to project's additional resource directories

#include "targetver.h"
#include "ScribbleApp.h"


// WinMain is the program's entry point. The program starts here.
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // Start Win32++
    CScribbleApp theApp;

    // Run the application
    return theApp.Run();
}
