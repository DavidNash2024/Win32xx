//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

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

	// Set the Window Class name
	cs.lpszClass = _T("Edit");

	// Set the extended style
//	cs.dwExStyle = WS_EX_CLIENTEDGE;
}

LRESULT CView::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MOUSEACTIVATE:
		TRACE("WM_MOUSEACTIVATE\n");
		SetFocus();
		break;
	case WM_SIZE:
		::InvalidateRect(hWnd, NULL, TRUE);
		break;	// Also do default processing
	}

	// pass unhandled messages on for default processing
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

void CView::OnPaint(HDC hDC)
{
	CRect rc = GetClientRect();

	// Centre some text in our view window
	::DrawText(hDC, _T("View Window"), -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

