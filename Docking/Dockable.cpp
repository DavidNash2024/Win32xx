//////////////////////////////////////////////
// DockContainer.cpp
//  Definitions for the CDockable class

#include "Dockable.h"
#include "../Win32++/mdi.h"

#include "DockFrame.h"

CDockable::CDockable() : m_NCHeight(20), m_DockState(0), m_DockWidth(0), m_pDockParent(NULL),
	              m_pDockChildLeft(0), m_pDockChildRight(0), m_pDockChildTop(0), m_pDockChildBottom(0)
{
}

void CDockable::Draw3DBorder(RECT& Rect)
{
	// Imitates the drawing of the WS_EX_CLIENTEDGE extended style
	// This draws a 2 pixel border around the specified Rect
	CDC dc = GetWindowDC(m_hWnd);
	CRect rcw = Rect;
	dc.CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
	MoveToEx(dc, 0, rcw.Height(), NULL);
	LineTo(dc, 0, 0);
	LineTo(dc, rcw.Width(), 0);
	dc.CreatePen(PS_SOLID,1, GetSysColor(COLOR_3DDKSHADOW));
	MoveToEx(dc, 1, rcw.Height()-2, NULL);
	LineTo(dc, 1, 1);
	LineTo(dc, rcw.Width()-2, 1);
	dc.CreatePen(PS_SOLID,1, GetSysColor(COLOR_3DHILIGHT));
	MoveToEx(dc, rcw.Width()-1, 0, NULL);
	LineTo(dc, rcw.Width()-1, rcw.Height()-1);
	LineTo(dc, 0, rcw.Height()-1);
	dc.CreatePen(PS_SOLID,1, GetSysColor(COLOR_3DLIGHT));
	MoveToEx(dc, rcw.Width()-2, 1, NULL);
	LineTo(dc, rcw.Width()-2, rcw.Height()-2);
	LineTo(dc, 1, rcw.Height()-2); 
}

CDockable* CDockable::GetDockChild(UINT DockSide)
{
	switch (DockSide)
	{
	case DS_DOCKED_LEFT:
		return m_pDockChildLeft;
	case DS_DOCKED_RIGHT:
		return m_pDockChildRight;
	case DS_DOCKED_TOP:
		return m_pDockChildTop;
	case DS_DOCKED_BOTTOM:
		return m_pDockChildBottom;
	default:
		return NULL;
	}
}

void CDockable::PreCreate(CREATESTRUCT &cs)
{
	cs.dwExStyle = WS_EX_TOOLWINDOW | WS_EX_CLIENTEDGE;
//	cs.dwExStyle = WS_EX_TOOLWINDOW;
}

void CDockable::SendNotify(UINT nMessageID)
{	
	// Send a docking notification to the frame
	DRAGPOS DragPos;
	DragPos.hdr.code = nMessageID;
	DragPos.hdr.hwndFrom = m_hWnd;
	GetCursorPos(&DragPos.ptPos);
	SendMessage(GetApp()->GetFrame()->GetHwnd(), WM_NOTIFY, 0, (LPARAM)&DragPos);
}

void CDockable::UnDock()
{
	DWORD dwStyle = WS_POPUP| WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_VISIBLE;
	SetWindowLongPtr(GWL_STYLE, dwStyle);
					
	// Supress redraw while we reposition the window
	SetRedraw(FALSE);
	CRect rc = GetWindowRect();
	SetParent(0);
	SetWindowPos(NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE);
	SetRedraw(TRUE);
	
	m_DockState = 0;
	GetApp()->GetFrame()->RecalcLayout();
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED | SWP_DRAWFRAME);
}

