//////////////////////////////////////////////
//FrameApp.cpp


#include "FrameApp.h"


// Definitions for the CFrameApp class
CFrameApp::CFrameApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
}

CFrameApp::~CFrameApp()
{
}

BOOL CFrameApp::InitInstance()
{
	//Start Tracing
	TRACE(_T("Tracing Started"));

	//Create the Frame Window
	m_Frame.Create();

	//End the program if the frame window creation fails
	if (!m_Frame.GetHwnd())
	{
		::MessageBox(NULL, _T("Failed to create Frame window"), _T("ERROR"), MB_ICONERROR);

		// returning FALSE ends the application
		return FALSE;
	}

	return TRUE;
}