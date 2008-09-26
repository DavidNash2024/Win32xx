// Win32++  Version 6.3
// Released: 7th October, 2008 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: http://users.bigpond.net.au/programming/
//
//
// Copyright (c) 2005-2008  David Nash
//
// Permission is hereby granted, free of charge, to
// any person obtaining a copy of this software and
// associated documentation files (the "Software"),
// to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom
// the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice
// shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////


///////////////////////////////////////////////////////
// dockable.h
//  Declaration of the CDockable class

#ifndef DOCKABLE_H
#define DOCKABLE_H


#include "wincore.h"
#include "gdi.h"
#include "Default_Resource.h"


// Docking Styles
#define DS_NOT_DOCKED       0x0000
#define DS_DOCKED_LEFT		0x0001
#define DS_DOCKED_RIGHT		0x0002
#define DS_DOCKED_TOP		0x0004
#define DS_DOCKED_BOTTOM	0x0008
#define DS_NODOCK_LEFT      0x0010
#define DS_NODOCK_RIGHT     0x0020
#define DS_NODOCK_TOP       0x0040
#define DS_NODOCK_BOTTOM    0x0080
#define DS_NO_RESIZE        0x0100
#define DS_NO_AUTO_RESIZE   0x0200
#define DS_NO_CAPTION       0x0400
#define DS_NO_UNDOCK        0x0800
#define DS_CLIENTEDGE       0x1000
#define DS_FLATLOOK         0x2000

// Docking Notifications
#define DN_DOCK_START		WM_APP + 1
#define DN_DOCK_MOVE		WM_APP + 2
#define DN_DOCK_END			WM_APP + 3
#define DN_BAR_START		WM_APP + 4
#define DN_BAR_MOVE			WM_APP + 5
#define DN_BAR_END			WM_APP + 6

// Docking Messages
#define DM_ISDOCKABLE       WM_APP + 7

namespace Win32xx
{

	typedef struct DRAGPOS
	{
		NMHDR hdr;
		POINT ptPos;
	} *LPDRAGPOS;

	// Forward declaration of CDockable, it's defined later
	class CDockable;


	/////////////////////////////////////////
	// Declaration of the CDockBar class
	//  It's the splitter bar that seperates the docked panes. 
	class CDockBar : public CWnd
	{
	public:
		CDockBar();
		virtual ~CDockBar();
		virtual void PreRegisterClass(WNDCLASS& wc);
		virtual void SendNotify(UINT nMessageID);
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		CDockable* GetDock() {return m_pDock;}
		void SetDock(CDockable* pDock) {m_pDock = pDock;}

	private:
		CDockable* m_pDock;
		HBRUSH m_hbrBackground;
	};


	/////////////////////////////////////////
	// Declaration of the CDockClient class
	//  It's a window inside a CDockable which includes all of this docked client.
	//  It's the remaining part of the CDockable that doesn't belong to the CDockable's children.
	//  The Dockable's view window is a child window of CDockClient.
	class CDockClient : public CWnd
	{
	public:
		CDockClient();
		virtual ~CDockClient() {}
		virtual void Draw3DBorder(RECT& Rect);
		virtual BOOL IsLeftButtonDown();
		virtual void PreRegisterClass(WNDCLASS& wc);
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual void SendNotify(UINT nMessageID);
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		CDockable* GetDock() {return m_pDock;}
		void SetDock(CDockable* pDock) {m_pDock = pDock;}

		CDockable* m_pDock;
		CWnd* m_pView;
		int m_NCHeight;
	};

	/////////////////////////////////////////
	// Declaration of the CDockHint class
	//  This class is used to indicate where a window could dock by displaying a blue
	//  tinted window.
	class CDockHint : public CWnd
	{
	public:
		CDockHint();
		virtual ~CDockHint();
		virtual void OnPaint(HDC hDC);
		void SetBitmap(HBITMAP hbmBlueTint);
	
