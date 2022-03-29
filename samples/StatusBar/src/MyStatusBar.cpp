/////////////////////////////
// MyStatusBar.cpp
//

#include "stdafx.h"
#include "MyStatusBar.h"
#include "resource.h"

// This code requires the uxtheme.lib  library.


/////////////////////////////////////
// CMyStatusBar function definitions.
//

CMyStatusBar::CMyStatusBar() : m_ownerDraw(_T("Owner Draw"))
{
}

CMyStatusBar::~CMyStatusBar()
{
}

// Adjusts the specified value for the current DPI.
int CMyStatusBar::AdjustForDPI(int value)
{
    CClientDC statusDC(*this);
    statusDC.SelectObject(GetFont());

    // Perform the DPI adjustment calculation.
    int defaultDPI = 96;
    int xDPI = statusDC.GetDeviceCaps(LOGPIXELSX);
    value = MulDiv(value, xDPI, defaultDPI);

    return value;
}

// Retrieves the width of the part required to contain the specified text.
int CMyStatusBar::GetTextPartWidth(LPCTSTR text) const
{
    CClientDC dc = GetDC();
    dc.SelectObject(GetFont());
    CSize textSize = dc.GetTextExtentPoint32(text);
    int width = textSize.cx;
    const int border = 8;
    width += border;

    return width;
}

// This function is called when the status bar is created and
// a HWND is attached to this CWnd.
void CMyStatusBar::OnAttach()
{
    // Start a timer for the progress bar
    const int timerID = 101;
    SetTimer(timerID, 100, 0);

    // Set the background color (ignored if XP themes are enabled)
    COLORREF backgrnd = RGB(125, 230, 255);
    SendMessage(SB_SETBKCOLOR, 0, backgrnd);

    // Create the Hyperlink
    m_hyperlink.Create(*this);

    // Create the ProgressBar
    m_progressBar.Create(*this);

    // Required by owner draw.
    const int statusID = 102;
    SetWindowLongPtr(GWLP_ID, statusID);
}

// Perform the owner drawing for part 4 in the status bar.
LRESULT CMyStatusBar::OnDrawItem(UINT, WPARAM, LPARAM lparam)
{
    // Retrieve the device context.
    LPDRAWITEMSTRUCT pDrawItem = (LPDRAWITEMSTRUCT)lparam;
    CDC dc(pDrawItem->hDC);

    // Set the memory dc's font to italic.
    CFont font = GetFont();
    LOGFONT lf = font.GetLogFont();
    lf.lfItalic = TRUE;
    CDC memDC(dc);
    memDC.CreateFontIndirect(lf);

    // Fill the owner draw gradient background.
    CRect partRect = pDrawItem->rcItem;
    COLORREF fillColor1 = RGB(230, 180, 0);
    COLORREF fillColor2 = RGB(240, 210, 90);
    memDC.GradientFill(fillColor1, fillColor2, partRect, TRUE);

    // Display the owner draw text.
    COLORREF textColor = RGB(10, 20, 250);
    memDC.SetTextColor(textColor);
    memDC.SetBkMode(TRANSPARENT);
    memDC.DrawText(m_ownerDraw, m_ownerDraw.GetLength(), partRect, DT_SINGLELINE | DT_VCENTER);
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
        COLORREF fillColor1 = RGB(125, 230, 255);
        COLORREF fillColor2 = RGB(250, 150, 150);
        memDC.GradientFill(fillColor1, fillColor2, GetClientRect(), TRUE);
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

// Called when the status bar is resized.
void CMyStatusBar::OnSize()
{
    // Create and reposition the status bar parts.
    SetupParts();
}

// Process the timer message.
void CMyStatusBar::OnTimer()
{
    // Change the Progress Bar indication.
    m_progressBar.OffsetPos(1);
    if (m_progressBar.GetRange(FALSE) == m_progressBar.GetPos())
        m_progressBar.SetPos(0);
}

// Creates the status bar parts and updates their widths.
// Positions the child windows over the status bar parts.
void CMyStatusBar::SetupParts()
{
    CString cap = LoadString(IDW_INDICATOR_CAPS);
    CString num = LoadString(IDW_INDICATOR_NUM);
    CString ovr = LoadString(IDW_INDICATOR_OVR);
    CString scrl = LoadString(IDW_INDICATOR_SCRL);
    CString link = m_hyperlink.GetLinkName();

    const int progressWidth = 100;
    const int gripWidth = 20;
    int iconSide = GetClientRect().Height();

    // Fill a vector with the status bar part widths.
    std::vector<int> partWidths;
    partWidths.push_back(GetTextPartWidth(link));
    partWidths.push_back(AdjustForDPI(progressWidth));
    partWidths.push_back(iconSide);
    partWidths.push_back(GetTextPartWidth(m_ownerDraw));
    partWidths.push_back(GetTextPartWidth(cap));
    partWidths.push_back(GetTextPartWidth(num));
    partWidths.push_back(GetTextPartWidth(ovr));
    partWidths.push_back(GetTextPartWidth(scrl));

    int sumWidths = 0;
    std::vector<int>::iterator iter;
    for (iter = partWidths.begin(); iter != partWidths.end(); ++iter)
    {
        sumWidths += *iter;
    }
    sumWidths += AdjustForDPI(gripWidth);

    // Insert the width for the first status bar part into the vector.
    CRect clientRect = GetClientRect();
    const int minWidth = 300;
    int width = MAX(minWidth, clientRect.right);
    std::vector<int>::iterator begin = partWidths.begin();
    partWidths.insert(begin, width - sumWidths);

    // Create or resize the status bar parts.
    for (iter = partWidths.begin(); iter != partWidths.end(); ++iter)
    {
        int part = static_cast<int>(iter - partWidths.begin());
        SetPartWidth(part, *iter);
    }

    // Reposition the hyperlink over part 1.
    CRect partRect = GetPartRect(1);
    const int border = 2;
    partRect.OffsetRect(border, 0);
    m_hyperlink.SetWindowPos(0, partRect, SWP_SHOWWINDOW);

    // Reposition the progress bar over part 2.
    partRect = GetPartRect(2);
    m_progressBar.SetWindowPos(0, partRect, SWP_SHOWWINDOW);

    // Add the icon to part 3.
    iconSide = iconSide - (iconSide % 8);
    SetPartIcon(3, (HICON)GetApp()->LoadImage(MAKEINTRESOURCE(IDW_MAIN), IMAGE_ICON, iconSide, iconSide));

    // Define part 4 as owner drawn.
    SetPartText(4, 0, SBT_OWNERDRAW);
}

// Updates the status indicators.
void CMyStatusBar::SetStatusIndicators()
{
    CString cap = LoadString(IDW_INDICATOR_CAPS);
    CString num = LoadString(IDW_INDICATOR_NUM);
    CString ovr = LoadString(IDW_INDICATOR_OVR);
    CString ins = LoadString(IDW_INDICATOR_INS);
    CString scrl = LoadString(IDW_INDICATOR_SCRL);

    CString status1 = (::GetKeyState(VK_CAPITAL) & 0x0001) ? cap : CString("");
    CString status2 = (::GetKeyState(VK_NUMLOCK) & 0x0001) ? num : CString("");
    CString status3 = (::GetKeyState(VK_INSERT)  & 0x0001) ? ovr : ins;
    CString status4 = (::GetKeyState(VK_SCROLL)  & 0x0001) ? scrl : CString("");

    // Update the indicators.
    SetPartText(5, status1);
    SetPartText(6, status2);
    SetPartText(7, status3);
    SetPartText(8, status4);
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
