//////////////////////////////////////
// MDIFrameApp.cpp

#include "stdafx.h"
#include "MDIFrameApp.h"


CMDIFrameApp::CMDIFrameApp()
{
}

BOOL CMDIFrameApp::InitInstance()
{
    //Create the Window
	m_MainMDIFrame.Create();	// throws a CWinException on failure  

	return TRUE;
}


