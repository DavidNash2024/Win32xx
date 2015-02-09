//////////////////////////////////////////////
//FrameApp.cpp

#include "stdafx.h"
#include "StatusBarApp.h"


// Definitions for the CStatusBarApp class
CStatusBarApp::CStatusBarApp()
{
}

CStatusBarApp::~CStatusBarApp()
{
}

BOOL CStatusBarApp::InitInstance()
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


