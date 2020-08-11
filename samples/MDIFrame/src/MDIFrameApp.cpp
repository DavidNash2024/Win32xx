/////////////////////////////
// MDIFrameApp.cpp
//

#include "stdafx.h"
#include "MDIFrameApp.h"

////////////////////////////////////
// CMDIFrameApp function definitions
//

// Constructor.
CMDIFrameApp::CMDIFrameApp()
{
}

// Called when the application starts.
BOOL CMDIFrameApp::InitInstance()
{
    // Create the MDI frame window.
    m_mainMDIFrame.Create();    // throws a CWinException on failure

    return TRUE;
}

