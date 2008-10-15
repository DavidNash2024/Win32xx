// Win32++  Version 6.3
// Released: 15th October, 2008 by:
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
#define DS_NOTDOCKED			0x0000
#define DS_DOCKED_LEFT			0x0001
#define DS_DOCKED_RIGHT			0x0002
#define DS_DOCKED_TOP			0x0004
#define DS_DOCKED_BOTTOM		0x0008
#define DS_NO_DOCKCHILD_LEFT	0x0010	
#define DS_NO_DOCKCHILD_RIGHT	0x0020	
#define DS_NO_DOCKCHILD_TOP		0x0040	
#define DS_NO_DOCKCHILD_BOTTOM	0x0080	
#define DS_NO_RESIZE			0x0100	
#define DS_NO_AUTO_RESIZE		0x0200	//Not Implemented yet
#define DS_NO_CAPTION			0x0400
#define DS_NO_UNDOCK			0x0800
#define DS_CLIENTEDGE			0x1000	
#define DS_FLATLOOK				0x2000	//Not Implemented yet

// Docking Notifications
#define DN_DOCK_START		WM_APP + 1
#define DN_DOCK_MOVE		WM_APP + 2
#define DN_DOCK_END			WM_APP + 3
#define DN_BAR_START		WM_APP + 4
#define DN_BAR_MOVE			WM_APP + 5
#define DN_BAR_END			WM_APP + 6

// Docking Messages
#define DM_ISDOCKABLE       WM_APP + 7
#define DM_MAKETOPMOST      WM_APP + 8

namespace Win32xx
{

	typedef struct DRAGPOS
	{
		NMHDR hdr;
		POINT ptPos;
	} *LPDRAGPOS;


	// A global function to report the state of the left mouse button
	inline BOOL IsLeftButtonDown()
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

	/////////////////////////////////////////
	// Declaration of the CDockable class
	//  A CDockable window allows other CDockable windows to be "docked" inside it.
	//  A CDockable can dock on the top, left, right or bottom side of a parent CDockable.
	//  There is no theoretical limit to the number of CDockables within CDockables.
	class CDockable : public CWnd
	{
	protected:
		//  A nested class for the splitter bar that seperates the docked panes.
		class CDockBar : public CWnd
		{
		public:
			CDockBar();
			virtual ~CDockBar();
			virtual int GetWidth() {return m_DockBarWidth;}
			virtual void OnPaint(HDC hDC);
			virtual void PreRegisterClass(WNDCLASS& wc);
			virtual void SendNotify(UINT nMessageID);
			virtual void SetColor(COLORREF color);
			virtual void SetWidth(int nWidth) {m_DockBarWidth = nWidth;}
			virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

			CDockable* GetDock() {return m_pDock;}
			void SetDock(CDockable* pDock) {m_pDock = pDock;}

		private:
			CDockable* m_pDock;
			HBRUSH m_hbrBackground;
			int m_DockBarWidth;
		};

		//  A nested class for the window inside a CDockable which includes all of this docked client.
		//  It's the remaining part of the CDockable that doesn't belong to the CDockable's children.
		//  The Dockable's view window is a child window of CDockClient.
		class CDockClient : public CWnd
		{
		public:
			CDockClient();
			virtual ~CDockClient() {}
			virtual void Draw3DBorder(RECT& Rect);
			virtual void PreRegisterClass(WNDCLASS& wc);
			virtual void PreCreate(CREATESTRUCT& cs);
			virtual void SendNotify(UINT nMessageID);
			virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

			tString GetCaption() {return m_tsCaption;}
			CDockable* GetDock() {return m_pDock;}
			void SetDock(CDockable* pDock) {m_pDock = pDock;}
			void SetCaption(LPCTSTR szCaption) {m_tsCaption = szCaption;}

			CDockable* m_pDock;
			CWnd* m_pView;
			int m_NCHeight;
		private:
			tString m_tsCaption;
		};

		//  This nested class is used to indicate where a window could dock by
		//  displaying a blue tinted window.
		class CDockHint : public CWnd
		{
		public:
			CDockHint();
			virtual ~CDockHint();
			virtual void OnPaint(HDC hDC);
			virtual void PreCreate(CREATESTRUCT &cs);
			void SetBitmap(HBITMAP hbmBlueTint);
			virtual void ShowHint(CDockable* pDockTarget, CDockable* pDockDrag, UINT uDockSide);

		private:
			HBITMAP m_hbmBlueTint;
		};

		class CDockTargeting : public CWnd
		{
		public:
			CDockTargeting();
			virtual ~CDockTargeting();
			virtual void OnPaint(HDC hDC);
			virtual void OnCreate() {SetRegion();}
			virtual void PreCreate(CREATESTRUCT &cs);
			virtual void SetRegion();
			virtual void ShowTargeting(LPDRAGPOS pDragPos);
			virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		};

