//////////////////////////////////////////////
// ContainerApp.cpp


#include "stdafx.h"
#include "ContainerApp.h"


// Definitions for the CDockContainerApp class
CDockContainerApp::CDockContainerApp()
{
}

CDockContainerApp::~CDockContainerApp()
{
}

BOOL CDockContainerApp::InitInstance()
{
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}


