/////////////////////////////
// main.cpp
//

#include "stdafx.h"
#include "SimpleApp.h"


// For VS2019
// int WINAPI WinMain (_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
int WINAPI WinMain(__in HINSTANCE, __in_opt HINSTANCE, __in LPSTR, __in int)
{
    try
    {
        // Start Win32++
        CSimpleRibbonApp theApp;

        // Run the application
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
