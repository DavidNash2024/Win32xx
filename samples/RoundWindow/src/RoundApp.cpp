/////////////////////////////
// SimpleApp.cpp
//

#include "RoundApp.h"

//////////////////////////////////
// CRoundApp function definitions.
//
CRoundApp::CRoundApp()
{
    // Constructor for CRoundApp
}

BOOL CRoundApp::InitInstance()
{
    // This function is called automatically when the application starts

    // Create the Window
    m_view.Create();    // throws a CWinException on failure

    return TRUE;
}
