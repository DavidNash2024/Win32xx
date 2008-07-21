//////////////////////////////////////////////
// DockContainer.cpp
//  Definitions for the CDockable class

#include "Dockable.h"
#include "../Win32++/mdi.h"

#include "DockFrame.h"

CDockable::CDockable() : m_DockState(DS_DOCK_LEFT)
{
}

void CDockable::PreCreate(CREATESTRUCT &cs)
{
	cs.dwExStyle = WS_EX_TOOLWINDOW;
}

void CDockable::SendNotify(UINT nMessageID)
{	
	// Send a USER_DRAGMOVE notification to the frame
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
	int NCHeight = 20;
	static BOOL bNCLButtonDown = FALSE;
	static CPoint Oldpt;
	
	switch (uMsg)
	{
	case WM_ACTIVATE:
		TRACE("WM_ACTIVATE\n");
		break;
	case WM_ACTIVATEAPP:
		TRACE("WM_ACTIVATEAPP\n");
		break;
	case WM_LBUTTONDOWN:
		TRACE("WM_LBUTTONDOWN\n");
		break;
	case WM_LBUTTONUP:
		TRACE("WM_LBUTTONUP\n");
		break;
	case WM_MOUSEACTIVATE:
		TRACE("WM_MOUSEACTIVATE\n");
		if (GetFocus() != hWnd) SetFocus();
		break;
	case WM_MOUSEMOVE:
		TRACE("WM_MOUSEMOVE\n");
		break;
	case WM_NCACTIVATE:
		TRACE("WM_NCACTIVATE\n");
		break;
	case WM_NCCALCSIZE:
		// Set the non-client area (This also controls the client area) 
		TRACE("WM_NCCALCSIZE\n");
		if (IsDocked())
		{
			LPRECT rc = (LPRECT)lParam;
			rc->top += NCHeight;
		}
		break;
	case WM_NCHITTEST:
		//	TRACE("WM_NCHITTEST\n");
		if (IsDocked())
		{
			CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			MapWindowPoints(NULL, m_hWnd, &pt, 1);
			// Indicate the point is in the caption if in the non-client area
			if (pt.y < 0) return HTCAPTION;
		}
		break;
	case WM_NCLBUTTONDOWN:
		TRACE("WM_NCLBUTTONDOWN\n");
		Oldpt.x = GET_X_LPARAM(lParam); 
		Oldpt.y = GET_Y_LPARAM(lParam);
		bNCLButtonDown = TRUE;
		if (IsDocked())
		{		
			return 0L;
		}
		else
		{
			// Send a USER_DRAGSTART notification to the frame
			SendNotify(USER_DRAGSTART);
		}
		break;

	case WM_NCLBUTTONUP:
		TRACE("WM_NCLBUTTONUP\n");
		{
			bNCLButtonDown = FALSE;
		}
		break;

	case WM_NCMOUSEMOVE:
		TRACE("WM_NCMOUSEMOVE\n");
		{
			if (IsDocked())
			{	
				// Discard phantom mouse move messages
				if ((Oldpt.x == GET_X_LPARAM(lParam)) && (Oldpt.y == GET_Y_LPARAM(lParam)))
					return 0L;

				if (bNCLButtonDown)
				{
					UnDock();
					PostMessage(WM_NCLBUTTONDOWN, wParam, lParam);
				}
			}
			else if (bNCLButtonDown)
			{
				// We get a WM_NCMOUSEMOVE (not WM_NCLBUTTONUP) when drag of non-docked window ends
				// Send a USER_DRAGEND notification to the frame
				SendNotify(USER_DRAGEND);
				bNCLButtonDown = FALSE;
			}		
		}
		break;

	case WM_NCPAINT:
		TRACE("WM_NCPAINT\n");
		if (IsDocked())
		{
			CDC dc = GetWindowDC(hWnd);
			CRect rc = GetWindowRect();
			MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rc, 2);
			rc.bottom = -rc.top;
			rc.top = 0;

			if (hWnd == GetFocus())
				SolidFill(dc, GetSysColor(COLOR_ACTIVECAPTION), &rc);	
			else
				SolidFill(dc, RGB(192, 192, 192), &rc);		
		}
		break;
	case WM_SETFOCUS:
		TRACE("WM_SETFOCUS\n");
		if (IsDocked())
			SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED | SWP_DRAWFRAME);
		break;

	case WM_KILLFOCUS:
		TRACE("WM_KILLFOCUS\n");
		if (IsDocked())
			SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED | SWP_DRAWFRAME);
		break;

	case WM_WINDOWPOSCHANGED:
	//	TRACE("WM_WINDOWPOSCHANGED\n");
		if (!IsDocked())
		{	
			// Send a USER_DRAGMOVE notification to the frame
			SendNotify(USER_DRAGMOVE);
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}