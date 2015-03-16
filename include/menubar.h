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


#ifndef _WIN32XX_MENUBAR_H_
#define _WIN32XX_MENUBAR_H_

#include "wincore.h"
#include "gdi.h"
#include "controls.h"
#include "toolbar.h"

namespace Win32xx
{

// The CMenuBar class provides a menu inside a rebar control.
// CMenuBar inherits from CToolBar.


	////////////////////////////////////
	// Declaration of the CMenuBar class
	//
	class CMenuBar : public CToolBar
	{

	public:
		CMenuBar();
		virtual ~CMenuBar();
		virtual void SetMenu(HMENU hMenu);

		void DrawAllMDIButtons(CDC* pDrawDC);
		CWnd* GetMDIClient() const;
		CWnd* GetActiveMDIChild() const;
		HMENU GetMenu() const {return m_hTopMenu;}
		virtual LRESULT OnMenuChar(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnSysCommand(WPARAM wParam, LPARAM lParam);

	protected:
	//Overridables
		virtual void OnAttach();
		virtual LRESULT OnDrawItem(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnExitMenuLoop(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnInitMenuPopup(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnKillFocus(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnMeasureItem(WPARAM wParam, LPARAM lParam);
		virtual BOOL	OnMenuInput(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnSysKeyDown(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnSysKeyUp(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnTBNDropDown(LPNMTOOLBAR pNMTB);
		virtual LRESULT OnTBNHotItemChange(LPNMTBHOTITEM pNMHI);
		virtual LRESULT OnWindowPosChanged(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnWindowPosChanging(WPARAM wParam, LPARAM lParam);
		virtual void	PreCreate(CREATESTRUCT &cs);
		virtual void	PreRegisterClass(WNDCLASS &wc);
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		CMenuBar(const CMenuBar&);				// Disable copy construction
		CMenuBar& operator = (const CMenuBar&); // Disable assignment operator
		void DoAltKey(WORD KeyCode);
		void DrawMDIButton(CDC* pDrawDC, int iButton, UINT uState);
		void ExitMenu();
		void GrabFocus();
		BOOL IsMDIChildMaxed() const;
		BOOL IsMDIFrame() const;
		LRESULT OnPopupMenu();
		void ReleaseFocus();
		void SetHotItem(int nHot);
		static LRESULT CALLBACK StaticMsgHook(int nCode, WPARAM wParam, LPARAM lParam);

		enum MDIButtonType
		{
			MDI_MIN = 0,
			MDI_RESTORE = 1,
			MDI_CLOSE = 2
		};

		BOOL  m_IsExitAfter;	// Exit after Popup menu ends
		BOOL  m_IsKeyMode;		// keyboard navigation mode
		BOOL  m_IsMenuActive;	// popup menu active
		BOOL  m_IsSelPopup;		// a popup (cascade) menu is selected
		HMENU m_hPopupMenu;		// handle to the popup menu
		HMENU m_hSelMenu;		// handle to the cascaded popup menu
		HMENU m_hTopMenu;		// handle to the top level menu
		HWND  m_hPrevFocus;		// handle to window which had focus
		CRect m_MDIRect[3];		// array of CRect for MDI buttons
		int   m_nHotItem;		// hot item
		int   m_nMDIButton;		// the MDI button (MDIButtonType) pressed
		CPoint m_OldMousePos;	// old Mouse position
		CWnd* m_pFrame;         // Pointer to the frame

	};  // class CMenuBar

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{

	/////////////////////////////////////
	// Definitions for the CMenuBar class
	//
	inline CMenuBar::CMenuBar()
	{
		m_IsExitAfter	= FALSE;
		m_hTopMenu		= NULL;
		m_nHotItem		= -1;
		m_IsSelPopup	= FALSE;
		m_hSelMenu		= NULL;
		m_IsMenuActive	= FALSE;
		m_IsKeyMode		= FALSE;
		m_hPrevFocus	= NULL;
		m_nMDIButton    = 0;
		m_hPopupMenu	= 0;
		m_pFrame		= 0;
	}

	inline CMenuBar::~CMenuBar()
	{
	}

	inline void CMenuBar::DoAltKey(WORD KeyCode)
	{
		//Handle key pressed with Alt held down
		UINT ID;
		if (SendMessage(TB_MAPACCELERATOR, KeyCode, (LPARAM) &ID))
		{
			GrabFocus();
			m_IsKeyMode = TRUE;
			SetHotItem(ID);
			m_IsMenuActive = TRUE;
			PostMessage(UWM_POPUPMENU, 0L, 0L);
		}
		else
			::MessageBeep(MB_OK);
	}

	inline void CMenuBar::DrawAllMDIButtons(CDC* pDrawDC)
	{
		if (!IsMDIFrame())
			return;

		if (IsMDIChildMaxed())
		{
			int cx = GetSystemMetrics(SM_CXSMICON);
			int cy = GetSystemMetrics(SM_CYSMICON);
			CRect rc = GetClientRect();
			int gap = 4;
			rc.right -= gap;

			// Assign values to each element of the CRect array
			for (int i = 0 ; i < 3 ; ++i)
			{
				int left = rc.right - (i+1)*cx - gap*(i+1);
				int top = rc.bottom/2 - cy/2;
				int right = rc.right - i*cx - gap*(i+1);
				int bottom = rc.bottom/2 + cy/2;
				::SetRect(&m_MDIRect[2 - i], left, top, right, bottom);
			}

			// Hide the MDI button if it won't fit
			for (int k = 0 ; k <= 2 ; ++k)
			{

				if (m_MDIRect[k].left < GetMaxSize().cx)
				{
					::SetRectEmpty(&m_MDIRect[k]);
				}
			}

			DrawMDIButton(pDrawDC, MDI_MIN, 0);
			DrawMDIButton(pDrawDC, MDI_RESTORE, 0);
			DrawMDIButton(pDrawDC, MDI_CLOSE, 0);
		}
	}

	inline void CMenuBar::DrawMDIButton(CDC* pDrawDC, int iButton, UINT uState)
	{
		if (!IsRectEmpty(&m_MDIRect[iButton]))
		{
			switch (uState)
			{
			case 0:
				{
					// Draw a grey outline
					pDrawDC->CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNFACE));
					pDrawDC->MoveTo(m_MDIRect[iButton].left, m_MDIRect[iButton].bottom);
					pDrawDC->LineTo(m_MDIRect[iButton].right, m_MDIRect[iButton].bottom);
					pDrawDC->LineTo(m_MDIRect[iButton].right, m_MDIRect[iButton].top);
					pDrawDC->LineTo(m_MDIRect[iButton].left, m_MDIRect[iButton].top);
					pDrawDC->LineTo(m_MDIRect[iButton].left, m_MDIRect[iButton].bottom);
				}
				break;
			case 1:
				{
					// Draw outline, white at top, black on bottom
					pDrawDC->CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					pDrawDC->MoveTo(m_MDIRect[iButton].left, m_MDIRect[iButton].bottom);
					pDrawDC->LineTo(m_MDIRect[iButton].right, m_MDIRect[iButton].bottom);
					pDrawDC->LineTo(m_MDIRect[iButton].right, m_MDIRect[iButton].top);
					pDrawDC->CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
					pDrawDC->LineTo(m_MDIRect[iButton].left, m_MDIRect[iButton].top);
					pDrawDC->LineTo(m_MDIRect[iButton].left, m_MDIRect[iButton].bottom);
				}

				break;
			case 2:
				{
					// Draw outline, black on top, white on bottom
					pDrawDC->CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
					pDrawDC->MoveTo(m_MDIRect[iButton].left, m_MDIRect[iButton].bottom);
					pDrawDC->LineTo(m_MDIRect[iButton].right, m_MDIRect[iButton].bottom);
					pDrawDC->LineTo(m_MDIRect[iButton].right, m_MDIRect[iButton].top);
					pDrawDC->CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					pDrawDC->LineTo(m_MDIRect[iButton].left, m_MDIRect[iButton].top);
					pDrawDC->LineTo(m_MDIRect[iButton].left, m_MDIRect[iButton].bottom);
				}
				break;
			}

			pDrawDC->CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

			switch (iButton)
			{
			case MDI_MIN:
				// Manually Draw Minimise button
				pDrawDC->MoveTo(m_MDIRect[0].left + 4, m_MDIRect[0].bottom -4);
				pDrawDC->LineTo(m_MDIRect[0].right - 4, m_MDIRect[0].bottom - 4);

				pDrawDC->MoveTo(m_MDIRect[0].left + 4, m_MDIRect[0].bottom -5);
				pDrawDC->LineTo(m_MDIRect[0].right - 4, m_MDIRect[0].bottom - 5);
				break;
			case MDI_RESTORE:
				// Manually Draw Restore Button
				pDrawDC->MoveTo(m_MDIRect[1].left + 3, m_MDIRect[1].top + 7);
				pDrawDC->LineTo(m_MDIRect[1].left + 3, m_MDIRect[1].bottom -4);
				pDrawDC->LineTo(m_MDIRect[1].right - 6, m_MDIRect[1].bottom -4);
				pDrawDC->LineTo(m_MDIRect[1].right - 6, m_MDIRect[1].top + 7);
				pDrawDC->LineTo(m_MDIRect[1].left + 3, m_MDIRect[1].top + 7);

				pDrawDC->MoveTo(m_MDIRect[1].left + 3, m_MDIRect[1].top + 8);
				pDrawDC->LineTo(m_MDIRect[1].right - 6, m_MDIRect[1].top + 8);

				pDrawDC->MoveTo(m_MDIRect[1].left + 5, m_MDIRect[1].top + 7);
				pDrawDC->LineTo(m_MDIRect[1].left + 5, m_MDIRect[1].top + 4);
				pDrawDC->LineTo(m_MDIRect[1].right - 4, m_MDIRect[1].top + 4);
				pDrawDC->LineTo(m_MDIRect[1].right - 4, m_MDIRect[1].bottom -6);
				pDrawDC->LineTo(m_MDIRect[1].right - 6, m_MDIRect[1].bottom -6);

				pDrawDC->MoveTo(m_MDIRect[1].left + 5, m_MDIRect[1].top + 5);
				pDrawDC->LineTo(m_MDIRect[1].right - 4, m_MDIRect[1].top + 5);
				break;
			case MDI_CLOSE:
				// Manually Draw Close Button
				pDrawDC->MoveTo(m_MDIRect[2].left + 4, m_MDIRect[2].top +5);
				pDrawDC->LineTo(m_MDIRect[2].right - 4, m_MDIRect[2].bottom -3);

				pDrawDC->MoveTo(m_MDIRect[2].left + 5, m_MDIRect[2].top +5);
				pDrawDC->LineTo(m_MDIRect[2].right - 4, m_MDIRect[2].bottom -4);

				pDrawDC->MoveTo(m_MDIRect[2].left + 4, m_MDIRect[2].top +6);
				pDrawDC->LineTo(m_MDIRect[2].right - 5, m_MDIRect[2].bottom -3);

				pDrawDC->MoveTo(m_MDIRect[2].right -5, m_MDIRect[2].top +5);
				pDrawDC->LineTo(m_MDIRect[2].left + 3, m_MDIRect[2].bottom -3);

				pDrawDC->MoveTo(m_MDIRect[2].right -5, m_MDIRect[2].top +6);
				pDrawDC->LineTo(m_MDIRect[2].left + 4, m_MDIRect[2].bottom -3);

				pDrawDC->MoveTo(m_MDIRect[2].right -6, m_MDIRect[2].top +5);
				pDrawDC->LineTo(m_MDIRect[2].left + 3, m_MDIRect[2].bottom -4);
				break;
			}
		}
	}

	inline void CMenuBar::ExitMenu()
	{
		ReleaseFocus();
		m_IsKeyMode = FALSE;
		m_IsMenuActive = FALSE;
		SendMessage(TB_PRESSBUTTON, (WPARAM)m_nHotItem, (LPARAM)MAKELONG (FALSE, 0));
		SetHotItem(-1);

		CPoint pt = GetCursorPos();
		ScreenToClient(pt);

		// Update mouse mouse position for hot tracking
		SendMessage(WM_MOUSEMOVE, 0L, (LPARAM)MAKELONG(pt.x, pt.y));
	}

	inline CWnd* CMenuBar::GetActiveMDIChild() const
	{
		CWnd* pMDIChild = NULL;
		if (GetMDIClient())
		{
			pMDIChild = FromHandle((HWND)GetMDIClient()->SendMessage(WM_MDIGETACTIVE, 0L, 0L));
		}

		return pMDIChild;
	}

	inline CWnd* CMenuBar::GetMDIClient() const
	{
		CWnd* pMDIClient = NULL;
		HWND hWnd = reinterpret_cast<HWND>(m_pFrame->SendMessage(UWM_GETFRAMEVIEW, 0L, 0L));
		CWnd* pWnd = FromHandle(hWnd);
		if (pWnd && pWnd->GetClassName() == _T("MDIClient"))
			pMDIClient = pWnd;

		return pMDIClient;
	}

	inline void CMenuBar::GrabFocus()
	{
		if (GetFocus() != this)
			m_hPrevFocus = ::SetFocus(m_hWnd);
		SetCapture();
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}

	inline BOOL CMenuBar::IsMDIChildMaxed() const
	{
		BOOL bMaxed = FALSE;
		if (GetMDIClient())
			GetMDIClient()->SendMessage(WM_MDIGETACTIVE, 0L, (LPARAM)&bMaxed);

		return bMaxed;
	}

	inline BOOL CMenuBar::IsMDIFrame() const
	{
		return (GetMDIClient() != 0);
	}

	inline LRESULT CMenuBar::OnMenuChar(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);

		if (!m_IsMenuActive)
			DoAltKey(LOWORD(wParam));

		return FinalWindowProc(WM_MENUCHAR, wParam, lParam);
	}

	inline void CMenuBar::OnAttach()
	{
		// We must send this message before sending the TB_ADDBITMAP or TB_ADDBUTTONS message
		SendMessage(TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0L);

		TLSData* pTLSData = GetApp()->GetTlsData();
		m_pFrame = pTLSData->pMainWnd;
	}

	inline LRESULT CMenuBar::OnDrawItem(WPARAM wParam, LPARAM lParam)
	{
		m_pFrame->SendMessage(WM_DRAWITEM, wParam, lParam);
		return TRUE; // handled
	}

	inline LRESULT CMenuBar::OnExitMenuLoop(WPARAM wParam, LPARAM lParam)
	{
		if (m_IsExitAfter)
			ExitMenu();
		m_pFrame->SendMessage(WM_EXITMENULOOP, wParam, lParam);

		return 0L;
	}

	inline LRESULT CMenuBar::OnInitMenuPopup(WPARAM wParam, LPARAM lParam)
	{
		m_pFrame->SendMessage(WM_INITMENUPOPUP, wParam, lParam);
		return 0L;
	}

	inline LRESULT CMenuBar::OnKeyDown(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);

		switch (wParam)
		{
		case VK_ESCAPE:
			ExitMenu();
			break;

		case VK_SPACE:
			ExitMenu();
			// Bring up the system menu
			GetAncestor()->PostMessage(WM_SYSCOMMAND, SC_KEYMENU, VK_SPACE);
			break;

		// Handle VK_DOWN,VK_UP and VK_RETURN together
		case VK_DOWN:
		case VK_UP:
		case VK_RETURN:
			// Always use PostMessage for USER_POPUPMENU (not SendMessage)
			PostMessage(UWM_POPUPMENU, 0L, 0L);
			break;

		case VK_LEFT:
			// Move left to next topmenu item
			(m_nHotItem > 0)? SetHotItem(m_nHotItem -1) : SetHotItem(GetButtonCount()-1);
			break;

		case VK_RIGHT:
			// Move right to next topmenu item
			(m_nHotItem < GetButtonCount() -1)? SetHotItem(m_nHotItem +1) : SetHotItem(0);
			break;

		default:
			// Handle Accelerator keys with Alt toggled down
			if (m_IsKeyMode)
			{
				UINT ID;
				if (SendMessage(TB_MAPACCELERATOR, wParam, (LPARAM) &ID))
				{
					m_nHotItem = ID;
					PostMessage(UWM_POPUPMENU, 0L, 0L);
				}
				else
					::MessageBeep(MB_OK);
			}
			break;
		} // switch (wParam)

		return 0L;	// Discard these messages
	}

	inline LRESULT CMenuBar::OnKillFocus(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);
		UNREFERENCED_PARAMETER(lParam);

		ExitMenu();
		return 0;
	}

	inline LRESULT CMenuBar::OnLButtonDown(WPARAM wParam, LPARAM lParam)
	{
		// Do default processing first
		CallWindowProc(GetPrevWindowProc(), WM_LBUTTONDOWN, wParam, lParam);

		GrabFocus();
		m_nMDIButton = 0;
		CPoint pt;

		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		if (IsMDIFrame())
		{
			if (IsMDIChildMaxed())
			{
				CClientDC MenuBarDC(this);
				m_nMDIButton = -1;

				if (m_MDIRect[0].PtInRect(pt)) m_nMDIButton = 0;
				if (m_MDIRect[1].PtInRect(pt)) m_nMDIButton = 1;
				if (m_MDIRect[2].PtInRect(pt)) m_nMDIButton = 2;

				if (m_nMDIButton >= 0)
				{
					DrawMDIButton(&MenuBarDC, MDI_MIN,     (0 == m_nMDIButton)? 2 : 0);
					DrawMDIButton(&MenuBarDC, MDI_RESTORE, (1 == m_nMDIButton)? 2 : 0);
					DrawMDIButton(&MenuBarDC, MDI_CLOSE,   (2 == m_nMDIButton)? 2 : 0);
				}

				// Bring up the MDI Child window's system menu when the icon is pressed
				if (HitTest() == 0)
				{
					m_nHotItem = 0;
					PostMessage(UWM_POPUPMENU, 0L, 0L);
				}
			}
		}

		return 0L;
	}

	inline LRESULT CMenuBar::OnLButtonUp(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);
		UNREFERENCED_PARAMETER(lParam);

		if (IsMDIFrame())
		{
			CWnd* pMDIClient = GetMDIClient();
			CWnd* pMDIChild = GetActiveMDIChild();
			assert(pMDIClient);

			if (pMDIChild && IsMDIChildMaxed())
			{
				CPoint pt = GetCursorPos();
				ScreenToClient(pt);

				// Process the MDI button action when the left mouse button is up
				if (m_MDIRect[0].PtInRect(pt))
				{
					if (MDI_MIN == m_nMDIButton)
						pMDIChild->ShowWindow(SW_MINIMIZE);
				}

				if (m_MDIRect[1].PtInRect(pt))
				{
					if (MDI_RESTORE == m_nMDIButton)
						pMDIClient->PostMessage(WM_MDIRESTORE, (WPARAM)pMDIChild->GetHwnd(), 0L);
				}

				if (m_MDIRect[2].PtInRect(pt))
				{
					if (MDI_CLOSE == m_nMDIButton)
						pMDIChild->PostMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
				}
			}
		}
		m_nMDIButton = 0;
		ExitMenu();

		return 0L;
	}

	inline LRESULT CMenuBar::OnMeasureItem(WPARAM wParam, LPARAM lParam)
	{
		m_pFrame->SendMessage(WM_MEASUREITEM, wParam, lParam);
		return TRUE; // handled
	}

	inline BOOL CMenuBar::OnMenuInput(UINT uMsg, WPARAM wParam, LPARAM lParam)
	// When a popup menu is active, StaticMsgHook directs all menu messages here
	{
		switch(uMsg)
		{
		case WM_KEYDOWN:
			m_IsExitAfter = FALSE;
			{
				switch (wParam)
				{
				case VK_ESCAPE:
					// Use default processing if inside a Sub Menu
					if ((m_hSelMenu) &&(m_hSelMenu != m_hPopupMenu))
						return FALSE;

					m_IsMenuActive = FALSE;
					m_IsKeyMode = TRUE;
					SendMessage(WM_CANCELMODE, 0L, 0L);
					SendMessage(TB_PRESSBUTTON, (WPARAM)m_nHotItem, (LPARAM)MAKELONG(FALSE, 0));
					SendMessage(TB_SETHOTITEM, (WPARAM)m_nHotItem, 0L);
					ExitMenu();
					break;

				case VK_LEFT:
					// Use default processing if inside a Sub Menu
					if ((m_hSelMenu) &&(m_hSelMenu != m_hPopupMenu))
						return FALSE;

					SendMessage(TB_PRESSBUTTON, (WPARAM)m_nHotItem, (LPARAM)MAKELONG(FALSE, 0));

					// Move left to next topmenu item
					(m_nHotItem > 0)? --m_nHotItem : m_nHotItem = GetButtonCount()-1;
					SendMessage(WM_CANCELMODE, 0L, 0L);

					// Always use PostMessage for USER_POPUPMENU (not SendMessage)
					PostMessage(UWM_POPUPMENU, 0L, 0L);
					PostMessage(WM_KEYDOWN, VK_DOWN, 0L);
					break;

				case VK_RIGHT:
					// Use default processing to open Sub Menu
					if (m_IsSelPopup)
						return FALSE;

					SendMessage(TB_PRESSBUTTON, (WPARAM)m_nHotItem, (LPARAM)MAKELONG(FALSE, 0));

					// Move right to next topmenu item
					(m_nHotItem < GetButtonCount()-1)? ++m_nHotItem : m_nHotItem = 0;
					SendMessage(WM_CANCELMODE, 0L, 0L);

					// Always use PostMessage for USER_POPUPMENU (not SendMessage)
					PostMessage(UWM_POPUPMENU, 0L, 0L);
					PostMessage(WM_KEYDOWN, VK_DOWN, 0L);
					break;

				case VK_RETURN:
					m_IsExitAfter = TRUE;
					break;

				} // switch (wParam)

			} // case WM_KEYDOWN

			return FALSE;

		case WM_CHAR:
			m_IsExitAfter = TRUE;
			return FALSE;

		case WM_LBUTTONDOWN:
			{
				m_IsExitAfter = TRUE;
				if (HitTest() >= 0)
				{
					// Cancel popup when we hit a button a second time
					SendMessage(WM_CANCELMODE, 0L, 0L);
					return TRUE;
				}
			}
			return FALSE;

		case WM_LBUTTONDBLCLK:
			// Perform default action for DblClick on MDI Maxed icon
			if (IsMDIChildMaxed() && (0 == HitTest()))
			{
				CWnd* pMDIChild = GetActiveMDIChild();
				assert(pMDIChild);
				CMenu* pChildMenu = pMDIChild->GetSystemMenu(FALSE);

				UINT nID = pChildMenu->GetDefaultItem(FALSE, 0);
				if (nID)
					pMDIChild->PostMessage(WM_SYSCOMMAND, nID, 0L);
			}

			m_IsExitAfter = TRUE;
			return FALSE;

		case WM_MENUSELECT:
			{
				// store info about selected item
				m_hSelMenu = (HMENU)lParam;
				m_IsSelPopup = ((HIWORD(wParam) & MF_POPUP) != 0);

				// Reflect message back to the frame window
				GetAncestor()->SendMessage(WM_MENUSELECT, wParam, lParam);
			}
			return TRUE;

		case WM_MOUSEMOVE:
			{
				CPoint pt;
				pt.x = GET_X_LPARAM(lParam);
				pt.y = GET_Y_LPARAM(lParam);

				// Skip if mouse hasn't moved
				if ((pt.x == m_OldMousePos.x) && (pt.y == m_OldMousePos.y))
					return FALSE;

				m_OldMousePos.x = pt.x;
				m_OldMousePos.y = pt.y;
				ScreenToClient(pt);

				// Reflect messages back to the MenuBar for hot tracking
				SendMessage(WM_MOUSEMOVE, 0L, MAKELPARAM(pt.x, pt.y));
			}
			break;

		}
		return FALSE;
	}

	inline LRESULT CMenuBar::OnMouseLeave(WPARAM wParam, LPARAM lParam)
	{
		if (IsMDIFrame())
		{
			if (IsMDIChildMaxed())
			{
				CClientDC MenuBarDC(this);

				DrawMDIButton(&MenuBarDC, MDI_MIN,     0);
				DrawMDIButton(&MenuBarDC, MDI_RESTORE, 0);
				DrawMDIButton(&MenuBarDC, MDI_CLOSE,   0);
			}
		}

		return FinalWindowProc(WM_MOUSELEAVE, wParam, lParam);
	}

	inline LRESULT CMenuBar::OnMouseMove(WPARAM wParam, LPARAM lParam)
	{
		CPoint pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		if (IsMDIFrame())
		{
			if (IsMDIChildMaxed())
			{
				CClientDC MenuBarDC(this);
				int MDIButton = -1;
				if (m_MDIRect[0].PtInRect(pt)) MDIButton = 0;
				if (m_MDIRect[1].PtInRect(pt)) MDIButton = 1;
				if (m_MDIRect[2].PtInRect(pt)) MDIButton = 2;

				if (MK_LBUTTON == wParam)  // mouse moved with left mouse button is held down
				{
					// toggle the MDI button image pressed/unpressed as required
					if (MDIButton >= 0)
					{
						DrawMDIButton(&MenuBarDC, MDI_MIN,     ((0 == MDIButton) && (0 == m_nMDIButton))? 2 : 0);
						DrawMDIButton(&MenuBarDC, MDI_RESTORE, ((1 == MDIButton) && (1 == m_nMDIButton))? 2 : 0);
						DrawMDIButton(&MenuBarDC, MDI_CLOSE,   ((2 == MDIButton) && (2 == m_nMDIButton))? 2 : 0);
					}
					else
					{
						DrawMDIButton(&MenuBarDC, MDI_MIN,     0);
						DrawMDIButton(&MenuBarDC, MDI_RESTORE, 0);
						DrawMDIButton(&MenuBarDC, MDI_CLOSE,   0);
					}
				}
				else	// mouse moved without left mouse button held down
				{
					if (MDIButton >= 0)
					{
						DrawMDIButton(&MenuBarDC, MDI_MIN,     (0 == MDIButton)? 1 : 0);
						DrawMDIButton(&MenuBarDC, MDI_RESTORE, (1 == MDIButton)? 1 : 0);
						DrawMDIButton(&MenuBarDC, MDI_CLOSE,   (2 == MDIButton)? 1 : 0);
					}
					else
					{
						DrawMDIButton(&MenuBarDC, MDI_MIN,     0);
						DrawMDIButton(&MenuBarDC, MDI_RESTORE, 0);
						DrawMDIButton(&MenuBarDC, MDI_CLOSE,   0);
					}
				}
			}
		}

		return FinalWindowProc(WM_MOUSEMOVE, wParam, lParam);
	}

	inline LRESULT CMenuBar::OnNotifyReflect(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);

		switch (((LPNMHDR)lParam)->code)
		{
		case TBN_DROPDOWN:		return OnTBNDropDown((LPNMTOOLBAR) lParam);
		case TBN_HOTITEMCHANGE:	return OnTBNHotItemChange((LPNMTBHOTITEM) lParam);
		}

		return 0L;
	}

	inline LRESULT CMenuBar::OnPopupMenu()
	{
		if (m_IsKeyMode)
			// Simulate a down arrow key press
			PostMessage(WM_KEYDOWN, VK_DOWN, 0L);

		m_IsKeyMode = FALSE;
		m_IsExitAfter = FALSE;
		m_OldMousePos = GetCursorPos();

		CWnd* pMaxMDIChild = NULL;
		if (IsMDIChildMaxed())
			pMaxMDIChild = GetActiveMDIChild();

		// Load the submenu
		int nMaxedOffset = IsMDIChildMaxed()? 1:0;
		m_hPopupMenu = ::GetSubMenu(m_hTopMenu, m_nHotItem - nMaxedOffset);
		if (IsMDIChildMaxed() && (0 == m_nHotItem) )
		m_hPopupMenu = ::GetSystemMenu(*pMaxMDIChild, FALSE);

        // Retrieve the bounding rectangle for the toolbar button
		CRect rc = GetItemRect(m_nHotItem);

		// convert rectangle to desktop coordinates
		ClientToScreen(rc);

		// Position popup above toolbar if it won't fit below
		TPMPARAMS tpm;
		tpm.cbSize = sizeof(TPMPARAMS);
		tpm.rcExclude = rc;

		// Set the hot button
		SendMessage(TB_SETHOTITEM, (WPARAM)m_nHotItem, 0L);
		SendMessage(TB_PRESSBUTTON, (WPARAM)m_nHotItem, (LPARAM)MAKELONG(TRUE, 0));

		m_IsSelPopup = FALSE;
		m_hSelMenu = NULL;
		m_IsMenuActive = TRUE;

		// We hook mouse input to process mouse and keyboard input during
		//  the popup menu. Messages are sent to StaticMsgHook.

		// Remove any remaining hook first
		TLSData* pTLSData = GetApp()->GetTlsData();
		pTLSData->pMenuBar = this;
		if (pTLSData->hMsgHook != NULL)
			::UnhookWindowsHookEx(pTLSData->hMsgHook);

		// Hook messages about to be processed by the shortcut menu
		pTLSData->hMsgHook = ::SetWindowsHookEx(WH_MSGFILTER, (HOOKPROC)StaticMsgHook, NULL, ::GetCurrentThreadId());

		// Display the shortcut menu
		BOOL bRightToLeft = FALSE;

#if (WINVER >= 0x0500)
		bRightToLeft = ((GetAncestor()->GetWindowLongPtr(GWL_EXSTYLE)) & WS_EX_LAYOUTRTL);
#endif

		int xPos = bRightToLeft? rc.right : rc.left;
		UINT nID = ::TrackPopupMenuEx(m_hPopupMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
			xPos, rc.bottom, m_hWnd, &tpm);

		// We get here once the TrackPopupMenuEx has ended
		m_IsMenuActive = FALSE;

		// Remove the message hook
		::UnhookWindowsHookEx(pTLSData->hMsgHook);
		pTLSData->hMsgHook = NULL;

		// Process MDI Child system menu
		if (IsMDIChildMaxed())
		{
			if (::GetSystemMenu(*pMaxMDIChild, FALSE) == m_hPopupMenu )
			{
				if (nID)
					pMaxMDIChild->SendMessage(WM_SYSCOMMAND, (WPARAM)nID, 0L);
			}
		}

		// Re-establish Focus
		if (m_IsKeyMode)
			GrabFocus();

		return 0L;
	}

	inline LRESULT CMenuBar::OnSysCommand(WPARAM wParam, LPARAM lParam)
	{
		if (SC_KEYMENU == wParam)
		{
			if (lParam == 0)
			{
				// Alt/F10 key toggled
				GrabFocus();
				m_IsKeyMode = TRUE;
				int nMaxedOffset = (IsMDIChildMaxed()? 1:0);
				SetHotItem(nMaxedOffset);
			}
			else
				// Handle key pressed with Alt held down
				DoAltKey((WORD)lParam);
		}

		return FinalWindowProc(WM_SYSCOMMAND, wParam, lParam);
	}

	inline LRESULT CMenuBar::OnSysKeyDown(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);

		if ((VK_MENU == wParam) || (VK_F10 == wParam))
			return 0L;

		return FinalWindowProc(WM_SYSKEYDOWN, wParam, lParam);
	}

	inline LRESULT CMenuBar::OnSysKeyUp(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);

		if ((VK_MENU == wParam) || (VK_F10 == wParam))
		{
			ExitMenu();
			return 0L;
		}

		return FinalWindowProc(WM_SYSKEYUP, wParam, lParam);
	}

