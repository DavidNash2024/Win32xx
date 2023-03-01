/////////////////////////////
// Hyperlink.cpp
//

#include "stdafx.h"
#include "Hyperlink.h"

//////////////////////////////////
// CHyperlink function definitions
//

// Constructor.
CHyperlink::CHyperlink() : m_isUrlVisited(false), m_isClicked(false), m_crVisited(RGB(128, 0, 128)),
                            m_crNotVisited(RGB(0,0,255))
{
    // Create the cursor
    m_cursor = ::LoadCursor(NULL, IDC_HAND);

    // IDC_HAND is not available on Win95, so load a reasonable alternative
    if( !m_cursor )
        m_cursor = ::LoadCursor(NULL, IDC_ARROW);
}

// Destructor.
CHyperlink::~CHyperlink()
{
}

// Called when the left mouse button is clicked.
LRESULT CHyperlink::OnLButtonDown()
{
    SetCapture();
    m_isClicked = TRUE;
    return 0;
}

// Called when the left mouse button is released.
LRESULT CHyperlink::OnLButtonUp(LPARAM lparam)
{
    ReleaseCapture();
    if(m_isClicked)
    {
        m_isClicked = FALSE;
        CPoint pt;
        pt.x = LOWORD(lparam);
        pt.y = HIWORD(lparam);
        ClientToScreen(pt);
        CRect rc = GetWindowRect();

        if (rc.PtInRect(pt)) OpenUrl();
    }

    return 0;
}

// Opens the default browser and displays the web page.
void CHyperlink::OpenUrl()
{
    LPCTSTR url = _T("http://sourceforge.net/projects/win32-framework/");

    HINSTANCE result = ::ShellExecute(NULL, _T("open"), url, NULL, NULL, SW_SHOWNORMAL);
    if (reinterpret_cast<INT_PTR>(result) > 32)
    {
        m_isUrlVisited = TRUE;

        // redraw the window to update the color
        Invalidate();
    }
}

// Handles messages reflected back from the dialog.
LRESULT CHyperlink::OnMessageReflect(UINT msg, WPARAM wparam, LPARAM)
{
    // Messages such as WM_CTLCOLORSTATIC are reflected back to the CWnd object that created them.
    if (msg ==  WM_CTLCOLORSTATIC)
    {
        CDC dc(reinterpret_cast<HDC>(wparam));
        dc.SetTextColor( m_isUrlVisited? m_crVisited : m_crNotVisited);
        dc.SetBkMode(TRANSPARENT);
        dc.SelectObject(m_urlFont);
        dc.Detach();
        return (LRESULT)GetSysColorBrush(COLOR_BTNFACE);
    }
    return 0;
}

// Modifies the cursor when its over the hyperlink control.
LRESULT CHyperlink::OnSetCursor()
{
    // Must use ::SetCursor here. CStatic::SetCursor does not do the same thing.
    ::SetCursor(m_cursor);

    return 1;  // Non-zero return prevents default processing
}

// Process the hyperlink control's window messages.
LRESULT CHyperlink::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_LBUTTONDOWN:  return OnLButtonDown();
        case WM_LBUTTONUP:    return OnLButtonUp(lparam);
        case WM_SETCURSOR:    return OnSetCursor();
        case WM_NCHITTEST:    return HTCLIENT;      // Claim that the mouse is in a client area
        }

        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}


