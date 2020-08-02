//////////////////////////////////////////////
// FastGDIApp.cpp

#include "stdafx.h"
#include "FastGDIApp.h"


// Definitions for the CFastGDIApp class
CFastGDIApp::CFastGDIApp()
{
}

CFastGDIApp::~CFastGDIApp()
{
}

BOOL CFastGDIApp::InitInstance()
{
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}


