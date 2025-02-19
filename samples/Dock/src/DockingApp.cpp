/////////////////////////////
// DockingApp.cpp
//

#include "stdafx.h"
#include "DockingApp.h"

///////////////////////////////////
// CDockingApp function definitions
//

// Called when the applications starts.
BOOL CDockingApp::InitInstance()
{
    // Create the frame window.
    m_MainFrame.Create();   // throws a CWinException on failure

    return TRUE;
}


