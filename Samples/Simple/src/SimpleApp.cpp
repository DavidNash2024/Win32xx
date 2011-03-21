////////////////////////////////////////
// SimpleApp.cpp

#include "SimpleApp.h"


// Definitions for the CSimpleApp class
CSimpleApp::CSimpleApp()
{
    // Constructor for CSimpleApp
}

BOOL CSimpleApp::InitInstance()
{
    // This function is called automatically when the application starts

    // Create the Window
    m_View.Create();

	return TRUE;
}
