// Win32++  Version 6.0
// Released: 4th March, 2008 by:
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


////////////////////////////////////////////////////////
// MDI.cpp
// Definitions for the CMDIApp, CMDIFrame, CMDIClient
//  and CMDIChildclasses:



#include "WinCore.h"
#include "MDI.h"
#include "Default_Resource.h"


namespace Win32xx
{
	///////////////////////////////////
	// Definitions for the CMDIApp class
	//
	CMDIApp::CMDIApp()
	{
		m_hAccel = LoadAccelerators (GetInstanceHandle(), MAKEINTRESOURCE(IDW_MAIN)) ;
	}

	int CMDIApp::MessageLoop()
	{
		MSG uMsg;
		int status;

		while ((status = ::GetMessage(&uMsg, NULL, 0, 0))!= 0)
		{
			if (-1 == status) return -1;
			if (!TranslateMDISysAccel(GetFrame()->GetView()->GetHwnd(), &uMsg) &&
				!TranslateAccelerator(m_hWndAccel, m_hAccel, &uMsg))
			{
				::TranslateMessage(&uMsg);
				::DispatchMessage(&uMsg);
			}
		}
		return LOWORD(uMsg.wParam);
	}

	/////////////////////////////////////
	// Definitions for the CMDIFrame class
	//
	CMDIFrame::CMDIFrame() : m_hActiveMDIChild(NULL)
	{
		m_bIsMDIFrame = TRUE;
		SetView(m_MDIClient);
	}

	CMDIFrame::~CMDIFrame()
	{
		// Ensure all MDI child objects are destroyed
		std::vector <CMDIChild*>::iterator v;

		while(m_MDIChildVect.size() > 0)
		{
			v = m_MDIChildVect.begin();
			delete *v;
			m_MDIChildVect.erase(v);
		}
	}

	void CMDIFrame::AddMDIChild(CMDIChild* pMDIChild)
	{
		m_MDIChildVect.push_back(pMDIChild);
		pMDIChild->Create(GetView()->GetHwnd());
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

		for (v = GetMDIChildVect().begin(); v < GetMDIChildVect().end(); v++)
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
					tStringStream sStream;
					sStream << _T("&") << nWindow+1 << _T("  ") << (*v)->GetWindowString();
					tString MenuString = sStream.str();
					if (MenuString.length() > MAX_MENU_STRING)
					{
						// Truncate the string if its too long
						MenuString.erase(MAX_MENU_STRING);
						MenuString += _T(" ...");
					}

					::AppendMenu(hMenuWindow, MF_STRING, IDW_FIRSTCHILD + nWindow, MenuString.c_str() );

					if (GetActiveMDIChild() == hwndMDIChild)
						::CheckMenuItem(hMenuWindow, IDW_FIRSTCHILD+nWindow, MF_CHECKED);

					nWindow++;
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

	LRESULT CMDIFrame::DefWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefFrameProc(hWnd, m_MDIClient, uMsg, wParam, lParam);
	}

	CMDIChild* CMDIFrame::GetActiveMDIChildCWnd() const
	{
		return (CMDIChild*)FromHandle(GetActiveMDIChild());
	}

	BOOL CMDIFrame::IsMDIChildMaxed() const
	{
		BOOL bMaxed = FALSE;
		::SendMessage(m_MDIClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMaxed);
		return bMaxed;
	}

