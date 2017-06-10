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
	m_Frame.Create();	// throws a CWinException on failure  

	return TRUE;
}

