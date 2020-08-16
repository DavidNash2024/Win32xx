/////////////////////////////
// SplitterApp.cpp
//

#include "stdafx.h"
#include "SplitterApp.h"

/////////////////////////////////////
// CSplitterApp function definitions.
//
CSplitterApp::CSplitterApp()
{
}

BOOL CSplitterApp::InitInstance()
{
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}



