//////////////////////////////////////////////
//FrameApp.cpp

#include "stdafx.h"
#include "ReBarApp.h"


// Definitions for the CReBarApp class
CReBarApp::CReBarApp()
{
}

CReBarApp::~CReBarApp()
{
}

BOOL CReBarApp::InitInstance()
{
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure    

    return TRUE;
}


