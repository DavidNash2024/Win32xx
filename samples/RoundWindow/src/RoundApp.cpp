////////////////////////////////////////
// SimpleApp.cpp

#include "RoundApp.h"


// Definitions for the CRoundApp class
CRoundApp::CRoundApp()
{
    // Constructor for CRoundApp
}

BOOL CRoundApp::InitInstance()
{
    // This function is called automatically when the application starts

    // Create the Window
    m_View.Create();	// throws a CWinException on failure  

	return TRUE;
}
