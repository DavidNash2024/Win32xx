/////////////////////////////
//FrameApp.cpp
//

#include "stdafx.h"
#include "StatusBarApp.h"

//////////////////////////////////////
// CStatusBarApp function definitions.
//
CStatusBarApp::CStatusBarApp()
{
}

CStatusBarApp::~CStatusBarApp()
{
}

BOOL CStatusBarApp::InitInstance()
{
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}

