//////////////////////////////////
// ScribbleApp.cpp

#include "stdafx.h"
#include "ScribbleApp.h"


CScribbleApp::CScribbleApp()
{
}

BOOL CScribbleApp::InitInstance()
{
    // Create the window.
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}
