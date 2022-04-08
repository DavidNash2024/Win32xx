/////////////////////////////
// main.cpp
//

#include "stdafx.h"
#include "DXApp.h"


// For VS2019
// int WINAPI WinMain (_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        // Start Win32++
        CDXApp MyApp;

        // Run the application.
        return MyApp.Run();
    }

    // Catch all unhandled CException types.
    catch (const CException &e)
    {
        // Display the exception and quit.
        ::MessageBox(NULL, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return -1;
    }
}
