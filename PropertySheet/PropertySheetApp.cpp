//////////////////////////////////////////////
//PropertySheetApp.cpp

#include "PropertySheetApp.h"


// Definitions for the CFrameApp class
CPropertySheetApp::CPropertySheetApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
}

CPropertySheetApp::~CPropertySheetApp()
{
}

BOOL CPropertySheetApp::InitInstance()
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

