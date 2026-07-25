/////////////////////////////
// DX12App.cpp
//


#include "DX12App.h"


////////////////////////////////
// CDXView function definitions.
//

// InitInstance is called when the application starts.
// Return TRUE to indicate success and run the message loop.
BOOL CDirectX12App::InitInstance()
{
    m_view.Create();
    return TRUE;
}

// OnIdle is called when the message queue is empty.
// Return TRUE to continue idle processing or FALSE to end idle processing
// until another message is queued.
BOOL CDirectX12App::OnIdle(LONG)
{
    m_view.Render();
    return TRUE;
}

