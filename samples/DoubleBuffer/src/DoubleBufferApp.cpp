/////////////////////////////
// DoubleBufferApp.cpp
//

#include "stdafx.h"
#include "DoubleBufferApp.h"

////////////////////////////////////////
// CDoubleBufferApp function definitions
//

// Called after the window is created.
BOOL CDoubleBufferApp::InitInstance()
{
    // Create the frame window.
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}

