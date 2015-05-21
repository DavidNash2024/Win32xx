//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "ScribbleApp.h"
#include "resource.h"

using namespace std;

CView::CView() : m_PenColor(RGB(0,0,0))
{
	m_Brush.CreateSolidBrush(RGB(255,255,230));
}

CView::~CView()
{
}

void CView::DrawLine(int x, int y)
{
	CClientDC dcClient(*this);
	dcClient.CreatePen(PS_SOLID, 1, m_points.back().color);
	dcClient.MoveTo(m_points.back().x, m_points.back().y);
	dcClient.LineTo(x, y);
}

CDoc& CView::GetDoc()
{
	CMainFrame& Frame = GetScribbleApp().GetMainFrame();
	return Frame.GetDoc();
}

void CView::OnDraw(CDC& dc)
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
	// Set the extra style to provide a sunken effect
	cs.dwExStyle = WS_EX_CLIENTEDGE;
}

void CView::PreRegisterClass(WNDCLASS &wc)
{
	// Set the background brush, class name and cursor
	wc.hbrBackground = m_Brush;
	wc.lpszClassName = _T("Scribble Window");
	wc.hCursor = ::LoadCursor(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR1));
}

LRESULT CView::OnLButtonDown(UINT, WPARAM, LPARAM lParam)
{
 	// Capture mouse input.
 	SetCapture();

	GetDoc().StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true, m_PenColor);

	return 0L;
}

LRESULT CView::OnLButtonUp(UINT, WPARAM, LPARAM lParam)
{
	//Release the capture on the mouse
	ReleaseCapture();

	GetDoc().StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), false, m_PenColor);

	return 0L;	
}

LRESULT CView::OnMouseMove(UINT, WPARAM wParam, LPARAM lParam)
{
	// hold down the left mouse button and move mouse to draw lines.
	if ( (wParam & MK_LBUTTON) && (GetCapture() == *this) )
	{
		DrawLine(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		GetDoc().StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true, m_PenColor);
	}

	return 0L;
}

LRESULT CView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

