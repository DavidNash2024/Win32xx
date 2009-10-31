// Win32++  Version 6.7
// Released: 6th November, 2009 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
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
// docking.h
//  Declaration of the CDocker class

#ifndef _DOCKING_H_
#define _DOCKING_H_


#include "wincore.h"
#include "gdi.h"
#include "toolbar.h"
#include "tab.h"
#include "frame.h"
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
#define DS_NO_CAPTION			0x0200  // Prevent display of caption when docked
#define DS_NO_UNDOCK			0x0400  // Prevent Undocking
#define DS_CLIENTEDGE			0x1000  // Has a 3D border when docked
#define DS_FLATLOOK				0x2000	// Reserved for future use
#define DS_DOCKED_CONTAINER		0x4000  // Dock a container within a container
#define DS_DOCKED_LEFTMOST      0x10000 // Leftmost outer docking
#define DS_DOCKED_RIGHTMOST     0x20000 // Rightmost outer docking
#define DS_DOCKED_TOPMOST		0x40000 // Topmost outer docking
#define DS_DOCKED_BOTTOMMOST	0x80000 // Bottommost outer docking

// Required for Dev-C++
#ifndef TME_NONCLIENT
  #define TME_NONCLIENT 0x00000010
#endif
#ifndef TME_LEAVE
  #define TME_LEAVE 0x000000002
#endif
#ifndef WM_NCMOUSELEAVE
  #define WM_NCMOUSELEAVE 0x000002A2
#endif

namespace Win32xx
{
  // Class declarations
  class CContainer;
	class CDocker;

  struct ContainerInfo
	{
		TCHAR szTitle[MAX_MENU_STRING];
		int iImage;
		CContainer* pContainer;
	};

	///////////////////////////////////////
	// Declaration of the CContainer class
	//  A CContainer is a CTab window. A CTab has a view window, and optionally a toolbar control.
	//  A top level CContainer can contain other CContainers. The view for each container
	//  (including the top level container) along with possibly its toolbar, is displayed
	//  within the container parent's view page.
	class CContainer : public CTab
	{
		// Nested class. This is the Wnd for the window displayed over the client area
		// of the tab control.  The toolbar and view window are child windows of the
		// viewpage window. Only the ViewPage of the parent CContainer is displayed. It's
		// contents are updated with the view window of the relevant container whenever
		// a different tab is selected.
		class CViewPage : public CWnd
		{

		public:
			CViewPage() : m_pView(NULL), m_pTab(NULL) {}
			virtual ~CViewPage() {}
			virtual CToolbar& GetToolbar() const {return (CToolbar&)m_Toolbar;}
			virtual CWnd* GetView() const	{return m_pView;}
			virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
			virtual void OnCreate();
			virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
			virtual void PreRegisterClass(WNDCLASS &wc);
			virtual void RecalcLayout();
			virtual void SetView(CWnd& wndView);
			virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

			CWnd* GetTabCtrl() const { return m_pTab;}

		private:
			CToolbar m_Toolbar;
			CWnd* m_pView;
			CWnd* m_pTab;
		};

	public:
		CContainer();
		virtual ~CContainer();
		virtual void AddContainer(CContainer* pContainer);
		virtual void AddToolbarButton(UINT nID, BOOL bEnabled = TRUE);
		virtual CContainer* GetContainerFromIndex(size_t iPage);
		virtual CContainer* GetContainerFromView(CWnd* pView) const;
		virtual int GetContainerIndex(CContainer* pContainer);
		virtual SIZE GetMaxTabTextSize();
		virtual CViewPage& GetViewPage() const	{ return (CViewPage&)m_ViewPage; }
		virtual void RecalcLayout();
		virtual void RemoveContainer(CContainer* pWnd);
		virtual void SelectPage(int iPage);
		virtual void SetTabSize();
		virtual void SetupToolbar();

		// Attributes
		CContainer* GetActiveContainer() const {return GetContainerFromView(GetActiveView());}
		CWnd* GetActiveView() const;
		std::vector<ContainerInfo>& GetAllContainers() const {return m_pContainerParent->m_vContainerInfo;}
		CContainer* GetContainerParent() { return m_pContainerParent; }
		tString GetDockCaption() const	{ return m_tsCaption; }
		HICON GetTabIcon() const		{ return m_hTabIcon; }
		LPCTSTR GetTabText() const		{ return m_tsTabText.c_str(); }
		virtual CToolbar& GetToolbar() const	{ return GetViewPage().GetToolbar(); }
		CWnd* GetView() const			{ return GetViewPage().GetView(); }
		BOOL IsContainer() const		{ return TRUE; }
		void SetDockCaption(LPCTSTR szCaption) { m_tsCaption = szCaption; }
		void SetTabIcon(HICON hTabIcon) { m_hTabIcon = hTabIcon; }
		void SetTabIcon(UINT nID_Icon);
		void SetTabText(LPCTSTR szText) { m_tsTabText = szText; }
		void SetTabText(UINT nTab, LPCTSTR szText);
		void SetView(CWnd& Wnd);

	protected:
		virtual void OnCreate();
		virtual void OnLButtonDown(WPARAM wParam, LPARAM lParam);
		virtual void OnLButtonUp(WPARAM wParam, LPARAM lParam);
		virtual void OnMouseLeave(WPARAM wParam, LPARAM lParam);
		virtual void OnMouseMove(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		std::vector<ContainerInfo> m_vContainerInfo;
		tString m_tsTabText;
		tString m_tsCaption;
		CViewPage m_ViewPage;
		int m_iCurrentPage;
		CContainer* m_pContainerParent;
		HICON m_hTabIcon;
		int m_nTabPressed;

	};

	typedef struct DRAGPOS
	{
		NMHDR hdr;
		POINT ptPos;
		UINT DockZone;
	} *LPDRAGPOS;

	/////////////////////////////////////////
	// Declaration of the CDocker class
	//  A CDocker window allows other CDocker windows to be "docked" inside it.
	//  A CDocker can dock on the top, left, right or bottom side of a parent CDocker.
	//  There is no theoretical limit to the number of CDockers within CDockers.
	class CDocker : public CWnd
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
			virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

			CDocker* GetDock()			{return m_pDock;}
			int GetWidth()					{return m_DockBarWidth;}
			void SetDock(CDocker* pDock)	{m_pDock = pDock;}
			void SetWidth(int nWidth)		{m_DockBarWidth = nWidth;}

		private:
			CDockBar(const CDockBar&);				// Disable copy construction
			CDockBar& operator = (const CDockBar&); // Disable assignment operator

			CDocker* m_pDock;
			HBRUSH m_hbrBackground;
			int m_DockBarWidth;
		};

		//  A nested class for the window inside a CDocker which includes all of this docked client.
		//  It's the remaining part of the CDocker that doesn't belong to the CDocker's children.
		//  The Docker's view window is a child window of CDockClient.
		class CDockClient : public CWnd
		{
		public:
			CDockClient();
			virtual ~CDockClient() {}
			virtual void Draw3DBorder(RECT& Rect);
			virtual void DrawCaption(WPARAM wParam);
			virtual void DrawCloseButton(CDC& DrawDC, BOOL bFocus);
			virtual void SendNotify(UINT nMessageID);

			tString GetCaption() const		{ return m_tsCaption; }
			CRect GetCloseRect();
			CWnd* GetView() const			{ return m_pView; }
			BOOL IsClosing();
			void SetDock(CDocker* pDock)	{ m_pDock = pDock;}
			void SetCaption(LPCTSTR szCaption) { m_tsCaption = szCaption; }
			void SetClosePressed()			{ m_IsClosePressed = TRUE; }
			void SetView(CWnd& Wnd)			{ m_pView = &Wnd; }

