// Win32++   Version 8.2
// Release Date: TBA
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2016  David Nash
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


////////////////////////////////////////////////////////
// wxx_mdi.h
//  Declaration of the CMDIChild and CMDIFrame classes

// The classes defined here add MDI frames support to Win32++. MDI
// (Multiple Document Interface) frames host one or more child windows. The
// child windows hosted by a MDI frame can be different types. For example,
// some MDI child windows could be used to edit text, while others could be
// used to display a bitmap. Four classes are defined here to support MDI
// frames:


// 1) CMDIFrame. This class inherits from CFrame, and adds the functionality
//    required by MDI frames. It keeps track of the MDI children created and
//    destroyed, and adjusts the menu when a MDI child is activated. Use the
//    AddMDIChild function to add MDI child windows to the MDI frame. Inherit
//    from CMDIFrame to create your own MDI frame.
//
// 2) CMDIChild: All MDI child windows (ie. CWnd classes) should inherit from
//    this class. Each MDI child type can have a different frame menu.

// Use the MDIFrame generic application as the starting point for your own MDI
// frame applications.
// Refer to the MDIDemo sample for an example on how to use these classes to
// create a MDI frame application with different types of MDI child windows.


#ifndef _WIN32XX_MDI_H_
#define _WIN32XX_MDI_H_

#include "wxx_frame.h"
#include <vector>


namespace Win32xx
{
    class CMDIChild;
    class CMDIFrame;
	typedef Shared_Ptr<CMDIChild> MDIChildPtr;

	/////////////////////////////////////
	// Declaration of the CMDIChild class
	//
	class CMDIChild : public CWnd
	{
		friend class CMDIFrame;
	public:
		CMDIChild(); 
		virtual ~CMDIChild();

		// These are the functions you might wish to override
		virtual HWND Create(HWND hWndParent = NULL);
		virtual void RecalcLayout();
	
		// These functions aren't virtual, and shouldn't be overridden
		CWnd& GetView() const			{ assert(m_pView); return *m_pView; }
		void SetView(CWnd& pwndView);
		void SetHandles(HMENU MenuName, HACCEL AccelName);
		CMDIFrame& GetMDIFrame() const;
		void MDIActivate() const;
		void MDIDestroy() const;
		void MDIMaximize() const;
		void MDIRestore() const;

	protected:
		// These are the functions you might wish to override
		virtual void OnClose();
		virtual int  OnCreate(CREATESTRUCT& cs);
		virtual LRESULT OnMDIActivate(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam);
		
		// Not intended to be overridden
		virtual LRESULT FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		CMDIChild(const CMDIChild&);				// Disable copy construction
		CMDIChild& operator = (const CMDIChild&); // Disable assignment operator

		CMDIFrame* m_pMDIFrame;		// pointer to the MDI Frame object
		CWnd* m_pView;				// pointer to the View CWnd object
		CMenu m_ChildMenu;
		HACCEL m_hChildAccel;
	};


	/////////////////////////////////////
	// Declaration of the CMDIFrame class
	//
	class CMDIFrame : public CFrame
	{
		friend class CMDIChild;     // CMDIChild uses m_hOrigMenu

	public:
		// A nested class inside CMDIFrame
		class CMDIClient : public CDocker::CDockClient
		{
			friend class CMDIFrame;

		public:
			CMDIClient() : m_pMDIFrame(0) {}
			virtual ~CMDIClient() {}
			CMDIFrame& GetMDIFrame() const; 

		protected:
			// Overridable virtual functions
			virtual HWND Create(HWND hWndParent);
			virtual LRESULT OnMDIActivate(UINT uMsg, WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnMDIDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam);
			virtual LRESULT OnMDISetMenu(UINT uMsg, WPARAM wParam, LPARAM lParam);
			virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

		private:
			CMDIFrame* m_pMDIFrame;
		};


		CMDIFrame();
		virtual ~CMDIFrame() {}

		// Overridable virtual functions
		virtual CMDIChild& AddMDIChild(CMDIChild* pMDIChild);
		virtual CMDIChild* GetActiveMDIChild() const;
		virtual CMenu GetActiveMenu() const;
		virtual CMDIClient& GetMDIClient() const { return const_cast<CMDIClient&>(m_MDIClient); }
		virtual BOOL IsMDIChildMaxed() const;
		virtual BOOL IsMDIFrame() const { return TRUE; }
		virtual void RemoveMDIChild(HWND hWnd);
		virtual BOOL RemoveAllMDIChildren();

