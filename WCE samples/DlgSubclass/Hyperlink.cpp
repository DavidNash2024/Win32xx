/////////////////////////////
// Hyperlink.cpp

#include "Hyperlink.h"

CHyperlink::CHyperlink() : m_bUrlVisited(FALSE), m_bClicked(FALSE), m_crVisited(RGB(128, 0, 128)),
                            m_crNotVisited(RGB(0,0,255))
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

BOOL CHyperlink::AttachDlgItem(UINT nID, CWnd& Parent)
{
    BOOL bSuccess = CWnd::AttachDlgItem(nID, Parent);;

    LOGFONT lf = GetFont().GetLogFont();
    lf.lfUnderline = TRUE;
    m_UrlFont.CreateFontIndirect(&lf);

    return bSuccess;
}

void CHyperlink::OnLButtonDown()
{
    SetCapture();
    m_bClicked = TRUE;
}

void CHyperlink::OnLButtonUp(LPARAM lParam)
{
    ReleaseCapture();
    if(m_bClicked)
    {
        m_bClicked = FALSE;
        POINT pt;
        RECT rc;
        pt.x = (short)LOWORD(lParam);
        pt.y = (short)HIWORD(lParam);
        ClientToScreen(pt);
        rc = GetWindowRect();

        if(PtInRect(&rc, pt))
            OpenUrl();
    }
}

void CHyperlink::OpenUrl()
{
    SHELLEXECUTEINFO ExecInfo = {0};;
    ExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ExecInfo.lpVerb = TEXT("open");
    ExecInfo.lpFile = TEXT("iexplore.exe");
    ExecInfo.lpParameters = GetWindowText();

    if (ShellExecuteEx(&ExecInfo))

    {
        m_bUrlVisited = TRUE;

        // redraw the window to update the color
        Invalidate();
    }
    else
        TRACE("ShellExecuteEx Failed");
}

LRESULT CHyperlink::OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Messages such as WM_CTLCOLORSTATIC are reflected back to the CWnd object that created them.
    if (uMsg ==  WM_CTLCOLORSTATIC)
    {
        CDC dc((HDC)wParam);

        dc.SetTextColor(m_bUrlVisited? m_crVisited : m_crNotVisited);
        dc.SetBkMode(TRANSPARENT);
        dc.SelectObject(m_UrlFont);
        dc.Detach();
        return (LRESULT)::GetSysColorBrush(COLOR_BTNFACE);
    }
    return 0L;
}

LRESULT CHyperlink::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_LBUTTONDOWN:
        OnLButtonDown();
        break;

    case WM_LBUTTONUP:
        OnLButtonUp(lParam);
        break;

    case WM_SETCURSOR:
        ::SetCursor(m_hCursor);
        return 1L;

    }

    // Pass unhandled messages on for default processing
    return WndProcDefault( uMsg, wParam, lParam );
}


