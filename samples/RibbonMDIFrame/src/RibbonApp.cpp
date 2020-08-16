/////////////////////////////
// RibbonApp.cpp
//

#include "stdafx.h"
#include "RibbonApp.h"

////////////////////////////////////////
// CRibbonFrameApp function definitions.
//
CRibbonFrameApp::CRibbonFrameApp()
{
    // Constructor for CRibbonFrameApp
}

CRibbonFrameApp::~CRibbonFrameApp()
{
}

BOOL CRibbonFrameApp::InitInstance()
{
    // This function is called automatically when the application starts

    // Create the Window
    m_Frame.Create();   // throws a CWinException on failure

    return TRUE;
}
