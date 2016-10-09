///////////////////////////////////////////////
// TextApp.cpp -  Definitions for the CTextApp class

#include "stdafx.h"
#include "TextApp.h"
#include "resource.h"


CTextApp::CTextApp()
{
}

BOOL CTextApp::InitInstance()
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

