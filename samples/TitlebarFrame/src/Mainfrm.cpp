/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include <dwmapi.h>
#include <uxtheme.h>
#include <vsstyle.h>
#include <vssym32.h>
#include "Mainfrm.h"
#include "resource.h"

// Add the following libraries to the linker.
#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "Dwmapi.lib")

// Scales the value to the window's dots per inch (dpi) value.
int dpi_scale(int value, UINT dpi)
{
    // A scale factor of 100 percent is 96 (logical) DPI.
    return (value * dpi) / 96;
}

// Center the inner rectangle within the outer rectangle.
void CenterRectInRect(CRect* innerRect, const CRect& outerRect)
{
    int innerWidth = innerRect->Width();
    int innerHeight = innerRect->Height();
    int outerWidth = outerRect.Width();
    int outerHeight = outerRect.Height();

    int paddingX = (outerWidth - innerWidth) / 2;
    int paddingY = (outerHeight - innerHeight) / 2;

    innerRect->left = outerRect.left + paddingX;
    innerRect->top = outerRect.top + paddingY;
    innerRect->right = innerRect->left + innerWidth;
    innerRect->bottom = innerRect->top + innerHeight;
}


//////////////////////////////////
// CMainFrame function definitions
//

// Constructor.
CMainFrame::CMainFrame() : m_isToolbarShown(true),
                           m_hoveredButton(TitlebarButton::None),
                           m_oldHoveredButton(TitlebarButton::None),
                           m_isMiniFrame(false)
{
}

// Destructor.
CMainFrame::~CMainFrame()
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    //Set m_View as the view window of the frame
    SetView(m_view);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\Titlebar Frame"));

    return CFrame::Create(parent);
}

// Draw title bar background
void CMainFrame::DrawBackground(CDC& dc) const
{
    CRect titlebarRect = GetTitlebarRect();
    COLORREF titlebarColor = IsActive() ? m_colors.active : m_colors.inactive;
    CBrush titlebarBrush(titlebarColor);
    dc.FillRect(titlebarRect, titlebarBrush);
}

// Draw the title bar close button.
void CMainFrame::DrawCloseButton(CDC& dc) const
{
    ButtonRects buttonRects = GetButtonRects();
    UINT dpi = GetDpiForWindow(*this);
    int iconDimension = dpi_scale(10, dpi);
    COLORREF itemColor = IsActive() ? m_colors.activeItem : m_colors.inactiveItem;
    dc.CreatePen(PS_SOLID, 1, itemColor);
    if (m_hoveredButton == TitlebarButton::Close)
    {
        CBrush fillBrush(m_colors.hoverClose);
        dc.FillRect(buttonRects.close, fillBrush);
        dc.CreatePen(PS_SOLID, 1, RGB(0xFF, 0xFF, 0xFF));
    }

    CRect iconRect;
    iconRect.right = iconDimension;
    iconRect.bottom = iconDimension;
    CenterRectInRect(&iconRect, buttonRects.close);
    dc.MoveTo(iconRect.left, iconRect.top);
    dc.LineTo(iconRect.right + 1, iconRect.bottom + 1);
    dc.MoveTo(iconRect.left, iconRect.bottom);
    dc.LineTo(iconRect.right + 1, iconRect.top - 1);
}

// Draw the title bar minimize button.
void CMainFrame::DrawMinimizeButton(CDC& dc) const
{
    COLORREF itemColor = IsActive() ? m_colors.activeItem : m_colors.inactiveItem;
    CBrush buttonIconBrush(itemColor);
    ButtonRects buttonRects = GetButtonRects();
    UINT dpi = GetDpiForWindow(*this);
    int iconDimension = dpi_scale(10, dpi);
    if (m_hoveredButton == TitlebarButton::Minimize)
    {
        COLORREF hoverColor = m_colors.hover;
        CBrush hoverBrush(hoverColor);
        dc.FillRect(buttonRects.minimize, hoverBrush);
    }

    CRect iconRect;
    iconRect.right = iconDimension;
    iconRect.bottom = 1;
    CenterRectInRect(&iconRect, buttonRects.minimize);
    dc.FillRect(iconRect, buttonIconBrush);
}

