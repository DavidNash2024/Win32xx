////////////////////////////////////////
// SimpleApp.cpp

#include "stdafx.h"
#include "SimpleApp.h"


// Definitions for the CSimpleRibbonApp class
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
