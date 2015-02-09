//////////////////////////////////////////////
//FrameApp.cpp

#include "stdafx.h"
#include "FrameApp.h"


// Definitions for the CToolBarDemoApp class
CToolBarDemoApp::CToolBarDemoApp()
{
}

CToolBarDemoApp::~CToolBarDemoApp()
{
}

BOOL CToolBarDemoApp::InitInstance()
{
	//Create the Frame Window
	if (m_Frame.Create() == 0)	
	{
		// We get here if the Frame creation fails
		
		::MessageBox(NULL, _T("Failed to create Frame window"), _T("ERROR"), MB_ICONERROR);
		return FALSE; // returning FALSE ends the application
	}

	return TRUE;
}


