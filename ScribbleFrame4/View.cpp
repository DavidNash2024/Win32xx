//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include <Windowsx.h>
#include "resource.h"
#include "view.h"
#include "../Win32++/GDI.h"


CView::CView() : m_PenColor(RGB(0,0,0))
{
	m_hBrush = ::CreateSolidBrush(RGB(255,255,230));
}

CView::~CView()
{
	::DeleteObject(m_hBrush);
}

void CView::ClearPoints()
{
	m_points.clear();
	::InvalidateRect(GetHwnd(), NULL, TRUE);
}

void CView::DrawLine(int x, int y)
{
	HDC hDC = ::GetDC(m_hWnd);
	HPEN hPen = ::CreatePen(PS_SOLID, 1, m_points.back().color);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	
	::MoveToEx(hDC, m_points.back().x, m_points.back().y, NULL); ;
	::LineTo(hDC, x, y);

	char str[80];
	::wsprintf(str, "Draw Point:  %d, %d\n", x, y);
	TRACE(str);
	
	::ReleaseDC(m_hWnd, hDC);
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); 

	// or we could do it like this:
	
	// CDC DrawDC = ::GetDC(m_hWnd);
	// DrawDC.CreatePen(PS_SOLID, 1, m_points.back().color);
	// ::MoveToEx(DrawDC, m_points.back().x, m_points.back().y, NULL); ;
	// ::LineTo(DrawDC, x, y);
}

void CView::OnPaint(HDC hDC)
{
	if (m_points.size() > 0)
	{
		bool bDraw = false;  //Start with the pen up
		for (unsigned int i = 0 ; i < m_points.size(); i++)
		{
			HPEN hPen = ::CreatePen(PS_SOLID, 1, m_points[i].color);
			HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
			if (bDraw) ::LineTo(hDC, m_points[i].x, m_points[i].y);
			else ::MoveToEx(hDC, m_points[i].x, m_points[i].y, NULL);
			bDraw = m_points[i].PenDown;
			::SelectObject(hDC, hOldPen);
			::DeleteObject(hPen);
		}
	}
}

void CView::PreCreate(CREATESTRUCT &cs)
{
	// Set the extra style to provide a sunken effect
	cs.dwExStyle = WS_EX_CLIENTEDGE;
}

void CView::PreRegisterClass(WNDCLASS &wc)
{
	// Set the background brush, class name and cursor
	wc.hbrBackground = m_hBrush;
	wc.lpszClassName = "Scribble Window";
	wc.hCursor = ::LoadCursor(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR1));
}

void CView::SetPen(COLORREF color)
{
	m_PenColor = color;
}

void CView::StorePoint(int x, int y, bool PenDown)
{
	PlotPoint P1;
	P1.x = x;
	P1.y = y;
	P1.PenDown = PenDown;
	P1.color = m_PenColor;

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
			DrawLine(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);
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

