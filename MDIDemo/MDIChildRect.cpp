//////////////////////////////////////////////
// MDIChildRect.cpp
//  Definitions for the CMDIChildRect class

#include "MDIChildRect.h"
#include "resource.h"


CMDIChildRect::CMDIChildRect()
{
	SetChildMenu(TEXT ("MdiMenuRect"));
}

CMDIChildRect::~CMDIChildRect()
{
}

void CMDIChildRect::OnInitialUpdate()
{
	::SetWindowText(m_hWnd, TEXT("Rectangle Window"));
	SetIconLarge(IDI_RECT);
	SetIconSmall(IDI_RECT);
}

LRESULT CMDIChildRect::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		SetTimer (hWnd, 1, 250, NULL) ;
		break;
	case WM_CLOSE:
		KillTimer(hWnd, 1);
		break;

	case WM_TIMER:            // Display a random rectangle
		{
			int nRed, nGreen, nBlue;
			int xLeft, xRight, yTop, yBottom;
			xLeft   = rand () % m_cxClientMax;
			xRight  = rand () % m_cxClientMax;
			yTop    = rand () % m_cyClientMax;
			yBottom = rand () % m_cyClientMax;
			nRed    = rand () & 255;
			nGreen  = rand () & 255;
			nBlue   = rand () & 255;

			HDC hdc = GetDC (m_hWnd);
			HBRUSH hBrush = ::CreateSolidBrush (RGB (nRed, nGreen, nBlue));
			HBRUSH hOldBrush = (HBRUSH)::SelectObject (hdc, hBrush);

            int Left   = (xLeft < xRight) ? xLeft : xRight;
            int Top    = (yTop < yBottom) ? yTop  : yBottom;
            int Right  = (xLeft > xRight) ? xLeft : xRight;
            int Bottom = (yTop > yBottom) ? yTop  : yBottom;
            ::Rectangle (hdc, Left, Top, Right, Bottom);

			::SelectObject (hdc, hOldBrush);
			::DeleteObject (hBrush);
			::ReleaseDC (m_hWnd, hdc);
		}
		break;

	case WM_SIZE:             // If not minimized, save the window size
          if (wParam != SIZE_MINIMIZED)
          {
               m_cxClientMax = LOWORD (lParam) ;
			   if (m_cxClientMax < 1) m_cxClientMax = 1;
               m_cyClientMax = HIWORD (lParam) ;
			   if (m_cyClientMax < 1) m_cyClientMax = 1;
          }
          break ;        // WM_SIZE must be processed by DefMDIChildProc
	}

	return 0L;
}
