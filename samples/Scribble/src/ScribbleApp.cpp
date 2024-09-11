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
    m_frame.Create();   // Throws a CWinException on failure.

    return TRUE;
}
