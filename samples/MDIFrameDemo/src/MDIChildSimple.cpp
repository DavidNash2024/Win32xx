//////////////////////////////////////////////
// MDIChildSimple.cpp
//  Definitions for the CViewSimple and CMDIChildSimple classes

#include "stdafx.h"
#include "MDIChildSimple.h"
#include "resource.h"


// CViewSimple definitions
CViewSimple::CViewSimple() : m_Color(RGB(0,0,255))
{
}

void CViewSimple::OnDraw(CDC* pDC)
{
	//Centre some text in our view window
	CRect rc = GetClientRect();
	pDC->SetTextColor(m_Color);
	pDC->DrawText(_T("View Window"), -1, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

LRESULT CViewSimple::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

// CMDIChildSimple definitions
CMDIChildSimple::CMDIChildSimple()
{
	HINSTANCE hResource = GetApp()->GetResourceHandle();
	HMENU hChildMenu = LoadMenu(hResource, _T("MdiMenuView"));
	SetHandles(hChildMenu, NULL);
	SetView(m_View);
}

CMDIChildSimple::~CMDIChildSimple()
{
}

int CMDIChildSimple::OnCreate(LPCREATESTRUCT pcs)
{
	SetWindowText(_T("Simple Window"));
	SetIconLarge(IDI_VIEW);
	SetIconSmall(IDI_VIEW);

	return CMDIChild::OnCreate(pcs);
}

void CMDIChildSimple::OnClose()
{
	int nResult = MessageBox(_T("OK to close Window?"), _T("File Close"), MB_YESNO);

	if (nResult == IDYES)
		MDIDestroy();
}

BOOL CMDIChildSimple::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (LOWORD(wParam))
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

LRESULT CMDIChildSimple::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//		Add case statements for each messages to be handled here
//	}

	// Do default processing for other messages
	return WndProcDefault(uMsg, wParam, lParam);
}
