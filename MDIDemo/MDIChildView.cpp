//////////////////////////////////////////////
// MDIChildView.cpp
//  Definitions for the CMDIChildView class

#include "MDIChildView.h"
#include "resource.h"


CMDIChildView::CMDIChildView() : m_Color(RGB(0,0,255))
{
	SetChildMenu(_T("MdiMenuView"));
}

CMDIChildView::~CMDIChildView()
{
}

void CMDIChildView::OnInitialUpdate()
{
	::SetWindowText(m_hWnd, _T("View Window"));
	SetIconLarge(IDI_VIEW);
	SetIconSmall(IDI_VIEW);
}

void CMDIChildView::OnClose()
{
	//Action to take when the window is about to close
	int Result = MessageBox(NULL, _T("OK to close Window?"), _T("File Close"), MB_YESNO);
	if (Result == IDYES)
		::SendMessage(m_hWndParent, WM_MDIDESTROY, (WPARAM)m_hWnd, 0);
}

void CMDIChildView::OnPaint(HDC hDC)
{
	//Centre some text in our view window
	RECT r;
	::GetClientRect(m_hWnd, &r);
	::SetTextColor(hDC, m_Color);
	::DrawText(hDC, _T("View Window"), -1, &r, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

BOOL CMDIChildView::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	switch (LOWORD(wParam))
	{
	case IDM_COLOR_BLACK:
		m_Color = RGB(0,0,0);
		::InvalidateRect(m_hWnd, NULL, TRUE);
		return TRUE;
	case IDM_COLOR_RED:
		m_Color = RGB(255, 0, 0);
		::InvalidateRect(m_hWnd, NULL, TRUE);
		return TRUE;
	case IDM_COLOR_GREEN:
		m_Color = RGB(0, 255, 0);
		::InvalidateRect(m_hWnd, NULL, TRUE);
		return TRUE;
	case IDM_COLOR_BLUE:
		m_Color = RGB(0, 0, 255);
		::InvalidateRect(m_hWnd, NULL, TRUE);
		return TRUE;
	case IDM_COLOR_WHITE:
		m_Color = RGB(255, 255, 255);
		::InvalidateRect(m_hWnd, NULL, TRUE);
		return TRUE;
	}

	return FALSE;
}

LRESULT CMDIChildView::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		OnClose();
		return 0L;	// Don't do default processing

	case WM_SIZE:
		::InvalidateRect(hWnd, NULL, TRUE);
		break;		// Also do default processing
	}
	
	// Do default processing for other messages
	return CMDIChild::WndProc(hWnd, uMsg, wParam, lParam);	
}
