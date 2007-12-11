//////////////////////////////////////////////
//FrameApp.cpp

#include "ThemesApp.h"


// Definitions for the CFrameApp class
CThemesApp::CThemesApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
}

CThemesApp::~CThemesApp()
{
}

BOOL CThemesApp::InitInstance()
{
	//Start Tracing
	TRACE(_T("Tracing Started"));

	//Create the Frame Window
	if (!m_Frame.Create())
	{
		// We get here if the Frame creation fails

		::MessageBox(NULL, _T("Failed to create Frame window"), _T("ERROR"), MB_ICONERROR);
		return FALSE; // returning FALSE ends the application
	}

	return TRUE;
}
