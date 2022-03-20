

#include "stdafx.h"
#include "PrintParent.h"

////////////////////////////////////
// CPrintParent function definitions
//

LRESULT CPrintParent::OnActivate(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // This message is sent when the preference dialog is closed,
    // and this window is activated.
    if (LOWORD(wparam) != WA_INACTIVE)
    {
        // Post a settings change message to the RichView.
        ::PostMessage(m_owner, UWM_SETTINGCHANGE, 0, 0);
    }

    return WndProcDefault(msg, wparam, lparam);
}

LRESULT CPrintParent::OnWindowPosChanged(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Sent when a different property page is selected.
    // Required for Windows 2000.
    ::PostMessage(m_owner, UWM_SETTINGCHANGE, 0, 0);
    return WndProcDefault(msg, wparam, lparam);
}

// The window procedure handles the window's messages.
LRESULT CPrintParent::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_ACTIVATE:          return OnActivate(msg, wparam, lparam);
    case WM_WINDOWPOSCHANGED:  return OnWindowPosChanged(msg, wparam, lparam);
    }

    return WndProcDefault(msg, wparam, lparam);
}