/////////////////////////////
// TabDemoApp.cpp
//

#include "stdafx.h"
#include "TabDemoApp.h"

////////////////////////////////////
// CTabDemoApp function definitions.
//
CTabDemoApp::CTabDemoApp()
{
}

CTabDemoApp::~CTabDemoApp()
{
}

BOOL CTabDemoApp::InitInstance()
{
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}


