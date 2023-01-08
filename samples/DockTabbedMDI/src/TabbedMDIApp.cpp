/////////////////////////////
// TabbedMDIApp.cpp
//

#include "stdafx.h"
#include "TabbedMDIApp.h"

/////////////////////////////////////////
// CDockContainerApp function definitions
//

// Constructor.
CTabbedMDIApp::CTabbedMDIApp()
{
}

// Destructor.
CTabbedMDIApp::~CTabbedMDIApp()
{
}

// Called when the application starts.
BOOL CTabbedMDIApp::InitInstance()
{
    // Create the frame window.
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}