// Draw the title bar maximize button.
void CMainFrame::DrawMaximizeButton(CDC& dc) const
{
    ButtonRects buttonRects = GetButtonRects();
    UINT dpi = GetDpiForWindow(*this);
    int iconDimension = dpi_scale(10, dpi);
    if (m_hoveredButton == TitlebarButton::Maximize)
    {
        COLORREF hoverColor = m_colors.hover;
        CBrush hoverBrush(hoverColor);
        dc.FillRect(buttonRects.maximize, hoverBrush);
    }

    CRect iconRect;
    iconRect.right = iconDimension;
    iconRect.bottom = iconDimension;
    CenterRectInRect(&iconRect, buttonRects.maximize);

    COLORREF itemColor = IsActive() ? m_colors.activeItem : m_colors.inactiveItem;
    dc.CreatePen(PS_SOLID, 1, itemColor);
    SelectObject(dc, GetStockObject(HOLLOW_BRUSH));
    dc.Rectangle(iconRect.left, iconRect.top, iconRect.right, iconRect.bottom);
}

// Draw the title bar caption.
void CMainFrame::DrawTitleText(CDC& dc) const
{
    HTHEME theme = ::OpenThemeData(*this, L"WINDOW");
    LOGFONT logicalFont;
    if (SUCCEEDED(GetThemeSysFont(theme, TMT_CAPTIONFONT, &logicalFont)))
    {
        dc.CreateFontIndirect(logicalFont);
    }

    // Calculate the title bar text rect.
    CString titlebarText = GetWindowText();
    ButtonRects buttonRects = GetButtonRects();
    CRect titlebarTextRect = GetTitlebarRect();
    titlebarTextRect.left += titlebarTextRect.Height();
    titlebarTextRect.right = buttonRects.minimize.left;

    // Draw the title bar text.
    DTTOPTS drawThemeOptions = { sizeof(drawThemeOptions) };
    drawThemeOptions.dwFlags = DTT_TEXTCOLOR;
    COLORREF itemColor = IsActive() ? m_colors.activeItem : m_colors.inactiveItem;
    drawThemeOptions.crText = itemColor;
    ::DrawThemeTextEx(
        theme,
        dc,
        0, 0,
        titlebarText,
        -1,
        DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS,
        &titlebarTextRect,
        &drawThemeOptions
    );

    ::CloseThemeData(theme);
}

// Draw the top shadow. Original is missing because of the client rect extension.
// Required on Windows 10. Not required on Windows 11.
void CMainFrame::DrawTopShadow(CDC& dc) const
{
    COLORREF shadowColor = m_colors.topShadow;
    COLORREF titlebarColor = IsActive() ? m_colors.active : m_colors.inactive;
    COLORREF topShadowColor = IsActive() ? shadowColor : RGB(
        (GetRValue(titlebarColor) + GetRValue(shadowColor)) / 2,
        (GetGValue(titlebarColor) + GetGValue(shadowColor)) / 2,
        (GetBValue(titlebarColor) + GetBValue(shadowColor)) / 2
    );

    CBrush topShadowBrush(topShadowColor);
    CRect topShadowRect = GetShadowRect();
    dc.FillRect(topShadowRect, topShadowBrush);
}

// Draw title bar icon for the system menu.
void CMainFrame::DrawWindowIcon(CDC& dc) const
{
    // Draw title bar icon (system button)
    HICON icon = GetApp()->LoadIcon(IDW_MAIN);
    if (icon == 0)
    {
        icon = GetApp()->LoadStandardIcon(IDI_APPLICATION);
    }
    int x = GetButtonRects().system.left;
    int y = GetButtonRects().system.top;
    int cx = GetButtonRects().system.Width();
    int cy = GetButtonRects().system.Height();
    dc.DrawIconEx(x, y, icon, cx, cy, 0, 0, DI_NORMAL);
}

// Retrieves the rect locations of the title bar buttons.
ButtonRects CMainFrame::GetButtonRects() const
{
    UINT dpi = GetDpiForWindow(*this);
    ButtonRects buttonRects;
    // Sadly SM_CXSIZE does not result in the right size buttons for Win10
    int buttonWidth = dpi_scale(47, dpi);
    buttonRects.close = GetTitlebarRect();
    buttonRects.close.top += GetShadowRect().Height();
    buttonRects.close.left = buttonRects.close.right - buttonWidth;

    buttonRects.maximize = buttonRects.close;
    buttonRects.maximize.left -= buttonWidth;
    buttonRects.maximize.right -= buttonWidth;

    buttonRects.minimize = buttonRects.maximize;
    buttonRects.minimize.left -= buttonWidth;
    buttonRects.minimize.right -= buttonWidth;

    int leftPadding = dpi_scale(8, dpi);
    double scale = 0.6;
    int titlebarHeight = GetTitlebarRect().Height();
    int buttonSize = static_cast<int>(titlebarHeight * scale);
    int border = (titlebarHeight - buttonSize) / 2;
    buttonRects.system.left = leftPadding;
    buttonRects.system.top = border;
    buttonRects.system.right = buttonRects.system.left + buttonSize;
    buttonRects.system.bottom = buttonRects.system.top + buttonSize;

    return buttonRects;
}

