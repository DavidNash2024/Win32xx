//////////////////////////////////////////////
//FrameApp.cpp

#include "stdafx.h"
#include "FrameApp.h"


// Definitions for the CToolBarDemoApp class
CToolBarDemoApp::CToolBarDemoApp()
{
}

CToolBarDemoApp::~CToolBarDemoApp()
{
}

BOOL CToolBarDemoApp::InitInstance()
{
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}


