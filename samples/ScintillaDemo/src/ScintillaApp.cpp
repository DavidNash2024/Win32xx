/////////////////////////////
// ScintillaApp.cpp
//

#include "pch.h"
#include "ScintillaApp.h"


/////////////////////////////////
// ScintillaApp function definitions
//

// Called when the application starts.
BOOL ScintillaApp::InitInstance()
{
    // Create the frame window.
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}

// Called when the message queue is empty.
BOOL ScintillaApp::OnIdle(LONG)
{
    m_frame.UpdateToolbar();

    return FALSE;
}


