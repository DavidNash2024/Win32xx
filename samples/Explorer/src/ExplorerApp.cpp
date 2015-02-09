///////////////////////////////////////////////
//ExplorerApp.cpp -  Definitions for the CExplorerApp class

#include "stdafx.h"
#include "ExplorerApp.h"
#include "resource.h"


CExplorerApp::CExplorerApp()
{
}

CExplorerApp::~CExplorerApp()
{
	::CoUninitialize(); // Shut down COM
}

BOOL CExplorerApp::InitInstance()
{
	// Initialise COM
	if (FAILED(CoInitialize(NULL)))
	{
		::MessageBox(NULL, _T("Failed to initialize COM"), _T("ERROR"), MB_ICONERROR);
		return FALSE;
	}
	
	//Create the Frame Window
	if (m_Frame.Create() == 0)
	{
		// We get here if the Frame creation fails

		::MessageBox(NULL, _T("Failed to create Frame window"), _T("ERROR"), MB_ICONERROR);
		return FALSE; // returning FALSE ends the application
	}

	return TRUE;
}

