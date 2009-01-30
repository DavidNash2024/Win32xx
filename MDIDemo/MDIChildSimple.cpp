//////////////////////////////////////////////
// MDIChildSimple.cpp
//  Definitions for the CViewSimple and CMDIChildSimple classes

#include "MDIChildSimple.h"
#include "resource.h"

// CViewSimple definitions
CViewSimple::CViewSimple() : m_Color(RGB(0,0,255))
{
}

void CViewSimple::OnPaint(HDC hDC)
{
	//Centre some text in our view window
	RECT rc = GetClientRect();
	::SetTextColor(hDC, m_Color);
	::DrawText(hDC, _T("View Window"), -1, &rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

LRESULT CViewSimple::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		// Force the window to be repainted
		::InvalidateRect(hWnd, NULL, TRUE);
		break;		// Also do default processing
	}

	// Do default processing for other messages
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

// CMDIChildSimple definitions
CMDIChildSimple::CMDIChildSimple()
{
	SetView(m_View);
	SetChildMenu(_T("MdiMenuView"));
}

CMDIChildSimple::~CMDIChildSimple()
{
}

void CMDIChildSimple::OnInitialUpdate()
{
	::SetWindowText(m_hWnd, _T("Simple Window"));
	SetIconLarge(IDI_VIEW);
	SetIconSmall(IDI_VIEW);
}

void CMDIChildSimple::OnClose()
{
	//Action to take when the window is about to close
	int Result = MessageBox(_T("OK to close Window?"), _T("File Close"), MB_YESNO);
	if (Result == IDYES)
		::SendMessage(GetParent(), WM_MDIDESTROY, (WPARAM)m_hWnd, 0);
}

BOOL CMDIChildSimple::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	switch (LOWORD(wParam))
	{
	case IDM_COLOR_BLACK:
		m_View.SetColor(RGB(0,0,0));
		::InvalidateRect(m_hWnd, NULL, TRUE);
		return TRUE;
	case IDM_COLOR_RED:
		m_View.SetColor(RGB(255, 0, 0));
		::InvalidateRect(m_hWnd, NULL, TRUE);
		return TRUE;
	case IDM_COLOR_GREEN:
		m_View.SetColor(RGB(0, 255, 0));
		::InvalidateRect(m_hWnd, NULL, TRUE);
		return TRUE;
	case IDM_COLOR_BLUE:
		m_View.SetColor(RGB(0, 0, 255));
		::InvalidateRect(m_hWnd, NULL, TRUE);
		return TRUE;
	case IDM_COLOR_WHITE:
		m_View.SetColor(RGB(255, 255, 255));
		::InvalidateRect(m_hWnd, NULL, TRUE);
		return TRUE;
	}

	return FALSE;
}

LRESULT CMDIChildSimple::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		OnClose();
		return 0L;	// Don't do default processing
	}

	// Do default processing for other messages
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}
