// Win32++   Version 7.8
// Release Date: 17th March 2015
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2015  David Nash
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
// mdi.h
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

#include "frame.h"
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
		virtual HWND Create(CWnd* pParent = NULL);
		virtual void RecalcLayout();

		// These functions aren't virtual, and shouldn't be overridden
		void SetHandles(HMENU MenuName, HACCEL AccelName);
		CMDIFrame* GetMDIFrame() const;
		CWnd* GetView() const	{return m_pView;}
		void SetView(CWnd& pwndView);
		void MDIActivate() const;
		void MDIDestroy() const;
		void MDIMaximize() const;
		void MDIRestore() const;

	protected:
		// These are the functions you might wish to override
		virtual void OnClose();
		virtual int  OnCreate(LPCREATESTRUCT pcs);
		virtual LRESULT OnMDIActivate(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnWindowPosChanged(WPARAM wParam, LPARAM lParam);
		// Its unlikely you would need to override these functions
		virtual LRESULT FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		CMDIChild(const CMDIChild&);				// Disable copy construction
		CMDIChild& operator = (const CMDIChild&); // Disable assignment operator

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
		class CDockMDIClient : public CDocker::CDockClient
		{
		public:
			CDockMDIClient() {}
			virtual ~CDockMDIClient() {}
			CMDIFrame* GetMDIFrame() const { return static_cast<CMDIFrame*>(GetParent()); }

		protected:
			HWND Create(CWnd* pParent);
			LRESULT OnMDIActivate(WPARAM wParam, LPARAM lParam);
			LRESULT OnMDIDestroy(WPARAM wParam, LPARAM lParam);
			LRESULT OnMDISetMenu(WPARAM wParam, LPARAM lParam);
			LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		};


		CMDIFrame();
		virtual ~CMDIFrame() {}

		virtual CMDIChild* AddMDIChild(MDIChildPtr pMDIChild);
		virtual CMDIChild* GetActiveMDIChild() const;
		virtual CMenu* GetActiveMenu() const;
		virtual CDockClient* GetDockClient() const	{ return (CDockClient*)(&m_DockMDIClient); }
		virtual CWnd* GetMDIClient() const { return (CWnd*)(GetDockClient()); }
		virtual BOOL IsMDIChildMaxed() const;
		virtual BOOL IsMDIFrame() const { return TRUE; }
		virtual void RemoveMDIChild(HWND hWnd);
		virtual BOOL RemoveAllMDIChildren();

		// These functions aren't virtual, so don't override them
		std::vector<MDIChildPtr>& GetAllMDIChildren() {return m_vMDIChild;}
		void MDICascade(int nType = 0) const;
		void MDIIconArrange() const;
		void MDIMaximize() const;
		void MDINext() const;
		void MDIPrev() const;
		void MDIRestore() const;
		void MDITile(int nType = 0) const;
		void SetActiveMDIChild(CMDIChild* pChild);

	protected:
		// These are the functions you might wish to override
		virtual void    OnClose();
		virtual LRESULT OnInitMenuPopup(WPARAM wParam, LPARAM lParam);
		virtual void    OnMenuUpdate(UINT nID);
		virtual void    OnViewStatusBar();
		virtual void    OnViewToolBar();
		virtual LRESULT OnWindowPosChanged(WPARAM wParam, LPARAM lParam);
		virtual void    RecalcLayout();
		virtual BOOL    PreTranslateMessage(MSG* pMsg);
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		CMDIFrame(const CMDIFrame&);				// Disable copy construction
		CMDIFrame& operator = (const CMDIFrame&); // Disable assignment operator
		void AppendMDIMenu(CMenu* pMenuWindow);
		LRESULT FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void UpdateFrameMenu(CMenu* pMenu);

		std::vector<MDIChildPtr> m_vMDIChild;
		CDockMDIClient m_DockMDIClient;
		HWND m_hActiveMDIChild;
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
		SetView(*GetDockClient());
	}

	inline CMDIChild* CMDIFrame::AddMDIChild(MDIChildPtr pMDIChild)
	// Adds a MDI child to the MDI frame. The pointer to the MDI child will be 
	//  automatically deleted when the MDI Frame destroys the MDI child.
	{
		assert(NULL != pMDIChild.get()); // Cannot add Null MDI Child

		m_vMDIChild.push_back(pMDIChild);
		pMDIChild->Create(GetMDIClient());

		return pMDIChild.get();
	}

	inline void CMDIFrame::AppendMDIMenu(CMenu* pMenuWindow)
	// Adds the additional menu items the the "Window" submenu when
	//  MDI child windows are created	
	{
		if (!pMenuWindow->GetHandle())
			return;

		// Delete previously appended items
		int nItems = pMenuWindow->GetMenuItemCount();
		UINT uLastID = pMenuWindow->GetMenuItemID(--nItems);
		if ((uLastID >= IDW_FIRSTCHILD) && (uLastID < IDW_FIRSTCHILD + 10))
		{
			while ((uLastID >= IDW_FIRSTCHILD) && (uLastID < IDW_FIRSTCHILD + 10))
			{
				pMenuWindow->DeleteMenu(nItems, MF_BYPOSITION);
				uLastID = pMenuWindow->GetMenuItemID(--nItems);
			}
			
			//delete the separator too
			pMenuWindow->DeleteMenu(nItems, MF_BYPOSITION);
		}

		int nWindow = 0;

		// Allocate an iterator for our MDIChild vector
		std::vector<MDIChildPtr>::iterator v;

		for (v = GetAllMDIChildren().begin(); v < GetAllMDIChildren().end(); ++v)
		{
			if ((*v)->GetWindowLongPtr(GWL_STYLE) & WS_VISIBLE)	// IsWindowVisible is unreliable here
			{
				// Add Separator
				if (nWindow == 0)
					pMenuWindow->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)NULL);

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

					pMenuWindow->AppendMenu(MF_STRING, IDW_FIRSTCHILD + nWindow, MenuString);

					if (GetActiveMDIChild() == (*v).get())
						pMenuWindow->CheckMenuItem(IDW_FIRSTCHILD+nWindow, MF_CHECKED);

					++nWindow;
				}
				else if (9 == nWindow)
				// For the 10th MDI child, add this menu item and return
				{
					pMenuWindow->AppendMenu(MF_STRING, IDW_FIRSTCHILD + nWindow, _T("&Windows..."));
					return;
				}
			}
		}
	}

	inline CMenu* CMDIFrame::GetActiveMenu() const
	// Returns a pointer to the menu of the Active MDI Child if any,
	// otherwise returns a pointer to the MDI Frame's menu
	{
		CMenu* pMenu = GetFrameMenu();
		
		if(GetActiveMDIChild())
			if (GetActiveMDIChild()->m_ChildMenu.GetHandle())
				pMenu = &GetActiveMDIChild()->m_ChildMenu;

		return pMenu;
	}

	inline LRESULT CMDIFrame::FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefFrameProc(m_hWnd, *GetMDIClient(), uMsg, wParam, lParam);
	}

	inline CMDIChild* CMDIFrame::GetActiveMDIChild() const
	// Returns a pointer to the active MDI child
	{
		return static_cast<CMDIChild*>(FromHandle(m_hActiveMDIChild));
	}

	inline BOOL CMDIFrame::IsMDIChildMaxed() const
	// Returns TRUE if a MDI child is maximized
	{
		BOOL bMaxed = FALSE;
		GetMDIClient()->SendMessage(WM_MDIGETACTIVE, 0L, (LPARAM)&bMaxed);
		return bMaxed;
	}

	inline void CMDIFrame::MDICascade(int nType /* = 0*/) const
	// Possible values for nType are:
	// MDITILE_SKIPDISABLED	Prevents disabled MDI child windows from being cascaded.	
	{
		assert(::IsWindow(m_hWnd));
		GetMDIClient()->SendMessage(WM_MDICASCADE, (WPARAM)nType, 0L);
	}

	inline void CMDIFrame::MDIIconArrange() const
	// Re-arranges the icons for minimized MDI children
	{
		assert(::IsWindow(m_hWnd));
		GetMDIClient()->SendMessage(WM_MDIICONARRANGE, 0L, 0L);
	}

	inline void CMDIFrame::MDIMaximize() const
	// Maximize the MDI child
	{
		assert(::IsWindow(m_hWnd));
		GetMDIClient()->SendMessage(WM_MDIMAXIMIZE, 0L, 0L);
	}

	inline void CMDIFrame::MDINext() const
	// Activates the next MDI child
	{
		assert(::IsWindow(m_hWnd));
		HWND hMDIChild = GetActiveMDIChild()->GetHwnd();
		GetMDIClient()->SendMessage(WM_MDINEXT, (WPARAM)hMDIChild, FALSE);
	}

	inline void CMDIFrame::MDIPrev() const
	// Activates the previous MDI child
	{
		assert(::IsWindow(m_hWnd));
		HWND hMDIChild = GetActiveMDIChild()->GetHwnd();
		GetMDIClient()->SendMessage(WM_MDINEXT, (WPARAM)hMDIChild, TRUE);
	}

	inline void CMDIFrame::MDIRestore() const
	// Restores a mimimized MDI child
	{
		assert(::IsWindow(m_hWnd));
		GetMDIClient()->SendMessage(WM_MDIRESTORE, 0L, 0L);
	}

	inline void CMDIFrame::MDITile(int nType /* = 0*/) const
	{
		// Possible values for nType are:
		// MDITILE_HORIZONTAL	Tiles MDI child windows so that one window appears above another.
		// MDITILE_SKIPDISABLED	Prevents disabled MDI child windows from being tiled.
		// MDITILE_VERTICAL		Tiles MDI child windows so that one window appears beside another.

		assert(::IsWindow(m_hWnd));
		GetMDIClient()->SendMessage(WM_MDITILE, (WPARAM)nType, 0L);
	}

	inline void CMDIFrame::OnClose()
	{
		if (RemoveAllMDIChildren())
		{
			CFrame::OnClose();
		}
	}

	inline LRESULT CMDIFrame::OnInitMenuPopup(WPARAM wParam, LPARAM lParam)
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

		return CFrame::OnInitMenuPopup(wParam, lParam);
	}

	inline void CMDIFrame::OnMenuUpdate(UINT nID)
	{
		// Update the check buttons before displaying the menu
		
		switch(nID)
		{
		case IDW_VIEW_STATUSBAR:
			GetActiveMenu()->CheckMenuItem(nID, m_ShowStatusBar ? MF_CHECKED : MF_UNCHECKED);
			break;
		case IDW_VIEW_TOOLBAR:
			GetActiveMenu()->EnableMenuItem(nID, m_UseToolBar ? MF_ENABLED : MF_DISABLED);
			GetActiveMenu()->CheckMenuItem(nID, m_ShowToolBar ? MF_CHECKED : MF_UNCHECKED);
			break;
		}	
	}

	inline void CMDIFrame::OnViewStatusBar()
	{
		CFrame::OnViewStatusBar();
		GetView()->RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
	}

	inline void CMDIFrame::OnViewToolBar()
	{
		CFrame::OnViewToolBar();
		GetView()->RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
	}

	inline LRESULT CMDIFrame::OnWindowPosChanged(WPARAM wParam, LPARAM lParam)
	{
		// MDI Child or MDI frame has been resized

		if (IsMenuBarUsed())
		{
			// Refresh MenuBar Window
			GetMenuBar()->SetMenu(GetMenuBar()->GetMenu());
		}

		return FinalWindowProc(WM_WINDOWPOSCHANGED, wParam, lParam);
	}

	inline BOOL CMDIFrame::PreTranslateMessage(MSG* pMsg)
	{
		if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
		{
			if (TranslateMDISysAccel(GetView()->GetHwnd(), pMsg))
				return TRUE;
		}

		return CFrame::PreTranslateMessage(pMsg);
	}

	inline void CMDIFrame::RecalcLayout()
	{
		CFrame::RecalcLayout();

		if (GetView()->IsWindow())
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
				UpdateFrameMenu(&GetActiveMDIChild()->m_ChildMenu);
			if (GetActiveMDIChild()->m_hChildAccel)
				GetApp()->SetAccelerators(GetActiveMDIChild()->m_hChildAccel, this);
		}
		else
		{
			if (IsMenuBarUsed())
				GetMenuBar()->SetMenu( GetFrameMenu()->GetHandle() );
			else
				SetMenu( GetFrameMenu() );

			GetApp()->SetAccelerators(GetFrameAccel(), this);
		}
	}

	inline void CMDIFrame::SetActiveMDIChild(CMDIChild* pChild)
	{
		assert ( pChild->IsWindow() );

		GetMDIClient()->SendMessage(WM_MDIACTIVATE, (WPARAM)pChild->GetHwnd(), 0L);

		// Verify
		assert ( m_hActiveMDIChild == pChild->GetHwnd() );
	}

	inline void CMDIFrame::UpdateFrameMenu(CMenu* pMenu)
	{
		int nMenuItems = pMenu->GetMenuItemCount();
		if (nMenuItems > 0)
		{
			// The Window menu is typically second from the right
			int nWindowItem = MAX (nMenuItems -2, 0);
			CMenu* pMenuWindow = pMenu->GetSubMenu(nWindowItem);

			if (pMenuWindow)
			{
				if (IsMenuBarUsed())
				{
					AppendMDIMenu(pMenuWindow);
					GetMenuBar()->SetMenu(pMenu->GetHandle());
				}
				else
				{
					GetMDIClient()->SendMessage (WM_MDISETMENU, (WPARAM)pMenu->GetHandle(), (LPARAM)pMenuWindow->GetHandle());
					DrawMenuBar();
				}
			}
		}
	}

	inline LRESULT CMDIFrame::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_WINDOWPOSCHANGED:	return OnWindowPosChanged(wParam, lParam);
		}
		
		return CFrame::WndProcDefault(uMsg, wParam, lParam);
	}


	/////////////////////////////////////
	//Definitions for the CDockMDIChild class
	//
	inline HWND CMDIFrame::CDockMDIClient::Create(CWnd* pParent)
	{
		assert(pParent != 0);

		CLIENTCREATESTRUCT clientcreate;
		clientcreate.hWindowMenu  = 0;
		clientcreate.idFirstChild = IDW_FIRSTCHILD;
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | MDIS_ALLCHILDSTYLES;
		HWND hWndParent = pParent? pParent->GetHwnd() : 0;

		// Create the view window
		CreateEx(WS_EX_CLIENTEDGE, _T("MDICLient"), TEXT(""), dwStyle, 0, 0, 0, 0, hWndParent, NULL, (PSTR) &clientcreate);

		return m_hWnd;
	}

	inline LRESULT CMDIFrame::CDockMDIClient::OnMDIActivate(WPARAM wParam, LPARAM lParam)
	{
		// Suppress redraw to avoid flicker when activating maximised MDI children
		SetRedraw(FALSE);
		LRESULT lr = CallWindowProc(GetPrevWindowProc(), WM_MDIACTIVATE, wParam, lParam);
		SetRedraw(TRUE);
		RedrawWindow(0, 0, RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);

		return lr;
	}

	inline LRESULT CMDIFrame::CDockMDIClient::OnMDIDestroy(WPARAM wParam, LPARAM lParam)
	{
		// Do default processing first
		CallWindowProc(GetPrevWindowProc(), WM_MDIDESTROY, wParam, lParam);

		// Now remove MDI child
		GetMDIFrame()->RemoveMDIChild((HWND) wParam);

		return 0L;
	}

	inline LRESULT CMDIFrame::CDockMDIClient::OnMDISetMenu(WPARAM wParam, LPARAM lParam)
	{
		if (GetMDIFrame()->IsMenuBarUsed())
		{
			return 0L;
		}

		return FinalWindowProc(WM_MDISETMENU, wParam, lParam);
	}

	inline LRESULT CMDIFrame::CDockMDIClient::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_MDIACTIVATE: return OnMDIActivate(wParam, lParam);
		case WM_MDIDESTROY:	return OnMDIDestroy(wParam, lParam);
		case WM_MDISETMENU:	return OnMDISetMenu(wParam, lParam);
		}
		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	}


	/////////////////////////////////////
	//Definitions for the CMDIChild class
	//
	inline CMDIChild::CMDIChild() : m_pView(NULL)
	{
		// Set the MDI Child's menu and accelerator in the constructor, like this ...
		//   HMENU hChildMenu = LoadMenu(GetApp()->GetResourceHandle(), _T("MdiMenuView"));
		//   HACCEL hChildAccel = LoadAccelerators(GetApp()->GetResourceHandle(), _T("MDIAccelView"));
		//   SetHandles(hChildMenu, hChildAccel);
	}

	inline CMDIChild::~CMDIChild()
	{
		if (IsWindow())
			GetParent()->SendMessage(WM_MDIDESTROY, (WPARAM)m_hWnd, 0L);
	}

	inline HWND CMDIChild::Create(CWnd* pParent /*= NULL*/)
	// We create the MDI child window and then maximize if required.
	// This technique avoids unnecessary flicker when creating maximized MDI children.
	{
		//Call PreCreate in case its overloaded
		PreCreate(*m_pcs);

		//Determine if the window should be created maximized
		BOOL bMax = FALSE;
		assert(pParent);
		pParent->SendMessage(WM_MDIGETACTIVE, 0L, (LPARAM)&bMax);
		bMax = bMax | (m_pcs->style & WS_MAXIMIZE);

		// Set the Window Class Name
		CString ClassName = _T("Win32++ MDI Child");
		if (m_pcs->lpszClass)
			ClassName = m_pcs->lpszClass;

		// Set the window style
		DWORD dwStyle;
		dwStyle = m_pcs->style & ~WS_MAXIMIZE;
		dwStyle |= WS_VISIBLE | WS_OVERLAPPEDWINDOW ;

		// Set window size and position
		int x = CW_USEDEFAULT;
		int	y = CW_USEDEFAULT;
		int cx = CW_USEDEFAULT;
		int cy = CW_USEDEFAULT;
		if(m_pcs->cx && m_pcs->cy)
		{
			x = m_pcs->x;
			y = m_pcs->y;
			cx = m_pcs->cx;
			cy = m_pcs->cy;
		}

		// Set the extended style
		DWORD dwExStyle = m_pcs->dwExStyle | WS_EX_MDICHILD;

		// Turn off redraw while creating the window
		pParent->SetRedraw(FALSE);

		// Create the window
		if (!CreateEx(dwExStyle, ClassName, m_pcs->lpszName, dwStyle, x, y,
			cx, cy, pParent->GetHwnd(), m_pcs->hMenu, m_pcs->lpCreateParams))
			throw CWinException(_T("CMDIChild::Create ... CreateEx failed"));

		if (bMax)
			ShowWindow(SW_MAXIMIZE);

		// Turn redraw back on
		pParent->SetRedraw(TRUE);
		pParent->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);

		// Ensure bits revealed by round corners (XP themes) are redrawn
		SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);

		if (m_ChildMenu.GetHandle())
			GetMDIFrame()->UpdateFrameMenu(&m_ChildMenu);
		if (m_hChildAccel)
			GetApp()->SetAccelerators(m_hChildAccel, this);

		return m_hWnd;
	}

	inline CMDIFrame* CMDIChild::GetMDIFrame() const
	{
		CMDIFrame* pMDIFrame = static_cast<CMDIFrame*>(GetParent()->GetParent());
		assert(dynamic_cast<CMDIFrame*>(pMDIFrame));
		return pMDIFrame;
	}

	inline LRESULT CMDIChild::FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefMDIChildProc(m_hWnd, uMsg, wParam, lParam);
	}

	inline void CMDIChild::MDIActivate() const
	{
		GetParent()->SendMessage(WM_MDIACTIVATE, (WPARAM)m_hWnd, 0L);
	}

	inline void CMDIChild::MDIDestroy() const
	{
		GetParent()->SendMessage(WM_MDIDESTROY, (WPARAM)m_hWnd, 0L);
	}

	inline void CMDIChild::MDIMaximize() const
	{
		GetParent()->SendMessage(WM_MDIMAXIMIZE, (WPARAM)m_hWnd, 0L);
	}

	inline void CMDIChild::MDIRestore() const
	{
		GetParent()->SendMessage(WM_MDIRESTORE, (WPARAM)m_hWnd, 0L);
	}

	inline void CMDIChild::OnClose()
	{
		// Override this to customise what happens when the window asks to be closed.
		MDIDestroy();
	}

	inline int CMDIChild::OnCreate(LPCREATESTRUCT pcs)
	{
		UNREFERENCED_PARAMETER(pcs);

		// Create the view window
		assert(GetView());			// Use SetView in CMDIChild's constructor to set the view window
		GetView()->Create(this);
		RecalcLayout();

		return 0;
	}

	inline void CMDIChild::RecalcLayout()
	{
		// Resize the View window
		CRect rc = GetClientRect();
		GetView()->SetWindowPos( NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW );
	}

	inline void CMDIChild::SetHandles(HMENU hMenu, HACCEL hAccel)
	{
		m_ChildMenu.Attach(hMenu);
		m_hChildAccel = hAccel;

		// Note: It is valid to call SetHandles before the window is created
		if (IsWindow())
		{
			CWnd* pWnd = GetMDIFrame()->GetActiveMDIChild();
			if (pWnd == this)
			{
				if (m_ChildMenu.GetHandle())
					GetMDIFrame()->UpdateFrameMenu(&m_ChildMenu);

				if (m_hChildAccel)
					GetApp()->SetAccelerators(m_hChildAccel, GetMDIFrame());
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

			if (m_hWnd != 0)
			{
				// The MDIChild is already created, so create and position the new view too
				assert(GetView());			// Use SetView in CMDIChild's constructor to set the view window

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

	inline LRESULT CMDIChild::OnMDIActivate(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);

		// This child is being activated
		if (lParam == (LPARAM) m_hWnd)
		{
			GetMDIFrame()->m_hActiveMDIChild = m_hWnd;
			// Set the menu to child default menu
			if (m_ChildMenu.GetHandle())
				GetMDIFrame()->UpdateFrameMenu(&m_ChildMenu);
			if (m_hChildAccel)
				GetApp()->SetAccelerators(m_hChildAccel, this);
		}

		// No child is being activated
		if (lParam == 0)
		{
			GetMDIFrame()->m_hActiveMDIChild = NULL;
			// Set the menu to frame's original menu
			GetMDIFrame()->UpdateFrameMenu( GetMDIFrame()->GetFrameMenu() );
			GetApp()->SetAccelerators(GetMDIFrame()->GetFrameAccel(), this);
		}
			
		return 0L;
	}

	inline LRESULT CMDIChild::OnWindowPosChanged(WPARAM wParam, LPARAM lParam)
	{
		RecalcLayout();
		return FinalWindowProc(WM_WINDOWPOSCHANGED, wParam, lParam);
	}

	inline LRESULT CMDIChild::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_MDIACTIVATE:		return OnMDIActivate(wParam, lParam);
		case WM_WINDOWPOSCHANGED:	return OnWindowPosChanged(wParam, lParam);
		}
		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	}


} // namespace Win32xx

#endif // _WIN32XX_MDI_H_

