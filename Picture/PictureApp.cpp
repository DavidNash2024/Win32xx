//////////////////////////////////////////////
//PictureApp.cpp


#include "PictureApp.h"


// Definitions for the CFrameApp class
CPictureApp::CPictureApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
}

CPictureApp::~CPictureApp()
{
}

BOOL CPictureApp::InitInstance()
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
