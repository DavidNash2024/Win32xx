//////////////////////////////////////////////
// App.cpp

#include "stdafx.h"
#include "App.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        // Start Win32++
        CApp theApp;

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


// Definitions for the CApp class
CApp::CApp()
{
}

CApp::~CApp()
{
}

BOOL CApp::InitInstance()
{
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure  

    return TRUE;
}

