///////////////////////////////
// View.cpp

#include "stdafx.h"
#include "View.h"
#include "resource.h"
#include "RibbonUI.h"

// Definitions for the CView class

void CView::OnCreate()
{
	// OnCreate is called automatically during window creation when a
	// WM_CREATE message received.

	// Tasks such as setting the icon, creating child windows, or anything
	// associated with creating windows are normally performed here.

	TRACE(_T("CView::OnCreate\n"));
	
}

void CView::OnInitialUpdate()
{
	// OnInitialUpdate is called after the window is created.
	// Tasks which are to be done after the window is created go here.

	TRACE(_T("CView::OnInitialUpdate\n"));
}

void CView::OnPaint(HDC hDC)
{
	// OnPaint is called automatically whenever a part of the
	// window needs to be repainted.

	// Centre some text in our view window
	CRect r = GetClientRect();
	::DrawText(hDC, _T("Simple Ribon Demo"), -1, &r, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CView::OnSize()
{
	CRect r = GetClientRect();
	InvalidateRect(&r, FALSE);
}

void CView::PreCreate(CREATESTRUCT& cs)
{
	// This function will be called automatically by Create. It provides an
	// opportunity to set various window parameters prior to window creation.
	// You are not required to set these parameters, any paramters which
	// aren't specified are set to reasonable defaults.

	// Set some optional parameters for the window
	cs.dwExStyle = WS_EX_CLIENTEDGE;		// Extended style
}

LRESULT CView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_WINDOWPOSCHANGING:
//	case WM_SIZE:
		OnSize();
		break;	// and also do default processing for this message
	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

