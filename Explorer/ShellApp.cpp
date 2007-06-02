///////////////////////////////////////////////
//ShellApp.cpp -  Definitions for the CShellApp class

#include "resource.h"
#include "ShellApp.h"


CShellApp::CShellApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
	//Start Tracing
	TRACE(TEXT("Tracing Started"));

	//Create the Frame Window
	m_Frame.Create();

	//End the program if the frame window creation fails
	if (!m_Frame.GetHwnd())
	{
		::MessageBox(NULL, TEXT("Failed to create Frame window"), TEXT("ERROR"), MB_ICONERROR);
		::PostQuitMessage(0);
	}
}

CShellApp::~CShellApp()
{
}
