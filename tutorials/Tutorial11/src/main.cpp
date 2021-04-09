////////////////////////////////
// Main.cpp

#include "stdafx.h"
#include "ScribbleApp.h"


// WinMain is the program's entry point. The program starts here.
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
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
