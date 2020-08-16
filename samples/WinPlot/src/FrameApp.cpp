/////////////////////////////
//FrameApp.cpp
//

#include "stdafx.h"
#include "FrameApp.h"

//////////////////////////////////
// CFrameApp function definitions.
//
CFrameApp::CFrameApp()
{
}

CFrameApp::~CFrameApp()
{
}

BOOL CFrameApp::InitInstance()
{
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}