	private:
		HBITMAP m_hbmBlueTint;
	};


	/////////////////////////////////////////
	// Declaration of the CDockable class
	//  A CDockable window allows other CDockable windows to be "docked" inside it.
	//  A CDockable can dock on the top, left, right or bottom side of a parent CDockable.
	//  There is no theoretical limit to the number of CDockables within CDockables.
	class CDockable : public CWnd
	{
	public:
		// Operations
		CDockable();
		virtual ~CDockable();
		virtual CDockable* AddDockChild(CDockable* pDockable, UINT uDockSide, int DockWidth);
		virtual void Dock(CDockable* hDockable, UINT uDockSide);
		virtual void DrawHashBar(HWND hBar, POINT Pos);
		virtual CDockable* GetDockableFromPoint(POINT pt);
		virtual CDockable* GetDockAncestor();
		virtual UINT GetDockSide(LPDRAGPOS pdp);
		virtual void OnCreate();
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual void PreRegisterClass(WNDCLASS &wc);
		virtual void RecalcDockLayout();
		virtual void RecalcDockChildLayout(CRect rc);
		virtual void SendNotify(UINT nMessageID);
		virtual void ShowDockHint(LPDRAGPOS pDragPos);
		void ShowStats();	// To-do: Remove this function
		virtual void UnDock();
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// Attributes
		virtual CDockBar& GetBar() const {return (CDockBar&)m_Bar;}
		CDockable* GetDockParent() const {return m_pDockParent;}
		DWORD GetDockStyle() const {return m_DockStyle;}
		CDockHint& GetDockHint() {return GetDockAncestor()->m_DockHint;}
		int GetDockWidth() const {return m_DockWidth;}
		CWnd* GetView() const {return m_Client.m_pView;}
		BOOL IsDocked() const {return (BOOL)(m_DockStyle & 0xF);}
		void SetDockStyle(DWORD dwDockStyle) {m_DockStyle = dwDockStyle;} //To-do: ensure sane dock styles
		void SetDockWidth(int DockWidth) {m_DockWidth = DockWidth;}
		void SetView(CWnd& View) {m_Client.m_pView = &View;}

	private:
		CDockBar m_Bar;
		CDockHint m_DockHint;
		CDockClient m_Client;
		CDockable* m_pDockParent;
		CDockable* m_pDockAncestor;
		
		std::vector <CDockable*> m_vDockChildren;
		int m_BarWidth;
		int m_DockWidth;
		double m_DockWidthRatio;
		int m_NCHeight;
		DWORD m_DockStyle;
		HBRUSH m_hbrDithered;
		HBITMAP	m_hbm;
		BOOL m_IsDraggingDockable;
		BOOL m_IsInDockZone;

	}; // class CDockable


	/////////////////////////////////////////
	// Definitions for the CDockBar class
	//
	inline CDockBar::CDockBar() : m_pDock(NULL)
	{
		m_hbrBackground = ::CreateSolidBrush(RGB(192,192,192));
	}

	inline CDockBar::~CDockBar()
	{
		::UnregisterClass(_T("Win32++ Bar"), GetApp()->GetInstanceHandle());
		::DeleteObject(m_hbrBackground);
	}

	inline void CDockBar::PreRegisterClass(WNDCLASS& wc)
	{
		wc.lpszClassName = _T("Win32++ Bar");
		wc.hbrBackground = m_hbrBackground;
	}

	inline void CDockBar::SendNotify(UINT nMessageID)
	{
		// Send a splitter bar notification to the parent
		DRAGPOS DragPos;
		DragPos.hdr.code = nMessageID;
		DragPos.hdr.hwndFrom = m_hWnd;
		GetCursorPos(&DragPos.ptPos);
		DragPos.ptPos.x += 1;
		SendMessage(m_hWndParent, WM_NOTIFY, 0, (LPARAM)&DragPos);
	}

