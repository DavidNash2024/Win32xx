//////////////////////////////////////
// MDIFrameApp.cpp

#include "stdafx.h"
#include "MDIFrameSplitterApp.h"


CMDIFrameSplitterApp::CMDIFrameSplitterApp()
{
}

BOOL CMDIFrameSplitterApp::InitInstance()
{
    //Create the Window
	if (m_MainMDIFrame.Create() == 0)
		return FALSE;	// End the application if the window creation fails

	return TRUE;
}


