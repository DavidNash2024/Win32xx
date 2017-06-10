//////////////////////////////////////
// DockingApp.cpp


#include "stdafx.h"
#include "DockingApp.h"


CDockingApp::CDockingApp()
{
}

BOOL CDockingApp::InitInstance()
{
    //Create the Window
	m_MainFrame.Create();	// throws a CWinException on failure

	return TRUE;
}


