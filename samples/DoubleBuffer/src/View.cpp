//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "stdafx.h"
#include "view.h"
#include "resource.h"


CView::CView()
{
}

CBitmap CView::CreateMaskBitmap(CBitmap& m_Blue, COLORREF clrTransparent)
{
	BITMAP bm;
	GetObject(m_Blue, sizeof(BITMAP), &bm);
	CBitmap bmMask;
	bmMask.CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);
	m_BallSize.cx = bm.bmWidth;
	m_BallSize.cy = bm.bmHeight;

	CMemDC dcMem(0);
	CMemDC dcMem2(0);

	SelectObject(dcMem, m_Blue);
	dcMem2.SelectObject(bmMask);

	SetBkColor(dcMem, clrTransparent);
	dcMem2.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, dcMem, 0, 0, SRCCOPY);
	dcMem.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, dcMem2, 0, 0, SRCINVERT);

	return bmMask;
}

void CView::OnDraw(CDC& dc)
// OnDraw is called when part or all of the window needs to be redrawn
{
	UNREFERENCED_PARAMETER(dc);
}


int CView::OnCreate(CREATESTRUCT& cs)
{
	UNREFERENCED_PARAMETER (cs);

	m_Blue.LoadBitmap(IDB_BLUE);
	m_Orange.LoadBitmap(IDB_ORANGE);
	m_Mask = CreateMaskBitmap(m_Blue, RGB(255,255,255));

	return 0;
}

void CView::OnInitialUpdate()
// OnInitialUpdate is called immediately after the window is created
{
	TRACE("View window created\n");

	SetTimer(ID_TIMER, 10, 0);
}

LRESULT CView::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CClientDC dc(*this);
	CRect rc = GetClientRect();


	static int x = 0;
	static int y = 0;
	static int cx = 1;
	static int cy = 1;

	x = x + cx;
	if (x > rc.Width() - m_BallSize.cx)
	{
		x = rc.Width() - m_BallSize.cx;
		cx = -1;
	}
	else if (x < 0)
	{
		x = 0;
		cx = 1;
	}
	
	y = y + cy;
	if (y > rc.Height() - m_BallSize.cy)
	{
		y = rc.Height() - m_BallSize.cy;
		cy = -1;
	}
	else if (y < 0)
	{
		y = 0;
		cy = 1;
	}

	CMemDC dcMemMask(dc);
	dcMemMask.SelectObject(m_Mask);
	
	CMemDC dcMemOrange(dc);
	dcMemOrange.SelectObject(m_Orange);

	CMemDC dcMemBlue(dc);
	dcMemBlue.SelectObject(m_Blue);

	CMemDC dcMem3(dc);
	dcMem3.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
	dcMem3.SolidFill(RGB(255,255,255), rc);

	// Copy the orange ball to the memory DC
	dcMem3.BitBlt(x, 0, m_BallSize.cx, m_BallSize.cy, dcMemOrange, 0, 0, SRCCOPY);

	// Copy the blue ball to the memory DC with mask
	dcMem3.BitBlt(0, y, 64, 64, dcMemMask, 0, 0, SRCERASE);
	dcMem3.BitBlt(0, y, 64, 64, dcMemMask, 0, 0, SRCINVERT);
	dcMem3.BitBlt(0, y, 64, 64, dcMemBlue, 0, 0, SRCINVERT);

	// Copy the memory DC to the client DC
	dc.BitBlt(0,0, rc.Width(), rc.Height(), dcMem3, 0, 0, SRCCOPY);
	
	return FinalWindowProc(uMsg, wParam, lParam);
}

void CView::PreCreate(CREATESTRUCT &cs)
{
	// Here we set the defaults used by the create function for the view window
	// Preforming this is optional, but doing so allows us to
	// take more precise control over the window we create.

	// Set the extended style
	cs.dwExStyle = WS_EX_CLIENTEDGE;
}

void CView::PreRegisterClass(WNDCLASS &wc)
{
	// Here we set the Window class parameters.
	// Preforming this is optional, but doing so allows us to
	// take more precise control over the type of window we create.

	// Set the Window Class name
	wc.lpszClassName = _T("Win32++ View");

	// Set a background brush to white
	wc.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);

	// Set the default cursor
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);

	// Set the class style (not to be confused with the window styles set in PreCreate)
	wc.style = CS_DBLCLKS;	// Generate left button double click messages
}

LRESULT CView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
// All window messages for this window pass through WndProc
{
	switch (uMsg)
	{
		case WM_TIMER:  return OnTimer(uMsg, wParam, lParam);
	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}