	BOOL CMDIFrame::OnCommandFrame(WPARAM wParam, LPARAM lParam)
	{
		switch (LOWORD(wParam))
		{
		case IDW_VIEW_STATUSBAR:
			OnViewStatusbar();
			::RedrawWindow(GetView()->GetHwnd(), NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
			break;
		case IDW_VIEW_TOOLBAR:
			OnViewToolbar();
			::RedrawWindow(GetView()->GetHwnd(), NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
			break;
		case IDW_WINDOW_ARRANGE:
			::PostMessage (GetView()->GetHwnd(), WM_MDIICONARRANGE, 0, 0) ;
			break;
		case IDW_WINDOW_CASCADE:
			::PostMessage (GetView()->GetHwnd(), WM_MDICASCADE, 0, 0) ;
			break;
		case IDW_WINDOW_CLOSEALL:
			RemoveAllMDIChildren();
			break;
		case IDW_WINDOW_TILE:
			::PostMessage (GetView()->GetHwnd(), WM_MDITILE, 0, 0) ;
			break;
		default:    // Pass to active child...
			{
				if (IsWindow (GetActiveMDIChild()))
					::SendMessage(GetActiveMDIChild(), WM_COMMAND, wParam, lParam);
			}
			break ;
		}
		return 0;
	}

	void CMDIFrame::OnClose()
	{
		if (RemoveAllMDIChildren())
			::DestroyWindow(m_hWnd);
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

	void CMDIFrame::RecalcLayout()
	{
		CFrame::RecalcLayout();
		::RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
	}

	BOOL CMDIFrame::RemoveAllMDIChildren()
	{
		// Allocate an iterator for our MDIChild vector
		std::vector <CMDIChild*>::iterator v;

		while(m_MDIChildVect.size() > 0)
		{
			v = m_MDIChildVect.begin();
			HWND hWnd = (*v)->GetHwnd();
			::SendMessage(hWnd, WM_CLOSE, 0, 0);
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

		for (v = m_MDIChildVect.begin(); v!= m_MDIChildVect.end(); v++)
		{
			if ((*v)->GetHwnd() == hWnd)
			{
				delete *v;
				m_MDIChildVect.erase(v);
				break;
			}
		}

		if ((GetActiveMDIChildCWnd()) && GetActiveMDIChildCWnd()->m_hChildMenu)
		{
			UpdateFrameMenu(GetActiveMDIChildCWnd()->m_hChildMenu);
		}
		else
		{
			if (IsMenubarUsed())
				GetMenubar().SetMenu(GetFrameMenu());
			else
				::SetMenu(m_hWnd, GetFrameMenu());
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

	LRESULT CMDIFrame::WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		return CFrame::WndProcDefault(hWnd, uMsg, wParam, lParam);
	}


	//////////////////////////////////////
	// Definitions for the CMDIClient class
	//
	CMDIClient::CMDIClient()
	{
	}

	CMDIClient::~CMDIClient()
	{
	}

	HWND CMDIClient::Create(HWND hWndParent /* = NULL*/)
	{
		CLIENTCREATESTRUCT clientcreate ;
		clientcreate.hWindowMenu  = m_hWnd;
		clientcreate.idFirstChild = IDW_FIRSTCHILD ;
		DWORD dword = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

		// Create the view window
		if (!CreateEx(WS_EX_CLIENTEDGE, _T("MDICLient"), TEXT(""),
				dword, 0, 0, 0, 0, hWndParent, NULL, (PSTR) &clientcreate))
				throw CWinException(TEXT("CMDIClient::Create ... CreateEx failed"));

		return m_hWnd;
	}

	LRESULT CMDIClient::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		CMDIFrame* pMDIFrame = (CMDIFrame*)FromHandle(m_hWndParent);
		switch (uMsg)
		{
		case WM_MDIDESTROY:
			{
				// Do default processing first
				CallPrevWindowProc(hWnd, uMsg, wParam, lParam);

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
				::SendMessage(m_hWnd, WM_SETREDRAW, FALSE, 0);
				LRESULT lr = CallPrevWindowProc(m_hWnd, WM_MDIACTIVATE, wParam, lParam);
				::SendMessage(m_hWnd, WM_SETREDRAW, TRUE, 0);
				::RedrawWindow(m_hWnd, 0, 0, RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);

				return lr;
			}
		}
		return CWnd::WndProcDefault(hWnd, uMsg, wParam, lParam);
	}


	/////////////////////////////////////
	//Definitions for the CMDIChild class
	//
	CMDIChild::CMDIChild() : m_hChildMenu(NULL)
	{
		// Set the MDI Child's menu in the constructor, like this ...

		// SetChildMenu(_T("MdiMenuView"));
	}

	CMDIChild::~CMDIChild()
	{
		if (IsWindow(m_hWnd))
			::SendMessage(m_hWndParent, WM_MDIDESTROY, (WPARAM)m_hWnd, 0);

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
		::SendMessage(hWndParent, WM_MDIGETACTIVE, 0, (LPARAM)&bMax);
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
		::SendMessage(hWndParent, WM_SETREDRAW, FALSE, 0);

		// Create the window
		if (!CreateEx(dwExStyle, szClassName, m_cs.lpszName, dwStyle, x, y,
			cx, cy, hWndParent, m_cs.hMenu, m_cs.lpCreateParams))
			throw CWinException(_T("CMDIChild::Create ... CreateEx failed"));

		if (bMax)
			::ShowWindow(m_hWnd, SW_MAXIMIZE);

		// Turn redraw back on
		::SendMessage(hWndParent, WM_SETREDRAW, TRUE, 0);
		::RedrawWindow(hWndParent, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);

		// Ensure bits revealed by round corners (XP themes) are redrawn
		::SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);

		CMDIFrame* pMDIFrame = (CMDIFrame*)FromHandle(GetAncestor(m_hWnd));
		if (m_hChildMenu)
			pMDIFrame->UpdateFrameMenu(m_hChildMenu);

		return m_hWnd;
	}

	LRESULT CMDIChild::DefWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefMDIChildProc(hWnd, uMsg, wParam, lParam);
	}

	BOOL CMDIChild::SetChildMenu(LPCTSTR MenuName)
	{
		HINSTANCE hInstance = GetApp()->GetInstanceHandle();
		m_hChildMenu = ::LoadMenu (hInstance, MenuName);

		HWND hWnd = (HWND)::SendMessage(GetParent(m_hWnd), WM_MDIGETACTIVE, 0, 0);
		if ((NULL != m_hWnd) &&(hWnd == m_hWnd) && (NULL != m_hChildMenu))
		{
			CMDIFrame* pFrame = (CMDIFrame*)FromHandle(GetAncestor(m_hWnd));
			if (m_hChildMenu)
				pFrame->UpdateFrameMenu(m_hChildMenu);
		}

		return (m_hChildMenu != NULL);
	}

	LRESULT CMDIChild::WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_MDIACTIVATE:
			{
				CMDIFrame* pMDIFrame = (CMDIFrame*)FromHandle(GetAncestor(m_hWnd));

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
		}
		return CWnd::WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

} // namespace Win32xx
