/////////////////////////////
// FrameApp.cpp
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
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}

// Called when the message queue is empty.
BOOL CFrameApp::OnIdle(LONG)
{
    m_frame.UpdateToolbar();

    return FALSE;
}
