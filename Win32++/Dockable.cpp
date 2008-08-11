//////////////////////////////////////////////
// DockContainer.cpp
//  Definitions for the CDockable class

#include "../Win32++/Dockable.h"
#include "../Win32++/Default_Resource.h"
#include "resource.h"

namespace Win32xx
{

	void CDockCaption::OnPaint(HDC hDC)
	{	
	//	CDC dc = hDC;
		CDC dc = GetDC(m_hWnd);
		CRect rc = GetClientRect();

		// Set the font for the title
		NONCLIENTMETRICS info = {0};
		info.cbSize = sizeof(info);
		SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
		dc.CreateFontIndirect(&info.lfStatusFont);

		// Set the Colours
		CDockable* pDock = (CDockable*)FromHandle(m_hWndParent);
		if (pDock->m_pView->GetHwnd() == GetFocus())
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

		dc.CreatePen(PS_SOLID, 1, RGB(160, 150, 140));
		Rectangle(dc, 0, 0, rc.Width(), rc.Height() );
		CRect rcText(4, 2, rc.Width()-4, rc.Height());
		::DrawText(dc, _T("Class View - Docking"), -1, &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//	dc.DetachDC();
	}

	void CDockCaption::SendNotify(UINT nMessageID)
	{
		// Send a docking notification to the frame
		DRAGPOS DragPos;
		DragPos.hdr.code = nMessageID;
		DragPos.hdr.hwndFrom = m_hWnd;
		GetCursorPos(&DragPos.ptPos);
		SendMessage(m_hWndParent, WM_NOTIFY, 0, (LPARAM)&DragPos);
	}

	LRESULT CDockCaption::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		static CPoint Oldpt;
		
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
			{
				TRACE("WM_LBUTTONDOWN\n");
				CDockable* pDock = (CDockable*)FromHandle(m_hWndParent);
				pDock->m_pView->SetFocus();
				Oldpt.x = GET_X_LPARAM(lParam);
				Oldpt.y = GET_Y_LPARAM(lParam);
			}
			break;

		case WM_MOUSEMOVE:
			{
				// Discard phantom mouse move messages
				if ((Oldpt.x == GET_X_LPARAM(lParam)) && (Oldpt.y == GET_Y_LPARAM(lParam)))
					return 0L;
					
				if (wParam & MK_LBUTTON)
				{
					// Undock the CDockable window
					SendNotify(DN_DOCK_START);
				}
			}
			break;

		case WM_WINDOWPOSCHANGED:
			Invalidate();
			break;
		}
		
		// pass unhandled messages on for default processing
		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

	CDockable::CDockable() : m_NCHeight(20), m_DockState(0), m_DockWidth(0), m_pDockParent(NULL),
					m_BarWidth(4), m_IsDraggingDockable(FALSE), m_IsInDockZone(FALSE)//, m_hDockParent(NULL)
	{
		WORD HashPattern[] = {0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA};
		m_hbm = ::CreateBitmap (8, 8, 1, 1, HashPattern);
		m_hbrDithered = ::CreatePatternBrush (m_hbm);
	}

	CDockable::~CDockable()
	{
		for (UINT u = 0 ; u < m_vDockChildren.size() ; ++u)
		{
			delete m_vDockChildren[u];
		}

		::DeleteObject(m_hbrDithered);
		::DeleteObject(m_hbm);
	}

	CDockable* CDockable::AddDockChild(CDockable* pDockable, UINT uDockSide, int DockWidth)
	{
		// Create the dockable window
		pDockable->SetDockWidth(DockWidth);
		pDockable->Create(m_hWnd);
		m_vDockChildren.push_back(pDockable);

		// Dock the dockable window
		Dock(pDockable, uDockSide);

		return pDockable;
	}

