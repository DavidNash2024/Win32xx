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
	m_Frame.Create();

	//End the program if the frame window creation fails
	if (!m_Frame.GetHwnd())
	{
		::MessageBox(NULL, _T("Failed to create Frame window"), _T("ERROR"), MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}
