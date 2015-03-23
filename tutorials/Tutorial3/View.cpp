//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "view.h"


CView::CView()
{
}

void CView::DrawLine(int x, int y)
{
	CClientDC dc(*this);
	dc.MoveTo(m_OldPt.x, m_OldPt.y);
	dc.LineTo(x, y);
}

void CView::OnDestroy()
{
	//End the program when window is destroyed
	::PostQuitMessage(0);
}

LRESULT CView::OnLButtonDown(LPARAM lParam)
{
 	// Capture mouse input.
 	SetCapture();

	m_OldPt.x = GET_X_LPARAM(lParam);
	m_OldPt.y = GET_Y_LPARAM(lParam);

	return 0L;
}

LRESULT CView::OnLButtonUp(LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	//Release the capture on the mouse
	ReleaseCapture();

	return 0L;
}

LRESULT CView::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	// hold down the left mouse button and move mouse to draw lines.
	if (wParam & MK_LBUTTON)
	{
		DrawLine(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		m_OldPt.x = GET_X_LPARAM(lParam);
		m_OldPt.y = GET_Y_LPARAM(lParam);
	}

	return 0L;
}

LRESULT CView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN: return OnLButtonDown(lParam);
	case WM_MOUSEMOVE:	 return OnMouseMove(wParam, lParam);
    case WM_LBUTTONUP:	 return OnLButtonUp(lParam);
	}

	//Use the default message handling for remaining messages
	return WndProcDefault(uMsg, wParam, lParam);
}
