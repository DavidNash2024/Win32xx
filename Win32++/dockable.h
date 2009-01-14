// Win32++  Version 6.4
// Released: ??th January, 2009 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: http://users.bigpond.net.au/programming/
//
//
// Copyright (c) 2005-2009  David Nash
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
#include "toolbar.h"
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
#define DS_DOCKED_CONTAINER		0x4000



namespace Win32xx
{

	class CContainer;

	struct TabPageInfo
	{
		TCHAR szTitle[MAX_MENU_STRING];
		HICON hIcon;
		CContainer* pwndContainer;
	};


	///////////////////////////////////////
	// Declaration of the CContainer class
	//  A CContainer has a tab control, a view window, and optionally a toolbar control.
	//  A top level CContainer can contain other CContainers. Each container (including the 
	//  top level container) is displayed within a tab page.
	class CContainer : public CWnd
	{
	public:
		// Nested class. This is the Wnd for the window displayed on a tab page
		// The toolbar and view window are child windows of the tabpage window.
		class CTabPage : public CWnd
		{
		public:
			CTabPage() : m_pwndView(NULL) {}
			virtual ~CTabPage() {}
			virtual CToolbar& GetToolbar() const		{return (CToolbar&)m_wndToolbar;}
			virtual CWnd* GetView() const		{return m_pwndView;}
			virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
			virtual void OnCreate();
			virtual void PreRegisterClass(WNDCLASS &wc);
			virtual void RecalcLayout();
			virtual void SetView(CWnd& wndView);
			virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		private:
			CToolbar m_wndToolbar;
			CWnd* m_pwndView;
		};

		CContainer();
		virtual ~CContainer();
		virtual void AddContainer(CContainer* pwndContainer);
		virtual void AddToolbarButton(UINT nID);
		virtual CContainer* GetContainer(int iPage);
		virtual std::vector<TabPageInfo> GetAllContainers() const {return m_vTabPageInfo;}
		virtual int GetContainerIndex(CContainer* pwndContainer);
		virtual SIZE GetMaxTabTextSize();
		virtual CTabPage& GetTabPage() const	{return (CTabPage&)m_wndPage;}
		virtual CToolbar& GetToolbar() const	{return m_wndPage.GetToolbar();}
		virtual CWnd* GetView() const			{return GetTabPage().GetView();}
		virtual CWnd* GetCurrentView() const	
		{		
			return m_pwndContainerParent->m_vTabPageInfo[m_iCurrentPage].pwndContainer->GetTabPage().GetView();
		}
		virtual void RemoveContainer(CContainer* pWnd);
		virtual void SelectPage(int iPage);
		virtual void SetTabSize();
		virtual void SetView(CWnd& Wnd);
		
		// Attributes
		LPCTSTR GetTabText() {return m_stTabText.c_str();}
		void SetTabText(LPCTSTR szText) {m_stTabText = szText;}
		HICON GetTabIcon() {return m_hTabIcon;}
		void SetTabIcon(HICON hTabIcon) {m_hTabIcon = hTabIcon;}
		void SetTabIcon(UINT nID_Icon)
		{ m_hTabIcon = LoadIcon(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(nID_Icon)); }
		



	protected:
		virtual void OnCreate();
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void Paint();
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		HIMAGELIST m_himlTab;
		std::vector<UINT> m_vToolbarData;
		std::vector<TabPageInfo> m_vTabPageInfo;
		int m_iCurrentPage;
		CTabPage m_wndPage;
		std::string m_stTabText;
		HICON m_hTabIcon;
		CContainer* m_pwndContainerParent;
		
	};

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
			virtual void DrawCaption(WPARAM wParam, BOOL bFocus);
			virtual void DrawCloseButton(CDC& DrawDC, UINT uState, BOOL bFocus);
			virtual BOOL IsClosing();
			virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
			virtual void PreRegisterClass(WNDCLASS& wc);
			virtual void PreCreate(CREATESTRUCT& cs);
			virtual void SendNotify(UINT nMessageID);
			virtual void SetClosing() {m_bClosing = TRUE;}
			virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

			tString GetCaption() {return m_tsCaption;}
			CDockable* GetDock() {return m_pDock;}
			void SetDock(CDockable* pDock) {m_pDock = pDock;}
			void SetCaption(LPCTSTR szCaption) {m_tsCaption = szCaption;}

			CDockable* m_pDock;
			CWnd* m_pwndView;
			int m_NCHeight;


		private:
			tString m_tsCaption;
			CRect m_rcClose;
			BOOL m_bClosing;
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

		friend class CDockTargeting;    // required by Borland compilers

	public:
		// Operations
		CDockable();
		virtual ~CDockable();
		virtual CDockable* AddDockedChild(CDockable* pDockable, UINT uDockStyle, int DockWidth, int nDockID = 0);
		virtual CDockable* AddUndockedChild(CDockable* pDockable, UINT uDockStyle, int DockWidth, RECT rc, int nDockID = 0);
		virtual void CloseAllDockables();
		virtual void DeleteDockable(CDockable* pDockable);
		virtual void Dock(CDockable* hDockable, UINT uDockSide);
		virtual void DrawHashBar(HWND hBar, POINT Pos);
		virtual CDockable* const GetDockableFromPoint(POINT pt);
		virtual CDockable* const GetDockAncestor();
		virtual CDockable* const GetDockFromID(int n_DockID);
		virtual void OnCreate();
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual void PreRegisterClass(WNDCLASS &wc);
		virtual void RecalcDockLayout();
		virtual void RecalcDockChildLayout(CRect rc);
		virtual void SendNotify(UINT nMessageID);
		virtual void Undock();
		virtual void UndockContainer(CContainer* pContainer);
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// Attributes
		std::vector <CDockable*> const GetAllDockables();
		std::vector <CDockable*> const GetDockChildren() {return m_vDockChildren;}
		tString GetCaption() {return GetDockClient().GetCaption();}
		CDockable* const GetDockable(int nDockID);
		virtual CDockBar& GetDockBar() const {return (CDockBar&)m_wndDockBar;}
		int GetDockID() const {return m_nDockID;}
		CDockable* GetDockParent() const {return m_pwndDockParent;}
		DWORD GetDockStyle() const {return m_DockStyle;}
		virtual CDockHint& GetDockHint() const {return m_pwndDockAncestor->m_wndDockHint;}
		virtual CDockTargeting& GetDockTargeting() const {return m_pwndDockAncestor->m_wndDockTargetting;}
		virtual CDockClient& GetDockClient() const {return (CDockClient&)m_wndDockClient;}
		int GetDockWidth() const {return m_DockWidth;}
		CWnd* GetView() const {return GetDockClient().m_pwndView;}
		BOOL IsClosing() const {return GetDockClient().IsClosing();}
		BOOL IsDocked() const;
	//	BOOL IsChildFocused() const;
		BOOL IsChildOfDockable(HWND hwnd) const;
		BOOL IsUndocked() const;
		virtual void SetBarColor(COLORREF color) {GetDockBar().SetColor(color);}
		virtual int GetBarWidth() {return GetDockBar().GetWidth();}
		virtual void SetBarWidth(int nWidth) {GetDockBar().SetWidth(nWidth);}
		void SetCaption(LPCTSTR szCaption) {GetDockClient().SetCaption(szCaption);}
		void SetDockStyle(DWORD dwDockStyle);
		void SetDockWidth(int DockWidth) {m_DockWidth = DockWidth;}
		void SetView(CWnd& wndView);

