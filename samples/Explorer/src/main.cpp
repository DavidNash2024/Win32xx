///////////////////////////////////////
// main.cpp

#include "stdafx.h"
#include "ExplorerApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        // Start Win32++
        CExplorerApp theApp;

        // Run the application
        return theApp.Run();
    }
    
    // catch all unhandled CException types
    catch (const CException &e)
    {
        // Display the exception and quit
        MessageBox(NULL, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return -1;
    }
}

