//////////////////////////////////////
// MDIDemoApp.h


#include "resource.h"
#include "MDIDemoApp.h"


CMDIDemoApp::CMDIDemoApp()
{
}

BOOL CMDIDemoApp::InitInstance()
{
    //Create the Window
	if (!m_MainMDIFrame.Create())
		return FALSE;	// End application if create fails

	return TRUE;
}


