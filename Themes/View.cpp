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
	TRACE("View window created");
}

void CView::PreCreate(CREATESTRUCT &cs)
{
	// Here we set the defaults used by the create function for the view window
	// Preforming this is optional, but doing so allows us to
	// take more precise control over the window we create.

	// Set the Window Class name
	cs.lpszClass = _T("View");

	// Set the extended style
	cs.dwExStyle = WS_EX_CLIENTEDGE;
}

LRESULT CView::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		::InvalidateRect(hWnd, NULL, TRUE);
		break;	// Also do default processing
	}

	// pass unhandled messages on for default processing
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

void CView::OnPaint(HDC hDC)
{
	RECT r;
	::GetClientRect(m_hWnd, &r);

	// Centre some text in our view window
	::DrawText(hDC, _T("Choose a theme from the menu"), -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

