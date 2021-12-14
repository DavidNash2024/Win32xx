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
