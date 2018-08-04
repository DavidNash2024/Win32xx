//////////////////////////////////////
// MDIDemoApp.h

#include "stdafx.h"
#include "MDIDemoApp.h"
#include "resource.h"


CMDIDemoApp::CMDIDemoApp()
{
}

BOOL CMDIDemoApp::InitInstance()
{
    //Create the Window
    m_mainMDIFrame.Create();    // throws a CWinException on failure  

    return TRUE;
}


