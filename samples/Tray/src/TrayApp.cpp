////////////////////////////////////////
// SimpleApp.cpp

#include "stdafx.h"
#include "TrayApp.h"


// Definitions for the CTrayApp class
CTrayApp::CTrayApp()
{
    // Constructor for CTrayApp
}

BOOL CTrayApp::InitInstance()
{
    // This function is called automatically when the application starts

    // Create the Window
    m_View.Create();    // throws a CWinException on failure  

    return TRUE;
}
