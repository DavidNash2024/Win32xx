/////////////////////////////
// Hyperlink.cpp


#include "Hyperlink.h"

CHyperlink::CHyperlink() : m_isUrlVisited(FALSE), m_isClicked(FALSE), m_visitedColor(RGB(128, 0, 128)),
                            m_notVisitedColor(RGB(0,0,255))
{
    // Create the cursor
    m_hCursor = ::LoadCursor(NULL, IDC_HAND);

    // IDC_HAND is not available on Win95, so load a reasonable alternative
    if( !m_hCursor )
        m_hCursor = ::LoadCursor(NULL, IDC_ARROW);
}

CHyperlink::~CHyperlink()
{
}

BOOL CHyperlink::AttachDlgItem(UINT id, CWnd& Parent)
{
    BOOL result = CWnd::AttachDlgItem(id, Parent);;

    LOGFONT lf = GetFont().GetLogFont();
    lf.lfUnderline = TRUE;
    m_urlFont.CreateFontIndirect(lf);

    return result;
}

void CHyperlink::OnLButtonDown()
{
    SetCapture();
    m_isClicked = TRUE;
}

void CHyperlink::OnLButtonUp(LPARAM lparam)
{
    ReleaseCapture();
    if(m_isClicked)
    {
        m_isClicked = FALSE;
        POINT pt;
        RECT rc;
        pt.x = (short)LOWORD(lparam);
        pt.y = (short)HIWORD(lparam);
        ClientToScreen(pt);
        rc = GetWindowRect();

        if(PtInRect(&rc, pt))
            OpenUrl();
    }
}

void CHyperlink::OpenUrl()
{
    SHELLEXECUTEINFO execInfo = {0};;
    execInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    execInfo.lpVerb = TEXT("open");
    execInfo.lpFile = TEXT("iexplore.exe");
    execInfo.lpParameters = GetWindowText();

    if (ShellExecuteEx(&execInfo))

    {
        m_isUrlVisited = TRUE;

        // redraw the window to update the color
        Invalidate();
    }
    else
        TRACE("ShellExecuteEx Failed");
}

LRESULT CHyperlink::OnMessageReflect(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Messages such as WM_CTLCOLORSTATIC are reflected back to the CWnd object that created them.
    if (msg ==  WM_CTLCOLORSTATIC)
    {
        CDC dc((HDC)wparam);

        dc.SetTextColor(m_isUrlVisited? m_visitedColor : m_notVisitedColor);
        dc.SetBkMode(TRANSPARENT);
        dc.SelectObject(m_urlFont);
        dc.Detach();
        return (LRESULT)::GetSysColorBrush(COLOR_BTNFACE);
    }
    return 0;
}

LRESULT CHyperlink::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_LBUTTONDOWN:
        OnLButtonDown();
        break;

    case WM_LBUTTONUP:
        OnLButtonUp(lparam);
        break;

    case WM_SETCURSOR:
        ::SetCursor(m_hCursor);
        return 1;

    }

    // Pass unhandled messages on for default processing
    return WndProcDefault( msg, wparam, lparam );
}


