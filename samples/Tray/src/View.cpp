///////////////////////////////
// View.cpp
//

#include "stdafx.h"
#include "View.h"
#include "resource.h"

//////////////////////////////
// CView function definitions.
//
BOOL CView::Minimize()
{
    NOTIFYICONDATA nid;
    ZeroMemory(&nid, sizeof(nid));
    nid.cbSize = sizeof(nid);
    nid.hWnd = GetHwnd();
    nid.uID = IDW_MAIN;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = MSG_TRAYICON;
    nid.hIcon = GetApp()->LoadIcon(IDW_MAIN);

    StrCopy(nid.szTip, _T("Tray Demo tooltip"), 128);

    Shell_NotifyIcon(NIM_ADD, &nid);
    ShowWindow(SW_HIDE);
    m_isMinimized = true;

    return TRUE;
}

BOOL CView::OnAbout()
{
    CString str = _T("Tray Example: Demonstrates minimizing a window to the tray.");
    MessageBox(str, _T("About Tray Example"), MB_OK | MB_ICONINFORMATION);
    return TRUE;
}

int CView::OnCreate(CREATESTRUCT&)
{
    // OnCreate is called automatically during window creation when a
    // WM_CREATE message received.

    // Tasks such as setting the icon, creating child windows, or anything
    // associated with creating windows are normally performed here.

    // Set the window's icon
    SetIconSmall(IDW_MAIN);
    SetIconLarge(IDW_MAIN);

    SetWindowText(LoadString(IDW_MAIN));        // Window title

    // Load the accelerator table
 //   HACCEL accel = LoadAccelerators(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_MAIN));
 //   if (accel)
 //       GetApp()->SetAccelerators(accel, *this);

    TRACE("OnCreate\n");

    return 0;
}

BOOL CView::OnCommand(WPARAM wparam, LPARAM)
{
    // OnCommand responds to menu and and toolbar input

    UINT id = LOWORD(wparam);

    switch(id)
    {
    case IDM_MINTOTRAY:     return Minimize();
    case IDM_FILE_EXIT:     return OnFileExit();
    case IDM_HELP_ABOUT:    return OnAbout();
    }

    return FALSE;
}

void CView::OnDestroy()
{
    // End the application when the window is destroyed
    ::PostQuitMessage(0);
}

LRESULT CView::OnDPIChanged(UINT, WPARAM, LPARAM lparam)
{
    LPRECT prc = reinterpret_cast<LPRECT>(lparam);
    SetWindowPos(0, *prc, SWP_SHOWWINDOW);

    return 0;
}

void CView::OnDraw(CDC& dc)
{
    // OnPaint is called automatically whenever a part of the
    // window needs to be repainted.

    // Use the message font for Windows 7 and higher.
    if (GetWinVersion() >= 2601)
    {
        NONCLIENTMETRICS info = GetNonClientMetrics();
        LOGFONT lf = DPIScaleLogfont(info.lfMessageFont, 10);
        dc.CreateFontIndirect(lf);
    }

    // Centre some text in our view window
    CRect rc = GetClientRect();
    CString text = LoadString(IDS_DRAWTEXT);
    dc.DrawText(text, text.GetLength(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

BOOL CView::OnFileExit()
{
    // End the application
    Close();
    return TRUE;
}

void CView::OnInitialUpdate()
{
    // OnInitialUpdate is called after the window is created.
    // Tasks which are to be done after the window is created go here.

    TRACE("OnInitialUpdate\n");
}

LRESULT CView::OnSize(UINT, WPARAM, LPARAM)
{
    // Force the window to be repainted during resizing
    Invalidate();
    return 0;
}

LRESULT CView::OnSysCommand(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Maximize and Minimuze requests end up here

    if (wparam == SC_MINIMIZE)  // User pressed minimize button
    {
        Minimize();
        return 0;
    }

    return FinalWindowProc(msg, wparam, lparam);
}

LRESULT CView::OnTrayIcon(UINT, WPARAM wparam, LPARAM lparam)
{
    // For a NOTIFYICONDATA with uVersion= 0, wparam and lparam have the following values:
    // The wparam parameter contains the identifier of the taskbar icon in which the event occurred.
    // The lparam parameter holds the mouse or keyboard message associated with the event.

    if (wparam != IDW_MAIN)
        return 0;

    if (lparam == WM_LBUTTONUP)
    {
        Restore();
    }
    else if (lparam == WM_RBUTTONUP)
    {
        CMenu topMenu(IDM_MINIMIZED);
        CMenu subMenu = topMenu.GetSubMenu(0);

        SetForegroundWindow();
        CPoint pos = GetCursorPos();
        UINT uSelected = subMenu.TrackPopupMenu(TPM_RETURNCMD | TPM_NONOTIFY, pos.x, pos.y, *this, NULL);

        switch (uSelected)
        {
        case IDM_MIN_RESTORE: Restore(); break;
        case IDM_MIN_ABOUT:   OnAbout(); break;
        case IDM_MIN_EXIT:    Destroy(); break;
        }
    }

    return 0;
}

void CView::PreCreate(CREATESTRUCT& cs)
{
    // This function will be called automatically by Create. It provides an
    // opportunity to set various window parameters prior to window creation.
    // You are not required to set these parameters, any parameters which
    // aren't specified are set to reasonable defaults.

    // Set some optional parameters for the window
    cs.dwExStyle = WS_EX_CLIENTEDGE;        // Extended style
    cs.lpszClass = _T("View Window");       // Window Class
    cs.x = DPIScaleInt(50);                 // top x
    cs.y = DPIScaleInt(50);                 // top y
    cs.cx = DPIScaleInt(400);               // width
    cs.cy = DPIScaleInt(300);               // height
    cs.hMenu = m_menu;
}

void CView::Restore()
{
    NOTIFYICONDATA nid;
    ZeroMemory(&nid, sizeof(nid));
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = GetHwnd();
    nid.uID = IDW_MAIN;
    Shell_NotifyIcon(NIM_DELETE, &nid);
    ShowWindow(SW_SHOW);
    m_isMinimized = false;
}

LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // This function is our message procedure. We process the messages for
    // the view window here.  Unprocessed messages are passed on for
    //  default processing.

    try
    {
        switch (msg)
        {
        case WM_DPICHANGED: return OnDPIChanged(msg, wparam, lparam);
        case WM_HELP:       return OnAbout();
        case WM_SIZE:       return OnSize(msg, wparam, lparam);
        case WM_SYSCOMMAND: return OnSysCommand(msg, wparam, lparam);
        case MSG_TRAYICON:  return OnTrayIcon(msg, wparam, lparam);
        }

        // Pass unhandled messages on for default processing.
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
