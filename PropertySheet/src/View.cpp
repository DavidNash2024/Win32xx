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
	TRACE(_T("View Window Created\n"));
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

void CView::OnPaint(HDC hDC)
{
	CRect r = GetClientRect();

	// Centre some text in our view window
	::DrawText(hDC, _T("Choose a Property Sheet from the menu"), -1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

LRESULT CView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		::InvalidateRect(m_hWnd, NULL, TRUE);
		break;	// Also do default processing
	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}