		protected:
			virtual void    OnLButtonDown(WPARAM wParam, LPARAM lParam);
			virtual void OnLButtonUp(WPARAM wParam, LPARAM lParam);
			virtual void    OnMouseActivate(WPARAM wParam, LPARAM lParam);
			virtual void    OnMouseMove(WPARAM wParam, LPARAM lParam);
			virtual void    OnNCCalcSize(WPARAM& wParam, LPARAM& lParam);
			virtual LRESULT OnNCHitTest(WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnNCLButtonDown(WPARAM wParam, LPARAM lParam);
			virtual void    OnNCMouseLeave(WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnNCMouseMove(WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnNCPaint(WPARAM wParam, LPARAM lParam);
			virtual void    OnWindowPosChanged(WPARAM wParam, LPARAM lParam);
			virtual void    PreRegisterClass(WNDCLASS& wc);
			virtual void    PreCreate(CREATESTRUCT& cs);
			virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

		private:
			CDockClient(const CDockClient&);				// Disable copy construction
			CDockClient& operator = (const CDockClient&); // Disable assignment operator
			
			tString m_tsCaption;
			CPoint m_Oldpt;
			CDocker* m_pDock;
			CWnd* m_pView;
			int m_NCHeight;
			BOOL m_IsClosePressed;
			BOOL m_bOldFocus;
			BOOL m_bCaptionPressed;
			BOOL m_IsTracking;
		};

		//  This nested class is used to indicate where a window could dock by
		//  displaying a blue tinted window.
		class CDockHint : public CWnd
		{
		public:
			CDockHint();
			virtual ~CDockHint();
			virtual RECT CalcHintRectContainer(CDocker* pDockTarget);
			virtual RECT CalcHintRectInner(CDocker* pDockTarget, CDocker* pDockDrag, UINT uDockSide);
			virtual RECT CalcHintRectOuter(CDocker* pDockDrag, UINT uDockSide);
			virtual void DisplayHint(CDocker* pDockTarget, CDocker* pDockDrag, UINT uDockSide);
			virtual void OnPaint(HDC hDC);
			virtual void PreCreate(CREATESTRUCT &cs);
			virtual void SetBitmap(HBITMAP hbmBlueTint);
			virtual void ShowHintWindow(CDocker* pDockTarget, CRect rcHint);

		private:
			CDockHint(const CDockHint&);				// Disable copy construction
			CDockHint& operator = (const CDockHint&); // Disable assignment operator

			HBITMAP m_hbmBlueTint;
			UINT m_uDockSideOld;
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
			CTarget(const CTarget&);				// Disable copy construction
			CTarget& operator = (const CTarget&); // Disable assignment operator

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
			CTargetCentre(const CTargetCentre&);				// Disable copy construction
			CTargetCentre& operator = (const CTargetCentre&); // Disable assignment operator

			BOOL m_bIsOverContainer;
			CDocker* m_pOldDockTarget;
		};

		class CTargetLeft : public CTarget
		{
		public:
			CTargetLeft() {SetImage(IDW_SDLEFT);}
			virtual BOOL CheckTarget(LPDRAGPOS pDragPos);
		
		private:
			CTargetLeft(const CTargetLeft&);				// Disable copy construction
			CTargetLeft& operator = (const CTargetLeft&); // Disable assignment operator
		};

		class CTargetTop : public CTarget
		{
		public:
			CTargetTop() {SetImage(IDW_SDTOP);}
			virtual BOOL CheckTarget(LPDRAGPOS pDragPos);
		private:
			CTargetTop(const CTargetTop&);				// Disable copy construction
			CTargetTop& operator = (const CTargetTop&); // Disable assignment operator
		};

		class CTargetRight : public CTarget
		{
		public:
			CTargetRight() {SetImage(IDW_SDRIGHT);}
			virtual BOOL CheckTarget(LPDRAGPOS pDragPos);

		private:
			CTargetRight(const CTargetRight&);				// Disable copy construction
			CTargetRight& operator = (const CTargetRight&); // Disable assignment operator
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
		friend class CDockClient;
		friend class CContainer;

	public:
		// Operations
		CDocker();
		virtual ~CDocker();
		virtual CDocker* AddDockedChild(CDocker* pDocker, DWORD dwDockStyle, int DockWidth, int nDockID = 0);
		virtual CDocker* AddUndockedChild(CDocker* pDocker, DWORD dwDockStyle, int DockWidth, RECT rc, int nDockID = 0);
		virtual void CloseAllDockers();
		virtual CContainer* GetContainer() const;
		virtual CDocker* GetDockFromPoint(POINT pt) const;
		virtual CDocker* GetDockAncestor() const;
		virtual CDocker* GetDockFromID(int n_DockID) const;
		virtual CDocker* GetDockFromView(CWnd* pView) const;
		virtual CDocker* GetDockTopLevel() const;
		virtual int GetDockWidth() const;
		virtual CTabbedMDI* GetTabbedMDI() const;
		virtual BOOL LoadRegistrySettings(tString tsRegistryKeyName);
		virtual void RecalcDockLayout();
		virtual void SaveRegistrySettings(tString tsRegistryKeyName);
		virtual BOOL VerifyDockers();

		// Attributes
		virtual CDockBar& GetDockBar() const {return (CDockBar&)m_DockBar;}
		virtual CDockClient& GetDockClient() const {return (CDockClient&)m_DockClient;}
		virtual CDockHint& GetDockHint() const {return m_pDockAncestor->m_DockHint;}

		std::vector <CDocker*> & GetAllDockers() const {return GetDockAncestor()->m_vAllDockers;}
		std::vector <CDocker*> & GetDockChildren() const {return (std::vector <CDocker*> &)m_vDockChildren;}
		int GetBarWidth() const {return GetDockBar().GetWidth();}
		tString GetCaption() const {return GetDockClient().GetCaption();}
		int GetDockID() const {return m_nDockID;}
		CDocker* GetDockParent() const {return m_pDockParent;}
		DWORD GetDockStyle() const {return m_DockStyle;}
		CWnd* GetView() const {return GetDockClient().GetView();}
		BOOL IsChildOfDocker(HWND hwnd) const;
		BOOL IsClosing() const {return GetDockClient().IsClosing();}
		BOOL IsDocked() const;
		BOOL IsDocker() const {return TRUE;}
		BOOL IsDragAutoResize();
		BOOL IsRelated(HWND hWnd) const;
		BOOL IsUndocked() const;
		void SetBarColor(COLORREF color) {GetDockBar().SetColor(color);}
		void SetBarWidth(int nWidth) {GetDockBar().SetWidth(nWidth);}
		void SetCaption(LPCTSTR szCaption);
		void SetDockStyle(DWORD dwDockStyle);
		void SetDockWidth(int DockWidth);
		void SetDragAutoResize(BOOL bAutoResize);
		void SetView(CWnd& wndView);

	protected:
		virtual CDocker* NewDockerFromID(int nID);
		virtual void OnActivate(WPARAM wParam, LPARAM lParam);
		virtual void OnCaptionTimer(WPARAM wParam, LPARAM lParam);
		virtual void OnCreate();
		virtual void OnDestroy(WPARAM wParam, LPARAM lParam);
		virtual void OnDockDestroyed(WPARAM wParam, LPARAM lParam);
		virtual void OnExitSizeMove(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual void OnSetFocus(WPARAM wParam, LPARAM lParam);
		virtual void OnSysColorChange(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnSysCommand(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnWindowPosChanging(WPARAM wParam, LPARAM lParam);
		virtual void OnWindowPosChanged(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual void PreRegisterClass(WNDCLASS &wc);
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		CDocker(const CDocker&);				// Disable copy construction
		CDocker& operator = (const CDocker&); // Disable assignment operator
		void CheckAllTargets(LPDRAGPOS pDragPos);
		void CloseAllTargets();
		void Dock(CDocker* hDocker, UINT uDockSide);
		void DockInContainer(CDocker* pDock, DWORD dwDockStyle);
		void DockOuter(CDocker* pDocker, DWORD dwDockStyle);
		void DrawAllCaptions();
		void DrawHashBar(HWND hBar, POINT Pos);
		void ConvertToChild(HWND hWndParent);
		void ConvertToPopup(RECT rc);
		void MoveDockChildren(CDocker* pDockTarget);
		void PromoteFirstChild();
		void RecalcDockChildLayout(CRect rc);
		void ResizeDockers(LPDRAGPOS pdp);
		void SendNotify(UINT nMessageID);
		void Undock();
		void UndockContainer(CContainer* pContainer);

		CDockBar		m_DockBar;
		CDockHint		m_DockHint;
		CDockClient		m_DockClient;
		CTargetCentre	m_TargetCentre;
		CTargetLeft		m_TargetLeft;
		CTargetTop		m_TargetTop;
		CTargetRight	m_TargetRight;
		CPoint			m_OldPoint;
		CTargetBottom	m_TargetBottom;
		CDocker*		m_pDockParent;
		CDocker*		m_pDockAncestor;

		std::vector <CDocker*> m_vDockChildren;
		std::vector <CDocker*> m_vAllDockers;	// Only used in DockAncestor

		BOOL m_BlockMove;
		BOOL m_Undocking;
		BOOL m_bIsClosing;
		BOOL m_bIsDragging;
		BOOL m_bDragAutoResize;
		int m_DockStartWidth;
		int m_nDockID;
		int m_nTimerCount;
		int m_NCHeight;
		DWORD m_dwDockZone;
		double m_DockWidthRatio;
		DWORD m_DockStyle;
		HBRUSH m_hbrDithered;
		HBITMAP	m_hbmHash;
		HWND m_hOldFocus;

	}; // class CDocker

	struct DockInfo
	{
		DWORD DockStyle;
		int DockWidth;
		int DockID;
		int DockParentID;
		RECT Rect;
	};

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace Win32xx
{

	/////////////////////////////////////////////////////////////
	// Definitions for the CDockBar class nested within CDocker
	//
	inline CDocker::CDockBar::CDockBar() : m_pDock(NULL), m_DockBarWidth(4)
	{
		m_hbrBackground = ::CreateSolidBrush(RGB(192,192,192));
	}

	inline CDocker::CDockBar::~CDockBar()
	{
		::DeleteObject(m_hbrBackground);
	}

	inline void CDocker::CDockBar::OnPaint(HDC hDC)
	{
		CRect rcClient = GetClientRect();
		CDC dcView = hDC;
		dcView.AttachBrush(m_hbrBackground);
		dcView.PatBlt(0, 0, rcClient.Width(), rcClient.Height(), PATCOPY);
		dcView.DetachBrush();
	}

	inline void CDocker::CDockBar::PreCreate(CREATESTRUCT &cs)
	{
		// Create a child window, initially hidden
		cs.style = WS_CHILD;
	}

	inline void CDocker::CDockBar::PreRegisterClass(WNDCLASS& wc)
	{
		wc.lpszClassName = _T("Win32++ Bar");
		wc.hbrBackground = m_hbrBackground;
	}

	inline void CDocker::CDockBar::SendNotify(UINT nMessageID)
	{
		// Send a splitter bar notification to the parent
		DRAGPOS DragPos;
		DragPos.hdr.code = nMessageID;
		DragPos.hdr.hwndFrom = m_hWnd;
		::GetCursorPos(&DragPos.ptPos);
		DragPos.ptPos.x += 1;
		SendMessage(GetParent(), WM_NOTIFY, 0L, (LPARAM)&DragPos);
	}

	inline void CDocker::CDockBar::SetColor(COLORREF color)
	{
		// Useful colors:
		// GetSysColor(COLOR_BTNFACE)	// Default Grey
		// RGB(196, 215, 250)			// Default Blue

		if (m_hbrBackground)
			::DeleteObject(m_hbrBackground);

		m_hbrBackground = ::CreateSolidBrush(color);
	}

	inline LRESULT CDocker::CDockBar::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

			case WM_ERASEBKGND:
				return 0;

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
				if (!(m_pDock->GetDockStyle() & DS_NO_RESIZE) && (GetCapture() == m_hWnd))
				{
					SendNotify(UWM_BAR_END);
					ReleaseCapture();
				}
				break;

			case WM_MOUSEMOVE:
				if (!(m_pDock->GetDockStyle() & DS_NO_RESIZE) && (GetCapture() == m_hWnd))
				{
					SendNotify(UWM_BAR_MOVE);
				}
				break;
			}
		}

		// pass unhandled messages on for default processing
		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	}


	////////////////////////////////////////////////////////////////
	// Definitions for the CDockClient class nested within CDocker
	//
	inline CDocker::CDockClient::CDockClient() : m_pView(0), m_NCHeight(20),
						m_IsClosePressed(FALSE), m_bOldFocus(FALSE), m_bCaptionPressed(FALSE),
						m_IsTracking(FALSE)
	{
	}

	inline void CDocker::CDockClient::Draw3DBorder(RECT& Rect)
	{
		// Imitates the drawing of the WS_EX_CLIENTEDGE extended style
		// This draws a 2 pixel border around the specified Rect
		CDC dc = GetWindowDC();
		CRect rcw = Rect;
		dc.CreatePen(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
		dc.MoveTo(0, rcw.Height());
		dc.LineTo(0, 0);
		dc.LineTo(rcw.Width(), 0);
		dc.CreatePen(PS_SOLID,1, GetSysColor(COLOR_3DDKSHADOW));
		dc.MoveTo(1, rcw.Height()-2);
		dc.LineTo(1, 1);
		dc.LineTo(rcw.Width()-2, 1);
		dc.CreatePen(PS_SOLID,1, GetSysColor(COLOR_3DHILIGHT));
		dc.MoveTo(rcw.Width()-1, 0);
		dc.LineTo(rcw.Width()-1, rcw.Height()-1);
		dc.LineTo(0, rcw.Height()-1);
		dc.CreatePen(PS_SOLID,1, GetSysColor(COLOR_3DLIGHT));
		dc.MoveTo(rcw.Width()-2, 1);
		dc.LineTo(rcw.Width()-2, rcw.Height()-2);
		dc.LineTo(1, rcw.Height()-2);
	}

	inline CRect CDocker::CDockClient::GetCloseRect()
	{
		// Calculate the close rect position in screen co-ordinates
		CRect rcClose;

		int gap = 4;
		CRect rc = GetWindowRect();
		int cx = GetSystemMetrics(SM_CXSMICON);
		int cy = GetSystemMetrics(SM_CYSMICON);

		rcClose.top = 2 + rc.top + m_NCHeight/2 - cy/2;
		rcClose.bottom = 2 + rc.top + m_NCHeight/2 + cy/2;
		rcClose.right = rc.right - gap;
		rcClose.left = rcClose.right - cx;

#if WINVER >= 0x0500
		if (GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_LAYOUTRTL)
		{
			rcClose.left = rc.left + gap;
			rcClose.right = rcClose.left + cx; 
		}
#endif


		return rcClose;
	}

	inline void CDocker::CDockClient::DrawCaption(WPARAM wParam)
	{
		if (IsWindow() && m_pDock->IsDocked() && !(m_pDock->GetDockStyle() & DS_NO_CAPTION))
		{
			BOOL bFocus = m_pDock->IsChildOfDocker(GetFocus());
			m_bOldFocus = FALSE;

			// Acquire the DC for our NonClient painting
			// Note the Microsoft documentation for this neglects to mention DCX_PARENTCLIP
			CDC dc;
			if ((wParam != 1) && (bFocus == m_bOldFocus))
				dc = GetDCEx((HRGN)wParam, DCX_WINDOW|DCX_INTERSECTRGN|DCX_PARENTCLIP);
			else
				dc 	= GetWindowDC();

			CRect rc = GetWindowRect();

			// Create and set up our memory DC
			CDC dcMem = dc.CreateCompatibleDC();
			int rcAdjust = (GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_CLIENTEDGE)? 2 : 0;
			int Width = MAX(rc.Width() -rcAdjust, 0);
			int Height = m_NCHeight + rcAdjust;
			dcMem.CreateCompatibleBitmap(dc, Width, Height);
			m_bOldFocus = bFocus;

			// Set the font for the title
			NONCLIENTMETRICS info = {0};
			info.cbSize = GetSizeofNonClientMetrics();
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
			dcMem.CreateFontIndirect(info.lfStatusFont);

			// Set the Colours
			if (bFocus)
			{
				dcMem.CreateSolidBrush(GetSysColor(COLOR_ACTIVECAPTION));
				dcMem.SetBkColor(GetSysColor(COLOR_ACTIVECAPTION));
				dcMem.SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
			}
			else
			{
				dcMem.CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
				dcMem.SetBkColor(GetSysColor(COLOR_BTNFACE));
				dcMem.SetTextColor(GetSysColor(COLOR_BTNTEXT));
			}

			// Draw the rectangle
			dcMem.CreatePen(PS_SOLID, 1, RGB(160, 150, 140));
			dcMem.Rectangle(rcAdjust, rcAdjust, rc.Width() -rcAdjust, m_NCHeight +rcAdjust);

			// Display the caption
			int cx = GetSystemMetrics(SM_CXSMICON);
			CRect rcText(4 +rcAdjust, rcAdjust, rc.Width() -4 - cx -rcAdjust, m_NCHeight +rcAdjust);
			dcMem.DrawText(m_tsCaption.c_str(), -1, rcText, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			// Draw the close button
			DrawCloseButton(dcMem, bFocus);

			// Draw the 3D border
			if (GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_CLIENTEDGE)
				Draw3DBorder(rc);

			// Copy the Memory DC to the window's DC
			dc.BitBlt(rcAdjust, rcAdjust, Width, Height, dcMem, rcAdjust, rcAdjust, SRCCOPY);
		}
	}

	inline void CDocker::CDockClient::DrawCloseButton(CDC& DrawDC, BOOL bFocus)
	{
		// The close button isn't displayed on Win95
		if (GetWinVersion() == 1400)  return;

		if (m_pDock->IsDocked() && !(m_pDock->GetDockStyle() & DS_NO_CAPTION))
		{
			// Determine the close button's drawing position relative to the window
			CRect rcClose = GetCloseRect();
			UINT uState = GetCloseRect().PtInRect(GetCursorPos())? m_IsClosePressed && IsLeftButtonDown()? 2 : 1 : 0;
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
						// Normal button
						if (bFocus)
							DrawDC.CreatePen(PS_SOLID, 1, GetSysColor(COLOR_ACTIVECAPTION));
						else
							DrawDC.CreatePen(PS_SOLID, 1, RGB(232, 228, 220));

						DrawDC.MoveTo(rcClose.left, rcClose.bottom);
						DrawDC.LineTo(rcClose.right, rcClose.bottom);
						DrawDC.LineTo(rcClose.right, rcClose.top);
						DrawDC.LineTo(rcClose.left, rcClose.top);
						DrawDC.LineTo(rcClose.left, rcClose.bottom);
						break;
					}

				case 1:
					{
						// Popped up button
						// Draw outline, white at top, black on bottom
						DrawDC.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
						DrawDC.MoveTo(rcClose.left, rcClose.bottom);
						DrawDC.LineTo(rcClose.right, rcClose.bottom);
						DrawDC.LineTo(rcClose.right, rcClose.top);
						DrawDC.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
						DrawDC.LineTo(rcClose.left, rcClose.top);
						DrawDC.LineTo(rcClose.left, rcClose.bottom);
					}

					break;
				case 2:
					{
						// Pressed button
						// Draw outline, black on top, white on bottom
						DrawDC.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
						DrawDC.MoveTo(rcClose.left, rcClose.bottom);
						DrawDC.LineTo(rcClose.right, rcClose.bottom);
						DrawDC.LineTo(rcClose.right, rcClose.top);
						DrawDC.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
						DrawDC.LineTo(rcClose.left, rcClose.top);
						DrawDC.LineTo(rcClose.left, rcClose.bottom);
					}
					break;
				}

				// Manually Draw Close Button
				if (bFocus)
					DrawDC.CreatePen(PS_SOLID, 1, RGB(230, 230, 230));
				else
					DrawDC.CreatePen(PS_SOLID, 1, RGB(64, 64, 64));

				DrawDC.MoveTo(rcClose.left + 3, rcClose.top +3);
				DrawDC.LineTo(rcClose.right - 3, rcClose.bottom -3);

				DrawDC.MoveTo(rcClose.left + 4, rcClose.top +3);
				DrawDC.LineTo(rcClose.right - 3, rcClose.bottom -4);

				DrawDC.MoveTo(rcClose.left + 3, rcClose.top +4);
				DrawDC.LineTo(rcClose.right - 4, rcClose.bottom -3);

				DrawDC.MoveTo(rcClose.right -4, rcClose.top +3);
				DrawDC.LineTo(rcClose.left + 2, rcClose.bottom -3);

				DrawDC.MoveTo(rcClose.right -4, rcClose.top +4);
				DrawDC.LineTo(rcClose.left + 3, rcClose.bottom -3);

				DrawDC.MoveTo(rcClose.right -5, rcClose.top +3);
				DrawDC.LineTo(rcClose.left + 2, rcClose.bottom -4);
			}
		}
	}

	inline BOOL CDocker::CDockClient::IsClosing()
	{
		return (m_IsClosePressed && !IsLeftButtonDown());
	}

	inline void CDocker::CDockClient::OnNCCalcSize(WPARAM& /*wParam*/, LPARAM& lParam)
	{
		// Sets the non-client area (and hence sets the client area)
		// This function modifies lParam
		if ((0 != m_pDock) && !(m_pDock->GetDockStyle() & DS_NO_CAPTION))
		{
			if (m_pDock->IsDocked())
			{
				LPRECT rc = (LPRECT)lParam;
				rc->top += m_NCHeight;
			}
		}
	}

	inline LRESULT CDocker::CDockClient::OnNCHitTest(WPARAM wParam, LPARAM lParam)
	{
		// Identify which part of the non-client area the cursor is over
		if ((0 != m_pDock) && !(m_pDock->GetDockStyle() & DS_NO_CAPTION))
		{
			if (m_pDock->IsDocked())
			{
				CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

				// Indicate if the point is in the close button (except for Win95)
				if ((GetWinVersion() > 1400) && (GetCloseRect().PtInRect(pt)))
					return HTCLOSE;

				MapWindowPoints(NULL, m_hWnd, &pt, 1);

				// Indicate if the point is in the caption
				if (pt.y < 0)
					return HTCAPTION;
			}
		}
		return CWnd::WndProcDefault(WM_NCHITTEST, wParam, lParam);
	}

	inline LRESULT CDocker::CDockClient::OnNCLButtonDown(WPARAM wParam, LPARAM lParam)
	{
		if ((0 != m_pDock) && !(m_pDock->GetDockStyle() & DS_NO_CAPTION))
		{
			if (HTCLOSE == wParam) 
			{
				m_IsClosePressed = TRUE;
				SetCapture();
			}

			m_bCaptionPressed = TRUE;
			m_Oldpt.x = GET_X_LPARAM(lParam);
			m_Oldpt.y = GET_Y_LPARAM(lParam);
			if (m_pDock->IsDocked())
			{
				CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				MapWindowPoints(NULL, m_hWnd, &pt, 1);
				m_pView->SetFocus();

				// Update the close button
				CDC dc = GetWindowDC();
				DrawCloseButton(dc, m_bOldFocus);

				return 0L;
			}
		}
		return CWnd::WndProcDefault(WM_NCLBUTTONDOWN, wParam, lParam);
	}

	inline void CDocker::CDockClient::OnLButtonUp(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		if ((0 != m_pDock) && !(m_pDock->GetDockStyle() & DS_NO_CAPTION))
		{
			m_bCaptionPressed = FALSE;
			if (m_IsClosePressed && GetCloseRect().PtInRect(GetCursorPos()))
			{
				// Destroy the docker
				if (m_pDock->GetView()->IsContainer())
				{
					CContainer* pContainer = ((CContainer*)m_pDock->GetView())->GetActiveContainer();
					CDocker* pDock = m_pDock->GetDockFromView(pContainer);
					pDock->GetDockClient().SetClosePressed();
					m_pDock->UndockContainer(pContainer);
					pDock->Destroy();
				}
				else
				{
					m_pDock->Undock();
					m_pDock->Destroy();
				}
			}
		}
	}

	inline void CDocker::CDockClient::OnLButtonDown(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		m_IsClosePressed = FALSE;
		ReleaseCapture();
		CDC dc = GetWindowDC();
		DrawCloseButton(dc, m_bOldFocus);
	}

	inline void CDocker::CDockClient::OnMouseActivate(WPARAM /*wParam*/, LPARAM /*lParam*/)
	// Focus changed, so redraw the captions
	{
		if ((0 != m_pDock) && !(m_pDock->GetDockStyle() & DS_NO_CAPTION))
		{
			m_pDock->GetDockAncestor()->PostMessage(UWM_DOCK_ACTIVATED, 0, 0);
		}
	}

	inline void CDocker::CDockClient::OnMouseMove(WPARAM wParam, LPARAM lParam)
	{
		OnNCMouseMove(wParam, lParam);
	}

	inline void CDocker::CDockClient::OnNCMouseLeave(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		m_IsTracking = FALSE;
		CDC dc = GetWindowDC();
		if ((0 != m_pDock) && !(m_pDock->GetDockStyle() & DS_NO_CAPTION) && m_pDock->IsDocked())
			DrawCloseButton(dc, m_bOldFocus);

		m_IsTracking = FALSE;
	}

	inline LRESULT CDocker::CDockClient::OnNCMouseMove(WPARAM wParam, LPARAM lParam)
	{
		if (!m_IsTracking)
		{
			TRACKMOUSEEVENT TrackMouseEventStruct = {0};
			TrackMouseEventStruct.cbSize = sizeof(TrackMouseEventStruct);
			TrackMouseEventStruct.dwFlags = TME_LEAVE|TME_NONCLIENT;
			TrackMouseEventStruct.hwndTrack = m_hWnd;
			_TrackMouseEvent(&TrackMouseEventStruct);
			m_IsTracking = TRUE;
		}

		if ((0 != m_pDock) && !(m_pDock->GetDockStyle() & DS_NO_CAPTION))
		{
			if (m_pDock->IsDocked())
			{
				// Discard phantom mouse move messages
				if ( (m_Oldpt.x == GET_X_LPARAM(lParam) ) && (m_Oldpt.y == GET_Y_LPARAM(lParam)))
					return 0L;

				if (IsLeftButtonDown() && (wParam == HTCAPTION)  && (m_bCaptionPressed))
				{
					CDocker* pDock = (CDocker*)FromHandle(GetParent());
					if (pDock)
						pDock->Undock();
				}

				// Update the close button
				CDC dc = GetWindowDC();
				DrawCloseButton(dc, m_bOldFocus);
			}

			m_bCaptionPressed = FALSE;
		}
		return CWnd::WndProcDefault(WM_MOUSEMOVE, wParam, lParam);
	}

	inline LRESULT CDocker::CDockClient::OnNCPaint(WPARAM wParam, LPARAM lParam)
	{
		if ((0 != m_pDock) && !(m_pDock->GetDockStyle() & DS_NO_CAPTION))
		{
			if (m_pDock->IsDocked())
			{
				DefWindowProc(WM_NCPAINT, wParam, lParam);
				DrawCaption(wParam);
				return 0;
			}
		}
		return CWnd::WndProcDefault(WM_NCPAINT, wParam, lParam);
	}

	inline void CDocker::CDockClient::OnWindowPosChanged(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		// Reposition the View window to cover the DockClient's client area
		CRect rc = GetClientRect();
		m_pView->SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
	}

	inline void CDocker::CDockClient::PreRegisterClass(WNDCLASS& wc)
	{
		wc.lpszClassName = _T("Win32++ DockClient");
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	}

	inline void CDocker::CDockClient::PreCreate(CREATESTRUCT& cs)
	{
		DWORD dwStyle = m_pDock->GetDockStyle();
		if ((dwStyle & DS_CLIENTEDGE) || (dwStyle & DS_FLATLOOK))
			cs.dwExStyle = WS_EX_CLIENTEDGE;

#if WINVER >= 0x0500
		if (m_pDock->GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_LAYOUTRTL)
			cs.dwExStyle |= WS_EX_LAYOUTRTL;
#endif
	}

	inline void CDocker::CDockClient::SendNotify(UINT nMessageID)
	{
		// Fill the DragPos structure with data
		DRAGPOS DragPos;
		DragPos.hdr.code = nMessageID;
		DragPos.hdr.hwndFrom = m_hWnd;
		::GetCursorPos(&DragPos.ptPos);

		// Send a DragPos notification to the docker
		SendMessage(GetParent(), WM_NOTIFY, 0L, (LPARAM)&DragPos);
	}

	inline LRESULT CDocker::CDockClient::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_LBUTTONUP:
			{
				ReleaseCapture();
				CDC dc = GetWindowDC();
				DrawCloseButton(dc, m_bOldFocus);
				OnLButtonUp(wParam, lParam);
			}
			break;

		case WM_MOUSEACTIVATE:
			OnMouseActivate(wParam, lParam);
			break;

		case WM_MOUSEMOVE:
			OnMouseMove(wParam, lParam);
			break;

		case WM_NCCALCSIZE:
			OnNCCalcSize(wParam, lParam);
			break;

		case WM_NCHITTEST:
			return OnNCHitTest(wParam, lParam);

		case WM_NCLBUTTONDOWN:
			return OnNCLButtonDown(wParam, lParam);

		case WM_NCMOUSEMOVE:
			return OnNCMouseMove(wParam, lParam);

		case WM_NCPAINT:
			return OnNCPaint(wParam, lParam);

		case WM_NCMOUSELEAVE:
			OnNCMouseLeave(wParam, lParam);
			break;

		case WM_WINDOWPOSCHANGED:
			OnWindowPosChanged(wParam, lParam);
			break;
		}

		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	}


	//////////////////////////////////////////////////////////////
	// Definitions for the CDockHint class nested within CDocker
	//
	inline CDocker::CDockHint::CDockHint() : m_hbmBlueTint(0), m_uDockSideOld(0)
	{
	}

	inline CDocker::CDockHint::~CDockHint()
	{
		if (m_hbmBlueTint) ::DeleteObject(m_hbmBlueTint);
	}

	inline RECT CDocker::CDockHint::CalcHintRectContainer(CDocker* pDockTarget)
	{
		// Calculate the hint window's position for container docking
		CRect rcHint = pDockTarget->GetDockClient().GetWindowRect();
		if (pDockTarget->GetDockClient().GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_CLIENTEDGE)
			rcHint.InflateRect(-2, -2);
		MapWindowPoints(NULL, pDockTarget->GetHwnd(), (LPPOINT)&rcHint, 2);

		return rcHint;
	}

	inline RECT CDocker::CDockHint::CalcHintRectInner(CDocker* pDockTarget, CDocker* pDockDrag, UINT uDockSide)
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
				Width = MAX(rcDockTarget.Width()/2 - pDockDrag->GetBarWidth(), pDockDrag->GetBarWidth());
		}
		else
		{
			Width = rcDockDrag.Height();
			if (Width >= (rcDockTarget.Height() - pDockDrag->GetBarWidth()))
				Width = MAX(rcDockTarget.Height()/2 - pDockDrag->GetBarWidth(), pDockDrag->GetBarWidth());
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

	inline RECT CDocker::CDockHint::CalcHintRectOuter(CDocker* pDockDrag, UINT uDockSide)
	{
		// Calculate the hint window's position for outer docking
		CDocker* pDockTarget = pDockDrag->GetDockAncestor();
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
				Width = MAX(pDockTarget->GetDockClient().GetClientRect().Width()/2 - BarWidth, BarWidth);
		}
		else
		{
			Width = rcDockDrag.Height();
			int BarWidth = pDockDrag->GetBarWidth();
			if (Width >= pDockTarget->GetDockClient().GetClientRect().Height() - pDockDrag->GetBarWidth())
				Width = MAX(pDockTarget->GetDockClient().GetClientRect().Height()/2 - BarWidth, BarWidth);
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

	inline void CDocker::CDockHint::DisplayHint(CDocker* pDockTarget, CDocker* pDockDrag, UINT uDockSide)
	{
		// Ensure a new hint window is created if dock side changes
		if (uDockSide != m_uDockSideOld)
		{
			Destroy();
			pDockTarget->RedrawWindow();
			pDockDrag->RedrawWindow(0, 0, RDW_FRAME|RDW_INVALIDATE);
		}
		m_uDockSideOld = uDockSide;

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

	inline void CDocker::CDockHint::OnPaint(HDC hDC)
	{
		// Display the blue tinted bitmap
		CRect rc = GetClientRect();
		CDC MemDC = CreateCompatibleDC(hDC);
		MemDC.AttachBitmap(m_hbmBlueTint);
		BitBlt(hDC, 0, 0, rc.Width(), rc.Height(), MemDC, 0, 0, SRCCOPY);
		MemDC.DetachBitmap();
	}

	inline void CDocker::CDockHint::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_POPUP;

		// WS_EX_TOOLWINDOW prevents the window being displayed on the taskbar
		cs.dwExStyle = WS_EX_TOOLWINDOW;

		cs.lpszClass = _T("Win32++ DockHint");
	}

	inline void CDocker::CDockHint::SetBitmap(HBITMAP hbm)
	{
		if (m_hbmBlueTint) ::DeleteObject(m_hbmBlueTint);
		m_hbmBlueTint = hbm;
	}

	inline void CDocker::CDockHint::ShowHintWindow(CDocker* pDockTarget, CRect rcHint)
	{
		// Process any queued messages first
		MSG msg;
		while (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		// Save the Dock window's blue tinted bitmap
		CDC dcTarget = pDockTarget->GetDC();
		CDC dcMem = dcTarget.CreateCompatibleDC();
		CRect rcBitmap = rcHint;
		dcMem.CreateCompatibleBitmap(dcTarget, rcBitmap.Width(), rcBitmap.Height());
		dcMem.BitBlt(0, 0, rcBitmap.Width(), rcBitmap.Height(), dcTarget, rcBitmap.left, rcBitmap.top, SRCCOPY);
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
	// Definitions for the CTargetCentre class nested within CDocker
	//
	inline CDocker::CTargetCentre::CTargetCentre() : m_bIsOverContainer(FALSE), m_pOldDockTarget(0)
	{
	}

	inline CDocker::CTargetCentre::~CTargetCentre()
	{
	}

	inline void CDocker::CTargetCentre::OnPaint(HDC hDC)
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

		CDC dcTarget = hDC;
		if (hbmCentre)	dcTarget.DrawBitmap(0, 0, 88, 88, hbmCentre, RGB(255,0,255));
		else TRACE(_T("Missing docking resource: Target Centre\n"));

		if (hbmLeft) dcTarget.DrawBitmap(0, 29, 31, 29, hbmLeft, RGB(255,0,255));
		else TRACE(_T("Missing docking resource: Target Left\n"));

		if (hbmTop) dcTarget.DrawBitmap(29, 0, 29, 31, hbmTop, RGB(255,0,255));
		else TRACE(_T("Missing docking resource: Target Top\n"));

		if (hbmRight) dcTarget.DrawBitmap(55, 29, 31, 29, hbmRight, RGB(255,0,255));
		else TRACE(_T("Missing docking resource: Target Right\n"));

		if (hbmBottom) dcTarget.DrawBitmap(29, 55, 29, 31, hbmBottom, RGB(255,0,255));
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
			dcTarget.DrawBitmap(31, 31, 25, 26, hbmMiddle, RGB(255,0,255));
			::DeleteObject(hbmMiddle);
		}

		dcTarget.DetachDC();
	}

	inline void CDocker::CTargetCentre::OnCreate()
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

	inline BOOL CDocker::CTargetCentre::CheckTarget(LPDRAGPOS pDragPos)
	{
		CDocker* pDockDrag = (CDocker*)FromHandle(pDragPos->hdr.hwndFrom);
		if (NULL == pDockDrag) return FALSE;

		CDocker* pDockTarget = pDockDrag->GetDockFromPoint(pDragPos->ptPos);
		if (NULL == pDockTarget) return FALSE;

		if (!IsWindow())	Create();
		m_bIsOverContainer = pDockTarget->GetView()->IsContainer();

		// Redraw the target if the dock target changes
		if (m_pOldDockTarget != pDockTarget)	Invalidate();
		m_pOldDockTarget = pDockTarget;

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
	// Definitions for the CTarget class nested within CDocker
	// CTarget is the base class for a number of CTargetXXX classes
	inline CDocker::CTarget::~CTarget()
	{
		if (m_hbmImage) ::DeleteObject(m_hbmImage);
	}

	inline void CDocker::CTarget::OnPaint(HDC hDC)
	{
		BITMAP bm;
		GetObject(GetImage(), sizeof(bm), &bm);
		int cxImage = bm.bmWidth;
		int cyImage = bm.bmHeight;
		CDC dcTarget = hDC;

		if (GetImage()) dcTarget.DrawBitmap(0, 0, cxImage, cyImage, GetImage(), RGB(255,0,255));
		else TRACE(_T("Missing docking resource\n"));

		dcTarget.DetachDC();
	}

	inline void CDocker::CTarget::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_POPUP;
		cs.dwExStyle = WS_EX_TOPMOST|WS_EX_TOOLWINDOW;
		cs.lpszClass = _T("Win32++ DockTargeting");
	}

	inline void CDocker::CTarget::SetImage(UINT nID)
	{
		m_hbmImage = (HBITMAP)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(nID),
		IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
	}

	////////////////////////////////////////////////////////////////
	// Definitions for the CTargetLeft class nested within CDocker
	//
	inline BOOL CDocker::CTargetLeft::CheckTarget(LPDRAGPOS pDragPos)
	{
		CDocker* pDockDrag = (CDocker*)FromHandle(pDragPos->hdr.hwndFrom);
		if (NULL == pDockDrag) return FALSE;

		CPoint pt = pDragPos->ptPos;
		CDocker* pDockTarget = pDockDrag->GetDockFromPoint(pt)->GetDockTopLevel();
		if (pDockTarget != pDockDrag->GetDockAncestor())
		{
			Destroy();
			return FALSE;
		}

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
	// Definitions for the CTargetTop class nested within CDocker
	//
	inline BOOL CDocker::CTargetTop::CheckTarget(LPDRAGPOS pDragPos)
	{
		CDocker* pDockDrag = (CDocker*)FromHandle(pDragPos->hdr.hwndFrom);
		if (NULL == pDockDrag) return FALSE;

		CPoint pt = pDragPos->ptPos;
		CDocker* pDockTarget = pDockDrag->GetDockFromPoint(pt)->GetDockTopLevel();
		if (pDockTarget != pDockDrag->GetDockAncestor())
		{
			Destroy();
			return FALSE;
		}

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
	// Definitions for the CTargetRight class nested within CDocker
	//
	inline BOOL CDocker::CTargetRight::CheckTarget(LPDRAGPOS pDragPos)
	{
		CDocker* pDockDrag = (CDocker*)FromHandle(pDragPos->hdr.hwndFrom);
		if (NULL == pDockDrag) return FALSE;

		CPoint pt = pDragPos->ptPos;
		CDocker* pDockTarget = pDockDrag->GetDockFromPoint(pt)->GetDockTopLevel();
		if (pDockTarget != pDockDrag->GetDockAncestor())
		{
			Destroy();
			return FALSE;
		}

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
	// Definitions for the CTargetBottom class nested within CDocker
	//
	inline BOOL CDocker::CTargetBottom::CheckTarget(LPDRAGPOS pDragPos)
	{
		CDocker* pDockDrag = (CDocker*)FromHandle(pDragPos->hdr.hwndFrom);
		if (NULL == pDockDrag) return FALSE;

		CPoint pt = pDragPos->ptPos;
		CDocker* pDockTarget = pDockDrag->GetDockFromPoint(pt)->GetDockTopLevel();
		if (pDockTarget != pDockDrag->GetDockAncestor())
		{
			Destroy();
			return FALSE;
		}

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
	// Definitions for the CDocker class
	//
	inline CDocker::CDocker() : m_pDockParent(NULL), m_BlockMove(FALSE), m_Undocking(FALSE),
		            m_bIsClosing(FALSE), m_bIsDragging(FALSE), m_bDragAutoResize(TRUE), m_DockStartWidth(0), m_nDockID(0),
		            m_nTimerCount(0), m_NCHeight(20), m_dwDockZone(0), m_DockWidthRatio(1.0), m_DockStyle(0), m_hOldFocus(0)
	{
		WORD HashPattern[] = {0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA};
		m_hbmHash = ::CreateBitmap (8, 8, 1, 1, HashPattern);
		m_hbrDithered = ::CreatePatternBrush (m_hbmHash);

		// Assume this docker is the DockAncestor for now.
		m_pDockAncestor = this;
	}

	inline CDocker::~CDocker()
	{
		GetDockBar().Destroy();
		::DeleteObject(m_hbrDithered);
		::DeleteObject(m_hbmHash);

		std::vector <CDocker*>::iterator iter;
		if (GetDockAncestor() == this)
		{
			// Destroy top level dockers with children first
			iter = GetAllDockers().begin();
			while(iter < GetAllDockers().end())
			{
				// top level dockers are always undocked!
				if ((*iter)->IsUndocked() && ((*iter)->GetDockChildren().size() > 0))
				{
					(*iter)->Destroy();
					delete (*iter);
					// A bunch of dockers may have been removed
					iter = GetAllDockers().begin();
				}
				else ++iter;
			}

			// Destroy all remaining related dockers
			// Contents of m_vAllDockers changes in this loop, so use a local copy
			std::vector<CDocker*> vAllDockers = GetAllDockers();
			for (iter = vAllDockers.begin(); iter < vAllDockers.end(); ++iter)
			{
				(*iter)->Destroy();
				delete(*iter);
			}
		}

		if (GetDockAncestor() != this)
		{
			for (iter = GetAllDockers().begin(); iter != GetAllDockers().end(); ++iter)
			{
				// Remove this child entry from the DockAncestor's m_vAllDockers vector
				if ((*iter) == this)
				{
					GetAllDockers().erase(iter);
					break;
				}
			}
		}
	}

	inline CDocker* CDocker::AddDockedChild(CDocker* pDocker, DWORD dwDockStyle, int DockWidth, int nDockID /* = 0*/)
	// This function creates the docker, and adds it to the docker heirachy as docked
	{
		// Create the docker window as a child of the frame window.
		// This pernamently sets the frame window as the docker window's owner,
		// even when its parent is subsequently changed.

		if (pDocker)
		{
			pDocker->SetDockWidth(DockWidth);
			pDocker->SetDockStyle(dwDockStyle);
			pDocker->m_nDockID = nDockID;
			pDocker->m_pDockAncestor = GetDockAncestor();
			pDocker->m_pDockParent = this;
			HWND hwndFrame = GetDockAncestor()->GetAncestor();
			pDocker->Create(hwndFrame);
			pDocker->SetParent(m_hWnd);

			// Store the Docker's pointer in the DockAncestor's vector for later deletion
			GetDockAncestor()->m_vAllDockers.push_back(pDocker);

			// Dock the docker window
			if (dwDockStyle & DS_DOCKED_CONTAINER)
				DockInContainer(pDocker, dwDockStyle);
			else
				Dock(pDocker, dwDockStyle);

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
		}

		return pDocker;
	}

	inline CDocker* CDocker::AddUndockedChild(CDocker* pDocker, DWORD dwDockStyle, int DockWidth, RECT rc, int nDockID /* = 0*/)
	// This function creates the docker, and adds it to the docker heirachy as undocked
	{
		if (pDocker)
		{
			pDocker->SetDockWidth(DockWidth);
			pDocker->SetDockStyle(dwDockStyle & 0XFFFFFF0);
			pDocker->m_nDockID = nDockID;
			pDocker->m_pDockAncestor = GetDockAncestor();

			// Initially create the as a child window of the frame
			// This makes the frame window the owner of our docker
			HWND hwndFrame = GetDockAncestor()->GetAncestor();
			pDocker->Create(hwndFrame);
			pDocker->SetParent(m_hWnd);

			// Change the Docker to a POPUP window
			DWORD dwStyle = WS_POPUP| WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_VISIBLE;
			pDocker->SetWindowLongPtr(GWL_STYLE, dwStyle);
			pDocker->SetRedraw(FALSE);
			pDocker->SetParent(0);
			pDocker->SetWindowPos(HWND_TOP, rc, SWP_SHOWWINDOW|SWP_FRAMECHANGED);
			pDocker->SetRedraw(TRUE);
			pDocker->RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
			pDocker->SetWindowText(pDocker->GetCaption().c_str());

			// Store the Docker's pointer in the DockAncestor's vector for later deletion
			GetDockAncestor()->m_vAllDockers.push_back(pDocker);
		}
		return pDocker;
	}

	inline void CDocker::CheckAllTargets(LPDRAGPOS pDragPos)
	{
		if (!GetDockAncestor()->m_TargetCentre.CheckTarget(pDragPos))
		{
			if (!GetDockAncestor()->m_TargetLeft.CheckTarget(pDragPos))
			{
				if(!GetDockAncestor()->m_TargetTop.CheckTarget(pDragPos))
				{
					if(!GetDockAncestor()->m_TargetRight.CheckTarget(pDragPos))
					{
						if(!GetDockAncestor()->m_TargetBottom.CheckTarget(pDragPos))
						{
							// Not in a docking zone, so clean up
							NMHDR nmhdr = pDragPos->hdr;
							CDocker* pDockDrag = (CDocker*)FromHandle(nmhdr.hwndFrom);
							if (pDockDrag)
							{
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
	}

	inline BOOL CDocker::VerifyDockers()
	// A diagnostic routine which verifies the integrity of the docking layout
	{
		BOOL bResult = TRUE;

		// Check dock ancestor
		std::vector<CDocker*>::iterator iter;

		for (iter = GetAllDockers().begin(); iter != GetAllDockers().end(); ++iter)
		{
			if (GetDockAncestor() != (*iter)->m_pDockAncestor)
			{
				TRACE(_T("Invalid Dock Ancestor\n"));
				bResult = FALSE;
			}
		}

		// Check presence of dock parent
		for (iter = GetAllDockers().begin(); iter != GetAllDockers().end(); ++iter)
		{
			if ((*iter)->IsUndocked() && (*iter)->m_pDockParent != 0)
			{
				TRACE(_T("Error: Undocked dockers should not have a dock parent\n"));
					bResult = FALSE;
			}

			if ((*iter)->IsDocked() && (*iter)->m_pDockParent == 0)
			{
				TRACE(_T("Error: Docked dockers should have a dock parent\n"));
					bResult = FALSE;
			}
		}

		// Check dock parent/child relationship
		for (iter = GetAllDockers().begin(); iter != GetAllDockers().end(); ++iter)
		{
			std::vector<CDocker*>::iterator iterChild;
			for (iterChild = (*iter)->GetDockChildren().begin(); iterChild != (*iter)->GetDockChildren().end(); ++iterChild)
			{
				if ((*iterChild)->m_pDockParent != (*iter))
				{
					TRACE(_T("Error: Docking parent/Child information mismatch\n"));
					bResult = FALSE;
				}
				if ((*iterChild)->GetParent() != (*iter)->GetHwnd())
				{
					TRACE(_T("Error: Incorrect windows child parent relationship\n"));
					bResult = FALSE;
				}
			}
		}

		// Check dock parent chain
		for (iter = GetAllDockers().begin(); iter != GetAllDockers().end(); ++iter)
		{
			CDocker* pDockTopLevel = (*iter)->GetDockTopLevel();
			if (pDockTopLevel->IsDocked())
				TRACE(_T("Error: Top level parent should be undocked\n"));
		}

		return bResult;
	}

	inline void CDocker::CloseAllDockers()
	{
		if (this != GetDockAncestor())
			throw CWinException(_T("Must call CloseAllDockers from the DockAncestor"));

		std::vector <CDocker*>::iterator v;

		SetRedraw(FALSE);
		std::vector<CDocker*> AllDockers = GetAllDockers();
		for (v = AllDockers.begin(); v != AllDockers.end(); ++v)
		{
			// The CDocker is destroyed when the window is destroyed
			(*v)->m_bIsClosing = TRUE;
			(*v)->Destroy();	// Destroy the window
		}

		GetDockChildren().clear();
		SetRedraw(TRUE);
		RecalcDockLayout();
	}

	inline void CDocker::CloseAllTargets()
	{
		GetDockAncestor()->m_TargetCentre.Destroy();
		GetDockAncestor()->m_TargetLeft.Destroy();
		GetDockAncestor()->m_TargetTop.Destroy();
		GetDockAncestor()->m_TargetRight.Destroy();
		GetDockAncestor()->m_TargetBottom.Destroy();
	}

	inline CDocker* CDocker::NewDockerFromID(int /*nID*/)
	{
		// Override this function to create the Docker objects as shown below

		CDocker* pDock = NULL;
	/*	switch(nID)
		{
		case ID_CLASSES:
			pDock = new CDockClasses;
			break;
		case ID_FILES:
			pDock = new CDockFiles;
			break;
		default:
			TRACE(_T("Unknown Dock ID\n"));
			break;
		} */

		return pDock;
	}

	inline void CDocker::Dock(CDocker* pDocker, UINT DockStyle)
	{
		pDocker->m_pDockParent = this;
		pDocker->m_BlockMove = FALSE;
		pDocker->SetDockStyle(DockStyle);
		m_vDockChildren.push_back(pDocker);
		pDocker->ConvertToChild(m_hWnd);

		// Limit the docked size to half the parent's size if it won't fit inside parent
		if (((DockStyle & 0xF)  == DS_DOCKED_LEFT) || ((DockStyle &0xF)  == DS_DOCKED_RIGHT))
		{
			int Width = GetDockClient().GetWindowRect().Width();
			int BarWidth = pDocker->GetBarWidth();
			if (pDocker->m_DockStartWidth >= (Width - BarWidth))
				pDocker->SetDockWidth(MAX(Width/2 - BarWidth, BarWidth));

			pDocker->m_DockWidthRatio = ((double)pDocker->m_DockStartWidth) / (double)GetWindowRect().Width();
		}
		else
		{
			int Height = GetDockClient().GetWindowRect().Height();
			int BarWidth = pDocker->GetBarWidth();
			if (pDocker->m_DockStartWidth >= (Height - BarWidth))
				pDocker->SetDockWidth(MAX(Height/2 - BarWidth, BarWidth));

			pDocker->m_DockWidthRatio = ((double)pDocker->m_DockStartWidth) / (double)GetWindowRect().Height();
		}

		// Redraw the docked windows
		::SetForegroundWindow(GetAncestor());
		GetDockTopLevel()->m_hOldFocus = pDocker->GetView()->GetHwnd();
		pDocker->GetView()->SetFocus();
		RecalcDockLayout();
	}

	inline void CDocker::DockInContainer(CDocker* pDock, DWORD dwDockStyle)
	{
		// Add a container to an existing container
		if ((dwDockStyle & DS_DOCKED_CONTAINER) && (pDock->GetView()->IsContainer()))
		{
			// Transfer any dock children to this docker
			pDock->MoveDockChildren(this);

			// Transfer container children to the target container
			CContainer* pContainer = (CContainer*)GetView();
			CContainer* pContainerSource = (CContainer*)pDock->GetView();

			if (pContainerSource->GetAllContainers().size() > 1)
			{
				// The container we're about to add has children, so transfer those first
				std::vector<ContainerInfo>::reverse_iterator riter;
				std::vector<ContainerInfo> AllContainers = pContainerSource->GetAllContainers();
				for ( riter = AllContainers.rbegin() ; riter < AllContainers.rend() -1; ++riter )
				{
					// Remove child container from pContainerSource
					CContainer* pContainerChild = (*riter).pContainer;
					pContainerChild->ShowWindow(SW_HIDE);
					pContainerSource->RemoveContainer(pContainerChild);

					// Add child container to this container
					pContainer->AddContainer(pContainerChild);

					CDocker* pDockChild = GetDockFromView(pContainerChild);
					pDockChild->SetParent(m_hWnd);
					pDockChild->m_pDockParent = this;
				}
			}

			pContainer->AddContainer((CContainer*)pDock->GetView());
			pDock->m_pDockParent = this;
			pDock->m_BlockMove = FALSE;
			pDock->ShowWindow(SW_HIDE);
			pDock->SetWindowLongPtr(GWL_STYLE, WS_CHILD);
			pDock->SetDockStyle(dwDockStyle);
			pDock->SetParent(m_hWnd);
		}
	}

	inline void CDocker::DockOuter(CDocker* pDocker, DWORD dwDockStyle)
	{
		pDocker->m_pDockParent = GetDockAncestor();

		DWORD OuterDocking = dwDockStyle & 0xF0000;
		DWORD DockSide = OuterDocking / 0x10000;
		dwDockStyle &= 0xFFF0FFFF;
		dwDockStyle |= DockSide;

		// Set the dock styles
		DWORD dwStyle = WS_CHILD | WS_VISIBLE;
		pDocker->m_BlockMove = FALSE;
		pDocker->SetWindowLongPtr(GWL_STYLE, dwStyle);
		pDocker->ShowWindow(SW_HIDE);
		pDocker->SetDockStyle(dwDockStyle);

		// Set the docking relationships
		std::vector<CDocker*>::iterator iter = GetDockAncestor()->m_vDockChildren.begin();
		GetDockAncestor()->m_vDockChildren.insert(iter, pDocker);
		pDocker->SetParent(GetDockAncestor()->GetHwnd());
		pDocker->GetDockBar().SetParent(GetDockAncestor()->GetHwnd());

		// Limit the docked size to half the parent's size if it won't fit inside parent
		if (((dwDockStyle & 0xF)  == DS_DOCKED_LEFT) || ((dwDockStyle &0xF)  == DS_DOCKED_RIGHT))
		{
			int Width = GetDockAncestor()->GetDockClient().GetWindowRect().Width();
			int BarWidth = pDocker->GetBarWidth();
			if (pDocker->m_DockStartWidth >= (Width - BarWidth))
				pDocker->SetDockWidth(MAX(Width/2 - BarWidth, BarWidth));

			pDocker->m_DockWidthRatio = ((double)pDocker->m_DockStartWidth) / (double)GetDockAncestor()->GetWindowRect().Width();
		}
		else
		{
			int Height = GetDockAncestor()->GetDockClient().GetWindowRect().Height();
			int BarWidth = pDocker->GetBarWidth();
			if (pDocker->m_DockStartWidth >= (Height - BarWidth))
				pDocker->SetDockWidth(MAX(Height/2 - BarWidth, BarWidth));

			pDocker->m_DockWidthRatio = ((double)pDocker->m_DockStartWidth) / (double)GetDockAncestor()->GetWindowRect().Height();
		}

		// Redraw the docked windows
		::SetFocus(GetAncestor());
		pDocker->GetView()->SetFocus();
		RecalcDockLayout();
	}

	inline void CDocker::DrawAllCaptions()
	{
		std::vector<CDocker*>::iterator iter;
		for (iter = GetAllDockers().begin(); iter != GetAllDockers().end(); iter++)
		{
			if ((*iter)->IsDocked())
				(*iter)->GetDockClient().DrawCaption((WPARAM)1);
		}
	}

	inline void CDocker::DrawHashBar(HWND hBar, POINT Pos)
	{
		// draws a hashed bar while the splitter bar is being dragged
		{
			CDocker* pDock = ((CDockBar*)FromHandle(hBar))->GetDock();
			if (NULL == pDock) return;

			BOOL bVertical = ((pDock->GetDockStyle() & 0xF) == DS_DOCKED_LEFT) || ((pDock->GetDockStyle() & 0xF) == DS_DOCKED_RIGHT);

			CDC BarDC = GetDC();
			BarDC.AttachBrush(m_hbrDithered);

			CRect rc;
			::GetWindowRect(hBar, &rc);
			MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rc, 2);
			int cx = rc.Width();
			int cy = rc.Height();
			int BarWidth = pDock->GetDockBar().GetWidth();

			if (bVertical)
				BarDC.PatBlt(Pos.x - BarWidth/2, rc.top, BarWidth, cy, PATINVERT);
			else
				BarDC.PatBlt(rc.left, Pos.y - BarWidth/2, cx, BarWidth, PATINVERT);

			BarDC.DetachBrush();
		}
	}

	inline CContainer* CDocker::GetContainer() const
	{
		CContainer* pContainer = NULL;
		if (GetView() && GetView()->IsContainer())
			pContainer = (CContainer*)GetView();

		return pContainer;
	}

	inline CDocker* CDocker::GetDockFromPoint(POINT pt) const
	// Retrieves the Docker whose view window contains the specified point
	{
		// Step 1: Find the top level Docker the point is over
		CDocker* pDockTop = NULL;

		// Iterate through all top level windows, checking for undocked docker
		HWND hWnd = GetWindow(GW_HWNDFIRST);
		while(hWnd)
		{
			if (IsRelated(hWnd))
			{
				CDocker* pDockTest = (CDocker*)FromHandle(hWnd);
				CRect rc = pDockTest->GetClientRect();
				MapWindowPoints(pDockTest->GetHwnd(), NULL, (LPPOINT)&rc, 2);
				if ((this != pDockTest) && PtInRect(&rc, pt))
				{
					pDockTop = pDockTest;
					break;
				}
			}

			hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
		}

		if (!pDockTop)
		{
			// Point not over an undocked docker. Perhaps over the DockAncestor?
			CRect rc = GetDockAncestor()->GetWindowRect();
			if (PtInRect(&rc, pt)) pDockTop = GetDockAncestor();
		}

		// Step 2: Find the docker child whose view window has the point
		CDocker* pDockTarget = NULL;
		if (pDockTop)
		{
			CDocker* pDockTest = pDockTop;
			HWND hWndTest = pDockTest->GetHwnd();

			while (IsRelated(hWndTest))
			{
				pDockTest = (CDocker*)FromHandle(hWndTest);
				CPoint ptLocal = pt;
				MapWindowPoints(NULL, hWndTest, &ptLocal, 1);
				HWND hTestNew = ChildWindowFromPoint(hWndTest, ptLocal);
				if (hTestNew == hWndTest) break;
				hWndTest = hTestNew;
			}

			if (pDockTest)
			{
				CRect rc = pDockTest->GetDockClient().GetWindowRect();
				if (PtInRect(&rc, pt)) pDockTarget = pDockTest;
			}
		}

		return pDockTarget;
	}

	inline CDocker* CDocker::GetDockAncestor() const
	// The GetDockAncestor function retrieves the pointer to the
	//  ancestor (root docker parent) of the Docker.
	{
		return m_pDockAncestor;
	}

	inline CDocker* CDocker::GetDockFromID(int n_DockID) const
	{
		std::vector <CDocker*>::iterator v;

		if (GetDockAncestor())
		{
			for (v = GetDockAncestor()->m_vAllDockers.begin(); v != GetDockAncestor()->m_vAllDockers.end(); v++)
			{
				if (n_DockID == (*v)->GetDockID())
					return *v;
			}
		}

		return 0;
	}

	inline CDocker* CDocker::GetDockFromView(CWnd* pView) const
	{
		CDocker* pDock = 0;
		std::vector<CDocker*>::iterator iter;
		std::vector<CDocker*> AllDockers = GetAllDockers();
		for (iter = AllDockers.begin(); iter != AllDockers.end(); ++iter)
		{
			if ((*iter)->GetView() == pView)
				pDock = (*iter);
		}

		return pDock;
	}

	inline CDocker* CDocker::GetDockTopLevel() const
	{
		CDocker* pDockTopLevel = (CDocker* const)this;

		while(pDockTopLevel->GetDockParent())
		{
			pDockTopLevel = pDockTopLevel->GetDockParent();
		}

		return pDockTopLevel;
	}

	inline int CDocker::GetDockWidth() const
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

	inline CTabbedMDI* CDocker::GetTabbedMDI() const
	{
		CTabbedMDI* pTabbedMDI = NULL;
		if (GetView() && GetView()->IsTabbedMDI())
			pTabbedMDI = (CTabbedMDI*)GetView();

		return pTabbedMDI;
	}

	inline BOOL CDocker::IsChildOfDocker(HWND hwnd) const
	// returns true if the specified window is a child of this docker
	{
		while ((hwnd != NULL) && (hwnd != GetDockAncestor()->GetHwnd()))
		{
			if (hwnd == m_hWnd) return TRUE;
			if (IsRelated(hwnd)) break;
			hwnd = ::GetParent(hwnd);
		}

		return FALSE;
	}

	inline BOOL CDocker::IsDocked() const
	{
		return (((m_DockStyle&0xF) || (m_DockStyle & DS_DOCKED_CONTAINER)) && !m_Undocking); // Boolean expression
	}

	inline BOOL CDocker::IsDragAutoResize()
	{
		return m_bDragAutoResize;
	}

	inline BOOL CDocker::IsRelated(HWND hWnd) const
	// Returns TRUE if the hWnd is a docker within this dock family
	{
		if (GetDockAncestor()->GetHwnd() == hWnd) return TRUE;

		std::vector<CDocker*>::iterator iter;
		for (iter = GetAllDockers().begin(); iter < GetAllDockers().end(); ++iter)
		{
			if ((*iter)->GetHwnd() == hWnd) return TRUE;
		}

		return FALSE;
	}

	inline BOOL CDocker::IsUndocked() const
	{
		return (!((m_DockStyle&0xF)|| (m_DockStyle & DS_DOCKED_CONTAINER)) && !m_Undocking); // Boolean expression
	}

	inline BOOL CDocker::LoadRegistrySettings(tString tsRegistryKeyName)
	{
		BOOL bResult = FALSE;

		if (0 != tsRegistryKeyName.size())
		{
			std::vector<DockInfo> vDockList;
			tString tsKey = _T("Software\\") + tsRegistryKeyName + _T("\\Dock Windows");
			HKEY hKey = 0;
			RegOpenKeyEx(HKEY_CURRENT_USER, tsKey.c_str(), 0, KEY_READ, &hKey);
			if (hKey)
			{
				DWORD dwType = REG_BINARY;
				DWORD BufferSize = sizeof(DockInfo);
				DockInfo di;
				int i = 0;
				TCHAR szNumber[16];
				tString tsSubKey = _T("DockChild");
				tsSubKey += _itot(i, szNumber, 10);

				// Fill the DockList vector from the registry
				while (0 == RegQueryValueEx(hKey, tsSubKey.c_str(), NULL, &dwType, (LPBYTE)&di, &BufferSize))
				{
					vDockList.push_back(di);
					i++;
					tsSubKey = _T("DockChild");
					tsSubKey += _itot(i, szNumber, 10);
				}

				RegCloseKey(hKey);
				if (vDockList.size() > 0) bResult = TRUE;
			}

			// Add dockers without parents first
			std::vector<DockInfo>::iterator iter;
			for (iter = vDockList.begin(); iter < vDockList.end() ; ++iter)
			{
				DockInfo di = (*iter);
				if (di.DockParentID == 0)
				{
					CDocker* pDocker = NewDockerFromID(di.DockID);
					if (pDocker)
					{
						if (di.DockStyle & 0xF)
							AddDockedChild(pDocker, di.DockStyle, di.DockWidth, di.DockID);
						else
							AddUndockedChild(pDocker, di.DockStyle, di.DockWidth, di.Rect, di.DockID);
					}
					else
					{
						TRACE(_T("Failed to add dockers without parents from registry"));
						bResult = FALSE;
					}
				}
			}

			// Remove dockers without parents from vDockList
			for (size_t n = vDockList.size(); n > 0; --n)
			{
				iter = vDockList.begin() + n-1;
				if ((*iter).DockParentID == 0)
					vDockList.erase(iter);
			}  

			// Add remaining dockers
			while (vDockList.size() > 0)
			{
				bool bFound = false;
				std::vector<DockInfo>::iterator iter;
				for (iter = vDockList.begin(); iter < vDockList.end(); ++iter)
				{
					DockInfo di = *iter;
					CDocker* pDockParent = GetDockFromID(di.DockParentID);
					
					if (pDockParent != 0)
					{
						CDocker* pDock = NewDockerFromID(di.DockID);
						if(pDock)
						{
							pDockParent->AddDockedChild(pDock, di.DockStyle, di.DockWidth, di.DockID);
							bFound = true;
							vDockList.erase(iter);
						}
						else
						{
							TRACE(_T("Failed to add dockers with parents from registry"));
							bResult = FALSE;
						}

						break;
					}
				}

				if (!bFound)
				{
					TRACE(_T("Orphaned dockers stored in registry "));
					bResult = FALSE;
				}
			}
		}

		if (!bResult) CloseAllDockers();
		return bResult;
	}

	inline void CDocker::MoveDockChildren(CDocker* pDockTarget)
	{
		// Transfer any dock children from the current docker to the target docker
		std::vector<CDocker*>::iterator iter;
		for (iter = GetDockChildren().begin(); iter < GetDockChildren().end(); ++iter)
		{
			pDockTarget->GetDockChildren().push_back(*iter);
			(*iter)->m_pDockParent = pDockTarget;
			(*iter)->SetParent(pDockTarget->GetHwnd());
			(*iter)->GetDockBar().SetParent(pDockTarget->GetHwnd());
		}
		GetDockChildren().clear();
	}

	inline void CDocker::OnActivate(WPARAM wParam, LPARAM /*lParam*/)
	{
		// Only top level undocked dockers get this message
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			GetDockTopLevel()->m_hOldFocus = GetFocus();

			// Send a notification of focus lost
			int idCtrl = ::GetDlgCtrlID(m_hOldFocus);
			NMHDR nhdr={0};
			nhdr.hwndFrom = m_hOldFocus;
			nhdr.idFrom = idCtrl;
			nhdr.code = UWM_FRAMELOSTFOCUS;
			SendMessage(WM_NOTIFY, (WPARAM)idCtrl, (LPARAM)&nhdr);
		}
	}

	inline void CDocker::OnCaptionTimer(WPARAM wParam, LPARAM /*lParam*/)
	{
		if (this == GetDockAncestor())
		{
			if (wParam == 1)
			{
				DrawAllCaptions();
				m_nTimerCount++;
				if (m_nTimerCount == 10)
				{
					KillTimer(m_hWnd, wParam);
					m_nTimerCount = 0;
				}
			}
		}
	}

	inline void CDocker::OnCreate()
	{
		// Create the various child windows
		GetDockClient().SetDock(this);
		GetDockClient().Create(m_hWnd);
		if (NULL == GetView())
			throw CWinException(_T("CDocker::OnCreate... View window is not assigned!\nUse SetView to set the View Window"));
		GetView()->Create(GetDockClient().GetHwnd());

		// Create the slider bar belonging to this docker
		GetDockBar().SetDock(this);
		if (GetDockAncestor() != this)
			GetDockBar().Create(GetParent());

		// Now remove the WS_POPUP style. It was required to allow this window
		// to be owned by the frame window.
		HWND hwndParent = GetParent();
		SetWindowLongPtr(GWL_STYLE, WS_CHILD);
		SetParent(hwndParent);	// Reinstate the window's parent

		// Set the default colour for the splitter bar
		CFrame* pFrame = 0;
		pFrame = (CFrame*)FromHandle(GetDockAncestor()->GetAncestor());
		COLORREF rgbColour = GetSysColor(COLOR_BTNFACE);
		if (pFrame && pFrame->IsFrame())
		{
			CRebar& RB = pFrame->GetRebar();
			if (RB.GetRebarTheme().UseThemes)
				rgbColour = RB.GetRebarTheme().clrBkgnd2;
		}

		SetBarColor(rgbColour);
	}

	inline void CDocker::OnDestroy(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		// Destroy any dock children first
		std::vector<CDocker*>::iterator iter;
		for (iter = GetDockChildren().begin(); iter < GetDockChildren().end(); ++iter)
		{
			(*iter)->Destroy();
		}

		if (GetView()->IsContainer() && IsUndocked())
		{
			CContainer* pContainer = (CContainer*)GetView();
			if (pContainer->GetAllContainers().size() > 1)
			{
				// This container has children, so destroy them now
				std::vector<ContainerInfo> AllContainers = pContainer->GetAllContainers();
				std::vector<ContainerInfo>::iterator iter;
				for (iter = AllContainers.begin(); iter < AllContainers.end(); ++iter)
				{
					if ((*iter).pContainer != pContainer)
					{
						// Reset container parent before destroying the dock window
						CDocker* pDock = GetDockFromView((*iter).pContainer);
						pContainer->SetParent(pDock->GetDockClient().GetHwnd());

						pDock->Destroy();
					}
				}
			}
		}

		GetDockBar().Destroy();

		// Post a destroy docker message
		GetDockAncestor()->PostMessage(UWM_DOCK_DESTROYED, (WPARAM)this, 0L);
	}

	inline void CDocker::OnDockDestroyed(WPARAM wParam, LPARAM /*lParam*/)
	{
		CDocker* pDock = (CDocker*)wParam;
		if (this == GetDockAncestor() && pDock != GetDockAncestor())
		{
			delete pDock;
		}
	}

	inline void CDocker::OnExitSizeMove(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		m_BlockMove = FALSE;
		m_bIsDragging = FALSE;
		SendNotify(UWM_DOCK_END);
	}

	inline LRESULT CDocker::OnNotify(WPARAM /*wParam*/, LPARAM lParam)
	{
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
				CDocker* pDock = (CDocker*)FromHandle(pdp->hdr.hwndFrom);
				if (NULL == pDock) break;

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
				if (!IsDragAutoResize())
					DrawHashBar(pdp->hdr.hwndFrom, pt);
				m_OldPoint = pt;
			}
			break;

		case UWM_BAR_MOVE:
			{
				CPoint pt = pdp->ptPos;
				MapWindowPoints(NULL, m_hWnd, &pt, 1);

				if (pt != m_OldPoint)
				{
					if (IsDragAutoResize())
						ResizeDockers(pdp);
					else
					{
						DrawHashBar(pdp->hdr.hwndFrom, m_OldPoint);
						DrawHashBar(pdp->hdr.hwndFrom, pt);
					}

					m_OldPoint = pt;
				}
			}
			break;

		case UWM_BAR_END:
			{
				POINT pt = pdp->ptPos;
				MapWindowPoints(NULL, m_hWnd, &pt, 1);

				if (!IsDragAutoResize())
					DrawHashBar(pdp->hdr.hwndFrom, pt);

				ResizeDockers(pdp);
			}
			break;
		case NM_SETFOCUS:
			GetDockAncestor()->PostMessage(UWM_DOCK_ACTIVATED, 0, 0);
			break;
		case UWM_FRAMEGOTFOCUS:
			GetDockAncestor()->PostMessage(UWM_DOCK_ACTIVATED, 0, 0);
			break;
		case UWM_FRAMELOSTFOCUS:
			GetDockAncestor()->PostMessage(UWM_DOCK_ACTIVATED, 0, 0);
			break;
		}
		return 0L;
	}

	inline void CDocker::ResizeDockers(LPDRAGPOS pdp)
	{
		POINT pt = pdp->ptPos;
		MapWindowPoints(NULL, m_hWnd, &pt, 1);

		CDocker* pDock = ((CDockBar*)FromHandle(pdp->hdr.hwndFrom))->GetDock();
		if (NULL == pDock) return;

		RECT rcDock = pDock->GetWindowRect();
		MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcDock, 2);

		double dBarWidth = pDock->GetDockBar().GetWidth();
		int iBarWidth    = pDock->GetDockBar().GetWidth();
		int DockWidth;

		switch (pDock->GetDockStyle() & 0xF)
		{
		case DS_DOCKED_LEFT:
			DockWidth = MAX(pt.x, iBarWidth/2) - rcDock.left - (int)(.5* dBarWidth);
			DockWidth = MAX(-iBarWidth, DockWidth);
			pDock->SetDockWidth(DockWidth);
			pDock->m_DockWidthRatio = ((double)pDock->m_DockStartWidth)/((double)pDock->m_pDockParent->GetWindowRect().Width());
			break;
		case DS_DOCKED_RIGHT:
			DockWidth = rcDock.right - MAX(pt.x, iBarWidth/2) - (int)(.5* dBarWidth);
			DockWidth = MAX(-iBarWidth, DockWidth);
			pDock->SetDockWidth(DockWidth);
			pDock->m_DockWidthRatio = ((double)pDock->m_DockStartWidth)/((double)pDock->m_pDockParent->GetWindowRect().Width());
			break;
		case DS_DOCKED_TOP:
			DockWidth = MAX(pt.y, iBarWidth/2) - rcDock.top - (int)(.5* dBarWidth);
			DockWidth = MAX(-iBarWidth, DockWidth);
			pDock->SetDockWidth(DockWidth);
			pDock->m_DockWidthRatio = ((double)pDock->m_DockStartWidth)/((double)pDock->m_pDockParent->GetWindowRect().Height());
			break;
		case DS_DOCKED_BOTTOM:
			DockWidth = rcDock.bottom - MAX(pt.y, iBarWidth/2) - (int)(.5* dBarWidth);
			DockWidth = MAX(-iBarWidth, DockWidth);
			pDock->SetDockWidth(DockWidth);
			pDock->m_DockWidthRatio = ((double)pDock->m_DockStartWidth)/((double)pDock->m_pDockParent->GetWindowRect().Height());
			break;
		}

		RecalcDockLayout();
	}

	inline void CDocker::OnSetFocus(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		if (IsUndocked() && m_hOldFocus)
			::SetFocus(m_hOldFocus);
		else
			// Pass focus on the the view window
			GetView()->SetFocus();

		if ((this == GetDockTopLevel()) && (this != GetDockAncestor()))
		{
			// Send a notification to top level window
			int idCtrl = ::GetDlgCtrlID(m_hOldFocus);
			NMHDR nhdr={0};
			nhdr.hwndFrom = m_hOldFocus;
			nhdr.idFrom = idCtrl;
			nhdr.code = NM_SETFOCUS;
			SendMessage(WM_NOTIFY, (WPARAM)idCtrl, (LPARAM)&nhdr);
		}
	}

	inline void CDocker::OnSysColorChange(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		if (this == GetDockAncestor())
		{
			CFrame* pFrame = 0;
			pFrame = (CFrame*)FromHandle(GetAncestor());
			COLORREF rgbColour = GetSysColor(COLOR_BTNFACE);
			if (pFrame && pFrame->IsFrame())
			{
				CRebar& RB = pFrame->GetRebar();
				if (RB.GetRebarTheme().UseThemes)
					rgbColour = RB.GetRebarTheme().clrBkgnd2;
			}

			// Set the splitter bar colour for each docker decendant
			std::vector<CDocker*>::iterator iter;
			for (iter = GetAllDockers().begin(); iter < GetAllDockers().end(); ++iter)
				(*iter)->SetBarColor(rgbColour);

			// Set the splitter bar colour for the docker ancestor
			SetBarColor(rgbColour);
		}
	}

	inline LRESULT CDocker::OnSysCommand(WPARAM wParam, LPARAM lParam)
	{
		switch(wParam&0xFFF0)
		{
		case SC_MOVE:
			// An undocked docker is being moved
			{
				BOOL bResult = FALSE;
				m_bIsDragging = TRUE;
				SetCursor(LoadCursor(NULL, IDC_ARROW));

				if (SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, &bResult, 0))
				{
					// Turn on DragFullWindows for this move
					SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, TRUE, 0, 0);

					// Process this message
					DefWindowProc(WM_SYSCOMMAND, wParam, lParam);

					// Return DragFullWindows to its previous state
					SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, bResult, 0, 0);
					return 0L;
				}
			}
			break;
		case SC_CLOSE:
			// The close button is pressed on an undocked docker
			m_bIsClosing = TRUE;
			break;
		}
		return CWnd::WndProcDefault(WM_SYSCOMMAND, wParam, lParam);
	}

	inline LRESULT CDocker::OnWindowPosChanging(WPARAM wParam, LPARAM lParam)
	{
		// Suspend dock drag moving while over dock zone
		if (m_BlockMove)
		{
        	LPWINDOWPOS pWndPos = (LPWINDOWPOS)lParam;
			pWndPos->flags |= SWP_NOMOVE|SWP_FRAMECHANGED;
			return 0;
		}

		return CWnd::WndProcDefault(WM_WINDOWPOSCHANGING, wParam, lParam);
	}

	inline void CDocker::OnWindowPosChanged(WPARAM /*wParam*/, LPARAM lParam)
	{
		if (m_bIsDragging)
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
		}
		else if (this == GetDockTopLevel())
		{
			// Reposition the dock children
			if (IsUndocked() && !m_bIsClosing) RecalcDockLayout();
		}
	}

	inline void CDocker::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_POPUP | WS_CHILD;
		cs.dwExStyle = WS_EX_TOOLWINDOW;
	}

	inline void CDocker::PreRegisterClass(WNDCLASS &wc)
	{
		wc.lpszClassName = _T("Win32++ Docker");
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	}

	inline void CDocker::RecalcDockChildLayout(CRect rc)
	{
		// This function positions the Docker window, along with the Docker's
		// dock children, and draws the splitter bars.
		// Note: This function is used recursively.

		// Step 1: Set this Docker's position	(unless it's a top level docker).
		// Note: All top level dockers are undocked, including the dock ancestor.
		if (IsDocked())
		{
			SetWindowPos(NULL, rc, SWP_SHOWWINDOW|SWP_FRAMECHANGED);
			MapWindowPoints(GetDockParent()->GetHwnd(), GetHwnd(), (LPPOINT)&rc, 2);
		}

		// Step 2: Set the position of each Docker child
		for (UINT u = 0; u < m_vDockChildren.size(); ++u)
		{
			CRect rcChild = rc;
			double DockWidth;

			// Calculate the size of the Docker child
			switch (m_vDockChildren[u]->GetDockStyle() & 0xF)
			{
			case DS_DOCKED_LEFT:
				DockWidth = MIN(m_vDockChildren[u]->m_DockWidthRatio*(GetWindowRect().Width()), rcChild.Width());
				rcChild.right = rcChild.left + (int)DockWidth;
				break;
			case DS_DOCKED_RIGHT:
				DockWidth = MIN(m_vDockChildren[u]->m_DockWidthRatio*(GetWindowRect().Width()), rcChild.Width());
				rcChild.left = rcChild.right - (int)DockWidth;
				break;
			case DS_DOCKED_TOP:
				DockWidth = MIN(m_vDockChildren[u]->m_DockWidthRatio*(GetWindowRect().Height()), rcChild.Height());
				rcChild.bottom = rcChild.top + (int)DockWidth;
				break;
			case DS_DOCKED_BOTTOM:
				DockWidth = MIN(m_vDockChildren[u]->m_DockWidthRatio*(GetWindowRect().Height()), rcChild.Height());
				rcChild.top = rcChild.bottom - (int)DockWidth;
				break;
			}

			if (m_vDockChildren[u]->IsDocked())
			{
				// Position the Docker child recursively (as it might also have Docker children)
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

		// Step 3: Set the client window position for this Docker
		if (IsDocked())
		{
			// This docker's client window occupies the area of the Docker window
			// that remains after the after the Docker children and splitter bars have
			// been taken into account.
			GetDockClient().SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW|SWP_FRAMECHANGED);
		}
		else
		{
			// The TopLevelDocker is always 'undocked'.
			GetDockClient().SetWindowPos(NULL, rc, SWP_SHOWWINDOW|SWP_FRAMECHANGED);
		}
	}

	inline void CDocker::RecalcDockLayout()
	{
		if (GetDockAncestor()->IsWindow())
		{
			CRect rc = GetDockTopLevel()->GetClientRect();
			GetDockTopLevel()->SetRedraw(FALSE);
			GetDockTopLevel()->RecalcDockChildLayout(rc);
			GetDockTopLevel()->SetRedraw(TRUE);
			GetDockTopLevel()->RedrawWindow();
		}
	}

	inline void CDocker::SaveRegistrySettings(tString tsRegistryKeyName)
	{
		VerifyDockers();
		// NOTE: This function assumes that each docker has a unique DockID

		std::vector<CDocker*>::iterator iter;
		std::vector<DockInfo> vDockList;

		if (0 != tsRegistryKeyName.size())
		{
			// Fill the DockList vector with the docking information
			for (iter = GetAllDockers().begin(); iter <  GetAllDockers().end(); ++iter)
			{
				DockInfo di	 = {0};
				di.DockID	 = (*iter)->GetDockID();
				di.DockStyle = (*iter)->GetDockStyle();
				di.DockWidth = (*iter)->GetDockWidth();
				di.Rect		 = (*iter)->GetWindowRect();
				if ((*iter)->GetDockParent())
					di.DockParentID = (*iter)->GetDockParent()->GetDockID();

				vDockList.push_back(di);
			}

			tString tsKeyName = _T("Software\\") + tsRegistryKeyName;
			HKEY hKey = NULL;
			HKEY hKeyDock = NULL;
			if (RegCreateKeyEx(HKEY_CURRENT_USER, tsKeyName.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL))
				throw (CWinException(_T("RegCreateKeyEx Failed")));

			RegDeleteKey(hKey, _T("Dock Windows"));
			if (RegCreateKeyEx(hKey, _T("Dock Windows"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeyDock, NULL))
				throw (CWinException(_T("RegCreateKeyEx Failed")));

			// Add the Dock windows information to the registry
			for (size_t u = 0; u < vDockList.size(); ++u)
			{
				DockInfo di = vDockList[u];
				TCHAR szNumber[16];
				tString tsSubKey = _T("DockChild");
				tsSubKey += _itot((int)u, szNumber, 10);
				RegSetValueEx(hKeyDock, tsSubKey.c_str(), 0, REG_BINARY, (LPBYTE)&di, sizeof(DockInfo));
			}

			RegCloseKey(hKeyDock);
			RegCloseKey(hKey);
		}
	}

	inline void CDocker::SendNotify(UINT nMessageID)
	{
		// Send a docking notification to the docker below the cursor
		DRAGPOS DragPos;
		DragPos.hdr.code = nMessageID;
		DragPos.hdr.hwndFrom = m_hWnd;
		::GetCursorPos(&DragPos.ptPos);
		DragPos.DockZone = m_dwDockZone;
		m_dwDockZone = 0;

		CDocker* pDock = GetDockFromPoint(DragPos.ptPos);

		if (pDock)
			pDock->SendMessage(WM_NOTIFY, 0L, (LPARAM)&DragPos);
		else
		{
			if (GetDockHint().IsWindow())		GetDockHint().Destroy();
			CloseAllTargets();
			m_BlockMove = FALSE;
		}
	}

	inline void CDocker::SetDockStyle(DWORD dwDockStyle)
	{
		if ((dwDockStyle & DS_CLIENTEDGE) != (m_DockStyle & DS_CLIENTEDGE))
		{
			if (dwDockStyle & DS_CLIENTEDGE)
			{
				DWORD dwExStyle = (DWORD)GetDockClient().GetWindowLongPtr(GWL_EXSTYLE)|WS_EX_CLIENTEDGE;
				GetDockClient().SetWindowLongPtr(GWL_EXSTYLE, dwExStyle);
				GetDockClient().RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_FRAME);
			}
			else
			{
				DWORD dwExStyle = (DWORD)GetDockClient().GetWindowLongPtr(GWL_EXSTYLE);
				dwExStyle &= ~WS_EX_CLIENTEDGE;
				GetDockClient().SetWindowLongPtr(GWL_EXSTYLE, dwExStyle);
				GetDockClient().RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_FRAME);
			}
		}

		if (IsWindow())
			RecalcDockLayout();
		m_DockStyle = dwDockStyle;
	}

	inline void CDocker::SetCaption(LPCTSTR szCaption)
	{
		GetDockClient().SetCaption(szCaption);
		SetWindowText(szCaption);
	}

	inline void CDocker::SetDockWidth(int DockWidth)
	{
		m_DockStartWidth = DockWidth;
		m_DockWidthRatio = 1.0;
	}

	inline void CDocker::SetDragAutoResize(BOOL bAutoResize)
	{
		m_bDragAutoResize = bAutoResize;
	}

	inline void CDocker::SetView(CWnd& wndView)
	{
		CWnd* pWnd = &wndView;
		GetDockClient().SetView(wndView);
		if (pWnd->IsContainer())
		{
			CContainer* pContainer = (CContainer*)&wndView;
			SetCaption(pContainer->GetDockCaption().c_str());
		}
	}

	inline void CDocker::PromoteFirstChild()
	{
		// Promote our first child to replace ourself
		if (m_pDockParent)
		{
			for (UINT u = 0 ; u < m_pDockParent->m_vDockChildren.size(); ++u)
			{
				if (m_pDockParent->m_vDockChildren[u] == this)
				{
					if (m_vDockChildren.size() > 0)
						// swap our first child for ourself as a child of the parent
						m_pDockParent->m_vDockChildren[u] = m_vDockChildren[0];
					else
						// remove ourself as a child of the parent
						m_pDockParent->m_vDockChildren.erase(m_pDockParent->m_vDockChildren.begin() + u);
					break;
				}
			}
		}

		// Transfer styles and data and children to the child docker
		CDocker* pDockFirstChild = NULL;
		if (m_vDockChildren.size() > 0)
		{
			pDockFirstChild = m_vDockChildren[0];
			pDockFirstChild->m_DockStyle = (pDockFirstChild->m_DockStyle & 0xFFFFFFF0 ) | (m_DockStyle & 0xF);
			pDockFirstChild->m_DockStartWidth = m_DockStartWidth;
			pDockFirstChild->m_DockWidthRatio = m_DockWidthRatio;

			if (m_pDockParent)
			{
				pDockFirstChild->m_pDockParent = m_pDockParent;
				pDockFirstChild->SetParent(m_pDockParent->GetHwnd());
				pDockFirstChild->GetDockBar().SetParent(m_pDockParent->GetHwnd());
			}
			else
			{
				std::vector<CDocker*>::iterator iter;
				for (iter = GetDockChildren().begin() + 1; iter < GetDockChildren().end(); ++iter)
					(*iter)->ShowWindow(SW_HIDE);

				pDockFirstChild->ConvertToPopup(GetWindowRect());
				pDockFirstChild->GetDockBar().ShowWindow(SW_HIDE);
			}

			m_vDockChildren.erase(m_vDockChildren.begin());
			MoveDockChildren(pDockFirstChild);
		}
	}

	inline void CDocker::ConvertToChild(HWND hWndParent)
	{
		DWORD dwStyle = WS_CHILD | WS_VISIBLE;
		SetWindowLongPtr(GWL_STYLE, dwStyle);
		SetParent(hWndParent);
		GetDockBar().SetParent(hWndParent);
	}

	inline void CDocker::ConvertToPopup(RECT rc)
	{
		// Change the window to an "undocked" style
		ShowWindow(SW_HIDE);
		DWORD dwStyle = WS_POPUP| WS_CAPTION | WS_SYSMENU | WS_THICKFRAME;
		SetWindowLongPtr(GWL_STYLE, dwStyle);

		// Change the window's parent and reposition it
		GetDockBar().ShowWindow(SW_HIDE);
		SetWindowPos(0, 0, 0, 0, 0, SWP_NOSENDCHANGING|SWP_HIDEWINDOW|SWP_NOREDRAW);
		m_pDockParent = 0;
		SetParent(0);
		SetWindowPos(NULL, rc, SWP_SHOWWINDOW|SWP_FRAMECHANGED|SWP_NOOWNERZORDER);
		GetDockClient().SetWindowPos(NULL, GetClientRect(), SWP_SHOWWINDOW);

		SetWindowText(GetCaption().c_str());
	}

	inline void CDocker::Undock()
	{
		// Return if we shouldn't undock
		if (GetDockStyle() & DS_NO_UNDOCK) return;

		// Undocking isn't supported on Win95
		if (1400 == GetWinVersion()) return;

		CDocker* pDockUndockedFrom = GetDockParent();
		if (!pDockUndockedFrom && (GetDockChildren().size() > 0))
			pDockUndockedFrom = GetDockChildren()[0];

		GetDockTopLevel()->m_hOldFocus = 0;
		PromoteFirstChild();
		m_pDockParent = 0;

		// Get the current mouse position
		CPoint pt = GetCursorPos();

		GetDockBar().ShowWindow(SW_HIDE);
		m_DockStyle = m_DockStyle & 0xFFFFFFF0;
		m_DockStyle &= ~DS_DOCKED_CONTAINER;

		// Position and draw the undocked window, unless it is about to be closed
		if (!IsClosing())
		{
			m_Undocking = TRUE;
			CRect rc;
			rc = GetDockClient().GetWindowRect();
			CRect rcTest = rc;
			rcTest.bottom = MIN(rcTest.bottom, rcTest.top + m_NCHeight);
			if ( !PtInRect(&rcTest, pt))
				rc.SetRect(pt.x - rc.Width()/2, pt.y - m_NCHeight/2, pt.x + rc.Width()/2, pt.y - m_NCHeight/2 + rc.Height());

			ConvertToPopup(rc);
			m_Undocking = FALSE;
		

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

		RecalcDockLayout();
        if ((pDockUndockedFrom) && (pDockUndockedFrom->GetDockTopLevel() != GetDockTopLevel()))
			pDockUndockedFrom->RecalcDockLayout();
		
	}

	inline void CDocker::UndockContainer(CContainer* pContainer)
	{
		// Return if we shouldn't undock
		if (GetDockFromView(pContainer)->GetDockStyle() & DS_NO_UNDOCK) return;

		// Undocking isn't supported on Win95
		if (1400 == GetWinVersion()) return;

		GetDockTopLevel()->m_hOldFocus = 0;
		CDocker* pDockUndockedFrom = GetDockFromView(pContainer->GetContainerParent());
		if (GetView() == pContainer)
		{
			// The parent container is being undocked, so we need
			// to transfer our child containers to a different docker

			// Choose a new docker from among the dockers for child containers
			CDocker* pDockNew = 0;
			CDocker* pDockOld = GetDockFromView(pContainer);
			std::vector<ContainerInfo> AllContainers = pContainer->GetAllContainers();
			std::vector<ContainerInfo>::iterator iter = AllContainers.begin();
			while ((0 == pDockNew) && (iter < AllContainers.end()))
			{
				if ((*iter).pContainer != pContainer)
					pDockNew = (CDocker*)FromHandle(::GetParent((*iter).pContainer->GetParent()));

				++iter;
			}

			if (pDockNew)
			{
				// Move containers from the old docker to the new docker
				CContainer* pContainerNew = (CContainer*)pDockNew->GetView();
				for (iter = AllContainers.begin(); iter != AllContainers.end(); ++iter)
				{
					if ((*iter).pContainer != pContainer)
					{
						CContainer* pChildContainer = (CContainer*)(*iter).pContainer;
						pContainer->RemoveContainer(pChildContainer);
						if (pContainerNew != pChildContainer)
						{
							pContainerNew->AddContainer(pChildContainer);
							CDocker* pDock = GetDockFromView(pChildContainer);
							pDock->SetParent(pDockNew->GetHwnd());
							pDock->m_pDockParent = pDockNew;
						}
					}
				}

				// Now transfer the old docker's settings to the new one.
				pDockUndockedFrom = pDockNew;
				pDockNew->m_DockStyle		= pDockOld->m_DockStyle;
				pDockNew->m_DockStartWidth	= pDockOld->m_DockStartWidth;
				pDockNew->m_DockWidthRatio	= pDockOld->m_DockWidthRatio;
				if (pDockOld->IsDocked())
				{
					pDockNew->m_pDockParent		= pDockOld->m_pDockParent;
					pDockNew->SetParent(pDockOld->GetParent());
				}
				else
				{
					CRect rc = pDockOld->GetWindowRect();
					pDockNew->ShowWindow(SW_HIDE);
					DWORD dwStyle = WS_POPUP| WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_VISIBLE;
					pDockNew->SetWindowLongPtr(GWL_STYLE, dwStyle);
					pDockNew->m_pDockParent = 0;
					pDockNew->SetParent(0);
					pDockNew->SetWindowPos(NULL, rc, SWP_SHOWWINDOW|SWP_FRAMECHANGED| SWP_NOOWNERZORDER);
				}
				pDockNew->GetDockBar().SetParent(pDockOld->GetParent());
				pDockNew->GetView()->SetFocus();

				// Transfer the Dock children to the new docker
				pDockOld->MoveDockChildren(pDockNew);

				// insert pDockNew into its DockParent's DockChildren vector
				if (pDockNew->m_pDockParent)
				{
					std::vector<CDocker*>::iterator p;
					for (p = pDockNew->m_pDockParent->m_vDockChildren.begin(); p != pDockNew->m_pDockParent->m_vDockChildren.end(); ++p)
					{
						if (*p == this)
						{
							pDockNew->m_pDockParent->m_vDockChildren.insert(p, pDockNew);
							break;
						}
					}
				}
			}
		}
		else
		{
			// This is a child container, so simply remove it from the parent
			CContainer* pContainerParent = (CContainer*)GetView();
			pContainerParent->RemoveContainer(pContainer);
			pContainerParent->SetTabSize();
			pContainerParent->SetFocus();
			pContainerParent->GetViewPage().SetParent(pContainerParent->GetHwnd());
		}

		// Finally do the actual undocking
		CDocker* pDock = GetDockFromView(pContainer);
		CRect rc = GetDockClient().GetWindowRect();
		MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rc, 2);
		pDock->GetDockClient().SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
		pDock->Undock();
		pDockUndockedFrom->RecalcDockLayout();
	}

	inline LRESULT CDocker::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_ACTIVATE:
			OnActivate(wParam, lParam);
			break;
		case WM_SYSCOMMAND:
			return OnSysCommand(wParam, lParam);

		case WM_EXITSIZEMOVE:
			OnExitSizeMove(wParam, lParam);
			break;
		case WM_WINDOWPOSCHANGING:
			return OnWindowPosChanging(wParam, lParam);

		case WM_WINDOWPOSCHANGED:
			OnWindowPosChanged(wParam, lParam);
			break;
		case WM_DESTROY:
			OnDestroy(wParam, lParam);
			break;
		case WM_SETFOCUS:
			OnSetFocus(wParam, lParam);
			break;
		case WM_TIMER:
			OnCaptionTimer(wParam, lParam);
			break;
		case UWM_DOCK_DESTROYED:
			OnDockDestroyed(wParam, lParam);
			break;
		case UWM_DOCK_ACTIVATED:
			DrawAllCaptions();
			SetTimer(m_hWnd, 1, 100, NULL);
			break;
		case WM_SYSCOLORCHANGE:
			OnSysColorChange(wParam, lParam);
			break;
		}

		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	}


