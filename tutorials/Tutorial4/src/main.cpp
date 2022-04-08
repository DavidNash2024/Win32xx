///////////////////////////////////////
// main.cpp

// Also add the Win32++\include  directory to project's additional include directories
// Also add the Win32++\include  directory to project's additional resource directories

// VS6 requires these macros to be defined.
#define WINVER          0x0410
#define _WIN32_WINDOWS  0x0410
#define _WIN32_IE       0x0401

#include "ScribbleApp.h"


// WinMain is the program's entry point. The program starts here.
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // Start Win32++
    CScribbleApp theApp;

    // Run the application
    return theApp.Run();
}
