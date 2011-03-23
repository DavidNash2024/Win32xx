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

void CSimpleView::OnPaint(CDC& dc)
{
	//Centre some text in our view window
	CRect rc = GetClientRect();
	dc.SetTextColor(m_Color);
	dc.DrawText(_T("View Window"), -1, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

LRESULT CSimpleView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		// Force the window to be repainted
		Invalidate();
		break;		// Also do default processing
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
	SetChildMenu(_T("MdiMenuView"));
}

CSimpleMDIChild::~CSimpleMDIChild()
{
}

void CSimpleMDIChild::OnInitialUpdate()
{
    // Set the window caption
	SetWindowText(_T("Simple Window"));
	
    // Set the window icons
    SetIconLarge(IDI_VIEW);
	SetIconSmall(IDI_VIEW);

	// Add Child dockers
	DWORD dwStyle = DS_CLIENTEDGE | DS_NO_CAPTION | DS_NO_UNDOCK;
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
	CDockSimple* pDockSimple = (CDockSimple*)GetView();
	CSimpleView* pView = (CSimpleView*)pDockSimple->GetView();

	switch (LOWORD(wParam))
	{
	case IDM_COLOR_BLACK:
		pView->SetColor(RGB(0,0,0));
		pView->Invalidate();
		return TRUE;
	case IDM_COLOR_RED:
		pView->SetColor(RGB(255, 0, 0));
		pView->Invalidate();
		return TRUE;
	case IDM_COLOR_GREEN:
		pView->SetColor(RGB(0, 255, 0));
		pView->Invalidate();
		return TRUE;
	case IDM_COLOR_BLUE:
		pView->SetColor(RGB(0, 0, 255));
		pView->Invalidate();
		return TRUE;
	case IDM_COLOR_WHITE:
		pView->SetColor(RGB(255, 255, 255));
		pView->Invalidate();
		return TRUE; 
	} 

	return FALSE;
}

LRESULT CSimpleMDIChild::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		CDocker* pDock = (CDocker*)GetView();
		pDock->RecalcDockLayout();
		break;
	}

	// Do default processing for other messages
	return WndProcDefault(uMsg, wParam, lParam);
}
