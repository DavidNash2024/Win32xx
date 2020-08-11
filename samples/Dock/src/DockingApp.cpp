/////////////////////////////
// DockingApp.cpp
//

#include "stdafx.h"
#include "DockingApp.h"

///////////////////////////////////
// CDockingApp function definitions
//

// Constructor.
CDockingApp::CDockingApp()
{
}

// Called when the applications starts.
BOOL CDockingApp::InitInstance()
{
    //Create the Window
    m_MainFrame.Create();   // throws a CWinException on failure

    return TRUE;
}


