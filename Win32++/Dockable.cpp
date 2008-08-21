//////////////////////////////////////////////
// DockContainer.cpp
//  Definitions for the CDockable class

#include "../Win32++/Dockable.h"
#include "../Win32++/Default_Resource.h"


namespace Win32xx
{
	BOOL CDockClient::IsLeftButtonDown()
	{
		SHORT state;
		if (GetSystemMetrics(SM_SWAPBUTTON))
			// Mouse buttons are swapped
			state = GetAsyncKeyState(VK_RBUTTON);
		else
			state = GetAsyncKeyState(VK_LBUTTON);    

		return (state & 0x8000);;
	}

	void CDockClient::SendNotify(UINT nMessageID)
	{
		// Send a docking notification to the frame
		DRAGPOS DragPos;
		DragPos.hdr.code = nMessageID;
		DragPos.hdr.hwndFrom = m_hWnd;
		GetCursorPos(&DragPos.ptPos);

		SendMessage(m_hWndParent, WM_NOTIFY, 0, (LPARAM)&DragPos);
	}

	LRESULT CDockClient::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		static CPoint Oldpt;

		switch (uMsg)
		{

		case WM_NCACTIVATE:
			break;
		case WM_NCCALCSIZE:
			// Set the non-client area (This also controls the client area)
			if (m_pDock->IsDocked())
			{
				LPRECT rc = (LPRECT)lParam;
				rc->top += m_NCHeight;
			}
			break;
		case WM_NCHITTEST:
			if (m_pDock->IsDocked())
			{
				CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				MapWindowPoints(NULL, m_hWnd, &pt, 1);
				
				// Indicate if the point is in the caption
				if (pt.y < 0) return HTCAPTION;
			} 
			break;
		case WM_NCLBUTTONDOWN:
			TRACE("WM_NCLBUTTONDOWN\n");
			Oldpt.x = GET_X_LPARAM(lParam);
			Oldpt.y = GET_Y_LPARAM(lParam);
			if (m_pDock->IsDocked())
			{
				CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				MapWindowPoints(NULL, hWnd, &pt, 1);
				m_pView->SetFocus();
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
			}
			break;

		case WM_NCMOUSEMOVE:
			{
				if (m_pDock->IsDocked())
				{
					// Discard phantom mouse move messages
					if ((Oldpt.x == GET_X_LPARAM(lParam)) && (Oldpt.y == GET_Y_LPARAM(lParam)))
						return 0L;

					if (IsLeftButtonDown() && (wParam == HTCAPTION))
					{
						CDockable* pDock = (CDockable*)FromHandle(m_hWndParent);
						pDock->UnDock();
						::PostMessage(m_hWndParent, WM_NCLBUTTONDOWN, wParam, lParam);
					}
				}
				else if (IsLeftButtonDown())
				{
					// We get a WM_NCMOUSEMOVE (not WM_NCLBUTTONUP) when drag of non-docked window ends
					// Send a DN_DOCK_END notification to the frame
					SendNotify(DN_DOCK_END);
				}
			} 
			break;

		case WM_NCPAINT:
			if (m_pDock->IsDocked())
			{
				CDC dc = GetWindowDC(hWnd);
				CRect rc = GetWindowRect();
				int rcAdjust = (GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_CLIENTEDGE)? 2 : 0;
				
				// Set the font for the title
				NONCLIENTMETRICS info = {0};
				info.cbSize = sizeof(info);
				SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
				dc.CreateFontIndirect(&info.lfStatusFont);

				// Set the Colours
				if (m_pView->GetHwnd() == GetFocus())
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
				Rectangle(dc, rcAdjust, rcAdjust, rc.Width() -rcAdjust, m_NCHeight +rcAdjust);
				CRect rcText(4 +rcAdjust, rcAdjust, rc.Width() -4 -rcAdjust, m_NCHeight +rcAdjust);
				::DrawText(dc, _T("Class View - Docking"), -1, &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
			}  
			break;

		case WM_WINDOWPOSCHANGED:
			{
				CRect rc = GetClientRect();
				m_pView->SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
			}
			break;
		case WM_MOUSEACTIVATE:
			// Focus changed, so redraw the captions
			m_pDock->GetDockAncestor()->RecalcDockLayout();
			break;
		}

		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

	CDockable::CDockable() : m_NCHeight(20), m_DockState(0), m_DockWidth(0), m_pDockParent(NULL),
					m_BarWidth(4)
	{
		WORD HashPattern[] = {0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA};
		m_hbm = ::CreateBitmap (8, 8, 1, 1, HashPattern);
		m_hbrDithered = ::CreatePatternBrush (m_hbm);
		m_Bar.m_pDockable = this;
		m_pDockAncestor = this;
		m_Client.SetDock(this);
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

		// Dock the dockable window
		Dock(pDockable, uDockSide);

		return pDockable;
	}

	void CDockable::Dock(CDockable* pDockable, UINT DockState)
	{
		// Set the docking relationships
		m_vDockChildren.push_back(pDockable);
		pDockable->SetParent(m_hWnd);
		pDockable->m_pDockParent = this;
		pDockable->m_Bar.SetParent(m_hWnd);
		
		// Set the dock styles
		DWORD dwStyle = WS_CHILD | WS_VISIBLE;
		pDockable->SetWindowLongPtr(GWL_STYLE, dwStyle);
		pDockable->SetDockState(DockState);

		// Redraw the docked windows
		RecalcDockLayout();
		pDockable->GetView()->SetFocus();
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

	CDockable* CDockable::GetDockableFromPoint(POINT pt)
	// Retrieves the top level Dockable at the given point 
	{
		CDockable* pDock = NULL;
		CRect rc = GetDockAncestor()->GetWindowRect();

		if (PtInRect(&rc, pt))
		{
			pDock=GetDockAncestor();
		}

		HWND hAncestor = GetDockAncestor()->GetHwnd();
		CPoint ptLocal = pt;
		MapWindowPoints(NULL, hAncestor, &ptLocal, 1);
		HWND hDockChild = ChildWindowFromPoint(hAncestor, ptLocal);
		
		while (SendMessage(hDockChild, DM_ISDOCKABLE, 0, 0))
		{
			pDock = (CDockable*)FromHandle(hDockChild);
			ptLocal = pt;
			MapWindowPoints(NULL, hDockChild, &ptLocal, 1);
			hDockChild = ChildWindowFromPoint(hDockChild, ptLocal);
			if (hDockChild == pDock->GetHwnd()) break;
		}

		return pDock;
	}

	CDockable* CDockable::GetDockAncestor()
	// The GetDockAncestor function retrieves the pointer to the 
	//  ancestor (root dockable parent) of the Dockable.
	{
		return m_pDockAncestor;
	}

	UINT CDockable::GetDockSide(LPDRAGPOS pdp)
	{
		//	CRect rcWindow = GetClientRect();
		//	MapWindowPoints(GetHwnd(), NULL, (LPPOINT)&rcWindow, 2);

		CRect rcWindow = GetView()->GetClientRect();
		MapWindowPoints(GetView()->GetHwnd(), NULL, (LPPOINT)&rcWindow, 2); 

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
		m_Client.Create(m_hWnd);
		GetView()->Create(m_Client.GetHwnd());
		if (SendMessage(m_hWndParent, DM_ISDOCKABLE, 0, 0))
			m_pDockAncestor = ((CDockable*)FromHandle(m_hWndParent))->m_pDockAncestor;
		else
			TRACE("This is the Ancestor\n");
	}

	LRESULT CDockable::OnNotify(WPARAM /*wParam*/, LPARAM lParam)
	{
		static CPoint OldPoint;
		LPDRAGPOS pdp = (LPDRAGPOS)lParam;

		switch (((LPNMHDR)lParam)->code)
		{
		case DN_DOCK_START:
			{
				UnDock();
				m_IsDraggingDockable = TRUE;
				SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(pdp->ptPos.x, pdp->ptPos.y));
			}
			break;

		case DN_DOCK_MOVE:
			{
				UINT uDockSide = GetDockSide((LPDRAGPOS)lParam);

				switch (uDockSide)
				{
				case DS_DOCKED_LEFT:
				//	TRACE("Could dock Left\n");
					m_IsInDockZone = TRUE;
					SetCursor(LoadCursor(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_TRACK4WAY)));
					break;
				case DS_DOCKED_RIGHT:
				//	TRACE("Could dock Right\n");
					m_IsInDockZone = TRUE;
					SetCursor(LoadCursor(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_TRACK4WAY)));
					break;
				case DS_DOCKED_TOP:
				//	TRACE("Could dock Top\n");
					m_IsInDockZone = TRUE;
					SetCursor(LoadCursor(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_TRACK4WAY)));
					break;
				case DS_DOCKED_BOTTOM:
				//	TRACE("Could dock Bottom\n");
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
				POINT pt = pdp->ptPos;
				MapWindowPoints(NULL, m_hWnd, &pt, 1);
				DrawHashBar(pdp->hdr.hwndFrom, pt);
				OldPoint = pt;
			}
			break;

		case DN_BAR_MOVE:
			{
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
			//	::SetWindowPos(m_vDockChildren[u]->GetHwnd(), NULL, rcChild.left, rcChild.top, rcChild.Width(), rcChild.Height(), SWP_SHOWWINDOW );
				rc.SubtractRect(rc, rcChild);

				// Draw the bar
				CRect rcBar = rc;
				UINT DockSide = m_vDockChildren[u]->GetDockState();
				if (DS_DOCKED_LEFT   == DockSide) rcBar.right  = rcBar.left + m_BarWidth;
				if (DS_DOCKED_RIGHT  == DockSide) rcBar.left   = rcBar.right - m_BarWidth;
				if (DS_DOCKED_TOP    == DockSide) rcBar.bottom = rcBar.top + m_BarWidth;
				if (DS_DOCKED_BOTTOM == DockSide) rcBar.top    = rcBar.bottom - m_BarWidth;

				hdwp = ::DeferWindowPos(hdwp, m_vDockChildren[u]->m_Bar.GetHwnd(), NULL, rcBar.left, rcBar.top, rcBar.Width(), rcBar.Height(), SWP_SHOWWINDOW );
			//	::SetWindowPos(m_vDockChildren[u]->m_Bar.GetHwnd(), NULL, rcBar.left, rcBar.top, rcBar.Width(), rcBar.Height(), SWP_SHOWWINDOW );
				rc.SubtractRect(rc, rcBar);
			}
		}

