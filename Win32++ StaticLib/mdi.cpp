// Win32++  Version 6.61
// Released: 30th August, 2009 by:
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


#include "mdi.h"


namespace Win32xx
{

	/////////////////////////////////////
	// Definitions for the CMDIFrame class
	//
	CMDIFrame::CMDIFrame() : m_hActiveMDIChild(NULL)
	{
		SetView(m_wndMDIClient);
	}

	CMDIFrame::~CMDIFrame()
	{
		// Ensure all MDI child objects are destroyed
		std::vector <CMDIChild*>::iterator v;

		while(m_vMDIChild.size() > 0)
		{
			v = m_vMDIChild.begin();
			(*v)->Destroy();
			delete *v;
			m_vMDIChild.erase(v);
		}
	}

	CMDIChild* CMDIFrame::AddMDIChild(CMDIChild* pMDIChild)
	{
		if (NULL == pMDIChild)
			throw CWinException(_T("Cannot add Null MDI Child"));

		m_vMDIChild.push_back(pMDIChild);
		pMDIChild->Create(GetView()->GetHwnd());

		return pMDIChild;
	}

	void CMDIFrame::AppendMDIMenu(HMENU hMenuWindow)
	{
		// Adds the additional menu items the the "Window" submenu when
		//  MDI child windows are created

		if (!IsMenu(hMenuWindow))
			return;

		// Delete previously appended items
		int nItems = ::GetMenuItemCount(hMenuWindow);
		UINT uLastID = ::GetMenuItemID(hMenuWindow, --nItems);
		if ((uLastID >= IDW_FIRSTCHILD) && (uLastID < IDW_FIRSTCHILD + 10))
		{
			while ((uLastID >= IDW_FIRSTCHILD) && (uLastID < IDW_FIRSTCHILD + 10))
			{
				::DeleteMenu(hMenuWindow, nItems, MF_BYPOSITION);
				uLastID = ::GetMenuItemID(hMenuWindow, --nItems);
			}
			//delete the separator too
			::DeleteMenu(hMenuWindow, nItems, MF_BYPOSITION);
		}

		int nWindow = 0;

		// Allocate an iterator for our MDIChild vector
		std::vector <CMDIChild*>::iterator v;

		for (v = GetAllMDIChildren().begin(); v < GetAllMDIChildren().end(); ++v)
		{
			HWND hwndMDIChild = (*v)->GetHwnd();
			if (::GetWindowLong(hwndMDIChild, GWL_STYLE) & WS_VISIBLE)	// IsWindowVisible is unreliable here
			{
				// Add Separator
				if (0 == nWindow)
					::AppendMenu(hMenuWindow, MF_SEPARATOR, 0, NULL);

				// Add a menu entry for each MDI child (up to 9)
				if (nWindow < 9)
				{
					tString tsMenuItem = (*v)->GetWindowString();

					if (tsMenuItem.length() > MAX_MENU_STRING -10)
					{
						// Truncate the string if its too long
						tsMenuItem.erase(tsMenuItem.length() - MAX_MENU_STRING +10);
						tsMenuItem += _T(" ...");
					}

					TCHAR szMenuString[MAX_MENU_STRING+1];
					wsprintf(szMenuString, _T("&%d %s"), nWindow+1, tsMenuItem.c_str());

					::AppendMenu(hMenuWindow, MF_STRING, IDW_FIRSTCHILD + nWindow, szMenuString);

					if (GetActiveMDIChild()->GetHwnd() == hwndMDIChild)
						::CheckMenuItem(hMenuWindow, IDW_FIRSTCHILD+nWindow, MF_CHECKED);

					++nWindow;
				}
				else if (9 == nWindow)
				// For the 10th MDI child, add this menu item and return
				{
					::AppendMenu(hMenuWindow, MF_STRING, IDW_FIRSTCHILD + nWindow, _T("&Windows..."));
					return;
				}
			}
		}
	}

