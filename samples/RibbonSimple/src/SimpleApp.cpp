/////////////////////////////
// SimpleApp.cpp
//

#include "stdafx.h"
#include "SimpleApp.h"

/////////////////////////////////////////
// CSimpleRibbonApp function definitions.
//
CSimpleRibbonApp::CSimpleRibbonApp()
{
    // Constructor for CSimpleRibbonApp
}

CSimpleRibbonApp::~CSimpleRibbonApp()
{
}

BOOL CSimpleRibbonApp::InitInstance()
{
    // This function is called automatically when the application starts

    // Create the Window
    m_view.Create();    // throws a CWinException on failure

    return TRUE;
}
