/////////////////////////////
// App.cpp
//

#include "stdafx.h"
#include "App.h"



////////////////////////////
// CApp function definitions
//

// Constructor.
CApp::CApp()
{
}

// Destructor.
CApp::~CApp()
{
}

// Called when the application starts.
BOOL CApp::InitInstance()
{
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}