		// These functions aren't virtual. Don't override these
		const std::vector<MDIChildPtr>& GetAllMDIChildren() const {return m_vMDIChild;}
		void MDICascade(int nType = 0) const;
		void MDIIconArrange() const;
		void MDIMaximize() const;
		void MDINext() const;
		void MDIPrev() const;
		void MDIRestore() const;
		void MDITile(int nType = 0) const;
		void SetActiveMDIChild(CMDIChild* pChild);

	protected:		
		// Overridable virtual functions
		virtual LRESULT FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual void    OnClose();
		virtual int		OnCreate(CREATESTRUCT& cs);
		virtual LRESULT OnInitMenuPopup(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual void    OnMenuUpdate(UINT nID);
		virtual BOOL    OnViewStatusBar();
		virtual BOOL    OnViewToolBar();
		virtual LRESULT OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual void    RecalcLayout();
		virtual BOOL    PreTranslateMessage(MSG& Msg);
		
		// Not intended to be overridden	
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		CMDIFrame(const CMDIFrame&);				// Disable copy construction
		CMDIFrame& operator = (const CMDIFrame&);	// Disable assignment operator
		void AppendMDIMenu(CMenu MenuWindow);
		void UpdateFrameMenu(CMenu Menu);

		std::vector<MDIChildPtr> m_vMDIChild;
		CMDIClient m_MDIClient;
		HWND m_hActiveMDIChild;
	};

	/////////////////////////////////////////
	// Declaration of the CMDIDockFrame class
	//
	class CMDIDockFrame : public CMDIFrame, public CDocker
	{
	public:
		CMDIDockFrame()						{ SetView(GetMDIClient()); }
		CRect GetViewRect() const			{ return CMDIFrame::GetViewRect(); }
		CWnd& GetView() const				{ return CMDIFrame::GetView(); }
		void RecalcViewLayout()				{ RecalcDockLayout(); }
		
		operator HWND() const				{ return GetHwnd(); }
		
		virtual CDocker::CDockClient& GetDockClient() const	
		{ 
			return const_cast<CMDIClient&>(GetMDIClient());
		}
		
		virtual void SetView(CWnd&)
		{ 
			// A MDI Frame's view is always the MDIClient window
			CMDIFrame::SetView(GetMDIClient()); 
			CDocker::SetView(GetMDIClient()); 
		}

	protected:
		void OnClose()						{ CMDIFrame::OnClose(); }
		int OnCreate(CREATESTRUCT& cs)		{ return CMDIFrame::OnCreate(cs); }
		void OnDestroy()					{ CDocker::OnDestroy(); CMDIFrame::OnDestroy(); }
		void OnMenuUpdate(UINT nID)			{ CMDIFrame::OnMenuUpdate(nID); }
		void PreCreate(CREATESTRUCT& cs)	{ CMDIFrame::PreCreate(cs); }
		void PreRegisterClass(WNDCLASS& wc) { CMDIFrame::PreRegisterClass(wc); }
		BOOL PreTranslateMessage(MSG& Msg)	{ return CMDIFrame::PreTranslateMessage(Msg); }
		LRESULT FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
		{ 
			return CMDIFrame::FinalWindowProc(uMsg, wParam, lParam); 
		}

		LRESULT OnNotify(WPARAM wParam, LPARAM lParam)
		// Called when a notification from a child window (WM_NOTIFY) is received.
		{
			LRESULT lr = CFrame::OnNotify(wParam, lParam);
			if (lr == 0)
				lr = CDocker::OnNotify(wParam, lParam);

			return lr;
		}

		LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
		// Handle the frame's window messages.
		{
			switch (uMsg)
			{

			case WM_WINDOWPOSCHANGED:	return CMDIFrame::OnWindowPosChanged(uMsg, wParam, lParam);
			case UWM_ISMDIFRAME:		return TRUE;		

			case WM_ACTIVATE:		return CFrame::OnActivate(uMsg, wParam, lParam);
			case WM_DRAWITEM:		return CFrame::OnDrawItem(uMsg, wParam, lParam);
			case WM_ERASEBKGND:		return 0L;
			case WM_EXITMENULOOP:	return CFrame::OnExitMenuLoop(uMsg, wParam, lParam);
			case WM_HELP:			return CFrame::OnHelp();
			case WM_INITMENUPOPUP:	return CFrame::OnInitMenuPopup(uMsg, wParam, lParam);
			case WM_MENUCHAR:		return CFrame::OnMenuChar(uMsg, wParam, lParam);
			case WM_MEASUREITEM:	return CFrame::OnMeasureItem(uMsg, wParam, lParam);
			case WM_MENUSELECT:		return CFrame::OnMenuSelect(uMsg, wParam, lParam);
			case WM_SETFOCUS:		return CFrame::OnSetFocus(uMsg, wParam, lParam);
			case WM_SIZE:			return CFrame::OnSize(uMsg, wParam, lParam);
			case WM_SYSCOLORCHANGE:	return CFrame::OnSysColorChange(uMsg, wParam, lParam);
			case WM_SYSCOMMAND:		return CFrame::OnSysCommand(uMsg, wParam, lParam);
		//	case WM_WINDOWPOSCHANGED: return CFrame::FinalWindowProc(uMsg, wParam, lParam);

			// Messages defined by Win32++
			case UWM_GETFRAMEVIEW:	return reinterpret_cast<LRESULT>(GetView().GetHwnd());
			case UWM_GETMBTHEME:	return reinterpret_cast<LRESULT>(&GetMenuBarTheme());
			case UWM_GETRBTHEME:	return reinterpret_cast<LRESULT>(&GetReBarTheme());
			case UWM_GETSBTHEME:	return reinterpret_cast<LRESULT>(&GetStatusBarTheme());
			case UWM_GETTBTHEME:	return reinterpret_cast<LRESULT>(&GetToolBarTheme());
			case UWM_DRAWRBBKGND:   return CFrame::DrawReBarBkgnd(*((CDC*) wParam), *((CReBar*) lParam));
			case UWM_DRAWSBBKGND:   return CFrame::DrawStatusBarBkgnd(*((CDC*) wParam), *((CStatusBar*) lParam));
			case UWM_ISFRAME:		return TRUE;

			} // switch uMsg

			return CDocker::WndProcDefault(uMsg, wParam, lParam);
		}

	};

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{

	/////////////////////////////////////
	// Definitions for the CMDIFrame class
	//
	inline CMDIFrame::CMDIFrame() : m_hActiveMDIChild(NULL)
	{
		SetView(GetMDIClient());
	}

	inline CMDIChild& CMDIFrame::AddMDIChild(CMDIChild* pMDIChild)
	// Adds a MDI child to the MDI frame. The pointer to the MDI child will be 
	//  automatically deleted when the MDI Frame destroys the MDI child.
	{
		assert(NULL != pMDIChild); // Cannot add Null MDI Child

		m_vMDIChild.push_back(MDIChildPtr(pMDIChild));
		pMDIChild->m_pMDIFrame = this;
		pMDIChild->Create(GetMDIClient());

		return *pMDIChild;
	}

	inline void CMDIFrame::AppendMDIMenu(CMenu MenuWindow)
	// Adds the additional menu items the the "Window" submenu when
	//  MDI child windows are created	
	{
		if (!MenuWindow.GetHandle())
			return;

		// Delete previously appended items
		int nItems = MenuWindow.GetMenuItemCount();
		UINT uLastID = MenuWindow.GetMenuItemID(--nItems);
		if ((uLastID >= IDW_FIRSTCHILD) && (uLastID < IDW_FIRSTCHILD + 10))
		{
			while ((uLastID >= IDW_FIRSTCHILD) && (uLastID < IDW_FIRSTCHILD + 10))
			{
				MenuWindow.DeleteMenu(nItems, MF_BYPOSITION);
				uLastID = MenuWindow.GetMenuItemID(--nItems);
			}
			
			//delete the separator too
			MenuWindow.DeleteMenu(nItems, MF_BYPOSITION);
		}

		int nWindow = 0;

		// Allocate an iterator for our MDIChild vector
		std::vector<MDIChildPtr>::const_iterator v;

		for (v = GetAllMDIChildren().begin(); v < GetAllMDIChildren().end(); ++v)
		{
			if ((*v)->IsWindowVisible())
			{
				// Add Separator
				if (nWindow == 0)
					MenuWindow.AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)NULL);

				// Add a menu entry for each MDI child (up to 9)
				if (nWindow < 9)
				{
					CString strMenuItem ( (*v)->GetWindowText() );

					if (strMenuItem.GetLength() > MAX_MENU_STRING -10)
					{
						// Truncate the string if its too long
						strMenuItem.Delete(strMenuItem.GetLength() - MAX_MENU_STRING +10);
						strMenuItem += _T(" ...");
					}

					CString MenuString;
					MenuString.Format(_T("&%d %s"), nWindow+1, strMenuItem.c_str());

					MenuWindow.AppendMenu(MF_STRING, IDW_FIRSTCHILD + nWindow, MenuString);

					if (GetActiveMDIChild() == (*v).get())
						MenuWindow.CheckMenuItem(IDW_FIRSTCHILD+nWindow, MF_CHECKED);

					++nWindow;
				}
				else if (9 == nWindow)
				// For the 10th MDI child, add this menu item and return
				{
					MenuWindow.AppendMenu(MF_STRING, IDW_FIRSTCHILD + nWindow, _T("&Windows..."));
					return;
				}
			}
		}
	}

