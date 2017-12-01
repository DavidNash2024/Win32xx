//////////////////////////////////////////////
//FrameApp.cpp

#include "stdafx.h"
#include "FrameApp.h"


// Definitions for the CFrameApp class
CFrameApp::CFrameApp()
{
}

CFrameApp::~CFrameApp()
{
}

BOOL CFrameApp::InitInstance()
{
    //Create the Frame Window
    m_Frame.Create();   // throws a CWinException on failure  

    return TRUE;
}


