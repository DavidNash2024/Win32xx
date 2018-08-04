//////////////////////////////////////////////
// ThemesApp.cpp

#include "stdafx.h"
#include "ThemesApp.h"


// Definitions for the CFrameApp class
CThemesApp::CThemesApp()
{
}

CThemesApp::~CThemesApp()
{
}

BOOL CThemesApp::InitInstance()
{
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure  

    return TRUE;
}
