/////////////////////////////
// ScintillaApp.cpp
//

#include "stdafx.h"
#include "ScintillaApp.h"


/////////////////////////////////
// ScintillaApp function definitions
//

// Constructor.
ScintillaApp::ScintillaApp()
{
}

// Destructor.
ScintillaApp::~ScintillaApp()
{
}

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


