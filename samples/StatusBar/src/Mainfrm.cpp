/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"


///////////////////////////////////
// CMainFrame function definitions.
//

// Constructor for CMainFrame. Its called after CFrame's constructor.
CMainFrame::CMainFrame() : m_colored("Colored text")
{
}

// Destructor for CMainFrame.
CMainFrame::~CMainFrame()
{
}

// Adjusts the specified value for the current DPI.
int CMainFrame::AdjustForDPI(int value)
{
    CClientDC statusDC(GetStatusBar());
    statusDC.SelectObject(GetStatusBar().GetFont());

    // Perform the DPI adjustment calculation.
    int defaultDPI = 96;
    int xDPI = statusDC.GetDeviceCaps(LOGPIXELSX);
    value = MulDiv(value, xDPI, defaultDPI);

    return value;
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    //Set m_view as the view window of the frame
    SetView(m_view);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\StatusBar Sample"));

    return CFrame::Create(parent);
}

// Perform the owner drawing for the statusbar.
void CMainFrame::DrawStatusBar(LPDRAWITEMSTRUCT pDrawItem)
{
    // All parts with text using the SBT_OWNERDRAW style are handled here.

    // Retrieve the device context.
    CDC dc(pDrawItem->hDC);
    CFont font = GetStatusBar().GetFont();
    LOGFONT lf = font.GetLogFont();
    CDC memDC(dc);
    CRect partRect = pDrawItem->rcItem;

    // Modify the background, font and text color for part 4.
    if (pDrawItem->itemID == 4)
    {
        // Change the font to italic.
        lf.lfItalic = TRUE;

        // Change the part's background.
        COLORREF fillColor1 = RGB(230, 180, 0);
        COLORREF fillColor2 = RGB(240, 210, 90);
        memDC.GradientFill(fillColor1, fillColor2, partRect, TRUE);

        // Change the text color.
        COLORREF textColor = RGB(10, 20, 250);
        memDC.SetTextColor(textColor);
    }

    // Create the font.
    memDC.CreateFontIndirect(lf);

    // Draw the owner drawn text.
    // The text is stored in the itemData.
    LPCTSTR text = reinterpret_cast<LPCTSTR>(pDrawItem->itemData);
    memDC.SetBkMode(TRANSPARENT);
    memDC.DrawText(text, lstrlen(text), partRect, DT_SINGLELINE | DT_VCENTER);

    // Copy the memory dc to the statusbar's dc.
    dc.BitBlt(0, 0, partRect.Width(), partRect.Height(), memDC, 0, 0, SRCCOPY);
}

BOOL CMainFrame::DrawStatusBarBkgnd(CDC& dc, CStatusBar& statusbar)
{
    // The background can be set here if XP themes are enabled
    if (IsXPThemed())
    {
        // Exclude the progress bar window from the update region.
        CRect rc = m_progressBar.GetWindowRect();
        statusbar.ScreenToClient(rc);
        dc.ExcludeClipRect(rc);

        // Fill the background with a color gradient
        rc = statusbar.GetClientRect();
        CMemDC memDC(dc);
        memDC.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
        COLORREF fillColor1 = RGB(125, 230, 255);
        COLORREF fillColor2 = RGB(250, 150, 150);
        memDC.GradientFill(fillColor1, fillColor2, rc, TRUE);
        dc.BitBlt(0, 0, rc.Width(), rc.Height(), memDC, 0, 0, SRCCOPY);

        return TRUE;
    }

    return FALSE;
}

// Retrieves the width of the part required to contain the specified text.
int CMainFrame::GetTextPartWidth(LPCTSTR text) const
{
    CClientDC dc = GetDC();
    dc.SelectObject(GetStatusBar().GetFont());
    CSize textSize = dc.GetTextExtentPoint32(text);
    int width = textSize.cx;
    const int border = 8;
    width += border;

    return width;
}

// OnCommand responds to menu and and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch(id)
    {
    case IDM_FILE_EXIT:      return OnFileExit();
    case IDW_VIEW_STATUSBAR: return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:   return OnViewToolBar();
    case IDM_HELP_ABOUT:     return OnHelp();
    }

    return FALSE;
}

// OnCreate controls the way the frame is created.
int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
    // Use the following functions to disable options.

    // UseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar.
    // UseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar.
    // UseOwnerDrawnMenu(FALSE);     // Don't use owner draw for popup menu items.
    // UseReBar(FALSE);              // Don't use a ReBar.
    // UseStatusBar(FALSE);          // Don't use a StatusBar.
    // UseThemes(FALSE);             // Don't use themes.
    // UseToolBar(FALSE);            // Don't use a ToolBar.

    // call the base class function
    CFrame::OnCreate(cs);

    // Start a timer for the progress bar
    const int timerID = 101;
    SetTimer(timerID, 100, 0);

    // Set the background color (ignored if XP themes are enabled)
    COLORREF backgrnd = RGB(125, 230, 255);
    GetStatusBar().SendMessage(SB_SETBKCOLOR, 0, backgrnd);

    // Create the Hyperlink
    m_hyperlink.Create(GetStatusBar());

    // Create the ProgressBar
    m_progressBar.Create(GetStatusBar());

    return 0;
}

