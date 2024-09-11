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
int CViewRect::OnCreate(CREATESTRUCT&)
{
    SetTimer(1, 250, nullptr) ;
    return 0;
}

// Called when the window is destroyed.
void CViewRect::OnDestroy()
{
    KillTimer(1);
}

void CViewRect::OnDraw(CDC& dc)
{
    for (const RectData& rd : m_rects)
    {
        dc.CreateSolidBrush(rd.color);
        CRect rc = rd.rect;
        dc.Rectangle(rc.left, rc.top, rc.right, rc.bottom);
    }
}

// Respond to a mouse click on the window.
LRESULT CViewRect::OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam)
{
    SetFocus();
    return FinalWindowProc(msg, wparam, lparam);
}

// Called when the window is resized.
LRESULT CViewRect::OnSize(UINT, WPARAM wparam, LPARAM lparam)
{
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
LRESULT CViewRect::OnTimer(UINT, WPARAM, LPARAM)
{
    if (m_rects.size() < 100)
    {
        int red, green, blue;
        int left, right, top, bottom;
        left = rand() % m_cxClientMax;
        right = rand() % m_cxClientMax;
        top = rand() % m_cyClientMax;
        bottom = rand() % m_cyClientMax;
        red = rand() & 255;
        green = rand() & 255;
        blue = rand() & 255;

        CClientDC RectDC(*this);
        COLORREF color(RGB(red, green, blue));
        RectDC.CreateSolidBrush(color);

        int rcLeft = (left < right) ? left : right;
        int rcTop = (top < bottom) ? top : bottom;
        int rcRight = (left > right) ? left : right;
        int rcBottom = (top > bottom) ? top : bottom;
        RectDC.Rectangle(rcLeft, rcTop, rcRight, rcBottom);

        RectData rectData(color, CRect(rcLeft, rcTop, rcRight, rcBottom));
        m_rects.push_back(rectData);
    }
    else
        KillTimer(1);

    return 0;
}

// Set the CREATESTURCT parameters before the window is created.
void CViewRect::PreCreate(CREATESTRUCT& cs)
{
    // Call base class to set defaults.
    CWnd::PreCreate(cs);
    cs.style |= WS_CLIPCHILDREN;

    // Add the WS_EX_COMPOSITED to reduce flicker.
    if (GetWinVersion() >= 3000)  // Windows 10 or later.
        cs.dwExStyle |= WS_EX_COMPOSITED;
}

// Processes the window's window messages.
LRESULT CViewRect::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_MOUSEACTIVATE:  return OnMouseActivate(msg, wparam, lparam);
        case WM_SIZE:           return OnSize(msg, wparam, lparam);
        case WM_TIMER:          return OnTimer(msg, wparam, lparam);
        }

        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    }

    return 0;
}