// Returns the button under the mouse cursor.
TitlebarButton CMainFrame::GetHoveredButton() const
{
    CPoint cursorPoint = GetCursorPos();
    ScreenToClient(cursorPoint);
    ButtonRects buttonRects = GetButtonRects();

    TitlebarButton hoveredButton = TitlebarButton::None;
    if (PtInRect(&buttonRects.system, cursorPoint))
    {
        hoveredButton = TitlebarButton::System;
    }
    else if (PtInRect(&buttonRects.close, cursorPoint))
    {
        hoveredButton = TitlebarButton::Close;
    }
    else if (PtInRect(&buttonRects.minimize, cursorPoint))
    {
        hoveredButton = TitlebarButton::Minimize;
    }
    else if (PtInRect(&buttonRects.maximize, cursorPoint))
    {
        hoveredButton = TitlebarButton::Maximize;
    }

    return hoveredButton;
}

// Returns the rect for the shadow at the top of the titlebar.
CRect CMainFrame::GetShadowRect() const
{
    const int shadowHeight = 1;
    CRect rect = GetClientRect();
    rect.bottom = rect.top + shadowHeight;
    return rect;
}

// Returns the rect for the titlebar.
// Adapted from:
// https://github.com/oberth/custom-chrome/blob/master/source/gui/window_helper.hpp#L52-L64
CRect CMainFrame::GetTitlebarRect() const
{
    CSize barSize;
    const int borders = 2;
    HTHEME theme = ::OpenThemeData(*this, L"WINDOW");
    UINT dpi = ::GetDpiForWindow(*this);
    ::GetThemePartSize(theme, NULL, WP_CAPTION, CS_ACTIVE, NULL, TS_TRUE, &barSize);
    ::CloseThemeData(theme);
    int height = dpi_scale(barSize.cy, dpi) + borders;

    CRect rect = GetClientRect();
    rect.bottom = rect.top + height;
    return rect;
}

// Returns the rect for the view area.
CRect CMainFrame::GetViewRect() const
{
    CRect clientRect = GetClientRect();
    clientRect.top = GetTitlebarRect().Height();

    if (GetStatusBar().IsWindow() && GetStatusBar().IsWindowVisible())
        clientRect = ExcludeChildRect(clientRect, GetStatusBar());

    if (GetReBar().IsWindow() && GetReBar().IsWindowVisible())
        clientRect = ExcludeChildRect(clientRect, GetReBar());
    else
        if (GetToolBar().IsWindow() && GetToolBar().IsWindowVisible())
            clientRect = ExcludeChildRect(clientRect, GetToolBar());

    return clientRect;
}

// Returns true of the window is maximized, false otherwise.
bool CMainFrame::IsMaximized() const
{
    WINDOWPLACEMENT placement = { 0 };
    placement.length = sizeof(WINDOWPLACEMENT);
    if (GetWindowPlacement(placement))
    {
        return placement.showCmd == SW_SHOWMAXIMIZED;
    }

    return false;
}

// Called when the window is activated or deactivated.
LRESULT CMainFrame::OnActivate(UINT msg, WPARAM wparam, LPARAM lparam)
{
    RECT titlebarRect = GetTitlebarRect();
    InvalidateRect(titlebarRect, FALSE);
    return CFrame::OnActivate(msg, wparam, lparam);
}

// Called when the frame window is asked to close.
void CMainFrame::OnClose()
{
    // Close the preview.
    if (GetView() == m_preview)
        OnPreviewClose();

    // Proceed with closing the frame.
    CFrame::OnClose();
}

