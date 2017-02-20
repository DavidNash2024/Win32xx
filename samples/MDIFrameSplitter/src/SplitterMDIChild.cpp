//////////////////////////////////////////////
// SplitterMDIChild.cpp
//  Definitions for the CSimpleView and CSplitterMDIChild classes

#include "stdafx.h"
#include "SplitterMDIChild.h"
#include "Files.h"
#include "Output.h"
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

LRESULT CSimpleView::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam)
// Respond to a mouse click on the window
{
	// Set window focus. The docker will now report this as active.
	SetFocus();
	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CSimpleView::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(uMsg);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	Invalidate();	// Force the window to be repainted
	return 0L;
}
LRESULT CSimpleView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MOUSEACTIVATE:		return OnMouseActivate(uMsg, wParam, lParam);
	case WM_SIZE:				return OnSize(uMsg, wParam, lParam);
	}

	// Do default processing for other messages
	return WndProcDefault(uMsg, wParam, lParam);
}

// CSplitterMDIChild definitions
CSplitterMDIChild::CSplitterMDIChild()
{
    // Set m_View as the view window of the MDI child
	SetView(m_View);
	
	// Set the menu for this MDI child
	SetHandles(LoadMenu(GetApp().GetResourceHandle(), _T("MdiMenuView")), NULL);
}

CSplitterMDIChild::~CSplitterMDIChild()
{
}

int CSplitterMDIChild::OnCreate(CREATESTRUCT& cs)
{
    // Set the window caption
	SetWindowText( _T("Splitter Window") );
	
    // Set the window icons
    SetIconLarge(IDI_VIEW);
	SetIconSmall(IDI_VIEW);

	return CMDIChild::OnCreate(cs);
}

void CSplitterMDIChild::OnInitialUpdate()
{
	// Add Child dockers
	DWORD dwStyle = DS_CLIENTEDGE | DS_NO_UNDOCK;
	m_View.SetDockStyle(dwStyle);
	CDocker* pDockLeft  = m_View.AddDockedChild(new CDockOutput, DS_DOCKED_LEFT  | dwStyle, 200, 0);
	CDocker* pDockRight = m_View.AddDockedChild(new CDockOutput, DS_DOCKED_RIGHT | dwStyle, 200, 0);
	pDockLeft->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | dwStyle, 0, 0);
	pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | dwStyle, 0, 0);	
}

BOOL CSplitterMDIChild::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // Respond to menu and toolbar input

	UNREFERENCED_PARAMETER(lParam);

	UINT nID = LOWORD(wParam);
	switch (nID)
	{
	case IDM_COLOR_BLACK:	OnColor(RGB(0,0,0));		return TRUE;
	case IDM_COLOR_RED:		OnColor(RGB(255,0,0));		return TRUE;
	case IDM_COLOR_GREEN:	OnColor(RGB(0,255,0));		return TRUE;
	case IDM_COLOR_BLUE:	OnColor(RGB(0,0,255));		return TRUE;
	case IDM_COLOR_WHITE:	OnColor(RGB(255,255,255));	return TRUE;
	} 

	return FALSE;
}

void CSplitterMDIChild::OnColor(COLORREF rgb)
{
	m_View.GetSimpleView().SetColor(rgb);
	m_View.GetSimpleView().Invalidate();
}

LRESULT CSplitterMDIChild::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	m_View.RecalcDockLayout();

	// Pass the message on for default processing
	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CSplitterMDIChild::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:	return OnSize(uMsg, wParam, lParam);
	}

	// Do default processing for other messages
	return WndProcDefault(uMsg, wParam, lParam);
}
