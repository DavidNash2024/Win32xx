//////////////////////////////////////////////
// FormApp.cpp

#include "stdafx.h"
#include "FormApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        // Start Win32++
        CFormApp theApp;

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


// Definitions for the CFormApp class
CFormApp::CFormApp()
{
}

CFormApp::~CFormApp()
{
}

BOOL CFormApp::InitInstance()
{
    //Create the Frame Window
    m_Frame.Create();   // throws a CWinException on failure  

    return TRUE;
}

