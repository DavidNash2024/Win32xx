///////////////////////////////
// ScribbleApp.cpp

#include "ScribbleApp.h"


// Constructor.
CScribbleApp::CScribbleApp()
{
}

// Called when the application starts.
BOOL CScribbleApp::InitInstance()
{
    // Create the view window.
    m_view.Create();

    return TRUE;
}
