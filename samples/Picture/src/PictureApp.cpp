/////////////////////////////
//PictureApp.cpp
//

#include "stdafx.h"
#include "PictureApp.h"

/////////////////////////////////
// CFrameApp function definitions
//

// Constructor.
CPictureApp::CPictureApp()
{
}

// Destructor.
CPictureApp::~CPictureApp()
{
}

// Called when the application starts.
BOOL CPictureApp::InitInstance()
{
    // Create the frame window.
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}

// Called when the message queue is empty.
BOOL CPictureApp::OnIdle(LONG)
{
    m_frame.UpdateToolbar();

    return FALSE;
}

