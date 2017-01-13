///////////////////////////////
// View.cpp

#include "View.h"
#include "resource.h"


// Definitions for the CView class
CView::CView()
{
	m_Brush.CreateSolidBrush( RGB(250, 230, 100) );
}

BOOL CView::OnColor()
{
	static COLORREF CustColors[16] = {0};	// array of custom colors
	CHOOSECOLOR cc;							// Structure used by ChooseColor
	ZeroMemory(&cc, sizeof(cc));

	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	cc.rgbResult = m_Brush.GetLogBrush().lbColor;
	cc.lpCustColors = CustColors;
	cc.hwndOwner = *this;
	
	// Initiate the Choose Color dialog
	if (ChooseColor(&cc)==TRUE) 
	{
		m_Brush = CreateSolidBrush(cc.rgbResult);
		SetClassLongPtr(GCLP_HBRBACKGROUND, (LONG_PTR)m_Brush.GetHandle());
		Invalidate();
	}

	return TRUE;
}

BOOL CView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (LOWORD(wParam))
	{
	case IDM_COLOR:		return OnColor();
	case IDM_EXIT:		return OnExit();
	}

	// return FALSE for unhandled commands
	return FALSE;
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

	// Set the window title
	SetWindowText(LoadString(IDW_MAIN));

	// Create a circular region
	CRgn rgn;
	m_Rect = CRect(50, 50, 300, 300);
	rgn.CreateEllipticRgnIndirect(m_Rect);

	// assign the region to the window
	SetWindowRgn(rgn, FALSE);

	TRACE("OnCreate\n");
	return 0;
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
	CRect rc = m_Rect;
	rc.OffsetRect(0, -GetSystemMetrics(SM_CYCAPTION) );
	CString cs = LoadString(IDW_MAIN);
	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(cs, cs.GetLength(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

BOOL CView::OnExit()
{
	// Post a WM_CLOSE message to end the application
	PostMessage(WM_CLOSE);

	return TRUE;
}

void CView::OnInitialUpdate()
{
	// OnInitialUpdate is called after the window is created.
	// Tasks which are to be done after the window is created go here.

	CenterWindow();
	TRACE("OnInitialUpdate\n");
}

void CView::PreCreate(CREATESTRUCT& cs)
{
	// This function will be called automatically by Create. It provides an
	// opportunity to set various window parameters prior to window creation.
	// You are not required to set these parameters, any paramters which
	// aren't specified are set to reasonable defaults.

	// Set some optional parameters for the window
	cs.x = 50;						// top x
	cs.y = 50;						// top y
	cs.cx = 400;					// width
	cs.cy = 350;					// height
	cs.style = WS_VISIBLE;			// Window is initially visible
}

void CView::PreRegisterClass(WNDCLASS& wc)
{
	// This function will be called automatically by Create.
	// Here we set the window's class paramaters.

	wc.hbrBackground = m_Brush;					// Background color
	wc.lpszClassName = _T("Round Window");		// Class Name
	wc.hCursor = ::LoadCursor(NULL,IDC_ARROW);	// Arrow cursor
}

LRESULT CView::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
// Respond to a left mouse button press
{
	SetCapture();
	
	// Save the relative mouse position
	m_Point = GetCursorPos();
	ScreenToClient(m_Point);

	// Adjust client co-ords to window co-ords.
	m_Point.x += GetSystemMetrics(SM_CXFIXEDFRAME);
	m_Point.y += GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFIXEDFRAME);

	// Pass this message on for default processing
	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CView::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam)
// Respond to a left mouse button release
{
	ReleaseCapture();

	// Pass this message on for default processing
	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CView::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
// Respond to a mouse move
{
	if (wParam & MK_LBUTTON)
	{
		CPoint pt = GetCursorPos();

		int x = pt.x - m_Point.x;
		int y = pt.y - m_Point.y;

		// Move the window
		SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE);
	}

	// Pass this message on for default processing
	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CView::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
// Respond to Right mouse button press
{
	// Create the menu
	CMenu Popup;
	Popup.CreatePopupMenu();
	Popup.AppendMenu(MF_BYPOSITION | MF_STRING, IDM_COLOR, _T("Color"));
	Popup.AppendMenu(MF_BYPOSITION | MF_STRING, IDM_EXIT, _T("Exit"));
	
	// Initiate the popup menu
	CPoint pt = GetCursorPos();
	Popup.TrackPopupMenu(TPM_TOPALIGN | TPM_LEFTALIGN, pt.x, pt.y, GetHwnd());

	// Pass this message on for default processing
	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// This function is our message procedure. We process the messages for
	// the view window here.  Unprocessed messages are passed on for
	//  default processing.

	switch(uMsg)
	{
		case WM_LBUTTONDOWN:	return OnLButtonDown(uMsg, wParam, lParam);
		case WM_LBUTTONUP:		return OnLButtonUp(uMsg, wParam, lParam);
		case WM_MOUSEMOVE:		return OnMouseMove(uMsg, wParam, lParam);
		case WM_RBUTTONDOWN:	return OnRButtonDown(uMsg, wParam, lParam);
	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}
