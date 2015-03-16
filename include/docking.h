// Win32++   Version 7.8
// Release Date: 17th March 2015
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2014  David Nash
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


// Acknowledgements:
// ================
// A special thanks to James Brown and Jeff Glatt for publishing tutorials
// on docking and making these freely available. This code uses some of the
// ideas presented in those tutorials.
//
// Catch22 Docking Tutorials by James Brown:
// http://www.catch22.net/tuts
//
// Docking Toolbars in Plain C by Jeff Glatt
// http://www.codeproject.com/Articles/10224/Docking-Toolbars-in-Plain-C


///////////////////////////////////////////////////////
// docking.h
//  Declaration of the CDocker class

#ifndef _WIN32XX_DOCKING_H_
#define _WIN32XX_DOCKING_H_


#include "wincore.h"
#include "gdi.h"
#include "toolbar.h"
#include "tab.h"
#include "default_resource.h"


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
#define DS_NO_CLOSE				0x0400	// Prevent closing of a docker while docked
#define DS_NO_UNDOCK			0x0800  // Prevent undocking of a docker
#define DS_CLIENTEDGE			0x1000  // Has a 3D border when docked
#define DS_NO_FIXED_RESIZE		0x2000	// Perform a proportional resize instead of a fixed size resize on dock children
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

#define TIMER_ID1 2101
#define TIMER_ID2 2102

namespace Win32xx
{
	// Class declarations
	class CDockContainer;
	class CDocker;

	typedef Shared_Ptr<CDocker> DockPtr;

	struct ContainerInfo
	{
		CString Title;
		int iImage;
		CDockContainer* pContainer;
		ContainerInfo() : iImage(0), pContainer(0) {}
	};

	///////////////////////////////////////
	// Declaration of the CDockContainer class
	//  A CDockContainer is a CTab window. A CTab has a view window, and optionally a toolbar control.
	//  A top level CDockContainer can contain other CDockContainers. The view for each container
	//  (including the top level container) along with possibly its toolbar, is displayed
	//  within the container parent's view page.
	class CDockContainer : public CTab
	{
	public:

		// Nested class. This is the Wnd for the window displayed over the client area
		// of the tab control.  The toolbar and view window are child windows of the
		// viewpage window. Only the ViewPage of the parent CDockContainer is displayed. It's
		// contents are updated with the view window of the relevant container whenever
		// a different tab is selected.
		class CViewPage : public CWnd
		{

		public:
			CViewPage() : m_pView(NULL), m_pTab(NULL) {}
			virtual ~CViewPage() {}
			virtual CToolBar* GetToolBar() const	{return (CToolBar*)&m_ToolBar;}
			virtual CWnd* GetView() const	{return m_pView;}
			virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
			virtual int OnCreate(LPCREATESTRUCT pcs);
			virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
			virtual void PreRegisterClass(WNDCLASS &wc);
			virtual void RecalcLayout();
			virtual void SetView(CWnd& wndView);
			virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

			CWnd* GetTabCtrl() const { return m_pTab;}

		private:
			CToolBar m_ToolBar;
			CString m_strTooltip;
			CWnd* m_pView;
			CWnd* m_pTab;
		};

	public:
		CDockContainer();
		virtual ~CDockContainer();
		virtual void AddContainer(CDockContainer* pContainer, BOOL bInsert = FALSE);
		virtual void AddToolBarButton(UINT nID, BOOL bEnabled = TRUE);
		virtual CDockContainer* GetContainerFromIndex(UINT nPage);
		virtual CDockContainer* GetContainerFromView(CWnd* pView) const;
		virtual int GetContainerIndex(CDockContainer* pContainer);
		virtual SIZE GetMaxTabTextSize();
		virtual CViewPage* GetViewPage() const	{ return (CViewPage*)&m_ViewPage; }
		virtual int GetTabImageID(UINT nTab) const;
		virtual CString GetTabText(UINT nTab) const;
		virtual void RecalcLayout();
		virtual void RemoveContainer(CDockContainer* pWnd);
		virtual void SelectPage(int nPage);
		virtual void SetTabSize();
		virtual void SetupToolBar();
		virtual void SwapTabs(UINT nTab1, UINT nTab2);

		// Attributes
		CDockContainer* GetActiveContainer() const {return GetContainerFromView(GetActiveView());}
		CWnd* GetActiveView() const;
		std::vector<ContainerInfo>* GetAllContainers() const {return &m_pContainerParent->m_vContainerInfo;}
		CDockContainer* GetContainerParent() const { return m_pContainerParent; }
		CString& GetDockCaption() const	{ return (CString&)m_csCaption; }
		HICON GetTabIcon() const		{ return m_hTabIcon; }
		LPCTSTR GetTabText() const		{ return m_strTabText; }
		virtual CToolBar* GetToolBar()	const { return GetViewPage()->GetToolBar(); }
		CWnd* GetView()	const			{ return GetViewPage()->GetView(); }
		void SetActiveContainer(CDockContainer* pContainer);
		void SetDockCaption(LPCTSTR szCaption) { m_csCaption = szCaption; }
		void SetHideSingleTab(BOOL HideSingle); 
		void SetTabIcon(HICON hTabIcon) { m_hTabIcon = hTabIcon; }
		void SetTabIcon(UINT nID_Icon);
		void SetTabIcon(int i, HICON hIcon) { CTab::SetTabIcon(i, hIcon); }
		void SetTabText(LPCTSTR szText) { m_strTabText = szText; }
		void SetTabText(UINT nTab, LPCTSTR szText);
		void SetView(CWnd& Wnd);