	inline LRESULT CMenuBar::OnTBNDropDown(LPNMTOOLBAR pNMTB)
	{
		UNREFERENCED_PARAMETER(pNMTB);

		// Always use PostMessage for USER_POPUPMENU (not SendMessage)
		PostMessage(UWM_POPUPMENU, 0L, 0L);

		return 0L;
	}

	inline LRESULT CMenuBar::OnTBNHotItemChange(LPNMTBHOTITEM pNMHI)
	{
		// This is the notification that a hot item change is about to occur
		// This is used to bring up a new popup menu when required

		CPoint pt = GetCursorPos();
		if (this == WindowFromPoint(pt))	// MenuBar window must be on top
		{
			DWORD flag = pNMHI->dwFlags;
			if ((flag & HICF_MOUSE) && !(flag & HICF_LEAVING))
			{
				int nButton = HitTest();
				if ((m_IsMenuActive) && (nButton != m_nHotItem))
				{
					SendMessage(TB_PRESSBUTTON, (WPARAM)m_nHotItem, (LPARAM)MAKELONG(FALSE, 0));
					m_nHotItem = nButton;
					SendMessage(WM_CANCELMODE, 0L, 0L);

					//Always use PostMessage for USER_POPUPMENU (not SendMessage)
					PostMessage(UWM_POPUPMENU, 0L, 0L);
				}
				m_nHotItem = nButton;
			}

			// Handle escape from popup menu
			if ((flag & HICF_LEAVING) && m_IsKeyMode)
			{
				m_nHotItem = pNMHI->idOld;
				PostMessage(TB_SETHOTITEM, (WPARAM)m_nHotItem, 0L);
			}
		}

		return 0L;
	}

