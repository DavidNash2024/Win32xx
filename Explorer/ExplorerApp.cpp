///////////////////////////////////////////////
//ExplorerApp.cpp -  Definitions for the CExplorerApp class

#include "resource.h"
#include "ExplorerApp.h"


CExplorerApp::CExplorerApp()
{
}

CExplorerApp::~CExplorerApp()
{
}

BOOL CExplorerApp::InitInstance()
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