	protected:
		virtual void OnAttach();
		virtual LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnSetFocus(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnSize(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnTCNSelChange(LPNMHDR pNMHDR);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual void SetToolBarImages(COLORREF crMask, UINT ToolBarID, UINT ToolBarHotID, UINT ToolBarDisabledID);
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		std::vector<ContainerInfo> m_vContainerInfo;	// vector of ContainerInfo structs
		std::vector<UINT> m_vToolBarData;				// vector of resource IDs for ToolBar buttons
		CString m_strTabText;
		CString m_csCaption;
		CImageList m_imlToolBar;
		CImageList m_imlToolBarHot;
		CImageList m_imlToolBarDis;
		CViewPage m_ViewPage;
		int m_iCurrentPage;
		CDockContainer* m_pContainerParent;
		HICON m_hTabIcon;
		int m_nTabPressed;
		BOOL m_IsHideSingleTab;
		CPoint m_OldMousePos;
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
	public:
		//  A nested class for the splitter bar that separates the docked panes.
		class CDockBar : public CWnd
		{
		public:
			CDockBar();
			virtual ~CDockBar();
			virtual void OnDraw(CDC* pDC);
			virtual void PreCreate(CREATESTRUCT &cs);
			virtual void PreRegisterClass(WNDCLASS& wc);
			virtual void SendNotify(UINT nMessageID);
			virtual void SetColor(COLORREF color);
			virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

			CDocker* GetDock() const		{return m_pDock;}
			int GetWidth() const			{return m_DockBarWidth;}
			void SetDock(CDocker* pDock)	{m_pDock = pDock;}
			void SetWidth(int nWidth)		{m_DockBarWidth = nWidth;}

		protected:
			virtual LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnSetCursor(WPARAM wParam, LPARAM lParam);

		private:
			CDockBar(const CDockBar&);				// Disable copy construction
			CDockBar& operator = (const CDockBar&); // Disable assignment operator

			CDocker* m_pDock;
			DRAGPOS m_DragPos;
			CBrush m_brBackground;
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
			virtual void DrawCaption();
			virtual void DrawCloseButton(CDC& DrawDC, BOOL bFocus);
			virtual CRect GetCloseRect() const;
			virtual void SendNotify(UINT nMessageID);

			CString& GetCaption() const		{ return (CString&)m_csCaption; }
			CWnd* GetView() const			{ return m_pView; }
			void SetDock(CDocker* pDock)	{ m_pDock = pDock;}
			void SetCaption(LPCTSTR szCaption) { m_csCaption = szCaption; }
			void SetCaptionColors(COLORREF Foregnd1, COLORREF Backgnd1, COLORREF ForeGnd2, COLORREF BackGnd2);
			void SetView(CWnd& wndView);

		protected:
			virtual void    OnLButtonDown(WPARAM wParam, LPARAM lParam);
			virtual LRESULT	OnLButtonUp(WPARAM wParam, LPARAM lParam);
			virtual LRESULT	OnMouseActivate(WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnNCCalcSize(WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnNCHitTest(WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnNCLButtonDblClk(WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnNCLButtonDown(WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnNCMouseLeave(WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnNCMouseMove(WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnNCPaint(WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnWindowPosChanged(WPARAM wParam, LPARAM lParam);
			virtual void    PreRegisterClass(WNDCLASS& wc);
			virtual void    PreCreate(CREATESTRUCT& cs);
			virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

		private:
			LRESULT MouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);
			CDockClient(const CDockClient&);				// Disable copy construction
			CDockClient& operator = (const CDockClient&);	// Disable assignment operator

			CString m_csCaption;
			CPoint m_Oldpt;
			CDocker* m_pDock;
			CWnd* m_pView;
			BOOL m_IsClosePressed;
			BOOL m_IsOldFocusStored;
			BOOL m_IsCaptionPressed;
			BOOL m_IsTracking;
			COLORREF m_Foregnd1;
			COLORREF m_Backgnd1;
			COLORREF m_Foregnd2;
			COLORREF m_Backgnd2;
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
			virtual void OnDraw(CDC* pDC);
			virtual void PreCreate(CREATESTRUCT &cs);

		private:
			CDockHint(const CDockHint&);				// Disable copy construction
			CDockHint& operator = (const CDockHint&); // Disable assignment operator

			CBitmap m_bmBlueTint;
			UINT m_uDockSideOld;
		};

		class CTarget : public CWnd
		{
		public:
			CTarget() {}
			virtual ~CTarget();
			virtual void OnDraw(CDC* pDC);
			virtual void PreCreate(CREATESTRUCT &cs);

		protected:
			CBitmap m_bmImage;

		private:
			CTarget(const CTarget&);				// Disable copy construction
			CTarget& operator = (const CTarget&); // Disable assignment operator
		};

		class CTargetCentre : public CTarget
		{
		public:
			CTargetCentre();
			virtual ~CTargetCentre();
			virtual void OnDraw(CDC* pDC);
			virtual int  OnCreate(LPCREATESTRUCT pcs);
			virtual BOOL CheckTarget(LPDRAGPOS pDragPos);
			BOOL IsOverContainer() { return m_IsOverContainer; }

		private:
			CTargetCentre(const CTargetCentre&);				// Disable copy construction
			CTargetCentre& operator = (const CTargetCentre&); // Disable assignment operator

			BOOL m_IsOverContainer;
			CDocker* m_pOldDockTarget;
		};

		class CTargetLeft : public CTarget
		{
		public:
			CTargetLeft() {m_bmImage.LoadImage(IDW_SDLEFT,0,0,0);}
			virtual BOOL CheckTarget(LPDRAGPOS pDragPos);

		private:
			CTargetLeft(const CTargetLeft&);				// Disable copy construction
			CTargetLeft& operator = (const CTargetLeft&); // Disable assignment operator
		};

		class CTargetTop : public CTarget
		{
		public:
			CTargetTop() {m_bmImage.LoadImage(IDW_SDTOP,0,0,0);}
			virtual BOOL CheckTarget(LPDRAGPOS pDragPos);
		private:
			CTargetTop(const CTargetTop&);				// Disable copy construction
			CTargetTop& operator = (const CTargetTop&); // Disable assignment operator
		};

		class CTargetRight : public CTarget
		{
		public:
			CTargetRight() {m_bmImage.LoadImage(IDW_SDRIGHT,0,0,0);}
			virtual BOOL CheckTarget(LPDRAGPOS pDragPos);

		private:
			CTargetRight(const CTargetRight&);				// Disable copy construction
			CTargetRight& operator = (const CTargetRight&); // Disable assignment operator
		};

		class CTargetBottom : public CTarget
		{
		public:
			CTargetBottom() {m_bmImage.LoadImage(IDW_SDBOTTOM,0,0,0);}
			virtual BOOL CheckTarget(LPDRAGPOS pDragPos);
		};

		friend class CTargetCentre;
		friend class CTargetLeft;
		friend class CTargetTop;
		friend class CTargetRight;
		friend class CTargetBottom;
		friend class CDockClient;
		friend class CDockContainer;

	public:
		// Operations
		CDocker();
		virtual ~CDocker();
		virtual CDocker* AddDockedChild(CDocker* pDocker, DWORD dwDockStyle, int DockSize, int nDockID = 0);
		virtual CDocker* AddUndockedChild(CDocker* pDocker, DWORD dwDockStyle, int DockSize, const RECT& rc, int nDockID = 0);
		virtual void Close();
		virtual void CloseAllDockers();
		virtual void Dock(CDocker* pDocker, UINT uDockSide);
		virtual void DockInContainer(CDocker* pDock, DWORD dwDockStyle);
		virtual CDockContainer* GetContainer() const;
		virtual CDocker* GetActiveDocker() const;
		virtual CDocker* GetDockAncestor() const;
		virtual CDocker* GetDockFromID(int n_DockID) const;
		virtual CDocker* GetDockFromPoint(POINT pt) const;
		virtual CDocker* GetDockFromView(CWnd* pView) const;
		virtual CDocker* GetTopmostDocker() const;
		virtual int GetDockSize() const;
		virtual CTabbedMDI* GetTabbedMDI() const;
		virtual int GetTextHeight();
		virtual void Hide();
		virtual BOOL LoadDockRegistrySettings(LPCTSTR szRegistryKeyName);
		virtual void RecalcDockLayout();
		virtual BOOL SaveDockRegistrySettings(LPCTSTR szRegistryKeyName);
		virtual void Undock(CPoint pt, BOOL bShowUndocked = TRUE);
		virtual void UndockContainer(CDockContainer* pContainer, CPoint pt, BOOL bShowUndocked);
		virtual BOOL VerifyDockers();

		// Attributes
		virtual CDockBar* GetDockBar() const		{ return const_cast<CDockBar*>(&m_DockBar); }
		virtual CDockClient* GetDockClient() const	{ return const_cast<CDockClient*>(&m_DockClient); }
		virtual CDockHint* GetDockHint() const		{ return &m_pDockAncestor->m_DockHint; }
		virtual CRect GetViewRect() const			{ return GetClientRect(); }

		std::vector <DockPtr> * GetAllDockChildren() const {return const_cast< std::vector<DockPtr>* >(&GetDockAncestor()->m_vAllDockChildren);}
		std::vector <CDocker*> * GetDockChildren()	const {return const_cast< std::vector<CDocker*>* >(&m_vDockChildren);}
		std::vector <CDocker*> * GetAllDockers()	const {return const_cast< std::vector<CDocker*>* >(&m_vAllDockers);}
		int GetBarWidth() const				{return GetDockBar()->GetWidth();}
		CString& GetCaption() const			{return GetDockClient()->GetCaption();}
		int GetDockID() const				{return m_nDockID;}
		CDocker* GetDockParent() const		{return m_pDockParent;}
		DWORD GetDockStyle() const			{return m_DockStyle;}
		CWnd* GetView() const				{return GetDockClient()->GetView();}
		BOOL IsChildOfDocker(const CWnd* pWnd) const;
		BOOL IsDocked() const;
		BOOL IsDragAutoResize() const;
		BOOL IsRelated(const CWnd* pWnd) const;
		BOOL IsUndocked() const;
		void SetBarColor(COLORREF color) {GetDockBar()->SetColor(color);}
		void SetBarWidth(int nWidth) {GetDockBar()->SetWidth(nWidth);}
		void SetCaption(LPCTSTR szCaption);
		void SetCaptionColors(COLORREF Foregnd1, COLORREF Backgnd1, COLORREF ForeGnd2, COLORREF BackGnd2);
		void SetCaptionHeight(int nHeight);
		void SetDockStyle(DWORD dwDockStyle);
		void SetDockSize(int DockSize);
		void SetDragAutoResize(BOOL bAutoResize);
		BOOL SetRedraw(BOOL bRedraw = TRUE);
		void SetView(CWnd& wndView);

	protected:
		virtual CDocker* NewDockerFromID(int idDock);
		virtual int  OnCreate(LPCREATESTRUCT pcs);
		virtual void OnDestroy();
		virtual LRESULT OnBarEnd(LPDRAGPOS pdp);
		virtual LRESULT OnBarMove(LPDRAGPOS pdp);
		virtual LRESULT OnBarStart(LPDRAGPOS pdp);
		virtual LRESULT OnDockActivated(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnDockDestroyed(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnDockEnd(LPDRAGPOS pdp);
		virtual LRESULT OnDockMove(LPDRAGPOS pdp);
		virtual LRESULT OnDockStart(LPDRAGPOS pdp);
		virtual LRESULT OnExitSizeMove(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNCLButtonDblClk(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnSysColorChange(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnSysCommand(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnTimer(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnWindowPosChanging(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnWindowPosChanged(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual void PreRegisterClass(WNDCLASS &wc);
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		CDocker(const CDocker&);				// Disable copy construction
		CDocker& operator = (const CDocker&);	// Disable assignment operator
		void CheckAllTargets(LPDRAGPOS pDragPos);
		void CloseAllTargets();
		void DockOuter(CDocker* pDocker, DWORD dwDockStyle);
		void DrawAllCaptions();
		void DrawHashBar(HWND hBar, POINT Pos);
		void ConvertToChild(HWND hWndParent);
		void ConvertToPopup(const CRect& rc);
		void MoveDockChildren(CDocker* pDockTarget);
		void MoveDockInContainerChildren(CDocker* pDockTarget);
		void PromoteFirstChild();
		void RecalcDockChildLayout(CRect& rc);
		void ResizeDockers(LPDRAGPOS pdp);
		CDocker* SeparateFromDock();
		void SendNotify(UINT nMessageID);
		void SetUndockPosition(CPoint pt);
		std::vector<CDocker*> SortDockers();

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

		std::vector <CDocker*> m_vDockChildren;		// Docker's immediate children
		std::vector <DockPtr> m_vAllDockChildren;	// All descendants of the DockAncestor (only for the DockAncestor)
		std::vector <CDocker*> m_vAllDockers;		// DockAncestor + all descendants (only for the DockAncestor)

		CRect m_rcBar;
		CRect m_rcChild;

		BOOL m_IsBlockMove;
		BOOL m_IsUndocking;
		BOOL m_IsClosing;
		BOOL m_IsDragging;
		BOOL m_IsDragAutoResize;
		int m_DockStartSize;
		int m_nDockID;
		int m_nRedrawCount;
		int m_NCHeight;
		DWORD m_dwDockZone;
		double m_DockSizeRatio;
		DWORD m_DockStyle;

	}; // class CDocker

	struct DockInfo
	{
		DWORD DockStyle;
		int DockSize;
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
		m_brBackground.CreateSolidBrush(RGB(192,192,192));
	}

	inline CDocker::CDockBar::~CDockBar()
	{
	}

	inline void CDocker::CDockBar::OnDraw(CDC* pDC)
	{
		CRect rcClient = GetClientRect();
		pDC->SelectObject(&m_brBackground);
		pDC->PatBlt(0, 0, rcClient.Width(), rcClient.Height(), PATCOPY);
	}

	inline LRESULT CDocker::CDockBar::OnLButtonDown(WPARAM wParam, LPARAM lParam)
	{
		if (!(m_pDock->GetDockStyle() & DS_NO_RESIZE))
		{
			SendNotify(UWN_BARSTART);
			SetCapture();
		}

		return FinalWindowProc(WM_LBUTTONDOWN, wParam, lParam);
	}

	inline LRESULT CDocker::CDockBar::OnLButtonUp(WPARAM wParam, LPARAM lParam)
	{
		if (!(m_pDock->GetDockStyle() & DS_NO_RESIZE) && (GetCapture() == this))
		{
			SendNotify(UWN_BAREND);
			ReleaseCapture();
		}

		return FinalWindowProc(WM_LBUTTONUP, wParam, lParam);
	}

	inline LRESULT CDocker::CDockBar::OnMouseMove(WPARAM wParam, LPARAM lParam)
	{
		if (!(m_pDock->GetDockStyle() & DS_NO_RESIZE) && (GetCapture() == this))
		{
			SendNotify(UWN_BARMOVE);
		}

		return FinalWindowProc(WM_MOUSEMOVE, wParam, lParam);
	}

	inline LRESULT CDocker::CDockBar::OnSetCursor(WPARAM wParam, LPARAM lParam)
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
			else TRACE("**WARNING** Missing cursor resource for slider bar\n");

			return TRUE;
		}
		else
			SetCursor(LoadCursor(NULL, IDC_ARROW));

		return FinalWindowProc(WM_SETCURSOR, wParam, lParam);
	}

	inline void CDocker::CDockBar::PreCreate(CREATESTRUCT &cs)
	{
		// Create a child window, initially hidden
		cs.style = WS_CHILD;
	}

	inline void CDocker::CDockBar::PreRegisterClass(WNDCLASS& wc)
	{
		wc.lpszClassName = _T("Win32++ Bar");
		wc.hbrBackground = m_brBackground;
	}

	inline void CDocker::CDockBar::SendNotify(UINT nMessageID)
	{
		// Send a splitter bar notification to the parent
		m_DragPos.hdr.code = nMessageID;
		m_DragPos.hdr.hwndFrom = m_hWnd;
		m_DragPos.ptPos = GetCursorPos();
		m_DragPos.ptPos.x += 1;
		GetParent()->SendMessage(WM_NOTIFY, 0L, (LPARAM)&m_DragPos);
	}

	inline void CDocker::CDockBar::SetColor(COLORREF color)
	{
		// Useful colors:
		// GetSysColor(COLOR_BTNFACE)	// Default Grey
		// RGB(196, 215, 250)			// Default Blue

		m_brBackground.DeleteObject();
		m_brBackground.CreateSolidBrush(color);
	}

	inline LRESULT CDocker::CDockBar::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_SETCURSOR:		return OnSetCursor(wParam, lParam);
		case WM_ERASEBKGND:		return 0L;
		case WM_LBUTTONDOWN:	return OnLButtonDown(wParam, lParam);
		case WM_LBUTTONUP:		return OnLButtonUp(wParam, lParam);
		case WM_MOUSEMOVE:		return OnMouseMove(wParam, lParam);
		}

		// pass unhandled messages on for default processing
		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	}


	////////////////////////////////////////////////////////////////
	// Definitions for the CDockClient class nested within CDocker
	//
	inline CDocker::CDockClient::CDockClient() : m_pView(0), m_IsClosePressed(FALSE),
						m_IsOldFocusStored(FALSE), m_IsCaptionPressed(FALSE), m_IsTracking(FALSE)
	{
		m_Foregnd1 = RGB(32,32,32);
		m_Backgnd1 = RGB(190,207,227);
		m_Foregnd2 =  GetSysColor(COLOR_BTNTEXT);
		m_Backgnd2 = GetSysColor(COLOR_BTNFACE);
	}

	inline void CDocker::CDockClient::Draw3DBorder(RECT& Rect)
	{
		// Imitates the drawing of the WS_EX_CLIENTEDGE extended style
		// This draws a 2 pixel border around the specified Rect
		CWindowDC dc(this);
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

	inline CRect CDocker::CDockClient::GetCloseRect() const
	{
		// Calculate the close rect position in screen co-ordinates
		CRect rcClose;

		int gap = 4;
		CRect rc = GetWindowRect();
		int cx = GetSystemMetrics(SM_CXSMICON);
		int cy = GetSystemMetrics(SM_CYSMICON);

		rcClose.top = 2 + rc.top + m_pDock->m_NCHeight/2 - cy/2;
		rcClose.bottom = 2 + rc.top + m_pDock->m_NCHeight/2 + cy/2;
		rcClose.right = rc.right - gap;
		rcClose.left = rcClose.right - cx;

#if (WINVER >= 0x0500)
		if (GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_LAYOUTRTL)
		{
			rcClose.left = rc.left + gap;
			rcClose.right = rcClose.left + cx;
		}
#endif


		return rcClose;
	}

	inline void CDocker::CDockClient::DrawCaption()
	{
		if (IsWindow() && m_pDock->IsDocked() && !(m_pDock->GetDockStyle() & DS_NO_CAPTION))
		{
			BOOL bFocus = m_pDock->IsChildOfDocker(GetFocus());
			m_IsOldFocusStored = FALSE;

			// Acquire the DC for our NonClient painting
			CDC* pDC = GetWindowDC();

			// Create and set up our memory DC
			CRect rc = GetWindowRect();
			CMemDC dcMem(pDC);
			int rcAdjust = (GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_CLIENTEDGE)? 2 : 0;
			int Width = MAX(rc.Width() -rcAdjust, 0);
			int Height = m_pDock->m_NCHeight + rcAdjust;
			dcMem.CreateCompatibleBitmap(pDC, Width, Height);
			m_IsOldFocusStored = bFocus;

			// Set the font for the title
			NONCLIENTMETRICS info;
			ZeroMemory(&info, sizeof(NONCLIENTMETRICS));
			info.cbSize = GetSizeofNonClientMetrics();
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
			dcMem.CreateFontIndirect(&info.lfStatusFont);

			// Set the Colours
			if (bFocus)
			{
				dcMem.SetTextColor(m_Foregnd1);
				dcMem.CreateSolidBrush(m_Backgnd1);
				dcMem.SetBkColor(m_Backgnd1);
			}
			else
			{
				dcMem.SetTextColor(m_Foregnd2);
				dcMem.CreateSolidBrush(m_Backgnd2);
				dcMem.SetBkColor(m_Backgnd2);
			}

			// Draw the rectangle
			dcMem.CreatePen(PS_SOLID, 1, RGB(160, 150, 140));
			dcMem.Rectangle(rcAdjust, rcAdjust, rc.Width() - rcAdjust, m_pDock->m_NCHeight + rcAdjust);

			// Display the caption
			int cx = (m_pDock->GetDockStyle() & DS_NO_CLOSE) ? 0 : GetSystemMetrics(SM_CXSMICON);
			CRect rcText(4 + rcAdjust, rcAdjust, rc.Width() - 4 - cx - rcAdjust, m_pDock->m_NCHeight + rcAdjust);
			dcMem.DrawText(m_csCaption, m_csCaption.GetLength(), rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

			// Draw the close button
			if (!(m_pDock->GetDockStyle() & DS_NO_CLOSE))
				DrawCloseButton(dcMem, bFocus);

			// Draw the 3D border
			if (GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_CLIENTEDGE)
				Draw3DBorder(rc);

			// Copy the Memory DC to the window's DC
			pDC->BitBlt(rcAdjust, rcAdjust, Width, Height, &dcMem, rcAdjust, rcAdjust, SRCCOPY);

			// Required for Win98/WinME
			pDC->Destroy();
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
			ScreenToClient(rcClose);

			if (GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_CLIENTEDGE)
			{
				rcClose.OffsetRect(2, m_pDock->m_NCHeight+2);
				if (GetWindowRect().Height() < (m_pDock->m_NCHeight+4))
					rcClose.OffsetRect(-2, -2);
			}
			else
				rcClose.OffsetRect(0, m_pDock->m_NCHeight-2);

			// Draw the outer highlight for the close button
			if (!IsRectEmpty(&rcClose))
			{
				switch (uState)
				{
				case 0:
					{
						// Normal button
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
					DrawDC.CreatePen(PS_SOLID, 1, m_Foregnd1);
				else
					DrawDC.CreatePen(PS_SOLID, 1, m_Foregnd2);

				DrawDC.MoveTo(rcClose.left + 3, rcClose.top +3);
				DrawDC.LineTo(rcClose.right - 2, rcClose.bottom -2);

				DrawDC.MoveTo(rcClose.left + 4, rcClose.top +3);
				DrawDC.LineTo(rcClose.right - 2, rcClose.bottom -3);

				DrawDC.MoveTo(rcClose.left + 3, rcClose.top +4);
				DrawDC.LineTo(rcClose.right - 3, rcClose.bottom -2);

				DrawDC.MoveTo(rcClose.right -3, rcClose.top +3);
				DrawDC.LineTo(rcClose.left + 2, rcClose.bottom -2);

				DrawDC.MoveTo(rcClose.right -3, rcClose.top +4);
				DrawDC.LineTo(rcClose.left + 3, rcClose.bottom -2);

				DrawDC.MoveTo(rcClose.right -4, rcClose.top +3);
				DrawDC.LineTo(rcClose.left + 2, rcClose.bottom -3);
			}
		}
	}

	inline LRESULT CDocker::CDockClient::OnNCCalcSize(WPARAM wParam, LPARAM lParam)
	{
		// Sets the non-client area (and hence sets the client area)
		// This function modifies lParam

		if ((0 != m_pDock) && !(m_pDock->GetDockStyle() & DS_NO_CAPTION))
		{
			if (m_pDock->IsDocked())
			{
				LPRECT rc = (LPRECT)lParam;
				rc->top += m_pDock->m_NCHeight;
			}
		}

		return CWnd::WndProcDefault(WM_NCCALCSIZE, wParam, lParam);
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

				ScreenToClient(pt);

				// Indicate if the point is in the caption
				if (pt.y < 0)
					return HTCAPTION;
			}
		}
		return CWnd::WndProcDefault(WM_NCHITTEST, wParam, lParam);
	}

	inline LRESULT CDocker::CDockClient::OnNCLButtonDblClk(WPARAM wParam, LPARAM lParam)
	{
		if ((0 != m_pDock) && !(m_pDock->GetDockStyle() & DS_NO_CAPTION))
		{
			if ((HTCLOSE == wParam) && !(m_pDock->GetDockStyle() & DS_NO_CLOSE))
			{
				m_IsClosePressed = TRUE;
				SetCapture();
			}

			m_IsCaptionPressed = TRUE;
			m_Oldpt.x = GET_X_LPARAM(lParam);
			m_Oldpt.y = GET_Y_LPARAM(lParam);
			if (m_pDock->IsDocked())
			{
				CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				ScreenToClient(pt);
				if (!GetView()->IsChild(GetFocus()))
					GetView()->SetFocus();

				// Update the close button
				if ((0 != m_pDock) && !(m_pDock->GetDockStyle() & DS_NO_CLOSE))
				{
					CWindowDC dc(this);
					DrawCloseButton(dc, m_IsOldFocusStored);
				}

				return 0L;
			}
		}
		return CWnd::WndProcDefault(WM_NCLBUTTONDBLCLK, wParam, lParam);
	}

	inline LRESULT CDocker::CDockClient::OnNCLButtonDown(WPARAM wParam, LPARAM lParam)
	{
		if ((0 != m_pDock) && !(m_pDock->GetDockStyle() & DS_NO_CAPTION))
		{
			if ((HTCLOSE == wParam) && !(m_pDock->GetDockStyle() & DS_NO_CLOSE))
			{
				m_IsClosePressed = TRUE;
				SetCapture();
			}

			m_IsCaptionPressed = TRUE;
			m_Oldpt.x = GET_X_LPARAM(lParam);
			m_Oldpt.y = GET_Y_LPARAM(lParam);
			if (m_pDock->IsDocked())
			{
				CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				ScreenToClient(pt);
				if (!GetView()->IsChild(GetFocus()))
					GetView()->SetFocus();

				// Update the close button
				if ((0 != m_pDock) && !(m_pDock->GetDockStyle() & DS_NO_CLOSE))
				{
					CWindowDC dc(this);
					DrawCloseButton(dc, m_IsOldFocusStored);
				}

				return 0L;
			}
		}

		return CWnd::WndProcDefault(WM_NCLBUTTONDOWN, wParam, lParam);
	}

	inline LRESULT CDocker::CDockClient::OnLButtonUp(WPARAM wParam, LPARAM lParam)
	{
		ReleaseCapture();
		if ((0 != m_pDock) && !(m_pDock->GetDockStyle() & DS_NO_CLOSE))
		{
			CWindowDC dc(this);
			DrawCloseButton(dc, m_IsOldFocusStored);
			if ((0 != m_pDock) && !(m_pDock->GetDockStyle() & (DS_NO_CAPTION|DS_NO_CLOSE)))
			{
				m_IsCaptionPressed = FALSE;
				if (m_IsClosePressed && GetCloseRect().PtInRect(GetCursorPos()))
				{
					// Destroy the docker
					if (m_pDock->GetContainer())
					{
						CDockContainer* pContainer = m_pDock->GetContainer()->GetActiveContainer();
						CDocker* pDock = m_pDock->GetDockFromView(pContainer);
						pDock->Close();
					}
					else
					{
						m_pDock->Close();
					}
				}
			}
		}

		return CWnd::WndProcDefault(WM_LBUTTONUP, wParam, lParam);
	}

	inline void CDocker::CDockClient::OnLButtonDown(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);
		UNREFERENCED_PARAMETER(lParam);

		m_IsClosePressed = FALSE;
		ReleaseCapture();
		CWindowDC dc(this);
		DrawCloseButton(dc, m_IsOldFocusStored);
	}

	inline LRESULT CDocker::CDockClient::OnMouseActivate(WPARAM wParam, LPARAM lParam)
	// Focus changed, so redraw the captions
	{
		if ((0 != m_pDock) && !(m_pDock->GetDockStyle() & DS_NO_CAPTION))
		{
			m_pDock->GetDockAncestor()->PostMessage(UWM_DOCKACTIVATE, 0, 0);
		}

		return FinalWindowProc(WM_MOUSEACTIVATE, wParam, lParam);
	}

	inline LRESULT CDocker::CDockClient::MouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (!m_IsTracking)
		{
			TRACKMOUSEEVENT TrackMouseEventStruct;
			ZeroMemory(&TrackMouseEventStruct, sizeof(TRACKMOUSEEVENT));
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

				if (IsLeftButtonDown() && (wParam == HTCAPTION)  && (m_IsCaptionPressed))
				{
					CDocker* pDock = static_cast<CDocker*>(GetParent());
					assert(dynamic_cast<CDocker*>(pDock));
					if (pDock)
						pDock->Undock(GetCursorPos());
				}

				// Update the close button
				if ((0 != m_pDock) && !(m_pDock->GetDockStyle() & DS_NO_CLOSE))
				{
					CWindowDC dc(this);
					DrawCloseButton(dc, m_IsOldFocusStored);
				}
			}

			m_IsCaptionPressed = FALSE;
		}

		return FinalWindowProc(uMsg, wParam, lParam);
	}

	inline LRESULT CDocker::CDockClient::OnMouseMove(WPARAM wParam, LPARAM lParam)
	{
		return MouseMove(WM_MOUSEMOVE, wParam, lParam);
	}

	inline LRESULT CDocker::CDockClient::OnNCMouseLeave(WPARAM wParam, LPARAM lParam)
	{
		m_IsTracking = FALSE;
		CWindowDC dc(this);
		if ((0 != m_pDock) && !(m_pDock->GetDockStyle() & (DS_NO_CAPTION|DS_NO_CLOSE)) && m_pDock->IsDocked())
			DrawCloseButton(dc, m_IsOldFocusStored);

		m_IsTracking = FALSE;

		return FinalWindowProc(WM_NCMOUSELEAVE, wParam, lParam);
	}

	inline LRESULT CDocker::CDockClient::OnNCMouseMove(WPARAM wParam, LPARAM lParam)
	{
		return MouseMove(WM_NCMOUSEMOVE, wParam, lParam);
	}

	inline LRESULT CDocker::CDockClient::OnNCPaint(WPARAM wParam, LPARAM lParam)
	{
		if ((0 != m_pDock) && !(m_pDock->GetDockStyle() & DS_NO_CAPTION))
		{
			if (m_pDock->IsDocked())
			{
				DefWindowProc(WM_NCPAINT, wParam, lParam);
				DrawCaption();
				return 0;
			}
		}
		return FinalWindowProc(WM_NCPAINT, wParam, lParam);
	}

	inline LRESULT CDocker::CDockClient::OnWindowPosChanged(WPARAM wParam, LPARAM lParam)
	{
		// Reposition the View window to cover the DockClient's client area
		CRect rc = GetClientRect();
		GetView()->SetWindowPos(NULL, rc, SWP_SHOWWINDOW);

		return FinalWindowProc(WM_WINDOWPOSCHANGED, wParam, lParam);
	}

	inline void CDocker::CDockClient::PreRegisterClass(WNDCLASS& wc)
	{
		wc.lpszClassName = _T("Win32++ DockClient");
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	}

	inline void CDocker::CDockClient::PreCreate(CREATESTRUCT& cs)
	{
		DWORD dwStyle = m_pDock->GetDockStyle();
		if (dwStyle & DS_CLIENTEDGE)
			cs.dwExStyle = WS_EX_CLIENTEDGE;

#if (WINVER >= 0x0500)
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
		DragPos.ptPos = GetCursorPos();

		// Send a DragPos notification to the docker
		GetParent()->SendMessage(WM_NOTIFY, 0L, (LPARAM)&DragPos);
	}

	inline void CDocker::CDockClient::SetCaptionColors(COLORREF Foregnd1, COLORREF Backgnd1, COLORREF Foregnd2, COLORREF Backgnd2)
	{
		// Set the colors used when drawing the caption
		// m_Foregnd1 Foreground colour (focused).  m_Backgnd1 Background colour (focused)
		// m_Foregnd2 Foreground colour (not focused). m_Backgnd2 Foreground colour (not focused)
		m_Foregnd1 = Foregnd1;
		m_Backgnd1 = Backgnd1;
		m_Foregnd2 = Foregnd2;
		m_Backgnd2 = Backgnd2;
	}

	inline void CDocker::CDockClient::SetView(CWnd& wndView)
	{
		if (m_pView != &wndView)
		{
			// Hide the existing view window (if any)
			if (m_pView && m_pView->IsWindow()) m_pView->ShowWindow(SW_HIDE);

			// Assign the view window
			m_pView = &wndView;

			if (m_hWnd != 0)
			{
				// The docker is already created, so create and position the new view too
				assert(GetView());			// Use SetView in the constructor to set the view window

				if (!GetView()->IsWindow())
					GetView()->Create(this);
				else
				{
					GetView()->SetParent(this);
					GetView()->ShowWindow();
				}
			}
		}
	}

	inline LRESULT CDocker::CDockClient::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_LBUTTONUP:			return OnLButtonUp(wParam, lParam);
		case WM_MOUSEACTIVATE:		return OnMouseActivate(wParam, lParam);
		case WM_MOUSEMOVE:			return OnMouseMove(wParam, lParam);
		case WM_NCCALCSIZE:			return OnNCCalcSize(wParam, lParam);
		case WM_NCHITTEST:			return OnNCHitTest(wParam, lParam);
		case WM_NCLBUTTONDBLCLK:	return OnNCLButtonDblClk(wParam, lParam);
		case WM_NCLBUTTONDOWN:		return OnNCLButtonDown(wParam, lParam);
		case WM_NCMOUSEMOVE:		return OnNCMouseMove(wParam, lParam);
		case WM_NCPAINT:			return OnNCPaint(wParam, lParam);
		case WM_NCMOUSELEAVE:		return OnNCMouseLeave(wParam, lParam);
		case WM_NOTIFY:				return m_pDock->SendMessage(uMsg, wParam, lParam);
		case WM_WINDOWPOSCHANGED:	return OnWindowPosChanged(wParam, lParam);
		}

		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	}


	//////////////////////////////////////////////////////////////
	// Definitions for the CDockHint class nested within CDocker
	//
	inline CDocker::CDockHint::CDockHint() : m_uDockSideOld(0)
	{
	}

	inline CDocker::CDockHint::~CDockHint()
	{
	}

	inline RECT CDocker::CDockHint::CalcHintRectContainer(CDocker* pDockTarget)
	{
		// Calculate the hint window's position for container docking
		CRect rcHint = pDockTarget->GetDockClient()->GetWindowRect();
		if (pDockTarget->GetDockClient()->GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_CLIENTEDGE)
			rcHint.InflateRect(-2, -2);
		pDockTarget->ScreenToClient(rcHint);

		return rcHint;
	}

	inline RECT CDocker::CDockHint::CalcHintRectInner(CDocker* pDockTarget, CDocker* pDockDrag, UINT uDockSide)
	{
		assert(pDockTarget);
		assert(pDockDrag);

		BOOL RTL = FALSE;
#if (WINVER >= 0x0500)
		RTL = (pDockTarget->GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_LAYOUTRTL);
#endif

		// Calculate the hint window's position for inner docking
		CDockClient* pDockClient = pDockTarget->GetDockClient();
		CRect rcHint = pDockClient->GetWindowRect();

		if (pDockClient->GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_CLIENTEDGE)
			rcHint.InflateRect(-2, -2);
		pDockTarget->ScreenToClient(rcHint);

		int Width;
		CRect rcDockDrag = pDockDrag->GetWindowRect();
		CRect rcDockTarget = pDockClient->GetWindowRect();
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
			if (RTL)	rcHint.left = rcHint.right - Width;
			else		rcHint.right = rcHint.left + Width;

			break;
		case DS_DOCKED_RIGHT:
			if (RTL)	rcHint.right = rcHint.left + Width;
			else		rcHint.left = rcHint.right - Width;

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
		assert(pDockDrag);

		// Calculate the hint window's position for outer docking
		CDocker* pDockTarget = pDockDrag->GetDockAncestor();
		CRect rcHint = pDockTarget->GetViewRect();
		if (pDockTarget->GetDockClient()->GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_CLIENTEDGE)
			rcHint.InflateRect(-2, -2);

		int Width;
		CRect rcDockDrag = pDockDrag->GetWindowRect();

		BOOL RTL = FALSE;
#ifdef WS_EX_LAYOUTRTL
		RTL = (pDockTarget->GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_LAYOUTRTL);
#endif

		int BarWidth = pDockDrag->GetBarWidth();
		CRect rcDockClient = pDockTarget->GetDockClient()->GetClientRect();

		// Limit the docked size to half the parent's size if it won't fit inside parent
		if ((uDockSide == DS_DOCKED_LEFTMOST) || (uDockSide  == DS_DOCKED_RIGHTMOST))
		{
			Width = rcDockDrag.Width();
			if (Width >= rcDockClient.Width() - BarWidth)
				Width = MAX(rcDockClient.Width()/2 - BarWidth, BarWidth);
		}
		else
		{
			Width = rcDockDrag.Height();
			if (Width >= rcDockClient.Height() - BarWidth)
				Width = MAX(rcDockClient.Height()/2 - BarWidth, BarWidth);
		}
		switch (uDockSide)
		{
		case DS_DOCKED_LEFTMOST:
			if(RTL)	rcHint.left = rcHint.right - Width;
			else	rcHint.right = rcHint.left + Width;

			break;
		case DS_DOCKED_RIGHTMOST:
			if(RTL)	rcHint.right = rcHint.left + Width;
			else	rcHint.left = rcHint.right - Width;

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
			pDockTarget->RedrawWindow( NULL, NULL, RDW_NOERASE | RDW_UPDATENOW );
			pDockDrag->RedrawWindow();
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

			// Save the Dock window's blue tinted bitmap
			CClientDC dcDesktop(NULL);
			CMemDC dcMem(&dcDesktop);
			CRect rcBitmap = rcHint;
			CRect rcTarget = rcHint;
			pDockTarget->ClientToScreen(rcTarget);

			m_bmBlueTint.DeleteObject();
			m_bmBlueTint.CreateCompatibleBitmap(&dcDesktop, rcBitmap.Width(), rcBitmap.Height());
			CBitmap* pOldBitmap = dcMem.SelectObject(&m_bmBlueTint);
			dcMem.BitBlt(0, 0, rcBitmap.Width(), rcBitmap.Height(), &dcDesktop, rcTarget.left, rcTarget.top, SRCCOPY);
			dcMem.SelectObject(pOldBitmap);
			m_bmBlueTint.TintBitmap(-64, -24, +128);

			// Create the Hint window
			if (!IsWindow())
			{
				Create(pDockTarget);
			}

			// Adjust hint shape for container in container docking
			if ((uDockSide & DS_DOCKED_CONTAINER) && rcHint.Height() > 50)
			{
				CRgn Rgn;
				Rgn.CreateRectRgn(0, 0, rcHint.Width(), rcHint.Height() -25);
				assert(Rgn.GetHandle());
				CRgn Rgn2;
				Rgn2.CreateRectRgn(5, rcHint.Height() -25, 60, rcHint.Height());
				Rgn.CombineRgn(&Rgn2, RGN_OR);
				SetWindowRgn(&Rgn, FALSE);
			}

			pDockTarget->ClientToScreen(rcHint);
			SetWindowPos(NULL, rcHint, SWP_SHOWWINDOW|SWP_NOZORDER|SWP_NOACTIVATE);
		}
	}

	inline void CDocker::CDockHint::OnDraw(CDC* pDC)
	{
		// Display the blue tinted bitmap
		CRect rc = GetClientRect();
		CMemDC MemDC(pDC);
		MemDC.SelectObject(&m_bmBlueTint);
		pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &MemDC, 0, 0, SRCCOPY);
	}

	inline void CDocker::CDockHint::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_POPUP;

		// WS_EX_TOOLWINDOW prevents the window being displayed on the taskbar
		cs.dwExStyle = WS_EX_TOOLWINDOW;

		cs.lpszClass = _T("Win32++ DockHint");
	}


	////////////////////////////////////////////////////////////////
	// Definitions for the CTargetCentre class nested within CDocker
	//
	inline CDocker::CTargetCentre::CTargetCentre() : m_IsOverContainer(FALSE), m_pOldDockTarget(0)
	{
	}

	inline CDocker::CTargetCentre::~CTargetCentre()
	{
	}

	inline void CDocker::CTargetCentre::OnDraw(CDC* pDC)
	{
		// Load the target bitmaps
		CBitmap bmCentre(IDW_SDCENTER);
		CBitmap bmLeft(IDW_SDLEFT);
		CBitmap bmRight(IDW_SDRIGHT);
		CBitmap bmTop(IDW_SDTOP);
		CBitmap bmBottom(IDW_SDBOTTOM);

		// Grey out invalid dock targets
		DWORD dwStyle = m_pOldDockTarget->GetDockStyle();
		if (dwStyle & DS_NO_DOCKCHILD_LEFT)  bmLeft.TintBitmap(150, 150, 150);
		if (dwStyle & DS_NO_DOCKCHILD_TOP) 	 bmTop.TintBitmap(150, 150, 150);
		if (dwStyle & DS_NO_DOCKCHILD_RIGHT) bmRight.TintBitmap(150, 150, 150);
		if (dwStyle & DS_NO_DOCKCHILD_BOTTOM) bmBottom.TintBitmap(150, 150, 150);

		// Draw the dock targets
		if (bmCentre.GetHandle())	pDC->DrawBitmap(0, 0, 88, 88, bmCentre, RGB(255,0,255));
		else TRACE("Missing docking resource: Target Centre\n");

		if (bmLeft.GetHandle()) pDC->DrawBitmap(0, 29, 31, 29, bmLeft, RGB(255,0,255));
		else TRACE("Missing docking resource: Target Left\n");

		if (bmTop.GetHandle()) pDC->DrawBitmap(29, 0, 29, 31, bmTop, RGB(255,0,255));
		else TRACE("Missing docking resource: Target Top\n");

		if (bmRight.GetHandle()) pDC->DrawBitmap(55, 29, 31, 29, bmRight, RGB(255,0,255));
		else TRACE("Missing docking resource: Target Right\n");

		if (bmBottom.GetHandle()) pDC->DrawBitmap(29, 55, 29, 31, bmBottom, RGB(255,0,255));
		else TRACE("Missing docking resource: Target Bottom\n");

		if (IsOverContainer())
		{
			CBitmap bmMiddle(IDW_SDMIDDLE);
			pDC->DrawBitmap(31, 31, 25, 26, bmMiddle, RGB(255,0,255));
		}
	}

	inline int CDocker::CTargetCentre::OnCreate(LPCREATESTRUCT pcs)
	{
		UNREFERENCED_PARAMETER(pcs);

		// Use a region to create an irregularly shapped window
		POINT ptArray[16] = { {0,29}, {22, 29}, {29, 22}, {29, 0},
		                      {58, 0}, {58, 22}, {64, 29}, {87, 29},
		                      {87, 58}, {64, 58}, {58, 64}, {58, 87},
		                      {29, 87}, {29, 64}, {23, 58}, {0, 58} };

		CRgn rgnPoly;
		rgnPoly.CreatePolygonRgn(ptArray, 16, WINDING);
		SetWindowRgn(&rgnPoly, FALSE);
		return 0;
	}

	inline BOOL CDocker::CTargetCentre::CheckTarget(LPDRAGPOS pDragPos)
	{
		CDocker* pDockDrag = static_cast<CDocker*>(FromHandlePermanent(pDragPos->hdr.hwndFrom));
		if (NULL == pDockDrag) return FALSE;

		CDocker* pDockTarget = pDockDrag->GetDockFromPoint(pDragPos->ptPos);
		if (NULL == pDockTarget) return FALSE;

		if (!IsWindow())	Create();
		m_IsOverContainer = (dynamic_cast<CDockContainer*>(pDockTarget->GetView()) != NULL);

		// Redraw the target if the dock target changes
		if (m_pOldDockTarget != pDockTarget)	Invalidate();
		m_pOldDockTarget = pDockTarget;

		int cxImage = 88;
		int cyImage = 88;

		CRect rcTarget = pDockTarget->GetDockClient()->GetWindowRect();
		int xMid = rcTarget.left + (rcTarget.Width() - cxImage)/2;
		int yMid = rcTarget.top + (rcTarget.Height() - cyImage)/2;
		SetWindowPos(&wndTopMost, xMid, yMid, cxImage, cyImage, SWP_NOACTIVATE|SWP_SHOWWINDOW);

		// Create the docking zone rectangles
		CPoint pt = pDragPos->ptPos;
		ScreenToClient(pt);
		CRect rcLeft(0, 29, 31, 58);
		CRect rcTop(29, 0, 58, 31);
		CRect rcRight(55, 29, 87, 58);
		CRect rcBottom(29, 55, 58, 87);
		CRect rcMiddle(31, 31, 56, 57);

		// Test if our cursor is in one of the docking zones
		if ((rcLeft.PtInRect(pt)) && !(pDockTarget->GetDockStyle() & DS_NO_DOCKCHILD_LEFT))
		{
			pDockDrag->m_IsBlockMove = TRUE;

			pDockTarget->GetDockHint()->DisplayHint(pDockTarget, pDockDrag, DS_DOCKED_LEFT);
			pDockDrag->m_dwDockZone = DS_DOCKED_LEFT;

			return TRUE;
		}
		else if ((rcTop.PtInRect(pt)) && !(pDockTarget->GetDockStyle() & DS_NO_DOCKCHILD_TOP))
		{
			pDockDrag->m_IsBlockMove = TRUE;
			pDockTarget->GetDockHint()->DisplayHint(pDockTarget, pDockDrag, DS_DOCKED_TOP);
			pDockDrag->m_dwDockZone = DS_DOCKED_TOP;
			return TRUE;
		}
		else if ((rcRight.PtInRect(pt)) && !(pDockTarget->GetDockStyle() & DS_NO_DOCKCHILD_RIGHT))
		{
			pDockDrag->m_IsBlockMove = TRUE;

			pDockTarget->GetDockHint()->DisplayHint(pDockTarget, pDockDrag, DS_DOCKED_RIGHT);
			pDockDrag->m_dwDockZone = DS_DOCKED_RIGHT;

			return TRUE;
		}
		else if ((rcBottom.PtInRect(pt)) && !(pDockTarget->GetDockStyle() & DS_NO_DOCKCHILD_BOTTOM))
		{
			pDockDrag->m_IsBlockMove = TRUE;
			pDockTarget->GetDockHint()->DisplayHint(pDockTarget, pDockDrag, DS_DOCKED_BOTTOM);
			pDockDrag->m_dwDockZone = DS_DOCKED_BOTTOM;
			return TRUE;
		}
		else if ((rcMiddle.PtInRect(pt)) && (IsOverContainer()))
		{
			pDockDrag->m_IsBlockMove = TRUE;
			pDockTarget->GetDockHint()->DisplayHint(pDockTarget, pDockDrag, DS_DOCKED_CONTAINER);
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
	}

	inline void CDocker::CTarget::OnDraw(CDC* pDC)
	{
		BITMAP bm = m_bmImage.GetBitmapData();
		int cxImage = bm.bmWidth;
		int cyImage = bm.bmHeight;

		if (m_bmImage != 0)
			pDC->DrawBitmap(0, 0, cxImage, cyImage, m_bmImage, RGB(255,0,255));
		else
			TRACE("Missing docking resource\n");
	}

	inline void CDocker::CTarget::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_POPUP;
		cs.dwExStyle = WS_EX_TOPMOST|WS_EX_TOOLWINDOW;
		cs.lpszClass = _T("Win32++ DockTargeting");
	}


	////////////////////////////////////////////////////////////////
	// Definitions for the CTargetLeft class nested within CDocker
	//
	inline BOOL CDocker::CTargetLeft::CheckTarget(LPDRAGPOS pDragPos)
	{
		CDocker* pDockDrag = static_cast<CDocker*>(FromHandlePermanent(pDragPos->hdr.hwndFrom));
		if (NULL == pDockDrag) return FALSE;

		CPoint pt = pDragPos->ptPos;
		CDocker* pDockTarget = pDockDrag->GetDockFromPoint(pt)->GetTopmostDocker();
		if (pDockTarget != pDockDrag->GetDockAncestor())
		{
			Destroy();
			return FALSE;
		}

		if (pDockTarget->GetDockStyle() & DS_NO_DOCKCHILD_LEFT)
			return FALSE;

		BITMAP bm = m_bmImage.GetBitmapData();
		int cxImage = bm.bmWidth;
		int cyImage = bm.bmHeight;

		if (!IsWindow())
		{
			Create();
			CRect rc = pDockTarget->GetViewRect();
			pDockTarget->ClientToScreen(rc);
			int yMid = rc.top + (rc.Height() - cyImage)/2;
			SetWindowPos(&wndTopMost, rc.left + 8, yMid, cxImage, cyImage, SWP_NOACTIVATE|SWP_SHOWWINDOW);
		}

		CRect rcLeft(0, 0, cxImage, cyImage);
		ScreenToClient(pt);

		// Test if our cursor is in one of the docking zones
		if (rcLeft.PtInRect(pt))
		{
			pDockDrag->m_IsBlockMove = TRUE;
			pDockTarget->GetDockHint()->DisplayHint(pDockTarget, pDockDrag, DS_DOCKED_LEFTMOST);
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
		CDocker* pDockDrag = static_cast<CDocker*>(FromHandlePermanent(pDragPos->hdr.hwndFrom));
		if (NULL == pDockDrag) return FALSE;

		CPoint pt = pDragPos->ptPos;
		CDocker* pDockTarget = pDockDrag->GetDockFromPoint(pt)->GetTopmostDocker();
		if (pDockTarget != pDockDrag->GetDockAncestor())
		{
			Destroy();
			return FALSE;
		}

		if (pDockTarget->GetDockStyle() & DS_NO_DOCKCHILD_TOP)
			return FALSE;

		BITMAP bm = m_bmImage.GetBitmapData();
		int cxImage = bm.bmWidth;
		int cyImage = bm.bmHeight;

		if (!IsWindow())
		{
			Create();
			CRect rc = pDockTarget->GetViewRect();
			pDockTarget->ClientToScreen(rc);
			int xMid = rc.left + (rc.Width() - cxImage)/2;
			SetWindowPos(&wndTopMost, xMid, rc.top + 8, cxImage, cyImage, SWP_NOACTIVATE|SWP_SHOWWINDOW);
		}

		CRect rcTop(0, 0, cxImage, cyImage);
		ScreenToClient(pt);

		// Test if our cursor is in one of the docking zones
		if (rcTop.PtInRect(pt))
		{
			pDockDrag->m_IsBlockMove = TRUE;
			pDockTarget->GetDockHint()->DisplayHint(pDockTarget, pDockDrag, DS_DOCKED_TOPMOST);
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
		CDocker* pDockDrag = static_cast<CDocker*>(FromHandlePermanent(pDragPos->hdr.hwndFrom));
		if (NULL == pDockDrag) return FALSE;

		CPoint pt = pDragPos->ptPos;
		CDocker* pDockTarget = pDockDrag->GetDockFromPoint(pt)->GetTopmostDocker();
		if (pDockTarget != pDockDrag->GetDockAncestor())
		{
			Destroy();
			return FALSE;
		}

		if (pDockTarget->GetDockStyle() & DS_NO_DOCKCHILD_RIGHT)
			return FALSE;

		BITMAP bm = m_bmImage.GetBitmapData();
		int cxImage = bm.bmWidth;
		int cyImage = bm.bmHeight;

		if (!IsWindow())
		{
			Create();
			CRect rc = pDockTarget->GetViewRect();
			pDockTarget->ClientToScreen(rc);
			int yMid = rc.top + (rc.Height() - cyImage)/2;
			SetWindowPos(&wndTopMost, rc.right - 8 - cxImage, yMid, cxImage, cyImage, SWP_NOACTIVATE|SWP_SHOWWINDOW);
		}

		CRect rcRight(0, 0, cxImage, cyImage);
		ScreenToClient(pt);

		// Test if our cursor is in one of the docking zones
		if (rcRight.PtInRect(pt))
		{
			pDockDrag->m_IsBlockMove = TRUE;
			pDockTarget->GetDockHint()->DisplayHint(pDockTarget, pDockDrag, DS_DOCKED_RIGHTMOST);
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
		CDocker* pDockDrag = static_cast<CDocker*>(FromHandlePermanent(pDragPos->hdr.hwndFrom));
		if (NULL == pDockDrag) return FALSE;

		CPoint pt = pDragPos->ptPos;
		CDocker* pDockTarget = pDockDrag->GetDockFromPoint(pt)->GetTopmostDocker();
		if (pDockTarget != pDockDrag->GetDockAncestor())
		{
			Destroy();
			return FALSE;
		}

		if (pDockTarget->GetDockStyle() & DS_NO_DOCKCHILD_BOTTOM)
			return FALSE;

		BITMAP bm = m_bmImage.GetBitmapData();
		int cxImage = bm.bmWidth;
		int cyImage = bm.bmHeight;

		if (!IsWindow())
		{
			Create();
			CRect rc = pDockTarget->GetViewRect();
			pDockTarget->ClientToScreen(rc);
			int xMid = rc.left + (rc.Width() - cxImage)/2;
			SetWindowPos(&wndTopMost, xMid, rc.bottom - 8 - cyImage, cxImage, cyImage, SWP_NOACTIVATE|SWP_SHOWWINDOW);
		}
		CRect rcBottom(0, 0, cxImage, cyImage);
		ScreenToClient(pt);

		// Test if our cursor is in one of the docking zones
		if (rcBottom.PtInRect(pt))
		{
			pDockDrag->m_IsBlockMove = TRUE;
			pDockTarget->GetDockHint()->DisplayHint(pDockTarget, pDockDrag, DS_DOCKED_BOTTOMMOST);
			pDockDrag->m_dwDockZone = DS_DOCKED_BOTTOMMOST;
			return TRUE;
		}

		return FALSE;
	}


	/////////////////////////////////////////
	// Definitions for the CDocker class
	//
	inline CDocker::CDocker() : m_pDockParent(NULL), m_IsBlockMove(FALSE), m_IsUndocking(FALSE), m_IsClosing(FALSE), 
		            m_IsDragging(FALSE), m_IsDragAutoResize(TRUE), m_DockStartSize(0), m_nDockID(0),
					m_nRedrawCount(0), m_NCHeight(0), m_dwDockZone(0), m_DockSizeRatio(1.0), m_DockStyle(0)
	{
		// Assume this docker is the DockAncestor for now.
		m_pDockAncestor = this;
		GetAllDockers()->push_back(this);
	}

	inline CDocker::~CDocker()
	{
		GetDockBar()->Destroy();

		std::vector<DockPtr>::iterator iter;
		if (GetDockAncestor() == this)
		{
			// Destroy all dock descendants of this dock ancestor
			for (iter = GetAllDockChildren()->begin(); iter != GetAllDockChildren()->end(); ++iter)
			{
				(*iter)->Destroy();
			}
		}
	}

	inline CDocker* CDocker::AddDockedChild(CDocker* pDocker, DWORD dwDockStyle, int DockSize, int nDockID /* = 0*/)
	// This function creates the docker, and adds it to the docker hierarchy as docked
	{
		// Create the docker window as a child of the frame window.
		// This permanently sets the frame window as the docker window's owner,
		// even when its parent is subsequently changed.

		assert(pDocker);

		// Store the Docker's pointer in the DockAncestor's vector for later deletion
		GetAllDockChildren()->push_back(DockPtr(pDocker));
		GetDockAncestor()->GetAllDockers()->push_back(pDocker);

		pDocker->SetDockStyle(dwDockStyle);
		pDocker->m_nDockID = nDockID;
		pDocker->m_pDockAncestor = GetDockAncestor();
		pDocker->m_pDockParent = this;
		pDocker->SetDockSize(DockSize);
		CWnd* pFrame = GetDockAncestor()->GetAncestor();
		pDocker->Create(pFrame);
		pDocker->SetParent(this);

		// Dock the docker window
		if (dwDockStyle & DS_DOCKED_CONTAINER)
			DockInContainer(pDocker, dwDockStyle);
		else
			Dock(pDocker, dwDockStyle);

		// Issue TRACE warnings for any missing resources
		HMODULE hMod= GetApp()->GetResourceHandle();

		if (!(dwDockStyle & DS_NO_RESIZE))
		{
			if (!FindResource(hMod, MAKEINTRESOURCE(IDW_SPLITH), RT_GROUP_CURSOR))
				TRACE("**WARNING** Horizontal cursor resource missing\n");
			if (!FindResource(hMod, MAKEINTRESOURCE(IDW_SPLITV), RT_GROUP_CURSOR))
				TRACE("**WARNING** Vertical cursor resource missing\n");
		}

		if (!(dwDockStyle & DS_NO_UNDOCK))
		{
			if (!FindResource(hMod, MAKEINTRESOURCE(IDW_SDCENTER), RT_BITMAP))
				TRACE("**WARNING** Docking center bitmap resource missing\n");
			if (!FindResource(hMod, MAKEINTRESOURCE(IDW_SDLEFT), RT_BITMAP))
				TRACE("**WARNING** Docking left bitmap resource missing\n");
			if (!FindResource(hMod, MAKEINTRESOURCE(IDW_SDRIGHT), RT_BITMAP))
				TRACE("**WARNING** Docking right bitmap resource missing\n");
			if (!FindResource(hMod, MAKEINTRESOURCE(IDW_SDTOP), RT_BITMAP))
				TRACE("**WARNING** Docking top bitmap resource missing\n");
			if (!FindResource(hMod, MAKEINTRESOURCE(IDW_SDBOTTOM), RT_BITMAP))
				TRACE("**WARNING** Docking center bottom resource missing\n");
		}

		if (dwDockStyle & DS_DOCKED_CONTAINER)
		{
			if (!FindResource(hMod, MAKEINTRESOURCE(IDW_SDMIDDLE), RT_BITMAP))
				TRACE("**WARNING** Docking container bitmap resource missing\n");
		}

		return pDocker;
	}

	inline CDocker* CDocker::AddUndockedChild(CDocker* pDocker, DWORD dwDockStyle, int DockSize, const RECT& rc, int nDockID /* = 0*/)
	// This function creates the docker, and adds it to the docker hierarchy as undocked
	{
		assert(pDocker);

		// Store the Docker's pointer in the DockAncestor's vector for later deletion
		GetAllDockChildren()->push_back(DockPtr(pDocker));
		GetDockAncestor()->GetAllDockers()->push_back(pDocker);

		pDocker->SetDockSize(DockSize);
		pDocker->SetDockStyle(dwDockStyle & 0XFFFFFF0);
		pDocker->m_nDockID = nDockID;
		pDocker->m_pDockAncestor = GetDockAncestor();

		// Initially create the as a child window of the frame
		// This makes the frame window the owner of our docker
		CWnd* pFrame = GetDockAncestor()->GetAncestor();
		pDocker->Create(pFrame);
		pDocker->SetParent(this);

		// Change the Docker to a POPUP window
		DWORD dwStyle = WS_POPUP| WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_VISIBLE;
		pDocker->SetWindowLongPtr(GWL_STYLE, dwStyle);
		pDocker->SetRedraw(FALSE);
		pDocker->SetParent(0);
		pDocker->SetWindowPos(0, rc, SWP_SHOWWINDOW|SWP_FRAMECHANGED);
		pDocker->SetRedraw(TRUE);
		pDocker->RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
		pDocker->SetWindowText(pDocker->GetCaption().c_str());

		return pDocker;
	}

	inline void CDocker::CheckAllTargets(LPDRAGPOS pDragPos)
	// Calls CheckTarget for each possible target zone
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
							CDocker* pDockDrag = static_cast<CDocker*>(FromHandlePermanent(nmhdr.hwndFrom));
							if (pDockDrag)
							{
								if (pDockDrag->m_IsBlockMove)
									pDockDrag->RedrawWindow(0, 0, RDW_FRAME|RDW_INVALIDATE);

								GetDockHint()->Destroy();
								pDockDrag->m_dwDockZone = 0;
								pDockDrag->m_IsBlockMove = FALSE;
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
		std::vector<DockPtr>::iterator iter;

		for (iter = GetAllDockChildren()->begin(); iter != GetAllDockChildren()->end(); ++iter)
		{
			if (GetDockAncestor() != (*iter)->m_pDockAncestor)
			{
				TRACE("Invalid Dock Ancestor\n");
				bResult = FALSE;
			}
		}

		// Check presence of dock parent
		for (iter = GetAllDockChildren()->begin(); iter != GetAllDockChildren()->end(); ++iter)
		{
			if ((*iter)->IsUndocked() && (*iter)->m_pDockParent != 0)
			{
				TRACE("Error: Undocked dockers should not have a dock parent\n");
					bResult = FALSE;
			}

			if ((*iter)->IsDocked() && (*iter)->m_pDockParent == 0)
			{
				TRACE("Error: Docked dockers should have a dock parent\n");
					bResult = FALSE;
			}
		}

		// Check dock parent/child relationship
		for (iter = GetAllDockChildren()->begin(); iter != GetAllDockChildren()->end(); ++iter)
		{
			std::vector<CDocker*>::iterator iterChild;
			for (iterChild = (*iter)->GetDockChildren()->begin(); iterChild != (*iter)->GetDockChildren()->end(); ++iterChild)
			{
				if ((*iterChild)->m_pDockParent != (*iter).get())
				{
					TRACE("Error: Docking parent/Child information mismatch\n");
					bResult = FALSE;
				}
				if ((*iterChild)->GetParent() != (*iter).get())
				{
					TRACE("Error: Incorrect windows child parent relationship\n");
					bResult = FALSE;
				}
			}
		}

		// Check dock parent chain
		for (iter = GetAllDockChildren()->begin(); iter != GetAllDockChildren()->end(); ++iter)
		{
			CDocker* pDockTopLevel = (*iter)->GetTopmostDocker();
			if (pDockTopLevel->IsDocked())
				TRACE("Error: Top level parent should be undocked\n");
		}

		return bResult;
	}

	inline void CDocker::Close()
	{
		if (IsDocked())
		{
			// Undock the docker and hide it
			Hide();
		}

		// Destroy the docker and its children
		Destroy();
	}

	inline void CDocker::CloseAllDockers()
	{
		assert(this == GetDockAncestor());	// Must call CloseAllDockers from the DockAncestor

		std::vector <DockPtr>::iterator v;

		if (IsWindow()) SetRedraw(FALSE);
		for (v = GetAllDockChildren()->begin(); v != GetAllDockChildren()->end(); ++v)
		{
			// The CDocker is destroyed when the window is destroyed
			(*v)->m_IsClosing = TRUE;
			(*v)->Destroy();	// Destroy the window
		}

		GetDockChildren()->clear();
		if (IsWindow()) SetRedraw(TRUE);

		// Delete any child containers this container might have
		if (GetContainer())
		{
			std::vector<ContainerInfo> AllContainers = *GetContainer()->GetAllContainers();
			std::vector<ContainerInfo>::iterator iter;
			for (iter = AllContainers.begin(); iter != AllContainers.end(); ++iter)
			{
				if (GetContainer() != (*iter).pContainer)
					GetContainer()->RemoveContainer((*iter).pContainer);
			}
		}

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

	inline void CDocker::Dock(CDocker* pDocker, UINT DockStyle)
	// Docks the specified docker inside this docker
	{
		assert(pDocker);

		pDocker->m_pDockParent = this;
		pDocker->m_IsBlockMove = FALSE;
		pDocker->SetDockStyle(DockStyle);
		m_vDockChildren.push_back(pDocker);
		pDocker->ConvertToChild(m_hWnd);

		// Limit the docked size to half the parent's size if it won't fit inside parent
		if (((DockStyle & 0xF)  == DS_DOCKED_LEFT) || ((DockStyle &0xF)  == DS_DOCKED_RIGHT))
		{
			int Width = GetDockClient()->GetWindowRect().Width();
			int BarWidth = pDocker->GetBarWidth();
			if (pDocker->m_DockStartSize >= (Width - BarWidth))
				pDocker->SetDockSize(MAX(Width/2 - BarWidth, BarWidth));

			pDocker->m_DockSizeRatio = ((double)pDocker->m_DockStartSize) / (double)GetWindowRect().Width();
		}
		else
		{
			int Height = GetDockClient()->GetWindowRect().Height();
			int BarWidth = pDocker->GetBarWidth();
			if (pDocker->m_DockStartSize >= (Height - BarWidth))
				pDocker->SetDockSize(MAX(Height/2 - BarWidth, BarWidth));

			pDocker->m_DockSizeRatio = ((double)pDocker->m_DockStartSize) / (double)GetWindowRect().Height();
		}

		// Redraw the docked windows
		if (GetAncestor()->IsWindowVisible())
		{
			GetAncestor()->SetForegroundWindow();
		
			pDocker->GetView()->SetFocus();
			GetTopmostDocker()->SetRedraw(FALSE);
			RecalcDockLayout();
			GetTopmostDocker()->SetRedraw(TRUE);
			GetTopmostDocker()->RedrawWindow();
		}
	}

	inline void CDocker::DockInContainer(CDocker* pDock, DWORD dwDockStyle)
	// Add a container to an existing container
	{
		if ((dwDockStyle & DS_DOCKED_CONTAINER) && (static_cast<CDockContainer*>(pDock->GetView())))
		{
			assert(dynamic_cast<CDockContainer*>(pDock->GetView()));
			
			// Transfer any dock children to this docker
			pDock->MoveDockInContainerChildren(this);

			// Transfer container children to the target container
			CDockContainer* pContainer = static_cast<CDockContainer*>(GetView());
			CDockContainer* pContainerSource = static_cast<CDockContainer*>(pDock->GetView());

			std::vector<ContainerInfo>::reverse_iterator riter;
			std::vector<ContainerInfo> AllContainers = *pContainerSource->GetAllContainers();
			for (riter = AllContainers.rbegin(); riter < AllContainers.rend(); ++riter)
			{
				CDockContainer* pContainerChild = (*riter).pContainer;
				if (pContainerChild != pContainerSource)
				{
					// Remove child container from pContainerSource
					pContainerChild->ShowWindow(SW_HIDE);
					pContainerSource->RemoveContainer(pContainerChild);
					CDocker* pDockChild = GetDockFromView(pContainerChild);
					pDockChild->SetParent(this);
					pDockChild->m_pDockParent = this;
				}
			}

			pDock->m_pDockParent = this;
			pDock->m_IsBlockMove = FALSE;
			pDock->ShowWindow(SW_HIDE);
			pDock->SetWindowLongPtr(GWL_STYLE, WS_CHILD);
			pDock->SetDockStyle(dwDockStyle);
			pDock->SetParent(this);
			pDock->GetDockBar()->SetParent(GetDockAncestor());
		
			// Insert the containers in reverse order
			for (riter = AllContainers.rbegin(); riter < AllContainers.rend(); ++riter)
			{
				pContainer->AddContainer( (*riter).pContainer, TRUE);
			}
		}
	}

	inline void CDocker::DockOuter(CDocker* pDocker, DWORD dwDockStyle)
	// Docks the specified docker inside the dock ancestor
	{
		assert(pDocker);

		pDocker->m_pDockParent = GetDockAncestor();

		DWORD OuterDocking = dwDockStyle & 0xF0000;
		DWORD DockSide = OuterDocking / 0x10000;
		dwDockStyle &= 0xFFF0FFFF;
		dwDockStyle |= DockSide;

		// Set the dock styles
		DWORD dwStyle = WS_CHILD | WS_VISIBLE;
		pDocker->m_IsBlockMove = FALSE;
		pDocker->SetWindowLongPtr(GWL_STYLE, dwStyle);
		pDocker->ShowWindow(SW_HIDE);
		pDocker->SetDockStyle(dwDockStyle);

		// Set the docking relationships
		std::vector<CDocker*>::iterator iter = GetDockAncestor()->m_vDockChildren.begin();
		GetDockAncestor()->m_vDockChildren.insert(iter, pDocker);
		pDocker->SetParent(GetDockAncestor());
		pDocker->GetDockBar()->SetParent(GetDockAncestor());

		// Limit the docked size to half the parent's size if it won't fit inside parent
		if (((dwDockStyle & 0xF)  == DS_DOCKED_LEFT) || ((dwDockStyle &0xF)  == DS_DOCKED_RIGHT))
		{
			int Width = GetDockAncestor()->GetDockClient()->GetWindowRect().Width();
			int BarWidth = pDocker->GetBarWidth();
			if (pDocker->m_DockStartSize >= (Width - BarWidth))
				pDocker->SetDockSize(MAX(Width/2 - BarWidth, BarWidth));

			pDocker->m_DockSizeRatio = ((double)pDocker->m_DockStartSize) / (double)GetDockAncestor()->GetWindowRect().Width();
		}
		else
		{
			int Height = GetDockAncestor()->GetDockClient()->GetWindowRect().Height();
			int BarWidth = pDocker->GetBarWidth();
			if (pDocker->m_DockStartSize >= (Height - BarWidth))
				pDocker->SetDockSize(MAX(Height/2 - BarWidth, BarWidth));

			pDocker->m_DockSizeRatio = ((double)pDocker->m_DockStartSize) / (double)GetDockAncestor()->GetWindowRect().Height();
		}

		// Redraw the docked windows
		GetAncestor()->SetFocus();
		pDocker->GetView()->SetFocus();
		RecalcDockLayout();
	}

	inline void CDocker::DrawAllCaptions()
	{
		std::vector<DockPtr>::iterator iter;
		for (iter = GetAllDockChildren()->begin(); iter != GetAllDockChildren()->end(); ++iter)
		{
			if ((*iter)->IsDocked())
				(*iter)->GetDockClient()->DrawCaption();
		}
	}

	inline void CDocker::DrawHashBar(HWND hBar, POINT Pos)
	// Draws a hashed bar while the splitter bar is being dragged
	{
		CDocker* pDock = (static_cast<CDockBar*>(FromHandlePermanent(hBar)))->GetDock();
		if (NULL == pDock) return;

		BOOL bVertical = ((pDock->GetDockStyle() & 0xF) == DS_DOCKED_LEFT) || ((pDock->GetDockStyle() & 0xF) == DS_DOCKED_RIGHT);
		CClientDC dcBar(this);

		WORD HashPattern[] = {0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA};
		CBitmap bmHash;
		CBrush brDithered;
		bmHash.CreateBitmap(8, 8, 1, 1, HashPattern);
		brDithered.CreatePatternBrush(&bmHash);
		dcBar.SelectObject(&brDithered);

		CRect rc = FromHandle(hBar)->GetWindowRect();
		ScreenToClient(rc);
		int cx = rc.Width();
		int cy = rc.Height();
		int BarWidth = pDock->GetDockBar()->GetWidth();

		if (bVertical)
			dcBar.PatBlt(Pos.x - BarWidth/2, rc.top, BarWidth, cy, PATINVERT);
		else
			dcBar.PatBlt(rc.left, Pos.y - BarWidth/2, cx, BarWidth, PATINVERT);
	}

	inline CDockContainer* CDocker::GetContainer() const
	{
		// returns NULL if not a CDockContainer*
		return dynamic_cast<CDockContainer*>(GetView());
	}

	inline CDocker* CDocker::GetActiveDocker() const
	// Returns the docker whose child window has focus
	{
		CWnd* pWnd = GetFocus();
		CDocker* pDock= NULL;
		while (pWnd && (pDock == NULL))
		{
			if (IsRelated(pWnd))
				pDock = static_cast<CDocker*>(pWnd);

			pWnd = pWnd->GetParent();
		}

		return pDock;
	}

	inline CDocker* CDocker::GetDockAncestor() const
	// The GetDockAncestor function retrieves the pointer to the
	//  ancestor (root docker parent) of the Docker.
	{
		return m_pDockAncestor;
	}

	inline CDocker* CDocker::GetDockFromPoint(POINT pt) const
	// Retrieves the Docker whose view window contains the specified point.
	// Used when dragging undocked dockers over other dockers to provide
	// the docker which needs to display the dock targets and dock hints.
	{
		// Step 1: Find the top level Docker the point is over
		// Start at the DockAncestor's Ancestor and work up the Z order
		CDocker* pDockTop = NULL;
		CWnd* pAncestor = GetDockAncestor()->GetAncestor();
		CWnd* pWnd = pAncestor;
		do
		{
			if (IsRelated(pWnd) || pWnd == pAncestor)
			{
				CDocker* pDockTest;
				if (pWnd == pAncestor)
					pDockTest = GetDockAncestor();
				else
					pDockTest = static_cast<CDocker*>(pWnd);

				CRect rc = pDockTest->GetClientRect();
				pDockTest->ClientToScreen(rc);
				if ((this != pDockTest) && rc.PtInRect(pt))
				{
					pDockTop = pDockTest;
				}
			}
			pWnd = pWnd->GetWindow(GW_HWNDPREV);

		} while (pWnd != pWnd->GetWindow(GW_HWNDFIRST));

		// Step 2: Find the docker child whose view window has the point
		CDocker* pDockTarget = NULL;
		if (pDockTop)
		{
			CDocker* pDockParent = pDockTop;
			CDocker* pDockTest = pDockParent;

			while (IsRelated(pDockTest))
			{
				pDockParent = pDockTest;
				CPoint ptLocal = pt;
				pDockParent->ScreenToClient(ptLocal);
				pDockTest = static_cast<CDocker*>(pDockParent->ChildWindowFromPoint(ptLocal));
				assert (pDockTest != pDockParent);
			}

			CRect rc = pDockParent->GetDockClient()->GetWindowRect();
			if (rc.PtInRect(pt)) pDockTarget = pDockParent;
		}

		return pDockTarget;
	}

	inline CDocker* CDocker::GetDockFromID(int n_DockID) const
	{
		std::vector<DockPtr>::iterator v;

		if (GetDockAncestor())
		{
			for (v = GetAllDockChildren()->begin(); v != GetAllDockChildren()->end(); ++v)
			{
				if (n_DockID == (*v)->GetDockID())
					return (*v).get();
			}
		}

		return 0;
	}

	inline CDocker* CDocker::GetDockFromView(CWnd* pView) const
	{
		CDocker* pDock = 0;
		std::vector<DockPtr>::iterator iter;
		for (iter = GetAllDockChildren()->begin(); iter != GetAllDockChildren()->end(); ++iter)
		{
			if ((*iter)->GetView() == pView)
				pDock = (*iter).get();
		}

		if (GetDockAncestor()->GetView() == pView)
			pDock = GetDockAncestor();

		return pDock;
	}

	inline int CDocker::GetDockSize() const
	{
		// Returns the size of the docker to be used if it is redocked
		// Note: This function returns 0 if the docker has the DS_DOCKED_CONTAINER style

		CRect rcParent;
		if (GetDockParent())
			rcParent = GetDockParent()->GetWindowRect();
		else
			rcParent = GetDockAncestor()->GetWindowRect();

		double DockSize = 0;
		if ((GetDockStyle() & DS_DOCKED_LEFT) || (GetDockStyle() & DS_DOCKED_RIGHT))
			DockSize = (double)(rcParent.Width()*m_DockSizeRatio);
		else if ((GetDockStyle() & DS_DOCKED_TOP) || (GetDockStyle() & DS_DOCKED_BOTTOM))
			DockSize = (double)(rcParent.Height()*m_DockSizeRatio);
		else if ((GetDockStyle() & DS_DOCKED_CONTAINER))
			DockSize = 0;

		return (int)DockSize;
	}

	inline CDocker* CDocker::GetTopmostDocker() const
	// Returns the docker's parent at the top of the Z order.
	// Could be the dock ancestor or an undocked docker.
	{
		CDocker* pDockTopLevel = (CDocker* const)this;

		while(pDockTopLevel->GetDockParent())
		{
			assert (pDockTopLevel != pDockTopLevel->GetDockParent());
			pDockTopLevel = pDockTopLevel->GetDockParent();
		}

		return pDockTopLevel;
	}

	inline CTabbedMDI* CDocker::GetTabbedMDI() const
	{
		CTabbedMDI* pTabbedMDI = NULL;
		if (dynamic_cast<CTabbedMDI*>(GetView()))
		{
			pTabbedMDI = static_cast<CTabbedMDI*>(GetView());
		}

		return pTabbedMDI;
	}

	inline int CDocker::GetTextHeight()
	{
		NONCLIENTMETRICS info;
		ZeroMemory(&info, sizeof(NONCLIENTMETRICS));
		info.cbSize = GetSizeofNonClientMetrics();
		SystemParametersInfo (SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
		LOGFONT lf = info.lfStatusFont;

		CClientDC dc(this);
		dc.CreateFontIndirect(&lf);
		CSize szText = dc.GetTextExtentPoint32(_T("Text"), lstrlen(_T("Text")));
		return szText.cy;
	}

	inline void CDocker::Hide()
	{
		// Undocks a docker (if needed) and hides it.
		// To unhide the docker, dock it.

		if (IsDocked())
		{
			CDockContainer* pContainer = GetContainer();

			if (pContainer)
			{
				if (pContainer == pContainer->GetContainerParent())
					UndockContainer(pContainer, GetCursorPos(), FALSE);			
				else
					pContainer->GetContainerParent()->RemoveContainer(pContainer);
			}
			else
			{
				CDocker* pDockUndockedFrom = SeparateFromDock();
				pDockUndockedFrom->RecalcDockLayout();
			}
		}

		ShowWindow(SW_HIDE);
	}

	inline BOOL CDocker::IsChildOfDocker(const CWnd* pWnd) const
	// returns true if the specified window is a child of this docker
	{
		while ((pWnd != NULL) && (pWnd != GetDockAncestor()))
		{
			if ( pWnd == this ) return TRUE;
			if (IsRelated(pWnd)) break;
			pWnd = pWnd->GetParent();
		}

		return FALSE;
	}

	inline BOOL CDocker::IsDocked() const
	{
		return (((m_DockStyle&0xF) || (m_DockStyle & DS_DOCKED_CONTAINER)) && !m_IsUndocking); // Boolean expression
	}

	inline BOOL CDocker::IsDragAutoResize() const
	{
		return m_IsDragAutoResize;
	}

	inline BOOL CDocker::IsRelated(const CWnd* pWnd) const
	// Returns TRUE if the hWnd is a docker within this dock family
	{
		if (GetDockAncestor() == pWnd) return TRUE;

		std::vector<DockPtr>::iterator iter;
		for (iter = GetAllDockChildren()->begin(); iter != GetAllDockChildren()->end(); ++iter)
		{
			if ((*iter).get() == pWnd) return TRUE;
		}

		return FALSE;
	}

	inline BOOL CDocker::IsUndocked() const
	{
		return (!((m_DockStyle&0xF)|| (m_DockStyle & DS_DOCKED_CONTAINER)) && !m_IsUndocking); // Boolean expression
	}

	inline BOOL CDocker::LoadDockRegistrySettings(LPCTSTR szRegistryKeyName)
	// Recreates the docker layout based on information stored in the registry.
	// Assumes the DockAncestor window is already created.
	{
		BOOL bResult = FALSE;

		if (szRegistryKeyName)
		{
			std::vector<DockInfo> vDockList;

			CString KeyName = _T("Software\\") + CString(szRegistryKeyName) + _T("\\Dock Windows");
			HKEY hKey = 0;
			RegOpenKeyEx(HKEY_CURRENT_USER, KeyName, 0, KEY_READ, &hKey);
			if (hKey)
			{
				DWORD dwType = REG_BINARY;
				DWORD BufferSize = sizeof(DockInfo);
				DockInfo di;
				int i = 0;
				CString SubKeyName;
				SubKeyName.Format(_T("DockChild%d"), i);

				// Fill the DockList vector from the registry
				while (0 == RegQueryValueEx(hKey, SubKeyName, NULL, &dwType, (LPBYTE)&di, &BufferSize))
				{
					vDockList.push_back(di);
					i++;
					SubKeyName.Format(_T("DockChild%d"), i);
				}

				RegCloseKey(hKey);
				if (vDockList.size() > 0) bResult = TRUE;
			}

			// Add dockers without parents first
			std::vector<DockInfo>::iterator iter;
			for (iter = vDockList.begin(); iter != vDockList.end() ; ++iter)
			{
				DockInfo di = (*iter);
				if (di.DockParentID == 0)
				{
					CDocker* pDocker = NewDockerFromID(di.DockID);
					if (pDocker)
					{
						if (di.DockStyle & 0xF)
							AddDockedChild(pDocker, di.DockStyle, di.DockSize, di.DockID);
						else
							AddUndockedChild(pDocker, di.DockStyle, di.DockSize, di.Rect, di.DockID);
					}
					else
					{
						TRACE("Failed to add dockers without parents from registry\n");
						bResult = FALSE;
					}
				}
			}

			// Remove dockers without parents from vDockList
			for (UINT n = (UINT)vDockList.size(); n > 0; --n)
			{
				iter = vDockList.begin() + n-1;
				if ((*iter).DockParentID == 0)
					vDockList.erase(iter);
			}

			// Add remaining dockers
			while (vDockList.size() > 0)
			{
				bool bFound = false;
				for (iter = vDockList.begin(); iter != vDockList.end(); ++iter)
				{
					DockInfo di = *iter;
					CDocker* pDockParent = GetDockFromID(di.DockParentID);

					if (pDockParent != 0)
					{
						CDocker* pDock = NewDockerFromID(di.DockID);
						if(pDock)
						{
							pDockParent->AddDockedChild(pDock, di.DockStyle, di.DockSize, di.DockID);
							bFound = true;
						}
						else
						{
							TRACE("Failed to add dockers with parents from registry\n");
							bResult = FALSE;
						}

						vDockList.erase(iter);
						break;
					}
				}

				if (!bFound)
				{
					TRACE("Orphaned dockers stored in registry\n");
					bResult = FALSE;
					break;
				}
			}

			// Restore Dock container tab order and active container
			KeyName = _T("Software\\") + CString(szRegistryKeyName) + _T("\\Dock Windows");
			hKey = 0;
			RegOpenKeyEx(HKEY_CURRENT_USER, KeyName, 0, KEY_READ, &hKey);
			if (hKey)
			{
				UINT uContainer = 0;
				CString SubKeyName;
				SubKeyName.Format(_T("DockContainer%u"), uContainer);
				HKEY hContainerKey = 0;
				while ( 0 == RegOpenKeyEx(hKey, SubKeyName, 0, KEY_READ, &hContainerKey) )
				{
					DWORD dwType = REG_DWORD;
					DWORD BufferSize = sizeof(int);

					// Load tab order
					UINT nTab = 0;
					UINT nTabID;
					std::vector<UINT> vTabOrder;
					CString strTabKey;
					strTabKey.Format(_T("Tab%u"), nTab);
					while ( 0 == RegQueryValueEx(hContainerKey, strTabKey, NULL, &dwType, (LPBYTE)&nTabID, &BufferSize) )
					{
						vTabOrder.push_back(nTabID);
						strTabKey.Format(_T("Tab%u"), ++nTab);
					}

					// Set tab order
					UINT nParentID;
					if ( 0 == RegQueryValueEx(hContainerKey, _T("Parent Container"), NULL, &dwType, (LPBYTE)&nParentID, &BufferSize) )
					{
						CDocker* pDock = GetDockFromID(nParentID);
						if (pDock)
						{
							CDockContainer* pParentContainer = pDock->GetContainer();
							if (pParentContainer)
							{
								for (UINT uTab = 0; uTab < vTabOrder.size(); ++uTab)
								{
									CDocker* pOldDocker = GetDockFromView(pParentContainer->GetContainerFromIndex(uTab));
									if (pOldDocker)
									{
										UINT uOldID = pOldDocker->GetDockID();

										std::vector<UINT>::iterator it = std::find(vTabOrder.begin(), vTabOrder.end(), uOldID);
										UINT uOldTab = (UINT)(it - vTabOrder.begin());

										if (uTab != uOldTab)
											pParentContainer->SwapTabs(uTab, uOldTab);
									}
								}
							}
						}
					}

					// Set the active container
					UINT nActiveContainer;
					if (RegQueryValueEx(hContainerKey, _T("Active Container"), NULL, &dwType, (LPBYTE)&nActiveContainer, &BufferSize) == 0)
					{
						CDocker* pDocker = GetDockFromID(nActiveContainer);
						if (pDocker)
						{
							CDockContainer* pContainer = pDocker->GetContainer();
							if (pContainer)
							{
								int nPage = pContainer->GetContainerIndex(pContainer);
								if (nPage >= 0)
									pContainer->SelectPage(nPage);
							}
						}
					}

					RegCloseKey(hContainerKey);
					SubKeyName.Format(_T("DockContainer%u"), ++uContainer);
				}

				RegCloseKey(hKey);
			}
		}

		if (!bResult) CloseAllDockers();
		return bResult;
	}

	inline void CDocker::MoveDockChildren(CDocker* pDockTarget)
	// Used internally by Dock and Undock
	{
		assert(pDockTarget);

		// Transfer any dock children from the current docker to the target docker
		std::vector<CDocker*>::iterator iter;
		for (iter = GetDockChildren()->begin(); iter != GetDockChildren()->end(); ++iter)
		{
			pDockTarget->GetDockChildren()->push_back(*iter);
			(*iter)->m_pDockParent = pDockTarget;
			(*iter)->SetParent(pDockTarget);
			(*iter)->GetDockBar()->SetParent(pDockTarget);
		}
		GetDockChildren()->clear();
	}

	inline void CDocker::MoveDockInContainerChildren(CDocker* pDockTarget)
	{
		std::vector<CDocker*>::iterator iter;

		while (GetDockChildren()->size() > 0)
		{
			iter = GetDockChildren()->begin();
			CDocker* pDock = *iter;			
			DWORD dwDockStyle = pDock->GetDockStyle() | DS_DOCKED_CONTAINER;
			pDock->SeparateFromDock();
			pDockTarget->DockInContainer( pDock, dwDockStyle );
		}
	}

	inline CDocker* CDocker::NewDockerFromID(int nID)
	// Used in LoadRegistrySettings. Creates a new Docker from the specified ID
	{
		UNREFERENCED_PARAMETER(nID);

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
			TRACE("Unknown Dock ID\n");
			break;
		} */

		return pDock;
	}

	inline LRESULT CDocker::OnBarStart(LPDRAGPOS pdp)
	{
		CPoint pt = pdp->ptPos;
		ScreenToClient(pt);
		if (!IsDragAutoResize())
			DrawHashBar(pdp->hdr.hwndFrom, pt);
		m_OldPoint = pt;

		return 0L;
	}

	inline LRESULT CDocker::OnBarMove(LPDRAGPOS pdp)
	{
		CPoint pt = pdp->ptPos;
		ScreenToClient(pt);

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

		return 0L;
	}

	inline LRESULT CDocker::OnBarEnd(LPDRAGPOS pdp)
	{
		POINT pt = pdp->ptPos;
		ScreenToClient(pt);

		if (!IsDragAutoResize())
			DrawHashBar(pdp->hdr.hwndFrom, pt);

		ResizeDockers(pdp);
		return 0L;
	}

	inline int CDocker::OnCreate(LPCREATESTRUCT pcs)
	{
		UNREFERENCED_PARAMETER(pcs);

#if (WINVER >= 0x0500)
		if (GetParent()->GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_LAYOUTRTL)
		{
			DWORD dwExStyle = (DWORD)GetWindowLongPtr(GWL_EXSTYLE);
			SetWindowLongPtr(GWL_EXSTYLE, dwExStyle | WS_EX_LAYOUTRTL);
		}
#endif

		// Create the various child windows
		GetDockClient()->SetDock(this);
		GetDockClient()->Create(this);

		assert(GetView());			// Use SetView in the docker's constructor to set the view window
		GetView()->Create(GetDockClient());

		// Create the slider bar belonging to this docker
		GetDockBar()->SetDock(this);
		if (GetDockAncestor() != this)
			GetDockBar()->Create(GetDockAncestor());

		// Now remove the WS_POPUP style. It was required to allow this window
		// to be owned by the frame window.
		SetWindowLongPtr(GWL_STYLE, WS_CHILD);
		SetParent(GetParent());		// Reinstate the window's parent

		// Set the default colour for the splitter bar
		COLORREF rgbColour = GetSysColor(COLOR_BTNFACE);
		CWnd* pFrame = GetDockAncestor()->GetAncestor();
		ReBarTheme* pTheme = reinterpret_cast<ReBarTheme*>(pFrame->SendMessage(UWM_GETRBTHEME, 0, 0));

		if (pTheme && pTheme->UseThemes && pTheme->clrBkgnd2 != 0)
				rgbColour =pTheme->clrBkgnd2;

		SetBarColor(rgbColour);

		// Set the caption height based on text height
		m_NCHeight = MAX(20, GetTextHeight() + 5);

		return 0;
	}

	inline void CDocker::OnDestroy()
	{
		KillTimer(TIMER_ID1);
		KillTimer(TIMER_ID2);

		// Destroy any dock children first
		std::vector<CDocker*>::iterator iter;
		for (iter = GetDockChildren()->begin(); iter != GetDockChildren()->end(); ++iter)
		{
			(*iter)->Destroy();
		}

		if (dynamic_cast<CDockContainer*>(GetView()) && IsUndocked())
		{
			CDockContainer* pContainer = static_cast<CDockContainer*>(GetView());
			if (pContainer->GetAllContainers()->size() > 1)
			{
				// This container has children, so destroy them now
				std::vector<ContainerInfo> * pAllContainers = pContainer->GetAllContainers();
				std::vector<ContainerInfo>::iterator iter1;
				for (iter1 = pAllContainers->begin(); iter1 < pAllContainers->end(); ++iter1)
				{
					if ((*iter1).pContainer != pContainer)
					{
						// Reset container parent before destroying the dock window
						CDocker* pDock = GetDockFromView((*iter1).pContainer);
						if (pContainer->IsWindow())
							pContainer->SetParent(pDock->GetDockClient());

						pDock->Destroy();
					}
				}
			}
		}

		GetDockBar()->Destroy();

		// Post a docker destroyed message
		if ( GetDockAncestor()->IsWindow() )
			GetDockAncestor()->PostMessage(UWM_DOCKDESTROYED, (WPARAM)this, 0L);
	}

	inline LRESULT CDocker::OnDockDestroyed(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);
		CDocker* pDock = reinterpret_cast<CDocker*>(wParam);

		assert( this == GetDockAncestor() );
		std::vector<DockPtr>::iterator iter;
		for (iter = GetAllDockChildren()->begin(); iter != GetAllDockChildren()->end(); ++iter)
		{
			if ((*iter).get() == pDock)
			{
				GetAllDockChildren()->erase(iter);
				break;
			}
		}

		std::vector<CDocker*> *pDockers = GetDockAncestor()->GetAllDockers();
		for (std::vector<CDocker*>::iterator it = pDockers->begin(); it < pDockers->end(); ++it)
		{
			if ((*it) == pDock)
			{
				pDockers->erase(it);
				break;
			}
		}

		return 0L;
	}

	inline LRESULT CDocker::OnDockStart(LPDRAGPOS pdp)
	{
		if (IsDocked())
		{
			Undock(GetCursorPos());
			SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(pdp->ptPos.x, pdp->ptPos.y));
		}

		return 0L;
	}

	inline LRESULT CDocker::OnDockMove(LPDRAGPOS pdp)
	{
		CheckAllTargets(pdp);
		return 0L;
	}

	inline LRESULT CDocker::OnDockEnd(LPDRAGPOS pdp)
	{
		CDocker* pDock = static_cast<CDocker*>(FromHandlePermanent(pdp->hdr.hwndFrom));
		assert(dynamic_cast<CDocker*>(pDock));
		if (NULL == pDock) return 0L;

		UINT DockZone = pdp->DockZone;
		CRect rc = pDock->GetWindowRect();

		switch(DockZone)
		{
		case DS_DOCKED_LEFT:
		case DS_DOCKED_RIGHT:
			pDock->SetDockSize(rc.Width());
			Dock(pDock, pDock->GetDockStyle() | DockZone);
			break;
		case DS_DOCKED_TOP:
		case DS_DOCKED_BOTTOM:
			pDock->SetDockSize(rc.Height());
			Dock(pDock, pDock->GetDockStyle() | DockZone);
			break;
		case DS_DOCKED_CONTAINER:
			{
				DockInContainer(pDock, pDock->GetDockStyle() | DockZone);
				CDockContainer* pContainer = static_cast<CDockContainer*>(GetView());
				assert(dynamic_cast<CDockContainer*>(pContainer));				
				pContainer->SelectPage(0);
			}
			break;
		case DS_DOCKED_LEFTMOST:
		case DS_DOCKED_RIGHTMOST:
			pDock->SetDockSize(rc.Width());
			DockOuter(pDock, pDock->GetDockStyle() | DockZone);
			break;
		case DS_DOCKED_TOPMOST:
		case DS_DOCKED_BOTTOMMOST:
			pDock->SetDockSize(rc.Height());
			DockOuter(pDock, pDock->GetDockStyle() | DockZone);
			break;
		}

		GetDockHint()->Destroy();
		CloseAllTargets();

		return 0L;
	}

	inline LRESULT CDocker::OnExitSizeMove(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);
		UNREFERENCED_PARAMETER(lParam);

		m_IsBlockMove = FALSE;
		m_IsDragging = FALSE;
		SendNotify(UWN_DOCKEND);

		return 0L;
	}

	inline LRESULT CDocker::OnNCLButtonDblClk(WPARAM wParam, LPARAM lParam)
	{
		m_IsDragging = FALSE;
		return FinalWindowProc(WM_NCLBUTTONDBLCLK, wParam, lParam);
	}

	inline LRESULT CDocker::OnNotify(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);
		LPDRAGPOS pdp = (LPDRAGPOS)lParam;

		switch (((LPNMHDR)lParam)->code)
		{
		case UWN_BARSTART:		return OnBarStart(pdp);
		case UWN_BARMOVE:		return OnBarMove(pdp);
		case UWN_BAREND:		return OnBarEnd(pdp);
		case UWN_DOCKSTART:		return OnDockStart(pdp);
		case UWN_DOCKMOVE:		return OnDockMove(pdp);
		case UWN_DOCKEND:		return OnDockEnd(pdp);
		}
		return 0L;
	}

	inline LRESULT CDocker::OnTimer(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);

		if (this == GetDockAncestor())
		{
			if (wParam == TIMER_ID1 || wParam == TIMER_ID2)
			{
				DrawAllCaptions();
				KillTimer(wParam);
			}
		}

		return 0L;
	}

	inline void CDocker::ResizeDockers(LPDRAGPOS pdp)
	// Called when the docker's splitter bar is dragged
	{
		assert(pdp);

		POINT pt = pdp->ptPos;
		ScreenToClient(pt);

		CDocker* pDock = (static_cast<CDockBar*>(FromHandlePermanent(pdp->hdr.hwndFrom)))->GetDock();
		if (NULL == pDock) return;

		RECT rcDock = pDock->GetWindowRect();
		ScreenToClient(rcDock);

		int iBarWidth    = pDock->GetDockBar()->GetWidth();
		double dBarWidth = iBarWidth;
		int DockSize;

		BOOL RTL = FALSE;
#ifdef WS_EX_LAYOUTRTL
		RTL = (GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_LAYOUTRTL);
#endif

		CRect rcDockParent = pDock->m_pDockParent->GetWindowRect();

		switch (pDock->GetDockStyle() & 0xF)
		{
		case DS_DOCKED_LEFT:
			if (RTL) DockSize = rcDock.right - MAX(pt.x, iBarWidth/2) - (int)(.5* dBarWidth);
			else     DockSize = MAX(pt.x, iBarWidth/2) - rcDock.left - (int)(.5* dBarWidth);

			DockSize = MAX(-iBarWidth, DockSize);
			pDock->SetDockSize(DockSize);
			pDock->m_DockSizeRatio = ((double)pDock->m_DockStartSize)/((double)rcDockParent.Width());
			break;
		case DS_DOCKED_RIGHT:
			if (RTL)  DockSize = MAX(pt.x, iBarWidth/2) - rcDock.left - (int)(.5* dBarWidth);
			else      DockSize = rcDock.right - MAX(pt.x, iBarWidth/2) - (int)(.5* dBarWidth);

			DockSize = MAX(-iBarWidth, DockSize);
			pDock->SetDockSize(DockSize);
			pDock->m_DockSizeRatio = ((double)pDock->m_DockStartSize)/((double)rcDockParent.Width());
			break;
		case DS_DOCKED_TOP:
			DockSize = MAX(pt.y, iBarWidth/2) - rcDock.top - (int)(.5* dBarWidth);
			DockSize = MAX(-iBarWidth, DockSize);
			pDock->SetDockSize(DockSize);
			pDock->m_DockSizeRatio = ((double)pDock->m_DockStartSize)/((double)rcDockParent.Height());
			break;
		case DS_DOCKED_BOTTOM:
			DockSize = rcDock.bottom - MAX(pt.y, iBarWidth/2) - (int)(.5* dBarWidth);
			DockSize = MAX(-iBarWidth, DockSize);
			pDock->SetDockSize(DockSize);
			pDock->m_DockSizeRatio = ((double)pDock->m_DockStartSize)/((double)rcDockParent.Height());
			break;
		}

		RecalcDockLayout();
	}

	inline LRESULT CDocker::OnSysColorChange(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);
		UNREFERENCED_PARAMETER(lParam);

		if (this == GetDockAncestor())
		{
			COLORREF rgbColour = GetSysColor(COLOR_BTNFACE);
			CWnd* pFrame = GetDockAncestor()->GetAncestor();
			ReBarTheme* pTheme = reinterpret_cast<ReBarTheme*>(pFrame->SendMessage(UWM_GETRBTHEME, 0, 0));

			if (pTheme && pTheme->UseThemes && pTheme->clrBand2 != 0)
				rgbColour = pTheme->clrBkgnd2;
			else
				rgbColour = GetSysColor(COLOR_BTNFACE);

			// Set the splitter bar colour for each docker descendant
			std::vector<DockPtr>::iterator iter;
			for (iter = GetAllDockChildren()->begin(); iter != GetAllDockChildren()->end(); ++iter)
				(*iter)->SetBarColor(rgbColour);

			// Set the splitter bar colour for the docker ancestor
			SetBarColor(rgbColour);
		}

		return FinalWindowProc(WM_SYSCOLORCHANGE, wParam, lParam);
	}

	inline LRESULT CDocker::OnSysCommand(WPARAM wParam, LPARAM lParam)
	{
		switch(wParam&0xFFF0)
		{
		case SC_MOVE:
			// An undocked docker is being moved
			{
				BOOL bResult = FALSE;
				m_IsDragging = TRUE;
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
			m_IsClosing = TRUE;
			break;
		}
		return FinalWindowProc(WM_SYSCOMMAND, wParam, lParam);
	}

	inline LRESULT CDocker::OnWindowPosChanging(WPARAM wParam, LPARAM lParam)
	{
		// Suspend dock drag moving while over dock zone
		if (m_IsBlockMove)
		{
        	LPWINDOWPOS pWndPos = (LPWINDOWPOS)lParam;
			pWndPos->flags |= SWP_NOMOVE|SWP_FRAMECHANGED;
			return 0;
		}

		return FinalWindowProc(WM_WINDOWPOSCHANGING, wParam, lParam);
	}

	inline LRESULT CDocker::OnWindowPosChanged(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);

		if (m_IsDragging)
		{
			// Send a Move notification to the parent
			if ( IsLeftButtonDown() )
			{
				LPWINDOWPOS wPos = (LPWINDOWPOS)lParam;
				if ((!(wPos->flags & SWP_NOMOVE)) || m_IsBlockMove)
					SendNotify(UWN_DOCKMOVE);
			}
			else
			{
				CloseAllTargets();
				m_IsBlockMove = FALSE;
			}
		}
		else if (this == GetTopmostDocker())
		{
			// Reposition the dock children
			if (IsUndocked() && IsWindowVisible() && !m_IsClosing) RecalcDockLayout();
		}

		return FinalWindowProc(WM_WINDOWPOSCHANGED, wParam, lParam);
	}

	inline void CDocker::PreCreate(CREATESTRUCT &cs)
	{
		// Specify the WS_POPUP style to have this window owned
		if (this != GetDockAncestor())
			cs.style = WS_POPUP;

		cs.dwExStyle = WS_EX_TOOLWINDOW;
	}

	inline void CDocker::PreRegisterClass(WNDCLASS &wc)
	{
		wc.lpszClassName = _T("Win32++ Docker");
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	}

	inline void CDocker::RecalcDockChildLayout(CRect& rc)
	{
		// This function positions the Docker's dock children, the Dockers client area
		//  and draws the dockbar bars.

		// Notes:
		// 1) This function is called recursively.
		// 2) The client area and child dockers are positioned simultaneously with
		//      DeferWindowPos to avoid drawing errors in complex docker arrangements.
		// 3) The docker's client area contains the docker's caption (if any) and the docker's view window.

		// Note: All top level dockers are undocked, including the dock ancestor.

		BOOL RTL = FALSE;
#ifdef WS_EX_LAYOUTRTL
		RTL = (GetWindowLongPtr(GWL_EXSTYLE) & WS_EX_LAYOUTRTL);
#endif
	
		if (IsDocked())
		{
			rc.OffsetRect(-rc.left, -rc.top);
		}

		HDWP hdwp = BeginDeferWindowPos((int)m_vDockChildren.size() +2);

		// Step 1: Calculate the position of each Docker child, DockBar, and Client window.
		//   The Client area = the docker rect minus the area of dock children and the dock bar (splitter bar).
		std::vector<CDocker*>::iterator iter;
		for (iter = m_vDockChildren.begin(); iter != m_vDockChildren.end(); ++iter)
		{
			CRect rcChild = rc;
			double DockSize = (*iter)->m_DockStartSize;
			int minSize = 30;

			// Calculate the size of the Docker children
			switch ((*iter)->GetDockStyle() & 0xF)
			{
			case DS_DOCKED_LEFT:
				if ((*iter)->GetDockStyle() & DS_NO_FIXED_RESIZE)
					DockSize = MIN((*iter)->m_DockSizeRatio*(GetWindowRect().Width()), rcChild.Width());

				if (RTL)
				{
					rcChild.left = rcChild.right - (int)DockSize;
					rcChild.left = MIN(rcChild.left, rc.right - minSize);
					rcChild.left = MAX(rcChild.left, rc.left + minSize);
				}
				else
				{
					rcChild.right = rcChild.left + (int)DockSize;
					rcChild.right = MAX(rcChild.right, rc.left + minSize);
					rcChild.right = MIN(rcChild.right, rc.right - minSize);
				}
				break;
			case DS_DOCKED_RIGHT:
				if ((*iter)->GetDockStyle() & DS_NO_FIXED_RESIZE)
					DockSize = MIN((*iter)->m_DockSizeRatio*(GetWindowRect().Width()), rcChild.Width());

				if (RTL)
				{
					rcChild.right = rcChild.left + (int)DockSize;
					rcChild.right = MAX(rcChild.right, rc.left + minSize);
					rcChild.right = MIN(rcChild.right, rc.right - minSize);
				}
				else
				{
					rcChild.left = rcChild.right - (int)DockSize;				
					rcChild.left = MIN(rcChild.left, rc.right - minSize);
					rcChild.left = MAX(rcChild.left, rc.left + minSize);
				}

				break;
			case DS_DOCKED_TOP:
				if ((*iter)->GetDockStyle() & DS_NO_FIXED_RESIZE)
					DockSize = MIN((*iter)->m_DockSizeRatio*(GetWindowRect().Height()), rcChild.Height());
				
				rcChild.bottom = rcChild.top + (int)DockSize;				
				rcChild.bottom = MAX(rcChild.bottom, rc.top + minSize);
				rcChild.bottom = MIN(rcChild.bottom, rc.bottom - minSize);
				break;
			case DS_DOCKED_BOTTOM:
				if ((*iter)->GetDockStyle() & DS_NO_FIXED_RESIZE)
					DockSize = MIN((*iter)->m_DockSizeRatio*(GetWindowRect().Height()), rcChild.Height());
				
				rcChild.top = rcChild.bottom - (int)DockSize;				
				rcChild.top = MIN(rcChild.top, rc.bottom - minSize);
				rcChild.top = MAX(rcChild.top, rc.top + minSize);
				
				break;
			}

			if ((*iter)->IsDocked())
			{
				// Position this docker's children
				hdwp = (*iter)->DeferWindowPos(hdwp, NULL, rcChild, SWP_SHOWWINDOW|SWP_FRAMECHANGED);
				(*iter)->m_rcChild = rcChild;

				rc.SubtractRect(rc, rcChild);

				// Calculate the dimensions of the splitter bar
				CRect rcBar = rc;
				DWORD DockSide = (*iter)->GetDockStyle() & 0xF;

				if (DS_DOCKED_LEFT   == DockSide)
				{
					if (RTL) rcBar.left   = rcBar.right - (*iter)->GetBarWidth();
					else	 rcBar.right  = rcBar.left + (*iter)->GetBarWidth();
				}

				if (DS_DOCKED_RIGHT  == DockSide)
				{
					if (RTL) rcBar.right  = rcBar.left + (*iter)->GetBarWidth();
					else	 rcBar.left   = rcBar.right - (*iter)->GetBarWidth();
				}

				if (DS_DOCKED_TOP    == DockSide) rcBar.bottom = rcBar.top + (*iter)->GetBarWidth();
				if (DS_DOCKED_BOTTOM == DockSide) rcBar.top    = rcBar.bottom - (*iter)->GetBarWidth();

				// Save the splitter bar position. We will reposition it later.
				(*iter)->m_rcBar = rcBar;
				rc.SubtractRect(rc, rcBar);
			}
		}

		// Step 2: Position the Dock client and dock bar
		if (GetDockClient()->IsWindow())
			hdwp = GetDockClient()->DeferWindowPos(hdwp, NULL, rc, SWP_SHOWWINDOW |SWP_FRAMECHANGED);
		EndDeferWindowPos(hdwp);

		// Position the dockbar. Only docked dockers have a dock bar.
		if (IsDocked())
		{
			CRect rcBar;
			rcBar.IntersectRect(m_rcBar, GetDockParent()->GetViewRect());
			
			// The SWP_NOCOPYBITS forces a redraw of the dock bar.
			GetDockBar()->SetWindowPos(NULL, rcBar, SWP_SHOWWINDOW|SWP_FRAMECHANGED|SWP_NOCOPYBITS);
		}

		// Step 3: Now recurse through the docker's children. They might have children of their own.
		for (iter = m_vDockChildren.begin(); iter < m_vDockChildren.end(); ++iter)
		{
			(*iter)->RecalcDockChildLayout((*iter)->m_rcChild);
		}
	}

	inline void CDocker::RecalcDockLayout()
	// Repositions the dock children of a top level docker
	{
		if (GetDockAncestor()->IsWindow())
		{
			CRect rc = GetTopmostDocker()->GetViewRect();
			GetTopmostDocker()->RecalcDockChildLayout(rc);
			GetTopmostDocker()->UpdateWindow();
		}
	}

	inline std::vector<CDocker*> CDocker::SortDockers()
	// Returns a vector of sorted dockers, used by SaveRegistrySettings.
	{
		std::vector<CDocker*> vSorted;
		std::vector<CDocker*>::iterator itSort;
		std::vector<DockPtr>::iterator itAll;

		// Add undocked top level dockers
		for (itAll = GetAllDockChildren()->begin(); itAll <  GetAllDockChildren()->end(); ++itAll)
		{
			if (!(*itAll)->GetDockParent())
				vSorted.push_back((*itAll).get());
		}

		// Add dock ancestor's children
		vSorted.insert(vSorted.end(), GetDockAncestor()->GetDockChildren()->begin(), GetDockAncestor()->GetDockChildren()->end());

		// Add other dock children
		int index = 0;
		itSort = vSorted.begin();
		while (itSort < vSorted.end())
		{
			vSorted.insert(vSorted.end(), (*itSort)->GetDockChildren()->begin(), (*itSort)->GetDockChildren()->end());
			itSort = vSorted.begin() + (++index);
		}

		// Add dockers docked in containers
		std::vector<CDocker*> vDockContainers;
		for (itSort = vSorted.begin(); itSort< vSorted.end(); ++itSort)
		{
			if ((*itSort)->GetContainer())
				vDockContainers.push_back(*itSort);
		}

		for (itSort = vDockContainers.begin(); itSort < vDockContainers.end(); ++itSort)
		{
			CDockContainer* pContainer = (*itSort)->GetContainer();

			for (UINT i = 0; i < pContainer->GetAllContainers()->size(); ++i)
			{
				CDockContainer* pChild = pContainer->GetContainerFromIndex(i);

				if (pChild != pContainer)
				{
					CDocker* pDock = GetDockFromView(pChild);
					vSorted.push_back(pDock);
				}
			}
		}

		return vSorted;
	}

	inline BOOL CDocker::SaveDockRegistrySettings(LPCTSTR szRegistryKeyName)
	// Stores the docking configuration in the registry
	// NOTE: This function assumes that each docker has a unique DockID
	{
		assert(VerifyDockers());

		std::vector<CDocker*> vSorted = SortDockers();
		std::vector<CDocker*>::iterator iter;
		std::vector<DockInfo> vDockInfo;

		if (szRegistryKeyName)
		{
			HKEY hKey = NULL;
			HKEY hKeyDock = NULL;
			HKEY hKeyContainer = NULL;
			CString KeyName = _T("Software\\") + CString(szRegistryKeyName);

			try
			{
				// Create/Open the App's registry key
				if (ERROR_SUCCESS != RegCreateKeyEx(HKEY_CURRENT_USER, KeyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL))
					throw (CWinException(_T("RegCreateKeyEx Failed")));

				// Remove Old Docking info ...
				// Remove existing DockContainer SubKeys
				UINT uDockContainer = 0;
				CString SubKeyName;
				SubKeyName.Format(_T("Dock Windows\\DockContainer%u"), uDockContainer);
				while (0 == RegDeleteKey(hKey, SubKeyName) )
				{
					SubKeyName.Format(_T("Dock Windows\\DockContainer%u"), ++uDockContainer);
				}

				// Remove the Dock Windows key
				RegDeleteKey(hKey, _T("Dock Windows"));

				// Fill the DockInfo vector with the docking information
				for (iter = vSorted.begin(); iter !=  vSorted.end(); ++iter)
				{
					DockInfo di;
					ZeroMemory(&di, sizeof(DockInfo));
					if (! (*iter)->IsWindow())
						throw (CWinException(_T("Can't save Docker in registry. \n")));;

					di.DockID	 = (*iter)->GetDockID();
					di.DockStyle = (*iter)->GetDockStyle();
					di.DockSize  = (*iter)->GetDockSize();
					di.Rect		 = (*iter)->GetWindowRect();
					if ((*iter)->GetDockParent())
						di.DockParentID = (*iter)->GetDockParent()->GetDockID();

					vDockInfo.push_back(di);
				}

				if (ERROR_SUCCESS != RegCreateKeyEx(hKey, _T("Dock Windows"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeyDock, NULL))
					throw (CWinException(_T("RegCreateKeyEx Failed")));

				// Add the Dock windows information to the registry
				for (UINT u = 0; u < vDockInfo.size(); ++u)
				{
					DockInfo di = vDockInfo[u];
					SubKeyName.Format(_T("DockChild%u"), u);
					if(ERROR_SUCCESS != RegSetValueEx(hKeyDock, SubKeyName, 0, REG_BINARY, (LPBYTE)&di, sizeof(DockInfo)))
						throw (CWinException(_T("RegSetValueEx failed")));
				}

				// Add dock container info to the registry
				UINT u1 = 0;
				for (iter = vSorted.begin(); iter != vSorted.end(); ++iter)
				{
					CDockContainer* pContainer = (*iter)->GetContainer();

					if (pContainer && ( !((*iter)->GetDockStyle() & DS_DOCKED_CONTAINER) ))
					{
						SubKeyName.Format(_T("DockContainer%u"), u1++);
						if (ERROR_SUCCESS != RegCreateKeyEx(hKeyDock, SubKeyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeyContainer, NULL))
							throw (CWinException(_T("RegCreateKeyEx Failed")));

						// Store the container group's parent
						int nID = GetDockFromView(pContainer)->GetDockID();
						if(ERROR_SUCCESS != RegSetValueEx(hKeyContainer, _T("Parent Container"), 0, REG_DWORD, (LPBYTE)&nID, sizeof(int)))
							throw (CWinException(_T("RegSetValueEx failed")));

						// Store the active (selected) container
						nID = GetDockFromView(pContainer->GetActiveContainer())->GetDockID();
						if(ERROR_SUCCESS != RegSetValueEx(hKeyContainer, _T("Active Container"), 0, REG_DWORD, (LPBYTE)&nID, sizeof(int)))
							throw (CWinException(_T("RegSetValueEx failed")));

						// Store the tab order
						for (UINT u2 = 0; u2 < pContainer->GetAllContainers()->size(); ++u2)
						{
							SubKeyName.Format(_T("Tab%u"), u2);
							CDockContainer* pTab = pContainer->GetContainerFromIndex(u2);
							int nTabID = GetDockFromView(pTab)->GetDockID();

							if(ERROR_SUCCESS != RegSetValueEx(hKeyContainer, SubKeyName, 0, REG_DWORD, (LPBYTE)&nTabID, sizeof(int)))
								throw (CWinException(_T("RegSetValueEx failed")));
						}

						RegCloseKey(hKeyContainer);
					}
				}

				RegCloseKey(hKeyDock);
				RegCloseKey(hKey);
			}

			catch (const CWinException& e)
			{
				// Roll back the registry changes by deleting the subkeys
				if (hKey)
				{
					if (hKeyDock)
					{
						// Remove existing DockContainer SubKeys
						UINT uDockContainer = 0;
						CString SubKeyName;
						SubKeyName.Format(_T("DockContainer%u"), uDockContainer);
						while (0 == RegDeleteKey(hKeyDock, SubKeyName) )
						{
							SubKeyName.Format(_T("DockContainer%u"), ++uDockContainer);
						}
						RegDeleteKey(hKeyDock, _T("Dock Windows"));
						RegCloseKey(hKeyDock);
					}

					RegCloseKey(hKey);
				}

				e.what();
				return FALSE;
			}
		}

		return TRUE;
	}

	inline void CDocker::SendNotify(UINT nMessageID)
	// Sends a docking notification to the docker below the cursor
	{
		DRAGPOS DragPos;
		DragPos.hdr.code = nMessageID;
		DragPos.hdr.hwndFrom = m_hWnd;
		DragPos.ptPos = GetCursorPos();
		DragPos.DockZone = m_dwDockZone;
		m_dwDockZone = 0;

		CDocker* pDock = GetDockFromPoint(DragPos.ptPos);

		if (pDock)
			pDock->SendMessage(WM_NOTIFY, 0L, (LPARAM)&DragPos);
		else
		{
			if (GetDockHint()->IsWindow())
				GetDockHint()->Destroy();

			CloseAllTargets();
			m_IsBlockMove = FALSE;
		}
	}

	inline void CDocker::SetDockStyle(DWORD dwDockStyle)
	{
		if (IsWindow())
		{
			if ((dwDockStyle & DS_CLIENTEDGE) != (m_DockStyle & DS_CLIENTEDGE))
			{
				if (dwDockStyle & DS_CLIENTEDGE)
				{
					DWORD dwExStyle = (DWORD)GetDockClient()->GetWindowLongPtr(GWL_EXSTYLE)|WS_EX_CLIENTEDGE;
					GetDockClient()->SetWindowLongPtr(GWL_EXSTYLE, (LONG_PTR)dwExStyle);
					GetDockClient()->RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_FRAME);
				}
				else
				{
					DWORD dwExStyle = (DWORD)GetDockClient()->GetWindowLongPtr(GWL_EXSTYLE);
					dwExStyle &= ~WS_EX_CLIENTEDGE;
					GetDockClient()->SetWindowLongPtr(GWL_EXSTYLE, (LONG_PTR)dwExStyle);
					GetDockClient()->RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_FRAME);
				}
			}

			RecalcDockLayout();
		}

		m_DockStyle = dwDockStyle;
	}

	inline void CDocker::SetCaption(LPCTSTR szCaption)
	// Sets the caption text
	{
		GetDockClient()->SetCaption(szCaption);

		if (IsWindow())
			SetWindowText(szCaption);
	}

	inline void CDocker::SetCaptionColors(COLORREF Foregnd1, COLORREF Backgnd1, COLORREF ForeGnd2, COLORREF BackGnd2)
	{
		GetDockClient()->SetCaptionColors(Foregnd1, Backgnd1, ForeGnd2, BackGnd2);
	}

	inline void CDocker::SetCaptionHeight(int nHeight)
	// Sets the height of the caption
	{
		m_NCHeight = nHeight;
		if (IsWindow())
		{
			RedrawWindow();
			RecalcDockLayout();
		}
	}

	inline void CDocker::SetDockSize(int DockSize)
	// Sets the size of a docked docker
	{
		if (IsDocked())
		{
			assert (m_pDockParent);
			CRect rc = m_pDockParent->GetWindowRect();
			switch (GetDockStyle() & 0xF)
			{
			case DS_DOCKED_LEFT:
			case DS_DOCKED_RIGHT:
				m_DockStartSize = MIN(DockSize,rc.Width());
				m_DockSizeRatio = ((double)m_DockStartSize)/((double)rc.Width());
				break;
			case DS_DOCKED_TOP:
			case DS_DOCKED_BOTTOM:
				m_DockStartSize = MIN(DockSize,rc.Height());
				m_DockSizeRatio = ((double)m_DockStartSize)/((double)rc.Height());
				break;
			}

			RecalcDockLayout();
		}
		else
		{
			m_DockStartSize = DockSize;
			m_DockSizeRatio = 1.0;
		}
	}

	inline void CDocker::SetDragAutoResize(BOOL bAutoResize)
	{
		m_IsDragAutoResize = bAutoResize;
	}

	inline void CDocker::SetView(CWnd& wndView)
	// Assigns the view window to the docker
	{
		CWnd* pWnd = &wndView;
		GetDockClient()->SetView(wndView);
		if (dynamic_cast<CDockContainer*>(pWnd))
		{
			CDockContainer* pContainer = static_cast<CDockContainer*>(&wndView);
			SetCaption(pContainer->GetDockCaption().c_str());
		}
	}

	inline void CDocker::PromoteFirstChild()
	// One of the steps required for undocking
	{
		// Promote our first child to replace ourself
		if (m_pDockParent)
		{
			std::vector<CDocker*>::iterator iter;
			std::vector<CDocker*>& vChild = m_pDockParent->m_vDockChildren;

			for (iter = vChild.begin(); iter != vChild.end(); ++iter)
			{
				if ((*iter) == this)
				{
					if (m_vDockChildren.size() > 0)
						// swap our first child for ourself as a child of the parent
						(*iter) = m_vDockChildren[0];
					else
						// remove ourself as a child of the parent
						vChild.erase(iter);

					// Done
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
			pDockFirstChild->m_DockStartSize = m_DockStartSize;
			pDockFirstChild->m_DockSizeRatio = m_DockSizeRatio;

			if (m_pDockParent)
			{
				pDockFirstChild->m_pDockParent = m_pDockParent;
				pDockFirstChild->SetParent(m_pDockParent);
				pDockFirstChild->GetDockBar()->SetParent(m_pDockParent);
			}
			else
			{
				std::vector<CDocker*>::iterator iter;
				for (iter = GetDockChildren()->begin() + 1; iter != GetDockChildren()->end(); ++iter)
					(*iter)->ShowWindow(SW_HIDE);

				pDockFirstChild->ConvertToPopup(GetWindowRect());
				pDockFirstChild->GetDockBar()->ShowWindow(SW_HIDE);
			}

			m_vDockChildren.erase(m_vDockChildren.begin());
			MoveDockChildren(pDockFirstChild);
		}
	}

	inline void CDocker::ConvertToChild(HWND hWndParent)
	{
		DWORD dwStyle = WS_CHILD | WS_VISIBLE;
		SetWindowLongPtr(GWL_STYLE, dwStyle);
		SetParent(FromHandle(hWndParent));
		GetDockBar()->SetParent(FromHandle(hWndParent));
	}

	inline void CDocker::ConvertToPopup(const CRect& rc)
	{
		// Change the window to an "undocked" style
		ShowWindow(SW_HIDE);
		DWORD dwStyle = WS_POPUP| WS_CAPTION | WS_SYSMENU | WS_THICKFRAME;
		SetWindowLongPtr(GWL_STYLE, (LONG_PTR)dwStyle);

		// Change the window's parent and reposition it
		GetDockBar()->ShowWindow(SW_HIDE);
		SetWindowPos(0, 0, 0, 0, 0, SWP_NOSENDCHANGING|SWP_HIDEWINDOW|SWP_NOREDRAW);
		m_pDockParent = 0;
		SetParent(0);
		SetWindowPos(NULL, rc, SWP_SHOWWINDOW|SWP_FRAMECHANGED|SWP_NOOWNERZORDER);
		GetDockClient()->SetWindowPos(NULL, GetClientRect(), SWP_SHOWWINDOW);

		SetWindowText(GetCaption().c_str());
	}

	inline CDocker* CDocker::SeparateFromDock()
	{
		// This performs some of the tasks required for undocking.
		// It is also used when a docker is hidden.
		CDocker* pDockUndockedFrom = GetDockParent();
		if (!pDockUndockedFrom && (m_vDockChildren.size() > 0))
			pDockUndockedFrom = m_vDockChildren[0];

		PromoteFirstChild();
		m_pDockParent = 0;

		GetDockBar()->ShowWindow(SW_HIDE);
		m_DockStyle = m_DockStyle & 0xFFFFFFF0;
		m_DockStyle &= ~DS_DOCKED_CONTAINER;

		return pDockUndockedFrom;
	}

	inline BOOL CDocker::SetRedraw(BOOL bRedraw /* = TRUE*/)
	{
		// Allows nested calls to SetRedraw.
		bRedraw? ++m_nRedrawCount : --m_nRedrawCount ;

		return (BOOL)SendMessage(WM_SETREDRAW, (m_nRedrawCount >= 0), 0L); 
	}

	inline void CDocker::SetUndockPosition(CPoint pt)
	{
		m_IsUndocking = TRUE;
		CRect rc;
		rc = GetDockClient()->GetWindowRect();
		CRect rcTest = rc;
		rcTest.bottom = MIN(rcTest.bottom, rcTest.top + m_NCHeight);
		if ( !rcTest.PtInRect(pt))
			rc.SetRect(pt.x - rc.Width()/2, pt.y - m_NCHeight/2, pt.x + rc.Width()/2, pt.y - m_NCHeight/2 + rc.Height());

		ConvertToPopup(rc);
		m_IsUndocking = FALSE;

		// Send the undock notification to the frame
		NMHDR nmhdr;
		ZeroMemory(&nmhdr, sizeof(NMHDR));
		nmhdr.hwndFrom = m_hWnd;
		nmhdr.code = UWN_UNDOCKED;
		nmhdr.idFrom = m_nDockID;
		CWnd* pFrame = GetDockAncestor()->GetAncestor();
		pFrame->SendMessage(WM_NOTIFY, (WPARAM)m_nDockID, (LPARAM)&nmhdr);

		// Initiate the window move
		SetCursorPos(pt.x, pt.y);
		ScreenToClient(pt);
		PostMessage(WM_SYSCOMMAND, (WPARAM)(SC_MOVE|0x0002), MAKELPARAM(pt.x, pt.y));
	}

	inline void CDocker::Undock(CPoint pt, BOOL bShowUndocked)
	{
		// Return if we shouldn't undock
		if (GetDockStyle() & DS_NO_UNDOCK) return;

		// Undocking isn't supported on Win95
		if (1400 == GetWinVersion()) return;

		CDocker* pDockUndockedFrom = SeparateFromDock();

		// Position and draw the undocked window, unless it is about to be closed
		if (bShowUndocked)
		{
			SetUndockPosition(pt);
		}

		RecalcDockLayout();
		if ((pDockUndockedFrom) && (pDockUndockedFrom->GetTopmostDocker() != GetTopmostDocker()))
			pDockUndockedFrom->RecalcDockLayout();
	}

	inline void CDocker::UndockContainer(CDockContainer* pContainer, CPoint pt, BOOL bShowUndocked)
	{
		assert(pContainer);
		assert(this == GetDockFromView(pContainer->GetContainerParent()));

		// Return if we shouldn't undock
		if (GetDockFromView(pContainer)->GetDockStyle() & DS_NO_UNDOCK) return;

		if (GetDockFromView(pContainer) == GetDockAncestor()) return;

		// Undocking isn't supported on Win95
		if (1400 == GetWinVersion()) return;

		CDocker* pDockUndockedFrom = GetDockFromView(pContainer->GetContainerParent());
		pDockUndockedFrom->ShowWindow(SW_HIDE);
		if (GetView() == pContainer)
		{
			// The parent container is being undocked, so we need
			// to transfer our child containers to a different docker

			// Choose a new docker from among the dockers for child containers
			CDocker* pDockNew = 0;
			CDocker* pDockOld = GetDockFromView(pContainer);
			std::vector<ContainerInfo> AllContainers = *pContainer->GetAllContainers();
			std::vector<ContainerInfo>::iterator iter = AllContainers.begin();
			while ((0 == pDockNew) && (iter < AllContainers.end()))
			{
				if ((*iter).pContainer != pContainer)
				{
					pDockNew = static_cast<CDocker*>(FromHandlePermanent(::GetParent((*iter).pContainer->GetParent()->GetHwnd())));
					assert(dynamic_cast<CDocker*>(pDockNew));
				}

				++iter;
			}

			if (pDockNew)
			{
				// Move containers from the old docker to the new docker
				CDockContainer* pContainerNew = static_cast<CDockContainer*>(pDockNew->GetView());
				assert(dynamic_cast<CDockContainer*>(pContainerNew));
				for (iter = AllContainers.begin(); iter != AllContainers.end(); ++iter)
				{
					if ((*iter).pContainer != pContainer)
					{
						CDockContainer* pChildContainer = static_cast<CDockContainer*>((*iter).pContainer);
						pContainer->RemoveContainer(pChildContainer);
						if (pContainerNew != pChildContainer)
						{
							pContainerNew->AddContainer(pChildContainer);
							CDocker* pDock = GetDockFromView(pChildContainer);
							pDock->SetParent(pDockNew);
							pDock->m_pDockParent = pDockNew;
						}
					}
				}

				// Now transfer the old docker's settings to the new one.
				pDockUndockedFrom = pDockNew;
				pDockNew->m_DockStyle		= pDockOld->m_DockStyle;
				pDockNew->m_DockStartSize	= pDockOld->m_DockStartSize;
				pDockNew->m_DockSizeRatio	= pDockOld->m_DockSizeRatio;
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
					pDockNew->SetWindowLongPtr(GWL_STYLE, (LONG_PTR)dwStyle);
					pDockNew->m_pDockParent = 0;
					pDockNew->SetParent(0);
					pDockNew->SetWindowPos(NULL, rc, SWP_SHOWWINDOW|SWP_FRAMECHANGED| SWP_NOOWNERZORDER);
				}
				pDockNew->GetDockBar()->SetParent(pDockOld->GetParent());
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
			CDockContainer* pContainerParent = static_cast<CDockContainer*>(GetView());
			assert(dynamic_cast<CDockContainer*>(pContainerParent));
			pContainerParent->RemoveContainer(pContainer);
			pContainerParent->SetTabSize();
			pContainerParent->SetFocus();
			pContainerParent->GetViewPage()->SetParent(pContainerParent);
		}

		// Finally do the actual undocking
		CDocker* pDock = GetDockFromView(pContainer);
		CRect rc = GetDockClient()->GetWindowRect();
		ScreenToClient(rc);
		pDock->GetDockClient()->SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
		pDock->Undock(pt, bShowUndocked);
		pDock->GetDockBar()->SetParent(GetDockAncestor());
		pDockUndockedFrom->ShowWindow();
		pDockUndockedFrom->RecalcDockLayout();
		pDock->BringWindowToTop();
	}

	inline LRESULT CDocker::OnDockActivated(WPARAM wParam, LPARAM lParam)
	{
		// Redraw captions to take account of focus change
		DrawAllCaptions();

		// The focus change for some controls can be delayed
		SetTimer(TIMER_ID1, 200, NULL);
		SetTimer(TIMER_ID2, 800, NULL);
		
		return CWnd::WndProcDefault(UWM_DOCKACTIVATE, wParam, lParam);
	}

	inline LRESULT CDocker::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_SYSCOMMAND:			return OnSysCommand(wParam, lParam);
		case WM_EXITSIZEMOVE:		return OnExitSizeMove(wParam, lParam);
		case WM_NCLBUTTONDBLCLK:	return OnNCLButtonDblClk(wParam, lParam);
		case WM_SYSCOLORCHANGE:		return OnSysColorChange(wParam, lParam);
		case WM_TIMER:				return OnTimer(wParam, lParam);
		case WM_WINDOWPOSCHANGING:	return OnWindowPosChanging(wParam, lParam);
		case WM_WINDOWPOSCHANGED:	return OnWindowPosChanged(wParam, lParam);

		// Messages defined by Win32++
		case UWM_DOCKACTIVATE:		return OnDockActivated(wParam, lParam);
		case UWM_DOCKDESTROYED:		return OnDockDestroyed(wParam, lParam);

		}

		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	}


	//////////////////////////////////////
	// Declaration of the CDockContainer class
	inline CDockContainer::CDockContainer() : m_iCurrentPage(0), m_hTabIcon(0), 
		                                m_nTabPressed(-1), m_IsHideSingleTab(FALSE)
	{
		m_pContainerParent = this;
	}

	inline CDockContainer::~CDockContainer()
	{
		if (m_hTabIcon != 0)
			DestroyIcon(m_hTabIcon);
	}

	inline void CDockContainer::AddContainer(CDockContainer* pContainer, BOOL bInsert /* = FALSE */)
	// Adds a container to the group. Set bInsert to TRUE to insert the container as the 
	//  first tab, or FALSE to add it as the last tab.
	{
		assert(pContainer);
		assert(this == m_pContainerParent); // Must be performed by parent container

		ContainerInfo ci;
		ci.pContainer = pContainer;
		ci.Title = pContainer->GetTabText();
		ci.iImage = GetODImageList()->Add( pContainer->GetTabIcon() );
		int iNewPage = 0;
		if (bInsert)
		{
			m_vContainerInfo.insert(m_vContainerInfo.begin(), ci);
		}
		else
		{
			iNewPage = (int)m_vContainerInfo.size();
			m_vContainerInfo.push_back(ci);
		}

		if (m_hWnd != 0)
		{
			TCITEM tie;
			ZeroMemory(&tie, sizeof(TCITEM));
			tie.mask = TCIF_TEXT | TCIF_IMAGE;
			tie.iImage = ci.iImage;
			tie.pszText = (LPTSTR)m_vContainerInfo[iNewPage].Title.c_str();
			InsertItem(iNewPage, &tie);
			SelectPage(iNewPage);
			SetTabSize();
		}

		pContainer->m_pContainerParent = this;
		if (pContainer->IsWindow())
		{
			// Set the parent container relationships
			pContainer->GetViewPage()->SetParent(this);
			pContainer->GetViewPage()->ShowWindow(SW_HIDE);
			RecalcLayout();
		}
	}

	inline void CDockContainer::AddToolBarButton(UINT nID, BOOL bEnabled /* = TRUE */)
	// Adds Resource IDs to toolbar buttons.
	// A resource ID of 0 is a separator
	{
		m_vToolBarData.push_back(nID);
		GetToolBar()->AddButton(nID, bEnabled);
	}

	inline CDockContainer* CDockContainer::GetContainerFromIndex(UINT nPage)
	{
		CDockContainer* pContainer = NULL;
		if (nPage < m_vContainerInfo.size())
			pContainer = m_vContainerInfo[nPage].pContainer;

		return pContainer;
	}

	inline CWnd* CDockContainer::GetActiveView() const
	// Returns a pointer to the active view window, or NULL if there is no active veiw.
	{
		CWnd* pWnd = NULL;
		if (m_pContainerParent->m_vContainerInfo.size() > 0)
		{
			CDockContainer* pActiveContainer = m_pContainerParent->m_vContainerInfo[m_pContainerParent->m_iCurrentPage].pContainer;
			if (pActiveContainer->GetViewPage()->GetView()->IsWindow())
				pWnd = pActiveContainer->GetViewPage()->GetView();
		}

		return pWnd;
	}

	inline CDockContainer* CDockContainer::GetContainerFromView(CWnd* pView) const
	{
		assert(pView);

		std::vector<ContainerInfo>::iterator iter;
		CDockContainer* pViewContainer = 0;
		for (iter = GetAllContainers()->begin(); iter != GetAllContainers()->end(); ++iter)
		{
			CDockContainer* pContainer = (*iter).pContainer;
			if (pContainer->GetView() == pView)
				pViewContainer = pContainer;
		}

		return pViewContainer;
	}

	inline int CDockContainer::GetContainerIndex(CDockContainer* pContainer)
	{
		assert(pContainer);
		int iReturn = -1;

		for (int i = 0; i < (int)m_pContainerParent->m_vContainerInfo.size(); ++i)
		{
			if (m_pContainerParent->m_vContainerInfo[i].pContainer == pContainer)
				iReturn = i;
		}

		return iReturn;
	}

	inline SIZE CDockContainer::GetMaxTabTextSize()
	{
		CSize Size;

		// Allocate an iterator for the ContainerInfo vector
		std::vector<ContainerInfo>::iterator iter;

		for (iter = m_vContainerInfo.begin(); iter != m_vContainerInfo.end(); ++iter)
		{
			CSize TempSize;
			CClientDC dc(this);
			NONCLIENTMETRICS info;
			ZeroMemory(&info, sizeof(NONCLIENTMETRICS));
			info.cbSize = GetSizeofNonClientMetrics();
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
			dc.CreateFontIndirect(&info.lfStatusFont);
			TempSize = dc.GetTextExtentPoint32(iter->Title, lstrlen(iter->Title));
			if (TempSize.cx > Size.cx)
				Size = TempSize;
		}

		return Size;
	}

	inline int CDockContainer::GetTabImageID(UINT nTab) const
	{
		assert (nTab < GetAllContainers()->size());
		return (*GetAllContainers())[nTab].iImage;
	}

	inline CString CDockContainer::GetTabText(UINT nTab) const
	{
		assert (nTab < GetAllContainers()->size());
		return (*GetAllContainers())[nTab].Title;
	}

	inline void CDockContainer::SetupToolBar()
	{
		// Use this function to set the Resource IDs for the toolbar(s).

/*		// Set the Resource IDs for the toolbar buttons
		AddToolBarButton( IDM_FILE_NEW   );
		AddToolBarButton( IDM_FILE_OPEN  );
		AddToolBarButton( IDM_FILE_SAVE  );
		AddToolBarButton( 0 );				// Separator
		AddToolBarButton( IDM_EDIT_CUT   );
		AddToolBarButton( IDM_EDIT_COPY  );
		AddToolBarButton( IDM_EDIT_PASTE );
		AddToolBarButton( 0 );				// Separator
		AddToolBarButton( IDM_FILE_PRINT );
		AddToolBarButton( 0 );				// Separator
		AddToolBarButton( IDM_HELP_ABOUT );
*/
	}

	inline void CDockContainer::OnAttach()
	{
		assert(GetView());			// Use SetView in the constructor to set the view window

		// Create and assign the tab's image list
		GetODImageList()->Create(16, 16, ILC_MASK|ILC_COLOR32, 0, 0);

		// Set the tab control's font
		NONCLIENTMETRICS info;
		ZeroMemory(&info, sizeof(NONCLIENTMETRICS));
		info.cbSize = GetSizeofNonClientMetrics();
		SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
		GetTabFont()->CreateFontIndirect(&info.lfStatusFont);
		SetFont(GetTabFont());

		ContainerInfo ci;
		ci.pContainer = this;
		ci.Title = GetTabText();
		ci.iImage = GetODImageList()->Add( GetTabIcon() );
		m_vContainerInfo.push_back(ci);

		// Create the page window
		GetViewPage()->Create(this);

		// Create the toolbar
		GetToolBar()->Create(GetViewPage());
		DWORD style = (DWORD)GetToolBar()->GetWindowLongPtr(GWL_STYLE);
		style |= CCS_NODIVIDER ;
		GetToolBar()->SetWindowLongPtr(GWL_STYLE, (LONG_PTR)style);
		SetupToolBar();
		if (m_vToolBarData.size() > 0)
		{
			// Set the toolbar images
			// A mask of 192,192,192 is compatible with AddBitmap (for Win95)
			if (!GetToolBar()->SendMessage(TB_GETIMAGELIST,  0L, 0L))
				SetToolBarImages(RGB(192,192,192), IDW_MAIN, 0, 0);

			GetToolBar()->SendMessage(TB_AUTOSIZE, 0L, 0L);
		}
		else
			GetToolBar()->Destroy();

		SetFixedWidth(TRUE);
		SetOwnerDraw(TRUE);

		// Add tabs for each container.
		for (int i = 0; i < (int)m_vContainerInfo.size(); ++i)
		{
			// Add tabs for each view.
			TCITEM tie;
			ZeroMemory(&tie, sizeof(TCITEM));
			tie.mask = TCIF_TEXT | TCIF_IMAGE;
			tie.iImage = m_vContainerInfo[i].iImage;
			tie.pszText = (LPTSTR)m_vContainerInfo[i].Title.c_str();
			InsertItem(i, &tie);
		}
	}

	inline LRESULT CDockContainer::OnLButtonDown(WPARAM wParam, LPARAM lParam)
	{
		// Overrides CTab::OnLButtonDown

		UNREFERENCED_PARAMETER(wParam);

		CPoint pt((DWORD)lParam);
		TCHITTESTINFO info;
		ZeroMemory(&info, sizeof(TCHITTESTINFO));
		info.pt = pt;
		m_nTabPressed = HitTest(info);

		return FinalWindowProc(WM_LBUTTONDOWN, wParam, lParam);
	}

	inline LRESULT CDockContainer::OnLButtonUp(WPARAM wParam, LPARAM lParam)
	{
		// Overrides CTab::OnLButtonUp and takes no action

		UNREFERENCED_PARAMETER(wParam);
		UNREFERENCED_PARAMETER(lParam);
		return FinalWindowProc(WM_LBUTTONUP, wParam, lParam);
	}

	inline LRESULT CDockContainer::OnMouseLeave(WPARAM wParam, LPARAM lParam)
	{
		// Overrides CTab::OnMouseLeave

		if (IsLeftButtonDown() && (m_nTabPressed >= 0))
		{
			CDocker* pDock = static_cast<CDocker*>(FromHandlePermanent(::GetParent(GetParent()->GetHwnd())));
			if (dynamic_cast<CDocker*>(pDock))
			{
				CDockContainer* pContainer = GetContainerFromIndex(m_iCurrentPage);
				pDock->UndockContainer(pContainer, GetCursorPos(), TRUE);
			}
		}

		m_nTabPressed = -1;
		return CTab::OnMouseLeave(wParam, lParam);
	}

	inline LRESULT CDockContainer::OnNotifyReflect(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);

		LPNMHDR pNMHDR = (LPNMHDR)lParam;
		switch (pNMHDR->code)
		{
		case TCN_SELCHANGE:	return OnTCNSelChange(pNMHDR);
		}

		return 0L;
	}

	inline LRESULT CDockContainer::OnTCNSelChange(LPNMHDR pNMHDR)
	{
		UNREFERENCED_PARAMETER(pNMHDR);

		// Display the newly selected tab page
		int nPage = GetCurSel();
		SelectPage(nPage);

		return 0L;
	}

	inline LRESULT CDockContainer::OnSetFocus(WPARAM wParam, LPARAM lParam)
	{
		// Sets the focus to the active view (or its child)
		CWnd* pPrevFocus = FromHandle((HWND)wParam);
		if (GetActiveView()->IsChild(pPrevFocus))
		{
			// return focus back to the child of the active view that had it before
			pPrevFocus->SetFocus();
		}
		else
			GetActiveView()->SetFocus();
		
		return FinalWindowProc(WM_SETFOCUS, wParam, lParam);
	}

	inline LRESULT CDockContainer::OnSize(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);
		UNREFERENCED_PARAMETER(lParam);

		RecalcLayout();
		return 0;
	}

	inline void CDockContainer::PreCreate(CREATESTRUCT &cs)
	{
		// For Tabs on the bottom, add the TCS_BOTTOM style
		CTab::PreCreate(cs);
		cs.style |= TCS_BOTTOM;
	}

	inline void CDockContainer::RecalcLayout()
	{
		if (GetContainerParent() == this)
		{
			// Set the tab sizes
			SetTabSize();

			// Position the View over the tab control's display area
			CRect rc = GetClientRect();
			AdjustRect(FALSE, &rc);
			CDockContainer* pContainer = m_vContainerInfo[m_iCurrentPage].pContainer;
			pContainer->GetViewPage()->SetWindowPos(0, rc, SWP_SHOWWINDOW);
			RedrawWindow(NULL, NULL, RDW_INVALIDATE|RDW_NOCHILDREN);
		}
	}

	inline void CDockContainer::RemoveContainer(CDockContainer* pWnd)
	{
		assert(pWnd);

		// Remove the tab
		int iTab = GetContainerIndex(pWnd);
		DeleteItem(iTab);

		// Remove the ContainerInfo entry
		std::vector<ContainerInfo>::iterator iter;
		int iImage = -1;
		for (iter = m_vContainerInfo.begin(); iter != m_vContainerInfo.end(); ++iter)
		{
			if (iter->pContainer == pWnd)
			{
				iImage = (*iter).iImage;
				if (iImage >= 0)
					RemoveImage(iImage);

				m_vContainerInfo.erase(iter);
				break;
			}
		}

		// Set the parent container relationships
		pWnd->GetViewPage()->SetParent(pWnd);
		pWnd->m_pContainerParent = pWnd;

		// Display next lowest page
		m_iCurrentPage = MAX(iTab -1, 0);
		if (IsWindow())
		{
			SelectPage(m_iCurrentPage);
			RecalcLayout();
		}
	}

	inline void CDockContainer::SelectPage(int nPage)
	{
		if (this != m_pContainerParent)
			m_pContainerParent->SelectPage(nPage);
		else
		{
			if ((nPage >= 0) && (nPage < (int)m_vContainerInfo.size() ))
			{
				if (GetCurSel() != nPage)
					SetCurSel(nPage);

				// Create the new container window if required
				if (!m_vContainerInfo[nPage].pContainer->IsWindow())
				{
					CDockContainer* pContainer = m_vContainerInfo[nPage].pContainer;
					pContainer->Create(GetParent());
					pContainer->GetViewPage()->SetParent(this);
				}

				// Determine the size of the tab page's view area
				CRect rc = GetClientRect();
				AdjustRect(FALSE, &rc);

				// Swap the pages over
				CDockContainer* pNewContainer = m_vContainerInfo[nPage].pContainer;
				for (std::vector<ContainerInfo>::iterator it = m_vContainerInfo.begin(); it != m_vContainerInfo.end(); ++it)
				{
					(*it).pContainer->GetViewPage()->ShowWindow(SW_HIDE);
				}

				pNewContainer->GetViewPage()->SetWindowPos(0, rc, SWP_SHOWWINDOW);
				pNewContainer->GetViewPage()->GetView()->SetFocus();

				// Adjust the docking caption
				CDocker* pDock = static_cast<CDocker*>(FromHandlePermanent(::GetParent(::GetParent(m_hWnd))));
				if (dynamic_cast<CDocker*>(pDock))
				{
					pDock->SetCaption(pNewContainer->GetDockCaption());
					pDock->RedrawWindow();
				}

				m_iCurrentPage = nPage;
			}
		}
	}

	inline void CDockContainer::SetActiveContainer(CDockContainer* pContainer)
	{
		int nPage = GetContainerIndex(pContainer);
		assert (0 <= nPage);
		SelectPage(nPage);
	}

	inline void CDockContainer::SetHideSingleTab(BOOL HideSingle)
	// Only display tabs if there are two or more.
	{	
		m_IsHideSingleTab = HideSingle;
		RecalcLayout();	
	}

	inline void CDockContainer::SetTabIcon(UINT nID_Icon)
	{
		HICON hIcon = (HICON)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(nID_Icon), IMAGE_ICON, 0, 0, LR_SHARED);
		SetTabIcon(hIcon);
	}

	inline void CDockContainer::SetTabSize()
	{
		CRect rc = GetClientRect();
		AdjustRect(FALSE, &rc);
		if (rc.Width() < 0 )
			rc.SetRectEmpty();

		int nItemWidth = 0;
		int nItemHeight = 1;
		if ((GetItemCount() != 1) || !m_IsHideSingleTab)
		{
			nItemWidth = MIN(25 + GetMaxTabTextSize().cx, (rc.Width()-2)/(int)m_vContainerInfo.size());
			nItemHeight = MAX(20, GetTextHeight() + 5);
		}
		SendMessage(TCM_SETITEMSIZE, 0L, MAKELPARAM(nItemWidth, nItemHeight));
	}

	inline void CDockContainer::SetTabText(UINT nTab, LPCTSTR szText)
	{
		CDockContainer* pContainer = GetContainerParent()->GetContainerFromIndex(nTab);
		pContainer->SetTabText(szText);

		CTab::SetTabText(nTab, szText);
	}

	inline void CDockContainer::SetToolBarImages(COLORREF crMask, UINT ToolBarID, UINT ToolBarHotID, UINT ToolBarDisabledID)
	// Either sets the imagelist or adds/replaces bitmap depending on ComCtl32.dll version
	// Assumes the width of the button image = height, minimum width = 16
	// The colour mask is ignored for 32bit bitmaps, but is required for 24bit bitmaps
	// The colour mask is often grey RGB(192,192,192) or magenta (255,0,255)
	// The hot and disabled bitmap resources can be 0
	// A disabled image list is created from the normal image list if one isn;t provided.
	{
		// ToolBar ImageLists require Comctl32.dll version 4.7 or later
		if (GetComCtlVersion() < 470)
		{
			// We are using COMCTL32.DLL version 4.0, so we can't use an ImageList.
			// Instead we simply set the bitmap.
			GetToolBar()->SetBitmap(ToolBarID);
			return;
		}

		// Set the button images
		CBitmap Bitmap(ToolBarID);
		assert(Bitmap.GetHandle());

		BITMAP bm = Bitmap.GetBitmapData();
		int cy = bm.bmHeight;
		int cx  = MAX(bm.bmHeight, 16);

		m_imlToolBar.DeleteImageList();
		m_imlToolBar.Create(cx, cy, ILC_COLOR32 | ILC_MASK, 0, 0);
		m_imlToolBar.Add( &Bitmap, crMask );
		GetToolBar()->SetImageList(&m_imlToolBar);

		if (ToolBarHotID)
		{
			CBitmap BitmapHot(ToolBarHotID);
			assert(BitmapHot);

			m_imlToolBarHot.DeleteImageList();
			m_imlToolBarHot.Create(cx, cy, ILC_COLOR32 | ILC_MASK, 0, 0);
			m_imlToolBarHot.Add( &BitmapHot, crMask );
			GetToolBar()->SetHotImageList(&m_imlToolBarHot);
		}

		if (ToolBarDisabledID)
		{
			CBitmap BitmapDisabled(ToolBarDisabledID);
			assert(BitmapDisabled);

			m_imlToolBarDis.DeleteImageList();
			m_imlToolBarDis.Create(cx, cy, ILC_COLOR32 | ILC_MASK, 0, 0);
			m_imlToolBarDis.Add( &BitmapDisabled, crMask );
			GetToolBar()->SetDisableImageList( &m_imlToolBarDis );
		}
		else
		{
			m_imlToolBarDis.DeleteImageList();
			m_imlToolBarDis.CreateDisabledImageList(&m_imlToolBar);
			GetToolBar()->SetDisableImageList( &m_imlToolBarDis );
		}
	}

	inline void CDockContainer::SetView(CWnd& Wnd)
	{
		GetViewPage()->SetView(Wnd);
	}

	inline void CDockContainer::SwapTabs(UINT nTab1, UINT nTab2)
	{
		if ((nTab1 < GetAllContainers()->size()) && (nTab2 < GetAllContainers()->size()) && (nTab1 != nTab2))
		{
			ContainerInfo CI1 = m_vContainerInfo[nTab1];
			ContainerInfo CI2 = m_vContainerInfo[nTab2];

			TCITEM Item1;
			ZeroMemory(&Item1, sizeof(TCITEM));
			Item1.mask = TCIF_IMAGE | TCIF_PARAM | TCIF_RTLREADING | TCIF_STATE | TCIF_TEXT;
			Item1.cchTextMax = CI1.Title.GetLength()+1;
			Item1.pszText = const_cast<LPTSTR>(CI1.Title.c_str());
			GetItem(nTab1, &Item1);

			TCITEM Item2;
			ZeroMemory(&Item2, sizeof(TCITEM));
			Item2.mask = TCIF_IMAGE | TCIF_PARAM | TCIF_RTLREADING | TCIF_STATE | TCIF_TEXT;
			Item2.cchTextMax = CI2.Title.GetLength()+1;
			Item2.pszText = const_cast<LPTSTR>(CI2.Title.c_str());
			GetItem(nTab2, &Item2);

			SetItem(nTab1, &Item2);
			SetItem(nTab2, &Item1);
			m_vContainerInfo[nTab1] = CI2;
			m_vContainerInfo[nTab2] = CI1;
		}
	}


	inline LRESULT CDockContainer::OnMouseMove(WPARAM wParam, LPARAM lParam)
	{
		CPoint pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		// Skip if mouse hasn't moved
		if ((pt.x == m_OldMousePos.x) && (pt.y == m_OldMousePos.y))
			return FALSE;

		m_OldMousePos.x = pt.x;
		m_OldMousePos.y = pt.y;

		if (IsLeftButtonDown())
		{
			TCHITTESTINFO info;
			ZeroMemory(&info, sizeof(TCHITTESTINFO));
			info.pt = CPoint((DWORD)lParam);
			int nTab = HitTest(info);
			if (nTab >= 0)
			{
				if (nTab !=  m_nTabPressed)
				{
					SwapTabs(nTab, m_nTabPressed);
					m_nTabPressed = nTab;
					SelectPage(nTab);
				}
			}
		}

		return CTab::OnMouseMove(wParam, lParam);
	}

	inline LRESULT CDockContainer::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:	return OnLButtonDown(wParam, lParam);
		case WM_LBUTTONUP:		return OnLButtonUp(wParam, lParam);
		case WM_MOUSELEAVE:		return OnMouseLeave(wParam, lParam);
		case WM_MOUSEMOVE:		return OnMouseMove(wParam, lParam);
		case WM_SETFOCUS:		return OnSetFocus(wParam, lParam);
		case WM_SIZE:			return OnSize(wParam, lParam);		
		} 

		// pass unhandled messages on to CTab for processing
		return CTab::WndProcDefault(uMsg, wParam, lParam);
	}


