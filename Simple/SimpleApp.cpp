////////////////////////////////////////
// SimpleApp.h

#include "SimpleApp.h"


// Definitions for the CSimpleApp class
CSimpleApp::CSimpleApp()
{
}

BOOL CSimpleApp::InitInstance()
{
    //Create the Window
    m_View.Create();

	return TRUE;
}
