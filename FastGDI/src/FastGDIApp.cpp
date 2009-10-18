//////////////////////////////////////////////
//FrameApp.cpp


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
	if (!m_Frame.Create())	
	{
		// We get here if the Frame creation fails
		
		::MessageBox(NULL, _T("Failed to create Frame window"), _T("ERROR"), MB_ICONERROR);
		return FALSE; // returning FALSE ends the application
	}

	return TRUE;
}


