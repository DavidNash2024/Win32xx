//////////////////////////////////////
// MDIDemoApp.h


#include "resource.h"
#include "MDIDemoApp.h"


CMDIDemoApp::CMDIDemoApp(HINSTANCE hInstance) : CMDIApp(hInstance)
{
}

BOOL CMDIDemoApp::InitInstance()
{
	//Start Tracing
	TRACE("Start Tracing");

    //Create the Window
	if (!m_MainMDIFrame.Create())
		return FALSE;	// End application if create fails

	return TRUE;
}


