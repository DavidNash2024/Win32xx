/////////////////////////////
//FrameApp.cpp
//

#include "stdafx.h"
#include "FrameApp.h"

/////////////////////////////////
// CFrameApp function definitions
//

// Constructor.
CFrameApp::CFrameApp()
{
}

// Destructor.
CFrameApp::~CFrameApp()
{
}

// Called when the application starts.
BOOL CFrameApp::InitInstance()
{
    // Create the frame window.
    m_Frame.Create();   // throws a CWinException on failure

    return TRUE;
}