	inline CMenu CMDIFrame::GetActiveMenu() const
	// Returns a pointer to the menu of the Active MDI Child if any,
	// otherwise returns a pointer to the MDI Frame's menu
	{
		CMenu Menu = GetFrameMenu();
		
		if(GetActiveMDIChild())
			if (GetActiveMDIChild()->m_ChildMenu.GetHandle())
				Menu = GetActiveMDIChild()->m_ChildMenu;

		return Menu;
	}

	inline LRESULT CMDIFrame::FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefFrameProc(*this, GetMDIClient(), uMsg, wParam, lParam);
	}

	inline CMDIChild* CMDIFrame::GetActiveMDIChild() const
	// Returns a pointer to the active MDI child
	{
		return static_cast<CMDIChild*>(GetCWndPtr(m_hActiveMDIChild));
	}

	inline BOOL CMDIFrame::IsMDIChildMaxed() const
	// Returns TRUE if a MDI child is maximized
	{
		BOOL bMaxed = FALSE;
		GetMDIClient().SendMessage(WM_MDIGETACTIVE, 0L, (LPARAM)&bMaxed);
		return bMaxed;
	}

	inline void CMDIFrame::MDICascade(int nType /* = 0*/) const
	// Possible values for nType are:
	// MDITILE_SKIPDISABLED	Prevents disabled MDI child windows from being cascaded.	
	{
		assert(IsWindow());
		GetMDIClient().SendMessage(WM_MDICASCADE, (WPARAM)nType, 0L);
	}

	inline void CMDIFrame::MDIIconArrange() const
	// Re-arranges the icons for minimized MDI children
	{
		assert(IsWindow());
		GetMDIClient().SendMessage(WM_MDIICONARRANGE, 0L, 0L);
	}

	inline void CMDIFrame::MDIMaximize() const
	// Maximize the MDI child
	{
		assert(IsWindow());
		GetMDIClient().SendMessage(WM_MDIMAXIMIZE, 0L, 0L);
	}

	inline void CMDIFrame::MDINext() const
	// Activates the next MDI child
	{
		assert(IsWindow());
		HWND hMDIChild = GetActiveMDIChild()->GetHwnd();
		GetMDIClient().SendMessage(WM_MDINEXT, (WPARAM)hMDIChild, FALSE);
	}

	inline void CMDIFrame::MDIPrev() const
	// Activates the previous MDI child
	{
		assert(IsWindow());
		HWND hMDIChild = GetActiveMDIChild()->GetHwnd();
		GetMDIClient().SendMessage(WM_MDINEXT, (WPARAM)hMDIChild, TRUE);
	}

	inline void CMDIFrame::MDIRestore() const
	// Restores a mimimized MDI child
	{
		assert(IsWindow());
		GetMDIClient().SendMessage(WM_MDIRESTORE, 0L, 0L);
	}

	inline void CMDIFrame::MDITile(int nType /* = 0*/) const
	{
		// Possible values for nType are:
		// MDITILE_HORIZONTAL	Tiles MDI child windows so that one window appears above another.
		// MDITILE_SKIPDISABLED	Prevents disabled MDI child windows from being tiled.
		// MDITILE_VERTICAL		Tiles MDI child windows so that one window appears beside another.

		assert(IsWindow());
		GetMDIClient().SendMessage(WM_MDITILE, (WPARAM)nType, 0L);
	}

	inline void CMDIFrame::OnClose()
	{
		if (RemoveAllMDIChildren())
		{
			CFrame::OnClose();
		}
	}

	inline int CMDIFrame::OnCreate(CREATESTRUCT& cs)
	{	
		GetMDIClient().m_pMDIFrame = this;
		return CFrame::OnCreate(cs);
	}

	inline LRESULT CMDIFrame::OnInitMenuPopup(UINT uMsg, WPARAM wParam, LPARAM lParam)
	// Called when the menu's modal loop begins (WM_INITMENUPOPUP received)
	{
		if (IsMDIChildMaxed())
		{
			CWnd* pMaxMDIChild = GetActiveMDIChild();
			assert(pMaxMDIChild);

			// Suppress owner drawing of the MDI child's system menu
			if (::GetSystemMenu(*pMaxMDIChild, FALSE) == (HMENU) wParam)
				return CWnd::WndProcDefault(WM_INITMENUPOPUP, wParam, lParam);
		}

		return CFrame::OnInitMenuPopup(uMsg, wParam, lParam);
	}

	inline void CMDIFrame::OnMenuUpdate(UINT nID)
	{
		// Updates the check buttons before displaying the menu

		switch (nID)
		{
		case IDW_VIEW_STATUSBAR:
			{
				BOOL IsVisible = GetStatusBar().IsWindow() && GetStatusBar().IsWindowVisible();
				GetActiveMenu().CheckMenuItem(nID, IsVisible ? MF_CHECKED : MF_UNCHECKED);
			}
			break;

		case IDW_VIEW_TOOLBAR:
			{
				BOOL IsVisible = GetToolBar().IsWindow() && GetToolBar().IsWindowVisible();
				GetActiveMenu().EnableMenuItem(nID, GetUseToolBar() ? MF_ENABLED : MF_DISABLED);
				GetActiveMenu().CheckMenuItem(nID, IsVisible ? MF_CHECKED : MF_UNCHECKED);
			}
			break;
		}

	}

	inline BOOL CMDIFrame::OnViewStatusBar()
	{
		CFrame::OnViewStatusBar();
		GetView().RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
		return TRUE;
	}

	inline BOOL CMDIFrame::OnViewToolBar()
	{
		CFrame::OnViewToolBar();
		GetView().RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
		return TRUE;
	}

	inline LRESULT CMDIFrame::OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// MDI Child or MDI frame has been resized

		if (IsMenuBarUsed())
		{
			// Refresh MenuBar Window
			GetMenuBar().SetMenu(GetMenuBar().GetMenu());
		}

		return FinalWindowProc(uMsg, wParam, lParam);
	}

	inline BOOL CMDIFrame::PreTranslateMessage(MSG& Msg)
	{
		if (WM_KEYFIRST <= Msg.message && Msg.message <= WM_KEYLAST)
		{
			if (TranslateMDISysAccel(GetView().GetHwnd(), &Msg))
				return TRUE;
		}

		return CFrame::PreTranslateMessage(Msg);
	}

	inline void CMDIFrame::RecalcLayout()
	{
		CFrame::RecalcLayout();

		if (GetView().IsWindow())
			MDIIconArrange();
	}

	inline BOOL CMDIFrame::RemoveAllMDIChildren()
	{
		BOOL bResult = TRUE;
		int Children = (int)m_vMDIChild.size();

		// Remove the children in reverse order
		for (int i = Children-1; i >= 0; --i)
		{
			MDIChildPtr pMDIChild = m_vMDIChild[i];

			// Ask the window to close. If it is destroyed, RemoveMDIChild gets called.
			pMDIChild->SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0L);	

			if (pMDIChild->IsWindow())
				bResult = FALSE;
		}

		return bResult;
	}

	inline void CMDIFrame::RemoveMDIChild(HWND hWnd)
	{
		// Allocate an iterator for our HWND map
		std::vector<MDIChildPtr>::iterator v;

		for (v = m_vMDIChild.begin(); v!= m_vMDIChild.end(); ++v)
		{
			if ((*v)->GetHwnd() == hWnd)
			{
				m_vMDIChild.erase(v);
				break;
			}
		}

		if (GetActiveMDIChild())
		{
			if (GetActiveMDIChild()->m_ChildMenu.GetHandle())
				UpdateFrameMenu(GetActiveMDIChild()->m_ChildMenu);
			if (GetActiveMDIChild()->m_hChildAccel)
				GetApp().SetAccelerators(GetActiveMDIChild()->m_hChildAccel, *this);
		}
		else
		{
			if (IsMenuBarUsed())
				GetMenuBar().SetMenu( GetFrameMenu() );
			else
				SetMenu( GetFrameMenu() );

			GetApp().SetAccelerators(GetFrameAccel(), *this);
		}
	}

	inline void CMDIFrame::SetActiveMDIChild(CMDIChild* pChild)
	{
		assert ( pChild->IsWindow() );

		GetMDIClient().SendMessage(WM_MDIACTIVATE, (WPARAM)pChild->GetHwnd(), 0L);

		// Verify
		assert ( m_hActiveMDIChild == pChild->GetHwnd() );
	}

	inline void CMDIFrame::UpdateFrameMenu(CMenu Menu)
	{
		int nMenuItems = Menu.GetMenuItemCount();
		if (nMenuItems > 0)
		{
			// The Window menu is typically second from the right
			int nWindowItem = MAX (nMenuItems -2, 0);
			CMenu MenuWindow = Menu.GetSubMenu(nWindowItem);

			if (MenuWindow.GetHandle())
			{
				if (IsMenuBarUsed())
				{
					AppendMDIMenu(MenuWindow);
					GetMenuBar().SetMenu(Menu);
				}
				else
				{
					GetMDIClient().SendMessage(WM_MDISETMENU, (WPARAM)Menu.GetHandle(), (LPARAM)MenuWindow.GetHandle());
					DrawMenuBar();
				}
			}
		}
	}

	inline LRESULT CMDIFrame::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			case WM_WINDOWPOSCHANGED:	return OnWindowPosChanged(uMsg, wParam, lParam);
			case UWM_ISMDIFRAME:		return TRUE;
		}
		
		return CFrame::WndProcDefault(uMsg, wParam, lParam);
	}


	/////////////////////////////////////
	//Definitions for the CDockMDIChild class
	//

	inline HWND CMDIFrame::CMDIClient::Create(HWND hWndParent)
	{
		assert(hWndParent != 0);

		CLIENTCREATESTRUCT clientcreate;
		clientcreate.hWindowMenu  = 0;
		clientcreate.idFirstChild = IDW_FIRSTCHILD;
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | MDIS_ALLCHILDSTYLES;

		// Create the view window
		return CreateEx(WS_EX_CLIENTEDGE, _T("MDICLient"), _T(""), dwStyle, 0, 0, 0, 0, hWndParent, NULL, (PSTR) &clientcreate);
	}

	inline CMDIFrame& CMDIFrame::CMDIClient::GetMDIFrame() const 
	{
		assert( m_pMDIFrame );
		return const_cast<CMDIFrame&>( *m_pMDIFrame );
	}

	inline LRESULT CMDIFrame::CMDIClient::OnMDIActivate(UINT, WPARAM wParam, LPARAM lParam)
	{
		// Suppress redraw to avoid flicker when activating maximised MDI children
		SetRedraw(FALSE);
		LRESULT lr = CallWindowProc(GetPrevWindowProc(), WM_MDIACTIVATE, wParam, lParam);
		SetRedraw(TRUE);
		RedrawWindow(0, 0, RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);

		return lr;
	}

	inline LRESULT CMDIFrame::CMDIClient::OnMDIDestroy(UINT, WPARAM wParam, LPARAM lParam)
	{
		// Do default processing first
		CallWindowProc(GetPrevWindowProc(), WM_MDIDESTROY, wParam, lParam);

		// Now remove MDI child
		GetMDIFrame().RemoveMDIChild((HWND) wParam);

		return 0L;
	}

	inline LRESULT CMDIFrame::CMDIClient::OnMDISetMenu(UINT uMsg,WPARAM wParam, LPARAM lParam)
	{
		if (GetMDIFrame().IsMenuBarUsed())
		{
			return 0L;
		}

		return FinalWindowProc(uMsg, wParam, lParam);
	}

	inline LRESULT CMDIFrame::CMDIClient::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_MDIACTIVATE: return OnMDIActivate(uMsg, wParam, lParam);
		case WM_MDIDESTROY:	return OnMDIDestroy(uMsg, wParam, lParam);
		case WM_MDISETMENU:	return OnMDISetMenu(uMsg, wParam, lParam);
		}
		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	}


	/////////////////////////////////////
	//Definitions for the CMDIChild class
	//
	inline CMDIChild::CMDIChild() : m_pMDIFrame(NULL), m_pView(NULL), m_hChildAccel(0)
	{
		// Set the MDI Child's menu and accelerator in the constructor, like this ...
		//   HMENU hChildMenu = LoadMenu(GetApp().GetResourceHandle(), _T("MdiMenuView"));
		//   HACCEL hChildAccel = LoadAccelerators(GetApp().GetResourceHandle(), _T("MDIAccelView"));
		//   SetHandles(hChildMenu, hChildAccel);
	}

	inline CMDIChild::~CMDIChild()
	{
		if (IsWindow())
			GetParent().SendMessage(WM_MDIDESTROY, (WPARAM)GetHwnd(), 0L);
	}

	inline HWND CMDIChild::Create(HWND hWndParent /*= NULL*/)
	// We create the MDI child window and then maximize if required.
	// This technique avoids unnecessary flicker when creating maximized MDI children.
	{
		CREATESTRUCT cs;
		WNDCLASS wc;

		ZeroMemory(&cs, sizeof(CREATESTRUCT));
		ZeroMemory(&wc, sizeof(WNDCLASS));

		//Call PreCreate in case its overloaded
		PreCreate(cs);

		//Determine if the window should be created maximized
		BOOL bMax = FALSE;
		CWnd* pParent = GetCWndPtr(hWndParent);
		assert(pParent);
		pParent->SendMessage(WM_MDIGETACTIVE, 0L, (LPARAM)&bMax);
		bMax = bMax | (cs.style & WS_MAXIMIZE);

		// Set the Window Class Name
		CString ClassName = _T("Win32++ MDI Child");
		if (cs.lpszClass)
			ClassName = cs.lpszClass;

		// Set the window style
		DWORD dwStyle;
		dwStyle = cs.style & ~WS_MAXIMIZE;
		dwStyle |= WS_VISIBLE | WS_OVERLAPPEDWINDOW ;

		// Set window size and position
		int x = CW_USEDEFAULT;
		int	y = CW_USEDEFAULT;
		int cx = CW_USEDEFAULT;
		int cy = CW_USEDEFAULT;
		if(cs.cx && cs.cy)
		{
			x = cs.x;
			y = cs.y;
			cx = cs.cx;
			cy = cs.cy;
		}

		// Set the extended style
		DWORD dwExStyle = cs.dwExStyle | WS_EX_MDICHILD;

		// Turn off redraw while creating the window
		pParent->SetRedraw(FALSE);

		// Create the window
		CreateEx(dwExStyle, ClassName, cs.lpszName, dwStyle, x, y,
			cx, cy, pParent->GetHwnd(), cs.hMenu, cs.lpCreateParams);

		if (bMax)
			ShowWindow(SW_MAXIMIZE);

		// Turn redraw back on
		pParent->SetRedraw(TRUE);
		pParent->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);

		// Ensure bits revealed by round corners (XP themes) are redrawn
		SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);

		if (m_ChildMenu.GetHandle())
			GetMDIFrame().UpdateFrameMenu(m_ChildMenu);
		if (m_hChildAccel)
			GetApp().SetAccelerators(m_hChildAccel, *this);

		return GetHwnd();
	}

	inline CMDIFrame& CMDIChild::GetMDIFrame() const
	{
		assert( m_pMDIFrame );
		return const_cast<CMDIFrame&>(*m_pMDIFrame);
	}

	inline LRESULT CMDIChild::FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefMDIChildProc(*this, uMsg, wParam, lParam);
	}

	inline void CMDIChild::MDIActivate() const
	{
		GetParent().SendMessage(WM_MDIACTIVATE, (WPARAM)GetHwnd(), 0L);
	}

	inline void CMDIChild::MDIDestroy() const
	{
		GetParent().SendMessage(WM_MDIDESTROY, (WPARAM)GetHwnd(), 0L);
	}

	inline void CMDIChild::MDIMaximize() const
	{
		GetParent().SendMessage(WM_MDIMAXIMIZE, (WPARAM)GetHwnd(), 0L);
	}

	inline void CMDIChild::MDIRestore() const
	{
		GetParent().SendMessage(WM_MDIRESTORE, (WPARAM)GetHwnd(), 0L);
	}

	inline void CMDIChild::OnClose()
	{
		// Override this to customise what happens when the window asks to be closed.
		MDIDestroy();
	}

	inline int CMDIChild::OnCreate(CREATESTRUCT& cs)
	{
		UNREFERENCED_PARAMETER(cs);

		// Create the view window
		assert( &GetView() );			// Use SetView in CMDIChild's constructor to set the view window
		GetView().Create(*this);
		RecalcLayout();

		return 0;
	}

	inline void CMDIChild::RecalcLayout()
	{
		// Resize the View window
		CRect rc = GetClientRect();
		GetView().SetWindowPos( NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW );
	}

	inline void CMDIChild::SetHandles(HMENU hMenu, HACCEL hAccel)
	{
		m_ChildMenu.Attach(hMenu);
		m_hChildAccel = hAccel;

		// Note: It is valid to call SetHandles before the window is created
		if (IsWindow())
		{
			CWnd* pWnd = GetMDIFrame().GetActiveMDIChild();
			if (pWnd == this)
			{
				if (m_ChildMenu.GetHandle())
					GetMDIFrame().UpdateFrameMenu(m_ChildMenu);

				if (m_hChildAccel)
					GetApp().SetAccelerators(m_hChildAccel, GetMDIFrame());
			}
		}
	}

	inline void CMDIChild::SetView(CWnd& wndView)
	// Sets or changes the View window displayed within the frame
	{
		if (m_pView != &wndView)
		{
			// Hide the existing view window (if any)
			if (m_pView && m_pView->IsWindow()) m_pView->ShowWindow(SW_HIDE);

			// Assign the view window
			m_pView = &wndView;

			if (GetHwnd() != 0)
			{
				// The MDIChild is already created, so create and position the new view too
				assert(&GetView());			// Use SetView in CMDIChild's constructor to set the view window

				if (!GetView().IsWindow())
					GetView().Create(*this);
				else
				{
					GetView().SetParent(*this);
					GetView().ShowWindow();
				}
				
				RecalcLayout();
			}
		}
	}

	inline LRESULT CMDIChild::OnMDIActivate(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(uMsg);
		UNREFERENCED_PARAMETER(wParam);

		// This child is being activated
		if (lParam == (LPARAM) GetHwnd())
		{
			GetMDIFrame().m_hActiveMDIChild = *this;
			// Set the menu to child default menu
			if (m_ChildMenu.GetHandle())
				GetMDIFrame().UpdateFrameMenu(m_ChildMenu);
			if (m_hChildAccel)
				GetApp().SetAccelerators(m_hChildAccel, *this);
		}

		// No child is being activated
		if (lParam == 0)
		{
			GetMDIFrame().m_hActiveMDIChild = NULL;
			// Set the menu to frame's original menu
			GetMDIFrame().UpdateFrameMenu( GetMDIFrame().GetFrameMenu() );
			GetApp().SetAccelerators(GetMDIFrame().GetFrameAccel(), *this);
		}
			
		return 0L;
	}

	inline LRESULT CMDIChild::OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		RecalcLayout();
		return FinalWindowProc(uMsg, wParam, lParam);
	}

	inline LRESULT CMDIChild::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			case WM_MDIACTIVATE:		return OnMDIActivate(uMsg, wParam, lParam);
			case WM_WINDOWPOSCHANGED:	return OnWindowPosChanged(uMsg, wParam, lParam);
		}
		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	}


} // namespace Win32xx

#endif // _WIN32XX_MDI_H_

