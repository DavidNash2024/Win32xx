/////////////////////////////
// Rect.cpp
//

#include "stdafx.h"
#include "Rect.h"
#include "resource.h"


/////////////////////////////////
// CViewRect function definitions
//

// Called when the window is created.
int CViewRect::OnCreate(CREATESTRUCT& cs)
{
    UNREFERENCED_PARAMETER(cs);

    SetTimer (1, 250, NULL) ;
    return 0;
}

// Called when the window is destroyed.
void CViewRect::OnDestroy()
{
    KillTimer(1);
}

// Respond to a mouse click on the window.
LRESULT CViewRect::OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam)
{
    SetFocus();
    return FinalWindowProc(msg, wparam, lparam);
}

// Called when the window is resized.
LRESULT CViewRect::OnSize(UINT msg, WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(msg);

    // If not minimized, save the window size.
    if (wparam != SIZE_MINIMIZED)
    {
        m_cxClientMax = LOWORD (lparam) ;
        if (m_cxClientMax < 1)
            m_cxClientMax = 1;

        m_cyClientMax = HIWORD (lparam) ;
        if (m_cyClientMax < 1)
            m_cyClientMax = 1;
    }

    return 0;
}

// Responds to the timer event. Displays a random rectangle.
LRESULT CViewRect::OnTimer(UINT msg, WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(wparam);
    UNREFERENCED_PARAMETER(lparam);

    int red, green, blue;
    int left, right, top, bottom;
    left   = rand () % m_cxClientMax;
    right  = rand () % m_cxClientMax;
    top    = rand () % m_cyClientMax;
    bottom = rand () % m_cyClientMax;
    red    = rand () & 255;
    green  = rand () & 255;
    blue   = rand () & 255;

    CClientDC RectDC(*this);
    RectDC.CreateSolidBrush (RGB (red, green, blue));

    int rcLeft   = (left < right) ? left : right;
    int rcTop    = (top < bottom) ? top  : bottom;
    int rcRight  = (left > right) ? left : right;
    int rcBottom = (top > bottom) ? top  : bottom;
    RectDC.Rectangle(rcLeft, rcTop, rcRight, rcBottom);

    return 0;
}

// Processes the window's window messages.
LRESULT CViewRect::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_MOUSEACTIVATE:  return OnMouseActivate(msg, wparam, lparam);
    case WM_SIZE:           return OnSize(msg, wparam, lparam);
    case WM_TIMER:          return OnTimer(msg, wparam, lparam);
    }

    return WndProcDefault(msg, wparam, lparam);
}