// Issue a close request to the frame.
BOOL CMainFrame::OnFileExit()
{
    Close();

    return TRUE;
}

// Called after the frame is created.
void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    TRACE("Frame created\n");
}

// Process the timer message.
void CMainFrame::OnTimer()
{
    // Change the Progress Bar indication.
    m_progressBar.OffsetPos(1);
    if (m_progressBar.GetRange(FALSE) == m_progressBar.GetPos())
        m_progressBar.SetPos(0);
}

// Specifies the images used on menu items.
void CMainFrame::SetupMenuIcons()
{
    // Use the MenuIcons bitmap for images in menu items.
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        AddMenuIcons(data, RGB(192, 192, 192), IDW_MAIN);
    else
        AddMenuIcons(data, RGB(192, 192, 192), IDB_TOOLBAR16);
}

// Configure the toolbar.
void CMainFrame::SetupToolBar()
{
    // Set the Resource IDs for the toolbar buttons
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_OPEN, FALSE);    // disabled button
    AddToolBarButton( IDM_FILE_SAVE, FALSE);    // disabled button

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_EDIT_CUT,   FALSE );  // disabled button
    AddToolBarButton( IDM_EDIT_COPY,  FALSE );  // disabled button
    AddToolBarButton( IDM_EDIT_PASTE, FALSE );  // disabled button

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_FILE_PRINT, FALSE);   // disabled button

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_HELP_ABOUT );
}

// Updates the status indicators.
void CMainFrame::SetStatusIndicators()
{
    if (IsWindow())
    {
        CString cap = LoadString(IDW_INDICATOR_CAPS);
        CString num = LoadString(IDW_INDICATOR_NUM);
        CString ovr = LoadString(IDW_INDICATOR_OVR);
        CString ins = LoadString(IDW_INDICATOR_INS);
        CString scrl = LoadString(IDW_INDICATOR_SCRL);
        CString empty;

        m_cap  = (::GetKeyState(VK_CAPITAL) & 0x0001) ? cap : empty;
        m_num  = (::GetKeyState(VK_NUMLOCK) & 0x0001) ? num : empty;
        m_ovr  = (::GetKeyState(VK_INSERT) & 0x0001) ? ovr : ins;
        m_scrl = (::GetKeyState(VK_SCROLL) & 0x0001) ? scrl : empty;

        // Update the indicators.
        // Use member variables for owner drawn text to keep the text in scope.
        GetStatusBar().SetPartText(5, m_cap,  SBT_OWNERDRAW);
        GetStatusBar().SetPartText(6, m_num,  SBT_OWNERDRAW);
        GetStatusBar().SetPartText(7, m_ovr,  SBT_OWNERDRAW);
        GetStatusBar().SetPartText(8, m_scrl, SBT_OWNERDRAW);
    }
}

// Reposition the statusbar parts. It's called when the statusbar is resized.
void CMainFrame::SetStatusParts()
{
    if (m_hyperlink.IsWindow())
    {
        const int progressWidth = 100;
        const int gripWidth = 20;
        int iconSide = GetStatusBar().GetClientRect().Height();

        // Fill a vector with the status bar part widths.
        std::vector<int> partWidths;
        partWidths.push_back(GetTextPartWidth(m_hyperlink.GetLinkName()));
        partWidths.push_back(AdjustForDPI(progressWidth));
        partWidths.push_back(iconSide);
        partWidths.push_back(GetTextPartWidth(m_colored));
        partWidths.push_back(GetTextPartWidth(LoadString(IDW_INDICATOR_CAPS)));
        partWidths.push_back(GetTextPartWidth(LoadString(IDW_INDICATOR_NUM)));
        partWidths.push_back(GetTextPartWidth(LoadString(IDW_INDICATOR_OVR)));
        partWidths.push_back(GetTextPartWidth(LoadString(IDW_INDICATOR_SCRL)));

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
            GetStatusBar().SetPartWidth(part, *iter);
        }

        // Reposition the hyperlink over part 1.
        CRect partRect = GetStatusBar().GetPartRect(1);
        const int border = 2;
        partRect.OffsetRect(border, 0);
        m_hyperlink.SetWindowPos(0, partRect, SWP_SHOWWINDOW);

        // Reposition the progress bar over part 2.
        partRect = GetStatusBar().GetPartRect(2);
        m_progressBar.SetWindowPos(0, partRect, SWP_SHOWWINDOW);

        // Add the icon to part 3.
        iconSide = iconSide - (iconSide % 8);
        GetStatusBar().SetPartIcon(3, (HICON)GetApp()->LoadImage(MAKEINTRESOURCE(IDW_MAIN), IMAGE_ICON, iconSide, iconSide));

        // Assign the colored text for part 4.
        GetStatusBar().SetPartText(4, m_colored, SBT_OWNERDRAW);
    }
}

// Process the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_TIMER:    OnTimer();     break;
        }

        return WndProcDefault(msg, wparam, lparam);
    }

    catch (const CException& e)
    {
        // Display the exception.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);
        return 0;
    }
}
