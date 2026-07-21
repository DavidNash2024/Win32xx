/////////////////////////////
//FrameApp.cpp
//

#include "pch.h"
#include "FrameApp.h"

/////////////////////////////////
// CFrameApp function definitions
//

// Called when the application starts.
BOOL CFrameApp::InitInstance()
{
    // Create the frame window.
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}

