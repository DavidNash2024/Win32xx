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
    // Create the frame window.
    m_frame.Create();   // Throws a CWinException on failure.

    return TRUE;
}

