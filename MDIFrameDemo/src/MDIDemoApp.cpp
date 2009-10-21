//////////////////////////////////////
// MDIDemoApp.h

#include "stdafx.h"
#include "MDIDemoApp.h"
#include "resource.h"


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


