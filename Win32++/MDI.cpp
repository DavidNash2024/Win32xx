// Win32++  Version 5.1 Beta
// Modified: 11th February, 2007 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: http://users.bigpond.net.au/programming/
//
//
// Copyright (c) 2005-2007  David Nash
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
// Definitions for the following classes:
// CMDIApp, CMDIFrame, CMDIClient and CMDIChild classes


#include "WinCore.h"
#include "MDI.h"
#include "Default_Resource.h"


namespace Win32xx
{
	///////////////////////////////////
	// Definitions for the CMDIApp class
	//
	CMDIApp::CMDIApp(HINSTANCE hInstance) : CWinApp(hInstance)
	{
		m_hAccel = LoadAccelerators (hInstance, MAKEINTRESOURCE(IDW_MAIN)) ;
	}

	int CMDIApp::MessageLoop()
	{
		MSG uMsg;
		int status;
		CFrame* pFrame = GetFrame();

		while ((status = ::GetMessage(&uMsg, NULL, 0, 0))!= 0)
		{
			if (status == -1) return -1;
			if (!TranslateMDISysAccel(pFrame->GetView()->GetHwnd(), &uMsg) &&
				!TranslateAccelerator(pFrame->GetHwnd(), m_hAccel, &uMsg))
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
	CMDIFrame::CMDIFrame()
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

	HWND CMDIFrame::GetActiveChild(BOOL* pIsMaxed /* = NULL */)
	{
		return (HWND)::SendMessage(GetMDIClient().GetHwnd(), WM_MDIGETACTIVE, 0, (LPARAM)pIsMaxed);
	}

	BOOL CMDIFrame::OnCommand(UINT nID)
	{
		switch (nID)
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
				if (IsWindow (GetActiveChild()))
					((CMDIChild*)GetCWndObject(GetActiveChild()))->OnCommand(nID);
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
			HMENU hMenu= m_Menubar.GetMenu();
			m_Menubar.SetMenu(hMenu);
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
			HWND hwnd = (*v)->GetHwnd();
			::SendMessage(hwnd, WM_CLOSE, 0, 0);
			if (::IsWindow(hwnd))
				return FALSE;

			RemoveMDIChild(hwnd);
		}
		return TRUE;
	}

	void CMDIFrame::RemoveMDIChild(HWND hwnd)
	{
		// Allocate an iterator for our HWND map
		std::vector <CMDIChild*>::iterator v;

		for (v = m_MDIChildVect.begin(); v!= m_MDIChildVect.end(); v++)
		{
			if ((*v)->GetHwnd() == hwnd)
			{
				delete *v;
				m_MDIChildVect.erase(v);
				break;
			}
		}
	}

	LRESULT CMDIFrame::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CLOSE:
			OnClose();
			return 0;
		case WM_COMMAND:
			OnCommand(LOWORD(wParam));
			break;	// Some commands require default processing
		case WM_CREATE:
			OnCreate();
			break;  // Continue default processing
		case WM_DESTROY:
			::SetMenu(m_hWnd, NULL);
			::KillTimer(m_hWnd, ID_STATUS_TIMER);
			// Post the WM_QUIT message to terminate the application.
			::PostQuitMessage(0);
			return 0;
		case WM_ERASEBKGND:
			// Avoids unnecessary flicker
			return 0;
		case WM_HELP:
			OnHelp();
			return 0;
		case WM_MENUCHAR:
			if ((IsMenubarUsed()) && (LOWORD(wParam)!= VK_SPACE))
			{
				// Activate Menubar for key pressed with Alt key held down
				GetMenubar().MenuChar(wParam, lParam);
				return -1;
			}
			break;
		case WM_MENUSELECT:
			OnMenuSelect(wParam, lParam);
			return 0;
		case WM_NOTIFY:
			return OnNotify(wParam, lParam);
		case WM_SETFOCUS:
			OnSetFocus();
			return 0;
		case WM_SIZE:
			RecalcLayout();
			::RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
			return 0;
		case WM_SYSCOLORCHANGE:
			// Changing themes trigger this
			OnSysColorChange();
			return 0;
		case WM_SYSCOMMAND:
			if ((wParam == SC_KEYMENU) && (lParam != VK_SPACE) && IsMenubarUsed())
			{
				GetMenubar().SysCommand(wParam, lParam);
				return 0L;
			}
			break;
		case WM_TIMER:
			if (wParam == ID_STATUS_TIMER)
				SetStatusIndicators();
			return 0L;
		case WM_WINDOWPOSCHANGED:
			// MDI Child or MDI frame has been resized
			OnWindowPosChanged();
			break; // Continue with default processing

		} // switch uMsg
		return ::DefFrameProc(hwnd, GetView()->GetHwnd(), uMsg, wParam, lParam);
	}


	//////////////////////////////////////
	// Definitions for the CMDIClient class
	//
	CMDIClient::CMDIClient() //: m_OldWindowProc(NULL)
	{
		Superclass(TEXT("MDICLIENT"), TEXT("SuperMDIClient"));
	}

	CMDIClient::~CMDIClient()
	{
	}

	HWND CMDIClient::Create(HWND hWndParent /* = NULL*/)
	{
		try
		{
			CLIENTCREATESTRUCT clientcreate ;
			clientcreate.hWindowMenu  = m_hWnd;
			clientcreate.idFirstChild = IDW_FIRSTCHILD ;
			DWORD dword = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

			// Create the view window
			if (!CreateEx(WS_EX_CLIENTEDGE, TEXT("SuperMDIClient"), TEXT(""),
					dword, 0, 0, 0, 0, hWndParent, NULL, (PSTR) &clientcreate))
					throw CWinException(TEXT("CMDIClient::Create ... CreateEx failed"));

			return m_hWnd;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CMDIClient::Create"));
		}

		return m_hWnd;
	}

	LRESULT CMDIClient::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_KEYDOWN:
			if((wParam == VK_CAPITAL) || (wParam == VK_NUMLOCK) ||(wParam == VK_SCROLL))
				// Send these keys to the frame window
				::PostMessage(m_hWndParent, WM_KEYDOWN, wParam, lParam);
			return 0;

		case WM_MDIDESTROY:
			{
				// Do default processing first
				CallPrevWindowProc(hWnd, uMsg, wParam, lParam);

				// Now remove MDI child
				CMDIFrame* pFrame = (CMDIFrame*)GetApp()->GetFrame();
				pFrame->RemoveMDIChild((HWND) wParam);
			}
			return 0; // Discard message

		case WM_MDISETMENU:
			{
				CFrame* pFrame = GetApp()->GetFrame();
				if (pFrame->GetMenubar().GetHwnd())
				{
					::PostMessage(pFrame->GetMenubar().GetHwnd(), WM_MDISETMENU, wParam, lParam);
					return 0;
				}
			}
			break;
		}
		return CWnd::WndProc(hWnd, uMsg, wParam, lParam);
	}


	/////////////////////////////////////
	//Definitions for the CMDIChild class
	//
	CMDIChild::CMDIChild() : m_hChildMenu(NULL)
	{
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
		try
		{
			//Call PreCreate in case its overloaded
			PreCreate(m_cs);

			//Determine if the window should be created maximized
			BOOL bMax = FALSE;
			::SendMessage(hWndParent, WM_MDIGETACTIVE, 0, (LPARAM)&bMax);
			bMax = bMax | (m_cs.style & WS_MAXIMIZE);

			// Set the Window Class Name
			TCHAR szClassName[MAX_STRING_SIZE + 1] = TEXT("Win32++ MDI Child");
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
				throw CWinException(TEXT("CMDIChild::Create ... CreateEx failed"));

			if (bMax)
				::ShowWindow(m_hWnd, SW_MAXIMIZE);

			// Turn redraw back on
			::SendMessage(hWndParent, WM_SETREDRAW, TRUE, 0);
			::RedrawWindow(hWndParent, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);

			return m_hWnd;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CMDIChild::Create"));
		}
		return m_hWnd;
	}

	int CMDIChild::GetMenuWindowPos(HMENU hMenu)
	// Finds the menu item called "Window" and returns its position
	{
		int nMenuItemCount = ::GetMenuItemCount(hMenu);
		int WindowItem = -1;

		for (int i = 0 ; i < nMenuItemCount; i++)
		{
			TCHAR szMenuName[MAX_MENU_STRING +1];
			::GetMenuString(hMenu, i, szMenuName, MAX_MENU_STRING, MF_BYPOSITION);

			if (lstrcmp(szMenuName, TEXT("Window")) == 0)
				WindowItem = i;
			if (lstrcmp(szMenuName, TEXT("&Window")) == 0)
				WindowItem = i;
		}

		return WindowItem;
	}

	BOOL CMDIChild::OnCommand(UINT /*nID*/)
	{
		// Override this to handle WM_COMMAND messages, for example

		//	switch (nID)
		//	{
		//	case IDM_FILE_NEW:
		//		OnFileNew();
		//		break;
		//	}

		// return 0;

		return 0;
	}

	BOOL CMDIChild::SetChildMenu(LPCTSTR MenuName)
	{
		HINSTANCE hInstance = GetApp()->GetInstanceHandle();
		m_hChildMenu = ::LoadMenu (hInstance, MenuName);

		HWND hwnd = (HWND)::SendMessage(GetParent(m_hWnd), WM_MDIGETACTIVE, 0, 0);
		if ((m_hWnd != NULL) &&(hwnd == m_hWnd) && (m_hChildMenu != NULL))
			UpdateFrameMenu(m_hChildMenu);

		return (m_hChildMenu != NULL);
	}

	void CMDIChild::UpdateFrameMenu(HMENU hMenu)
	{
		CFrame* pFrame = GetApp()->GetFrame();
		int nWindowItem = GetMenuWindowPos(hMenu);
		HMENU hMenuWindow = ::GetSubMenu (hMenu, nWindowItem);

		::SendMessage (GetParent(m_hWnd), WM_MDISETMENU, (WPARAM) hMenu, (LPARAM)hMenuWindow);

		if (pFrame->IsMenubarUsed())
			pFrame->GetMenubar().SetMenu(hMenu);
		else
			::DrawMenuBar(pFrame->GetHwnd());
	}

	LRESULT CMDIChild::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_COMMAND:
			OnCommand(LOWORD(wParam));
			break;
		case WM_MDIACTIVATE:
			{
				CFrame* pFrame = GetApp()->GetFrame();

				// This child is being activated
				if (lParam == (LPARAM) m_hWnd)
					// Set the menu to child default menu if gaining focus
					UpdateFrameMenu(m_hChildMenu);

				// No child is being activated
				if (lParam == 0)
					// Set the menu to frame default menu if losing focus
					UpdateFrameMenu(pFrame->GetFrameMenu());

				::DrawMenuBar(pFrame->GetHwnd());
			}
			return 0 ;
		case WM_NOTIFY:
			return OnNotify(wParam, lParam);
		case WM_PAINT:
			{
				::PAINTSTRUCT ps;
				HDC hDC = ::BeginPaint(hwnd, &ps);
				OnPaint(hDC);
				::EndPaint(hwnd, &ps);
			}
			return 0;
		}

		return ::DefMDIChildProc(hwnd, uMsg, wParam, lParam);
	}

} // namespace Win32xx
