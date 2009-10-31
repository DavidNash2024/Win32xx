//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "stdafx.h"
#include "view.h"


CView::CView()
{
}

void CView::OnInitialUpdate()
{
	// OnInitialUpdate is called immediately after the window is created
	TRACE(_T("View window created\n"));
}

void CView::PreCreate(CREATESTRUCT &cs)
{
	// Here we set the defaults used by the create function for the view window
	// Preforming this is optional, but doing so allows us to
	// take more precise control over the window we create.

	// Set the extended style
	cs.dwExStyle = WS_EX_CLIENTEDGE;
}

void CView::RegisterClass(WNDCLASS &wc)
{
	// Here we set the Window class parameters.
	// Preforming this is optional, but doing so allows us to
	// take more precise control over the type of window we create.

	// Set the Window Class name
	wc.lpszClassName = _T("View");

	// Set the class style (not to be confused with the window styles set in PreCreate)
	wc.style = CS_DBLCLKS;	// Generate left button double click messages
}

LRESULT CView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		::InvalidateRect(m_hWnd, NULL, TRUE);
		break;	// Also do default processing

	case WM_LBUTTONDBLCLK:
		TRACE("WM_LBUTTONDBLCLK\n");
		break;

	case WM_LBUTTONDOWN:
		TRACE("WM_LBUTTONDOWN\n");
		break;

	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

void CView::OnPaint(HDC hDC)
{
	CRect rc = GetClientRect();

	// Centre some text in our view window
	::DrawText(hDC, _T("View Window"), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

