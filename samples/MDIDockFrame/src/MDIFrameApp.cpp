/////////////////////////////
// MDIFrameApp.cpp
//

#include "stdafx.h"
#include "MDIFrameApp.h"

////////////////////////////////////
// CMDIFrameApp function definitions
//

// Called when the application starts.
BOOL CMDIFrameApp::InitInstance()
{
    // Create the frame window.
    m_mainMDIFrame.Create();    // throws a CWinException on failure.

    return TRUE;
}

