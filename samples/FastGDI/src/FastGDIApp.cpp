//////////////////////////////////////////////
// FastGDIApp.cpp

#include "stdafx.h"
#include "FastGDIApp.h"

///////////////////////////////////
// CFastGDIApp function definitions
//

// Called when the application starts.
BOOL CFastGDIApp::InitInstance()
{
    // Create the frame window.
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}

// Called when the message queue is empty.
BOOL CFastGDIApp::OnIdle(LONG)
{
    m_frame.UpdateToolbar();

    return FALSE;
}
