#include "MainFrm.h"
#include "Resource.h"



CMainFrame::CMainFrame() : m_PenColor(RGB(0,0,0))
{
	// Set the Resource IDs for the toolbar buttons
	AddToolbarButton( 0 );			// Separator
	AddToolbarButton( IDM_RED  );
	AddToolbarButton( IDM_BLUE );
	AddToolbarButton( IDM_GREEN );
	AddToolbarButton( IDM_BLACK );
}

void CMainFrame::DrawLine(short x, short y)
{
	HDC hDC = ::GetDC(m_hWnd);
	HPEN hPen = ::CreatePen(PS_SOLID, 1, m_points.back().color);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	
	::MoveToEx(hDC, m_points.back().x, m_points.back().y, NULL); ;
	::LineTo(hDC, x, y);
	
	::ReleaseDC(m_hWnd, hDC);
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	// Respond to menu and toolbar selections
	switch (LOWORD(wParam))
	{
	// Respond to menu items
	case IDM_NEW:
		m_points.clear();
		::InvalidateRect(m_hWnd, NULL, TRUE);
		break;
	case IDM_HELP_ABOUT:
		{
			CDialog HelpDialog(IDW_ABOUT, m_hWnd);
			HelpDialog.DoModal();
		}
		break;

	// Respond to Toolbar buttons
	case IDM_RED:
		m_PenColor = RGB(255, 0, 0);
		TRACE(_T("Red Pen Selected \n"));
		break;
	case IDM_BLUE:
		m_PenColor = RGB(0, 0, 255);
		TRACE(_T("Blue Pen Selected \n"));
		break;
	case IDM_GREEN:
		m_PenColor = RGB(0, 191, 0);
		TRACE(_T("Green Pen Selected \n"));
		break;
	case IDM_BLACK:
		m_PenColor = RGB(0, 0, 0);
		TRACE(_T("Black Pen Selected \n"));
		break;

	// Respond to the accelerator key
	case IDW_QUIT:
		::SendMessage(m_hWnd, WM_CLOSE, 0L, 0L);
		break;
	}

	return 0;
}

void CMainFrame::OnInitialUpdate()
{
	// Startup code goes here
}

void CMainFrame::OnLButtonDown(WPARAM /*wParam*/, LPARAM lParam)
{
	// Capture mouse input.
	::SetCapture(m_hWnd);

	StorePoint(LOWORD(lParam), HIWORD(lParam), true);
}

void CMainFrame::OnLButtonUp(WPARAM /*wParam*/, LPARAM lParam)
{
	//Release the capture on the mouse
	::ReleaseCapture();

	StorePoint(LOWORD(lParam), HIWORD(lParam), false);
}

void CMainFrame::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	// hold down the left mouse button and move mouse to draw lines.
	if (wParam & MK_LBUTTON)
	{	
		TCHAR str[80];
		::wsprintf(str, TEXT("Draw Point:  %hd, %hd\n"), LOWORD(lParam), HIWORD(lParam));
		TRACE(str);

		DrawLine(LOWORD(lParam), HIWORD(lParam));
		StorePoint(LOWORD(lParam), HIWORD(lParam), true);
	}
}

void CMainFrame::OnPaint(HDC hDC)
{
	// Redraw our client area
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

void CMainFrame::SetPen(COLORREF color)
{
	m_PenColor = color;
}

void CMainFrame::StorePoint(int x, int y, bool PenDown)
{
	PlotPoint P1;
	P1.x = x;
	P1.y = y;
	P1.PenDown = PenDown;
	P1.color = m_PenColor;

	m_points.push_back(P1); //Add the point to the vector
}

LRESULT CMainFrame::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// handle left mouse button up/down and mouse move messages
	// a seperate function for each case keeps the code tidy.
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		OnLButtonDown(wParam, lParam);
		break;

	case WM_MOUSEMOVE:
		OnMouseMove(wParam, lParam);
        break;

    case WM_LBUTTONUP:
		OnLButtonUp(wParam, lParam);
		break;
	}

	// Pass unhandled messages on to WndProcDefault
	return WndProcDefault(hwnd, uMsg, wParam, lParam);
}