// OnCommand responds to menu and and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_FILE_OPEN:       return OnFileOpen();
    case IDM_FILE_SAVE:       return OnFileSave();
    case IDM_FILE_SAVEAS:     return OnFileSave();
    case IDM_FILE_PREVIEW:    return OnFilePreview();
    case IDM_FILE_PRINT:      return OnFilePrint();
    case IDM_FILE_EXIT:       return OnFileExit();
    case IDM_MODE_FULL:
    {
        m_isMiniFrame = false;

        // Check the full frame radio button.
        CMenu ViewMenu = GetFrameMenu().GetSubMenu(3);
        ViewMenu.CheckMenuRadioItem(IDM_MODE_MINI, IDM_MODE_FULL, IDM_MODE_FULL, 0);

        // Show the menu bar in the rebar.
        GetReBar().ShowBand(0, TRUE);
        RecalcLayout();
        return TRUE;
    }
    case IDM_MODE_MINI:
    {
        m_isMiniFrame = true;

        // Check the mini frame radio button.
        CMenu ViewMenu = GetFrameMenu().GetSubMenu(3);
        ViewMenu.CheckMenuRadioItem(IDM_MODE_MINI, IDM_MODE_FULL, IDM_MODE_MINI, 0);

        // Hide the menu bar in the rebar.
        GetReBar().ShowBand(0, FALSE);
        RecalcLayout();
        return TRUE;
    }

    case IDW_VIEW_STATUSBAR:  return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:    return OnViewToolBar();
    case IDM_HELP_ABOUT:      return OnHelp();
    }

    return FALSE;
}

// OnCreate controls the way the frame is created.
// Overriding CFrame::OnCreate is optional.
int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
    // Inform the application of the frame change to force redrawing with the new
    // client area that is extended into the title bar.
    SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);

    // Call the base class function.
    int result = CFrame::OnCreate(cs);

    if (GetReBar().IsWindow())
    {
        // Adjust the rebar style so we can reposition it.
        DWORD style = (DWORD)GetReBar().GetWindowLongPtr(GWL_STYLE);
        style += CCS_NOPARENTALIGN;
        GetReBar().SetWindowLongPtr(GWL_STYLE, style);
    }

    // Create the menubar for the mini frame.
    m_menubar2.Create(*this);
    m_menubar2.SetupMenuBar(GetFrameMenu());

    // Check the full mode radio button.
    CMenu ViewMenu = GetFrameMenu().GetSubMenu(3);
    ViewMenu.CheckMenuRadioItem(IDM_MODE_MINI, IDM_MODE_FULL, IDM_MODE_FULL, 0);

    // Adjust the title bar colors to match the rebar theme.
    ReBarTheme theme = GetReBarTheme();
    m_colors.active = theme.clrBkgnd1;


    m_colors.inactive = RGB((GetRValue(m_colors.active) * 2 + 256) / 3,
                            (GetGValue(m_colors.active) * 2 + 256) / 3,
                            (GetBValue(m_colors.active) * 2 + 256) / 3);

    m_colors.hover = RGB( (GetRValue(m_colors.active) + 180) / 2,
                          (GetGValue(m_colors.active) + 180) / 2,
                          (GetBValue(m_colors.active) + 180) / 2 );

    return result;
}

// Handles CustomDraw notification from WM_NOTIFY.
LRESULT CMainFrame::OnCustomDraw(LPNMHDR pNMHDR)
{
    if (::SendMessage(pNMHDR->hwndFrom, UWM_GETCTOOLBAR, 0, 0))
    {
        // Custom draw the menubar within the rebar.
        if (pNMHDR->hwndFrom == GetMenuBar())
            return CustomDrawMenuBar(pNMHDR);

        // Custom draw the menubar within the titlebar.
        if (pNMHDR->hwndFrom == m_menubar2)
        {
            LPNMTBCUSTOMDRAW pCustomDraw = (LPNMTBCUSTOMDRAW)pNMHDR;
            if (pCustomDraw->nmcd.dwDrawStage == CDDS_PREPAINT)
            {
                // Set titlebar's menubar background color before doing
                // the rest of the custom drawing.
                CRect rect = m_menubar2.GetClientRect();
                COLORREF titlebarColor = IsActive() ? m_colors.active : m_colors.inactive;
                CBrush brush(titlebarColor);
                FillRect(pCustomDraw->nmcd.hdc, rect, brush);
            }
            return CustomDrawMenuBar(pNMHDR);
        }

        // Custom draw the toolbar within the rebar.
        return CustomDrawToolBar(pNMHDR);
    }

    return 0;
}

// Limit the minimum size of the window.
LRESULT CMainFrame::OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam)
{
    LPMINMAXINFO lpMMI = (LPMINMAXINFO)lparam;
    lpMMI->ptMinTrackSize.x = 550;
    lpMMI->ptMinTrackSize.y = 400;
    return WndProcDefault(msg, wparam, lparam);
}