LRESULT CDockable::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static BOOL IsNcLButtonDown = FALSE;
	static BOOL InCaption = FALSE;
	static CPoint Oldpt;
	
	switch (uMsg)
	{
	case WM_ACTIVATE:
		break;
	case WM_ACTIVATEAPP:
		break;
	case WM_CLOSE:
		IsNcLButtonDown = FALSE;
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_MOUSEACTIVATE:
		if (GetFocus() != hWnd) SetFocus();
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_NCACTIVATE:
		break;
	case WM_NCCALCSIZE:
		// Set the non-client area (This also controls the client area) 
		if (IsDocked())
		{
			LPRECT rc = (LPRECT)lParam;
			rc->top += m_NCHeight;
		}
		break;
	case WM_NCHITTEST:
		if (IsDocked())
		{
			CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			MapWindowPoints(NULL, m_hWnd, &pt, 1);
			// Indicate the point is in the caption if in the non-client area
			if (pt.y < 0) return HTCAPTION;
		}
		break;
	case WM_NCLBUTTONDOWN:
		Oldpt.x = GET_X_LPARAM(lParam); 
		Oldpt.y = GET_Y_LPARAM(lParam);
		IsNcLButtonDown = TRUE;
		if (IsDocked())
		{	
			CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			MapWindowPoints(NULL, hWnd, &pt, 1);
			InCaption = (pt.y < 0); // Boolean expression
			return 0L;
		}
		else
		{
			// Send a DN_DOCK_START notification to the frame
			SendNotify(DN_DOCK_START);
		}
		break;

	case WM_NCLBUTTONUP:
		{
			IsNcLButtonDown = FALSE;
		}
		break;

	case WM_NCMOUSEMOVE:
		{
			if (IsDocked())
			{	
				// Discard phantom mouse move messages
				if ((Oldpt.x == GET_X_LPARAM(lParam)) && (Oldpt.y == GET_Y_LPARAM(lParam)))
					return 0L;

				if (IsNcLButtonDown && InCaption)
				{
					UnDock();
					PostMessage(WM_NCLBUTTONDOWN, wParam, lParam);
				}
			}
			else if (IsNcLButtonDown)
			{
				// We get a WM_NCMOUSEMOVE (not WM_NCLBUTTONUP) when drag of non-docked window ends
				// Send a DN_DOCK_END notification to the frame
				SendNotify(DN_DOCK_END);
				IsNcLButtonDown = FALSE;
			}		
		}
		break;

	case WM_NCPAINT:
		if (IsDocked())
		{		
			CDC dc = GetWindowDC(hWnd);
			CRect rc = GetWindowRect();

			// Set the font for the title
			NONCLIENTMETRICS info = {0};
			info.cbSize = sizeof(info);
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
			dc.CreateFontIndirect(&info.lfStatusFont);
			
			// Set the Colours
			if (hWnd == GetFocus())
			{
				dc.CreateSolidBrush(GetSysColor(COLOR_ACTIVECAPTION));
				::SetBkColor(dc, GetSysColor(COLOR_ACTIVECAPTION));
				::SetTextColor(dc, RGB(255, 255, 255));
			}
			else
			{
				dc.CreateSolidBrush(RGB(232, 228, 220));
				::SetBkColor(dc, RGB(232, 228, 220));
				::SetTextColor(dc, RGB(0, 0, 0));
			}
		
			// Handle the WS_EX_CLIENTEDGE style
			if (GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_CLIENTEDGE)
			{
				dc.CreatePen(PS_SOLID, 1, RGB(160, 150, 140));		
				Rectangle(dc, 2, 2, rc.Width()-2, 2 + m_NCHeight);

				// Set the title
				CRect rcText(8, 2, rc.Width()-4, 2 + m_NCHeight);			
				::DrawText(dc, _T("Class View - Docking"), -1, &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

				// Draw the client edge
				Draw3DBorder(rc);

				// Cleanup possible rubbish in the client area
				if ((rc.Height() < m_NCHeight+4) || (rc.Width() < 4))
					Invalidate();
			}
			else
			{
				dc.CreatePen(PS_SOLID, 1, RGB(160, 150, 140));		
				Rectangle(dc, 0, 0, rc.Width(), m_NCHeight);
				CRect rcText(8, 2, rc.Width()-4, 2 + m_NCHeight);
				::DrawText(dc, _T("Class View - Docking"), -1, &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
			}

			return 0;
		}
		break; 
	case WM_SETFOCUS:
		if (IsDocked())
			SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED | SWP_DRAWFRAME);
		break;

	case WM_KILLFOCUS:
		if (IsDocked())
			SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED | SWP_DRAWFRAME);
		break;

	case WM_WINDOWPOSCHANGED:
		if (!IsDocked())
		{	
			// Send a DN_DOCK_MOVE notification to the frame
			SendNotify(DN_DOCK_MOVE);
		}
		break;

	case DN_CANDOCKHERE:
		return TRUE;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}