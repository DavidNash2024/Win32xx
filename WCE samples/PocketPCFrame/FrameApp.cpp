/////////////////////////////
// FrameApp.cpp


#include "FrameApp.h"

// Construcror.
CWceFrameApp::CWceFrameApp()
{
}

// Called when the application starts.
BOOL CWceFrameApp::InitInstance()
{
    //Create the Window
    m_frame.Create();

    return TRUE;
}
