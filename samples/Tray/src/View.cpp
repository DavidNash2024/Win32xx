///////////////////////////////
// View.cpp

#include "stdafx.h"
#include "View.h"
#include "resource.h"


// Definitions for the CView class
void CView::Minimize()
{
    NOTIFYICONDATA nid = { 0 };
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = GetHwnd();
    nid.uID = IDW_MAIN;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = MSG_TRAYICON;
	nid.hIcon = (HICON) (::LoadImage (GetModuleHandle(NULL), MAKEINTRESOURCE (IDW_MAIN), IMAGE_ICON,
		::GetSystemMetrics (SM_CXSMICON), ::GetSystemMetrics (SM_CYSMICON), 0));

	lstrcpy(nid.szTip, _T("Tray Demo tooltip"));

    Shell_NotifyIcon(NIM_ADD, &nid);
    ShowWindow(SW_HIDE);
    m_IsMinimized = true;
}

void CView::OnAbout()
{
	MessageBox(_T("Tray Example: Demonstrates minimizing a window to the tray."), _T("About Tray Example"), MB_OK | MB_ICONINFORMATION);
}

int CView::OnCreate(LPCREATESTRUCT pcs)
{
	// OnCreate is called automatically during window creation when a
	// WM_CREATE message received.

	// Tasks such as setting the icon, creating child windows, or anything
	// associated with creating windows are normally performed here.

	UNREFERENCED_PARAMETER(pcs);

	// Set the window's icon
	SetIconSmall(IDW_MAIN);
	SetIconLarge(IDW_MAIN);

	SetWindowText(LoadString(IDW_MAIN).c_str());		// Window title

	TRACE("OnCreate\n");

	return 0;
}

BOOL CView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// OnCommand responds to menu and and toolbar input

	UNREFERENCED_PARAMETER(lParam);

	switch(LOWORD(wParam))
	{
	case IDM_MINTOTRAY:		Minimize();		return TRUE;
	case IDM_FILE_EXIT:		OnFileExit();	return TRUE;
	case IDM_HELP_ABOUT:	OnAbout();		return TRUE;
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
	CString cs = LoadString(IDW_MAIN);
	dc.DrawText(cs, cs.GetLength(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
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

LRESULT CView::OnSize()
{
	// Force the window to be repainted during resizing
	Invalidate();
	return 0L;
}

LRESULT CView::OnSysCommand(WPARAM wParam, LPARAM lParam)
{
	// Maximize and Minimuze requests end up here

	if (wParam == SC_MINIMIZE)	// User pressed minimize button
	{
		Minimize();
		return 0L;
	}

	return FinalWindowProc(WM_SYSCOMMAND, wParam, lParam);
}	

LRESULT CView::OnTrayIcon(WPARAM wParam, LPARAM lParam)
{
	// For a NOTIFYICONDATA with uVersion= 0, wParam and lParam have the following values:
	// The wParam parameter contains the identifier of the taskbar icon in which the event occurred.
	// The lParam parameter holds the mouse or keyboard message associated with the event.
    if (wParam != IDW_MAIN)
		return 0L;

	if (lParam == WM_LBUTTONUP)
    {
        Restore();
    }
    else if (lParam == WM_RBUTTONUP)
    {
		CMenu TopMenu(IDM_MINIMIZED);
		CMenu SubMenu = TopMenu.GetSubMenu(0);

        SetForegroundWindow();
		CPoint pt = GetCursorPos();
		UINT uSelected = SubMenu.TrackPopupMenu(TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, *this, NULL);

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
	cs.dwExStyle = WS_EX_CLIENTEDGE;		// Extended style
	cs.lpszClass = _T("View Window");		// Window Class
	cs.x = 50;								// top x
	cs.y = 50;								// top y
	cs.cx = 400;							// width
	cs.cy = 300;							// height
	cs.hMenu =  LoadMenu(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_MAIN));
}

void CView::Restore()
{
    NOTIFYICONDATA nid = { 0 };
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = GetHwnd();
    nid.uID = IDW_MAIN;
    Shell_NotifyIcon(NIM_DELETE, &nid);
    ShowWindow(SW_SHOW);
    m_IsMinimized = false;
}

LRESULT CView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// This function is our message procedure. We process the messages for
	// the view window here.  Unprocessed messages are passed on for
	//  default processing.

	switch(uMsg)
	{
	case WM_SIZE:       return OnSize();
	case WM_SYSCOMMAND: return OnSysCommand(wParam, lParam);
	case MSG_TRAYICON:  return OnTrayIcon(wParam, lParam);
	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}
