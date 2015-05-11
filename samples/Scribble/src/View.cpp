//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "stdafx.h"
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
	dcClient.CreatePen(PS_SOLID, 1, GetPoints().back().color);
	dcClient.MoveTo(GetPoints().back().x, GetPoints().back().y);
	dcClient.LineTo(x, y);
}

CDoc& CView::GetDoc()
{
	CMainFrame& Frame = GetScribbleApp().GetMainFrame();
	return Frame.GetDoc();
}

std::vector<PlotPoint>& CView::GetPoints()
{ 
	return GetDoc().GetPoints(); 
}	

void CView::OnDraw(CDC& dc)
{
	// Here we use double buffering (drawing to a memory DC) for smoother rendering
	// Set up our Memory DC and bitmap
	CMemDC MemDC(dc);
	int Width = GetClientRect().Width();
	int Height = GetClientRect().Height();
	MemDC.CreateCompatibleBitmap(dc, Width, Height);
	MemDC.FillRect(GetClientRect(), m_Brush);

	CPen pen;

	if (GetPoints().size() > 0)
	{
		bool bDraw = false;  //Start with the pen up
		for (int i = 0 ; i < GetPoints().size(); ++i)
		{
			pen.CreatePen(PS_SOLID, 1, GetPoints()[i].color);
			MemDC.SelectObject(pen);

			if (bDraw)
				MemDC.LineTo(GetPoints()[i].x, GetPoints()[i].y);
			else
				MemDC.MoveTo(GetPoints()[i].x, GetPoints()[i].y);

			bDraw = GetPoints()[i].PenDown;
		}
	}

	// Copy from the memory DC to our painting dc
	dc.BitBlt(0, 0, Width, Height, MemDC, 0, 0, SRCCOPY);
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

LRESULT CView::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
 	// Capture mouse input.
 	SetCapture();
	GetDoc().StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true, m_PenColor);

	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CView::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//Release the capture on the mouse
	ReleaseCapture();
	GetDoc().StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), false, m_PenColor);

	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CView::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// hold down the left mouse button and move mouse to draw lines.
	if ( (wParam & MK_LBUTTON) && (GetCapture() == *this) )
	{
		CString str;
		str.Format( _T("Draw Point:  %hd, %hd\n"), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) );
		TRACE(str);

		DrawLine(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		GetDoc().StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true, m_PenColor);
	}
	
	return FinalWindowProc(uMsg, wParam, lParam);
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