		if (IsDocked())
		{
			hdwp = ::DeferWindowPos(hdwp, m_Client.GetHwnd(), NULL, rc.left, rc.top, rc.Width() , rc.Height(), SWP_SHOWWINDOW|SWP_FRAMECHANGED);
		//	::SetWindowPos(m_Client.GetHwnd(), NULL, rc.left, rc.top, rc.Width() , rc.Height(), SWP_SHOWWINDOW|SWP_FRAMECHANGED );
		}
		else
		{
			if (this != GetDockAncestor())
				rc = GetClientRect();

			hdwp = ::DeferWindowPos(hdwp, m_Client.GetHwnd(), NULL, rc.left, rc.top, rc.Width() , rc.Height(), SWP_SHOWWINDOW|SWP_FRAMECHANGED);
		//	::SetWindowPos(m_Client.GetHwnd(), NULL, rc.left, rc.top, rc.Width() , rc.Height(), SWP_SHOWWINDOW|SWP_FRAMECHANGED );	
		}
		
		EndDeferWindowPos(hdwp);
		ShowStats();
	}

	void CDockable::SendNotify(UINT nMessageID)
	{
		// Send a docking notification to the parent window
		DRAGPOS DragPos;
		DragPos.hdr.code = nMessageID;
		DragPos.hdr.hwndFrom = m_hWnd;
		GetCursorPos(&DragPos.ptPos);

		CDockable* pDock = GetDockableFromPoint(DragPos.ptPos);
		
		if (pDock)
			SendMessage(pDock->GetHwnd(), WM_NOTIFY, 0, (LPARAM)&DragPos);

	}

	void CDockable::ShowStats()
	{
		TCHAR text[80];
		HWND hParent = 0;
		if (m_pDockParent) hParent = m_pDockParent->GetHwnd();
		wsprintf(text, "Parent %#08lX,  Children %d", hParent, m_vDockChildren.size());
		if (IsDocked()) lstrcat(text, _T(" Docked"));
		::SetWindowText(GetView()->GetHwnd(), text);
		::SetWindowText(m_hWnd, text);
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
		m_Bar.ShowWindow(SW_HIDE);

		for (UINT u = 0 ; u < m_pDockParent->m_vDockChildren.size(); ++u)
		{
			if (m_pDockParent->m_vDockChildren[u] == this)
			{
				if (m_vDockChildren.size() > 0)
					m_pDockParent->m_vDockChildren[u] = m_vDockChildren[0];
				else
					m_pDockParent->m_vDockChildren.erase(m_pDockParent->m_vDockChildren.begin() + u);
				break;
			}
		}

		// Promote the first child to replace this Dock parent
		if (m_vDockChildren.size() > 0)
		{
			m_vDockChildren[0]->m_DockState = m_DockState;
			m_vDockChildren[0]->m_DockWidth = m_DockWidth;
			m_vDockChildren[0]->m_pDockParent = m_pDockParent;
			m_vDockChildren[0]->SetParent(m_pDockParent->GetHwnd());
			m_vDockChildren[0]->m_Bar.SetParent(m_pDockParent->GetHwnd());			
		}

		// Transfer the remaining dock children to the first dock child
		for (UINT u1 = 1; u1 < m_vDockChildren.size(); ++u1)
		{
			m_vDockChildren[u1]->m_pDockParent = m_vDockChildren[0];
			m_vDockChildren[u1]->SetParent(m_vDockChildren[0]->GetHwnd());
			m_vDockChildren[u1]->m_Bar.SetParent(m_vDockChildren[0]->GetHwnd());
			m_vDockChildren[0]->m_vDockChildren.push_back(m_vDockChildren[u1]);
		}
		
		m_DockState = 0;
		m_vDockChildren.clear();
		m_pDockParent = NULL;		

		GetDockAncestor()->RecalcDockLayout();
		
		// Set the undocked window to the correct size and redraw
		CRect rcClient = m_Client.GetWindowRect();
		SetWindowPos(NULL, rcClient,  SWP_FRAMECHANGED | SWP_DRAWFRAME);
		Invalidate();
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
		
		// To-do : Do we need this case?
		case WM_MOUSEACTIVATE:
			// Focus changed, so redraw the captions
			GetDockAncestor()->RecalcDockLayout();
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
					// Send a Move notification to the parent
					SendNotify(DN_DOCK_MOVE);
				}

				// Reposition the dock children
				RecalcDockLayout();
			}
			break;

		case DM_CANDOCKHERE:
			return TRUE;

		case DM_ISDOCKABLE:
			// This is a CDockable window
			return TRUE;

		}

		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

	void CDockBar::SendNotify(UINT nMessageID)
	{
		// Send a splitter bar notification to the parent
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
					UINT uSide = m_pDockable->GetDockState();
					if ((uSide == DS_DOCKED_LEFT) || (uSide == DS_DOCKED_RIGHT))
						SetCursor(LoadCursor(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_SPLITH)));
					else
						SetCursor(LoadCursor(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_SPLITV)));

					return TRUE;
				}
			}
			break;

		case WM_LBUTTONDOWN:
			{
				SendNotify(DN_BAR_START);
				SetCapture();
				m_IsCaptured = TRUE;
			}
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
