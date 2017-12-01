//////////////////////////////////////////////
//TabDemoApp.cpp

#include "stdafx.h"
#include "TabDemoApp.h"


// Definitions for the CTabDemoApp class
CTabDemoApp::CTabDemoApp()
{
}

CTabDemoApp::~CTabDemoApp()
{
}

BOOL CTabDemoApp::InitInstance()
{
    //Create the Frame Window
    m_Frame.Create();   // throws a CWinException on failure    

    return TRUE;
}


