/////////////////////////////
// SimpleApp.cpp
//

// VS6 requires these macros to be defined.
#define WINVER          0x0410
#define _WIN32_WINDOWS  0x0410
#define _WIN32_IE       0x0401

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
