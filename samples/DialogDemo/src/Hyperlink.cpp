/////////////////////////////
// Hyperlink.cpp
//

#include "stdafx.h"
#include "Hyperlink.h"

//////////////////////////////////
// CHyperlink function definitions
//

// Constructor.
CHyperlink::CHyperlink() : m_isUrlVisited(FALSE), m_isClicked(FALSE), m_crVisited(RGB(128, 0, 128)),
                            m_crNotVisited(RGB(0,0,255))
{
    // Create the cursor
    m_hCursor = ::LoadCursor(NULL, IDC_HAND);

    // IDC_HAND is not available on Win95, so load a reasonable alternative
    if( !m_hCursor )
        m_hCursor = ::LoadCursor(NULL, IDC_ARROW);
}

// Destructor.
CHyperlink::~CHyperlink()
{
}

// Called when the window handle (HWND) is attached to CHyperlink.
void CHyperlink::OnAttach()
{
    CFont Font = GetFont();
    LOGFONT lf = Font.GetLogFont();
    lf.lfUnderline = TRUE;
    m_urlFont.CreateFontIndirect(lf);
}

// Called when the left mouse button is clicked.
void CHyperlink::OnLButtonDown()
{
    SetCapture();
    m_isClicked = TRUE;
}

// Called when the left mouse button is released.
void CHyperlink::OnLButtonUp(LPARAM lparam)
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
LRESULT CHyperlink::OnMessageReflect(UINT msg, WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    // Messages such as WM_CTLCOLORSTATIC are reflected back to the CWnd object that created them.
    if (msg ==  WM_CTLCOLORSTATIC)
    {
        CDC dc(reinterpret_cast<HDC>(wparam));
        dc.SetTextColor( m_isUrlVisited? m_crVisited : m_crNotVisited);
        dc.SetBkMode(TRANSPARENT);
        dc.SelectObject(m_urlFont);
        return (LRESULT)GetSysColorBrush(COLOR_BTNFACE);
    }
    return 0;
}

// Modifies the cursor when its over the hyperlink control.
LRESULT CHyperlink::OnSetCursor()
{
    // Must use ::SetCursor here. CStatic::SetCursor does not do the same thing.
    ::SetCursor(m_hCursor);

    return 1;  // Non-zero return prevents default processing
}

// Process the hyperlink control's window messages.
LRESULT CHyperlink::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_LBUTTONDOWN:  OnLButtonDown();      break;
    case WM_LBUTTONUP:    OnLButtonUp(lparam);  break;
    case WM_SETCURSOR:    return OnSetCursor();
    case WM_NCHITTEST:    return HTCLIENT;      // Claim that the mouse is in a client area
    }

    return WndProcDefault(msg, wparam, lparam);
}


