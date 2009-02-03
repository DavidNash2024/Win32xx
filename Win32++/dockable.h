// Win32++  Version 6.4
// Released: 4th February, 2009 by:
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
#define DS_DOCKED_LEFT			0x0001  // Dock the child left
#define DS_DOCKED_RIGHT			0x0002  // Dock the child right
#define DS_DOCKED_TOP			0x0004  // Dock the child top
#define DS_DOCKED_BOTTOM		0x0008  // Dock the child bottom
#define DS_NO_DOCKCHILD_LEFT	0x0010  // Prevent a child docking left
#define DS_NO_DOCKCHILD_RIGHT	0x0020  // Prevent a child docking right
#define DS_NO_DOCKCHILD_TOP		0x0040  // Prevent a child docking at the top
#define DS_NO_DOCKCHILD_BOTTOM	0x0080  // Prevent a child docking at the bottom
#define DS_NO_RESIZE			0x0100  // Prevent resizing
#define DS_NO_AUTO_RESIZE		0x0200	// Not Implemented yet
#define DS_NO_CAPTION			0x0400  // No caption when docked
#define DS_NO_UNDOCK			0x0800  // Prevent Undocking
#define DS_CLIENTEDGE			0x1000  // Has a 3D border when docked
#define DS_FLATLOOK				0x2000	// Not Implemented yet
#define DS_DOCKED_CONTAINER		0x4000  // Dock a container within a container 
#define DS_DOCKED_LEFTMOST      0x10000 // Leftmost outer docking
#define DS_DOCKED_RIGHTMOST     0x20000 // Rightmost outer docking
#define DS_DOCKED_TOPMOST		0x40000 // Topmost outer docking
#define DS_DOCKED_BOTTOMMOST	0x80000 // Bottommost outer docking



namespace Win32xx
{

	class CContainer;

	struct TabPageInfo
	{
		TCHAR szTitle[MAX_MENU_STRING];
		int iImage;
		CContainer* pWndContainer;
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
			CTabPage() : m_pWndView(NULL) {}
			virtual ~CTabPage() {}
			virtual CToolbar& GetToolbar() const		{return (CToolbar&)m_WndToolbar;}
			virtual CWnd* GetView() const		{return m_pWndView;}
			virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
			virtual void OnCreate();
			virtual void PreRegisterClass(WNDCLASS &wc);
			virtual void RecalcLayout();
			virtual void SetView(CWnd& wndView);
			virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		private:
			CToolbar m_WndToolbar;
			CWnd* m_pWndView;
		};

		CContainer();
		virtual ~CContainer();
		virtual void AddContainer(CContainer* pWndContainer);
		virtual void AddToolbarButton(UINT nID);
		virtual CContainer* GetContainerFromIndex(int iPage);
		virtual CContainer* GetContainerFromView(CWnd* pWndView) const;
		virtual int GetContainerIndex(CContainer* pWndContainer);
		virtual SIZE GetMaxTabTextSize();
		virtual void RemoveContainer(CContainer* pWnd);
		virtual void SelectPage(int iPage);
		virtual void SetTabSize();	
		
		// Attributes
		virtual CTabPage& GetTabPage() const	{return (CTabPage&)m_WndPage;}
		virtual CToolbar& GetToolbar() const	{return m_WndPage.GetToolbar();}
				
		CContainer* GetActiveContainer() const {return GetContainerFromView(GetActiveView());}
		CWnd* GetActiveView() const;
		std::vector<TabPageInfo>& GetAllContainers() const {return m_pWndContainerParent->m_vTabPageInfo;}
		CContainer* GetContainerParent() { return m_pWndContainerParent; }
		tString GetDockCaption() const	{ return m_tsCaption; }
		HICON GetTabIcon()				{ return m_hTabIcon; }
		LPCTSTR GetTabText() const		{ return m_tsTabText.c_str(); }
		CWnd* GetView() const			{ return GetTabPage().GetView(); }
		BOOL IsContainer() const		{ return TRUE; }
		void SetTabIcon(HICON hTabIcon) { m_hTabIcon = hTabIcon; }
		void SetTabIcon(UINT nID_Icon);
		void SetTabText(LPCTSTR szText) { m_tsTabText = szText; }
		void SetDockCaption(LPCTSTR szCaption) { m_tsCaption = szCaption; }
		void SetView(CWnd& Wnd);
				
