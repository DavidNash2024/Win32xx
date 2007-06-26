//////////////////////////////////////////////
//FrameApp.cpp

#define WINVER 0x0400
#include "FrameApp.h"


// Definitions for the CFrameApp class
CFrameApp::CFrameApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
	//Start Tracing
	TRACE("Tracing Started");

	//Create the Frame Window
	m_Frame.Create();

	//End the program if the frame window creation fails
	if (!m_Frame.GetHwnd())
	{
		::MessageBox(NULL, _T("Failed to create Frame window"), _T("ERROR"), MB_ICONERROR);
		::PostQuitMessage(0);
	}
}

CFrameApp::~CFrameApp()
{
}
