//////////////////////////////////////////////
//FrameApp.cpp

#include "stdafx.h"
#include "DoubleBufferApp.h"


// Definitions for the CDoubleBufferApp class
CDoubleBufferApp::CDoubleBufferApp()
{
}

CDoubleBufferApp::~CDoubleBufferApp()
{
}

BOOL CDoubleBufferApp::InitInstance()
{
    //Create the Frame Window
    m_Frame.Create();   // throws a CWinException on failure    

    return TRUE;
}


