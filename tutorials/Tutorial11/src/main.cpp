////////////////////////////////
// Main.cpp

#include "stdafx.h"
#include "ScribbleApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        // Start Win32++
        CScribbleApp theApp;

        // Run the application
        return theApp.Run();
    }
    
    // catch all unhandled CException types
    catch (const CException &e)
    {
        // Display the exception and quit
        MessageBox(NULL, e.GetErrorString(), e.GetText(), MB_ICONWARNING);

        return -1;
    }
}
