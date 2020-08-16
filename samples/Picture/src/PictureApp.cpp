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
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}
