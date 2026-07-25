/////////////////////////////
// DXApp.cpp
//

#include "DXApp.h"

//////////////////////////////
// CDXApp function definitions
//

// Called when the application starts.
BOOL CDXApp::InitInstance()
{
    // Creates the DirectX view window
    m_dxView.Create();

    return TRUE;
}

// OnIdle is called when the message queue is empty.
// Return TRUE to continue idle processing or FALSE to end idle processing
// until another message is queued.
BOOL CDXApp::OnIdle(LONG)
{
    m_dxView.Render();
    return TRUE;
}

