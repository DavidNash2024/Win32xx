/////////////////////////////
// TextApp.cpp
//

#include "stdafx.h"
#include "TextApp.h"
#include "resource.h"

////////////////////////////////
// CTextApp function definitions
//

// Constructor.
CTextApp::CTextApp()
{
}

// Called when the application starts.
BOOL CTextApp::InitInstance()
{
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}

// Called when the message queue is empty.
BOOL CTextApp::OnIdle(LONG)
{
    m_frame.UpdateToolbar();

    return FALSE;
}