// Issue a close request to the frame to end the program.
BOOL CMainFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Called after the window is created.
void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    TRACE("Frame created\n");
}

// Create the File Open dialog to choose the file to load.
BOOL CMainFrame::OnFileOpen()
{
    CString filter = _T("Program Files (*.cpp; *.h)|*.cpp; *.h|All Files (*.*)|*.*||");
    CFileDialog fileDlg(TRUE);    // TRUE for file open
    fileDlg.SetFilter(filter);
    fileDlg.SetDefExt(_T(".cpp"));

    // Bring up the file open dialog retrieve the selected filename
    if (fileDlg.DoModal(*this) == IDOK)
    {
        GetDoc().FileLoad(fileDlg.GetPathName());
    }

    return TRUE;
}

// Create the File Save dialog to choose the file to save.
BOOL CMainFrame::OnFileSave()
{
    CString filter = _T("Program Files (*.cpp; *.h)|*.cpp; *.h|All Files (*.*)|*.*||");
    CFileDialog fileDlg(FALSE);    // FALSE for file save
    fileDlg.SetFilter(filter);
    fileDlg.SetDefExt(_T(".cpp"));

    // Bring up the file save dialog retrieve the selected filename
    if (fileDlg.DoModal(*this) == IDOK)
    {
        GetDoc().FileStore(fileDlg.GetPathName());
    }

    return TRUE;
}

// Previews a print job before sending it to the printer.
BOOL CMainFrame::OnFilePreview()
{
    try
    {
        m_isToolbarShown = GetToolBar().IsWindow() && GetToolBar().IsWindowVisible();

        // Get the device contect of the default or currently chosen printer
        CPrintDialog printDlg;
        CDC printerDC = printDlg.GetPrinterDC();

        // Create the preview window if required
        if (!m_preview.IsWindow())
            m_preview.Create(*this);

        // Specify the source of the PrintPage function
        m_preview.SetSource(m_view);

        // Set the preview's owner (for messages)
        m_preview.DoPrintPreview(*this);

        // Swap views
        SetView(m_preview);

        // Hide the menu and toolbar
        ShowMenu(FALSE);
        ShowToolBar(FALSE);

        // Update status
        CString status = _T("Printer: ") + printDlg.GetDeviceName();
        SetStatusText(status);
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), _T("Print Preview Failed"), MB_ICONWARNING);
        SetView(m_view);
        ShowMenu(GetFrameMenu() != 0);
        ShowToolBar(m_isToolbarShown);
    }

    return TRUE;
}

// Bring up a dialog to choose the printer.
BOOL CMainFrame::OnFilePrint()
{
    CPrintDialog printdlg;

    try
    {
        if (IDOK == printdlg.DoModal(*this))
        {
            m_view.QuickPrint(_T("Frame Sample"));
        }

    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), _T("Print Failed"), MB_ICONWARNING);
    }

    return TRUE;
}

// Called when a menu is active, and a key is pressed other than an accelerator.
LRESULT CMainFrame::OnMenuChar(UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (m_isMiniFrame)
    {
        if ((m_menubar2.IsWindow()) && (LOWORD(wparam) != VK_SPACE))
        {
            // Activate the menubar for key a pressed with the Alt key is held down.
            m_menubar2.MenuChar(WM_MENUCHAR, wparam, lparam);
            return -1;
        }
    }

    return CFrame::OnMenuChar(msg, wparam, lparam);
}

// Handle WM_NCCALCSIZE to extend client (paintable) area into the title bar region.
LRESULT CMainFrame::OnNCCalcSize(UINT, WPARAM wparam, LPARAM lparam)
{
    // lParam points to an NCCALCSIZE_PARAMS structure if wParam is TRUE.
    if (wparam)
    {
        UINT dpi = ::GetDpiForWindow(*this);

        int frameX = ::GetSystemMetricsForDpi(SM_CXFRAME, dpi);
        int frameY = ::GetSystemMetricsForDpi(SM_CYFRAME, dpi);
        int padding = ::GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpi);

        NCCALCSIZE_PARAMS* params = (NCCALCSIZE_PARAMS*)lparam;
        RECT* clientRect = params->rgrc;

        clientRect->right -= frameX + padding;
        clientRect->left += frameX + padding;
        clientRect->bottom -= frameY + padding;

        if (IsMaximized())
        {
            clientRect->top += padding;
        }
    }

    return 0;
}

