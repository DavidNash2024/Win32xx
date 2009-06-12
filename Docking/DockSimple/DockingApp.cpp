//////////////////////////////////////
// MDIFrameApp.cpp


#include "resource.h"
#include "DockingApp.h"


CDockingApp::CDockingApp()
{
}

BOOL CDockingApp::InitInstance()
{
    //Create the Window
	if (!m_MainFrame.Create())
		return FALSE;	// End the application if the window creation fails

	return TRUE;
}


