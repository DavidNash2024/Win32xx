/////////////////////////////
// App.cpp
//

#include "stdafx.h"
#include "App.h"



////////////////////////////
// CApp function definitions
//

// Called when the application starts.
BOOL CApp::InitInstance()
{
    // Create the frame window.
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}