// Track when mouse hovers each of the title bar buttons to draw the highlight correctly.
LRESULT CMainFrame::OnNCMouseMove(UINT msg, WPARAM wparam, LPARAM lparam)
{
    TitlebarButton newHoveredButton = GetHoveredButton();
    ButtonRects buttonRects = GetButtonRects();

    if (newHoveredButton != m_hoveredButton)
    {
        m_hoveredButton = newHoveredButton;
        if (IsLeftButtonDown() && (newHoveredButton != m_oldHoveredButton))
        {
            m_hoveredButton = TitlebarButton::None;
        }
        InvalidateRect(buttonRects.close, FALSE);
        InvalidateRect(buttonRects.minimize, FALSE);
        InvalidateRect(buttonRects.maximize, FALSE);
    }

    return WndProcDefault(msg, wparam, lparam);
}

// Update the hovered button when the mouse leaves the non-client area.
LRESULT CMainFrame::OnNCMouseLeave(UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (m_hoveredButton != TitlebarButton::None)
    {
        RECT titlebarRect = GetTitlebarRect();
        InvalidateRect(titlebarRect, FALSE);
        m_hoveredButton = TitlebarButton::None;
    }

    return WndProcDefault(msg, wparam, lparam);
}

// Repositions the frame's child windows.
void CMainFrame::RecalcLayout()
{
    // Resize the status bar
    if (GetStatusBar().IsWindow() && GetStatusBar().IsWindowVisible())
    {
        VERIFY(GetStatusBar().SetWindowPos(0, 0, 0, 0, 0, SWP_SHOWWINDOW));
        SetStatusParts();
        SetStatusIndicators();
    }

    // Reposition the rebar.
    if (GetReBar().IsWindow())
    {
        CRect rc = GetTitlebarRect();

        int cyRB = 0;
        for (UINT u = 0; u < GetReBar().GetRowCount(); ++u)
            cyRB += GetReBar().GetRowHeight(u);

        GetReBar().SetWindowPos(NULL, 0, rc.Height(), rc.Width(), cyRB, SWP_SHOWWINDOW);
    }

    if (m_isMiniFrame)
    {
        // Position the second menubar.
        CRect rect = GetTitlebarRect();

        if (m_menubar2.IsWindow())
        {
            CRect menuRect = GetClientRect();
            CSize size = m_menubar2.GetMaxSize();
            menuRect.left = GetButtonRects().system.right;
            menuRect.top = (rect.Height() - size.cy) / 2;;
            menuRect.right = menuRect.left + size.cx;
            menuRect.bottom = menuRect.top + size.cy;
            m_menubar2.SetWindowPos(NULL, menuRect, SWP_SHOWWINDOW);
        }
    }
    else if (m_menubar2.IsWindow())
    {
        // Hide the second menubar.
        m_menubar2.SetWindowPos(NULL, CRect(0,0,0,0), 0);
    }

    // Position the view window.
    if (GetView().IsWindow())
        RecalcViewLayout();

    // Adjust rebar bands.
    if (GetReBar().IsWindow())
    {
        if (GetReBarTheme().UseThemes && GetReBarTheme().BandsLeft)
            GetReBar().MoveBandsLeft();

        if (GetMenuBar().IsWindow())
            SetMenuBarBandSize();
    }
}

// Returns the hot spot location of the cursor's position.
LRESULT CMainFrame::OnNCHitTest(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Let the default procedure handle resizing areas
    LRESULT hit = WndProcDefault(msg, wparam, lparam);
    switch (hit)
    {
    case HTNOWHERE:
    case HTRIGHT:
    case HTLEFT:
    case HTTOPLEFT:
    case HTTOP:
    case HTTOPRIGHT:
    case HTBOTTOMRIGHT:
    case HTBOTTOM:
    case HTBOTTOMLEFT:
    {
        return hit;
    }
    }

    // Provide the hit test for the caption buttons.
    switch (m_hoveredButton)
    {
    case TitlebarButton::System:      return HTSYSMENU;
    case TitlebarButton::Minimize:    return HTMINBUTTON;
    case TitlebarButton::Maximize:    return HTMAXBUTTON;
    case TitlebarButton::Close:       return HTCLOSE;
    }

    // Looks like adjustment happening in NCCALCSIZE is messing with the detection
    // of the top hit area so manually fixing that.
    UINT dpi = ::GetDpiForWindow(*this);
    int frameY = ::GetSystemMetricsForDpi(SM_CYFRAME, dpi);
    int padding = ::GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpi);
    CPoint cursorPoint(lparam);
    ScreenToClient(cursorPoint);
    if (cursorPoint.y > 0 && cursorPoint.y < frameY + padding)
    {
        return HTTOP;
    }

    // Since we are drawing our own caption, this needs to be a custom test
    if (cursorPoint.y < GetTitlebarRect().bottom)
    {
        return HTCAPTION;
    }

    return HTCLIENT;
}

