//////////////////////////////////////////////
// FastGDIApp.cpp

#include "stdafx.h"
#include "FastGDIApp.h"

///////////////////////////////////
// CFastGDIApp function definitions
//

// Constructor.
CFastGDIApp::CFastGDIApp()
{
}

// Destructor.
CFastGDIApp::~CFastGDIApp()
{
}

// Called when the application starts.
BOOL CFastGDIApp::InitInstance()
{
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}

