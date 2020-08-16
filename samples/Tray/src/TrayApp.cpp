/////////////////////////////
// SimpleApp.cpp
//

#include "stdafx.h"
#include "TrayApp.h"

/////////////////////////////////
// CTrayApp function definitions.
//
CTrayApp::CTrayApp()
{
    // Constructor for CTrayApp
}

BOOL CTrayApp::InitInstance()
{
    // This function is called automatically when the application starts

    // Create the Window
    m_view.Create();    // throws a CWinException on failure

    return TRUE;
}