	//////////////////////////////////////
	// Declaration of the CContainer class
	inline CContainer::CContainer() : m_iCurrentPage(0), m_hTabIcon(0), m_nTabPressed(-1)
	{
		m_pContainerParent = this;
	}

	inline CContainer::~CContainer()
	{
	}

	inline void CContainer::AddContainer(CContainer* pContainer)
	{
		if (this == m_pContainerParent)
		{
			ContainerInfo ci = {0};
			ci.pContainer = pContainer;
			lstrcpy(ci.szTitle, pContainer->GetTabText());
			ci.iImage = ImageList_AddIcon(GetImageList(), pContainer->GetTabIcon());
			int iNewPage = (int)m_vContainerInfo.size();
			m_vContainerInfo.push_back(ci);

			if (m_hWnd)
			{
				TCITEM tie = {0};
				tie.mask = TCIF_TEXT | TCIF_IMAGE;
				tie.iImage = ci.iImage;
				tie.pszText = m_vContainerInfo[iNewPage].szTitle;
			//	InsertItem(iNewPage, &tie);
				TabCtrl_InsertItem(m_hWnd, iNewPage, &tie);

				SetTabSize();
			}

			pContainer->m_pContainerParent = this;
			if (pContainer->IsWindow())
			{
				// Set the parent container relationships
				pContainer->GetViewPage().SetParent(m_hWnd);
				pContainer->GetViewPage().ShowWindow(SW_HIDE);
			}
		}
	}

