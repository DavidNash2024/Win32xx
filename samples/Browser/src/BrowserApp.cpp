/////////////////////////////
// BrowserApp.cpp
//

#include "stdafx.h"
#include "BrowserApp.h"

///////////////////////////////////
// CBrowserApp function definitions
//

// Constructor for CBrowserApp
CBrowserApp::CBrowserApp()
{
}

// This function is called automatically when the application starts.
BOOL CBrowserApp::InitInstance()
{
    // Create the Frame
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}
