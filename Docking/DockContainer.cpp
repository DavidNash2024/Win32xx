//////////////////////////////////////////////
// DockContainer.cpp
//  Definitions for the CDockContainer class

#include "DockContainer.h"
#include "../Win32++/mdi.h"


CDockContainer::CDockContainer() : m_IsDocked(TRUE)
{
}

void CDockContainer::PreCreate(CREATESTRUCT &cs)
{
	cs.dwExStyle = WS_EX_TOOLWINDOW;
}

LRESULT CDockContainer::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		TRACE("WM_NCCALCSIZE\n");
		if (m_IsDocked)
		{
			LPRECT rc = (LPRECT)lParam;
			rc->top += NCHeight;
		}
		break;
	case WM_NCHITTEST:
		//	TRACE("WM_NCHITTEST\n");
		if (m_IsDocked)
		{
			CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			MapWindowPoints(NULL, m_hWnd, &pt, 1);
			if (pt.y < 0) return HTCAPTION;
		}
		break;
	case WM_NCLBUTTONDOWN:
		TRACE("WM_NCLBUTTONDOWN\n");
		Oldpt.x = GET_X_LPARAM(lParam); 
		Oldpt.y = GET_Y_LPARAM(lParam);
		if (m_IsDocked)
		{		
			bNCLButtonDown = TRUE;
			return 0L;
		}
		break;

	case WM_NCLBUTTONUP:
		TRACE("WM_NCLBUTTONUP\n");
		if (m_IsDocked)
		{		
			bNCLButtonDown = FALSE;
		}
		break;

	case WM_NCMOUSEMOVE:
		TRACE("WM_NCMOUSEMOVE\n");

		if (m_IsDocked)
		{	
			// Discard phantom mouse move messages
			if ((Oldpt.x == GET_X_LPARAM(lParam)) && (Oldpt.y == GET_Y_LPARAM(lParam)))
				return 0L;


			if (bNCLButtonDown)
			{
				DWORD dwStyle = GetWindowLongPtr(GWL_STYLE);
				dwStyle &= ~WS_CHILD;
				dwStyle |= WS_POPUP| WS_CAPTION | WS_SYSMENU  ;
				SetWindowLongPtr(GWL_STYLE, dwStyle);
				
				// Supress redraw while we reposition the window
				SetRedraw(FALSE);
				CRect rc = GetWindowRect();
				SetParent(0);
				SetWindowPos(NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE);
				SetRedraw(TRUE);
				
				m_IsDocked = FALSE;
				GetApp()->GetFrame()->RecalcLayout();
				SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED | SWP_DRAWFRAME);
				PostMessage(WM_NCLBUTTONDOWN, wParam, lParam);
			}
		}
		break;

	case WM_NCPAINT:
		TRACE("WM_NCPAINT\n");
		if (m_IsDocked)
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
		if (m_IsDocked)
			SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED | SWP_DRAWFRAME);
		break;

	case WM_KILLFOCUS:
		TRACE("WM_KILLFOCUS\n");
		if (m_IsDocked)
			SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED | SWP_DRAWFRAME);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}