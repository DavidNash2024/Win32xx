///////////////////////////////////////////////////////////
//SplitterApp.cpp -  Definitions for the CSplitterApp class

#include "stdafx.h"
#include "SplitterApp.h"
#include "resource.h"


CSplitterApp::CSplitterApp()
{
}

BOOL CSplitterApp::InitInstance()
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



