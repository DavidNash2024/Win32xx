/////////////////////////////
// Miniframe.cpp
//

#include "stdafx.h"
#include <dwmapi.h>
#include <Uxtheme.h>
#include <vsstyle.h>
#include <vssym32.h>
#include "MiniFrame.h"
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


///////////////////////////////////
// CMiniframe function definitions.
//

// Draw the title bar background.
void CMiniFrame::DrawBackground(CDC& dc) const
{
    CRect titlebarRect = GetTitlebarRect();
    COLORREF titlebarColor = IsActive() ? m_colors.active : m_colors.inactive;
    CBrush titlebarBrush(titlebarColor);
    dc.FillRect(titlebarRect, titlebarBrush);
}

// Draw the title bar close button.
void CMiniFrame::DrawCloseButton(CDC& dc) const
{
    ButtonRects buttonRects = GetButtonRects();
    UINT dpi = ::GetDpiForWindow(*this);
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

// Draw the title bar maximize button.
void CMiniFrame::DrawMaximizeButton(CDC& dc) const
{
    ButtonRects buttonRects = GetButtonRects();
    UINT dpi = ::GetDpiForWindow(*this);
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

// Draw the title bar minimize button.
void CMiniFrame::DrawMinimizeButton(CDC& dc) const
{
    COLORREF itemColor = IsActive() ? m_colors.activeItem : m_colors.inactiveItem;
    CBrush buttonIconBrush(itemColor);
    ButtonRects buttonRects = GetButtonRects();
    UINT dpi = ::GetDpiForWindow(*this);
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

// Draw the title bar caption.
void CMiniFrame::DrawTitleText(CDC& dc) const
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
void CMiniFrame::DrawTopShadow(CDC& dc) const
{
    // Draw the top shadow. Original is missing because of the client rect extension.
    COLORREF titlebarColor = IsActive() ? m_colors.active : m_colors.inactive;
    COLORREF shadowColor = m_colors.topShadow;
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
void CMiniFrame::DrawWindowIcon(CDC& dc) const
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
ButtonRects CMiniFrame::GetButtonRects() const
{
    UINT dpi = ::GetDpiForWindow(*this);
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
TitlebarButton CMiniFrame::GetHoveredButton() const
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
CRect CMiniFrame::GetShadowRect() const
{
    const int shadowHeight = 1;
    CRect rect = GetClientRect();
    rect.bottom = rect.top + shadowHeight;
    return rect;
}

// Returns the rect for the titlebar.
// Adapted from:
// https://github.com/oberth/custom-chrome/blob/master/source/gui/window_helper.hpp#L52-L64
CRect CMiniFrame::GetTitlebarRect() const
{
    CSize barSize;
    const int borders = 2;
    HTHEME theme = ::OpenThemeData(*this, L"WINDOW");
    UINT dpi = ::GetDpiForWindow(*this);
    ::GetThemePartSize(theme, nullptr, WP_CAPTION, CS_ACTIVE, nullptr, THEMESIZE::TS_TRUE, &barSize);
    ::CloseThemeData(theme);
    int height = dpi_scale(barSize.cy, dpi) + borders;

    CRect rect = GetClientRect();
    rect.bottom = rect.top + height;
    return rect;
}

// Retrieves the rect of the view area.
CRect CMiniFrame::GetViewRect() const
{
    CRect rc = GetClientRect();
    rc.top = GetTitlebarRect().bottom;
    return rc;
}

// Returns true of the window is maximized, false otherwise.
bool CMiniFrame::IsMaximized() const
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
LRESULT CMiniFrame::OnActivate(UINT msg, WPARAM wparam, LPARAM lparam)
{
    RECT titlebarRect = GetTitlebarRect();
    InvalidateRect(titlebarRect, FALSE);
    return FinalWindowProc(msg, wparam, lparam);
}

// OnCommand responds to menu input.
BOOL CMiniFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);
    switch (id)
    {
 //   case IDM_FILE_OPEN:       return OnFileOpen();
 //   case IDM_FILE_SAVE:       return OnFileSave();
 //   case IDM_FILE_SAVEAS:     return OnFileSave();
 //   case IDM_FILE_PREVIEW:    return OnFilePreview();
 //   case IDM_FILE_PRINT:      return OnFilePrint();
    case IDM_FILE_EXIT:       return OnFileExit();
    case IDM_HELP_ABOUT:      return OnHelp();
    }

    return FALSE;
}

// OnCreate is called automatically during window creation when a
// WM_CREATE message received.
int CMiniFrame::OnCreate(CREATESTRUCT&)
{
    // Inform the application of the frame change to force redrawing with the new
    // client area that is extended into the title bar.
    SetWindowPos(HWND_TOP, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);

    // Set the window's icon.
    SetIconSmall(IDW_MAIN);
    SetIconLarge(IDW_MAIN);

    // Set the window title.
    SetWindowText(LoadString(IDW_MAIN));

    // Configure the menu bar.
    m_menubar.Create(*this);
    m_menu.LoadMenu(IDW_MAIN);
    m_menubar.SetupMenuBar(m_menu);
    m_view.Create(*this);
    m_accel = LoadAccelerators(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_MAIN));
    if (m_accel)
        GetApp()->SetAccelerators(m_accel, *this);

    // Position the window.
    CenterWindow();

    return 0;
}

// Called when the window is destroyed.
void CMiniFrame::OnDestroy()
{
    // End the application.
    ::PostQuitMessage(0);
}

// Called when the effective dots per inch (dpi) for a window has changed.
// This occurs when:
//  - The window is moved to a new monitor that has a different DPI.
//  - The DPI of the monitor hosting the window changes.
LRESULT CMiniFrame::OnDpiChanged(UINT, WPARAM, LPARAM lparam)
{
    LPRECT prc = reinterpret_cast<LPRECT>(lparam);
    SetWindowPos(HWND_TOP, *prc, SWP_SHOWWINDOW);

    m_menubar.SetupMenuBar(m_menu);
    RecalcLayout();

    return 0;
}

// Called when the window's background is drawn.
LRESULT CMiniFrame::OnEraseBkGnd(UINT, WPARAM, LPARAM)
{
    // Suppress background drawing to eliminate flicker.
    return 0;
}

// Issue a close request to the window to end the program.
BOOL CMiniFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Displays the About dialog when F1 is pressed or when selected from the menu.
BOOL CMiniFrame::OnHelp()
{
    // Ensure only one dialog displayed even for multiple hits of the F1 button.
    if (!m_aboutDialog.IsWindow())
    {
        m_aboutDialog.DoModal(*this);
    }

    return TRUE;
}

// Limit the minimum size of the window.
LRESULT CMiniFrame::OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam)
{
    LPMINMAXINFO lpMMI = (LPMINMAXINFO)lparam;
    lpMMI->ptMinTrackSize.x = DpiScaleInt(400);
    lpMMI->ptMinTrackSize.y = DpiScaleInt(300);

    return FinalWindowProc(msg, wparam, lparam);
}

// Handle WM_NCCALCSIZE to extend client (paintable) area into the title bar region.
LRESULT CMiniFrame::OnNCCalcSize(UINT, WPARAM wparam, LPARAM lparam)
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

// Returns the hot spot location of the cursor's position.
LRESULT CMiniFrame::OnNCHitTest(UINT msg, WPARAM wparam, LPARAM lparam)
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

// Handle left mouse button clicks in the non-client area.
LRESULT CMiniFrame::OnNCLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
   m_oldHoveredButton = m_hoveredButton;
   if ((m_hoveredButton != TitlebarButton::None) && (m_hoveredButton != TitlebarButton::System))
   {
        // remove default handling of the click to avoid it counting as drag.
       return 0;
   }

    // Default handling allows for dragging and double click to maximize.
    return FinalWindowProc(msg, wparam, lparam);
}

