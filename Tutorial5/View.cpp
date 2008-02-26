//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include <Windowsx.h>	// defines GET_X_LPARAM
#include "view.h"
#include "resource.h"


CView::CView()
{
}

void CView::DrawLine(int x, int y)
{
	HDC hDC = ::GetDC(m_hWnd);

	::MoveToEx(hDC, m_points.back().x, m_points.back().y, NULL); ;
	::LineTo(hDC, x, y);

	::ReleaseDC(m_hWnd, hDC);
}

void CView::OnPaint(HDC hDC)
{
	if (m_points.size() > 0)
	{
		bool bDraw = false;  //Start with the pen up

		for (unsigned int i = 0 ; i < m_points.size(); i++)
		{
			if (bDraw) ::LineTo(hDC, m_points[i].x, m_points[i].y);
			else ::MoveToEx(hDC, m_points[i].x, m_points[i].y, NULL);
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

LRESULT CView::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
 		// Capture mouse input.
 		::SetCapture(hWnd);

		StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);
		break;

	case WM_MOUSEMOVE:
         // hold down the left mouse button and move mouse to draw lines.
         if (wParam & MK_LBUTTON)
        {
			StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);

			// Redraw the window without erasing
			::InvalidateRect(m_hWnd, NULL, FALSE);
        }
        break;

    case WM_LBUTTONUP:
		{
			//Release the capture on the mouse
			::ReleaseCapture();

			StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), false);
		}
		break;

	//Not Required within a CFrame
	/*case WM_DESTROY:
		//End the program when window is destroyed
		::PostQuitMessage(0);
		break; */
	}

	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}