	public:
		// Operations
		CDockable();
		virtual ~CDockable();
		virtual CDockable* AddDockedChild(CDockable* pDockable, UINT uDockStyle, int DockWidth);
		virtual CDockable* AddUnDocked(CDockable* pDockable, UINT uDockStyle, int DockWidth, RECT rc);
		virtual void DeleteDockable(CDockable* pDockable);
		virtual void Dock(CDockable* hDockable, UINT uDockSide);
		virtual void DrawHashBar(HWND hBar, POINT Pos);
		virtual CDockable* const GetDockableFromPoint(POINT pt);
		virtual CDockable* const GetDockAncestor();
		virtual void OnCreate();
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual void PreRegisterClass(WNDCLASS &wc);
		virtual void RecalcDockLayout();
		virtual void RecalcDockChildLayout(CRect rc);
		virtual void SendNotify(UINT nMessageID);
		virtual void UnDock();
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// Attributes
		tString GetCaption() {return GetDockClient().GetCaption();}
		virtual CDockBar& GetDockBar() const {return (CDockBar&)m_DockBar;}
		CDockable* GetDockParent() const {return m_pDockParent;}
		DWORD GetDockStyle() const {return m_DockStyle;}
		virtual CDockHint& GetDockHint() const {return m_pDockAncestor->m_DockHint;}
		virtual CDockTargeting& GetDockTargeting() const {return m_pDockAncestor->m_DockTargeting;}
		virtual CDockClient& GetDockClient() const {return (CDockClient&)m_DockClient;}
		int GetDockWidth() const {return m_DockWidth;}
		CWnd* GetView() const {return GetDockClient().m_pView;}
		BOOL IsDocked() const {return m_Docked;}
		virtual void SetBarColor(COLORREF color) {GetDockBar().SetColor(color);}
		virtual int GetBarWidth() {return GetDockBar().GetWidth();}
		virtual void SetBarWidth(int nWidth) {GetDockBar().SetWidth(nWidth);}
		void SetCaption(LPCTSTR szCaption) {GetDockClient().SetCaption(szCaption);}
		void SetDockStyle(DWORD dwDockStyle); 
		void SetDockWidth(int DockWidth) {m_DockWidth = DockWidth;}
		void SetView(CWnd& View) {GetDockClient().m_pView = &View;}

		UINT m_DockZone;
		BOOL m_BlockMove;
		BOOL m_Docked;
	
	private:
		CDockBar m_DockBar;
		CDockHint m_DockHint;
		CDockClient m_DockClient;
		CDockTargeting m_DockTargeting;
		CDockable* m_pDockParent;
		CDockable* m_pDockAncestor;

		std::vector <CDockable*> m_vDockChildren;
		std::vector <CDockable*> m_vAllDockables;	// Only used in DockAncestor
		int m_DockWidth;
		double m_DockWidthRatio;
		int m_NCHeight;
		DWORD m_DockStyle;
		HBRUSH m_hbrDithered;
		HBITMAP	m_hbmHash;

	}; // class CDockable


	/////////////////////////////////////////////////////////////
	// Definitions for the CDockBar class nested within CDockable
	//
	inline CDockable::CDockBar::CDockBar() : m_pDock(NULL), m_DockBarWidth(4)
	{
		m_hbrBackground = ::CreateSolidBrush(RGB(192,192,192));
	}

	inline CDockable::CDockBar::~CDockBar()
	{
		::UnregisterClass(_T("Win32++ Bar"), GetApp()->GetInstanceHandle());
		::DeleteObject(m_hbrBackground);
	}

	inline void CDockable::CDockBar::OnPaint(HDC hDC)
	{
		CRect rcClient = GetClientRect();
		CDC dcView = hDC;
		dcView.AttachBrush(m_hbrBackground);
		PatBlt(dcView, 0, 0, rcClient.Width(), rcClient.Height(), PATCOPY);
		dcView.DetachBrush();
	}

	inline void CDockable::CDockBar::PreRegisterClass(WNDCLASS& wc)
	{
		wc.lpszClassName = _T("Win32++ Bar");
		wc.hbrBackground = m_hbrBackground;
	}

	inline void CDockable::CDockBar::SendNotify(UINT nMessageID)
	{
		// Send a splitter bar notification to the parent
		DRAGPOS DragPos;
		DragPos.hdr.code = nMessageID;
		DragPos.hdr.hwndFrom = m_hWnd;
		GetCursorPos(&DragPos.ptPos);
		DragPos.ptPos.x += 1;
		SendMessage(m_hWndParent, WM_NOTIFY, 0, (LPARAM)&DragPos);
	}

	inline void CDockable::CDockBar::SetColor(COLORREF color)
	{
		// Useful colors:
		// GetSysColor(COLOR_BTNFACE)	// Default Grey
		// RGB(196, 215, 250)			// Default Blue

		if (m_hbrBackground)
			::DeleteObject(m_hbrBackground);

		m_hbrBackground = ::CreateSolidBrush(color);
	}

	inline LRESULT CDockable::CDockBar::WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		{
			switch (uMsg)
			{
			case WM_SETCURSOR:
				{
					if (!(m_pDock->GetDockStyle() & DS_NO_RESIZE))
					{
						DWORD dwSide = GetDock()->GetDockStyle() & 0xF;
						if ((dwSide == DS_DOCKED_LEFT) || (dwSide == DS_DOCKED_RIGHT))
							SetCursor(LoadCursor(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_SPLITH)));
						else
							SetCursor(LoadCursor(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_SPLITV)));

						return TRUE;
					}
					else
						SetCursor(LoadCursor(NULL, IDC_ARROW));
				}
				break;

			case WM_LBUTTONDOWN:
				{
					if (!(m_pDock->GetDockStyle() & DS_NO_RESIZE))
					{
						SendNotify(DN_BAR_START);
						SetCapture();
					}
				}
				break;

			case WM_LBUTTONUP:
				if (!(m_pDock->GetDockStyle() & DS_NO_RESIZE) && (GetCapture() == hWnd))
				{
					SendNotify(DN_BAR_END);
					ReleaseCapture();
				}
				break;