	///////////////////////////////////////////
	// Declaration of the nested CViewPage class
	inline BOOL CDockContainer::CViewPage::OnCommand(WPARAM wParam, LPARAM lParam)
	{
		CDockContainer* pContainer = static_cast<CDockContainer*>(GetParent());
		assert(dynamic_cast<CDockContainer*>(pContainer));
		BOOL bResult = FALSE;
		if (pContainer && pContainer->GetActiveContainer() && pContainer->GetActiveContainer()->IsWindow())
			bResult = (BOOL)pContainer->GetActiveContainer()->SendMessage(WM_COMMAND, wParam, lParam);

		return bResult;
	}

	inline int CDockContainer::CViewPage::OnCreate(LPCREATESTRUCT pcs)
	{
		UNREFERENCED_PARAMETER(pcs);
		if (m_pView)
			m_pView->Create(this);

		return 0;
	}

	inline LRESULT CDockContainer::CViewPage::OnNotify(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);

		switch (((LPNMHDR)lParam)->code)
		{

		// Display tooltips for the toolbar
		case TTN_GETDISPINFO:
			{
				int iIndex =  GetToolBar()->HitTest();
				LPNMTTDISPINFO lpDispInfo = (LPNMTTDISPINFO)lParam;
				if (iIndex >= 0)
				{
					int nID = GetToolBar()->GetCommandID(iIndex);
					if (nID > 0)
					{
						m_strTooltip = LoadString(nID);
						lpDispInfo->lpszText = (LPTSTR)m_strTooltip.c_str();
					}
					else
						m_strTooltip = _T("");
				}
			}
			break;
		case NM_CUSTOMDRAW:
			{
				if (((LPNMHDR)lParam)->hwndFrom == GetToolBar()->GetHwnd())
				{
					// Pass Toolbar's custom draw up to CFrame
					return GetAncestor()->SendMessage(WM_NOTIFY, wParam, lParam);
				}
			}
			break;
		} // switch LPNMHDR

