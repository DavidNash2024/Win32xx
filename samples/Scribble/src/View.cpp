//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "stdafx.h"
#include "view.h"
#include "resource.h"


using namespace std;

CView::CView() : m_PenColor(RGB(0,0,0))
{
	m_Brush.CreateSolidBrush(RGB(255,255,230));
}

CView::~CView()
{
}

void CView::ClearPoints()
{
	m_points.clear();
	Invalidate();
}

void CView::DrawLine(int x, int y)
{
	CClientDC dcClient(*this);
	dcClient.CreatePen(PS_SOLID, 1, m_points.back().color);
	dcClient.MoveTo(m_points.back().x, m_points.back().y);
	dcClient.LineTo(x, y);
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

	if (m_points.size() > 0)
	{
		bool bDraw = false;  //Start with the pen up
		for (unsigned int i = 0 ; i < m_points.size(); i++)
		{

			MemDC.CreatePen(PS_SOLID, 1, m_points[i].color);
			if (bDraw)
				MemDC.LineTo(m_points[i].x, m_points[i].y);
			else
				MemDC.MoveTo(m_points[i].x, m_points[i].y);

			bDraw = m_points[i].PenDown;
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

BOOL CView::FileOpen(LPCTSTR szFilename)
{
	// empty the PlotPoint vector
	m_points.clear();

	BOOL bResult = FALSE;

	try
	{
		CArchive ar;
		ar.Open(szFilename, CArchive::read);
		ar >> *this;
		bResult = TRUE;
	}

	catch (const CWinException &e)
	{
		// An exception occurred. Display the relevant information.
		MessageBox(e.GetText(), _T("Failed to Load File"), MB_ICONWARNING);
		
		m_points.clear();
	}

	Invalidate();
	return bResult;
}

BOOL CView::FileSave(LPCTSTR szFilename)
{
	BOOL bResult = TRUE;

	try
	{
		CArchive ar;
		ar.Open(szFilename, CArchive::write);
		ar << *this;
	}
	catch (const CWinException &e)
	{
		// An exception occurred. Display the relevant information.
		MessageBox(e.GetText(), _T("Failed to Save File"), MB_ICONWARNING);
	}

	return bResult;
}

LRESULT CView::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
 	// Capture mouse input.
 	SetCapture();
	StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);

	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CView::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//Release the capture on the mouse
	ReleaseCapture();
	StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), false);

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
		StorePoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);
	}
	
	return FinalWindowProc(uMsg, wParam, lParam);
}

void CView::Serialize(CArchive &ar)
// Uses CArchive to stream data to or from a file
{

	if (ar.IsStoring())
	{
		// Store the number of points
		UINT nPoints = m_points.size();
		ar << nPoints;
		
		// Store the PlotPoint data
		std::vector<PlotPoint>::iterator iter;
		for (iter = m_points.begin(); iter < m_points.end(); ++iter)
		{
			ar.Write( &(*iter), sizeof(PlotPoint) );
		}
	}
	else
	{
		UINT nPoints;
		PlotPoint pp = {0};
		m_points.clear();

		// Load the number of points
		ar >> nPoints;

		// Load the PlotPoint data
		for (UINT u = 0; u < nPoints; ++u)
		{
			ar.Read(&pp, sizeof(PlotPoint));
			m_points.push_back(pp);
		}
	}

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

/*
CArchive& operator<<(CArchive& ar, CView& v)
{
	v.Serialize(ar);
	return ar;
}

CArchive&  operator>>(CArchive& ar, CView& v)
{
	v.Serialize(ar);
	return ar;
}
*/
