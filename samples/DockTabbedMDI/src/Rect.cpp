//////////////////////////////////////////////
// Rect.cpp
//  Definitions for the CViewRect class

#include "stdafx.h"
#include "Rect.h"
#include "resource.h"

// CViewRect definitions

int CViewRect::OnCreate(LPCREATESTRUCT pcs)
{
	UNREFERENCED_PARAMETER(pcs);

	SetTimer (1, 250, NULL) ;
	return 0;
}

void CViewRect::OnDestroy()
{
	KillTimer(1);
}

LRESULT CViewRect::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(uMsg);

	// If not minimized, save the window size
	if (wParam != SIZE_MINIMIZED)
	{
		m_cxClientMax = LOWORD (lParam) ;
		if (m_cxClientMax < 1)
			m_cxClientMax = 1;

		m_cyClientMax = HIWORD (lParam) ;
		if (m_cyClientMax < 1)
			m_cyClientMax = 1;
	}

	return 0L;
}

LRESULT CViewRect::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam)
// Display a random rectangle
{
	UNREFERENCED_PARAMETER(uMsg);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	int nRed, nGreen, nBlue;
	int xLeft, xRight, yTop, yBottom;
	xLeft   = rand () % m_cxClientMax;
	xRight  = rand () % m_cxClientMax;
	yTop    = rand () % m_cyClientMax;
	yBottom = rand () % m_cyClientMax;
	nRed    = rand () & 255;
	nGreen  = rand () & 255;
	nBlue   = rand () & 255;

	CClientDC RectDC(*this);
	RectDC.CreateSolidBrush (RGB (nRed, nGreen, nBlue));

    int Left   = (xLeft < xRight) ? xLeft : xRight;
    int Top    = (yTop < yBottom) ? yTop  : yBottom;
    int Right  = (xLeft > xRight) ? xLeft : xRight;
    int Bottom = (yTop > yBottom) ? yTop  : yBottom;
    RectDC.Rectangle(Left, Top, Right, Bottom);

	return 0L;
}

LRESULT CViewRect::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_TIMER:	return OnTimer(uMsg, wParam, lParam);
	case WM_SIZE:   return OnSize(uMsg, wParam, lParam);
	}

	return WndProcDefault(uMsg, wParam, lParam);
}