	inline LRESULT CMenuBar::OnWindowPosChanged(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);
		UNREFERENCED_PARAMETER(lParam);

		InvalidateRect(&m_MDIRect[0], TRUE);
		InvalidateRect(&m_MDIRect[1], TRUE);
		InvalidateRect(&m_MDIRect[2], TRUE);
		{
			CClientDC MenuBarDC(this);
			DrawAllMDIButtons(&MenuBarDC);
		}

		RedrawWindow();
		return FinalWindowProc(WM_WINDOWPOSCHANGED, wParam, lParam);
	}

	inline LRESULT CMenuBar::OnWindowPosChanging(WPARAM wParam, LPARAM lParam)
	{
		// Bypass CToolBar::WndProcDefault for this message
		return FinalWindowProc(WM_WINDOWPOSCHANGING, wParam, lParam);
	}

	inline void CMenuBar::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TBSTYLE_TOOLTIPS | TBSTYLE_LIST | TBSTYLE_FLAT | CCS_NODIVIDER | CCS_NORESIZE;
	}

	inline void CMenuBar::PreRegisterClass(WNDCLASS &wc)
	{
		// Set the Window Class
		wc.lpszClassName =  TOOLBARCLASSNAME;
	}

	inline void CMenuBar::ReleaseFocus()
	{
		if (m_hPrevFocus)
			::SetFocus(m_hPrevFocus);

		m_hPrevFocus = NULL;
		::ReleaseCapture();
	}

	inline void CMenuBar::SetHotItem(int nHot)
	{
		m_nHotItem = nHot;
		SendMessage(TB_SETHOTITEM, (WPARAM)m_nHotItem, 0L);
	}

	inline void CMenuBar::SetMenu(HMENU hMenu)
	{
		assert(::IsWindow(m_hWnd));

		m_hTopMenu = hMenu;
		int nMaxedOffset = (IsMDIChildMaxed()? 1:0);

		// Remove any existing buttons
		while (SendMessage(TB_BUTTONCOUNT,  0L, 0L) > 0)
		{
			if(!SendMessage(TB_DELETEBUTTON, 0L, 0L))
				break;
		}

		// Set the Bitmap size to zero
		SendMessage(TB_SETBITMAPSIZE, 0L, MAKELPARAM(0, 0));

		if (IsMDIChildMaxed())
		{
			// Create an extra button for the MDI child system menu
			// Later we will custom draw the window icon over this button
			TBBUTTON tbb;
			ZeroMemory(&tbb, sizeof(TBBUTTON));
			tbb.fsState = TBSTATE_ENABLED;
			tbb.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE ;
			tbb.iString = (INT_PTR)_T(" ");
			SendMessage(TB_ADDBUTTONS, 1, (WPARAM)&tbb);
			SetButtonText(0, _T("    "));
		}

		for (int i = 0 ; i < ::GetMenuItemCount(hMenu); ++i)
		{
			// Assign the ToolBar Button struct
			TBBUTTON tbb;
			ZeroMemory(&tbb, sizeof(TBBUTTON));
			tbb.idCommand = i  + nMaxedOffset;	// Each button needs a unique ID
			tbb.fsState = TBSTATE_ENABLED;
			tbb.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE | TBSTYLE_DROPDOWN;
			tbb.iString = (INT_PTR)_T(" ");
			SendMessage(TB_ADDBUTTONS, 1, (WPARAM)&tbb);

			// Add the menu title to the string table
			std::vector<TCHAR> vMenuName( MAX_MENU_STRING+1, _T('\0') );
			TCHAR* szMenuName = &vMenuName[0];
			GetMenuString(hMenu, i, szMenuName, MAX_MENU_STRING, MF_BYPOSITION);
			SetButtonText(i  + nMaxedOffset, szMenuName);
		}
	}

	inline LRESULT CALLBACK CMenuBar::StaticMsgHook(int nCode, WPARAM wParam, LPARAM lParam)
	{
		assert(GetApp());
		MSG* pMsg = reinterpret_cast<MSG*>(lParam);
		TLSData* pTLSData = GetApp()->GetTlsData();
		assert(pTLSData);
		CMenuBar* pMenuBar = pTLSData->pMenuBar;

		if (pMenuBar && (MSGF_MENU == nCode))
		{
			// process menu message
			if (pMenuBar->OnMenuInput(pMsg->message, pMsg->wParam, pMsg->lParam))
			{
				return TRUE;
			}
		}

		return CallNextHookEx(pTLSData->hMsgHook, nCode, wParam, lParam);
	}

	inline LRESULT CMenuBar::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CHAR:				return 0L;  // Discard these messages
		case WM_DRAWITEM:			return OnDrawItem(wParam, lParam);
		case WM_EXITMENULOOP:		return OnExitMenuLoop(wParam, lParam);
		case WM_INITMENUPOPUP:		return OnInitMenuPopup(wParam, lParam);
		case WM_KEYDOWN:			return OnKeyDown(wParam, lParam);
		case WM_KILLFOCUS:			return OnKillFocus(wParam, lParam);
		case WM_LBUTTONDBLCLK:		return OnLButtonDown(wParam, lParam);
		case WM_LBUTTONDOWN:		return OnLButtonDown(wParam, lParam);
		case WM_LBUTTONUP:			return OnLButtonUp(wParam, lParam);
		case WM_MEASUREITEM:		return OnMeasureItem(wParam, lParam);
		case WM_MOUSELEAVE:			return OnMouseLeave(wParam, lParam);
		case WM_MOUSEMOVE:			return OnMouseMove(wParam, lParam);
		case WM_SYSKEYDOWN:			return OnSysKeyDown(wParam, lParam);
		case WM_SYSKEYUP:			return OnSysKeyUp(wParam, lParam);
		case WM_WINDOWPOSCHANGED:	return OnWindowPosChanged(wParam, lParam);
		case WM_WINDOWPOSCHANGING:	return OnWindowPosChanging(wParam, lParam);

		// Messages defined by Win32++
		case UWM_POPUPMENU:			return OnPopupMenu();

		} // switch (uMsg)

		return CToolBar::WndProcDefault(uMsg, wParam, lParam);
	} // LRESULT CMenuBar::WndProcDefault(...)

}

#endif 	// _WIN32XX_MENUBAR_H_
