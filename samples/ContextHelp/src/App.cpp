/////////////////////////////
// App.cpp
//

#include "stdafx.h"
#include "App.h"


// For VS2019
// int WINAPI WinMain (_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
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


////////////////////////////
// CApp function definitions
//

// Constructor.
CApp::CApp()
{
}

// Destructor.
CApp::~CApp()
{
}

// Called when the application starts.
BOOL CApp::InitInstance()
{
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}

