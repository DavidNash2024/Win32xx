//////////////////////////////////////////////
// MDIChildRect.cpp
//  Definitions for the CViewRect and CMDIChildRect class

#include "stdafx.h"
#include "MDIChildRect.h"
#include "resource.h"


////////////////////////
// CViewRect definitions

int CViewRect::OnCreate(LPCREATESTRUCT pcs)
{
	UNREFERENCED_PARAMETER(pcs);
	SetTimer (1, 250, NULL) ;
	return 0;
}


// Note: The MDIChild gets an OnClose, but its view window doesn't
//       so we use OnDestroy here
void CViewRect::OnDestroy()
{	
	KillTimer(1);
}

LRESULT CViewRect::OnSize(WPARAM wParam, LPARAM lParam)
{
	if (wParam != SIZE_MINIMIZED)
	{
		m_cxClientMax = LOWORD (lParam) ;
		if (m_cxClientMax < 1) 
			m_cxClientMax = 1;
		
		m_cyClientMax = HIWORD (lParam) ;
		if (m_cyClientMax < 1) 
			m_cyClientMax = 1;
	}

	return 0;
}

LRESULT CViewRect::OnTimer(WPARAM wParam, LPARAM lParam)
{
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

	CClientDC dcRect(*this);
	dcRect.CreateSolidBrush (RGB (nRed, nGreen, nBlue));

    int Left   = (xLeft < xRight) ? xLeft : xRight;
    int Top    = (yTop < yBottom) ? yTop  : yBottom;
    int Right  = (xLeft > xRight) ? xLeft : xRight;
    int Bottom = (yTop > yBottom) ? yTop  : yBottom;
	dcRect.Rectangle(Left, Top, Right, Bottom);

	return 0L;
}


LRESULT CViewRect::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_TIMER:  return OnTimer(wParam, lParam);		// Display a random rectangle
	case WM_SIZE:   return OnSize(wParam, lParam);		// If not minimized, save the window size
	}

	return WndProcDefault(uMsg, wParam, lParam);
}

////////////////////////////
// CMDIChildRect definitions
CMDIChildRect::CMDIChildRect()
{
	HINSTANCE hResource = GetApp()->GetResourceHandle();
	HMENU hChildMenu = LoadMenu(hResource, _T("MdiMenuRect"));
	SetHandles(hChildMenu, NULL);
	SetView(m_RectView);
}

CMDIChildRect::~CMDIChildRect()
{
}

int CMDIChildRect::OnCreate(LPCREATESTRUCT pcs)
{
	SetWindowText(_T("Rectangle Window"));
	SetIconLarge(IDI_RECT);
	SetIconSmall(IDI_RECT);

	return CMDIChild::OnCreate(pcs);
}