	inline void CContainer::AddToolbarButton(UINT nID, BOOL bEnabled /* = TRUE */)
	// Adds Resource IDs to toolbar buttons.
	// A resource ID of 0 is a separator
	{
		GetToolbar().AddToolbarButton(nID, bEnabled);
	}

	inline CContainer* CContainer::GetContainerFromIndex(size_t iPage)
	{
		if (iPage < m_vContainerInfo.size())
			return (CContainer*)m_vContainerInfo[iPage].pContainer;
		else
			return NULL;
	}

	inline CWnd* CContainer::GetActiveView() const
	{
		if (m_vContainerInfo.size() > 0)
		{
			CContainer* pActiveContainer = m_pContainerParent->m_vContainerInfo[m_iCurrentPage].pContainer;
			return pActiveContainer->GetViewPage().GetView();
		}
		else
			return NULL;
	}

	inline CContainer* CContainer::GetContainerFromView(CWnd* pView) const
	{
		std::vector<ContainerInfo>::iterator iter;
		CContainer* pViewContainer = 0;
		for (iter = GetAllContainers().begin(); iter != GetAllContainers().end(); ++iter)
		{
			CContainer* pContainer = (*iter).pContainer;
			if (pContainer->GetView() == pView)
				pViewContainer = pContainer;
		}

		return pViewContainer;
	}

