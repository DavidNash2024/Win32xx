//////////////////////////////////////
// ScribbleApp.cpp

#include "ScribbleApp.h"

// Constructor.
CScribbleApp::CScribbleApp()
{
}

// Called when the application starts.
BOOL CScribbleApp::InitInstance()
{
    //Create the Window
    m_frame.Create();

    return TRUE;
}