// Map button clicks to the correct messages for the window.
LRESULT CMiniFrame::OnNCLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam)
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

    return FinalWindowProc(msg, wparam, lparam);
}

// Update the hovered button when the mouse leaves the non-client area.
LRESULT CMiniFrame::OnNCMouseLeave(UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (m_hoveredButton != TitlebarButton::None)
    {
        RECT titlebarRect = GetTitlebarRect();
        InvalidateRect(titlebarRect, FALSE);
        m_hoveredButton = TitlebarButton::None;
    }

    return FinalWindowProc(msg, wparam, lparam);
}

// Track when mouse hovers each of the title bar buttons to draw the highlight correctly.
LRESULT CMiniFrame::OnNCMouseMove(UINT msg, WPARAM wparam, LPARAM lparam)
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

    return FinalWindowProc(msg, wparam, lparam);
}

// Display a system menu with a right mouse button click on the titlebar.
LRESULT CMiniFrame::OnNCRButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
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

    return FinalWindowProc(msg, wparam, lparam);
}

// Processes notification (WM_NOTIFY) messages from a child window.
LRESULT CMiniFrame::OnNotify(WPARAM, LPARAM lparam)
{
    LPNMHDR pHeader = (LPNMHDR)lparam;
    switch (pHeader->code)
    {
    case NM_CUSTOMDRAW:
        if (pHeader->hwndFrom == m_menubar.GetHwnd())
        {
            // Set the background color of the menubar.
            LPNMTBCUSTOMDRAW pCustomDraw = (LPNMTBCUSTOMDRAW)lparam;
            CRect rect = m_menubar.GetClientRect();
            COLORREF titlebarColor = IsActive() ? m_colors.active : m_colors.inactive;
            CBrush brush(titlebarColor);
            FillRect(pCustomDraw->nmcd.hdc, rect, brush);
        }

        return CDRF_DODEFAULT;   // Do default drawing.
    }

    return 0;
}