	inline int CContainer::GetContainerIndex(CContainer* pContainer)
	{
		for (int i = 0; i < (int)m_vContainerInfo.size(); ++i)
		{
			if (m_vContainerInfo[i].pContainer == pContainer)
				return i;
		}

		return -1;
	}

	inline SIZE CContainer::GetMaxTabTextSize()
	{
		CSize Size;

		// Allocate an iterator for the ContainerInfo vector
		std::vector<ContainerInfo>::iterator iter;

		for (iter = m_vContainerInfo.begin(); iter != m_vContainerInfo.end(); ++iter)
		{
			CSize TempSize;
			CDC dc = GetDC();
			NONCLIENTMETRICS info = {0};
			info.cbSize = GetSizeofNonClientMetrics();
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
			dc.CreateFontIndirect(info.lfStatusFont);
			TempSize = dc.GetTextExtentPoint32(iter->szTitle, lstrlen(iter->szTitle));
			if (TempSize.cx > Size.cx)
				Size = TempSize;
		}

		return Size;
	}

	inline void CContainer::SetupToolbar()
	{
		// Use this function to set the Resource IDs for the toolbar(s).

/*		// Set the Resource IDs for the toolbar buttons
		AddToolbarButton( IDM_FILE_NEW   );
		AddToolbarButton( IDM_FILE_OPEN  );
		AddToolbarButton( IDM_FILE_SAVE  );
		AddToolbarButton( 0 );				// Separator
		AddToolbarButton( IDM_EDIT_CUT   );
		AddToolbarButton( IDM_EDIT_COPY  );
		AddToolbarButton( IDM_EDIT_PASTE );
		AddToolbarButton( 0 );				// Separator
		AddToolbarButton( IDM_FILE_PRINT );
		AddToolbarButton( 0 );				// Separator
		AddToolbarButton( IDM_HELP_ABOUT );
*/
	}

