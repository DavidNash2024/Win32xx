/////////////////////////////
// MyStatusBar.cpp
//

#include "stdafx.h"
#include "MyStatusBar.h"

/////////////////////////////////////
// CMyStatusBar function definitions.
//

// This function is called when the status bar is created and
// a HWND is attached to this CWnd.
void CMyStatusBar::OnAttach()
{
    // Start a timer for the progress bar
    SetTimer(101, 100, 0);

    // Create the StatusBar parts
    SetStatusParts();

    // Set the background color (ignored if XP themes are enabled)
    SendMessage(SB_SETBKCOLOR, 0, RGB(125, 230, 255));

    // Create the Hyperlink
    m_hyperlink.Create(*this);
    CRect rc = GetClientRect();
    rc.DeflateRect(3, 3, 3, 3);
    rc.right = 100;
    m_hyperlink.SetWindowPos(0, rc, SWP_SHOWWINDOW);

    // Create the ProgressBar
    m_progressBar.Create(*this);
    CRect partRect = GetPartRect(1);
    m_progressBar.SetWindowPos(0, partRect, SWP_SHOWWINDOW);

    // Add the icon
    SetPartIcon(2, (HICON)GetApp()->LoadImage(MAKEINTRESOURCE(IDW_MAIN), IMAGE_ICON, 16, 16));

    // Set a Part as owner drawn. The part's drawing is performed in CMainFrame::OnDrawItem.
    SetPartText(3, _T(""), SBT_OWNERDRAW);
}

// Perform the owner draw for Part 3 in the status bar
LRESULT CMyStatusBar::OnDrawItem(UINT, WPARAM, LPARAM lparam)
{
    LPDRAWITEMSTRUCT pDrawItem = (LPDRAWITEMSTRUCT)lparam;

    if (!IsWindow()) return 0;

    CRect partRect = pDrawItem->rcItem;
    CDC dc(pDrawItem->hDC);
    CDC memDC(dc);

    // Set the font to italic
    CFont Font = GetFont();
    LOGFONT lf = Font.GetLogFont();
    lf.lfItalic = TRUE;

    // Use a memory DC for flicker free drawing.
    memDC.CreateFontIndirect(lf);
    // Display the gradient background and text
    memDC.GradientFill(RGB(230, 180, 0), RGB(240, 210, 90), partRect, TRUE);
    memDC.SetTextColor(RGB(10, 20, 250));
    memDC.SetBkMode(TRANSPARENT);
    memDC.TextOut(partRect.left, partRect.top, _T("Owner Draw"), 10);
    dc.BitBlt(0, 0, partRect.Width(), partRect.Height(), memDC, 0, 0, SRCCOPY);

    return TRUE;
}

BOOL CMyStatusBar::OnEraseBkgnd(CDC& dc)
{
    // The background can be set here if XP themes are enabled
    if (IsXPThemed())
    {
        // Exclude the progress bar window from the update region.
        CRect rc = m_progressBar.GetWindowRect();
        ScreenToClient(rc);
        dc.ExcludeClipRect(rc);

        // Fill the background with a color gradient
        rc = GetClientRect();
        CMemDC memDC(dc);
        memDC.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
        memDC.GradientFill(RGB(125, 230, 255), RGB(250, 150, 150), GetClientRect(), TRUE);
        dc.BitBlt(0, 0, rc.Width(), rc.Height(), memDC, 0, 0, SRCCOPY);

        return TRUE;
    }

    return FALSE;
}

// Called when a message is reflected back from the parent window.
LRESULT CMyStatusBar::OnMessageReflect(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_DRAWITEM:   return OnDrawItem(msg, wparam, lparam);
    }

    return FinalWindowProc(msg, wparam, lparam);
}

// Called before the window is created to set some window creation parameters.
void CMyStatusBar::PreCreate(CREATESTRUCT& cs)
{
    cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CCS_BOTTOM;

    // Suppress the gripper unless XPThemes are enabled.
    if (IsXPThemed())
        cs.style |= SBARS_SIZEGRIP;
}

// Creates several panes within the status bar.
void CMyStatusBar::SetStatusParts()
{
    // Retrieve the text size for the hyperlink.
    CString str;
    str.GetWindowText(m_hyperlink);

    CDC dc = GetWindowDC();
    CSize sz = dc.GetTextExtentPoint32(str);

    // Create the StatusBar parts.
    CRect clientRect = GetClientRect();
    int parts = IsXPThemed()?  236 : 216;
    int width = MAX(sz.cx + parts, clientRect.right);

    SetPartWidth(0, width - parts); // Win3++ hyperlink
    SetPartWidth(1, 100);         // Progress bar
    SetPartWidth(2, 26);          // Icon
    SetPartWidth(3, 90);          // Owner Drawn text

    if (IsXPThemed())
        SetPartWidth(4, 20);      // Gripper

    // Set the hyperlink window size and position.
    CRect rc = GetPartRect(0);
    rc.right = sz.cx;
    if (m_hyperlink.IsWindow())
        m_hyperlink.SetWindowPos(0, rc, SWP_SHOWWINDOW);
}

// Called when the status bar is resized.
void CMyStatusBar::OnSize()
{
    // Reposition the StatusBar parts.
    SetStatusParts();

    // Reposition the Progress Bar.
    CRect partRect = GetPartRect(1);
    m_progressBar.SetWindowPos(0, partRect, SWP_SHOWWINDOW);
}

// Process the timer message.
void CMyStatusBar::OnTimer()
{
    // Change the Progress Bar indication.
    m_progressBar.OffsetPos(1);
    if (m_progressBar.GetRange(FALSE) == m_progressBar.GetPos())
        m_progressBar.SetPos(0);
}

// Process the statusbar's messages
LRESULT CMyStatusBar::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_TIMER:    OnTimer();     break;
        case WM_SIZE:     OnSize();      break;
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