	LRESULT CMDIFrame::FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefFrameProc(m_hWnd, m_wndMDIClient, uMsg, wParam, lParam);
	}

	CMDIChild* CMDIFrame::GetActiveMDIChild() const
	{
		return (CMDIChild*)FromHandle(m_hActiveMDIChild);
	}

	BOOL CMDIFrame::IsMDIChildMaxed() const
	{
		BOOL bMaxed = FALSE;
		::SendMessage(m_wndMDIClient, WM_MDIGETACTIVE, 0L, (LPARAM)&bMaxed);
		return bMaxed;
	}

	void CMDIFrame::OnClose()
	{
		if (RemoveAllMDIChildren())
		{
			CFrame::OnClose();
			Destroy();
		}
	}

	void CMDIFrame::OnViewStatusbar()
	{
		CFrame::OnViewStatusbar();
		UpdateCheckMarks();
		::RedrawWindow(GetView()->GetHwnd(), NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
	}

	void CMDIFrame::OnViewToolbar()
	{
		CFrame::OnViewToolbar();
		UpdateCheckMarks();
		::RedrawWindow(GetView()->GetHwnd(), NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
	}

	void CMDIFrame::OnWindowPosChanged()
	{
		if (IsMenubarUsed())
		{
			// Refresh Menubar Window
			HMENU hMenu= GetMenubar().GetMenu();
			GetMenubar().SetMenu(hMenu);
			UpdateCheckMarks();
		}
	}

	BOOL CMDIFrame::PreTranslateMessage(MSG* pMsg)
	{
		if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
		{
			if (TranslateMDISysAccel(GetView()->GetHwnd(), pMsg))
				return TRUE;
		}

		return CFrame::PreTranslateMessage(pMsg);
	}

	void CMDIFrame::RecalcLayout()
	{
		CFrame::RecalcLayout();
		::PostMessage (GetView()->GetHwnd(), WM_MDIICONARRANGE, 0L, 0L) ;
	}

	BOOL CMDIFrame::RemoveAllMDIChildren()
	{
		// Allocate an iterator for our MDIChild vector
		std::vector <CMDIChild*>::iterator v;

		while(m_vMDIChild.size() > 0)
		{
			v = m_vMDIChild.begin();
			HWND hWnd = (*v)->GetHwnd();
			::SendMessage(hWnd, WM_CLOSE, 0L, 0L);
			if (::IsWindow(hWnd))
				return FALSE;

			RemoveMDIChild(hWnd);
		}
		return TRUE;
	}

	void CMDIFrame::RemoveMDIChild(HWND hWnd)
	{
		// Allocate an iterator for our HWND map
		std::vector <CMDIChild*>::iterator v;

		for (v = m_vMDIChild.begin(); v!= m_vMDIChild.end(); ++v)
		{
			if ((*v)->GetHwnd() == hWnd)
			{
				(*v)->Destroy();
				delete *v;
				m_vMDIChild.erase(v);
				break;
			}
		}

		if ((GetActiveMDIChild()) && GetActiveMDIChild()->m_hChildMenu)
		{
			UpdateFrameMenu(GetActiveMDIChild()->m_hChildMenu);
		}
		else
		{
			if (IsMenubarUsed())
				GetMenubar().SetMenu(GetFrameMenu());
			else
				::SetMenu(m_hWnd, GetFrameMenu());
		}
	}

	void CMDIFrame::SetActiveMDIChild(CMDIChild* pChild)
	{
		if (!pChild->IsWindow())
			throw CWinException(_T("CMDIFrame::SetActiveMDIChild  ... Invalid MDI child"));

		m_wndMDIClient.SendMessage(WM_MDIACTIVATE, (WPARAM)pChild->GetHwnd(), 0L);

		// Verify
		if (m_hActiveMDIChild != pChild->GetHwnd())
			throw CWinException(_T("CMDIFrame::SetActiveMDIChild  ... Failed"));
	}

	void CMDIFrame::UpdateCheckMarks()
	{
		if ((GetActiveMDIChild()) && GetActiveMDIChild()->m_hChildMenu)
		{
			HMENU hMenu = GetActiveMDIChild()->m_hChildMenu;

			UINT uCheck = GetToolbar().IsWindowVisible()? MF_CHECKED : MF_UNCHECKED;
			::CheckMenuItem(hMenu, IDW_VIEW_TOOLBAR, uCheck);

			uCheck = GetStatusbar().IsWindowVisible()? MF_CHECKED : MF_UNCHECKED;
			::CheckMenuItem (hMenu, IDW_VIEW_STATUSBAR, uCheck);
		}
	}

	void CMDIFrame::UpdateFrameMenu(HMENU hMenu)
	{
		int nWindowItem = GetMenuItemPos(hMenu, _T("Window"));
		HMENU hMenuWindow = ::GetSubMenu (hMenu, nWindowItem);

		if (IsMenubarUsed())
		{
			AppendMDIMenu(hMenuWindow);
			GetMenubar().SetMenu(hMenu);
		}
		else
		{
			::SendMessage (GetView()->GetHwnd(), WM_MDISETMENU, (WPARAM) hMenu, (LPARAM)hMenuWindow);
			::DrawMenuBar(GetHwnd());
		}

		UpdateCheckMarks();
	}

	LRESULT CMDIFrame::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CLOSE:
			OnClose();
			return 0;

		case WM_WINDOWPOSCHANGED:
			// MDI Child or MDI frame has been resized
			OnWindowPosChanged();
			break; // Continue with default processing

		} // switch uMsg
		return CFrame::WndProcDefault(uMsg, wParam, lParam);
	}

	HWND CMDIFrame::CMDIClient::Create(HWND hWndParent /* = NULL*/)
	{
		CLIENTCREATESTRUCT clientcreate ;
		clientcreate.hWindowMenu  = m_hWnd;
		clientcreate.idFirstChild = IDW_FIRSTCHILD ;
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | MDIS_ALLCHILDSTYLES;

		// Create the view window
		if (!CreateEx(WS_EX_CLIENTEDGE, _T("MDICLient"), TEXT(""),
				dwStyle, 0, 0, 0, 0, hWndParent, NULL, (PSTR) &clientcreate))
				throw CWinException(TEXT("CMDIClient::Create ... CreateEx failed"));

		return m_hWnd;
	}

	LRESULT CMDIFrame::CMDIClient::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		CMDIFrame* pMDIFrame = (CMDIFrame*)FromHandle(GetAncestor());
		switch (uMsg)
		{
		case WM_MDIDESTROY:
			{
				// Do default processing first
				CallWindowProc(GetPrevWindowProc(), uMsg, wParam, lParam);

				// Now remove MDI child
				pMDIFrame->RemoveMDIChild((HWND) wParam);
			}
			return 0; // Discard message

		case WM_MDISETMENU:
			{
				if (pMDIFrame->IsMenubarUsed())
				{
					return 0L;
				}
			}
			break;

		case WM_MDIACTIVATE:
			{
				// Suppress redraw to avoid flicker when activating maximised MDI children
				::SendMessage(m_hWnd, WM_SETREDRAW, FALSE, 0L);
				LRESULT lr = CallWindowProc(GetPrevWindowProc(), WM_MDIACTIVATE, wParam, lParam);
				::SendMessage(m_hWnd, WM_SETREDRAW, TRUE, 0L);
				::RedrawWindow(m_hWnd, 0, 0, RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);

				return lr;
			}
		}
		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	}


	/////////////////////////////////////
	//Definitions for the CMDIChild class
	//
	CMDIChild::CMDIChild() : m_pwndView(NULL), m_hChildMenu(NULL)
	{
		// Set the MDI Child's menu in the constructor, like this ...

		// SetChildMenu(_T("MdiMenuView"));
	}

	CMDIChild::~CMDIChild()
	{
		if (IsWindow())
			::SendMessage(GetParent(), WM_MDIDESTROY, (WPARAM)m_hWnd, 0L);

		if (m_hChildMenu)
			::DestroyMenu(m_hChildMenu);
	}

	HWND CMDIChild::Create(HWND hWndParent /*= NULL*/)
	// A bug in MS Windows prevents us from creating maximized MDI children.
	// We can work around that by creating the MDI child window
	// and then maximizing if required.

	{
		//Call PreCreate in case its overloaded
		PreCreate(m_cs);

		//Determine if the window should be created maximized
		BOOL bMax = FALSE;
		::SendMessage(hWndParent, WM_MDIGETACTIVE, 0L, (LPARAM)&bMax);
		bMax = bMax | (m_cs.style & WS_MAXIMIZE);

		// Set the Window Class Name
		TCHAR szClassName[MAX_STRING_SIZE + 1] = _T("Win32++ MDI Child");
		if (m_cs.lpszClass)
			lstrcpyn(szClassName, m_cs.lpszClass, MAX_STRING_SIZE);

		// Set Parent
		if (!hWndParent)
			hWndParent = m_cs.hwndParent;

		// Set the window style
		DWORD dwStyle;
		dwStyle = m_cs.style & ~WS_MAXIMIZE;
		dwStyle |= WS_VISIBLE | WS_OVERLAPPEDWINDOW ;

		// Set window size and position
		int x = CW_USEDEFAULT;
		int	y = CW_USEDEFAULT;
		int cx = CW_USEDEFAULT;
		int cy = CW_USEDEFAULT;
		if(m_cs.cx && m_cs.cy)
		{
			x = m_cs.x;
			y = m_cs.y;
			cx = m_cs.cx;
			cy = m_cs.cy;
		}

		// Set the extended style
		DWORD dwExStyle = m_cs.dwExStyle | WS_EX_MDICHILD;

		// Turn off redraw while creating the window
		::SendMessage(hWndParent, WM_SETREDRAW, FALSE, 0L);

		// Create the window
		if (!CreateEx(dwExStyle, szClassName, m_cs.lpszName, dwStyle, x, y,
			cx, cy, hWndParent, m_cs.hMenu, m_cs.lpCreateParams))
			throw CWinException(_T("CMDIChild::Create ... CreateEx failed"));

		if (bMax)
			::ShowWindow(m_hWnd, SW_MAXIMIZE);

		// Turn redraw back on
		::SendMessage(hWndParent, WM_SETREDRAW, TRUE, 0L);
		::RedrawWindow(hWndParent, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);

		// Ensure bits revealed by round corners (XP themes) are redrawn
		::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);

		CMDIFrame* pMDIFrame = (CMDIFrame*)FromHandle(GetAncestor());
		if (m_hChildMenu)
			pMDIFrame->UpdateFrameMenu(m_hChildMenu);

		return m_hWnd;
	}

	LRESULT CMDIChild::FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefMDIChildProc(m_hWnd, uMsg, wParam, lParam);
	}

	void CMDIChild::OnCreate()
	{
		// Create the view window
		if (NULL == m_pwndView)
			throw CWinException(_T("CMDIChild::OnCreate ... View window is not assigned!\nUse SetView to set the View Window"));

		m_pwndView->Create(m_hWnd);
	}

	BOOL CMDIChild::SetChildMenu(LPCTSTR MenuName)
	{
		HINSTANCE hInstance = GetApp()->GetInstanceHandle();
		m_hChildMenu = ::LoadMenu (hInstance, MenuName);

		HWND hWnd = (HWND)::SendMessage(GetParent(), WM_MDIGETACTIVE, 0L, 0L);
		if ((NULL != m_hWnd) &&(hWnd == m_hWnd) && (NULL != m_hChildMenu))
		{
			CMDIFrame* pFrame = (CMDIFrame*)FromHandle(GetAncestor());
			if (m_hChildMenu)
				pFrame->UpdateFrameMenu(m_hChildMenu);
		}

		return (m_hChildMenu != NULL);
	}

	void CMDIChild::SetView(CWnd& View)
	// Sets or changes the View window displayed within the frame
	{
		// Destroy the existing view window (if any)
		if (m_pwndView) m_pwndView->Destroy();

		// Assign the view window
		m_pwndView = &View;

		SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	}

	LRESULT CMDIChild::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_MDIACTIVATE:
			{
				CMDIFrame* pMDIFrame = (CMDIFrame*)FromHandle(GetAncestor());

				// This child is being activated
				if (lParam == (LPARAM) m_hWnd)
				{
					pMDIFrame->m_hActiveMDIChild = m_hWnd;
					// Set the menu to child default menu
					if (m_hChildMenu)
						pMDIFrame->UpdateFrameMenu(m_hChildMenu);
				}

				// No child is being activated
				if (0 == lParam)
				{
					pMDIFrame->m_hActiveMDIChild = NULL;
					// Set the menu to frame's original menu
					pMDIFrame->UpdateFrameMenu(pMDIFrame->GetFrameMenu());
				}
			}
			return 0L ;

		case WM_WINDOWPOSCHANGED:
			{
				// Resize the View window
				CRect rc = GetClientRect();
				m_pwndView->SetWindowPos( NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW );
			}
		}
		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	}


} // namespace Win32xx