		return 0L;
	}

	inline void CDockContainer::CViewPage::PreRegisterClass(WNDCLASS &wc)
	{
		wc.lpszClassName = _T("Win32++ TabPage");
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	}

	inline void CDockContainer::CViewPage::RecalcLayout()
	{
		CRect rc = GetClientRect();
		if (GetToolBar()->IsWindow())
		{
			GetToolBar()->SendMessage(TB_AUTOSIZE, 0L, 0L);
			CRect rcToolBar = GetToolBar()->GetClientRect();
			rc.top += rcToolBar.Height();
		}

		GetView()->SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
	}

	inline void CDockContainer::CViewPage::SetView(CWnd& wndView)
	// Sets or changes the View window displayed within the frame
	{
		if (m_pView != &wndView)
		{
			// Hide the existing view window (if any)
			if (m_pView && m_pView->IsWindow()) m_pView->ShowWindow(SW_HIDE);

			// Assign the view window
			m_pView = &wndView;

			if (m_hWnd != 0)
			{
				// The frame is already created, so create and position the new view too
				assert(GetView());			// Use SetView in CMainFrame's constructor to set the view window

				if (!GetView()->IsWindow())
					GetView()->Create(this);
				else
				{
					GetView()->SetParent(this);
					GetView()->ShowWindow();
				}

				RecalcLayout();
			}
		}
	}


	inline LRESULT CDockContainer::CViewPage::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_SIZE:
			RecalcLayout();
			break;
		}

		// pass unhandled messages on for default processing
		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	}

} // namespace Win32xx

#endif // _WIN32XX_DOCKING_H_

