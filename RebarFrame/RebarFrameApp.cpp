///////////////////////////////////////////////
//RebarFrameApp.cpp
// Definitions for the CRebarFrameApp class

#include "resource.h"
#include "RebarFrameApp.h"


CRebarFrameApp::CRebarFrameApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
	//Start Tracing
	TRACE(TEXT("Tracing started"));

	//Create the Frame Window
	m_Frame.Create();

	//End the program if the frame window creation fails
	if (!m_Frame.GetHwnd())
	{
		::MessageBox(NULL, TEXT("Failed to create Frame window"), TEXT("ERROR"), MB_ICONERROR);
		::PostQuitMessage(0);
	}
}
