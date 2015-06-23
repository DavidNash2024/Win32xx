//////////////////////////////////////////////
// SimpleMDIChild.cpp
//  Definitions for the CSimpleView and CSimpleMDIChild classes

#include "stdafx.h"
#include "Files.h"
#include "Output.h"
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

	Invalidate();	// Force the window to be repainted
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
	SetHandles(LoadMenu(GetApp().GetResourceHandle(), _T("MdiMenuView")), NULL);
}

CSimpleMDIChild::~CSimpleMDIChild()
{
}

int CSimpleMDIChild::OnCreate(LPCREATESTRUCT pcs)
{
    // Set the window caption
	SetWindowText( _T("Simple Window") );
	
    // Set the window icons
    SetIconLarge(IDI_VIEW);
	SetIconSmall(IDI_VIEW);

	return CMDIChild::OnCreate(pcs);
}

void CSimpleMDIChild::OnInitialUpdate()
{
	// Add Child dockers
	DWORD dwStyle = DS_CLIENTEDGE | DS_NO_UNDOCK;
	m_View.SetDockStyle(dwStyle);
	CDocker* pDockLeft  = m_View.AddDockedChild(new CDockOutput, DS_DOCKED_LEFT  | dwStyle, 200, 0);
	CDocker* pDockRight = m_View.AddDockedChild(new CDockOutput, DS_DOCKED_RIGHT | dwStyle, 200, 0);
	pDockLeft->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | dwStyle, 0, 0);
	pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | dwStyle, 0, 0);	
}

BOOL CSimpleMDIChild::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // Respond to menu and toolbar input

	UNREFERENCED_PARAMETER(lParam);

	switch (LOWORD(wParam))
	{
	case IDM_COLOR_BLACK:	OnColor(RGB(0,0,0));		return TRUE;
	case IDM_COLOR_RED:		OnColor(RGB(255,0,0));		return TRUE;
	case IDM_COLOR_GREEN:	OnColor(RGB(0,255,0));		return TRUE;
	case IDM_COLOR_BLUE:	OnColor(RGB(0,0,255));		return TRUE;
	case IDM_COLOR_WHITE:	OnColor(RGB(255,255,255));	return TRUE;
	} 

	return FALSE;
}

void CSimpleMDIChild::OnColor(COLORREF rgb)
{
	CDockSimple& DockSimple = static_cast<CDockSimple&>(GetView());
	CSimpleView& View = static_cast<CSimpleView&>(DockSimple.GetView());
	View.SetColor(rgb);
	View.Invalidate();
}

LRESULT CSimpleMDIChild::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CDocker& Dock = static_cast<CDocker&>(GetView());
	Dock.RecalcDockLayout();

	// Pass the message on for default processing
	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CSimpleMDIChild::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:	return OnSize(uMsg, wParam, lParam);
	}

	// Do default processing for other messages
	return WndProcDefault(uMsg, wParam, lParam);
}
