/////////////////////////////
// ThemesApp.cpp
//

#include "stdafx.h"
#include "ThemesApp.h"

//////////////////////////////////
// CFrameApp function definitions.
//
CThemesApp::CThemesApp()
{
}

CThemesApp::~CThemesApp()
{
}

BOOL CThemesApp::InitInstance()
{
    // Create the frame window.
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}
