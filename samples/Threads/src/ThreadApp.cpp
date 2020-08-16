/////////////////////////////
//ThreadApp.cpp
//

#include "stdafx.h"
#include "ThreadApp.h"

///////////////////////////////////
// CThreadApp function definitions.
//
CThreadApp::CThreadApp()
{
}

CThreadApp::~CThreadApp()
{
}

BOOL CThreadApp::InitInstance()
{
    m_mainWnd.Create(); // throws a CWinException on failure

    return TRUE;
}
