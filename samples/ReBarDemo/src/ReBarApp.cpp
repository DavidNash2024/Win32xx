//////////////////////////////////////////////
//FrameApp.cpp

#include "stdafx.h"
#include "ReBarApp.h"


// Definitions for the CReBarApp class
CReBarApp::CReBarApp()
{
}

CReBarApp::~CReBarApp()
{
}

BOOL CReBarApp::InitInstance()
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