	protected:
		virtual void OnCreate();
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void Paint();
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		CTabPage m_WndPage;
		std::vector<UINT> m_vToolbarData;
		std::vector<TabPageInfo> m_vTabPageInfo;
		tString m_tsTabText;
		tString m_tsCaption;
		int m_iCurrentPage;
		HICON m_hTabIcon;
		HIMAGELIST m_himlTab;
		CContainer* m_pWndContainerParent;
		
	};

	typedef struct DRAGPOS
	{
		NMHDR hdr;
		POINT ptPos;
		UINT DockZone;
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
			virtual void OnPaint(HDC hDC);
			virtual void PreCreate(CREATESTRUCT &cs);
			virtual void PreRegisterClass(WNDCLASS& wc);
			virtual void SendNotify(UINT nMessageID);
			virtual void SetColor(COLORREF color);
			virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

			CDockable* GetDock()			{return m_pDock;}
			int GetWidth()					{return m_DockBarWidth;}
			void SetDock(CDockable* pDock)	{m_pDock = pDock;}
			void SetWidth(int nWidth)		{m_DockBarWidth = nWidth;}

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
			virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
			virtual void PreRegisterClass(WNDCLASS& wc);
			virtual void PreCreate(CREATESTRUCT& cs);
			virtual void SendNotify(UINT nMessageID);
			virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

			tString GetCaption() const		{ return m_tsCaption; }
			CWnd* GetView() const			{ return m_pWndView; }
			BOOL IsClosing();
			void SetDock(CDockable* pDock)	{ m_pDock = pDock;}
			void SetCaption(LPCTSTR szCaption) { m_tsCaption = szCaption; }
			void SetClosePressed()			{ m_bClosePressed = TRUE; }
			void SetView(CWnd& Wnd)			{ m_pWndView = &Wnd; }

		private:
			CRect m_rcClose;
			tString m_tsCaption;
			CDockable* m_pDock;
			CWnd* m_pWndView;
			int m_NCHeight;			
			BOOL m_bClosePressed;
		};

		//  This nested class is used to indicate where a window could dock by
		//  displaying a blue tinted window.
		class CDockHint : public CWnd
		{
		public:
			CDockHint();
			virtual ~CDockHint();
			virtual RECT CalcHintRectContainer(CDockable* pDockTarget);
			virtual RECT CalcHintRectInner(CDockable* pDockTarget, CDockable* pDockDrag, UINT uDockSide);
			virtual RECT CalcHintRectOuter(CDockable* pDockDrag, UINT uDockSide);
			virtual void DisplayHint(CDockable* pDockTarget, CDockable* pDockDrag, UINT uDockSide);		
			virtual void OnPaint(HDC hDC);
			virtual void PreCreate(CREATESTRUCT &cs);
			virtual void ShowHintWindow(CDockable* pDockTarget, CRect rcHint);

			void SetBitmap(HBITMAP hbmBlueTint);

		private:
			HBITMAP m_hbmBlueTint;
		};

		class CTarget : public CWnd
		{
		public:
			CTarget() : m_hbmImage(0) {}
			virtual ~CTarget(); 
			virtual void OnPaint(HDC hDC); 
			virtual void PreCreate(CREATESTRUCT &cs);
			
			HBITMAP GetImage()		{return m_hbmImage;}
			void SetImage(UINT nID); 

		private:
			HBITMAP m_hbmImage;
		};

		class CTargetCentre : public CTarget
		{
		public:
			CTargetCentre();
			virtual ~CTargetCentre();
			virtual void OnPaint(HDC hDC);
			virtual void OnCreate();
			virtual BOOL CheckTarget(LPDRAGPOS pDragPos);
			BOOL IsOverContainer() { return m_bIsOverContainer; }

		private:
			BOOL m_bIsOverContainer;
		};

		class CTargetLeft : public CTarget
		{
		public:
			CTargetLeft() {SetImage(IDW_SDLEFT);}
			virtual BOOL CheckTarget(LPDRAGPOS pDragPos);
		};

		class CTargetTop : public CTarget
		{
		public:
			CTargetTop() {SetImage(IDW_SDTOP);}
			virtual BOOL CheckTarget(LPDRAGPOS pDragPos);
		};

		class CTargetRight : public CTarget
		{
		public:
			CTargetRight() {SetImage(IDW_SDRIGHT);}
			virtual BOOL CheckTarget(LPDRAGPOS pDragPos);
		};

		class CTargetBottom : public CTarget
		{
		public:
			CTargetBottom() {SetImage(IDW_SDBOTTOM);}
			virtual BOOL CheckTarget(LPDRAGPOS pDragPos);
		};

		friend class CTargetCentre;    
		friend class CTargetLeft;
		friend class CTargetTop;
		friend class CTargetRight;
		friend class CTargetBottom;

	public:
		// Operations
		CDockable();
		virtual ~CDockable();
		virtual CDockable* AddDockedChild(CDockable* pDockable, DWORD dwDockStyle, int DockWidth, int nDockID = 0);
		virtual CDockable* AddUndockedChild(CDockable* pDockable, DWORD dwDockStyle, int DockWidth, RECT rc, int nDockID = 0);
		virtual void CheckAllTargets(LPDRAGPOS pDragPos);
		virtual void CloseAllDockables();
		virtual void CloseAllTargets();
		virtual void DeleteDockable(CDockable* pDockable);
		virtual void Dock(CDockable* hDockable, UINT uDockSide);
		virtual void DockInContainer(CDockable* pDock, DWORD dwDockStyle);
		virtual void DockOuter(CDockable* pDockable, DWORD dwDockStyle);
		virtual void DrawHashBar(HWND hBar, POINT Pos);
		virtual CDockable* GetDockFromPoint(POINT pt) const;
		virtual CDockable* GetDockAncestor() const;
		virtual CDockable* GetDockFromID(int n_DockID) const;
		virtual CDockable* GetDockFromView(CWnd* pView) const;
		virtual int GetDockWidth() const;
		virtual void OnCreate();
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual void RecalcDockLayout();
		virtual void RecalcDockChildLayout(CRect rc);
		virtual void SendNotify(UINT nMessageID);
		virtual void Undock();
		virtual void UndockContainer(CContainer* pContainer);	

		// Attributes
		virtual CDockBar& GetDockBar() const {return (CDockBar&)m_WndDockBar;}
		virtual CDockClient& GetDockClient() const {return (CDockClient&)m_WndDockClient;}
		virtual CDockHint& GetDockHint() const {return m_pWndDockAncestor->m_WndDockHint;}	
		
		std::vector <CDockable*> & GetAllDockables() const {return GetDockAncestor()->m_vAllDockables;}
		std::vector <CDockable*> & GetDockChildren() const {return (std::vector <CDockable*> &)m_vDockChildren;}
		int GetBarWidth() const {return GetDockBar().GetWidth();}
		tString GetCaption() const {return GetDockClient().GetCaption();}
		int GetDockID() const {return m_nDockID;}
		CDockable* GetDockParent() const {return m_pWndDockParent;}
		DWORD GetDockStyle() const {return m_DockStyle;}		
		CWnd* GetView() const {return GetDockClient().GetView();}
		BOOL IsChildOfDockable(HWND hwnd) const;
		BOOL IsClosing() const {return GetDockClient().IsClosing();}
		BOOL IsDocked() const;
		BOOL IsDockable() const {return TRUE;}
		BOOL IsUndocked() const;
		void SetBarColor(COLORREF color) {GetDockBar().SetColor(color);}
		void SetBarWidth(int nWidth) {GetDockBar().SetWidth(nWidth);}
		void SetCaption(LPCTSTR szCaption);
		void SetDockStyle(DWORD dwDockStyle);
		void SetDockWidth(int DockWidth); 
		void SetView(CWnd& wndView);

	protected:
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual void PreRegisterClass(WNDCLASS &wc);
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		CDockBar		m_WndDockBar;
		CDockHint		m_WndDockHint;
		CDockClient		m_WndDockClient;
		CTargetCentre	m_WndCentreTarget;
		CTargetLeft		m_WndLeftTarget;
		CTargetTop		m_WndTopTarget;
		CTargetRight	m_WndRightTarget;
		CTargetBottom	m_WndBottomTarget;
		CDockable*		m_pWndDockParent;
		CDockable*		m_pWndDockAncestor;

		std::vector <CDockable*> m_vDockChildren;
		std::vector <CDockable*> m_vAllDockables;	// Only used in DockAncestor

		BOOL m_BlockMove;
		BOOL m_Undocking;
		int m_DockStartWidth;
		int m_nDockID;
		int m_NCHeight;
		DWORD m_dwDockZone;
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

	inline void CDockable::CDockBar::PreCreate(CREATESTRUCT &cs)
	{
		// Create a child window, initially hidden
		cs.style = WS_CHILD;
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
		SendMessage(GetParent(), WM_NOTIFY, 0, (LPARAM)&DragPos);
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
						HCURSOR hCursor;
						DWORD dwSide = GetDock()->GetDockStyle() & 0xF;
						if ((dwSide == DS_DOCKED_LEFT) || (dwSide == DS_DOCKED_RIGHT))
							hCursor = LoadCursor(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_SPLITH));
						else
							hCursor = LoadCursor(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_SPLITV));

						if (hCursor) SetCursor(hCursor);
						else TRACE(_T("Missing cursor resource for slider bar\n"));

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
	inline CDockable::CDockClient::CDockClient() : m_pWndView(0), m_NCHeight(20), m_bClosePressed(FALSE)
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
		// Determine the close button's drawing position relative to the window
		CRect rcClose = m_rcClose;
		MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcClose, 2);
	
		if (GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_CLIENTEDGE)
		{
			rcClose.OffsetRect(2, m_NCHeight+2);
			if (GetWindowRect().Height() < (m_NCHeight+4))
				rcClose.OffsetRect(-2, -2);
		}
		else
			rcClose.OffsetRect(0, m_NCHeight-2);

		// Draw the outer highlight for the close button
		if (!IsRectEmpty(&rcClose))
		{
			switch (uState)
			{
			case 0:
				{
					if (bFocus)
						DrawDC.CreatePen(PS_SOLID, 1, GetSysColor(COLOR_ACTIVECAPTION));
					else
						DrawDC.CreatePen(PS_SOLID, 1, RGB(232, 228, 220));
					
					::MoveToEx(DrawDC, rcClose.left, rcClose.bottom, NULL);
					::LineTo(DrawDC, rcClose.right, rcClose.bottom);
					::LineTo(DrawDC, rcClose.right, rcClose.top);
					::LineTo(DrawDC, rcClose.left, rcClose.top);
					::LineTo(DrawDC, rcClose.left, rcClose.bottom);
					break;
				}

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

			// Manually Draw Close Button
			if (bFocus)
				DrawDC.CreatePen(PS_SOLID, 1, RGB(230, 230, 230));
			else
				DrawDC.CreatePen(PS_SOLID, 1, RGB(64, 64, 64));

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
		return m_bClosePressed;
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
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	}

	inline void CDockable::CDockClient::PreCreate(CREATESTRUCT& cs)
	{
		DWORD dwStyle = m_pDock->GetDockStyle();
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
		SendMessage(GetParent(), WM_NOTIFY, 0, (LPARAM)&DragPos);
	}

	inline LRESULT CDockable::CDockClient::WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		static CPoint Oldpt;
		static HWND hwndButtonDown = 0;

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
				if (HTCLOSE == wParam) m_bClosePressed = TRUE;
				else	m_bClosePressed = FALSE;

				hwndButtonDown = hWnd;
				Oldpt.x = GET_X_LPARAM(lParam);
				Oldpt.y = GET_Y_LPARAM(lParam);
				if (m_pDock->IsDocked())
				{
					CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
					MapWindowPoints(NULL, hWnd, &pt, 1);				
					m_pWndView->SetFocus();				
					return 0L;
				}
				break;
			
			case WM_NCLBUTTONUP:
				hwndButtonDown = 0;
				if ((HTCLOSE == wParam) && m_bClosePressed)
				{
					// Process this message first
					DefWindowProc(uMsg, wParam, lParam);

					// Now destroy the dockable
					if (m_pDock->GetView()->IsContainer())
					{
						CContainer* pContainer = ((CContainer*)m_pDock->GetView())->GetActiveContainer();				
						CDockable* pDock = m_pDock->GetDockFromView(pContainer);					
						pDock->GetDockClient().SetClosePressed();
						m_pDock->UndockContainer(pContainer);					
						pDock->Destroy();
					}
					else
					{
						m_pDock->Undock();
						m_pDock->Destroy();
					}
					
					return 0;
				}
				m_bClosePressed = FALSE;
				break;

			case WM_NCMOUSEMOVE:
				{
					if (m_pDock->IsDocked())
					{
						// Discard phantom mouse move messages
						if ( (Oldpt.x == GET_X_LPARAM(lParam) ) && (Oldpt.y == GET_Y_LPARAM(lParam)))
							return 0L;

						if (IsLeftButtonDown() && (wParam == HTCAPTION)  && (hWnd == hwndButtonDown))
						{
							CDockable* pDock = (CDockable*)FromHandle(GetParent());
							pDock->Undock();
						}						
					}

					hwndButtonDown = 0;
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
					m_pWndView->SetFocus();					
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
				m_pWndView->SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
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

	inline RECT CDockable::CDockHint::CalcHintRectContainer(CDockable* pDockTarget)
	{ 
		// Calculate the hint window's position for container docking
		CRect rcHint = pDockTarget->GetDockClient().GetWindowRect();
		if (pDockTarget->GetDockClient().GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_CLIENTEDGE)
			rcHint.InflateRect(-2, -2);
		MapWindowPoints(NULL, pDockTarget->GetHwnd(), (LPPOINT)&rcHint, 2);

		return rcHint;
	}

	inline RECT CDockable::CDockHint::CalcHintRectInner(CDockable* pDockTarget, CDockable* pDockDrag, UINT uDockSide)
	{
		// Calculate the hint window's position for inner docking
		CRect rcHint = pDockTarget->GetDockClient().GetWindowRect();
		if (pDockTarget->GetDockClient().GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_CLIENTEDGE)
			rcHint.InflateRect(-2, -2);
		MapWindowPoints(NULL, pDockTarget->GetHwnd(), (LPPOINT)&rcHint, 2);

		int Width;
		CRect rcDockDrag = pDockDrag->GetWindowRect();			
		CRect rcDockTarget = pDockTarget->GetDockClient().GetWindowRect();
		if ((uDockSide  == DS_DOCKED_LEFT) || (uDockSide  == DS_DOCKED_RIGHT))
		{
			Width = rcDockDrag.Width();
			if (Width >= (rcDockTarget.Width() - pDockDrag->GetBarWidth()))
				Width = max(rcDockTarget.Width()/2 - pDockDrag->GetBarWidth(), pDockDrag->GetBarWidth());
		}
		else
		{
			Width = rcDockDrag.Height();
			if (Width >= (rcDockTarget.Height() - pDockDrag->GetBarWidth()))
				Width = max(rcDockTarget.Height()/2 - pDockDrag->GetBarWidth(), pDockDrag->GetBarWidth());
		}
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

		return rcHint;
	}

	inline RECT CDockable::CDockHint::CalcHintRectOuter(CDockable* pDockDrag, UINT uDockSide)
	{
		// Calculate the hint window's position for outer docking
		CDockable* pDockTarget = pDockDrag->GetDockAncestor();
		CRect rcHint = pDockTarget->GetClientRect();
		if (pDockTarget->GetDockClient().GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_CLIENTEDGE)
			rcHint.InflateRect(-2, -2);

		int Width;
		CRect rcDockDrag = pDockDrag->GetWindowRect();			
		CRect rcDockTarget = pDockTarget->GetDockClient().GetWindowRect();

		// Limit the docked size to half the parent's size if it won't fit inside parent
		if ((uDockSide == DS_DOCKED_LEFTMOST) || (uDockSide  == DS_DOCKED_RIGHTMOST))
		{
			Width = rcDockDrag.Width();
			int BarWidth = pDockDrag->GetBarWidth();
			if (Width >= pDockTarget->GetDockClient().GetClientRect().Width() - pDockDrag->GetBarWidth())
				Width = max(pDockTarget->GetDockClient().GetClientRect().Width()/2 - BarWidth, BarWidth);
		}
		else
		{
			Width = rcDockDrag.Height();
			int BarWidth = pDockDrag->GetBarWidth();
			if (Width >= pDockTarget->GetDockClient().GetClientRect().Height() - pDockDrag->GetBarWidth())
				Width = max(pDockTarget->GetDockClient().GetClientRect().Height()/2 - BarWidth, BarWidth);
		} 
		switch (uDockSide)
		{
		case DS_DOCKED_LEFTMOST:
			rcHint.right = rcHint.left + Width;
			break;
		case DS_DOCKED_RIGHTMOST:
			rcHint.left = rcHint.right - Width;
			break;
		case DS_DOCKED_TOPMOST:
			rcHint.bottom = rcHint.top + Width;
			break;
		case DS_DOCKED_BOTTOMMOST:
			rcHint.top = rcHint.bottom - Width;
			break;
		}

		return rcHint;
	}

	inline void CDockable::CDockHint::DisplayHint(CDockable* pDockTarget, CDockable* pDockDrag, UINT uDockSide)
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

		if (!IsWindow())
		{
			CRect rcHint;

			if (uDockSide & 0xF)
				rcHint = CalcHintRectInner(pDockTarget, pDockDrag, uDockSide);
			else if (uDockSide & 0xF0000)
				rcHint = CalcHintRectOuter(pDockDrag, uDockSide);
			else if (uDockSide & DS_DOCKED_CONTAINER)
				rcHint = CalcHintRectContainer(pDockTarget);
			else
				return;
			
			ShowHintWindow(pDockTarget, rcHint);
		
			// Put the dragged window back on top (for Vista AERO)
			pDockDrag->SetWindowPos(HWND_TOP, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
			pDockDrag->RedrawWindow(0, 0, RDW_INVALIDATE|RDW_FRAME|RDW_UPDATENOW);
		}
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

	inline void CDockable::CDockHint::ShowHintWindow(CDockable* pDockTarget, CRect rcHint)
	{
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
		{
			Create(pDockTarget->GetHwnd());
		}

		MapWindowPoints(pDockTarget->GetHwnd(), NULL, (LPPOINT)&rcHint, 2);
		SetWindowPos(NULL, rcHint, SWP_SHOWWINDOW|SWP_NOZORDER|SWP_NOACTIVATE);
	}


	////////////////////////////////////////////////////////////////
	// Definitions for the CTargetCentre class nested within CDockable
	//
	inline CDockable::CTargetCentre::CTargetCentre() : m_bIsOverContainer(FALSE)
	{
	}

	inline CDockable::CTargetCentre::~CTargetCentre()
	{
	}

	inline void CDockable::CTargetCentre::OnPaint(HDC hDC)
	{
		HBITMAP hbmCentre = (HBITMAP)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_SDCENTER),
						                  IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		HBITMAP hbmLeft= (HBITMAP)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_SDLEFT),
						                  IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		HBITMAP hbmRight= (HBITMAP)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_SDRIGHT),
						                  IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		HBITMAP hbmTop= (HBITMAP)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_SDTOP),
						                  IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		HBITMAP hbmBottom= (HBITMAP)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_SDBOTTOM),
						                  IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

		if (hbmCentre)	DrawBitmap(hDC, 0, 0, 88, 88, hbmCentre, RGB(255,0,255));
		else TRACE(_T("Missing docking resource: Target Centre\n"));

		if (hbmLeft) DrawBitmap(hDC, 0, 29, 31, 29, hbmLeft, RGB(255,0,255));
		else TRACE(_T("Missing docking resource: Target Left\n"));
		
		if (hbmTop) DrawBitmap(hDC, 29, 0, 29, 31, hbmTop, RGB(255,0,255));
		else TRACE(_T("Missing docking resource: Target Top\n"));

		if (hbmRight) DrawBitmap(hDC, 55, 29, 31, 29, hbmRight, RGB(255,0,255));
		else TRACE(_T("Missing docking resource: Target Right\n"));
		
		if (hbmBottom) DrawBitmap(hDC, 29, 55, 29, 31, hbmBottom, RGB(255,0,255));
		else TRACE(_T("Missing docking resource: Target Bottom\n"));

		::DeleteObject(hbmCentre);
		::DeleteObject(hbmLeft);
		::DeleteObject(hbmRight);
		::DeleteObject(hbmTop);
		::DeleteObject(hbmBottom);

		if (IsOverContainer())
		{
			HBITMAP hbmMiddle = (HBITMAP)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_SDMIDDLE),
						                  IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
			DrawBitmap(hDC, 31, 31, 25, 26, hbmMiddle, RGB(255,0,255));
			::DeleteObject(hbmMiddle);
		}
	}

	inline void CDockable::CTargetCentre::OnCreate()
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

	inline BOOL CDockable::CTargetCentre::CheckTarget(LPDRAGPOS pDragPos)
	{
		CDockable* pDockDrag = (CDockable*)FromHandle(pDragPos->hdr.hwndFrom);
		CDockable* pDockTarget = pDockDrag->GetDockFromPoint(pDragPos->ptPos);
		static CDockable* pOldDockTarget = 0;
		
		if (!IsWindow())	Create();
		m_bIsOverContainer = pDockTarget->GetView()->IsContainer();
		
		// Redraw the target if the dock target changes
		if (pOldDockTarget != pDockTarget)	Invalidate();
		pOldDockTarget = pDockTarget;
		
		int cxImage = 88;
		int cyImage = 88;
		
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
		CRect rcMiddle(31, 31, 56, 57);

		// Test if our cursor is in one of the docking zones
		if ((PtInRect(&rcLeft, pt)) && !(pDockTarget->GetDockStyle() & DS_NO_DOCKCHILD_LEFT))
		{
			pDockDrag->m_BlockMove = TRUE;
			pDockTarget->GetDockHint().DisplayHint(pDockTarget, pDockDrag, DS_DOCKED_LEFT);
			pDockDrag->m_dwDockZone = DS_DOCKED_LEFT;
			return TRUE;
		}
		else if ((PtInRect(&rcTop, pt)) && !(pDockTarget->GetDockStyle() & DS_NO_DOCKCHILD_TOP))
		{
			pDockDrag->m_BlockMove = TRUE;
			pDockTarget->GetDockHint().DisplayHint(pDockTarget, pDockDrag, DS_DOCKED_TOP);
			pDockDrag->m_dwDockZone = DS_DOCKED_TOP;
			return TRUE;
		}
		else if ((PtInRect(&rcRight, pt)) && !(pDockTarget->GetDockStyle() & DS_NO_DOCKCHILD_RIGHT))
		{
			pDockDrag->m_BlockMove = TRUE;
			pDockTarget->GetDockHint().DisplayHint(pDockTarget, pDockDrag, DS_DOCKED_RIGHT);
			pDockDrag->m_dwDockZone = DS_DOCKED_RIGHT;
			return TRUE;
		}
		else if ((PtInRect(&rcBottom, pt)) && !(pDockTarget->GetDockStyle() & DS_NO_DOCKCHILD_BOTTOM))
		{
			pDockDrag->m_BlockMove = TRUE;
			pDockTarget->GetDockHint().DisplayHint(pDockTarget, pDockDrag, DS_DOCKED_BOTTOM);
			pDockDrag->m_dwDockZone = DS_DOCKED_BOTTOM;
			return TRUE;
		}
		else if ((PtInRect(&rcMiddle, pt)) && (IsOverContainer()))
		{
			pDockDrag->m_BlockMove = TRUE;
			pDockTarget->GetDockHint().DisplayHint(pDockTarget, pDockDrag, DS_DOCKED_CONTAINER);
			pDockDrag->m_dwDockZone = DS_DOCKED_CONTAINER;
			return TRUE;
		}
		else
			return FALSE;
	}

	////////////////////////////////////////////////////////////////
	// Definitions for the CTarget class nested within CDockable
	// CTarget is the base class for a number of CTargetXXX classes
	inline CDockable::CTarget::~CTarget()
	{
		if (m_hbmImage) ::DeleteObject(m_hbmImage);
	}

	inline void CDockable::CTarget::OnPaint(HDC hDC)
	{
		BITMAP bm;
		GetObject(GetImage(), sizeof(bm), &bm);
		int cxImage = bm.bmWidth;
		int cyImage = bm.bmHeight;
				
		if (GetImage()) DrawBitmap(hDC, 0, 0, cxImage, cyImage, GetImage(), RGB(255,0,255));
		else TRACE(_T("Missing docking resource\n"));
	}

	inline void CDockable::CTarget::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_POPUP;
		cs.dwExStyle = WS_EX_TOPMOST|WS_EX_TOOLWINDOW;
		cs.lpszClass = _T("Win32++ DockTargeting");
	}

	inline void CDockable::CTarget::SetImage(UINT nID)
	{
		m_hbmImage = (HBITMAP)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(nID),
		IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
	}

	////////////////////////////////////////////////////////////////
	// Definitions for the CTargetLeft class nested within CDockable
	//
	inline BOOL CDockable::CTargetLeft::CheckTarget(LPDRAGPOS pDragPos)
	{
		CDockable* pDockDrag = (CDockable*)FromHandle(pDragPos->hdr.hwndFrom);
		CDockable* pDockTarget = pDockDrag->GetDockAncestor();
		BITMAP bm;
		GetObject(GetImage(), sizeof(bm), &bm);
		int cxImage = bm.bmWidth;
		int cyImage = bm.bmHeight;	

		if (!IsWindow())
		{
			Create();
			CRect rc = pDockTarget->GetWindowRect();
			int yMid = rc.top + (rc.Height() - cyImage)/2;
			SetWindowPos(HWND_TOPMOST, rc.left + 10, yMid, cxImage, cyImage, SWP_NOACTIVATE|SWP_SHOWWINDOW);
		} 

		CRect rcLeft(0, 0, cxImage, cyImage);
		CPoint pt = pDragPos->ptPos;
		MapWindowPoints(NULL, m_hWnd, &pt, 1);

		// Test if our cursor is in one of the docking zones
		if ((PtInRect(&rcLeft, pt)) && !(pDockTarget->GetDockStyle() & DS_NO_DOCKCHILD_LEFT))
		{
			pDockDrag->m_BlockMove = TRUE;
			pDockTarget->GetDockHint().DisplayHint(pDockTarget, pDockDrag, DS_DOCKED_LEFTMOST);
			pDockDrag->m_dwDockZone = DS_DOCKED_LEFTMOST;
			return TRUE;
		}

		return FALSE;
	}


	////////////////////////////////////////////////////////////////
	// Definitions for the CTargetTop class nested within CDockable
	//
	inline BOOL CDockable::CTargetTop::CheckTarget(LPDRAGPOS pDragPos)
	{
		CDockable* pDockDrag = (CDockable*)FromHandle(pDragPos->hdr.hwndFrom);
		CDockable* pDockTarget = pDockDrag->GetDockAncestor();
		BITMAP bm;
		GetObject(GetImage(), sizeof(bm), &bm);
		int cxImage = bm.bmWidth;
		int cyImage = bm.bmHeight;	

		if (!IsWindow())
		{
			Create();
			CRect rc = pDockTarget->GetWindowRect();
			int xMid = rc.left + (rc.Width() - cxImage)/2;
			SetWindowPos(HWND_TOPMOST, xMid, rc.top + 10, cxImage, cyImage, SWP_NOACTIVATE|SWP_SHOWWINDOW);
		}
		

		CRect rcTop(0, 0, cxImage, cyImage);
		CPoint pt = pDragPos->ptPos;
		MapWindowPoints(NULL, m_hWnd, &pt, 1);

		// Test if our cursor is in one of the docking zones
		if ((PtInRect(&rcTop, pt)) && !(pDockTarget->GetDockStyle() & DS_NO_DOCKCHILD_TOP))
		{
			pDockDrag->m_BlockMove = TRUE;
			pDockTarget->GetDockHint().DisplayHint(pDockTarget, pDockDrag, DS_DOCKED_TOPMOST);
			pDockDrag->m_dwDockZone = DS_DOCKED_TOPMOST;
			return TRUE;
		}

		return FALSE;
	}


	////////////////////////////////////////////////////////////////
	// Definitions for the CTargetRight class nested within CDockable
	//
	inline BOOL CDockable::CTargetRight::CheckTarget(LPDRAGPOS pDragPos)
	{
		CDockable* pDockDrag = (CDockable*)FromHandle(pDragPos->hdr.hwndFrom);
		CDockable* pDockTarget = pDockDrag->GetDockAncestor();
		BITMAP bm;
		GetObject(GetImage(), sizeof(bm), &bm);
		int cxImage = bm.bmWidth;
		int cyImage = bm.bmHeight;	

		if (!IsWindow())
		{
			Create();
			CRect rc = pDockTarget->GetWindowRect();
			int yMid = rc.top + (rc.Height() - cyImage)/2;
			SetWindowPos(HWND_TOPMOST, rc.right - 10 - cxImage, yMid, cxImage, cyImage, SWP_NOACTIVATE|SWP_SHOWWINDOW);
		}
		
		CRect rcRight(0, 0, cxImage, cyImage);
		CPoint pt = pDragPos->ptPos;
		MapWindowPoints(NULL, m_hWnd, &pt, 1);

		// Test if our cursor is in one of the docking zones
		if ((PtInRect(&rcRight, pt)) && !(pDockTarget->GetDockStyle() & DS_NO_DOCKCHILD_RIGHT))
		{
			pDockDrag->m_BlockMove = TRUE;
			pDockTarget->GetDockHint().DisplayHint(pDockTarget, pDockDrag, DS_DOCKED_RIGHTMOST);
			pDockDrag->m_dwDockZone = DS_DOCKED_RIGHTMOST;
			return TRUE;
		}

		return FALSE;
	}


	////////////////////////////////////////////////////////////////
	// Definitions for the CTargetBottom class nested within CDockable
	//
	inline BOOL CDockable::CTargetBottom::CheckTarget(LPDRAGPOS pDragPos)
	{
		CDockable* pDockDrag = (CDockable*)FromHandle(pDragPos->hdr.hwndFrom);
		CDockable* pDockTarget = pDockDrag->GetDockAncestor();
		BITMAP bm;
		GetObject(GetImage(), sizeof(bm), &bm);
		int cxImage = bm.bmWidth;
		int cyImage = bm.bmHeight;	

		if (!IsWindow())
		{
			Create();
			CRect rc = pDockTarget->GetWindowRect();
			int xMid = rc.left + (rc.Width() - cxImage)/2;
			SetWindowPos(HWND_TOPMOST, xMid, rc.bottom - 10 - cyImage, cxImage, cyImage, SWP_NOACTIVATE|SWP_SHOWWINDOW);
		}
		CRect rcBottom(0, 0, cxImage, cyImage);
		CPoint pt = pDragPos->ptPos;
		MapWindowPoints(NULL, m_hWnd, &pt, 1);

		// Test if our cursor is in one of the docking zones
		if ((PtInRect(&rcBottom, pt)) && !(pDockTarget->GetDockStyle() & DS_NO_DOCKCHILD_BOTTOM))
		{
			pDockDrag->m_BlockMove = TRUE;
			pDockTarget->GetDockHint().DisplayHint(pDockTarget, pDockDrag, DS_DOCKED_BOTTOMMOST);
			pDockDrag->m_dwDockZone = DS_DOCKED_BOTTOMMOST;
			return TRUE;
		}

		return FALSE;
	}


	/////////////////////////////////////////
	// Definitions for the CDockable class
	//
	inline CDockable::CDockable() : m_pWndDockParent(NULL), m_BlockMove(FALSE), m_Undocking(FALSE), 
		            m_DockStartWidth(0), m_nDockID(0), m_NCHeight(20), m_dwDockZone(0),
					m_DockWidthRatio(1.0), m_DockStyle(0)
	{
		WORD HashPattern[] = {0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA};
		m_hbmHash = ::CreateBitmap (8, 8, 1, 1, HashPattern);
		m_hbrDithered = ::CreatePatternBrush (m_hbmHash);
		
		// Assume this dockable is the DockAncestor for now.
		m_pWndDockAncestor = this;
	}

	inline CDockable::~CDockable()
	{
		::DeleteObject(m_hbrDithered);
		::DeleteObject(m_hbmHash);
		
		std::vector <CDockable*>::iterator iter;
		if (GetDockAncestor() == this)
		{
			// Ensure all child dockables are destroyed
			iter = GetAllDockables().begin();
			while (iter != GetAllDockables().end())
			{
				delete(*iter);
				iter = GetAllDockables().begin();
			} 
		}
		else
		{
			for (iter = GetAllDockables().begin(); iter != GetAllDockables().end(); ++iter)
			{
				// Remove this child entry from the DockAncestor's m_vAllDockables vector 
				if ((*iter) == this)
				{
					GetAllDockables().erase(iter);
					break;
				}
			} 
		}
	}

	inline CDockable* CDockable::AddDockedChild(CDockable* pDockable, DWORD dwDockStyle, int DockWidth, int nDockID /* = 0*/)
	// This function creates the dockable, and adds it to the dockable heirachy as docked
	{
		// Create the dockable window as a child of the frame window. 
		// This pernamently sets the frame window as the dockable window's owner,
		// even when its parent is subsequently changed.
		pDockable->SetDockWidth(DockWidth);
		pDockable->SetDockStyle(dwDockStyle);
		pDockable->m_nDockID = nDockID;
		pDockable->m_pWndDockAncestor = GetDockAncestor();
		HWND hwndFrame = GetAncestor();
		pDockable->Create(hwndFrame);
		pDockable->SetParent(m_hWnd);
		
		// Dock the dockable window
		if (dwDockStyle & DS_DOCKED_CONTAINER)
			DockInContainer(pDockable, dwDockStyle);
		else
			Dock(pDockable, dwDockStyle);

		// Store the Dockable's pointer in the DockAncestor's vector for later deletion
		GetDockAncestor()->m_vAllDockables.push_back(pDockable);

		// Issue TRACE warnings for any missing resources
		HMODULE hMod= GetApp()->GetInstanceHandle();
		if (!(dwDockStyle & DS_NO_RESIZE))
		{
			if (!FindResource(hMod, MAKEINTRESOURCE(IDW_SPLITH), RT_GROUP_CURSOR))
				TRACE(_T("\n**WARNING** Horizontal cursor resource missing\n"));
			if (!FindResource(hMod, MAKEINTRESOURCE(IDW_SPLITV), RT_GROUP_CURSOR))
				TRACE(_T("\n**WARNING** Vertical cursor resource missing\n"));
		}
		if (!(dwDockStyle & DS_NO_UNDOCK))
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
		if (dwDockStyle & DS_DOCKED_CONTAINER)
		{
			if (!FindResource(hMod, MAKEINTRESOURCE(IDW_SDMIDDLE), RT_BITMAP))
				TRACE(_T("\n**WARNING** Docking container bitmap resource missing\n"));
		}

		return pDockable;
	}

	inline CDockable* CDockable::AddUndockedChild(CDockable* pDockable, DWORD dwDockStyle, int DockWidth, RECT rc, int nDockID /* = 0*/)
	// This function creates the dockable, and adds it to the dockable heirachy as undocked
	{
		pDockable->SetDockWidth(DockWidth);
		pDockable->SetDockStyle(dwDockStyle & 0XFFFFFF0);
		pDockable->m_nDockID = nDockID;
		pDockable->m_pWndDockAncestor = GetDockAncestor();

		// Initially create the as a child window of the frame
		// This makes the frame window the owner of our dockable
		HWND hwndFrame = GetAncestor();
		pDockable->Create(hwndFrame);
		pDockable->SetParent(m_hWnd);

		// Change the Dockable to a POPUP window
		DWORD dwStyle = WS_POPUP| WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_VISIBLE;
		pDockable->SetWindowLongPtr(GWL_STYLE, dwStyle);
		pDockable->SetRedraw(FALSE);
		pDockable->SetParent(0);
		pDockable->SetWindowPos(HWND_TOP, rc, SWP_SHOWWINDOW|SWP_FRAMECHANGED);
		pDockable->SetRedraw(TRUE);
		pDockable->RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
		pDockable->SetWindowText(pDockable->GetCaption().c_str());

		// Store the Dockable's pointer in the DockAncestor's vector for later deletion
		GetDockAncestor()->m_vAllDockables.push_back(pDockable);
		return pDockable;
	}

	inline void CDockable::CheckAllTargets(LPDRAGPOS pDragPos)
	{
		if (!GetDockAncestor()->m_WndCentreTarget.CheckTarget(pDragPos))
		{
			if (!GetDockAncestor()->m_WndLeftTarget.CheckTarget(pDragPos))
			{
				if(!GetDockAncestor()->m_WndTopTarget.CheckTarget(pDragPos))
				{
					if(!GetDockAncestor()->m_WndRightTarget.CheckTarget(pDragPos))
					{
						if(!GetDockAncestor()->m_WndBottomTarget.CheckTarget(pDragPos))
						{
							// Not in a docking zone, so clean up
							NMHDR nmhdr = pDragPos->hdr;
							CDockable* pDockDrag = (CDockable*)FromHandle(nmhdr.hwndFrom);

							if (pDockDrag->m_BlockMove)
								pDockDrag->RedrawWindow(0, 0, RDW_FRAME|RDW_INVALIDATE);

							GetDockHint().Destroy();
							pDockDrag->m_dwDockZone = 0;
							pDockDrag->m_BlockMove = FALSE;
						}
					}
				}
			}
		}		
	}

	inline void CDockable::CloseAllDockables()
	{
		if (this != GetDockAncestor())
			throw CWinException(_T("Must call CloseAllDockables from the DockAncestor"));

		std::vector <CDockable*>::iterator v;

		SetRedraw(FALSE);
		for (v = m_vAllDockables.begin(); v != m_vAllDockables.end(); ++v)
		{
			// Destroy the window
			if (*v)  (*v)->Destroy();
		}

		m_vDockChildren.clear();

		// Process any queued messages now. The UWM_DOCK_DESTROYED messages will be
		//  processed at this point, deleting the dockables.
		MSG msg;
		while (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		
		SetRedraw(TRUE);
		RecalcDockLayout();
	}

	inline void CDockable::CloseAllTargets()
	{
		GetDockAncestor()->m_WndCentreTarget.Destroy();
		GetDockAncestor()->m_WndLeftTarget.Destroy();
		GetDockAncestor()->m_WndTopTarget.Destroy();
		GetDockAncestor()->m_WndRightTarget.Destroy();
		GetDockAncestor()->m_WndBottomTarget.Destroy();
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
		// Set the dock styles
		GetDockAncestor()->SetRedraw(FALSE);
		DWORD dwStyle = WS_CHILD | WS_VISIBLE;
		pDockable->m_BlockMove = FALSE;
		pDockable->SetWindowLongPtr(GWL_STYLE, dwStyle);
		pDockable->SetDockStyle(DockStyle);

		// Set the docking relationships
		m_vDockChildren.push_back(pDockable);		
		pDockable->SetParent(m_hWnd);
		pDockable->m_pWndDockParent = this;
		pDockable->GetDockBar().SetParent(m_hWnd);

		// Limit the docked size to half the parent's size if it won't fit inside parent
		if (((DockStyle & 0xF)  == DS_DOCKED_LEFT) || ((DockStyle &0xF)  == DS_DOCKED_RIGHT))
		{
			int Width = GetDockClient().GetWindowRect().Width();
			int BarWidth = pDockable->GetBarWidth();
			if (pDockable->m_DockStartWidth >= (Width - BarWidth))
				pDockable->SetDockWidth(max(Width/2 - BarWidth, BarWidth));  

			pDockable->m_DockWidthRatio = ((double)pDockable->m_DockStartWidth) / (double)GetWindowRect().Width();
		}
		else
		{
			int Height = GetDockClient().GetWindowRect().Height();
			int BarWidth = pDockable->GetBarWidth();
			if (pDockable->m_DockStartWidth >= (Height - BarWidth))
				pDockable->SetDockWidth(max(Height/2 - BarWidth, BarWidth));  

			pDockable->m_DockWidthRatio = ((double)pDockable->m_DockStartWidth) / (double)GetWindowRect().Height();
		} 

		// Redraw the docked windows
		pDockable->GetView()->SetFocus();
		GetDockAncestor()->SetRedraw(TRUE);
		RecalcDockLayout();
	}

	inline void CDockable::DockInContainer(CDockable* pDock, DWORD dwDockStyle)
	{
		if ((dwDockStyle & DS_DOCKED_CONTAINER) && (pDock->GetView()->IsContainer()))
		{
			// Add a container to an existing docked container
			pDock->m_BlockMove = FALSE;
			pDock->ShowWindow(SW_HIDE);
			pDock->SetWindowLongPtr(GWL_STYLE, WS_CHILD);
			pDock->SetParent(m_hWnd);
			CContainer* pContainer = (CContainer*)GetView();	
			CContainer* pContainerSource = (CContainer*)pDock->GetView();
			
			if (pContainerSource->GetAllContainers().size() > 1)
			{
				// The container we're about to add has children, so transfer those first
				std::vector<TabPageInfo>::reverse_iterator riter;
				for ( riter = pContainerSource->GetAllContainers().rbegin() ; riter < pContainerSource->GetAllContainers().rend() -1; ++riter )
				{
					// Remove child container from pContainerSource
					CContainer* pContainerChild = (*riter).pWndContainer;
					pContainerChild->ShowWindow(SW_HIDE);
					pContainerSource->RemoveContainer(pContainerChild);
					
					// Add child container to this container
					pContainer->AddContainer(pContainerChild);

					CDockable* pDockChild = GetDockFromView(pContainerChild);
					pDockChild->SetParent(m_hWnd);
				}
			}

			pContainer->AddContainer((CContainer*)pDock->GetView());
			pDock->SetDockStyle(dwDockStyle);
			pDock->SetParent(m_hWnd);
		}
	}

	inline void CDockable::DockOuter(CDockable* pDockable, DWORD dwDockStyle)
	{
		DWORD OuterDocking = dwDockStyle & 0xF0000;
		DWORD DockSide = OuterDocking / 0x10000; 
		dwDockStyle &= 0xFFF0FFFF;
		dwDockStyle |= DockSide;	

		// Set the dock styles
		GetDockAncestor()->SetRedraw(FALSE);
		DWORD dwStyle = WS_CHILD | WS_VISIBLE;
		pDockable->m_BlockMove = FALSE;
		pDockable->SetWindowLongPtr(GWL_STYLE, dwStyle);
		pDockable->SetDockStyle(dwDockStyle);

		// Set the docking relationships
		std::vector<CDockable*>::iterator iter = GetDockAncestor()->m_vDockChildren.begin();
		GetDockAncestor()->m_vDockChildren.insert(iter, pDockable);
		pDockable->SetParent(GetDockAncestor()->GetHwnd());
		pDockable->m_pWndDockParent = GetDockAncestor();
		pDockable->GetDockBar().SetParent(GetDockAncestor()->GetHwnd());

		// Limit the docked size to half the parent's size if it won't fit inside parent
		if (((dwDockStyle & 0xF)  == DS_DOCKED_LEFT) || ((dwDockStyle &0xF)  == DS_DOCKED_RIGHT))
		{
			int Width = GetDockAncestor()->GetDockClient().GetWindowRect().Width();
			int BarWidth = pDockable->GetBarWidth();
			if (pDockable->m_DockStartWidth >= (Width - BarWidth))
				pDockable->SetDockWidth(max(Width/2 - BarWidth, BarWidth));  

			pDockable->m_DockWidthRatio = ((double)pDockable->m_DockStartWidth) / (double)GetDockAncestor()->GetWindowRect().Width();
		}
		else
		{
			int Height = GetDockAncestor()->GetDockClient().GetWindowRect().Height();
			int BarWidth = pDockable->GetBarWidth();
			if (pDockable->m_DockStartWidth >= (Height - BarWidth))
				pDockable->SetDockWidth(max(Height/2 - BarWidth, BarWidth));  

			pDockable->m_DockWidthRatio = ((double)pDockable->m_DockStartWidth) / (double)GetDockAncestor()->GetWindowRect().Height();
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

	inline CDockable* CDockable::GetDockFromPoint(POINT pt) const
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

	inline CDockable* CDockable::GetDockAncestor() const
	// The GetDockAncestor function retrieves the pointer to the
	//  ancestor (root dockable parent) of the Dockable.
	{
		return m_pWndDockAncestor;
	}

	inline CDockable* CDockable::GetDockFromID(int n_DockID) const
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

	inline CDockable* CDockable::GetDockFromView(CWnd* pView) const
	{
		CDockable* pDock = 0;
		std::vector<CDockable*>::iterator iter;
		std::vector<CDockable*> AllDockables = GetAllDockables();
		for (iter = AllDockables.begin(); iter != AllDockables.end(); ++iter)
		{
			if ((*iter)->GetView() == pView)
				pDock = (*iter);
		}

		return pDock;
	}

	inline int CDockable::GetDockWidth() const
	{
		CRect rcParent;
		if (GetDockParent())
			rcParent = GetDockParent()->GetWindowRect();
		else
			rcParent = GetDockAncestor()->GetWindowRect();

		double DockWidth;
		if ((GetDockStyle() & DS_DOCKED_LEFT) || (GetDockStyle() & DS_DOCKED_RIGHT))
			DockWidth = (double)(rcParent.Width()*m_DockWidthRatio);
		else
			DockWidth = (double)(rcParent.Height()*m_DockWidthRatio);
			
		return (int)DockWidth;
	}
	
	inline BOOL CDockable::IsDocked() const
	{
		return (((m_DockStyle&0xF) || (m_DockStyle & DS_DOCKED_CONTAINER)) && !m_Undocking); // Boolean expression
	}

	inline BOOL CDockable::IsChildOfDockable(HWND hwnd) const
	// returns true if the specified window is a child of this dockable
	{
		while (hwnd != NULL)
		{
			if (hwnd == m_hWnd) 
				return TRUE;
			
			if (TRUE == ::SendMessage(hwnd, UWM_IS_DOCKABLE, 0, 0))
				break;

			hwnd = ::GetParent(hwnd);
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
		GetDockClient().SetDock(this);
		GetDockClient().Create(m_hWnd);	
		if (NULL == GetView())
			throw CWinException(_T("CDockable::OnCreate... View window is not assigned!\nUse SetView to set the View Window"));
		GetView()->Create(GetDockClient().GetHwnd());

		// Create the slider bar belonging to this dockable
		GetDockBar().SetDock(this);
		if (GetDockAncestor() != this)
			GetDockBar().Create(GetParent());

		// Now remove the WS_POPUP style. It was required to allow this window
		// to be owned by the frame window.
		HWND hwndParent = GetParent();
		SetWindowLongPtr(GWL_STYLE, WS_CHILD);
		SetParent(hwndParent);	// Reinstate the window's parent
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
				CheckAllTargets((LPDRAGPOS)lParam);
			}
			break;

		case UWM_DOCK_END:
			{
				CDockable* pDock = (CDockable*)FromHandle(pdp->hdr.hwndFrom);
				UINT DockZone = pdp->DockZone;
				CRect rc = pDock->GetWindowRect();

				switch(DockZone)
				{
				case DS_DOCKED_LEFT:				
				case DS_DOCKED_RIGHT:
					pDock->SetDockWidth(rc.Width());
					Dock(pDock, pDock->GetDockStyle() | DockZone);
					break;
				case DS_DOCKED_TOP:
				case DS_DOCKED_BOTTOM:
					pDock->SetDockWidth(rc.Height());
					Dock(pDock, pDock->GetDockStyle() | DockZone);
					break;
				case DS_DOCKED_CONTAINER:
					{
						DockInContainer(pDock, pDock->GetDockStyle() | DockZone);
						CContainer* pContainer = (CContainer*)GetView();
						int iPage = pContainer->GetContainerIndex((CContainer*)pDock->GetView());
						pContainer->SelectPage(iPage);
					}
					break;
				case DS_DOCKED_LEFTMOST:
				case DS_DOCKED_RIGHTMOST:
					pDock->SetDockWidth(rc.Width());
					DockOuter(pDock, pDock->GetDockStyle() | DockZone);
					break;
				case DS_DOCKED_TOPMOST:
				case DS_DOCKED_BOTTOMMOST:
					pDock->SetDockWidth(rc.Height());
					DockOuter(pDock, pDock->GetDockStyle() | DockZone);
					break;
				}
				
				GetDockHint().Destroy();
				CloseAllTargets();
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
					pDock->m_DockWidthRatio = ((double)pDock->m_DockStartWidth + dBarWidth )/((double)pDock->m_pWndDockParent->GetWindowRect().Width() - dBarWidth);
					break;
				case DS_DOCKED_RIGHT:
					DockWidth = rcDock.right - max(pt.x, iBarWidth/2) - (int)(.5 + 1.5*dBarWidth);
					DockWidth = max(-iBarWidth, DockWidth);
					pDock->SetDockWidth(DockWidth);
					pDock->m_DockWidthRatio = ((double)pDock->m_DockStartWidth + dBarWidth)/((double)pDock->m_pWndDockParent->GetWindowRect().Width() - dBarWidth);
					break;
				case DS_DOCKED_TOP:
					DockWidth = max(pt.y, iBarWidth/2) - rcDock.top - (int)(.5 + 1.5*dBarWidth);
					DockWidth = max(-iBarWidth, DockWidth);
					pDock->SetDockWidth(DockWidth);
					pDock->m_DockWidthRatio = ((double)pDock->m_DockStartWidth + dBarWidth)/((double)pDock->m_pWndDockParent->GetWindowRect().Height() - dBarWidth);
					break;
				case DS_DOCKED_BOTTOM:
					DockWidth = rcDock.bottom - max(pt.y, iBarWidth/2) - (int)(.5 + 1.5*dBarWidth);
					DockWidth = max(-iBarWidth, DockWidth);
					pDock->SetDockWidth(DockWidth);
					pDock->m_DockWidthRatio = ((double)pDock->m_DockStartWidth + dBarWidth)/((double)pDock->m_pWndDockParent->GetWindowRect().Height() - dBarWidth);
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
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
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
		// Send a docking notification to the dockable below the cursor
		DRAGPOS DragPos;
		DragPos.hdr.code = nMessageID;
		DragPos.hdr.hwndFrom = m_hWnd;
		::GetCursorPos(&DragPos.ptPos);
		DragPos.DockZone = m_dwDockZone;
		m_dwDockZone = 0;

		CDockable* pDock = GetDockFromPoint(DragPos.ptPos);

		if (pDock)
			pDock->SendMessage(WM_NOTIFY, 0, (LPARAM)&DragPos);
		else
		{
			if (GetDockHint().IsWindow())		GetDockHint().Destroy();
			CloseAllTargets();
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

	inline void CDockable::SetCaption(LPCTSTR szCaption) 
	{
		GetDockClient().SetCaption(szCaption);
		SetWindowText(szCaption);
	}

	inline void CDockable::SetDockWidth(int DockWidth) 
	{
		m_DockStartWidth = DockWidth;
		m_DockWidthRatio = 1.0;
	}

	inline void CDockable::SetView(CWnd& wndView)
	{
		CWnd* pWnd = &wndView;
		GetDockClient().SetView(wndView);
		if (pWnd->IsContainer())
		{
			CContainer* pContainer = (CContainer*)&wndView;
			SetCaption(pContainer->GetDockCaption().c_str());
		}
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
				for (UINT u = 0 ; u < m_pWndDockParent->m_vDockChildren.size(); ++u)
				{
					if (m_pWndDockParent->m_vDockChildren[u] == this)
					{
						if (m_vDockChildren.size() > 0)
							m_pWndDockParent->m_vDockChildren[u] = m_vDockChildren[0];
						else				
							m_pWndDockParent->m_vDockChildren.erase(m_pWndDockParent->m_vDockChildren.begin() + u);
						break;
					}
				}

				if (m_vDockChildren.size() > 0)
				{
					m_vDockChildren[0]->m_DockStyle = (m_vDockChildren[0]->m_DockStyle & 0xFFFFFFF0 ) | (m_DockStyle & 0xF);
					m_vDockChildren[0]->m_DockStartWidth = m_DockStartWidth;
					m_vDockChildren[0]->m_DockWidthRatio = m_DockWidthRatio;
					m_vDockChildren[0]->m_pWndDockParent = m_pWndDockParent;
					m_vDockChildren[0]->SetParent(m_pWndDockParent->GetHwnd());
					m_vDockChildren[0]->GetDockBar().SetParent(m_pWndDockParent->GetHwnd());
				}

				// Transfer the remaining dock children to the first dock child
				for (UINT u1 = 1; u1 < m_vDockChildren.size(); ++u1)
				{
					m_vDockChildren[u1]->m_pWndDockParent = m_vDockChildren[0];
					m_vDockChildren[u1]->SetParent(m_vDockChildren[0]->GetHwnd());
					m_vDockChildren[u1]->GetDockBar().SetParent(m_vDockChildren[0]->GetHwnd());
					m_vDockChildren[0]->m_vDockChildren.push_back(m_vDockChildren[u1]);
				}

				m_vDockChildren.clear();
				m_pWndDockParent = 0;
			}

			// Position and draw the undocked window, unless it is about to be closed
			if (!IsClosing())
			{
				// Change the window to an "undocked" style
				DWORD dwStyle = WS_POPUP| WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_VISIBLE;
				SetWindowLongPtr(GWL_STYLE, dwStyle);

				GetDockBar().ShowWindow(SW_HIDE);
				m_vDockChildren.clear();
				m_pWndDockParent = NULL;
				m_DockStyle = m_DockStyle & 0xFFFFFFF0;
				m_DockStyle &= ~DS_DOCKED_CONTAINER;
				RecalcDockLayout();

				// Supress redraw while we reposition the window
				SetRedraw(FALSE);
				CRect rc = GetDockClient().GetWindowRect();
				SetParent(0);
				m_Undocking = FALSE;

				SetWindowPos(NULL, pt.x - rc.Width()/2, pt.y - m_NCHeight/2, rc.Width(), rc.Height(), SWP_FRAMECHANGED| SWP_NOOWNERZORDER);
				
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
				if ((*iter).pWndContainer != pContainer)
				{
					pDockNew = (CDockable*)FromHandle(::GetParent((*iter).pWndContainer->GetParent()));	
					break;
				}
			}

			if (pDockNew)
			{
				// Move containers from the old dockable to the new dockable
				CContainer* pContainerNew = (CContainer*)pDockNew->GetView();
				for (iter = AllContainers.begin(); iter != AllContainers.end(); ++iter)
				{
					if ((*iter).pWndContainer != pContainer)
					{
						pContainer->RemoveContainer((*iter).pWndContainer);
						if (pContainerNew != (*iter).pWndContainer)
						{
							pContainerNew->AddContainer((*iter).pWndContainer);
							CDockable* pDock = GetDockFromView((*iter).pWndContainer);
							pDock->SetParent(pDockNew->GetHwnd());
						}
					}
				}

				// Now swap the dockables over
				pDockNew->m_vDockChildren	= m_vDockChildren;
				pDockNew->m_pWndDockParent	= m_pWndDockParent;
				pDockNew->m_DockStyle		= m_DockStyle;
				pDockNew->m_DockStartWidth	= m_DockStartWidth;
				pDockNew->m_DockWidthRatio	= m_DockWidthRatio;
				pDockNew->SetParent(GetParent());
				pDockNew->GetDockBar().SetParent(GetParent());
				pDockNew->GetView()->SetFocus();

				// insert pDockNew into the the DockParent's DockChildren vector
				std::vector<CDockable*>::iterator p;
				for (p = pDockNew->m_pWndDockParent->m_vDockChildren.begin(); p != pDockNew->m_pWndDockParent->m_vDockChildren.end(); ++p)
				{
					if (*p == this)
					{
						pDockNew->m_pWndDockParent->m_vDockChildren.insert(p, pDockNew);
						break;
					}
				}
			}			
		}
		else
		{
			// This is a child container, so simply remove it from the parent
			((CContainer*)GetView())->RemoveContainer(pContainer);
			((CContainer*)GetView())->SetTabSize();
			((CContainer*)GetView())->SetFocus();
		}
		
		// Finally do the actual undocking
		CDockable* pDock = GetDockFromView(pContainer);
		CRect rc = GetDockClient().GetWindowRect();
		MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rc, 2);
		pDock->GetDockClient().SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
		pDock->Undock();
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
						// Turn on DragFullWindows for this move
						SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, TRUE, 0, 0);			
					
						// Process this message
						DefWindowProc(uMsg, wParam, lParam);
						
						// Return DragFullWindows to its previous state
						SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, bResult, 0, 0);
						return 0L;
					}
				}
			}
			break; 

		case WM_EXITSIZEMOVE:
			m_BlockMove = FALSE;
			SendNotify(UWM_DOCK_END);
			break;

		case WM_WINDOWPOSCHANGING:
			{
				// Suspend dock drag moving while over dock zone
				if (m_BlockMove)
				{
                	LPWINDOWPOS pWndPos = (LPWINDOWPOS)lParam;
					pWndPos->flags |= SWP_NOMOVE|SWP_FRAMECHANGED;
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
						CloseAllTargets();
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
			{
				ShowWindow(SW_HIDE);
			}
			break;
		case WM_DESTROY:
			{
				// Post a destroy dockable message
				if (GetDockAncestor() != this)
					::PostMessage(GetDockAncestor()->GetHwnd(), UWM_DOCK_DESTROYED, (WPARAM)this, 0);
			}
			break;

		case WM_SETFOCUS:
			// Pass focus on the the view window
			GetView()->SetFocus();
			break;

		case UWM_DOCK_DESTROYED:
			{
				CDockable* pDock = (CDockable*)wParam;
				if (pDock != GetDockAncestor())
				{
					delete pDock;
				}
			}
		}

		return CWnd::WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

	///////////////////////////////////////////
	// Declaration of the nested CTabPage class
	inline BOOL CContainer::CTabPage::OnCommand(WPARAM wParam, LPARAM lParam)
	{
		return (BOOL)::SendMessage(GetParent(), WM_COMMAND, wParam, lParam);
	}

	inline void CContainer::CTabPage::OnCreate()
	{
		m_pWndView->Create(m_hWnd);
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
		CRect rcToolbar = m_WndToolbar.GetClientRect();
		rc.top += rcToolbar.Height();
		GetView()->SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
	}

	inline void CContainer::CTabPage::SetView(CWnd& wndView)
	// Sets or changes the View window displayed within the frame
	{
		// Assign the view window
		m_pWndView = &wndView;

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
				::SendMessage(::GetParent(GetParent()), WM_NOTIFY, wParam, lParam);
				break;
			}

			break;
		}

		// pass unhandled messages on for default processing
		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

	//////////////////////////////////////
	// Declaration of the CContainer class
	inline CContainer::CContainer() : m_iCurrentPage(0), m_hTabIcon(0)
	{
		m_himlTab = ImageList_Create(16, 16, ILC_MASK|ILC_COLOR32, 0, 0);
		TabCtrl_SetImageList(m_hWnd, m_himlTab);
		m_pWndContainerParent = this;

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

	inline void CContainer::AddContainer(CContainer* pWndContainer)
	{
		if (this == m_pWndContainerParent)
		{
			TabPageInfo tbi = {0};
			tbi.pWndContainer = pWndContainer;
			lstrcpy(tbi.szTitle, pWndContainer->GetTabText());
			tbi.iImage = ImageList_AddIcon(m_himlTab, pWndContainer->GetTabIcon());
			int iNewPage = m_vTabPageInfo.size();
			m_vTabPageInfo.push_back(tbi);		

			if (m_hWnd)
			{
				TCITEM tie = {0};
				tie.mask = TCIF_TEXT | TCIF_IMAGE;
				tie.iImage = tbi.iImage;
				tie.pszText = m_vTabPageInfo[iNewPage].szTitle;
				TabCtrl_InsertItem(m_hWnd, iNewPage, &tie);

				SetTabSize();
			}

			pWndContainer->m_pWndContainerParent = this;
			if (pWndContainer->IsWindow())
			{
				// Set the parent container relationships
				pWndContainer->GetTabPage().SetParent(m_hWnd);
				pWndContainer->GetTabPage().ShowWindow(SW_HIDE);
			}
		}
	}

	inline void CContainer::AddToolbarButton(UINT nID)
	// Adds Resource IDs to toolbar buttons.
	// A resource ID of 0 is a separator
	{
		m_vToolbarData.push_back(nID);
	}

	inline CContainer* CContainer::GetContainerFromIndex(int iPage)
	{
		if ((iPage >= 0) && (iPage < (int)m_vTabPageInfo.size()))
			return m_vTabPageInfo[iPage].pWndContainer;
		else
			return NULL;
	}

	inline CWnd* CContainer::GetActiveView() const
	{	
		if (m_vTabPageInfo.size() > 0)
			return m_pWndContainerParent->m_vTabPageInfo[m_iCurrentPage].pWndContainer->GetTabPage().GetView();
		else
			return NULL;
	}

	inline CContainer* CContainer::GetContainerFromView(CWnd* pWndView) const
	{
		std::vector<TabPageInfo>::iterator iter;
		CContainer* pContainer = 0;
		for (iter = GetAllContainers().begin(); iter != GetAllContainers().end(); ++iter)
		{
			if ((*iter).pWndContainer->GetView() == pWndView)
				pContainer = (*iter).pWndContainer;
		}

		return pContainer;
	}

	inline int CContainer::GetContainerIndex(CContainer* pWndContainer)
	{
		for (int i = 0; i < (int)m_vTabPageInfo.size(); ++i)
		{
			if (m_vTabPageInfo[i].pWndContainer == pWndContainer)
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

		TabPageInfo tbi = {0};
		tbi.pWndContainer = this;
		lstrcpy(tbi.szTitle, GetTabText());
		tbi.iImage = ImageList_AddIcon(m_himlTab, GetTabIcon()); 
		m_vTabPageInfo.push_back(tbi);
						
		// Create the page window
		m_WndPage.Create(m_hWnd);

		// Create the toolbar
		if (m_vToolbarData.size() > 0)
		{
			GetToolbar().Create(m_WndPage.GetHwnd());
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
		if (rcTab.Height() < 0) rcTab.top = rcTab.bottom;
		if (rcTab.Width() < 0) rcTab.left = rcTab.right;
		
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
		if (iTab > 0)
		{
			TabCtrl_DeleteItem(m_hWnd, iTab);
		}
	
		// Remove the tabPageInfo entry
		std::vector<TabPageInfo>::iterator iter;
		int iImage = -1;
		for (iter = m_vTabPageInfo.begin(); iter != m_vTabPageInfo.end(); ++iter)
		{
			if (iter->pWndContainer == pWnd)
			{
				iImage = (*iter).iImage;
				if (iImage >= 0) 
					TabCtrl_RemoveImage(m_hWnd, iImage);
				
				m_vTabPageInfo.erase(iter);
				break;
			}
		}

		// Set the parent container relationships
		pWnd->GetTabPage().SetParent(pWnd->GetHwnd());
		pWnd->m_pWndContainerParent = pWnd;

		// Display the first page
		SelectPage(0);
	}

	inline void CContainer::SelectPage(int iPage)
	{
		if ((iPage >= 0) && (iPage < (int)m_vTabPageInfo.size() ))
		{
			TabCtrl_SetCurSel(m_hWnd, iPage);

			// Create the new container window if required
			if (!m_vTabPageInfo[iPage].pWndContainer->IsWindow())
			{
				m_vTabPageInfo[iPage].pWndContainer->Create(GetParent());
				m_vTabPageInfo[iPage].pWndContainer->GetTabPage().SetParent(m_hWnd);
			}
			
			// Determine the size of the tab page's view area
			CRect rc = GetClientRect();
			TabCtrl_AdjustRect(m_hWnd, FALSE, &rc);

			// Swap the pages over
			m_vTabPageInfo[m_iCurrentPage].pWndContainer->GetTabPage().ShowWindow(SW_HIDE);
			m_vTabPageInfo[iPage].pWndContainer->GetTabPage().SetWindowPos(HWND_TOP, rc, SWP_SHOWWINDOW);
			m_vTabPageInfo[iPage].pWndContainer->GetTabPage().GetView()->SetFocus();

			// Adjust the docking caption
			if (::SendMessage(::GetParent(GetParent()), UWM_IS_DOCKABLE, 0, 0))
			{
				CDockable* pDock = (CDockable*)FromHandle(::GetParent(GetParent()));
				pDock->SetCaption(m_vTabPageInfo[iPage].pWndContainer->GetDockCaption().c_str());
				pDock->RedrawWindow();
			}
			
			m_iCurrentPage = iPage;
		}
	}

	inline void CContainer::SetTabSize()
	{
		CRect rc = GetClientRect();
		TabCtrl_AdjustRect(m_hWnd, FALSE, &rc);
		int nItemWidth = min(25 + GetMaxTabTextSize().cx, (rc.Width()-2)/(int)m_vTabPageInfo.size());
		SendMessage(TCM_SETITEMSIZE, 0, MAKELPARAM(nItemWidth, 20));
	}

	inline void CContainer::SetTabIcon(UINT nID_Icon)
	{
		HICON hIcon = LoadIcon(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(nID_Icon));
		SetTabIcon(hIcon);
	}

	inline void CContainer::SetView(CWnd& Wnd)
	{
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
					m_vTabPageInfo[m_iCurrentPage].pWndContainer->GetTabPage().SetWindowPos(HWND_TOP, rc, SWP_SHOWWINDOW);
				} 			
			}
			break;

		case WM_MOUSELEAVE:
			{
				IsTracking = FALSE;
				if (IsLeftButtonDown())
				{
					if (::SendMessage(::GetParent(GetParent()), UWM_IS_DOCKABLE, 0, 0)) 
					{
						CContainer* pContainer = GetContainerFromIndex(m_iCurrentPage);

						CDockable* pDock = (CDockable*) FromHandle(::GetParent(GetParent()));
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
				GetActiveView()->SetFocus();
			}
			break;
		}

		// pass unhandled messages on for default processing
		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

} // namespace Win32xx

#endif // DOCKABLE_H

