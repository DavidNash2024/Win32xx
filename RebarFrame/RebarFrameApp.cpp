///////////////////////////////////////////////
//RebarFrameApp.cpp
// Definitions for the CRebarFrameApp class

#include "resource.h"
#include "RebarFrameApp.h"


CRebarFrameApp::CRebarFrameApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
	//Start Tracing
	TRACE(_T("Tracing started"));

	//Create the Frame Window
	m_Frame.Create();

	//End the program if the frame window creation fails
	if (!m_Frame.GetHwnd())
	{
		::MessageBox(NULL, _T("Failed to create Frame window"), _T("ERROR"), MB_ICONERROR);
		::PostQuitMessage(0);
	}
}
