//////////////////////////////////////////////
//FrameApp.cpp

#include "NetworkApp.h"


// Definitions for the CNetworkApp class
CNetworkApp::CNetworkApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
}

CNetworkApp::~CNetworkApp()
{
}

BOOL CNetworkApp::InitInstance()
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