	inline void CContainer::OnCreate()
	{
		if (NULL == GetView())
			throw CWinException(_T("CContainer::OnCreate... View window not assigned!\nUse SetView to set the View Window"));

		ContainerInfo ci = {0};
		ci.pContainer = this;
		lstrcpy(ci.szTitle, GetTabText());
		ci.iImage = ImageList_AddIcon(GetImageList(), GetTabIcon());
		m_vContainerInfo.push_back(ci);

		// Create the page window
		GetViewPage().Create(m_hWnd);

		// Create the toolbar
		GetToolbar().Create(GetViewPage().GetHwnd());
		DWORD style = (DWORD)GetToolbar().GetWindowLongPtr(GWL_STYLE);
		style |= CCS_NODIVIDER ;//| CCS_NORESIZE;
		GetToolbar().SetWindowLongPtr(GWL_STYLE, style);
		SetupToolbar();
		if (GetToolbar().GetToolbarData().size() > 0)
		{
			// Set the toolbar images
			// A mask of 192,192,192 is compatible with AddBitmap (for Win95)
			if (!GetToolbar().SendMessage(TB_GETIMAGELIST,  0L, 0L))
				GetToolbar().SetImages(RGB(192,192,192), IDW_MAIN, 0, 0);

			GetToolbar().SendMessage(TB_AUTOSIZE, 0L, 0L);
		}
		else
			GetToolbar().Destroy();

		// Add tabs for each container.
		for (int i = 0; i < (int)m_vContainerInfo.size(); ++i)
		{
			// Add tabs for each view.
			TCITEM tie = {0};
			tie.mask = TCIF_TEXT | TCIF_IMAGE;
			tie.iImage = i;
			tie.pszText = m_vContainerInfo[i].szTitle;
			TabCtrl_InsertItem(m_hWnd, i, &tie);
		}
	}

