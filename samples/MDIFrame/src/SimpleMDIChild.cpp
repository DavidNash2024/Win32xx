//////////////////////////////////////////////
// SimpleMDIChild.cpp
//  Definitions for the CSimpleView and CSimpleMDIChild classes

#include "stdafx.h"
#include "SimpleMDIChild.h"
#include "resource.h"


// CSimpleView definitions
CSimpleView::CSimpleView() : m_Color(RGB(0,0,255))
{
}

void CSimpleView::OnDraw(CDC& dc)
{
	//Centre some text in our view window
	CRect rc = GetClientRect();
	dc.SetTextColor(m_Color);
	dc.DrawText(_T("View Window"), -1, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

LRESULT CSimpleView::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(uMsg);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	// Force the window to be repainted
	Invalidate();
	return 0L;
}

LRESULT CSimpleView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:	return OnSize(uMsg, wParam, lParam);
	}

	// Do default processing for other messages
	return WndProcDefault(uMsg, wParam, lParam);
}

// CSimpleMDIChild definitions
CSimpleMDIChild::CSimpleMDIChild()
{
    // Set m_View as the view window of the MDI child
	SetView(m_View);
	
	// Set the menu for this MDI child
	m_Menu.LoadMenu(_T("MdiMenuView"));
	SetHandles(m_Menu, NULL);
}

CSimpleMDIChild::~CSimpleMDIChild()
{
}

int CSimpleMDIChild::OnCreate(CREATESTRUCT& cs)
{
    // Set the window caption
	SetWindowText( _T("Simple Window") );
	
    // Set the window icons
    SetIconLarge(IDI_VIEW);
	SetIconSmall(IDI_VIEW);

	return CMDIChild::OnCreate(cs);
}

BOOL CSimpleMDIChild::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // Respond to menu and toolbar input

	UNREFERENCED_PARAMETER(lParam);

	UINT nID = LOWORD(wParam);
	switch (nID)
	{
	case IDM_COLOR_BLACK:
		m_View.SetColor(RGB(0,0,0));
		Invalidate();
		return TRUE;
	case IDM_COLOR_RED:
		m_View.SetColor(RGB(255, 0, 0));
		Invalidate();
		return TRUE;
	case IDM_COLOR_GREEN:
		m_View.SetColor(RGB(0, 255, 0));
		Invalidate();
		return TRUE;
	case IDM_COLOR_BLUE:
		m_View.SetColor(RGB(0, 0, 255));
		Invalidate();
		return TRUE;
	case IDM_COLOR_WHITE:
		m_View.SetColor(RGB(255, 255, 255));
		Invalidate();
		return TRUE;
	}

	return FALSE;
}

LRESULT CSimpleMDIChild::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//		Add case statements for each messages to be handled here
//	}

	// Do default processing for other messages
	return WndProcDefault(uMsg, wParam, lParam);
}
