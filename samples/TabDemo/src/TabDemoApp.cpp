//////////////////////////////////////////////
//TabDemoApp.cpp

#include "stdafx.h"
#include "TabDemoApp.h"


// Definitions for the CTabDemoApp class
CTabDemoApp::CTabDemoApp()
{
}

CTabDemoApp::~CTabDemoApp()
{
}

BOOL CTabDemoApp::InitInstance()
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