// Handle left mouse button double clicks in the non-client area.
LRESULT CMainFrame::OnNCLButtonDblClk(UINT msg, WPARAM wparam, LPARAM lparam)
{
    CPoint cursorPoint = GetCursorPos();
    ScreenToClient(cursorPoint);

    // Convert a double click to a single click for the system menu.
    if (PtInRect(GetButtonRects().system, cursorPoint))
    {
        return OnNCLButtonDown(msg, wparam, lparam);
    }

    return WndProcDefault(msg, wparam, lparam);
}

// Handle left mouse button clicks in the non-client area.
LRESULT CMainFrame::OnNCLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
    m_oldHoveredButton = m_hoveredButton;
    if (m_hoveredButton == TitlebarButton::System)
    {
        OnSystemButton();
        return 0;
    }
    else if (m_hoveredButton != TitlebarButton::None)
    {
        // remove default handling of the click to avoid it counting as drag.
        return 0;
    }

    // Default handling allows for dragging and double click to maximize.
    return WndProcDefault(msg, wparam, lparam);
}

// Map button clicks to the correct messages for the window.
LRESULT CMainFrame::OnNCLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (m_hoveredButton == m_oldHoveredButton)
    {
        if (m_hoveredButton == TitlebarButton::Close)
        {
            Close();
            return 0;
        }
        else if (m_hoveredButton == TitlebarButton::Minimize)
        {
            ShowWindow(SW_MINIMIZE);
            return 0;
        }
        else if (m_hoveredButton == TitlebarButton::Maximize)
        {
            int mode = IsMaximized() ? SW_NORMAL : SW_MAXIMIZE;
            ShowWindow(mode);
            return 0;
        }
    }

    return WndProcDefault(msg, wparam, lparam);
}

// Display a system menu with a right mouse button click on the titlebar.
LRESULT CMainFrame::OnNCRButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
    CPoint cursor(lparam);
    CPoint clientCursor = cursor;
    ScreenToClient(clientCursor);  // Convert cursor position to window coordinates.

    CRect titlebar = GetTitlebarRect();
    if (titlebar.PtInRect(clientCursor))
    {
        // Display the system menu.
        CMenu systemMenu = GetSystemMenu(TRUE);
        UINT flags = TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL | TPM_RETURNCMD;
        UINT command = systemMenu.TrackPopupMenu(flags, cursor.x, cursor.y, *this);

        // Process the system command.
        SendMessage(WM_SYSCOMMAND, command, 0);
    }

    return WndProcDefault(msg, wparam, lparam);
}

// Called when any part of the window is repainted.
LRESULT CMainFrame::OnPaint(UINT, WPARAM, LPARAM)
{
    CPaintDC dc(*this);

    // Draw the title bar.
    DrawBackground(dc);
    DrawMinimizeButton(dc);
    DrawMaximizeButton(dc);
    DrawCloseButton(dc);
    if (!m_isMiniFrame)
    {
        DrawTitleText(dc);
    }
    DrawWindowIcon(dc);
    DrawTopShadow(dc);

    return 0;
}

// Called when the Print Preview's "Close" button is pressed.
LRESULT CMainFrame::OnPreviewClose()
{
    // Swap the view.
    SetView(m_view);

    // Show the menu and toolbar.
    if (!m_isMiniFrame)
    {
        ShowMenu(GetFrameMenu() != 0);
    }
    ShowToolBar(m_isToolbarShown);

    SetStatusText(LoadString(IDW_READY));

    return 0;
}

// Called when the Print Preview's "Print Now" button is pressed.
LRESULT CMainFrame::OnPreviewPrint()
{
    m_view.QuickPrint(_T("Frame Sample"));
    return 0;
}

