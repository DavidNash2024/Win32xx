//////////////////////////////
// MainWin.Cpp

#include "MainWin.h"


CMainWin::CMainWin()
{
    ::InitCommonControls();
}

int CMainWin::OnCreate(CREATESTRUCT& cs)
{
    UNREFERENCED_PARAMETER(cs);

    m_button.Create(*this);
    return 0;
}

LRESULT CMainWin::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
        case WM_DESTROY:
            // Terminate the application.
            ::PostQuitMessage(0);
            return 0;
    }

    // Pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}
