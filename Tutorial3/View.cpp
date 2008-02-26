//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include <Windowsx.h>
#include "view.h"


CView::CView()
{
}

void CView::DrawLine(int x, int y)
{
	HDC hDC = ::GetDC(m_hWnd);
	
	::MoveToEx(hDC, m_OldPt.x, m_OldPt.y, NULL); ;
	::LineTo(hDC, x, y);
	
	::ReleaseDC(m_hWnd, hDC);
}

LRESULT CView::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
 		// Capture mouse input.
 		::SetCapture(hWnd);
		m_OldPt.x = GET_X_LPARAM(lParam);
		m_OldPt.y = GET_Y_LPARAM(lParam);

		break;

	case WM_MOUSEMOVE:
         // hold down the left mouse button and move mouse to draw lines.
         if (wParam & MK_LBUTTON)
        {	
			DrawLine(LOWORD(lParam), HIWORD(lParam));
			m_OldPt.x = GET_X_LPARAM(lParam);
			m_OldPt.y = GET_Y_LPARAM(lParam);

        }
        break;

    case WM_LBUTTONUP:
		{
			//Release the capture on the mouse
			::ReleaseCapture();
		}
		break;

	case WM_DESTROY:
		//End the program when window is destroyed
		::PostQuitMessage(0);
		break;
	}

	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}
