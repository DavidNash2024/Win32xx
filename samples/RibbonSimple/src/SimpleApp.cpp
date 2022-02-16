/////////////////////////////
// SimpleApp.cpp
//

#include "stdafx.h"
#include "SimpleApp.h"

/////////////////////////////////////////
// CSimpleRibbonApp function definitions.
//

// Constructor for CSimpleRibbonApp.
CSimpleRibbonApp::CSimpleRibbonApp()
{  
}

// Destructor for CSimpleRibbonApp.
CSimpleRibbonApp::~CSimpleRibbonApp()
{
}

// This function is called automatically when the application starts.
BOOL CSimpleRibbonApp::InitInstance()
{
    // Create the Window
    m_view.Create();    // throws a CWinException on failure.

    return TRUE;
}
