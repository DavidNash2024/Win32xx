//////////////////////////////////////////////
//FrameApp.cpp

#include "FormApp.h"


// Definitions for the CFrameApp class
CFormApp::CFormApp()
{
}

CFormApp::~CFormApp()
{
}

BOOL CFormApp::InitInstance()
{
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

