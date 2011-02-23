//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "view.h"
#include "resource.h"


CView::CView() : m_PenColor(RGB(0,0,0))
{
	m_hBrush = ::CreateSolidBrush(RGB(255,255,230));
}

CView::~CView()
{
	::DeleteObject(m_hBrush);
}

void CView::DrawLine(int x, int y)
{
	CDC dc = GetDC();

	dc.CreatePen(PS_SOLID, 1, m_points.back().color);
	dc.MoveTo(m_points.back().x, m_points.back().y);
	dc.LineTo(x, y);
}

void CView::OnPaint(CDC& dc)
{
	if (m_points.size() > 0)
	{
		bool bDraw = false;  //Start with the pen up
		for (unsigned int i = 0 ; i < m_points.size(); i++)
		{
			dc.CreatePen(PS_SOLID, 1, m_points[i].color);
			if (bDraw)
				dc.LineTo(m_points[i].x, m_points[i].y);
			else
				dc.MoveTo(m_points[i].x, m_points[i].y);
			
			bDraw = m_points[i].PenDown;
		}
	}
}

void CView::PreCreate(CREATESTRUCT &cs)
{
	// Set the extra style to provide a sunken edge
	cs.dwExStyle = WS_EX_CLIENTEDGE;
}

void CView::PreRegisterClass(WNDCLASS &wc)
{
	// Set the background brush and cursor
	wc.hbrBackground = m_hBrush;
	wc.lpszClassName = _T("Scribble Window");
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

void CView::OnLButtonDown(LPARAM lParam)
{
 	// Capture mouse input.
 	SetCapture();

	StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);
}

void CView::OnLButtonUp(LPARAM lParam)
{
	{
		//Release the capture on the mouse
		ReleaseCapture();

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
	}

	//Use the default message handling for remaining messages
	return WndProcDefault(uMsg, wParam, lParam);
}




