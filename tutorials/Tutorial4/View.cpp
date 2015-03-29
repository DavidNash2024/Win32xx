//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "view.h"


CView::CView()
{
}

void CView::DrawLine(int x, int y)
{
	CClientDC dcClient(*this);
	dcClient.MoveTo(m_points.back().x, m_points.back().y);
	dcClient.LineTo(x, y);
}

void CView::OnDestroy()
{
	//End the program when window is destroyed
	::PostQuitMessage(0);
}

void CView::OnDraw(CDC* pDC)
{
	if (m_points.size() > 0)
	{
		bool bDraw = false;  //Start with the pen up

		for (unsigned int i = 0 ; i < m_points.size(); i++)
		{
			if (bDraw) pDC->LineTo(m_points[i].x, m_points[i].y);
			else pDC->MoveTo(m_points[i].x, m_points[i].y);
			bDraw = m_points[i].PenDown;
		}
	}
}

void CView::StorePoint(int x, int y, bool PenDown)
{
	PlotPoint P1;
	P1.x = x;
	P1.y = y;
	P1.PenDown = PenDown;

	m_points.push_back(P1); //Add the point to the vector
}

LRESULT CView::OnLButtonDown(UINT, WPARAM,LPARAM lParam)
{
 	// Capture mouse input.
 	SetCapture();

	StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);

	return 0L;
}

LRESULT CView::OnLButtonUp(UINT, WPARAM, LPARAM lParam)
{
	//Release the capture on the mouse
	ReleaseCapture();

	StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), false);

	return 0L;
}

LRESULT CView::OnMouseMove(UINT, WPARAM wParam, LPARAM lParam)
{
	// hold down the left mouse button and move mouse to draw lines.
	if ( (wParam & MK_LBUTTON) && (GetCapture() == *this) )
	{
		DrawLine(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);
	}

	return 0L;
}

LRESULT CView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN: return	OnLButtonDown(uMsg, wParam, lParam);
	case WM_MOUSEMOVE:	 return OnMouseMove(uMsg, wParam, lParam);
    case WM_LBUTTONUP:	 return OnLButtonUp(uMsg, wParam, lParam);
	}

	//Use the default message handling for remaining messages
	return WndProcDefault(uMsg, wParam, lParam);
}

