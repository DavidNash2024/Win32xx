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

	TRACE("OnCreate");
}

void CView::OnDestroy()
{
	// Post a message to end the application
	::PostQuitMessage(0);
}

void CView::OnInitialUpdate()
{
	// OnInitial is called after the window is created.
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

void CView::OnSize()
{
	// Force the window to be repainted during resizing
	::InvalidateRect(m_hWnd, NULL, TRUE);
}

LRESULT CView::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// This function is our message procedure. We process the messages for
	// the view window here, and pass the unprocessed messages to
	// CWnd::WndProc for default processing.

	switch(uMsg)
	{
	case WM_DESTROY:
		OnDestroy();
		break;	// and also do default processing for this message

	case WM_SIZE:
		OnSize();
		break;	// and also do default processing for this message
	}

	return 0L;	// Return zero to pass unhandled on for default processing
}
