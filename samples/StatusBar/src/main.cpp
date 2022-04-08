/////////////////////////////
// main.cpp
//

#include "stdafx.h"
#include "StatusBarApp.h"


// For VS2019
// int WINAPI WinMain (_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        // Start Win32++
        CStatusBarApp theApp;

        // Run the application and the message loop
        return theApp.Run();
    }

    // catch all unhandled CException types
    catch (const CException &e)
    {
        // Display the exception and quit
        MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return -1;
    }
}

