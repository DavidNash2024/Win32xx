/////////////////////////////
// ExplorerApp.cpp
//

#include "stdafx.h"
#include "ExplorerApp.h"
#include "resource.h"

////////////////////////////////////
// CExplorerApp function definitions
//

// Destructor.
CExplorerApp::~CExplorerApp()
{
}

// Called when the application starts.
BOOL CExplorerApp::InitInstance()
{
    // Create the frame window.
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}

