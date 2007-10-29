///////////////////////////////
// View.cpp

#include "resource.h"
#include "View.h"


// Definitions for the CView class
void CView::OnCreate()
{
	// OnCreate is called automatically during window creation when a
	// WM_CREATE message received.

	// Tasks such as setting the icon, creating child windows, or other tasks 
	// associated with creating windows are normally performed here.

	// Set the window's icon
	SetIconSmall(IDW_MAIN);
	SetIconLarge(IDW_MAIN);

	m_MyTab.Create(m_hWnd);
	m_StaticWnd.CreateEx(0, "STATIC", "", WS_CHILD | WS_VISIBLE | WS_BORDER,
						0, 0, CW_USEDEFAULT, CW_USEDEFAULT, m_hWnd, NULL, NULL);

	TRACE("OnCreate");
}

void CView::OnDestroy()
{
	// Post a message to end the application
	::PostQuitMessage(0);
}

void CView::OnInitialUpdate()
{
	// OnInitialUpdate is called after the window is created.
	// Tasks which are to done after the window is created go here.

	TRACE("OnInitialUpdate");
}

void CView::OnPaint(HDC hDC)
{
	// OnPaint is called automatically whenever a part of the
	// window needs to be repainted.

	// Centre some text in our view window
	RECT r;
	::GetClientRect(m_hWnd, &r);
	::DrawText(hDC, LoadString(IDW_MAIN), -1, &r, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CView::PreCreate(CREATESTRUCT& cs)
{
	// This function will be called automaticly by Create. It provides an
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
	cs.lpszName = LoadString(IDW_MAIN);		// Window title
}

LRESULT CView::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// This function is our message procedure. We process the messages for
	// the view window here.  Unprocessed messages are passed on for 
	//  default processing.

	switch(uMsg)
	{
	case WM_DESTROY:
		OnDestroy();
		return 0;	// return a value. No default processing
	
	case WM_SIZE: 
		{
			HDWP hdwp;
			RECT rc;

			// Calculate the display rectangle, assuming the
			// tab control is the size of the client area.
			SetRect(&rc, 0, 0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			TabCtrl_AdjustRect(m_MyTab.GetHwnd(), FALSE, &rc);

			// Size the tab control to fit the client area.
			hdwp = BeginDeferWindowPos(2);
			DeferWindowPos(hdwp, m_MyTab.GetHwnd(), NULL, 0, 0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), SWP_NOMOVE | SWP_NOZORDER);

			// Position and size the static control to fit the
			// tab control's display area, and make sure the
			// static control is in front of the tab control.
			DeferWindowPos(hdwp, m_StaticWnd.GetHwnd(), HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, 0 );
			EndDeferWindowPos(hdwp);
		}
		break;

        case WM_NOTIFY:
			switch(((LPNMHDR) lParam)->code)
			{
                case TCN_SELCHANGE: 
				{
					int iPage = TabCtrl_GetCurSel(m_MyTab.GetHwnd());
					SendMessage(m_StaticWnd.GetHwnd(), WM_SETTEXT, 0, (LPARAM) LoadString(IDS_FIRSTDAY + iPage));
				}
				break;
            }
            break;
	}

	// pass unhandled messages on for default processing
	return WndProcDefault(hWnd, uMsg, wParam, lParam);	
}





