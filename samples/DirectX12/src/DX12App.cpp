/////////////////////////////
// DX12App.cpp
//


#include "DX12App.h"


////////////////////////////////
// CDXView function definitions.
//

BOOL CDirectX12App::InitInstance()
{
    m_view.Create();
    return TRUE;
}

int CDirectX12App::MessageLoop()
{
    bool running = true;

    while (running)
    {
        // Process standard Windows messages.
        MSG msg = {};
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                running = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Break early if the window was closed to avoid rendering to an invalid surface.
        if (!running) break;

        m_view.Render();
    }

    return 0;
}