// Called when any part of the window is repainted.
LRESULT CMiniFrame::OnPaint(UINT, WPARAM, LPARAM)
{
    CPaintDC dc(*this);

    // Draw the title bar.
    DrawBackground(dc);
    DrawMinimizeButton(dc);
    DrawMaximizeButton(dc);
    DrawCloseButton(dc);
//    DrawTitleText(dc);
    DrawWindowIcon(dc);
    DrawTopShadow(dc);

    return 0;
}

// Called when the window is resized.
LRESULT CMiniFrame::OnSize(UINT msg, WPARAM wparam, LPARAM lparam)
{
    RecalcLayout();
    return FinalWindowProc(msg, wparam, lparam);
}

// Called in response to system command.
LRESULT CMiniFrame::OnSysCommand(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Pass menu accelerator keys to the menubar.
    if ((SC_KEYMENU == wparam) && (VK_SPACE != lparam) && m_menubar.IsWindow())
    {
        m_menubar.SysCommand(msg, wparam, lparam);
        return 0;
    }
    return FinalWindowProc(msg, wparam, lparam);
}

// Called before the window is created to set the CREATESTRUCT parameters.
void CMiniFrame::PreCreate(CREATESTRUCT& cs)
{
    // Set some optional parameters for the window
    cs.lpszClass = _T("MiniFrame Window");  // Window Class
    cs.x = DpiScaleInt(50);                 // top x
    cs.y = DpiScaleInt(50);                 // top y
    cs.cx = DpiScaleInt(400);               // width
    cs.cy = DpiScaleInt(300);               // height
}

// Repositions the child windows.
void CMiniFrame::RecalcLayout() const
{
    // Redraw the title bar.
    CRect rect = GetTitlebarRect();
    InvalidateRect(rect, FALSE);

    // Position the menu bar.
    if (m_menubar.IsWindow())
    {
        CRect menuRect = GetClientRect();
        CSize size = m_menubar.GetMaxSize();
        menuRect.left = GetButtonRects().system.right;
        menuRect.top = (rect.Height() - size.cy) / 2;
        menuRect.right = menuRect.left + size.cx;
        menuRect.bottom = menuRect.top + size.cy;
        m_menubar.SetWindowPos(HWND_TOP, menuRect, SWP_SHOWWINDOW);
    }

    // Position the view window.
    if (m_view.IsWindow())
    {
        rect = GetViewRect();
        m_view.SetWindowPos(HWND_TOP, rect, SWP_SHOWWINDOW);
    }

}

// Process the window's messages
LRESULT CMiniFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_ACTIVATE:           return OnActivate(msg, wparam, lparam);
        case WM_DPICHANGED:         return OnDpiChanged(msg, wparam, lparam);
        case WM_ERASEBKGND:         return OnEraseBkGnd(msg, wparam, lparam);
        case WM_GETMINMAXINFO:      return OnGetMinMaxInfo(msg, wparam, lparam);
        case WM_HELP:               return OnHelp();
        case WM_NCMOUSELEAVE:       return OnNCMouseLeave(msg, wparam, lparam);
        case WM_NCCALCSIZE:         return OnNCCalcSize(msg, wparam, lparam);
        case WM_NCHITTEST:          return OnNCHitTest(msg, wparam, lparam);
        case WM_NCLBUTTONDOWN:      return OnNCLButtonDown(msg, wparam, lparam);
        case WM_NCLBUTTONUP:        return OnNCLButtonUp(msg, wparam, lparam);
        case WM_NCMOUSEMOVE:        return OnNCMouseMove(msg, wparam, lparam);
        case WM_NCRBUTTONDOWN:      return OnNCRButtonDown(msg, wparam, lparam);
        case WM_PAINT:              return OnPaint(msg, wparam, lparam);
        case WM_SIZE:               return OnSize(msg, wparam, lparam);
        case WM_SYSCOMMAND:         return OnSysCommand(msg, wparam, lparam);
        }

        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << _T("\n") << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(NULL, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(NULL, str1, _T("Error: std::exception"), MB_ICONERROR);
    }

    return 0;
}