	inline void CContainer::OnLButtonDown(WPARAM wParam, LPARAM lParam)
	{
		CPoint pt((DWORD)lParam);
		TCHITTESTINFO info = {0};
		info.pt = pt;
		m_nTabPressed = HitTest(info);

		CTab::OnLButtonDown(wParam, lParam);
	}

	inline void CContainer::OnLButtonUp(WPARAM /*wParam*/, LPARAM /*lParam*/)
	{
		// Override CTab::OnLButtonUp and take no action
	}

	inline void CContainer::OnMouseLeave(WPARAM wParam, LPARAM lParam)
	{

		if (IsLeftButtonDown() && (m_nTabPressed >= 0))
		{
			CDocker* pDock = (CDocker*)FromHandle(::GetParent(GetParent()));
			if (pDock && pDock->IsDocker())
			{
				CContainer* pContainer = GetContainerFromIndex(m_iCurrentPage);
				pDock->UndockContainer(pContainer);
			}
		}

		m_nTabPressed = -1;
		CTab::OnMouseLeave(wParam, lParam);
	}

	inline void CContainer::OnMouseMove(WPARAM wParam, LPARAM lParam)
	{
		CTab::OnMouseMove(wParam, lParam);
	}

	inline LRESULT CContainer::OnNotifyReflect(WPARAM /*wParam*/, LPARAM lParam)
	{
		switch (((LPNMHDR)lParam)->code)
		{
		case TCN_SELCHANGE:
			{
				// Display the newly selected tab page
				int iPage = GetCurSel();
				SelectPage(iPage);
			}
			break;
		}

		return 0L;
	}

