///////////////////////////////////////////////////////////////
//SuperclassApp.cpp -  Definitions for the CSuperclassApp class


#include "SuperclassApp.h"


CSuperclassApp::CSuperclassApp(HINSTANCE hInstance) : CWinApp(hInstance)
{
	//Start Tracing
	TRACE("Tracing Started");
	TRACE("Superclassing occurs\nbefore the window is created.");
	TRACE("\nAs a result our window procedure recieves\nWM_NCCREATE and WM_CREATE messages.\n");
	//Create the Outer Window
	m_Win.Create();
}

CSuperclassApp::~CSuperclassApp()
{
}