			case WM_MOUSEMOVE:
				if (!(m_pDock->GetDockStyle() & DS_NO_RESIZE) && (GetCapture() == hWnd))
				{
					SendNotify(DN_BAR_MOVE);
				}
				break;
			}
		}

		// pass unhandled messages on for default processing
		return CWnd::WndProcDefault(hWnd, uMsg, wParam, lParam);
	}


	////////////////////////////////////////////////////////////////
	// Definitions for the CDockClient class nested within CDockable
	//
	inline CDockable::CDockClient::CDockClient() : m_pView(0), m_NCHeight(20)
	{
	}

	inline void CDockable::CDockClient::Draw3DBorder(RECT& Rect)
	{
		// Imitates the drawing of the WS_EX_CLIENTEDGE extended style
		// This draws a 2 pixel border around the specified Rect
		CDC dc = GetWindowDC();
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

	inline void CDockable::CDockClient::PreRegisterClass(WNDCLASS& wc)
	{
		wc.lpszClassName = _T("Win32++ DockClient");
	}

	inline void CDockable::CDockClient::PreCreate(CREATESTRUCT& cs)
	{
		DWORD dwStyle = GetDock()->GetDockStyle();
		if ((dwStyle & DS_CLIENTEDGE) || (dwStyle & DS_FLATLOOK))
			cs.dwExStyle = WS_EX_CLIENTEDGE;
	}

	inline void CDockable::CDockClient::SendNotify(UINT nMessageID)
	{
		// Fill the DragPos structure with data
		DRAGPOS DragPos;
		DragPos.hdr.code = nMessageID;
		DragPos.hdr.hwndFrom = m_hWnd;
		GetCursorPos(&DragPos.ptPos);

		// Send a DragPos notification to the dockable
		SendMessage(m_hWndParent, WM_NOTIFY, 0, (LPARAM)&DragPos);
	}

	inline LRESULT CDockable::CDockClient::WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		static CPoint Oldpt;

		if ((0 != m_pDock) && !(m_pDock->GetDockStyle() & DS_NO_CAPTION))
		{
			switch (uMsg)
			{
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
					DefWindowProc(uMsg, wParam, lParam);

					// Acquire the DC for our NonClient painting
					// Note the Microsoft documentation for this neglects to mention DCX_PARENTCLIP
					CDC dc;
					if (wParam != 1)
						dc = GetDCEx((HRGN)wParam, DCX_WINDOW|DCX_INTERSECTRGN|DCX_PARENTCLIP);
					else
						dc 	= GetWindowDC();

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
					::DrawText(dc, m_tsCaption.c_str(), -1, &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

					if (GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_CLIENTEDGE)
						Draw3DBorder(rc);
					
					return 0;
				}
				break;	// also do default painting

			case WM_MOUSEACTIVATE:
				// Focus changed, so redraw the captions
				{
					m_pView->SetFocus();
					m_pDock->RecalcDockLayout();
				}
				break;

			}
		}

		switch (uMsg)
		{
		case WM_WINDOWPOSCHANGED:
			{
				// Reposition the View window to cover the DockClient's client area
				CRect rc = GetClientRect();
				m_pView->SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
			}
			break;
		}
	
		return CWnd::WndProcDefault(hWnd, uMsg, wParam, lParam);
	}


	//////////////////////////////////////////////////////////////
	// Definitions for the CDockHint class nested within CDockable
	//
	inline CDockable::CDockHint::CDockHint() : m_hbmBlueTint(0)
	{
	}

	inline CDockable::CDockHint::~CDockHint()
	{
		if (m_hbmBlueTint) ::DeleteObject(m_hbmBlueTint);
	}

	inline void CDockable::CDockHint::OnPaint(HDC hDC)
	{
		CRect rc = GetClientRect();
		CDC MemDC = CreateCompatibleDC(hDC);
		MemDC.AttachBitmap(m_hbmBlueTint);
		BitBlt(hDC, 0, 0, rc.Width(), rc.Height(), MemDC, 0, 0, SRCCOPY);
		MemDC.DetachBitmap();
	}

	inline void CDockable::CDockHint::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_POPUP;

		// WS_EX_TOOLWINDOW prevents the window being displayed on the taskbar
		cs.dwExStyle = WS_EX_TOOLWINDOW;

		cs.lpszClass = _T("Win32++ DockHint");
	}

	inline void CDockable::CDockHint::SetBitmap(HBITMAP hbm)
	{
		if (m_hbmBlueTint) ::DeleteObject(m_hbmBlueTint);
		m_hbmBlueTint = hbm;
	}

	inline void CDockable::CDockHint::ShowHint(CDockable* pDockTarget, CDockable* pDockDrag, UINT uDockSide)
	{
		if (uDockSide & 0xF)
		{
			// Ensure a new hint window is created if dock side changes
			static UINT uDockSideOld = 0;
			if (uDockSide != uDockSideOld)
			{
				Destroy();
				pDockTarget->RedrawWindow();
			}
			uDockSideOld = uDockSide;

			// Create the hint window if required
			if (!IsWindow())
			{
				// Calculate the hint window's position
				CRect rcHint = pDockTarget->GetDockClient().GetWindowRect();
				if (pDockTarget->GetDockClient().GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_CLIENTEDGE)
					rcHint.InflateRect(-2, -2);
				MapWindowPoints(NULL, pDockTarget->GetHwnd(), (LPPOINT)&rcHint, 2);

				int Width = pDockDrag->GetDockWidth();
				CRect rcDockTarget = pDockTarget->GetDockClient().GetWindowRect();
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

				// Process any queued messages first
				MSG msg;
				while (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}

				// Save the Dock window's blue tinted bitmap
				CDC dcTarget;
				dcTarget = pDockTarget->GetWindowDC();

				CDC dcMem = CreateCompatibleDC(dcTarget);
				CRect rcBitmap = rcHint;
				dcMem.CreateCompatibleBitmap(dcTarget, rcBitmap.Width(), rcBitmap.Height());
				BitBlt(dcMem, 0, 0, rcBitmap.Width(), rcBitmap.Height(), dcTarget, rcBitmap.left, rcBitmap.top, SRCCOPY);
				HBITMAP hbmDock = dcMem.DetachBitmap();
				TintBitmap(hbmDock, -80, -64, 0);
				SetBitmap(hbmDock);

				// Create the Hint window
				if (!IsWindow())
					Create();
				MapWindowPoints(pDockTarget->GetHwnd(), NULL, (LPPOINT)&rcHint, 2);

				SetWindowPos(NULL, rcHint, SWP_SHOWWINDOW|SWP_NOZORDER|SWP_NOACTIVATE);
			}
		}
		else if (IsWindow()) Destroy();
	}


	////////////////////////////////////////////////////////////////
	// Definitions for the CDockTargeting class nested within CDockable
	//
	inline CDockable::CDockTargeting::CDockTargeting()
	{
	}

	inline CDockable::CDockTargeting::~CDockTargeting()
	{
	}

	inline void CDockable::CDockTargeting::OnPaint(HDC hDC)
	{
		HBITMAP hbmCenter = (HBITMAP)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_SDCENTER),
						                  IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		HBITMAP hbmLeft= (HBITMAP)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_SDLEFT),
						                  IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		HBITMAP hbmRight= (HBITMAP)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_SDRIGHT),
						                  IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		HBITMAP hbmTop= (HBITMAP)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_SDTOP),
						                  IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		HBITMAP hbmBottom= (HBITMAP)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_SDBOTTOM),
						                  IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

		DrawBitmap(hDC, 0, 0, 88, 88, hbmCenter, RGB(255,0,255));
		DrawBitmap(hDC, 0, 29, 31, 29, hbmLeft, RGB(255,0,255));
		DrawBitmap(hDC, 29, 0, 29, 31, hbmTop, RGB(255,0,255));
		DrawBitmap(hDC, 55, 29, 31, 29, hbmRight, RGB(255,0,255));
		DrawBitmap(hDC, 29, 55, 29, 31, hbmBottom, RGB(255,0,255));

		::DeleteObject(hbmCenter);
		::DeleteObject(hbmLeft);
		::DeleteObject(hbmRight);
		::DeleteObject(hbmTop);
		::DeleteObject(hbmBottom);
	}


	inline void CDockable::CDockTargeting::PreCreate(CREATESTRUCT &cs)
	{
		cs.cx = 88;
		cs.cy = 88;
		cs.style = WS_POPUP;

		// WS_EX_TOOLWINDOW prevents the window being displayed on the taskbar
		// WS_EX_TOPMOST keeps the window on top of all non Topmost windows
		cs.dwExStyle = WS_EX_TOPMOST|WS_EX_TOOLWINDOW;

		cs.lpszClass = _T("Win32++ DockTargeting");
	}

	inline void CDockable::CDockTargeting::SetRegion()
	{
		// Use a region to create an irregularly shapped window
		POINT ptArray[16] = { {0,29}, {22, 29}, {29, 22}, {29, 0},
		                      {58, 0}, {58, 22}, {64, 29}, {87, 29},
		                      {87, 58}, {64, 58}, {58, 64}, {58, 87},
		                      {29, 87}, {29, 64}, {23, 58}, {0, 58} };

		HRGN hrgnPoly = CreatePolygonRgn(ptArray, 16, WINDING);
		SetWindowRgn(hrgnPoly, FALSE);

		DeleteObject(hrgnPoly);
	}

	inline void CDockable::CDockTargeting::ShowTargeting(LPDRAGPOS pDragPos)
	{
		CDockable* pDockDrag = (CDockable*)FromHandle(pDragPos->hdr.hwndFrom);
		CDockable* pDockTarget = pDockDrag->GetDockableFromPoint(pDragPos->ptPos);

		int cxImage = 88;
		int cyImage = 88;

		// Ensure a new window if the dock target changes
		static CDockable* pDockTargetOld = 0;
		if (pDockTarget != pDockTargetOld)
			Destroy();

		pDockTargetOld = pDockTarget;

		if (!IsWindow())
			Create();

		CRect rcTarget = pDockTarget->GetDockClient().GetWindowRect();
		int xMid = rcTarget.left + (rcTarget.Width() - cxImage)/2;
		int yMid = rcTarget.top + (rcTarget.Height() - cyImage)/2;

		SetWindowPos(HWND_TOPMOST, xMid, yMid, cxImage, cyImage, SWP_NOACTIVATE|SWP_SHOWWINDOW);

		// Create the docking zone rectangles
		CPoint pt = pDragPos->ptPos;
		MapWindowPoints(NULL, m_hWnd, &pt, 1);
		CRect rcLeft(0, 29, 31, 58);
		CRect rcTop(29, 0, 58, 31);
		CRect rcRight(55, 29, 87, 58);
		CRect rcBottom(29, 55, 58, 87);

		// Test if our cursor is in one of the docking zones
		if ((PtInRect(&rcLeft, pt)) && !(pDockTarget->GetDockStyle() & DS_NO_DOCKCHILD_LEFT))
		{
			pDockDrag->m_BlockMove = TRUE;
			pDockTarget->GetDockHint().ShowHint(pDockTarget, pDockDrag, DS_DOCKED_LEFT);
			pDockTarget->m_DockZone = DS_DOCKED_LEFT;
		}
		else if ((PtInRect(&rcTop, pt)) && !(pDockTarget->GetDockStyle() & DS_NO_DOCKCHILD_TOP))
		{
			pDockDrag->m_BlockMove = TRUE;
			pDockTarget->GetDockHint().ShowHint(pDockTarget, pDockDrag, DS_DOCKED_TOP);
			pDockTarget->m_DockZone = DS_DOCKED_TOP;
		}
		else if ((PtInRect(&rcRight, pt)) && !(pDockTarget->GetDockStyle() & DS_NO_DOCKCHILD_RIGHT))
		{
			pDockDrag->m_BlockMove = TRUE;
			pDockTarget->GetDockHint().ShowHint(pDockTarget, pDockDrag, DS_DOCKED_RIGHT);
			pDockTarget->m_DockZone = DS_DOCKED_RIGHT;
		}
		else if ((PtInRect(&rcBottom, pt)) && !(pDockTarget->GetDockStyle() & DS_NO_DOCKCHILD_BOTTOM))
		{
			pDockDrag->m_BlockMove = TRUE;
			pDockTarget->GetDockHint().ShowHint(pDockTarget, pDockDrag, DS_DOCKED_BOTTOM);
			pDockTarget->m_DockZone = DS_DOCKED_BOTTOM;
		}
		else
		{
			// Not in a docking zone, so clean up
			pDockTarget->GetDockHint().Destroy();
			pDockTarget->m_DockZone = 0;
			pDockDrag->m_BlockMove = FALSE;
		}
	}

	inline LRESULT CDockable::CDockTargeting::WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch(uMsg)
		{
		case WM_ERASEBKGND:
			return 0;
		}
		return CWnd::WndProcDefault(hWnd, uMsg, wParam, lParam);
	}


	/////////////////////////////////////////
	// Definitions for the CDockable class
	//
	inline CDockable::CDockable() :  m_DockZone(0), m_BlockMove(FALSE), m_Docked(FALSE), m_pDockParent(NULL), 
					m_DockWidth(0), m_DockWidthRatio(1.0), m_NCHeight(20), m_DockStyle(0)
	{
		WORD HashPattern[] = {0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA};
		m_hbmHash = ::CreateBitmap (8, 8, 1, 1, HashPattern);
		m_hbrDithered = ::CreatePatternBrush (m_hbmHash);
		m_pDockAncestor = 0;
	}

	inline CDockable::~CDockable()
	{
		::DeleteObject(m_hbrDithered);
		::DeleteObject(m_hbmHash);

		// Ensure all dockables are destroyed
		std::vector <CDockable*>::iterator v;
		if (this == GetDockAncestor())
		{
			while(m_vAllDockables.size() > 0)
			{
				v = GetDockAncestor()->m_vAllDockables.begin();
				delete *v;
				GetDockAncestor()->m_vAllDockables.erase(v);
			}
		}
	}

	inline CDockable* CDockable::AddDockedChild(CDockable* pDockable, UINT uDockStyle, int DockWidth)
	{
		// Create the dockable window
		pDockable->SetDockWidth(DockWidth);
		pDockable->SetDockStyle(uDockStyle);
		pDockable->Create(m_hWnd);

		// Dock the dockable window
		Dock(pDockable, uDockStyle);

		// Store the Dockable's pointer in the DockAncestor's vector for later deletion
		GetDockAncestor()->m_vAllDockables.push_back(pDockable);

		// Issue TRACE warnings for any missing resources
		HMODULE hMod= GetApp()->GetInstanceHandle();
		if (!(uDockStyle & DS_NO_RESIZE))
		{
			if (!FindResource(hMod, MAKEINTRESOURCE(IDW_SPLITH), RT_GROUP_CURSOR))
				TRACE(_T("\n**WARNING** Horizontal cursor resource missing\n"));
			if (!FindResource(hMod, MAKEINTRESOURCE(IDW_SPLITV), RT_GROUP_CURSOR))
				TRACE(_T("\n**WARNING** Vertical cursor resource missing\n"));
		}
		if (!(uDockStyle & DS_NO_UNDOCK))
		{
			if (!FindResource(hMod, MAKEINTRESOURCE(IDW_SDCENTER), RT_BITMAP))
				TRACE(_T("\n**WARNING** Docking center bitmap resource missing\n"));
			if (!FindResource(hMod, MAKEINTRESOURCE(IDW_SDLEFT), RT_BITMAP))
				TRACE(_T("\n**WARNING** Docking left bitmap resource missing\n"));
			if (!FindResource(hMod, MAKEINTRESOURCE(IDW_SDRIGHT), RT_BITMAP))
				TRACE(_T("\n**WARNING** Docking right bitmap resource missing\n"));
			if (!FindResource(hMod, MAKEINTRESOURCE(IDW_SDTOP), RT_BITMAP))
				TRACE(_T("\n**WARNING** Docking top bitmap resource missing\n"));
			if (!FindResource(hMod, MAKEINTRESOURCE(IDW_SDBOTTOM), RT_BITMAP))
				TRACE(_T("\n**WARNING** Docking center bottom resource missing\n"));
		}

		return pDockable;
	}

	inline CDockable* CDockable::AddUnDocked(CDockable* pDockable, UINT uDockStyle, int DockWidth, RECT rc)
	{
		pDockable->SetDockWidth(DockWidth);
		pDockable->SetDockStyle(uDockStyle & 0XFFFFFF0);
		
		// Initially create the Dockable as a child window
		// This makes our Dockable "owned" by the DockAncestor
		pDockable->Create(m_hWnd);
		
		// Change the Dockable to a POPUP window
		DWORD dwStyle = WS_POPUP| WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_VISIBLE;
		pDockable->SetWindowLongPtr(GWL_STYLE, dwStyle);
		pDockable->SetRedraw(FALSE);
		pDockable->SetParent(0);
		pDockable->SetWindowPos(HWND_TOP, rc, SWP_SHOWWINDOW|SWP_FRAMECHANGED);
		pDockable->SetRedraw(TRUE);
		pDockable->RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);

		// Store the Dockable's pointer in the DockAncestor's vector for later deletion
		GetDockAncestor()->m_vAllDockables.push_back(pDockable);
		return pDockable;
	}

	inline void CDockable::DeleteDockable(CDockable* pDockable)
	{
		if ((this == GetDockAncestor()) && (this != pDockable))
		{
			UnDock();
			Destroy();
			std::vector <CDockable*>::iterator v;
			for (v = GetDockAncestor()->m_vAllDockables.begin(); v < GetDockAncestor()->m_vAllDockables.end(); v++)
			{
				if (*v == pDockable)
				{
					delete *v;
					GetDockAncestor()->m_vAllDockables.erase(v);
					break;
				}
			}
		}
		else
			throw CWinException(_T("Invalid use of RemoveDockable"));
	}

	inline void CDockable::Dock(CDockable* pDockable, UINT DockStyle)
	{
		// Set the dock styles
		DWORD dwStyle = WS_CHILD | WS_VISIBLE;
		pDockable->m_BlockMove = FALSE;
		pDockable->SetWindowLongPtr(GWL_STYLE, dwStyle);
		pDockable->SetDockStyle(DockStyle);
		pDockable->SetWindowPos(HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE );

		// Set the docking relationships
		m_vDockChildren.push_back(pDockable);
		pDockable->SetParent(m_hWnd);
		pDockable->m_pDockParent = this;
		pDockable->GetDockBar().SetParent(m_hWnd);

		// Adjust docked width if required
		if (((DockStyle & 0xF)  == DS_DOCKED_LEFT) || ((DockStyle &0xF)  == DS_DOCKED_RIGHT))
		{
			double Width = GetDockClient().GetWindowRect().Width();
			double BarWidth = pDockable->GetBarWidth();
			if (pDockable->GetDockWidth() > (Width/2.0 - BarWidth))
				pDockable->SetDockWidth( (int)(Width/2.0 - BarWidth));

			pDockable->m_DockWidthRatio = ((double)pDockable->GetDockWidth() + BarWidth) / (double)GetWindowRect().Width();
		}
		else
		{
			double Height = GetDockClient().GetWindowRect().Height();
			double BarWidth = pDockable->GetBarWidth();
			if (pDockable->GetDockWidth() > (Height/2.0 - BarWidth))
				pDockable->SetDockWidth( (int)(Height/2.0 - BarWidth));

			pDockable->m_DockWidthRatio = ((double)pDockable->GetDockWidth() + BarWidth) / (double)GetWindowRect().Height();
		}

		// Redraw the docked windows
		pDockable->GetView()->SetFocus();
		pDockable->SetWindowPos(HWND_NOTOPMOST, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE );
		pDockable->m_Docked = TRUE;
		RecalcDockLayout();
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
			int BarWidth = pDock->GetDockBar().GetWidth();

			if (bVertical)
				::PatBlt (BarDC, Pos.x - BarWidth/2, rc.top, BarWidth, cy, PATINVERT);
			else
				::PatBlt (BarDC, rc.left, Pos.y - BarWidth/2, cx, BarWidth, PATINVERT);

			BarDC.DetachBrush();
		}
	}

	inline CDockable* const CDockable::GetDockableFromPoint(POINT pt)
	// Retrieves the top level Dockable at the given point
	{
		CDockable* pDock = NULL;
		CRect rc = GetDockAncestor()->GetDockClient().GetWindowRect();
		if (PtInRect(&rc, pt))
			pDock=GetDockAncestor();

		HWND hAncestor = GetDockAncestor()->GetHwnd();
		CPoint ptLocal = pt;
		MapWindowPoints(NULL, hAncestor, &ptLocal, 1);
		HWND hDockChild = ChildWindowFromPoint(hAncestor, ptLocal);

		CDockable* pDockChild = 0;
		while (SendMessage(hDockChild, DM_ISDOCKABLE, 0, 0))
		{
			pDockChild = (CDockable*)FromHandle(hDockChild);
			ptLocal = pt;
			MapWindowPoints(NULL, hDockChild, &ptLocal, 1);
			hDockChild = ChildWindowFromPoint(hDockChild, ptLocal);
		}

		if (pDockChild)
		{
			rc = pDockChild->GetDockClient().GetWindowRect();
			if (PtInRect(&rc, pt))
				pDock = pDockChild;
		}

		return pDock;
	}

	inline CDockable* const CDockable::GetDockAncestor()
	// The GetDockAncestor function retrieves the pointer to the
	//  ancestor (root dockable parent) of the Dockable.
	{
		return m_pDockAncestor;
	}

	/*
	inline UINT const CDockable::GetDockSide(LPDRAGPOS pdp)
	{
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
	*/

	inline void CDockable::OnCreate()
	{
		GetDockBar().SetDock(this);
		GetDockClient().SetDock(this);
		GetDockBar().Create(m_hWndParent);
		GetDockClient().Create(m_hWnd);
		if (NULL == GetView())
			throw CWinException(_T("No View window assigned to Dockable")); 
		GetView()->Create(GetDockClient().GetHwnd());
		if (SendMessage(m_hWndParent, DM_ISDOCKABLE, 0, 0))
			m_pDockAncestor = ((CDockable*)FromHandle(m_hWndParent))->m_pDockAncestor;
		else
			m_pDockAncestor = this;
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
					SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(pdp->ptPos.x, pdp->ptPos.y));
				}
			}
			break;

		case DN_DOCK_MOVE:
			{
				GetDockTargeting().ShowTargeting((LPDRAGPOS)lParam);
			}
			break;

		case DN_DOCK_END:
			{
				CDockable* pDock = (CDockable*)FromHandle(pdp->hdr.hwndFrom);
				UINT DockZone = m_DockZone;
				m_DockZone = 0;
				pDock->m_DockZone = 0;
				if (DockZone) Dock(pDock, pDock->GetDockStyle() | DockZone);
				GetDockHint().Destroy();
				GetDockTargeting().Destroy();
			}
			break;

		case DN_BAR_START:
			{
				CPoint pt = pdp->ptPos;
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
				double BarWidth = pDock->GetDockBar().GetWidth();

				switch (pDock->GetDockStyle() & 0xF)
				{
				case DS_DOCKED_LEFT:
					pDock->SetDockWidth(pt.x - rcDock.left - (int)(.5 + 1.5*BarWidth));
					pDock->m_DockWidthRatio = ((double)pDock->GetDockWidth() + BarWidth )/((double)pDock->m_pDockParent->GetWindowRect().Width() - BarWidth);
					break;
				case DS_DOCKED_RIGHT:
					pDock->SetDockWidth(rcDock.right - pt.x - (int)(.5 + 1.5*BarWidth));
					pDock->m_DockWidthRatio = ((double)pDock->GetDockWidth() + BarWidth)/((double)pDock->m_pDockParent->GetWindowRect().Width() - BarWidth);
					break;
				case DS_DOCKED_TOP:
					pDock->SetDockWidth(pt.y - rcDock.top - (int)(.5 + 1.5*BarWidth));
					pDock->m_DockWidthRatio = ((double)pDock->GetDockWidth() + BarWidth)/((double)pDock->m_pDockParent->GetWindowRect().Height() - BarWidth);
					break;
				case DS_DOCKED_BOTTOM:
					pDock->SetDockWidth(rcDock.bottom - pt.y - (int)(.5 + 1.5*BarWidth));
					pDock->m_DockWidthRatio = ((double)pDock->GetDockWidth() + BarWidth)/((double)pDock->m_pDockParent->GetWindowRect().Height() - BarWidth);
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
		cs.dwExStyle = WS_EX_TOOLWINDOW;
	}

	inline void CDockable::PreRegisterClass(WNDCLASS &wc)
	{
		wc.lpszClassName = _T("Win32++ Dockable");
	}

	inline void CDockable::RecalcDockChildLayout(CRect rc)
	{
		// This function positions the Dockable window, along with the Dockable's
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
			double BarWidth = m_vDockChildren[u]->GetBarWidth();

			// Calculate the size of the Dockable child
			switch (m_vDockChildren[u]->GetDockStyle() & 0xF)
			{
			case DS_DOCKED_LEFT:
				DockWidth = min(m_vDockChildren[u]->m_DockWidthRatio*(GetWindowRect().Width() - BarWidth), rcChild.Width());
				rcChild.right = rcChild.left + (int)DockWidth;
				break;
			case DS_DOCKED_RIGHT:
				DockWidth = min(m_vDockChildren[u]->m_DockWidthRatio*(GetWindowRect().Width() - BarWidth), rcChild.Width());
				rcChild.left = rcChild.right - (int)DockWidth;
				break;
			case DS_DOCKED_TOP:
				DockWidth = min(m_vDockChildren[u]->m_DockWidthRatio*(GetWindowRect().Height() - BarWidth), rcChild.Height());
				rcChild.bottom = rcChild.top + (int)DockWidth;
				break;
			case DS_DOCKED_BOTTOM:
				DockWidth = min(m_vDockChildren[u]->m_DockWidthRatio*(GetWindowRect().Height() - BarWidth), rcChild.Height());
				rcChild.top = rcChild.bottom - (int)DockWidth;
				break;
			}

			if (m_vDockChildren[u]->GetDockStyle() & 0xF)
			{
				// Position the Dockable child recursively (as it might also have Dockable children)
				m_vDockChildren[u]->RecalcDockChildLayout(rcChild);
				rc.SubtractRect(rc, rcChild);

				// Calculate the dimensions of the splitter bar
				CRect rcBar = rc;
				DWORD DockSide = m_vDockChildren[u]->GetDockStyle() & 0xF;

				if (DS_DOCKED_LEFT   == DockSide) rcBar.right  = rcBar.left + m_vDockChildren[u]->GetBarWidth();
				if (DS_DOCKED_RIGHT  == DockSide) rcBar.left   = rcBar.right - m_vDockChildren[u]->GetBarWidth();
				if (DS_DOCKED_TOP    == DockSide) rcBar.bottom = rcBar.top + m_vDockChildren[u]->GetBarWidth();
				if (DS_DOCKED_BOTTOM == DockSide) rcBar.top    = rcBar.bottom - m_vDockChildren[u]->GetBarWidth();

				// Draw the splitter bar
				m_vDockChildren[u]->GetDockBar().SetWindowPos(NULL, rcBar, SWP_SHOWWINDOW|SWP_FRAMECHANGED );
				rc.SubtractRect(rc, rcBar);
			}
		}

		// Step 3: Set the client window position for this Dockable
		if (IsDocked())
		{
			// This dockable's client window occupies the area of the Dockable window
			// that remains after the after the Dockable children and splitter bars have
			// been taken into account.
			GetDockClient().SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW|SWP_FRAMECHANGED);
		}
		else
		{
			// The dock ancestor (the parent of all Dockables) is always 'undocked'.
			if (this != GetDockAncestor())
				rc = GetClientRect();

			GetDockClient().SetWindowPos(NULL, rc, SWP_SHOWWINDOW|SWP_FRAMECHANGED);
		}
	}

	inline void CDockable::RecalcDockLayout()
	{
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
		else
		{
			if (GetDockHint().IsWindow())		GetDockHint().Destroy();
			if (GetDockTargeting().IsWindow())	GetDockTargeting().Destroy();
			m_BlockMove = FALSE;
		}
	}

	inline void CDockable::SetDockStyle(DWORD dwDockStyle)
	{
		if ((dwDockStyle & DS_CLIENTEDGE) != (m_DockStyle & DS_CLIENTEDGE))
		{
			if (dwDockStyle & DS_CLIENTEDGE)
			{
				DWORD dwExStyle = GetDockClient().GetWindowLongPtr(GWL_EXSTYLE)|WS_EX_CLIENTEDGE;
				GetDockClient().SetWindowLongPtr(GWL_EXSTYLE, dwExStyle);
				GetDockClient().RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_FRAME);
			}
			else
			{
				DWORD dwExStyle = GetDockClient().GetWindowLongPtr(GWL_EXSTYLE);
				dwExStyle &= ~WS_EX_CLIENTEDGE;
				GetDockClient().SetWindowLongPtr(GWL_EXSTYLE, dwExStyle);
				GetDockClient().RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_FRAME);
			}
		}
		
		m_DockStyle = dwDockStyle;
	}

	inline void CDockable::UnDock()
	{
		if (IsDocked() && !(GetDockStyle() & DS_NO_UNDOCK))
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
				m_vDockChildren[0]->m_DockStyle = (m_vDockChildren[0]->m_DockStyle & 0xFFFFFFF0 ) | (m_DockStyle & 0xF);
				m_vDockChildren[0]->m_DockWidth = m_DockWidth;
				m_vDockChildren[0]->m_DockWidthRatio = m_DockWidthRatio;
				m_vDockChildren[0]->m_pDockParent = m_pDockParent;
				m_vDockChildren[0]->SetParent(m_pDockParent->GetHwnd());
				m_vDockChildren[0]->GetDockBar().SetParent(m_pDockParent->GetHwnd());
			}

			// Transfer the remaining dock children to the first dock child
			for (UINT u1 = 1; u1 < m_vDockChildren.size(); ++u1)
			{
				m_vDockChildren[u1]->m_pDockParent = m_vDockChildren[0];
				m_vDockChildren[u1]->SetParent(m_vDockChildren[0]->GetHwnd());
				m_vDockChildren[u1]->GetDockBar().SetParent(m_vDockChildren[0]->GetHwnd());
				m_vDockChildren[0]->m_vDockChildren.push_back(m_vDockChildren[u1]);
			}

			// Change the window to an "undocked" style
			DWORD dwStyle = WS_POPUP| WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_VISIBLE;
			SetWindowLongPtr(GWL_STYLE, dwStyle);

			GetDockBar().ShowWindow(SW_HIDE);
			m_vDockChildren.clear();
			m_pDockParent = NULL;
			m_DockStyle = m_DockStyle & 0xFFFFFFF0;
			RecalcDockLayout();

			// Supress redraw while we reposition the window
			SetRedraw(FALSE);
			CRect rc = GetDockClient().GetWindowRect();
			SetParent(0);
			SetWindowPos(NULL, rc, SWP_SHOWWINDOW|SWP_FRAMECHANGED| SWP_NOOWNERZORDER);
			SetRedraw(TRUE);

			// Redraw all the windows
			GetDockAncestor()->RedrawWindow();
			CPoint pt;
			pt.GetCursorPos();
			MapWindowPoints(NULL, m_hWnd, &pt, 1);
			::PostMessage(m_hWnd, WM_NCLBUTTONDOWN, (WPARAM)HTCAPTION, (LPARAM)&pt);
			RedrawWindow();
			m_Docked = FALSE;
		}
	}

	inline LRESULT CDockable::WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		static CPoint Oldpt;

		switch (uMsg)
		{
		case WM_ACTIVATE:
			if (!IsDocked())
			{
				if (WA_INACTIVE == LOWORD(wParam))
					PostMessage(DM_MAKETOPMOST, TRUE, 0);
				else
					PostMessage(DM_MAKETOPMOST, FALSE, 0);
			}
			break;

		case WM_EXITSIZEMOVE:
			m_BlockMove = FALSE;
			SendNotify(DN_DOCK_END);
			break;

		case WM_WINDOWPOSCHANGING:
			{
				// Suspend dock drag moving while over dock zone
				if (m_BlockMove)
				{
                	LPWINDOWPOS pWndPos = (LPWINDOWPOS)lParam;
					pWndPos->flags = SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED| SWP_NOOWNERZORDER;
					return 0;
				}
				break;
			}

		case WM_WINDOWPOSCHANGED:
			{
				if ( !IsDocked() && ( hWnd != GetDockAncestor()->GetHwnd() ) )
				{
					// Send a Move notification to the parent
					if ( IsLeftButtonDown() )
					{
						LPWINDOWPOS wPos = (LPWINDOWPOS)lParam;
						if ((!(wPos->flags & SWP_NOMOVE)) || m_BlockMove)
							SendNotify(DN_DOCK_MOVE);
					}

					CRect rc = GetClientRect();
					GetDockClient().SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
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

		case DM_MAKETOPMOST:
			if (wParam)
			{
				// Make the window topmost
				SetWindowPos(HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_SHOWWINDOW);
			}
			else
			{
				// Make the window non-topmost
				HWND hAncestor = GetAncestor(GetDockAncestor()->GetHwnd());
				::SendMessage(hAncestor, WM_SETREDRAW, (WPARAM)FALSE, 0);
				::SetWindowPos(hAncestor, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_SHOWWINDOW);
				SetWindowPos(HWND_NOTOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);
				::SendMessage(hAncestor, WM_SETREDRAW, (WPARAM)TRUE, 0);
				::RedrawWindow(hAncestor, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
			}
			return 0;
		}

		return CWnd::WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

} // namespace Win32xx

#endif // DOCKABLE_H