	void CDockable::Dock(CDockable* pDockable, UINT DockState)
	{
		pDockable->SetParent(m_hWnd);
		pDockable->m_pDockParent = this;
		DWORD dwStyle = WS_CHILD | WS_VISIBLE;
		pDockable->SetWindowLongPtr(GWL_STYLE, dwStyle);
		pDockable->SetDockState(DockState);

		RecalcDockLayout();

		SetForegroundWindow();
		pDockable->SetFocus();
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

	void CDockable::DrawCaption()
	{
		if (IsDocked())
		{
			CDC dc = GetDC(m_hWnd);
			CRect rc = GetClientRect();

			// Set the font for the title
			NONCLIENTMETRICS info = {0};
			info.cbSize = sizeof(info);
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
			dc.CreateFontIndirect(&info.lfStatusFont);

			// Set the Colours
			if (m_hWnd == GetFocus())
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

			dc.CreatePen(PS_SOLID, 1, RGB(160, 150, 140));
			Rectangle(dc, 0, 0, rc.Width(), m_NCHeight);
			CRect rcText(8, 2, rc.Width()-4, 2 + m_NCHeight);
			::DrawText(dc, _T("Class View - Docking"), -1, &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
		}
	}

	void CDockable::DrawHashBar(HWND hBar, POINT Pos)
	{
		// draws a hashed bar while the splitter bar is being dragged
		{
			CDockable* pDock = ((CDockBar*)FromHandle(hBar))->m_pDockable;
			BOOL bVertical = (pDock->GetDockState() == DS_DOCKED_LEFT) || (pDock->GetDockState() == DS_DOCKED_RIGHT);

			CDC BarDC = ::GetDC(m_hWnd);
			BarDC.AttachBrush(m_hbrDithered);

			CRect rc;
			::GetWindowRect(hBar, &rc);
			MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rc, 2);
			int cx = rc.Width();
			int cy = rc.Height();

			if (bVertical)
				::PatBlt (BarDC, Pos.x - m_BarWidth/2, rc.top, m_BarWidth, cy, PATINVERT);
			else
				::PatBlt (BarDC, rc.left, Pos.y - m_BarWidth/2, cx, m_BarWidth, PATINVERT);

			BarDC.DetachBrush();
		}
	}

	UINT CDockable::GetDockSide(LPDRAGPOS pdp)
	{
		CRect rcWindow = GetClientRect();
		MapWindowPoints(GetHwnd(), NULL, (LPPOINT)&rcWindow, 2);

		CRect rcLeft = rcWindow;
		rcLeft.right = rcLeft.left + 30;
		if (rcLeft.PtInRect(pdp->ptPos))
			return DS_DOCKED_LEFT;

		CRect rcRight = rcWindow;
		rcRight.left = rcRight.right - 30;
		if (rcRight.PtInRect(pdp->ptPos))
			return DS_DOCKED_RIGHT;

		CRect rcTop = rcWindow;
		rcTop.InflateRect(-30, 0);
		rcTop.bottom = rcRight.top + 30;
		if (rcTop.PtInRect(pdp->ptPos))
			return DS_DOCKED_TOP;

		CRect rcBottom = rcWindow;
		rcBottom.InflateRect(-30, 0);
		rcBottom.top = rcRight.bottom - 30;
		if (rcBottom.PtInRect(pdp->ptPos))
			return DS_DOCKED_BOTTOM;

		return 0;
	}

	void CDockable::OnCreate()
	{
		m_Bar.Create(m_hWndParent);
		m_pView->Create(m_hWnd);
		m_Caption.Create(m_hWnd);
	}

	LRESULT CDockable::OnNotify(WPARAM /*wParam*/, LPARAM lParam)
	{
		static CPoint OldPoint;
		LPDRAGPOS pdp = (LPDRAGPOS)lParam;

		switch (((LPNMHDR)lParam)->code)
		{
		case DN_DOCK_START:
			{
				TRACE("Drag Start notification\n");
				UnDock();
				m_IsDraggingDockable = TRUE;
				SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(pdp->ptPos.x, pdp->ptPos.y));
			}
			break;

		case DN_DOCK_MOVE:
			TRACE("Drag Move notification\n");
			{
				UINT uDockSide = GetDockSide((LPDRAGPOS)lParam);

				CPoint pt = pdp->ptPos;
				MapWindowPoints(NULL, m_hWnd, &pt, 1);
				HWND hChild = ChildWindowFromPoint(m_hWnd, pt);

				if (SendMessage(hChild, DN_CANDOCKHERE, 0, 0))
					TRACE("*** Can dock here\n");
				else
					TRACE("!!! Can't dock here\n");

				switch (uDockSide)
				{
				case DS_DOCKED_LEFT:
					TRACE("Could dock Left\n");
					m_IsInDockZone = TRUE;
					SetCursor(LoadCursor(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_TRACK4WAY)));
					break;
				case DS_DOCKED_RIGHT:
					TRACE("Could dock Right\n");
					m_IsInDockZone = TRUE;
					SetCursor(LoadCursor(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_TRACK4WAY)));
					break;
				case DS_DOCKED_TOP:
					TRACE("Could dock Top\n");
					m_IsInDockZone = TRUE;
					SetCursor(LoadCursor(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_TRACK4WAY)));
					break;
				case DS_DOCKED_BOTTOM:
					TRACE("Could dock Bottom\n");
					m_IsInDockZone = TRUE;
					SetCursor(LoadCursor(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_TRACK4WAY)));
					break;
				default:
					m_IsInDockZone = FALSE;
					SetCursor(LoadCursor(NULL, IDC_ARROW));
					break;
				}
			}
			break;

		case DN_DOCK_END:
			TRACE("Drag End notification\n");
			{
				UINT DockSide = GetDockSide(pdp);
				HWND hDockable = pdp->hdr.hwndFrom;
				CDockable* pDock = (CDockable*)FromHandle(hDockable);
				if (DockSide) Dock(pDock, DockSide);
				m_IsDraggingDockable = FALSE;
				SetCursor(LoadCursor(NULL, IDC_ARROW));
			}
			break;

		case DN_BAR_START:
			{
				TRACE("Start drag bar notification\n");
				POINT pt = pdp->ptPos;
				MapWindowPoints(NULL, m_hWnd, &pt, 1);
				DrawHashBar(pdp->hdr.hwndFrom, pt);
				OldPoint = pt;
			}
			break;

		case DN_BAR_MOVE:
			{
				TRACE("bar dragged notification\n");
				CPoint pt = pdp->ptPos;
				MapWindowPoints(NULL, m_hWnd, &pt, 1);

				CDockable* pDock = ((CDockBar*)FromHandle(pdp->hdr.hwndFrom))->m_pDockable;
				RECT rcDock = pDock->GetWindowRect();
				MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcDock, 2);

				if (pt != OldPoint)
				{
					DrawHashBar(pdp->hdr.hwndFrom, OldPoint);
					DrawHashBar(pdp->hdr.hwndFrom, pt);
					OldPoint = pt;
				}
			}
			break;

		case DN_BAR_END:
			{
				TRACE("End drag bar notification\n");
				POINT pt = pdp->ptPos;
				MapWindowPoints(NULL, m_hWnd, &pt, 1);

				CDockable* pDock = ((CDockBar*)FromHandle(pdp->hdr.hwndFrom))->m_pDockable;
				RECT rcDock = pDock->GetWindowRect();
				MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcDock, 2);

				DrawHashBar(pdp->hdr.hwndFrom, pt);

				switch (pDock->GetDockState())
				{
				case DS_DOCKED_LEFT:
					pDock->SetDockWidth(pt.x - rcDock.left - m_BarWidth/2);
					break;
				case DS_DOCKED_RIGHT:
					pDock->SetDockWidth(rcDock.right - pt.x - m_BarWidth/2);
					break;
				case DS_DOCKED_TOP:
					pDock->SetDockWidth(pt.y - rcDock.top - m_BarWidth/2);
					break;
				case DS_DOCKED_BOTTOM:
					pDock->SetDockWidth(rcDock.bottom - pt.y - m_BarWidth/2);
					break;
				}

				RecalcDockLayout();
			}
			break;
		case DN_FOCUSCHANGE:
			// Pass this notification up to the top level Dockable, then RecalDockLayout
			// The RecalcLayout redraws the caption when docked
			if (m_pDockParent) 	SendNotify(DN_FOCUSCHANGE);
			else				RecalcDockLayout();
			break;
		}

		return 0L;
	}

	void CDockable::PreCreate(CREATESTRUCT &cs)
	{
	//	cs.dwExStyle = WS_EX_TOOLWINDOW | WS_EX_CLIENTEDGE;
		cs.dwExStyle = WS_EX_TOOLWINDOW;
	}

	void CDockable::PreRegisterClass(WNDCLASS &wc)
	{
		wc.lpszClassName = _T("Win32++ Dockable");
	}

	void CDockable::RecalcDockLayout()
	{
		HDWP hdwp = BeginDeferWindowPos(4);

		CRect rc = GetWindowRect();
		MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rc, 2);

		for (UINT u = 0; u < m_vDockChildren.size(); ++u)
		{
			CRect rcChild = rc;
			int DockWidth;
			switch (m_vDockChildren[u]->GetDockState())
			{
			case DS_DOCKED_LEFT:
				DockWidth = min(m_vDockChildren[u]->GetDockWidth(), rcChild.Width());
				rcChild.right = rcChild.left + DockWidth;
				break;
			case DS_DOCKED_RIGHT:
				DockWidth = min(m_vDockChildren[u]->GetDockWidth(), rcChild.Width());
				rcChild.left = rcChild.right - DockWidth;
				break;
			case DS_DOCKED_TOP:
				DockWidth = min(m_vDockChildren[u]->GetDockWidth(), rcChild.Height());
				rcChild.bottom = rcChild.top + DockWidth;
				break;
			case DS_DOCKED_BOTTOM:
				DockWidth = min(m_vDockChildren[u]->GetDockWidth(), rcChild.Height());
				rcChild.top = rcChild.bottom - DockWidth;
				break;
			}

			if (m_vDockChildren[u]->IsDocked())
			{
				hdwp = ::DeferWindowPos(hdwp, m_vDockChildren[u]->GetHwnd(), NULL, rcChild.left, rcChild.top, rcChild.Width(), rcChild.Height(), SWP_SHOWWINDOW );
				rc.SubtractRect(rc, rcChild);

				// Draw the bar
				CRect rcBar = rc;
				UINT DockSide = m_vDockChildren[u]->GetDockState();
				if (DS_DOCKED_LEFT   == DockSide) rcBar.right  = rcBar.left + m_BarWidth;
				if (DS_DOCKED_RIGHT  == DockSide) rcBar.left   = rcBar.right - m_BarWidth;
				if (DS_DOCKED_TOP    == DockSide) rcBar.bottom = rcBar.top + m_BarWidth;
				if (DS_DOCKED_BOTTOM == DockSide) rcBar.top    = rcBar.bottom - m_BarWidth;

				hdwp = ::DeferWindowPos(hdwp, m_vDockChildren[u]->m_Bar.GetHwnd(), NULL, rcBar.left, rcBar.top, rcBar.Width(), rcBar.Height(), SWP_SHOWWINDOW );

				m_vDockChildren[u]->m_Bar.m_pDockable = m_vDockChildren[u];
				rc.SubtractRect(rc, rcBar);
			}
			else
				m_vDockChildren[u]->m_Bar.ShowWindow(SW_HIDE);
		}

		if (IsDocked())
		{
			hdwp = ::DeferWindowPos(hdwp, m_Caption.GetHwnd(), NULL, rc.left, rc.top, rc.Width(), m_NCHeight, SWP_SHOWWINDOW );
			hdwp = ::DeferWindowPos(hdwp, m_pView->GetHwnd(), NULL, rc.left, rc.top + m_NCHeight, rc.Width() , rc.Height()- m_NCHeight, SWP_SHOWWINDOW );
		}
		else
		{
			m_Caption.ShowWindow(SW_HIDE);
			hdwp = ::DeferWindowPos(hdwp, m_pView->GetHwnd(), NULL, rc.left, rc.top, rc.Width() , rc.Height(), SWP_SHOWWINDOW );
		}
		
		EndDeferWindowPos(hdwp);
	}

	void CDockable::SendNotify(UINT nMessageID)
	{
		// Send a docking notification to the parent window
		DRAGPOS DragPos;
		DragPos.hdr.code = nMessageID;
		DragPos.hdr.hwndFrom = m_hWnd;
		GetCursorPos(&DragPos.ptPos);
		if (m_pDockParent)
			SendMessage(m_pDockParent->GetHwnd(), WM_NOTIFY, 0, (LPARAM)&DragPos);
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
		m_pDockParent->RecalcDockLayout();
		SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED | SWP_DRAWFRAME);
	//	m_pDockParent = NULL;
	}

	LRESULT CDockable::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		static BOOL IsNcLButtonDown = FALSE;
		static CPoint Oldpt;

		switch (uMsg)
		{
		case WM_EXITSIZEMOVE:
			SendNotify(DN_DOCK_END);
			break;
		case WM_MOUSEACTIVATE:
			if (m_pDockParent)
				SendNotify(DN_FOCUSCHANGE);
			else
				RecalcDockLayout();
			break;
		case WM_LBUTTONUP:
			{
				IsNcLButtonDown = FALSE;
			}
			break;

		case WM_WINDOWPOSCHANGED:
			{
				if (!IsDocked())
				{
					// Send a DN_DOCK_MOVE notification to the frame
					SendNotify(DN_DOCK_MOVE);
				}

				// Reposition the dock children
				RecalcDockLayout();
			}
			break;

		case DN_CANDOCKHERE:
			return TRUE;

		}

		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

	void CDockBar::SendNotify(UINT nMessageID)
	{
		// Send a splitter bar notification to the frame
		DRAGPOS DragPos;
		DragPos.hdr.code = nMessageID;
		DragPos.hdr.hwndFrom = m_hWnd;
		GetCursorPos(&DragPos.ptPos);
		DragPos.ptPos.x += 1;
		SendMessage(m_hWndParent, WM_NOTIFY, 0, (LPARAM)&DragPos);
	}

	LRESULT CDockBar::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_SETCURSOR:
			{
				if (m_pDockable)
				{
					UINT uSide = m_pDockable->m_DockState;
					if ((uSide == DS_DOCKED_LEFT) || (uSide == DS_DOCKED_RIGHT))
						SetCursor(LoadCursor(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_SPLITH)));
					else
						SetCursor(LoadCursor(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_SPLITV)));

					return TRUE;
				}
			}
			break;

		case WM_LBUTTONDOWN:
			SendNotify(DN_BAR_START);
			SetCapture();
			m_IsCaptured = TRUE;
			break;

		case WM_LBUTTONUP:
			if (m_IsCaptured)
			{
				SendNotify(DN_BAR_END);
				ReleaseCapture();
				m_IsCaptured = FALSE;
			}
			break;

		case WM_MOUSEMOVE:
			if (m_IsCaptured)
			{
				SendNotify(DN_BAR_MOVE);
			}
			break;
		}

		// pass unhandled messages on for default processing
		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

}
