///////////////////////////////////////////////
//ShellApp.cpp -  Definitions for the CShellApp class

#include "resource.h"
#include "ShellApp.h"


CShellApp::CShellApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
}

CShellApp::~CShellApp()
{
}

BOOL CShellApp::InitInstance()
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