///////////////////////////////////////////////////////////
//SplitterApp.cpp -  Definitions for the CSplitterApp class

#include "stdafx.h"
#include "SplitterApp.h"


CSplitterApp::CSplitterApp()
{
}

BOOL CSplitterApp::InitInstance()
{
    //Create the Frame Window
    m_frame.Create();   // throws a CWinException on failure

    return TRUE;
}



