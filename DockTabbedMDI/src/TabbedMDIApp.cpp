//////////////////////////////////////////////
// TabbedMDIApp.cpp


#include "stdafx.h"
#include "TabbedMDIApp.h"


// Definitions for the CContainerApp class
CTabbedMDIApp::CTabbedMDIApp()
{
}

CTabbedMDIApp::~CTabbedMDIApp()
{
}

BOOL CTabbedMDIApp::InitInstance()
{
	//Create the Frame Window
	if (!m_Frame.Create())	
	{
		// We get here if the Frame creation fails
		
		::MessageBox(NULL, _T("Failed to create Frame window"), _T("ERROR"), MB_ICONERROR);
		return FALSE; // returning FALSE ends the application
	}

	return TRUE;
}


