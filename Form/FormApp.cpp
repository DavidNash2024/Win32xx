//////////////////////////////////////////////
//FrameApp.cpp

#include "FormApp.h"


// Definitions for the CFrameApp class
CFormApp::CFormApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
}

CFormApp::~CFormApp()
{
}

BOOL CFormApp::InitInstance()
{
	//Start Tracing
	TRACE("Tracing Started");

	//Create the Frame Window
	m_Frame.Create();

	//End the program if the frame window creation fails
	if (!m_Frame.GetHwnd())
	{
		::MessageBox(NULL, _T("Failed to create Frame window"), _T("ERROR"), MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

