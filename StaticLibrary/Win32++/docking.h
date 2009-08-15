// Win32++  Version 6.6
// Released: 17th August, 2009 by:
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
			virtual void DrawCaption(WPARAM wParam, BOOL bFocus);
			virtual void DrawCloseButton(CDC& DrawDC, UINT uState, BOOL bFocus);
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
			virtual void    OnMouseActivate(WPARAM wParam, LPARAM lParam);
			virtual void    OnNCCalcSize(WPARAM& wParam, LPARAM& lParam);
			virtual LRESULT OnNCHitTest(WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnNCLButtonDown(WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnNCLButtonUp(WPARAM wParam, LPARAM lParam);
			virtual void    OnNCMouseLeave(WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnNCMouseMove(WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnNCPaint(WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
			virtual void    OnWindowPosChanged(WPARAM wParam, LPARAM lParam);
			virtual void    PreRegisterClass(WNDCLASS& wc);
			virtual void    PreCreate(CREATESTRUCT& cs);
			virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

		private:
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
			virtual void ShowHintWindow(CDocker* pDockTarget, CRect rcHint);

			void SetBitmap(HBITMAP hbmBlueTint);

		private:
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
			CDocker* m_pOldDockTarget;
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
		void CheckAllTargets(LPDRAGPOS pDragPos);
		void CloseAllTargets();
		void Dock(CDocker* hDocker, UINT uDockSide);
		void DockInContainer(CDocker* pDock, DWORD dwDockStyle);
		void DockOuter(CDocker* pDocker, DWORD dwDockStyle);
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


#endif // _DOCKING_H_