	inline LRESULT CDockBar::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_SETCURSOR:
			{
				if (GetDock())
				{
					DWORD dwSide = GetDock()->GetDockStyle() & 0xF;
					if ((dwSide == DS_DOCKED_LEFT) || (dwSide == DS_DOCKED_RIGHT))
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
			}
			break;

		case WM_LBUTTONUP:
			if (GetCapture() == hWnd)
			{
				SendNotify(DN_BAR_END);
				ReleaseCapture();
			}
			break;

		case WM_MOUSEMOVE:
			if (GetCapture() == hWnd)
			{
				SendNotify(DN_BAR_MOVE);
			}
			break;
		}

		// pass unhandled messages on for default processing
		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}


	/////////////////////////////////////////
	// Definitions for the CDockClient class
	//
	inline CDockClient::CDockClient() : m_pView(0), m_NCHeight(20)
	{
	}

	inline void CDockClient::PreRegisterClass(WNDCLASS& wc)
	{
		wc.lpszClassName = _T("Win32++ DockClient");
	}

	inline void CDockClient::PreCreate(CREATESTRUCT& cs)
	{
		cs.dwExStyle = WS_EX_CLIENTEDGE;
	}

	inline BOOL CDockClient::IsLeftButtonDown()
	{
		SHORT state;
		if (GetSystemMetrics(SM_SWAPBUTTON))
			// Mouse buttons are swapped
			state = GetAsyncKeyState(VK_RBUTTON);
		else
			// Mouse buttons are not swapped
			state = GetAsyncKeyState(VK_LBUTTON);

		// returns true if the left mouse button is down
		return (state & 0x8000);
	}

	inline void CDockClient::SendNotify(UINT nMessageID)
	{
		// Fill the DragPos structure with data
		DRAGPOS DragPos;
		DragPos.hdr.code = nMessageID;
		DragPos.hdr.hwndFrom = m_hWnd;
		GetCursorPos(&DragPos.ptPos);

		// Send a DragPos notification to the dockable
		SendMessage(m_hWndParent, WM_NOTIFY, 0, (LPARAM)&DragPos);
	}

	inline LRESULT CDockClient::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		static CPoint Oldpt;

		switch (uMsg)
		{
		case WM_NCACTIVATE:
			break;
		case WM_NCCALCSIZE:
			// Sets the non-client area (and hence sets the client area)
			if (m_pDock->IsDocked())
			{
				LPRECT rc = (LPRECT)lParam;
				rc->top += m_NCHeight;
			}
			break;
		case WM_NCHITTEST:
			// Identify which part of the non-client area the cursor is over
			if (m_pDock->IsDocked())
			{
				CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				MapWindowPoints(NULL, m_hWnd, &pt, 1);

				// Indicate if the point is in the caption
				if (pt.y < 0) return HTCAPTION;
			}
			break;
		case WM_NCLBUTTONDOWN:
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
				// Send a DN_DOCK_START notification to the dockable
				SendNotify(DN_DOCK_START);
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
				// Acquire the DC for our NonClient painting
				CDC dc;
				if (wParam != 1)
					dc = GetDCEx(hWnd, (HRGN)wParam, DCX_WINDOW|DCX_INTERSECTRGN|DCX_PARENTCLIP);
				else
					dc 	= GetWindowDC(hWnd);
				
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

				Draw3DBorder(rc);
			}
			break;	// also do default painting

		case WM_WINDOWPOSCHANGED:
			{
				// Reposition the View window to cover the DockClient's client area
				CRect rc = GetClientRect();
				m_pView->SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
			}
			break;
		case WM_MOUSEACTIVATE:
			// Focus changed, so redraw the captions
			{
				m_pView->SetFocus();
				m_pDock->RecalcDockLayout();
			}
			break;
		}

		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}


	/////////////////////////////////////////
	// Definitions for the CDockHint class
	//
	inline CDockHint::CDockHint() : m_hbmBlueTint(0)
	{
	}
	
	inline CDockHint::~CDockHint()
	{
		if (m_hbmBlueTint) ::DeleteObject(m_hbmBlueTint);
	}

	inline void CDockHint::OnPaint(HDC hDC)
	{

		CRect rc = GetClientRect();
		CDC MemDC = CreateCompatibleDC(hDC);
		MemDC.AttachBitmap(m_hbmBlueTint);
		BitBlt(hDC, 0, 0, rc.Width(), rc.Height(), MemDC, 0, 0, SRCCOPY);
		MemDC.DetachBitmap();

	}

	inline void CDockHint::SetBitmap(HBITMAP hbm)
	{
		if (m_hbmBlueTint) ::DeleteObject(m_hbmBlueTint);
		m_hbmBlueTint = hbm;
	}


	/////////////////////////////////////////
	// Definitions for the CDockable class
	//
	inline CDockable::CDockable() :  m_pDockParent(NULL), m_BarWidth(4), m_DockWidth(0),
					m_DockWidthRatio(1.0), m_NCHeight(20), m_DockStyle(0)
	{
		WORD HashPattern[] = {0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA};
		m_hbm = ::CreateBitmap (8, 8, 1, 1, HashPattern);
		m_hbrDithered = ::CreatePatternBrush (m_hbm);
		m_Bar.SetDock(this);
		m_Client.SetDock(this);
		m_pDockAncestor = this;
	}

	inline CDockable::~CDockable()
	{
		for (UINT u = 0 ; u < m_vDockChildren.size() ; ++u)
		{
			delete m_vDockChildren[u];
		}

		::DeleteObject(m_hbrDithered);
		::DeleteObject(m_hbm);
	}

	inline CDockable* CDockable::AddDockChild(CDockable* pDockable, UINT uDockSide, int DockWidth)
	{
		// Create the dockable window
		pDockable->SetDockWidth(DockWidth);
		pDockable->SetDockStyle(uDockSide);
		pDockable->Create(m_hWnd);

		// Dock the dockable window
		Dock(pDockable, uDockSide);

		return pDockable;
	}

	inline void CDockable::Dock(CDockable* pDockable, UINT DockStyle)
	{
		// Set the docking relationships
		m_vDockChildren.push_back(pDockable);
		pDockable->SetParent(m_hWnd);
		pDockable->m_pDockParent = this;
		pDockable->m_Bar.SetParent(m_hWnd);

		// Set the dock styles
		DWORD dwStyle = WS_CHILD | WS_VISIBLE;
		pDockable->SetWindowLongPtr(GWL_STYLE, dwStyle);
		pDockable->SetDockStyle(DockStyle);
		
		// Adjust docked width if required
		if (((DockStyle & 0xF)  == DS_DOCKED_LEFT) || ((DockStyle &0xF)  == DS_DOCKED_RIGHT))
		{
			double Width = GetWindowRect().Width();
			if (pDockable->GetDockWidth() > ((Width -m_BarWidth)/2.0))
				pDockable->SetDockWidth(max(m_BarWidth, (int)((Width -m_BarWidth)/2.0)));
			
			pDockable->m_DockWidthRatio = (double)pDockable->GetDockWidth() / (double)GetWindowRect().Width();
		}
		else
		{
			double Height = GetWindowRect().Height();
			if (pDockable->GetDockWidth() > ((Height -m_BarWidth)/2.0))
				pDockable->SetDockWidth(max(m_BarWidth, (int)((Height -m_BarWidth)/2.0)));
			
			pDockable->m_DockWidthRatio = (double)pDockable->GetDockWidth() / (double)GetWindowRect().Height();
		}
		
		// Redraw the docked windows
		pDockable->GetView()->SetFocus();
		RecalcDockLayout();		
	}

	inline void CDockClient::Draw3DBorder(RECT& Rect)
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

	inline void CDockable::DrawHashBar(HWND hBar, POINT Pos)
	{
		// draws a hashed bar while the splitter bar is being dragged
		{
			CDockable* pDock = ((CDockBar*)FromHandle(hBar))->GetDock();
			BOOL bVertical = ((pDock->GetDockStyle() & 0xF) == DS_DOCKED_LEFT) || ((pDock->GetDockStyle() & 0xF) == DS_DOCKED_RIGHT);

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

	inline CDockable* CDockable::GetDockableFromPoint(POINT pt)
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

	inline CDockable* CDockable::GetDockAncestor()
	// The GetDockAncestor function retrieves the pointer to the
	//  ancestor (root dockable parent) of the Dockable.
	{
		return m_pDockAncestor;
	}

	inline UINT CDockable::GetDockSide(LPDRAGPOS pdp)
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

	inline void CDockable::OnCreate()
	{
		m_Bar.Create(m_hWndParent);
		m_Client.Create(m_hWnd);
		GetView()->Create(m_Client.GetHwnd());
		if (SendMessage(m_hWndParent, DM_ISDOCKABLE, 0, 0))
			m_pDockAncestor = ((CDockable*)FromHandle(m_hWndParent))->m_pDockAncestor;
	}

	inline LRESULT CDockable::OnNotify(WPARAM /*wParam*/, LPARAM lParam)
	{
		static CPoint OldPoint;
		LPDRAGPOS pdp = (LPDRAGPOS)lParam;

		switch (((LPNMHDR)lParam)->code)
		{
		case DN_DOCK_START:
			{
				if (IsDocked())
				{
					UnDock();
					m_IsDraggingDockable = TRUE;
					SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(pdp->ptPos.x, pdp->ptPos.y));
				}
			}
			break;

		case DN_DOCK_MOVE:
			{
				UINT uDockSide = GetDockSide((LPDRAGPOS)lParam);

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
				ShowDockHint((LPDRAGPOS)lParam);
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
				GetDockHint().Destroy();
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

				CDockable* pDock = ((CDockBar*)FromHandle(pdp->hdr.hwndFrom))->GetDock();
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

				CDockable* pDock = ((CDockBar*)FromHandle(pdp->hdr.hwndFrom))->GetDock();
				RECT rcDock = pDock->GetWindowRect();
				MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcDock, 2);

				DrawHashBar(pdp->hdr.hwndFrom, pt);

				switch (pDock->GetDockStyle() & 0xF)
				{
				case DS_DOCKED_LEFT:
					pDock->SetDockWidth(pt.x - rcDock.left - m_BarWidth/2);
					pDock->m_DockWidthRatio = (double)pDock->GetDockWidth()/(double)pDock->m_pDockParent->GetWindowRect().Width();
					break;
				case DS_DOCKED_RIGHT:
					pDock->SetDockWidth(rcDock.right - pt.x - m_BarWidth/2);
					pDock->m_DockWidthRatio = (double)pDock->GetDockWidth()/(double)pDock->m_pDockParent->GetWindowRect().Width();
					break;
				case DS_DOCKED_TOP:
					pDock->SetDockWidth(pt.y - rcDock.top - m_BarWidth/2);
					pDock->m_DockWidthRatio = (double)pDock->GetDockWidth()/(double)pDock->m_pDockParent->GetWindowRect().Height();
					break;
				case DS_DOCKED_BOTTOM:
					pDock->SetDockWidth(rcDock.bottom - pt.y - m_BarWidth/2);
					pDock->m_DockWidthRatio = (double)pDock->GetDockWidth()/(double)pDock->m_pDockParent->GetWindowRect().Height();
					break;
				}

				RecalcDockLayout();
			}
			break;
		}

		return 0L;
	}

	inline void CDockable::PreCreate(CREATESTRUCT &cs)
	{
	//	cs.dwExStyle = WS_EX_TOOLWINDOW | WS_EX_CLIENTEDGE;
		cs.dwExStyle = WS_EX_TOOLWINDOW;
	}

	inline void CDockable::PreRegisterClass(WNDCLASS &wc)
	{
		wc.lpszClassName = _T("Win32++ Dockable");
	}

	inline void CDockable::RecalcDockChildLayout(CRect rc)
	{	 
		// This funcion positions the Dockable window, along with the Dockable's
		// dock children, and draws the splitter bars.
		// Note: This function is used recursively.

		// Step 1: Set this Dockable's position	(unless this is the dock ancestor).
		// Note:The dock ancestor is the parent of all dockables. It's size is   
		// not managed here. It's typically set in CFrame::RecalcLayout.
		if (this != GetDockAncestor())
		{
			SetWindowPos(NULL, rc, SWP_SHOWWINDOW|SWP_FRAMECHANGED);
			MapWindowPoints(GetDockParent()->GetHwnd(), GetHwnd(), (LPPOINT)&rc, 2);
		}
			
		// Step 2: Set the position of each Dockable child 
		for (UINT u = 0; u < m_vDockChildren.size(); ++u)
		{
			CRect rcChild = rc;
			double DockWidth;

			// Calculate the size of the Dockable child
			switch (m_vDockChildren[u]->GetDockStyle() & 0xF)
			{
			case DS_DOCKED_LEFT:
				DockWidth = min(m_vDockChildren[u]->m_DockWidthRatio*GetWindowRect().Width(), rcChild.Width());
				rcChild.right = rcChild.left + (int)DockWidth;
				break;
			case DS_DOCKED_RIGHT:
				DockWidth = min(m_vDockChildren[u]->m_DockWidthRatio*GetWindowRect().Width(), rcChild.Width());
				rcChild.left = rcChild.right - (int)DockWidth;
				break;
			case DS_DOCKED_TOP:
				DockWidth = min(m_vDockChildren[u]->m_DockWidthRatio*GetWindowRect().Height(), rcChild.Height());
				rcChild.bottom = rcChild.top + (int)DockWidth;
				break;
			case DS_DOCKED_BOTTOM:
				DockWidth = min(m_vDockChildren[u]->m_DockWidthRatio*GetWindowRect().Height(), rcChild.Height());
				rcChild.top = rcChild.bottom - (int)DockWidth;
				break;
			}

			if (m_vDockChildren[u]->IsDocked())
			{
				// Position the Dockable child recursively (as it might also have Dockable children) 
				m_vDockChildren[u]->RecalcDockChildLayout(rcChild);
				rc.SubtractRect(rc, rcChild);

				// Calculate the dimensions of the splitter bar
				CRect rcBar = rc;
				DWORD DockSide = m_vDockChildren[u]->GetDockStyle() & 0xF;
				if (DS_DOCKED_LEFT   == DockSide) rcBar.right  = rcBar.left + m_BarWidth;
				if (DS_DOCKED_RIGHT  == DockSide) rcBar.left   = rcBar.right - m_BarWidth;
				if (DS_DOCKED_TOP    == DockSide) rcBar.bottom = rcBar.top + m_BarWidth;
				if (DS_DOCKED_BOTTOM == DockSide) rcBar.top    = rcBar.bottom - m_BarWidth;

				// Draw the splitter bar
				m_vDockChildren[u]->m_Bar.SetWindowPos(NULL, rcBar, SWP_SHOWWINDOW|SWP_FRAMECHANGED );
				rc.SubtractRect(rc, rcBar);
			}
		}

		// Step 3: Set the client window position for this Dockable
		if (IsDocked())
		{
			// This dockable's client window occupies the area of the Dockable window
			// that remains after the after the Dockable children and splitter bars have
			// been taken into account.
			m_Client.SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW|SWP_FRAMECHANGED);
		}
		else
		{
			// The dock ancestor (the parent of all Dockables) is always 'undocked'. 
			if (this != GetDockAncestor())
				rc = GetClientRect();

			m_Client.SetWindowPos(NULL, rc, SWP_SHOWWINDOW|SWP_FRAMECHANGED);
		}
	
		ShowStats();
	}

	inline void CDockable::RecalcDockLayout()
	{
		TRACE("RecalcDockLayout\n");
		CRect rc = GetDockAncestor()->GetWindowRect();
		MapWindowPoints(NULL, GetDockAncestor()->GetHwnd(), (LPPOINT)&rc, 2);
		GetDockAncestor()->RecalcDockChildLayout(rc);
		GetDockAncestor()->RedrawWindow();
	}

	inline void CDockable::SendNotify(UINT nMessageID)
	{
		// Send a docking notification to the parent window
		DRAGPOS DragPos;
		DragPos.hdr.code = nMessageID;
		DragPos.hdr.hwndFrom = m_hWnd;
		GetCursorPos(&DragPos.ptPos);

		CDockable* pDock = GetDockableFromPoint(DragPos.ptPos);

		if (pDock)
			pDock->SendMessage(WM_NOTIFY, 0, (LPARAM)&DragPos);

	}
	
	inline void CDockable::ShowDockHint(LPDRAGPOS pDragPos)
	{
		UINT uDockSide = GetDockSide(pDragPos) & 0xF;
		CDockable* pDockTarget = GetDockableFromPoint(pDragPos->ptPos);
		CDockable* pDockDrag = (CDockable*)FromHandle(pDragPos->hdr.hwndFrom);
		if (uDockSide & 0xF)
		{
			// Create the hint window if required
			if (!GetDockHint().IsWindow()) 
			{
				// Calculate the hint window's position
				CRect rcHint; 
				if (pDockTarget == GetDockAncestor())
					rcHint = pDockTarget->m_Client.GetClientRect();
				else
				{
					rcHint = pDockTarget->m_Client.GetWindowRect();
					MapWindowPoints(NULL, pDockTarget->GetHwnd(), (LPPOINT)&rcHint, 2);
				}

				int Width = pDockDrag->GetDockWidth();
				CRect rcDockTarget = pDockTarget->GetWindowRect();			 
				if ((uDockSide  == DS_DOCKED_LEFT) || (uDockSide  == DS_DOCKED_RIGHT))
					Width = min(Width, rcDockTarget.Width()/2);
				else
					Width = min(Width, rcDockTarget.Height()/2);	

				switch (uDockSide)
				{
				case DS_DOCKED_LEFT:
					rcHint.right = rcHint.left + Width;
					break;
				case DS_DOCKED_RIGHT:
					rcHint.left = rcHint.right - Width;
					break;
				case DS_DOCKED_TOP:
					rcHint.bottom = rcHint.top + Width;
					break;
				case DS_DOCKED_BOTTOM:
					rcHint.top = rcHint.bottom - Width;
					break;
				}
				
							
				// Save the Dock window's blue tinted bitmap
				CDC DockTargetDC;
				if (pDockTarget == GetDockAncestor())
					DockTargetDC = GetDC(pDockTarget->m_Client.GetHwnd());
				else
					DockTargetDC = GetWindowDC(pDockTarget->m_Client.GetHwnd());
				
				CDC MemDC = CreateCompatibleDC(DockTargetDC);
				CRect rcDock = pDockTarget->GetClientRect();
				MemDC.CreateCompatibleBitmap(DockTargetDC, rcHint.Width(), rcHint.Height());
				BitBlt(MemDC, 0, 0, rcHint.Width(), rcHint.Height(), DockTargetDC, rcHint.left, rcHint.top, SRCCOPY);
				HBITMAP hbmDock = MemDC.DetachBitmap();
				TintBitmap(hbmDock, -128, -128, 0);
				GetDockHint().SetBitmap(hbmDock);

				// Create the Hint window
				GetDockHint().Create(GetDockAncestor()->GetHwnd());
				if (pDockTarget == GetDockAncestor())
					MapWindowPoints(pDockTarget->m_Client.GetHwnd(), GetDockAncestor()->GetHwnd(), (LPPOINT)&rcHint, 2);
				else
					MapWindowPoints(pDockTarget->GetHwnd(), GetDockAncestor()->GetHwnd(), (LPPOINT)&rcHint, 2);
				
				GetDockHint().SetWindowPos(NULL, rcHint, SWP_SHOWWINDOW|SWP_NOZORDER);
			}
		}
		else if (GetDockHint().IsWindow()) GetDockHint().Destroy();

	}

	inline void CDockable::ShowStats()
	{
		// ToDo:  Remove this function
		TCHAR text[80];
		TCHAR AllText[160];
		wsprintf(text, _T("Dockable: %#08lx"), GetHwnd());
		::SetWindowText(m_hWnd, text);
		lstrcpy(AllText, text);
		
		if (m_pDockParent)
		{
			wsprintf(text, _T("\r\nDockParent: %#08lx"), GetDockParent()->GetHwnd());
			lstrcat(AllText, text);
		}
		else if (this == GetDockAncestor())
			lstrcat(AllText, _T("\r\nThis is the DockAncestor")); 
		
		CRect rc = m_Client.GetClientRect();
		wsprintf(text, _T("\r\n#Children %d \r\nSize %d x %d"), m_vDockChildren.size(), rc.Width(), rc.Height());
		lstrcat(AllText, text);
		if (IsDocked()) lstrcat(AllText, _T("\r\nDocked"));
		::SetWindowText(GetView()->GetHwnd(), AllText);
	}

	inline void CDockable::UnDock()
	{
		if (IsDocked())
		{
			// Promote the first child to replace this Dock parent 
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
		
			if (m_vDockChildren.size() > 0)
			{
				m_vDockChildren[0]->m_DockStyle = m_DockStyle;
				m_vDockChildren[0]->m_DockWidth = m_DockWidth;
				m_vDockChildren[0]->m_DockWidthRatio = m_DockWidthRatio;
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

			// Change the window to an "undocked" style
			DWORD dwStyle = WS_POPUP| WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_VISIBLE;
			SetWindowLongPtr(GWL_STYLE, dwStyle);

			m_Bar.ShowWindow(SW_HIDE);			
			m_vDockChildren.clear();
			m_pDockParent = NULL;		
			m_DockStyle = 0;
			RecalcDockLayout();
			ShowStats();

			// Supress redraw while we reposition the window
			SetRedraw(FALSE);
			CRect rc = m_Client.GetWindowRect();
			SetParent(0);
			SetWindowPos(NULL, rc, SWP_SHOWWINDOW|SWP_FRAMECHANGED);
			SetRedraw(TRUE);
			
			// Redraw all the windows
			GetDockAncestor()->RedrawWindow();
			RedrawWindow();
		}
	}

	inline LRESULT CDockable::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		static CPoint Oldpt;

		switch (uMsg)
		{
		case WM_EXITSIZEMOVE:
			SendNotify(DN_DOCK_END);
			break;

		case WM_WINDOWPOSCHANGED:
			{
				if (!IsDocked() && (hWnd != GetDockAncestor()->GetHwnd()))
				{
					// Send a Move notification to the parent
					SendNotify(DN_DOCK_MOVE);
					CRect rc = GetClientRect();
					m_Client.SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
				}

				if (hWnd == GetDockAncestor()->GetHwnd())
				{
					// Reposition the dock children
					RecalcDockLayout();
				}
			}
			break;

		case DM_ISDOCKABLE:
			// This is a CDockable window
			return TRUE;

		}

		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

} // namespace Win32xx

#endif // DOCKABLE_H

