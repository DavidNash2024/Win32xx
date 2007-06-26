//////////////////////////////////////////////
//View.cpp
// Definitions for the CView class

#include "view.h"


CView::CView()
{
}

void CView::PreCreate(CREATESTRUCT &cs)
{
	cs.lpszClass = TEXT("View");
	cs.dwExStyle = WS_EX_CLIENTEDGE;
	cs.style = WS_CHILD | WS_VISIBLE;
}


LRESULT CView::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		::InvalidateRect(m_hWnd, NULL, TRUE);
		break;
	}

	return 0L;
}

void CView::OnPaint(HDC hDC)
{
	RECT r;
	::GetClientRect(m_hWnd, &r);
	::DrawText(hDC, TEXT("Client Window"), -1, &r, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

