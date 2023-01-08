///////////////////////////////
// ScribbleApp.cpp

// VS6 requires these macros to be defined.
#define WINVER          0x0410
#define _WIN32_WINDOWS  0x0410
#define _WIN32_IE       0x0401

#include "ScribbleApp.h"

// Constructor.
CScribbleApp::CScribbleApp()
{
}

// Called when the application starts.
BOOL CScribbleApp::InitInstance()
{
    // Create the frame window.
    m_view.Create();

    return TRUE;
}
