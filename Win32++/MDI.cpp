// Win32++  Version 5.4
// Released: 24th August, 2007 by:
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

		while ((status = ::GetMessage(&uMsg, NULL, 0, 0))!= 0)
		{
			if (status == -1) return -1;
			if (!TranslateMDISysAccel(m_hWndAccel, &uMsg) &&
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

	LRESULT CMDIFrame::DefWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefFrameProc(hWnd, m_MDIClient.GetHwnd(), uMsg, wParam, lParam);
	}

	HWND CMDIFrame::GetActiveMDIChild(BOOL* pIsMaxed /* = NULL */)
	{
		return (HWND)::SendMessage(GetMDIClient().GetHwnd(), WM_MDIGETACTIVE, 0, (LPARAM)pIsMaxed);
	}

	BOOL CMDIFrame::IsMDIChildMaxed()
	{
		BOOL bMaxed = FALSE;
		GetActiveMDIChild(&bMaxed);
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

			CFrame* pFrame = GetApp()->GetFrame();
			pFrame->UpdateCheckMarks();
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

	void CMDIClient::PreCreate(CREATESTRUCT &cs)
	{
		static CLIENTCREATESTRUCT clientcreate;
		clientcreate.idFirstChild = IDW_FIRSTCHILD;

		cs.lpCreateParams = &clientcreate;
		cs.lpszClass = _T("MDICLient");
		cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		cs.dwExStyle = WS_EX_CLIENTEDGE;
	}

	LRESULT CMDIClient::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
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

			return m_hWnd;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(_T("Exception in CMDIChild::Create"));
		}
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
		if ((m_hWnd != NULL) &&(hWnd == m_hWnd) && (m_hChildMenu != NULL))
		{
			UpdateFrameMenu(m_hChildMenu);
		}

		return (m_hChildMenu != NULL);
	}

	void CMDIChild::UpdateFrameMenu(HMENU hMenu)
	{
		CFrame* pFrame = GetApp()->GetFrame();
		int nWindowItem = pFrame->GetMenuItemPos(hMenu, _T("Window"));

		HMENU hMenuWindow = ::GetSubMenu (hMenu, nWindowItem);
		::SendMessage (GetParent(m_hWnd), WM_MDISETMENU, (WPARAM) hMenu, (LPARAM)hMenuWindow);

		if (pFrame->IsMenubarUsed())
			pFrame->GetMenubar().SetMenu(hMenu);
		else
			::DrawMenuBar(pFrame->GetHwnd());

		pFrame->UpdateCheckMarks();
	}

	LRESULT CMDIChild::WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_MDIACTIVATE:
			{
				CMDIFrame* pMDIFrame = (CMDIFrame*)GetApp()->GetFrame();

				// This child is being activated
				if (lParam == (LPARAM) m_hWnd)
				{
					// Set the menu to child default menu
					UpdateFrameMenu(m_hChildMenu);
				}

				// No child is being activated
				if (lParam == 0)
				{
					// Set the menu to frame's original menu				
					UpdateFrameMenu(pMDIFrame->GetFrameMenu());
				}

				::DrawMenuBar(pMDIFrame->GetHwnd());
			}
			return 0L ;
		}
		return CWnd::WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

} // namespace Win32xx