	private:
		CDockBar m_wndDockBar;
		CDockHint m_wndDockHint;
		CDockClient m_wndDockClient;
		CDockTargeting m_wndDockTargetting;
		CDockable* m_pwndDockParent;
		CDockable* m_pwndDockAncestor;

		std::vector <CDockable*> m_vDockChildren;
		std::vector <CDockable*> m_vAllDockables;	// Only used in DockAncestor

		BOOL m_BlockMove;
		BOOL m_Undocking;
		BOOL m_DragFullWindows;
		int m_DockWidth;
		int m_nDockID;
		int m_NCHeight;
		UINT m_DockZone;
		double m_DockWidthRatio;
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
		::GetCursorPos(&DragPos.ptPos);
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
						SendNotify(UWM_BAR_START);
						SetCapture();
					}
				}
				break;

			case WM_LBUTTONUP:
				if (!(m_pDock->GetDockStyle() & DS_NO_RESIZE) && (GetCapture() == hWnd))
				{
					SendNotify(UWM_BAR_END);
					ReleaseCapture();
				}
				break;

			case WM_MOUSEMOVE:
				if (!(m_pDock->GetDockStyle() & DS_NO_RESIZE) && (GetCapture() == hWnd))
				{
					SendNotify(UWM_BAR_MOVE);
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
	inline CDockable::CDockClient::CDockClient() : m_pwndView(0), m_NCHeight(20), m_bClosing(FALSE)
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

	inline void CDockable::CDockClient::DrawCaption(WPARAM wParam, BOOL bFocus)
	{
		// Acquire the DC for our NonClient painting
		// Note the Microsoft documentation for this neglects to mention DCX_PARENTCLIP

		if (!(m_pDock->GetDockStyle() & DS_NO_CAPTION))
		{
			static BOOL bOldFocus = FALSE;
			CDC dc;
			if ((wParam != 1) && (bFocus == bOldFocus))
				dc = GetDCEx((HRGN)wParam, DCX_WINDOW|DCX_INTERSECTRGN|DCX_PARENTCLIP);
			else
				dc 	= GetWindowDC();

			bOldFocus = bFocus;

			CRect rc = GetWindowRect();
			int rcAdjust = (GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_CLIENTEDGE)? 2 : 0;

			// Set the font for the title
			NONCLIENTMETRICS info = {0};
			info.cbSize = sizeof(info);
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
			dc.CreateFontIndirect(&info.lfStatusFont);

			// Set the Colours
			if (bFocus)
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

			// Draw the rectangle
			dc.CreatePen(PS_SOLID, 1, RGB(160, 150, 140));
			Rectangle(dc, rcAdjust, rcAdjust, rc.Width() -rcAdjust, m_NCHeight +rcAdjust);

			// Display the caption
			int cx = GetSystemMetrics(SM_CXSMICON);
			int cy = GetSystemMetrics(SM_CYSMICON);
			CRect rcText(4 +rcAdjust, rcAdjust, rc.Width() -4 - cx -rcAdjust, m_NCHeight +rcAdjust);
			::DrawText(dc, m_tsCaption.c_str(), -1, &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			// Draw the close button
			int gap = 4;
			m_rcClose.right = rc.right - gap;
			m_rcClose.left = m_rcClose.right - cx;
			m_rcClose.top = 2 + rc.top + m_NCHeight/2 - cy/2;
			m_rcClose.bottom = 2 + rc.top + m_NCHeight/2 + cy/2;
			DrawCloseButton(dc, 0, bFocus);
			
			// Draw the 3D border
			if (GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_CLIENTEDGE)
				Draw3DBorder(rc);
		}
	}

	inline void CDockable::CDockClient::DrawCloseButton(CDC& DrawDC, UINT uState, BOOL bFocus)
	{
		CRect rcClose = m_rcClose;
		MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcClose, 2);
		rcClose.OffsetRect(2, m_NCHeight+2);

		if (!IsRectEmpty(&rcClose))
		{
			switch (uState)
			{
			case 1:
				{
					// Draw outline, white at top, black on bottom
					DrawDC.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					::MoveToEx(DrawDC, rcClose.left, rcClose.bottom, NULL);
					::LineTo(DrawDC, rcClose.right, rcClose.bottom);
					::LineTo(DrawDC, rcClose.right, rcClose.top);
					DrawDC.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
					::LineTo(DrawDC, rcClose.left, rcClose.top);
					::LineTo(DrawDC, rcClose.left, rcClose.bottom);
				}

				break;
			case 2:
				{
					// Draw outline, black on top, white on bottom
					DrawDC.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
					::MoveToEx(DrawDC, rcClose.left, rcClose.bottom, NULL);
					::LineTo(DrawDC, rcClose.right, rcClose.bottom);
					::LineTo(DrawDC, rcClose.right, rcClose.top);
					DrawDC.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					::LineTo(DrawDC, rcClose.left, rcClose.top);
					::LineTo(DrawDC, rcClose.left, rcClose.bottom);
				}
				break;
			}

			if (bFocus)
				DrawDC.CreatePen(PS_SOLID, 1, RGB(230, 230, 230));
			else
				DrawDC.CreatePen(PS_SOLID, 1, RGB(64, 64, 64));

			// Manually Draw Close Button
			::MoveToEx(DrawDC, rcClose.left + 3, rcClose.top +3, NULL);
			::LineTo(DrawDC, rcClose.right - 3, rcClose.bottom -3);

			::MoveToEx(DrawDC, rcClose.left + 4, rcClose.top +3, NULL);
			::LineTo(DrawDC, rcClose.right - 3, rcClose.bottom -4);

			::MoveToEx(DrawDC, rcClose.left + 3, rcClose.top +4, NULL);
			::LineTo(DrawDC, rcClose.right - 4, rcClose.bottom -3);

			::MoveToEx(DrawDC, rcClose.right -4, rcClose.top +3, NULL);
			::LineTo(DrawDC, rcClose.left + 2, rcClose.bottom -3);

			::MoveToEx(DrawDC, rcClose.right -4, rcClose.top +4, NULL);
			::LineTo(DrawDC, rcClose.left + 3, rcClose.bottom -3);

			::MoveToEx(DrawDC, rcClose.right -5, rcClose.top +3, NULL);
			::LineTo(DrawDC, rcClose.left + 2, rcClose.bottom -4);
		}
	}

	inline BOOL CDockable::CDockClient::IsClosing()
	{
		return m_bClosing;
	}

	inline LRESULT CDockable::CDockClient::OnNotify(WPARAM /*wParam*/, LPARAM lParam)
	{
		switch (((LPNMHDR)lParam)->code)
		{
		case NM_SETFOCUS:
			if (m_pDock->IsDocked() && !(m_pDock->GetDockStyle() & DS_NO_CAPTION))
				DrawCaption((WPARAM)1, TRUE);
			break;
		case UWM_FRAMELOSTFOCUS:
			if (m_pDock->IsDocked() && !(m_pDock->GetDockStyle() & DS_NO_CAPTION))
				DrawCaption((WPARAM)1, FALSE);
			break;
		}

		return 0;
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
		::GetCursorPos(&DragPos.ptPos);

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

					// Indicate if the point is in the close button
					if (m_rcClose.PtInRect(pt))
						return HTCLOSE;

					MapWindowPoints(NULL, m_hWnd, &pt, 1);

					// Indicate if the point is in the caption
					if (pt.y < 0)
						return HTCAPTION;
				}
				break;

			case WM_NCLBUTTONDOWN:
				if (HTCLOSE == wParam) m_bClosing = TRUE;
				else	m_bClosing = FALSE;

				Oldpt.x = GET_X_LPARAM(lParam);
				Oldpt.y = GET_Y_LPARAM(lParam);
				if (m_pDock->IsDocked())
				{
					CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
					MapWindowPoints(NULL, hWnd, &pt, 1);
					
					m_pwndView->SetFocus();
					
					return 0L;
				}
				break;
			
			case WM_NCLBUTTONUP:
				if ((HTCLOSE == wParam) && m_bClosing)
				{
					// Process this message first
					DefWindowProc(uMsg, wParam, lParam);

					// Now destroy the dockable
					CDockable* pDock = (CDockable*)FromHandle(m_hWndParent);
					pDock->Undock();
					pDock->Destroy();
					return 0;
				}
				m_bClosing = FALSE;
				break;

			case WM_NCMOUSEMOVE:
				{
					if (m_pDock->IsDocked())
					{
						// Discard phantom mouse move messages
						if ( (Oldpt.x == GET_X_LPARAM(lParam) ) && (Oldpt.y == GET_Y_LPARAM(lParam)))
							return 0L;

						if (IsLeftButtonDown() && (wParam == HTCAPTION)  && !m_bClosing)
						{
							CDockable* pDock = (CDockable*)FromHandle(m_hWndParent);
							pDock->Undock();
						}
					}
				}
				break;

			case WM_NCPAINT:
				if (m_pDock->IsDocked())
				{
					DefWindowProc(uMsg, wParam, lParam);
					BOOL bFocus = m_pDock->IsChildOfDockable(GetFocus());
					DrawCaption(wParam, bFocus);
					return 0;
				}
				break;	// also do default painting

			case WM_MOUSEACTIVATE:
				// Focus changed, so redraw the captions
				{
					m_pwndView->SetFocus();
					
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
				m_pwndView->SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
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
		// Display the blue tinted bitmap
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
				pDockDrag->RedrawWindow(0, 0, RDW_FRAME|RDW_INVALIDATE);
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
					Create(pDockTarget->GetHwnd());
				MapWindowPoints(pDockTarget->GetHwnd(), NULL, (LPPOINT)&rcHint, 2);
				SetWindowPos(NULL, rcHint, SWP_SHOWWINDOW|SWP_NOZORDER|SWP_NOACTIVATE);
				
				// Put the dragged window back on top (for Vista AERO)
				pDockDrag->SetWindowPos(HWND_TOP, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
				pDockDrag->RedrawWindow(0, 0, RDW_INVALIDATE|RDW_FRAME|RDW_UPDATENOW);
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
	//	static CDockable* pDockTargetOld = 0;
	//	if (pDockTarget != pDockTargetOld)
	//		Destroy();

	//	pDockTargetOld = pDockTarget;

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

			if (pDockDrag->m_BlockMove)
				pDockDrag->RedrawWindow(0, 0, RDW_FRAME|RDW_INVALIDATE);

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
	inline CDockable::CDockable() : m_pwndDockParent(NULL), m_pwndDockAncestor(NULL), m_BlockMove(FALSE), 
		            m_Undocking(FALSE), m_DragFullWindows(FALSE), m_DockWidth(0), m_nDockID(0), m_NCHeight(20), 
					m_DockZone(0), m_DockWidthRatio(1.0), m_DockStyle(0)
	{
		WORD HashPattern[] = {0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA};
		m_hbmHash = ::CreateBitmap (8, 8, 1, 1, HashPattern);
		m_hbrDithered = ::CreatePatternBrush (m_hbmHash);
		m_pwndDockAncestor = 0;
	}

	inline CDockable::~CDockable()
	{
		::DeleteObject(m_hbrDithered);
		::DeleteObject(m_hbmHash);

		// Ensure all dockables are destroyed		
		std::vector <CDockable*>::iterator v;
		if (this == GetDockAncestor())
		{
			while (m_vAllDockables.size() > 0)
			{
				v = m_vAllDockables.begin();
				(*v)->Destroy();
				delete *v;
				m_vAllDockables.erase(v);
			}
		}
	}

	inline CDockable* CDockable::AddDockedChild(CDockable* pDockable, UINT uDockStyle, int DockWidth, int nDockID /* = 0*/)
	{
		// Create the dockable window with the frame window as its parent. 
		// This pernamently sets the frame window as the dockable window's owner,
		// even when its parent is subsequently changed.
		pDockable->SetDockWidth(DockWidth);
		pDockable->SetDockStyle(uDockStyle);
		pDockable->m_nDockID = nDockID;
		HWND hwndFrame = GetAncestor();
		pDockable->Create(hwndFrame);
		pDockable->SetParent(m_hWnd);
		pDockable->m_pwndDockAncestor = GetDockAncestor();

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

	inline CDockable* CDockable::AddUndockedChild(CDockable* pDockable, UINT uDockStyle, int DockWidth, RECT rc, int nDockID /* = 0*/)
	{
		pDockable->SetDockWidth(DockWidth);
		pDockable->SetDockStyle(uDockStyle & 0XFFFFFF0);
		pDockable->m_nDockID = nDockID;

		// Initially create the as a child window of the frame
		// This makes the frame window the owner of our dockable
	//	pDockable->Create(m_hWnd);
		HWND hwndFrame = GetAncestor();
		pDockable->Create(hwndFrame);
		pDockable->SetParent(m_hWnd);
		pDockable->m_pwndDockAncestor = GetDockAncestor();

		// Change the Dockable to a POPUP window
		DWORD dwStyle = WS_POPUP| WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_VISIBLE;
		pDockable->SetWindowLongPtr(GWL_STYLE, dwStyle);
		pDockable->SetRedraw(FALSE);
		pDockable->SetParent(0);
		pDockable->SetWindowPos(HWND_TOP, rc, SWP_SHOWWINDOW|SWP_FRAMECHANGED);
		pDockable->SetRedraw(TRUE);
		pDockable->RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
		pDockable->SetWindowText(GetCaption().c_str());

		// Store the Dockable's pointer in the DockAncestor's vector for later deletion
		GetDockAncestor()->m_vAllDockables.push_back(pDockable);
		return pDockable;
	}

	inline void CDockable::CloseAllDockables()
	{
		if (this != GetDockAncestor())
			throw CWinException(_T("Must call CloseAllDockables from the DockAncestor"));

		std::vector <CDockable*>::iterator v;

		SetRedraw(FALSE);
		while(m_vAllDockables.size() > 0)
		{
			v = m_vAllDockables.begin();
			if (*v)
			{
				if ((*v)->IsWindow())
				{
					(*v)->GetDockClient().SetClosing();
					(*v)->Undock();
				}
				(*v)->Destroy();
			}
			delete *v;
			m_vAllDockables.erase(v);
		}

		SetRedraw(TRUE);
		RedrawWindow();
	}

	inline void CDockable::DeleteDockable(CDockable* pDockable)
	{
		if ((this == GetDockAncestor()) && (this != pDockable))
		{
			pDockable->Undock();
			pDockable->Destroy();
			std::vector <CDockable*>::iterator v;
			for (v = GetDockAncestor()->m_vAllDockables.begin(); v != GetDockAncestor()->m_vAllDockables.end(); v++)
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
			throw CWinException(_T("Must call RemoveDockable from the DockAncestor"));
	}

	inline void CDockable::Dock(CDockable* pDockable, UINT DockStyle)
	{
		if (DockStyle & DS_DOCKED_CONTAINER)
		{
			// Add a container to an existing docked container
			CContainer* pContainer = (CContainer*)GetView();
			pContainer->AddContainer((CContainer*)pDockable->GetView());
			pDockable->SetWindowLongPtr(GWL_STYLE, WS_CHILD);
			pDockable->SetDockStyle(DockStyle);
			return;
		}

		// Set the dock styles
		GetDockAncestor()->SetRedraw(FALSE);
		DWORD dwStyle = WS_CHILD | WS_VISIBLE;
		pDockable->m_BlockMove = FALSE;
		pDockable->SetWindowLongPtr(GWL_STYLE, dwStyle);
		pDockable->SetDockStyle(DockStyle);

		// Set the docking relationships
		m_vDockChildren.push_back(pDockable);
		pDockable->SetParent(m_hWnd);
		pDockable->m_pwndDockParent = this;
		pDockable->GetDockBar().SetParent(m_hWnd);

		// Limit the docked size to half the parent's size
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
		::SetFocus(GetAncestor());
		pDockable->GetView()->SetFocus();
		GetDockAncestor()->SetRedraw(TRUE);
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

	inline std::vector <CDockable*> const CDockable::GetAllDockables()
	{
		// Clean up m_vAllDockables first by removing destroyed CDockable windows
	//	if (this == GetDockAncestor())
	//	{
			std::vector<CDockable*>::reverse_iterator ritor;;
			for (ritor = GetDockAncestor()->m_vAllDockables.rbegin(); ritor < GetDockAncestor()->m_vAllDockables.rend(); ++ritor)
			{
				// Delete any closed dockables
				if (!(*ritor)->IsWindow() && (this != (*ritor)))
				{
					delete *ritor;
					m_vAllDockables.erase(ritor.base()-1);
				}
			}
	//	}
	//	else
	//		throw CWinException(_T("Must call GetAllDockables from the DockAncestor\n"));

		// now return m_vAllDockables
		return GetDockAncestor()->m_vAllDockables;
	}

	inline CDockable* const CDockable::GetDockable(int nDockID)
	{
		int nDockMax = GetDockAncestor()->m_vAllDockables.size();
		for (int i = 0; i < nDockMax; i++)
		{
			if (nDockID == GetDockAncestor()->m_vAllDockables[i]->m_nDockID)
				return GetDockAncestor()->m_vAllDockables[i];
		}

		return 0;
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
		while (::SendMessage(hDockChild, UWM_IS_DOCKABLE, 0, 0))
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
		return m_pwndDockAncestor;
	}

	inline CDockable* const CDockable::GetDockFromID(int n_DockID)
	{
		std::vector <CDockable*>::iterator v;

		if (GetDockAncestor())
		{
			for (v = GetDockAncestor()->m_vAllDockables.begin(); v != GetDockAncestor()->m_vAllDockables.end(); v++)
			{
				if (n_DockID == (*v)->GetDockID())
					return *v;
			}
		}

		return 0;
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
	
	inline BOOL CDockable::IsDocked() const
	{
		return (((m_DockStyle&0xF) || (m_DockStyle & DS_DOCKED_CONTAINER)) && !m_Undocking); // Boolean expression
	}

/*	inline BOOL CDockable::IsChildFocused() const
	// returns true if this dockable or any of its children has focus
	{
		HWND hwnd = ::GetFocus();
		while (hwnd != NULL)
		{
			if (hwnd == m_hWnd) 
				return TRUE;
			
			if (TRUE == ::SendMessage(hwnd, UWM_IS_DOCKABLE, 0, 0))
				break;

			hwnd = GetParent(hwnd);
		}

		return FALSE;
	}
*/
	inline BOOL CDockable::IsChildOfDockable(HWND hwnd) const
	// returns true if the specified window is a child of this dockable
	{
		while (hwnd != NULL)
		{
			if (hwnd == m_hWnd) 
				return TRUE;
			
			if (TRUE == ::SendMessage(hwnd, UWM_IS_DOCKABLE, 0, 0))
				break;

			hwnd = GetParent(hwnd);
		}

		return FALSE;
	}

	inline BOOL CDockable::IsUndocked() const
	{
		return (!((m_DockStyle&0xF)|| (m_DockStyle & DS_DOCKED_CONTAINER)) && !m_Undocking); // Boolean expression
	}

	inline void CDockable::OnCreate()
	{
		// Create the various child windows
		GetDockBar().SetDock(this);
		GetDockClient().SetDock(this);
		GetDockBar().Create(m_hWndParent);
		GetDockClient().Create(m_hWnd);	
		if (NULL == GetView())
			throw CWinException(_T("CDockable::OnCreate... View window is not assigned!\nUse SetView to set the View Window"));
		GetView()->Create(GetDockClient().GetHwnd());

		// Assume this dockable is the DockAncestor for now.
		m_pwndDockAncestor = this;
		
		// Now remove the WS_POPUP style. It was required to allow this window
		// to be owned by the frame window.
		SetWindowLongPtr(GWL_STYLE, WS_CHILD);
		SetParent(m_hWndParent);
	}

	inline LRESULT CDockable::OnNotify(WPARAM /*wParam*/, LPARAM lParam)
	{
		static CPoint OldPoint;
		LPDRAGPOS pdp = (LPDRAGPOS)lParam;

		switch (((LPNMHDR)lParam)->code)
		{
		case UWM_DOCK_START:
			{
				if (IsDocked())
				{
					Undock();
					SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(pdp->ptPos.x, pdp->ptPos.y));
				}
			}
			break;

		case UWM_DOCK_MOVE:
			{
				GetDockTargeting().ShowTargeting((LPDRAGPOS)lParam);
			}
			break;

		case UWM_DOCK_END:
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

		case UWM_BAR_START:
			{
				CPoint pt = pdp->ptPos;
				MapWindowPoints(NULL, m_hWnd, &pt, 1);
				DrawHashBar(pdp->hdr.hwndFrom, pt);
				OldPoint = pt;
			}
			break;

		case UWM_BAR_MOVE:
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

		case UWM_BAR_END:
			{
				POINT pt = pdp->ptPos;
				MapWindowPoints(NULL, m_hWnd, &pt, 1);

				CDockable* pDock = ((CDockBar*)FromHandle(pdp->hdr.hwndFrom))->GetDock();
				RECT rcDock = pDock->GetWindowRect();
				MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcDock, 2);

				DrawHashBar(pdp->hdr.hwndFrom, pt);
				double dBarWidth = pDock->GetDockBar().GetWidth();
				int iBarWidth    = pDock->GetDockBar().GetWidth();
				int DockWidth;

				switch (pDock->GetDockStyle() & 0xF)
				{
				case DS_DOCKED_LEFT:
					DockWidth = max(pt.x, iBarWidth/2) - rcDock.left - (int)(.5 + 1.5*dBarWidth);
					DockWidth = max(-iBarWidth, DockWidth);
					pDock->SetDockWidth(DockWidth);
					pDock->m_DockWidthRatio = ((double)pDock->GetDockWidth() + dBarWidth )/((double)pDock->m_pwndDockParent->GetWindowRect().Width() - dBarWidth);
					break;
				case DS_DOCKED_RIGHT:
					DockWidth = rcDock.right - max(pt.x, iBarWidth/2) - (int)(.5 + 1.5*dBarWidth);
					DockWidth = max(-iBarWidth, DockWidth);
					pDock->SetDockWidth(DockWidth);
					pDock->m_DockWidthRatio = ((double)pDock->GetDockWidth() + dBarWidth)/((double)pDock->m_pwndDockParent->GetWindowRect().Width() - dBarWidth);
					break;
				case DS_DOCKED_TOP:
					DockWidth = max(pt.y, iBarWidth/2) - rcDock.top - (int)(.5 + 1.5*dBarWidth);
					DockWidth = max(-iBarWidth, DockWidth);
					pDock->SetDockWidth(DockWidth);
					pDock->m_DockWidthRatio = ((double)pDock->GetDockWidth() + dBarWidth)/((double)pDock->m_pwndDockParent->GetWindowRect().Height() - dBarWidth);
					break;
				case DS_DOCKED_BOTTOM:
					DockWidth = rcDock.bottom - max(pt.y, iBarWidth/2) - (int)(.5 + 1.5*dBarWidth);
					DockWidth = max(-iBarWidth, DockWidth);
					pDock->SetDockWidth(DockWidth);
					pDock->m_DockWidthRatio = ((double)pDock->GetDockWidth() + dBarWidth)/((double)pDock->m_pwndDockParent->GetWindowRect().Height() - dBarWidth);
					break;
				}

				RecalcDockLayout();
			}
			break;
		case NM_SETFOCUS:
			{
				if (this == GetDockAncestor())
				{
					std::vector<CDockable*>::iterator iter;
					for (iter = m_vAllDockables.begin(); iter != m_vAllDockables.end(); ++iter)
					{
						if ((*iter)->IsChildOfDockable(::GetFocus()))
						{
							(*iter)->GetDockClient().DrawCaption((WPARAM)1, TRUE);
						}
					}
				}
			}
			break;
		case UWM_FRAMELOSTFOCUS:
			{
				if (this == GetDockAncestor())
				{
					std::vector<CDockable*>::iterator iter;
					for (iter = m_vAllDockables.begin(); iter != m_vAllDockables.end(); ++iter)
					{
						if ((*iter)->IsChildOfDockable(::GetFocus()))
						{
							(*iter)->GetDockClient().DrawCaption((WPARAM)1, FALSE);
						}
					}
				}
			}
		}

		return 0L;

	}

	inline void CDockable::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_POPUP | WS_CHILD;
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

			if (m_vDockChildren[u]->IsDocked())
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
		::GetCursorPos(&DragPos.ptPos);

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

		if (IsWindow())
			RecalcDockLayout();
		m_DockStyle = dwDockStyle;
	}

	inline void CDockable::SetView(CWnd& wndView)
	{
		GetDockClient().m_pwndView = &wndView;
	}

	inline void CDockable::Undock()
	{
		// Undocking isn't supported on Win95
		if (1400 == GetWinVersion()) return;

		if (IsDocked() && !(GetDockStyle() & DS_NO_UNDOCK))
		{
			m_Undocking = TRUE;

			// Get the current mouse position
			CPoint pt = GetCursorPos();

			if (GetDockStyle() & DS_DOCKED_CONTAINER)
			{
				CContainer* pContainer = (CContainer*)GetView();
				pContainer->GetTabPage().SetParent(pContainer->GetHwnd());
			}
			else
			{
				// Promote the first child to replace this Dock parent
				for (UINT u = 0 ; u < m_pwndDockParent->m_vDockChildren.size(); ++u)
				{
					if (m_pwndDockParent->m_vDockChildren[u] == this)
					{
						if (m_vDockChildren.size() > 0)
							m_pwndDockParent->m_vDockChildren[u] = m_vDockChildren[0];
						else
							m_pwndDockParent->m_vDockChildren.erase(m_pwndDockParent->m_vDockChildren.begin() + u);
						break;
					}
				}

				if (m_vDockChildren.size() > 0)
				{
					m_vDockChildren[0]->m_DockStyle = (m_vDockChildren[0]->m_DockStyle & 0xFFFFFFF0 ) | (m_DockStyle & 0xF);
					m_vDockChildren[0]->m_DockWidth = m_DockWidth;
					m_vDockChildren[0]->m_DockWidthRatio = m_DockWidthRatio;
					m_vDockChildren[0]->m_pwndDockParent = m_pwndDockParent;
					m_vDockChildren[0]->SetParent(m_pwndDockParent->GetHwnd());
					m_vDockChildren[0]->GetDockBar().SetParent(m_pwndDockParent->GetHwnd());
				}

				// Transfer the remaining dock children to the first dock child
				for (UINT u1 = 1; u1 < m_vDockChildren.size(); ++u1)
				{
					m_vDockChildren[u1]->m_pwndDockParent = m_vDockChildren[0];
					m_vDockChildren[u1]->SetParent(m_vDockChildren[0]->GetHwnd());
					m_vDockChildren[u1]->GetDockBar().SetParent(m_vDockChildren[0]->GetHwnd());
					m_vDockChildren[0]->m_vDockChildren.push_back(m_vDockChildren[u1]);
				}

			}

			// Position and draw the undocked window, unless it is about to be closed
			if (!IsClosing())
			{
				// Change the window to an "undocked" style
				DWORD dwStyle = WS_POPUP| WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_VISIBLE;
				SetWindowLongPtr(GWL_STYLE, dwStyle);

				GetDockBar().ShowWindow(SW_HIDE);
				m_vDockChildren.clear();
				m_pwndDockParent = NULL;
				m_DockStyle = m_DockStyle & 0xFFFFFFF0;
				m_DockStyle &= ~DS_DOCKED_CONTAINER;
				RecalcDockLayout();

				// Supress redraw while we reposition the window
				SetRedraw(FALSE);
				CRect rc = GetDockClient().GetWindowRect();
				SetParent(0);
				m_Undocking = FALSE;
				SetWindowPos(NULL, rc, SWP_FRAMECHANGED| SWP_NOOWNERZORDER);
				SetWindowText(GetCaption().c_str());

				// Re-enable redraw
				SetRedraw(TRUE);
			}
			else
			{
				GetDockBar().ShowWindow(SW_HIDE);
				RecalcDockLayout();
			}

			// Redraw all the windows
			GetDockAncestor()->RedrawWindow();
			RedrawWindow();

			// Send the undock notification to the frame
			NMHDR nmhdr = {0};
			nmhdr.hwndFrom = m_hWnd;
			nmhdr.code = UWM_UNDOCKED;
			nmhdr.idFrom = m_nDockID;
			HWND hwndFrame = GetDockAncestor()->GetAncestor();
			::SendMessage(hwndFrame, WM_NOTIFY, m_nDockID, (LPARAM)&nmhdr);

			// Initiate the window move
			SetCursorPos(pt.x, pt.y);
			MapWindowPoints(NULL, m_hWnd, &pt, 1);
			PostMessage(WM_SYSCOMMAND, (WPARAM)(SC_MOVE|0x0002), MAKELPARAM(pt.x, pt.y));
		}

		m_Undocking = FALSE;
	}

	inline void CDockable::UndockContainer(CContainer* pContainer)
	{
		// Undocking isn't supported on Win95
		if (1400 == GetWinVersion()) return;

		// Ensure pContainer really points to a container
		if (!(pContainer->IsContainer()))
			throw CWinException(_T("CDockable::UndockContainer... Not a container!")); 
		
		if (GetView() == pContainer)
		{
			// The parent container is being undocked, so we need
			// to transfer our child containers to a different dockable
			
			// Choose a new dockable from among the dockables for child containers
			CDockable* pDockNew = 0;
			std::vector<TabPageInfo>::iterator iter;
			std::vector<TabPageInfo> AllContainers = pContainer->GetAllContainers();
			for (iter = AllContainers.begin(); iter != AllContainers.end(); ++iter)
			{
				if ((*iter).pwndContainer != pContainer)
				{
					pDockNew = (CDockable*)FromHandle(GetParent(GetParent((*iter).pwndContainer->GetHwnd())));				
					break;
				}
			}
			
			// Move containers from the old dockable to the new dockable
			CContainer* pContainerNew = (CContainer*)pDockNew->GetView();
			for (iter = AllContainers.begin(); iter != AllContainers.end(); ++iter)
			{
				if ((*iter).pwndContainer != pContainer)
				{
					pContainer->RemoveContainer((*iter).pwndContainer);
					if (pContainerNew != (*iter).pwndContainer)
						pContainerNew->AddContainer((*iter).pwndContainer);
				}
			}

			// Now swap the dockables over
			pDockNew->m_vDockChildren	= m_vDockChildren;
			pDockNew->m_pwndDockParent	= m_pwndDockParent;
			pDockNew->m_DockStyle		= m_DockStyle;
			pDockNew->m_DockWidth		= m_DockWidth;
			pDockNew->m_DockWidthRatio	= m_DockWidthRatio;
			pDockNew->SetParent(m_hWndParent);
			pDockNew->GetDockBar().SetParent(m_hWndParent);

			// insert pDockNew into the the DockParent's DockChildren vector
			std::vector<CDockable*>::iterator p;
			for (p = pDockNew->m_pwndDockParent->m_vDockChildren.begin(); p != pDockNew->m_pwndDockParent->m_vDockChildren.end(); ++p)
			{
				if (*p == this)
				{
					pDockNew->m_pwndDockParent->m_vDockChildren.insert(p, pDockNew);
					break;
				}
			}
		}
		else
			// This is a child container, so simply remove it from the parent
			((CContainer*)GetView())->RemoveContainer(pContainer);

		// Finally do the actual undocking
		for (int i = 0; i < (int)GetAllDockables().size(); ++i)
		{
			CDockable* pDock = GetAllDockables()[i];
			if (pContainer == pDock->GetView())
			{
				CRect rc = GetDockClient().GetWindowRect();
				MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rc, 2);
				pDock->GetDockClient().SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
				pDock->Undock();
				break;
			}
		}
	}

	inline LRESULT CDockable::WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		static CPoint Oldpt;

		switch (uMsg)
		{
		case UWM_IS_DOCKABLE:	// A message to test if this is a Container window
			return TRUE;
		
		case WM_SYSCOMMAND:
			{
				// Test if the window is being moved
				if ((wParam&0xFFF0) == SC_MOVE)
				{
					BOOL bResult = FALSE;

					if (SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, &bResult, 0))
					{
						// Turn on DragFullWindows during the window move
						m_DragFullWindows = bResult;
						SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, TRUE, 0, 0);
					}
				}
			}
			break; 

		case WM_EXITSIZEMOVE:
			m_BlockMove = FALSE;
			SendNotify(UWM_DOCK_END);
			SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, m_DragFullWindows, 0, 0);
			break;

		case WM_WINDOWPOSCHANGING:
			{
				// Suspend dock drag moving while over dock zone
				if (m_BlockMove)
				{
                	LPWINDOWPOS pWndPos = (LPWINDOWPOS)lParam;
					pWndPos->flags = SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED|SWP_NOOWNERZORDER|SWP_NOREDRAW;
					return 0;
				}
				break;
			}

		case WM_WINDOWPOSCHANGED:
			{
				if ( IsUndocked() && ( hWnd != GetDockAncestor()->GetHwnd() ) )
				{
					// Send a Move notification to the parent
					if ( IsLeftButtonDown() )
					{
						LPWINDOWPOS wPos = (LPWINDOWPOS)lParam;
						if ((!(wPos->flags & SWP_NOMOVE)) || m_BlockMove)
							SendNotify(UWM_DOCK_MOVE);
					}
					else
					{
						if (GetDockTargeting().IsWindow())	GetDockTargeting().Destroy();
						m_BlockMove = FALSE;
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

		case WM_CLOSE:
			ShowWindow(SW_HIDE);
			break;

		case WM_SETFOCUS:
			// Pass focus on the the view window
			GetView()->SetFocus();
			break;
		}

		return CWnd::WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

	///////////////////////////////////////////
	// Declaration of the nested CTabPage class
	inline BOOL CContainer::CTabPage::OnCommand(WPARAM wParam, LPARAM lParam)
	{
		return (BOOL)::SendMessage(m_hWndParent, WM_COMMAND, wParam, lParam);
	}

	inline void CContainer::CTabPage::OnCreate()
	{
		{
			m_pwndView->Create(m_hWnd);
		}
	}

	inline void CContainer::CTabPage::PreRegisterClass(WNDCLASS &wc)
	{
		wc.lpszClassName = _T("Win32++ TabPage");
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	}

	inline void CContainer::CTabPage::RecalcLayout()
	{
		GetToolbar().SendMessage(TB_AUTOSIZE, 0, 0);
		CRect rc = GetClientRect();
		CRect rcToolbar = m_wndToolbar.GetClientRect();
		rc.top += rcToolbar.Height();
		GetView()->SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
	}

	inline void CContainer::CTabPage::SetView(CWnd& wndView)
	// Sets or changes the View window displayed within the frame
	{
		// Assign the view window
		m_pwndView = &wndView;

		if (m_hWnd)
		{
			// The container is already created, so create and position the new view too
			GetView()->Create(m_hWnd);
			RecalcLayout();
		}
	}

	inline LRESULT CContainer::CTabPage::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_SIZE:
			RecalcLayout();
			break;
		case WM_NOTIFY:
			switch (((LPNMHDR)lParam)->code)
			{
			// Send the focus change notifications to the grandparent
			case NM_KILLFOCUS:
			case NM_SETFOCUS:
			case UWM_FRAMELOSTFOCUS:
				::SendMessage(GetParent(m_hWndParent), WM_NOTIFY, wParam, lParam);
				break;
			}

			break;
		}

		// pass unhandled messages on for default processing
		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

	//////////////////////////////////////
	// Declaration of the CContainer class
	inline CContainer::CContainer() : m_iCurrentPage(0), m_hTabIcon(0), m_pwndContainerParent(NULL)
	{
		m_himlTab = ImageList_Create(16, 16, ILC_MASK|ILC_COLOR32, 0, 0);

		// Set the Resource IDs for the toolbar buttons
	//	AddToolbarButton( IDM_FILE_NEW   );
	//	AddToolbarButton( IDM_FILE_OPEN  );
	//	AddToolbarButton( IDM_FILE_SAVE  );
	//	AddToolbarButton( 0 );				// Separator
	//	AddToolbarButton( IDM_EDIT_CUT   );
	//	AddToolbarButton( IDM_EDIT_COPY  );
	//	AddToolbarButton( IDM_EDIT_PASTE );
	//	AddToolbarButton( 0 );				// Separator
	//	AddToolbarButton( IDM_FILE_PRINT );
	//	AddToolbarButton( 0 );				// Separator
	//	AddToolbarButton( IDM_HELP_ABOUT );
	}

	inline CContainer::~CContainer()
	{
		ImageList_Destroy(m_himlTab);
	}

	inline void CContainer::AddContainer(CContainer* pwndContainer)
	{
		TabPageInfo tbi = {0};
		tbi.pwndContainer = pwndContainer;
		lstrcpy(tbi.szTitle, pwndContainer->GetTabText());
		tbi.hIcon = pwndContainer->GetTabIcon();
		int iNewPage = m_vTabPageInfo.size();

		m_vTabPageInfo.push_back(tbi);
		ImageList_AddIcon(m_himlTab, pwndContainer->GetTabIcon());

		if (m_hWnd)
		{
			TCITEM tie = {0};
			tie.mask = TCIF_TEXT | TCIF_IMAGE;
			tie.iImage = iNewPage;
			tie.pszText = m_vTabPageInfo[iNewPage].szTitle;
			TabCtrl_InsertItem(m_hWnd, iNewPage, &tie);

			SetTabSize();
		}

		pwndContainer->m_pwndContainerParent = this;
	}

	inline void CContainer::AddToolbarButton(UINT nID)
	// Adds Resource IDs to toolbar buttons.
	// A resource ID of 0 is a separator
	{
		m_vToolbarData.push_back(nID);
	}

	inline CContainer* CContainer::GetContainer(int iPage)
	{
		if ((iPage >= 0) && (iPage < (int)m_vTabPageInfo.size()))
			return m_vTabPageInfo[iPage].pwndContainer;
		else
			return NULL;
	}

	inline int CContainer::GetContainerIndex(CContainer* pwndContainer)
	{
		for (int i = 0; i < (int)m_vTabPageInfo.size(); ++i)
		{
			if (m_vTabPageInfo[i].pwndContainer == pwndContainer)
				return i;
		}

		return -1;
	}

	inline SIZE CContainer::GetMaxTabTextSize()
	{
		CSize Size;

		// Allocate an iterator for the TabPageInfo vector
		std::vector<TabPageInfo>::iterator iter;

		for (iter = m_vTabPageInfo.begin(); iter != m_vTabPageInfo.end(); ++iter)
		{
			CSize TempSize;
			CDC dc = GetDC();
			NONCLIENTMETRICS info = {0};
			info.cbSize = sizeof(info);
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
			dc.CreateFontIndirect(&info.lfStatusFont);
			GetTextExtentPoint32(dc, iter->szTitle, lstrlen(iter->szTitle), &TempSize);
			if (TempSize.cx > Size.cx)
				Size = TempSize;
		}

		return Size;
	}

	inline void CContainer::OnCreate()
	{		
		if (NULL == GetView())
			throw CWinException(_T("CContainer::OnCreate... View window not assigned!\nUse SetView to set the View Window"));
				
		// Create the page window
		m_wndPage.Create(m_hWnd);

		// Create the toolbar
		if (m_vToolbarData.size() > 0)
		{
			GetToolbar().Create(m_wndPage.GetHwnd());
			DWORD style = (DWORD)GetToolbar().GetWindowLongPtr(GWL_STYLE);
			style |= CCS_NODIVIDER ;//| CCS_NORESIZE;
			GetToolbar().SetWindowLongPtr(GWL_STYLE, style);
			int iButtons = GetToolbar().SetButtons(m_vToolbarData);

			// Set the toolbar images
			// A mask of 192,192,192 is compatible with AddBitmap (for Win95)
			GetToolbar().SetImages(iButtons, RGB(192,192,192), IDW_MAIN, 0, 0);
			GetToolbar().SendMessage(TB_AUTOSIZE, 0, 0);
		}

		for (int i = 0; i < (int)m_vTabPageInfo.size(); ++i)
		{
			// Add tabs for each view.
			TCITEM tie = {0};
			tie.mask = TCIF_TEXT | TCIF_IMAGE;
			tie.iImage = i;
			tie.pszText = m_vTabPageInfo[i].szTitle;
			TabCtrl_InsertItem(m_hWnd, i, &tie);
		}

		m_pwndContainerParent = this;
	}

	inline LRESULT CContainer::OnNotifyReflect(WPARAM /*wParam*/, LPARAM lParam)
	{
		switch (((LPNMHDR)lParam)->code)
		{
		case TCN_SELCHANGE:
			{
				// Display the newly selected tab page
				int iPage = TabCtrl_GetCurSel(m_hWnd);
				SelectPage(iPage);
			}
			break;
		}

		return 0L;
	}

	inline void CContainer::Paint()
	{
		
		// Microsoft's drawing for a tab control is rubbish, so we do our own.
		// We use double buffering and regions to eliminate flicker

		// Create the memory DC and bitmap
		CDC dcMem = ::CreateCompatibleDC(NULL);
		CRect rcClient = GetClientRect();
		CDC dcView = GetDC();
		dcMem.CreateCompatibleBitmap(dcView, rcClient.Width(), rcClient.Height());

		// Create a clipping region. Its the overall tab window's region,
		//  less the region belonging to the individual tab view's client area
		HRGN hrgnSrc1 = ::CreateRectRgn(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
		CRect rcTab = GetClientRect();
		TabCtrl_AdjustRect(m_hWnd, FALSE, &rcTab);
		HRGN hrgnSrc2 = ::CreateRectRgn(rcTab.left, rcTab.top, rcTab.right, rcTab.bottom);
		HRGN hrgnClip = ::CreateRectRgn(0, 0, 0, 0);
		::CombineRgn(hrgnClip, hrgnSrc1, hrgnSrc2, RGN_DIFF); 

		// Use the region in the memory DC to paint the grey background
		dcMem.AttachClipRegion(hrgnClip);
		dcMem.CreateSolidBrush(RGB(230, 230, 230));
		::PaintRgn(dcMem, hrgnClip);

		// Draw the tab buttons:
		for (int i = 0; i < TabCtrl_GetItemCount(m_hWnd); ++i)
		{
			CRect rcItem;
			TabCtrl_GetItemRect(m_hWnd, i, &rcItem);
			if (i == TabCtrl_GetCurSel(m_hWnd))
			{
				dcMem.CreateSolidBrush(RGB(248,248,248));
				SetBkColor(dcMem, RGB(248,248,248));
			}
			else
			{
				dcMem.CreateSolidBrush(RGB(200,200,200));
				SetBkColor(dcMem, RGB(200,200,200));
			}
			dcMem.CreatePen(PS_SOLID, 1, RGB(160, 160, 160));
			RoundRect(dcMem, rcItem.left+1, rcItem.top, rcItem.right+2, rcItem.bottom, 6, 6);

			if (rcItem.Width() >= 24)
			{
				TCHAR szText[30];
				TCITEM tcItem = {0};
				tcItem.mask = TCIF_TEXT | TCIF_IMAGE;
				tcItem.cchTextMax = 30;
				tcItem.pszText = szText;
				TabCtrl_GetItem(m_hWnd, i, &tcItem);

				// Draw the icon
				ImageList_Draw(m_himlTab, tcItem.iImage, dcMem, rcItem.left+5, rcItem.top+2, ILD_NORMAL);

				// Draw the text
				NONCLIENTMETRICS info = {0};
				info.cbSize = sizeof(info);
				SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
				dcMem.CreateFontIndirect(&info.lfStatusFont);
				CRect rcText = rcItem;
				rcText.left += 24;
				::DrawText(dcMem, szText, -1, &rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
			}
		}  

		// Draw a lighter rectangle touching the tab buttons
		CRect rcItem;
		TabCtrl_GetItemRect(m_hWnd, 0, &rcItem);
		int left = rcItem.left +1;
		int right = rcTab.right -1;
		int top = rcTab.bottom;
		int bottom = top + 3;
		dcMem.CreateSolidBrush(RGB(248,248,248));
		dcMem.CreatePen(PS_SOLID, 1, RGB(248,248,248));
		Rectangle(dcMem, left, top, right, bottom);

		// Draw a darker line below the rectangle
		dcMem.CreatePen(PS_SOLID, 1, RGB(160, 160, 160));
		MoveToEx(dcMem, left-1, bottom, NULL);
		LineTo(dcMem, right, bottom);
		dcMem.CreatePen(PS_SOLID, 1, RGB(248,248,248));

		// Draw a lighter line below the rectangle for the selected tab
		TabCtrl_GetItemRect(m_hWnd, TabCtrl_GetCurSel(m_hWnd), &rcItem);
		OffsetRect(&rcItem, 1, 1);
		MoveToEx(dcMem, rcItem.right, rcItem.top, NULL);
		LineTo(dcMem, rcItem.left, rcItem.top);

		// Now copy our from our memory DC to the window DC
		dcMem.DetachClipRegion();
		dcView.AttachClipRegion(hrgnClip);
		BitBlt(dcView, 0, 0, rcClient.Width(), rcClient.Height(), dcMem, 0, 0, SRCCOPY);

		// Cleanup
		::DeleteObject(hrgnSrc1);
		::DeleteObject(hrgnSrc2);
		// hrgnClip is attached to dcView, so it will be deleted automatically	
	}

	inline void CContainer::PreCreate(CREATESTRUCT& cs)
	{
		cs.style = WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | TCS_OWNERDRAWFIXED | TCS_FIXEDWIDTH | TCS_BOTTOM ;
		cs.lpszClass = WC_TABCONTROL;

	//	cs.dwExStyle = WS_EX_COMPOSITED;
	}

	inline void CContainer::RemoveContainer(CContainer* pWnd)
	{
		if (this == pWnd)
			throw CWinException(_T("CContainer::RemoveContainer... Can't remove ourself"));

		if (0 == pWnd)
			throw CWinException(_T("CContainer::RemoveContainer... Can't remove NULL container"));

		// Remove the tab
		int iTab = GetContainerIndex(pWnd);
		if (0 <= iTab)
			TabCtrl_DeleteItem(m_hWnd, iTab);
	
		// Remove the tabPageInfo entry
		std::vector<TabPageInfo>::iterator iter;	
		for (iter = m_vTabPageInfo.begin(); iter != m_vTabPageInfo.end(); ++iter)
		{
			if (iter->pwndContainer == pWnd)
			{
				m_vTabPageInfo.erase(iter);
				break;
			}
		}

		if (iTab == m_iCurrentPage)
			SelectPage(0);

		pWnd->m_pwndContainerParent = pWnd;
	}

	inline void CContainer::SelectPage(int iPage)
	{
		if (m_vTabPageInfo[iPage].pwndContainer->GetTabPage().GetHwnd() == NULL)
		{
			m_vTabPageInfo[iPage].pwndContainer->GetTabPage().Create(m_hWnd);
		}

		m_vTabPageInfo[iPage].pwndContainer->GetTabPage().SetParent(m_hWnd);

		// Determine the size of the tab page's view area
		CRect rc = GetClientRect();
		TabCtrl_AdjustRect(m_hWnd, FALSE, &rc);

		// Swap the pages over
		m_vTabPageInfo[m_iCurrentPage].pwndContainer->GetTabPage().ShowWindow(SW_HIDE);
		m_vTabPageInfo[iPage].pwndContainer->GetTabPage().SetWindowPos(HWND_TOP, rc, SWP_SHOWWINDOW);
		m_vTabPageInfo[iPage].pwndContainer->GetTabPage().GetView()->SetFocus();

		m_iCurrentPage = iPage;
	}

	inline void CContainer::SetTabSize()
	{
		CRect rc = GetClientRect();
		TabCtrl_AdjustRect(m_hWnd, FALSE, &rc);
		int nItemWidth = min(25 + GetMaxTabTextSize().cx, (rc.Width()-2)/(int)m_vTabPageInfo.size());
		SendMessage(TCM_SETITEMSIZE, 0, MAKELPARAM(nItemWidth, 20));
	}

	inline void CContainer::SetView(CWnd& Wnd)
	{
		TabPageInfo tbi = {0};
		tbi.pwndContainer = this;
		lstrcpy(tbi.szTitle, GetTabText());
		tbi.hIcon = GetTabIcon();

		m_vTabPageInfo.push_back(tbi);
		ImageList_AddIcon(m_himlTab, GetTabIcon());

		GetTabPage().SetView(Wnd);
	}

	inline LRESULT CContainer::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		static BOOL IsTracking = FALSE;

		switch (uMsg)
		{
		case UWM_IS_CONTAINER:	// A message to test if this is a Container window
			return TRUE;

		case WM_PAINT:
			{
				// Remove all pending paint requests
				PAINTSTRUCT ps;
				::BeginPaint(hWnd, &ps);
				::EndPaint(hWnd, &ps);

				// Now call our local Paint
				Paint();
			}
			return 0;
		case WM_ERASEBKGND:
			return 0;
		case WM_SIZE:
			{
				if ((int)m_vTabPageInfo.size() > m_iCurrentPage)
				{				
					// Set the tab sizes
					SetTabSize();

					// Position the View over the tab control's display area
					CRect rc = GetClientRect();
					TabCtrl_AdjustRect(m_hWnd, FALSE, &rc);						
					m_vTabPageInfo[m_iCurrentPage].pwndContainer->GetTabPage().SetWindowPos(HWND_TOP, rc, SWP_SHOWWINDOW);
				}
			}
			break;

		case WM_MOUSELEAVE:
			{
				IsTracking = FALSE;
				if (IsLeftButtonDown())
				{
					if (::SendMessage(GetParent(m_hWndParent), UWM_IS_DOCKABLE, 0, 0)) 
					{
						TRACE("Should Undock\n");
						CContainer* pContainer = GetContainer(m_iCurrentPage);

						CDockable* pDock = (CDockable*) FromHandle(GetParent(m_hWndParent));
						pDock->UndockContainer(pContainer);
					}
				}
			}
			break;

		case WM_LBUTTONDOWN:
			if (!IsTracking)	
			{
				TRACKMOUSEEVENT TrackMouseEventStruct = {0};
				TrackMouseEventStruct.cbSize = sizeof(TrackMouseEventStruct);
				TrackMouseEventStruct.dwFlags = TME_LEAVE;
				TrackMouseEventStruct.hwndTrack = m_hWnd;
				TrackMouseEvent(&TrackMouseEventStruct);
				IsTracking = TRUE;
			}
			break; 
		case WM_MOUSEMOVE:
			if (!IsTracking && !IsLeftButtonDown())	
			{
				TRACKMOUSEEVENT TrackMouseEventStruct = {0};
				TrackMouseEventStruct.cbSize = sizeof(TrackMouseEventStruct);
				TrackMouseEventStruct.dwFlags = TME_LEAVE;
				TrackMouseEventStruct.hwndTrack = m_hWnd;
				TrackMouseEvent(&TrackMouseEventStruct);
				IsTracking = TRUE;
			}
			break;
		case WM_SETFOCUS:
			{
				// Pass focus on to the current view
				GetCurrentView()->SetFocus();
			}
			break;
		}

		// pass unhandled messages on for default processing
		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

} // namespace Win32xx

#endif // DOCKABLE_H

