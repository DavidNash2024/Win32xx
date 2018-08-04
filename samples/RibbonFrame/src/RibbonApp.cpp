////////////////////////////////////////
// RibbonApp.cpp

#include "stdafx.h"
#include "RibbonApp.h"


// Definitions for the CRibbonFrameApp class
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
    m_frame.Create();   // throws a CWinException on failure  

    return TRUE;
}
