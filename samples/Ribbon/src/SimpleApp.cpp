/////////////////////////////
// SimpleApp.cpp
//

#include "stdafx.h"
#include "SimpleApp.h"

/////////////////////////////////////////
// CSimpleRibbonApp function definitions.
//

// This function is called automatically when the application starts.
BOOL CSimpleRibbonApp::InitInstance()
{
    // Create the window.
    m_view.Create();    // throws a CWinException on failure.

    return TRUE;
}
