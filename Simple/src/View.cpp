///////////////////////////////
// View.cpp

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

	// Set the window title
	SetWindowText(CResString(IDW_MAIN));

	TRACE(_T("OnCreate\n"));
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
	CRect r = GetClientRect();
	dc.DrawText(CResString(IDW_MAIN), -1, r, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
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
}

void CView::OnSize()
{
	// Force the window to be repainted during resizing
	Invalidate();
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
		return 0;	// return a value. No default processing

	case WM_SIZE:
		OnSize();
		break;	// and also do default processing for this message
	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}
