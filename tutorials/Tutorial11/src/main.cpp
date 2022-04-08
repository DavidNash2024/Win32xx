////////////////////////////////
// Main.cpp

#include "stdafx.h"
#include "ScribbleApp.h"


// WinMain is the program's entry point. The program starts here.
// For VS2019
// int WINAPI WinMain (_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        // Start Win32++
        CScribbleApp theApp;

        // Run the application.
        return theApp.Run();
    }

    // Catch all unhandled CException types.
    catch (const CException &e)
    {
        // Display the exception and quit.
        ::MessageBox(0, e.GetErrorString(), e.GetText(), MB_ICONWARNING);

        return -1;
    }
}