// Called when the Print Preview's "Print Setup" button is pressed.
LRESULT CMainFrame::OnPreviewSetup()
{
    // Call the print setup dialog.
    CPrintDialog printDlg(PD_PRINTSETUP);
    try
    {
        // Display the print dialog
        if (printDlg.DoModal(*this) == IDOK)
        {
            CString status = _T("Printer: ") + printDlg.GetDeviceName();
            SetStatusText(status);
        }
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);
    }

    // Initiate the print preview.
    m_preview.DoPrintPreview(*this);

    return 0;
}

// Called when the window is resized.
LRESULT CMainFrame::OnSize(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Redraw the title bar.
    CRect rect = GetTitlebarRect();
    InvalidateRect(rect, FALSE);

    return CFrame::OnSize(msg, wparam, lparam);
}

// Called when the window is maximized, minimized or restored.
LRESULT CMainFrame::OnSysCommand(UINT msg, WPARAM wparam, LPARAM lparam)
{
    m_hoveredButton = TitlebarButton::None;

    if (m_isMiniFrame)
    {
        // Pass menu keystrokes to the CMenuBar to process alt keys, F10 etc.
        if ((SC_KEYMENU == wparam) && (VK_SPACE != lparam) && GetMenuBar().IsWindow())
        {
            m_menubar2.SysCommand(msg, wparam, lparam);
            return 0;
        }
    }

    return CFrame::OnSysCommand(msg, wparam, lparam);
}

// The system menu is displayed when the application's icon is clicked.
void CMainFrame::OnSystemButton() const
{
    SetForegroundWindow();

    // Calculate the position of the system menu.
    CRect rc = GetButtonRects().system;
    rc.bottom = GetTitlebarRect().bottom;
    ClientToScreen(rc);
    TPMPARAMS tpm;
    tpm.cbSize = sizeof(tpm);
    tpm.rcExclude = rc;

    // Display the system menu.
    CMenu systemMenu = GetSystemMenu(TRUE);
    UINT flags = TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL | TPM_RETURNCMD;
    UINT command = systemMenu.TrackPopupMenuEx(flags, rc.left, rc.bottom, *this, &tpm);

    // Process the system command.
    SendMessage(WM_SYSCOMMAND, command, 0);
}

// Configure the menu icons.
void CMainFrame::SetupMenuIcons()
{
    // Set the bitmap used for menu icons
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        AddMenuIcons(data, RGB(192, 192, 192), IDW_MAIN, 0);
    else
        AddMenuIcons(data, RGB(192, 192, 192), IDB_MENUICONS);
}

// Set the resource IDs and images for the toolbar buttons.
void CMainFrame::SetupToolBar()
{
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_OPEN  );
    AddToolBarButton( IDM_FILE_SAVE  );

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_EDIT_CUT,   FALSE );  // disabled button
    AddToolBarButton( IDM_EDIT_COPY,  FALSE );  // disabled button
    AddToolBarButton( IDM_EDIT_PASTE, FALSE );  // disabled button

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_FILE_PRINT );

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_HELP_ABOUT );
}

// Process the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_ACTIVATE:           return OnActivate(msg, wparam, lparam);
        case WM_GETMINMAXINFO:      return OnGetMinMaxInfo(msg, wparam, lparam);
        case WM_MENUCHAR:           return OnMenuChar(msg, wparam, lparam);
        case WM_NCMOUSELEAVE:       return OnNCMouseLeave(msg, wparam, lparam);
        case WM_NCCALCSIZE:         return OnNCCalcSize(msg, wparam, lparam);
        case WM_NCHITTEST:          return OnNCHitTest(msg, wparam, lparam);
        case WM_NCLBUTTONDBLCLK:    return OnNCLButtonDblClk(msg, wparam, lparam);
        case WM_NCLBUTTONDOWN:      return OnNCLButtonDown(msg, wparam, lparam);
        case WM_NCLBUTTONUP:        return OnNCLButtonUp(msg, wparam, lparam);
        case WM_NCMOUSEMOVE:        return OnNCMouseMove(msg, wparam, lparam);
        case WM_NCRBUTTONDOWN:      return OnNCRButtonDown(msg, wparam, lparam);
        case WM_PAINT:              return OnPaint(msg, wparam, lparam);
        case WM_SIZE:               return OnSize(msg, wparam, lparam);
        case WM_SYSCOMMAND:         return OnSysCommand(msg, wparam, lparam);

        case UWM_PREVIEWCLOSE:      return OnPreviewClose();
        case UWM_PREVIEWPRINT:      return OnPreviewPrint();
        case UWM_PREVIEWSETUP:      return OnPreviewSetup();
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

