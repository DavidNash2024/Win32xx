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
	if (!m_Frame.Create())
	{
		// We get here if the Frame creation fails

		::MessageBox(NULL, _T("Failed to create Frame window"), _T("ERROR"), MB_ICONERROR);
		return FALSE; // returning FALSE ends the application
	}

	return TRUE;
}

