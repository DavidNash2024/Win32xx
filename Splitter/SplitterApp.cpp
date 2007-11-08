///////////////////////////////////////////////////////////
//SplitterApp.cpp -  Definitions for the CSplitterApp class


#include "resource.h"
#include "SplitterApp.h"


CSplitterApp::CSplitterApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
}

BOOL CSplitterApp::InitInstance()
{
	//Open the Trace Window
	TRACE(_T("Trace started"));

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



