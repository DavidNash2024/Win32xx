//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "view.h"
#include "../Win32++/gdi.h"


CView::CView()
{
}

void CView::DrawLine(int x, int y)
{
	CDC DrawDC = GetDC();
	DrawDC.MoveTo(m_points.back().x, m_points.back().y);
	DrawDC.LineTo(x, y);
}

void CView::OnPaint(HDC hDC)
{
	CDC PaintDC = hDC;

	if (m_points.size() > 0)
	{
		bool bDraw = false;  //Start with the pen up
		for (unsigned int i = 0 ; i < m_points.size(); i++)
		{		
			if (bDraw)
				PaintDC.LineTo(m_points[i].x, m_points[i].y);
			else 
				PaintDC.MoveTo(m_points[i].x, m_points[i].y);
			
			bDraw = m_points[i].PenDown;
		}
	}

	PaintDC.DetachDC();	// Otherwise the DC would be deleted
}

void CView::StorePoint(int x, int y, bool PenDown)
{
	PlotPoint P1;
	P1.x = x;
	P1.y = y;
	P1.PenDown = PenDown;

	m_points.push_back(P1); //Add the point to the vector
}

void CView::OnLButtonDown(LPARAM lParam)
{
 	// Capture mouse input.
 	::SetCapture(m_hWnd);

	StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);
}

void CView::OnLButtonUp(LPARAM lParam)
{
	{
		//Release the capture on the mouse
		::ReleaseCapture();

		StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), false);
	}
}

void CView::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	// hold down the left mouse button and move mouse to draw lines.
	if (wParam & MK_LBUTTON)
	{
		DrawLine(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);
	}
}

LRESULT CView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		OnLButtonDown(lParam);
		break;

	case WM_MOUSEMOVE:
		OnMouseMove(wParam, lParam);
        break;

    case WM_LBUTTONUP:
		OnLButtonUp(lParam);
		break;

	case WM_DESTROY:
		//End the program when window is destroyed
		::PostQuitMessage(0);
		break;
	}

	//Use the default message handling for remaining messages
	return WndProcDefault(uMsg, wParam, lParam);
}

