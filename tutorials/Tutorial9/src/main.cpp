////////////////////////////////
// Main.cpp

// Also add the Win32++\include  directory to project's additional include directories.
// Also add the Win32++\include  directory to project's additional resource directories.

#include "ScribbleApp.h"


// WinMain is the program's entry point. The program starts here.
int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
    // Start Win32++.
    CScribbleApp theApp;

    // Run the application.
    return theApp.Run();
}
