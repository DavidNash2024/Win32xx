//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "view.h"


CView::CView()
{
}

CView::~CView()
{
}

void CView::DrawLine(int x, int y)
// Draws a line in the window's client area
{
	CClientDC clientDC(*this);
	clientDC.MoveTo(m_OldPt.x, m_OldPt.y);
	clientDC.LineTo(x, y);
}

void CView::OnDestroy()
{
	//End the program when window is destroyed
	::PostQuitMessage(0);
}

LRESULT CView::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
// Called when the left mouse button is pressed while the cursor is over the window.
{
 	// Capture mouse input.
 	SetCapture();

	m_OldPt.x = GET_X_LPARAM(lParam);
	m_OldPt.y = GET_Y_LPARAM(lParam);

	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CView::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam)
// Called when the left mouse button is released
{
	//Release the capture on the mouse
	ReleaseCapture();

	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CView::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
// Called when the mouse is moved while captured
{
	// hold down the left mouse button and move mouse to draw lines.
	if ( (wParam & MK_LBUTTON) && (GetCapture() == *this) )
	{
		DrawLine(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		m_OldPt.x = GET_X_LPARAM(lParam);
		m_OldPt.y = GET_Y_LPARAM(lParam);
	}
	
	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
// Called to handle the window's messages
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:	return OnLButtonDown(uMsg, wParam, lParam);
	case WM_MOUSEMOVE:		return OnMouseMove(uMsg, wParam, lParam);
	case WM_LBUTTONUP:		return OnLButtonUp(uMsg, wParam, lParam);	
	}

	//Use the default message handling for remaining messages
	return WndProcDefault(uMsg, wParam, lParam);
}

