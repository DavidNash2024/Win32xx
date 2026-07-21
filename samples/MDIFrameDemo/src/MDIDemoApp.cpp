/////////////////////////////
// MDIDemoApp.h
//

#include "pch.h"
#include "MDIDemoApp.h"
#include "resource.h"

///////////////////////////////////
// CMDIDemoApp function definitions
//

// Called when the application starts.
BOOL CMDIDemoApp::InitInstance()
{
    // Create the window
    m_mainMDIFrame.Create();    // throws a CWinException on failure

    return TRUE;
}
