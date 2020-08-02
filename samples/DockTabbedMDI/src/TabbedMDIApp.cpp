//////////////////////////////////////////////
// TabbedMDIApp.cpp

#include "stdafx.h"
#include "TabbedMDIApp.h"


// Definitions for the CDockContainerApp class
CTabbedMDIApp::CTabbedMDIApp()
{
}

CTabbedMDIApp::~CTabbedMDIApp()
{
}

BOOL CTabbedMDIApp::InitInstance()
{
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}


