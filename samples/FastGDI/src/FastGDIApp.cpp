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

