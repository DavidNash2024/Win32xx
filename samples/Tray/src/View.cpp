///////////////////////////////
// View.cpp

#include "stdafx.h"
#include "View.h"
#include "resource.h"


// Definitions for the CView class
void CView::Minimize()
{
    NOTIFYICONDATA nid;
    ZeroMemory(&nid, sizeof(nid));
    nid.cbSize = sizeof(nid);
    nid.hWnd = GetHwnd();
    nid.uID = IDW_MAIN;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = MSG_TRAYICON;
    nid.hIcon = (HICON) (::LoadImage (GetModuleHandle(NULL), MAKEINTRESOURCE (IDW_MAIN), IMAGE_ICON,
        ::GetSystemMetrics (SM_CXSMICON), ::GetSystemMetrics (SM_CYSMICON), 0));

    StrCopy(nid.szTip, _T("Tray Demo tooltip"), 128);

    Shell_NotifyIcon(NIM_ADD, &nid);
    ShowWindow(SW_HIDE);
    m_isMinimized = true;
}

void CView::OnAbout()
{
    CString str = _T("Tray Example: Demonstrates minimizing a window to the tray.");
    MessageBox(str, _T("About Tray Example"), MB_OK | MB_ICONINFORMATION);
}

int CView::OnCreate(CREATESTRUCT& cs)
{
    // OnCreate is called automatically during window creation when a
    // WM_CREATE message received.

    // Tasks such as setting the icon, creating child windows, or anything
    // associated with creating windows are normally performed here.

    UNREFERENCED_PARAMETER(cs);

    // Set the window's icon
    SetIconSmall(IDW_MAIN);
    SetIconLarge(IDW_MAIN);

    SetWindowText(LoadString(IDW_MAIN).c_str());        // Window title

    TRACE("OnCreate\n");

    return 0;
}

BOOL CView::OnCommand(WPARAM wparam, LPARAM lparam)
{
    // OnCommand responds to menu and and toolbar input

    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_MINTOTRAY:     Minimize();     return TRUE;
    case IDM_FILE_EXIT:     OnFileExit();   return TRUE;
    case IDM_HELP_ABOUT:    OnAbout();      return TRUE;
    }

    return FALSE;
}

void CView::OnDestroy()
{
    // End the application when the window is destroyed
    ::PostQuitMessage(0);
}

void CView::OnDraw(CDC& dc)
{
    // OnPaint is called automatically whenever a part of the
    // window needs to be repainted.

    // Centre some text in our view window
    CRect rc = GetClientRect();
    CString text = LoadString(IDS_DRAWTEXT);
    dc.DrawText(text, text.GetLength(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CView::OnFileExit()
{
    // End the application
    ::PostQuitMessage(0);
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

LRESULT CView::OnTrayIcon(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // For a NOTIFYICONDATA with uVersion= 0, wparam and lparam have the following values:
    // The wparam parameter contains the identifier of the taskbar icon in which the event occurred.
    // The lparam parameter holds the mouse or keyboard message associated with the event.
    UNREFERENCED_PARAMETER(msg);
    
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
    // You are not required to set these parameters, any paramters which
    // aren't specified are set to reasonable defaults.

    // Set some optional parameters for the window
    cs.dwExStyle = WS_EX_CLIENTEDGE;        // Extended style
    cs.lpszClass = _T("View Window");       // Window Class
    cs.x = 50;                              // top x
    cs.y = 50;                              // top y
    cs.cx = 400;                            // width
    cs.cy = 300;                            // height
    cs.hMenu =  LoadMenu(GetApp().GetResourceHandle(), MAKEINTRESOURCE(IDW_MAIN));
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

    switch(msg)
    {
    case WM_SIZE:       return OnSize(msg, wparam, lparam);
    case WM_SYSCOMMAND: return OnSysCommand(msg, wparam, lparam);
    case MSG_TRAYICON:  return OnTrayIcon(msg, wparam, lparam);
    }

    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}
