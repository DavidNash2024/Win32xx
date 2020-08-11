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

// Overrides the application's default message loop.
// Calls Render during idle processing
int CDXApp::MessageLoop()
{
    // Main message loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            m_dxView.Render();
        }
    }

    return LOWORD(msg.wParam);
}

