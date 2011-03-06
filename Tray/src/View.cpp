///////////////////////////////
// View.cpp

#include "stdafx.h"
#include "View.h"
#include "resource.h"


// Definitions for the CView class
void CView::OnCreate()
{
	// OnCreate is called automatically during window creation when a
	// WM_CREATE message received.

	// Tasks such as setting the icon, creating child windows, or anything
	// associated with creating windows are normally performed here.

	// Set the window's icon
	SetIconSmall(IDW_MAIN);
	SetIconLarge(IDW_MAIN);

	SetWindowText(CLoadString(IDW_MAIN));		// Window title

	TRACE(_T("OnCreate\n"));
}

void CView::OnAbout()
{
	MessageBox(_T("Tray Example: Demonstrates minimizing a window to the tray."), _T("About Tray Example"), MB_OK | MB_ICONINFORMATION);
}

BOOL CView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// OnCommand responds to menu and and toolbar input

	UNREFERENCED_PARAMETER(lParam);

	switch(LOWORD(wParam))
	{
	case IDM_MINTOTRAY:
		Minimize();
		return TRUE;
	case IDM_FILE_EXIT:
		// End the application
		PostQuitMessage(0);
		return TRUE;
	case IDM_HELP_ABOUT:
		OnAbout();
		return TRUE;
	}

	return FALSE;
}

void CView::OnDestroy()
{
	// End the application when the window is destroyed
	::PostQuitMessage(0);
}

void CView::OnInitialUpdate()
{
	// OnInitialUpdate is called after the window is created.
	// Tasks which are to be done after the window is created go here.

	TRACE(_T("OnInitialUpdate\n"));
}

void CView::OnPaint(CDC& dc)
{
	// OnPaint is called automatically whenever a part of the
	// window needs to be repainted.

	// Centre some text in our view window
	CRect rc = GetClientRect();
	dc.DrawText(CLoadString(IDW_MAIN), -1, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CView::OnTrayIcon(WPARAM wParam, LPARAM lParam)
{
	// For a NOTIFYICONDATA with uVersion= 0, wParam and lParam have the following values:
	// The wParam parameter contains the identifier of the taskbar icon in which the event occurred.
	// The lParam parameter holds the mouse or keyboard message associated with the event.
    if (wParam != IDW_MAIN)
		return;
    
	if (lParam == WM_LBUTTONUP) 
    {
        Restore();
    }
    else if (lParam == WM_RBUTTONUP) 
    {
		HMENU hTopMenu = LoadMenu(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDM_MINIMIZED));
		HMENU hSubMenu = GetSubMenu(hTopMenu, 0);
        
        SetForegroundWindow();
		CPoint pt = GetCursorPos();
		UINT uSelected = TrackPopupMenu(hSubMenu, TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, 0, m_hWnd, NULL); 

		switch (uSelected)
		{
		case IDM_MIN_RESTORE:
			Restore();
			break;
		case IDM_MIN_ABOUT:
			OnAbout();
			break;
		case IDM_MIN_EXIT:
			Destroy();
			break;
		}
    }
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

void CView::OnSize()
{
	// Force the window to be repainted during resizing
	Invalidate();
}

void CView::Minimize()
{
    NOTIFYICONDATA nid = { 0 };
    nid.cbSize = sizeof(NOTIFYICONDATA); 
    nid.hWnd = m_hWnd; 
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

void CView::Restore()
{
    NOTIFYICONDATA nid = { 0 };
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = m_hWnd;
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
	case WM_DESTROY:
		OnDestroy();
		return 0;
	case WM_SIZE:
		OnSize();
		break;	
	case WM_SYSCOMMAND:
		if (wParam == SC_MINIMIZE)	// User pressed minimize button
		{
			Minimize();
			return 0;
		}
		break;
	case MSG_TRAYICON:
		OnTrayIcon(wParam, lParam);
		break;
	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}