	inline void CContainer::PreCreate(CREATESTRUCT &cs)
	{
		// For Tabs on the bottom, add the TCS_BOTTOM style
		cs.style = WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | TCS_OWNERDRAWFIXED | TCS_FIXEDWIDTH | TCS_BOTTOM;
		cs.lpszClass = WC_TABCONTROL;
	}

	inline void CContainer::RecalcLayout()
	{
		if ((int)m_vContainerInfo.size() > m_iCurrentPage)
		{
			// Set the tab sizes
			SetTabSize();

			// Position the View over the tab control's display area
			CRect rc = GetClientRect();
			AdjustRect(FALSE, &rc);
			CContainer* pContainer = m_vContainerInfo[m_iCurrentPage].pContainer;
			pContainer->GetViewPage().SetWindowPos(HWND_TOP, rc, SWP_SHOWWINDOW);
		}
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
		//	DeleteItem(iTab);
			TabCtrl_DeleteItem(m_hWnd, iTab);
		}

		// Remove the ContainerInfo entry
		std::vector<ContainerInfo>::iterator iter;
		int iImage = -1;
		for (iter = m_vContainerInfo.begin(); iter != m_vContainerInfo.end(); ++iter)
		{
			if (iter->pContainer == pWnd)
			{
				iImage = (*iter).iImage;
				if (iImage >= 0)
					TabCtrl_RemoveImage(m_hWnd, iImage);

				m_vContainerInfo.erase(iter);
				break;
			}
		}

		// Set the parent container relationships
		pWnd->GetViewPage().SetParent(pWnd->GetHwnd());
		pWnd->m_pContainerParent = pWnd;

		// Display the first page
		m_iCurrentPage = 0;
		if (IsWindow())
			SelectPage(0);
	}

	inline void CContainer::SelectPage(int iPage)
	{
		if ((iPage >= 0) && (iPage < (int)m_vContainerInfo.size() ))
		{
			SetCurSel(iPage);

			// Create the new container window if required
			if (!m_vContainerInfo[iPage].pContainer->IsWindow())
			{
				CContainer* pContainer = m_vContainerInfo[iPage].pContainer;
				pContainer->Create(GetParent());
				pContainer->GetViewPage().SetParent(m_hWnd);
			}

			// Determine the size of the tab page's view area
			CRect rc = GetClientRect();
			AdjustRect(FALSE, &rc);

			// Swap the pages over
			CContainer* pOldContainer = m_vContainerInfo[m_iCurrentPage].pContainer;
			CContainer* pNewContainer = m_vContainerInfo[iPage].pContainer;
			pOldContainer->GetViewPage().ShowWindow(SW_HIDE);
			pNewContainer->GetViewPage().SetWindowPos(HWND_TOP, rc, SWP_SHOWWINDOW);
			pNewContainer->GetViewPage().GetView()->SetFocus();

			// Adjust the docking caption
			CDocker* pDock = (CDocker*)FromHandle(::GetParent(GetParent()));
			if (pDock && pDock->IsDocker())
			{
				pDock->SetCaption(pNewContainer->GetDockCaption().c_str());
				pDock->RedrawWindow();
			}

			m_iCurrentPage = iPage;
		}
	}

	inline void CContainer::SetTabIcon(UINT nID_Icon)
	{
		HICON hIcon = LoadIcon(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(nID_Icon));
		SetTabIcon(hIcon);
	}

	inline void CContainer::SetTabSize()
	{
		CRect rc = GetClientRect();
		AdjustRect(FALSE, &rc);
		if (rc.Width() < 0 )
			rc.SetRectEmpty();

		int nItemWidth = MIN(25 + GetMaxTabTextSize().cx, (rc.Width()-2)/(int)m_vContainerInfo.size());
		SendMessage(TCM_SETITEMSIZE, 0L, MAKELPARAM(nItemWidth, 20));
	}

	inline void CContainer::SetTabText(UINT nTab, LPCTSTR szText)
	{
		CContainer* pContainer = GetContainerParent()->GetContainerFromIndex(nTab);
		pContainer->SetTabText(szText);

		CTab::SetTabText(nTab, szText);
	}

	inline void CContainer::SetView(CWnd& Wnd)
	{
		GetViewPage().SetView(Wnd);
	}

	inline LRESULT CContainer::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_SIZE:
			RecalcLayout();
			return 0;
		case WM_SETFOCUS:
			{
				// Pass focus on to the current view
				GetActiveView()->SetFocus();
			}
			break;
		}

		// pass unhandled messages on to CTab for processing
		return CTab::WndProcDefault(uMsg, wParam, lParam);
	}


	///////////////////////////////////////////
	// Declaration of the nested CViewPage class
	inline BOOL CContainer::CViewPage::OnCommand(WPARAM wParam, LPARAM lParam)
	{
		return (BOOL)::SendMessage(GetParent(), WM_COMMAND, wParam, lParam);
	}

	inline void CContainer::CViewPage::OnCreate()
	{
		if (m_pView)
			m_pView->Create(m_hWnd);
	}

	inline LRESULT CContainer::CViewPage::OnNotify(WPARAM /*wParam*/, LPARAM lParam)
	{

		switch (((LPNMHDR)lParam)->code)
		{

		// Display tooltips for the toolbar
		case TTN_GETDISPINFO:
			{
				int iIndex =  GetToolbar().HitTest();
				LPNMTTDISPINFO lpDispInfo = (LPNMTTDISPINFO)lParam;
				if (iIndex >= 0)
				{
					int nID = GetToolbar().GetCommandID(iIndex);
					if (nID > 0) lpDispInfo->lpszText = (LPTSTR)LoadString(nID);
				}
			}
			break;
		} // switch LPNMHDR

		return 0L;
	}

	inline void CContainer::CViewPage::PreRegisterClass(WNDCLASS &wc)
	{
		wc.lpszClassName = _T("Win32++ TabPage");
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	}

	inline void CContainer::CViewPage::RecalcLayout()
	{
		GetToolbar().SendMessage(TB_AUTOSIZE, 0L, 0L);
		CRect rc = GetClientRect();
		CRect rcToolbar = m_Toolbar.GetClientRect();
		rc.top += rcToolbar.Height();
		GetView()->SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
	}

	inline void CContainer::CViewPage::SetView(CWnd& wndView)
	// Sets or changes the View window displayed within the frame
	{
		// Assign the view window
		m_pView = &wndView;

		if (m_hWnd)
		{
			if (!m_pView->IsWindow())
			{
				// The container is already created, so create and position the new view too
				GetView()->Create(m_hWnd);
			}

			RecalcLayout();
		}
	}

	inline LRESULT CContainer::CViewPage::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	}

} // namespace Win32xx

#endif // _DOCKING_H_

