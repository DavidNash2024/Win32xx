/////////////////////////////
// SimpleApp.cpp
//

#include "RoundApp.h"


//////////////////////////////////
// CRoundApp function definitions.
//

// Constructor for CRoundApp
CRoundApp::CRoundApp()
{
}

// This function is called automatically when the application starts.
BOOL CRoundApp::InitInstance()
{
    // Create the Window.
    m_view.Create();    // Throws a CWinException on failure.

    return TRUE;
}
