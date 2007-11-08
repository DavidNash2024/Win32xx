//////////////////////////////////////
// MDIFrameApp.cpp


#include "resource.h"
#include "MDIFrameApp.h"


CMDIFrameApp::CMDIFrameApp(HINSTANCE hInstance) : CMDIApp(hInstance)
{
}

BOOL CMDIFrameApp::InitInstance()
{
	//Start Tracing
	TRACE("Start Tracing");

    //Create the Window
	if (!m_MainMDIFrame.Create())
		return FALSE;	// End the application if the window creation fails

	return TRUE;
}


