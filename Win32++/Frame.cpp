// Win32++  Version 5.0.2 Beta 
// Modified: 13th January, 2007 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
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
// Frame.cpp
// Definitions for the following classes:
// CFrame, CStatusbar and CToolbar classes
//

#include "Frame.h"
#include <windowsx.h>
#include <richedit.h>
#include "Default_Resource.h"
#include <vector>
#include "MDI.h"


namespace Win32xx
{

	//////////////////////////////////////
	// Definitions for the CStatusbar class
	//
	void CStatusbar::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CCS_BOTTOM | SBARS_SIZEGRIP;
		cs.lpszClass = STATUSCLASSNAME;
	}

	void CStatusbar::SetPaneSizes(std::vector<int> StatusPaneSizes)
	{
		try
		{
			int nPanes = StatusPaneSizes.size();		

			// Create the int array from StatusPaneSizes vector
			int* PaneWidths = new int[nPanes];
			for (int i = 0 ; i < nPanes ; i++)
			{
				PaneWidths[i] = StatusPaneSizes[i];
			}

			// Create the statusbar panes
			if (!::SendMessage(m_hWnd, SB_SETPARTS, nPanes, (LPARAM)PaneWidths))
				throw (CWinException(TEXT("CStatusbar::SetPaneSize failed")));

			delete []PaneWidths;
		}
		
		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CStatusbar::SetPaneSizes"));
		}
	}

	void CStatusbar::SetText(std::vector<LPCTSTR> StatusText )
	{
		try
		{
			int nParts = ::SendMessage(m_hWnd, SB_GETPARTS, 0, 0);
			int nText = StatusText.size();
			
			// int iMin = min(nParts, nText)
			int iMin = (nParts < nText) ? nParts : nText;

			for (int i = 0 ; i < iMin; i++)
			{
				::SendMessage(m_hWnd, SB_SETTEXT, i, (LPARAM)StatusText[i]);
			}
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CStatusbar::SetText"));
		}
	}


	////////////////////////////////////
	// Definitions for the CToolbar class
	//
	CToolbar::CToolbar()
	{
	}

	CToolbar::~CToolbar()
	{
	}

	void CToolbar::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT;
		cs.lpszClass = TOOLBARCLASSNAME;

		// Add extra styles for toolbars inside a rebar
		CFrame* pFrame = GetApp()->GetFrame();
		if (pFrame->IsRebarUsed())
			cs.style |= CCS_NODIVIDER | CCS_NORESIZE;
	}

	void CToolbar::DisableButton(const int iButtonID)
	{
		// An example of iButtonID would be ID_FILE_OPEN
		::SendMessage(m_hWnd, TB_ENABLEBUTTON, (WPARAM)iButtonID, (LPARAM) MAKELONG(FALSE, 0));
	}

	void CToolbar::EnableButton(const int iButtonID)
	{
		::SendMessage(m_hWnd, TB_ENABLEBUTTON, (WPARAM)iButtonID, (LPARAM) MAKELONG(TRUE,0 ));
	}

	///////////////////////////////////
	// Definitions for the CRebar class
	//
	CRebar::CRebar()
	{
	}

	BOOL CRebar::DeleteBand(int nBand)
	{
		return (BOOL)::SendMessage(m_hWnd, RB_DELETEBAND, nBand, 0);
	}

	int CRebar::GetBand(HWND hWnd) const
	// Returns the zero based band number for this window handle
	{
		int nResult = -1;
		if (hWnd == NULL) return nResult;

		for (int nBand = 0; nBand < GetBandCount(); nBand++)
		{
			REBARBANDINFO rbbi = {0};
			rbbi.fMask = RBBIM_CHILD;
			GetBandInfo(nBand, &rbbi);
			if (rbbi.hwndChild == hWnd)
				nResult = nBand;
		}
		return nResult;
	}

	int CRebar::GetBandCount() const
	{
		return (int)::SendMessage(m_hWnd, RB_GETBANDCOUNT, 0, 0);
	}

	BOOL CRebar::GetBandInfo(int nBand, LPREBARBANDINFO prbbi) const
	{
		// REBARBANDINFO describes individual BAND characteristics
		prbbi->cbSize = sizeof(REBARBANDINFO);
		return (BOOL)::SendMessage(m_hWnd, RB_GETBANDINFO, nBand, (LPARAM)prbbi);
	}

	BOOL CRebar::GetBarInfo(LPREBARINFO prbi) const
	{
		// REBARINFO describes overall rebar control characteristics
		prbi->cbSize = sizeof(REBARINFO);
		return (BOOL)::SendMessage(m_hWnd, RB_GETBARINFO, 0, (LPARAM)prbi);
	}

	BOOL CRebar::InsertBand(int nBand, LPREBARBANDINFO prbbi)
	{
		return (BOOL)::SendMessage(m_hWnd, RB_INSERTBAND, nBand, (LPARAM)(LPREBARBANDINFO)prbbi);
	}

	void CRebar::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
                         CCS_NODIVIDER | RBS_VARHEIGHT | RBS_BANDBORDERS;

		cs.lpszClass = REBARCLASSNAME;
	}

	void CRebar::SetBandBitmap(int nBand, HBITMAP hBackground)
	{
		REBARBANDINFO rbbi = {0};
		rbbi.fMask  = RBBIM_STYLE;
		GetBandInfo(nBand, &rbbi);
		rbbi.fMask  |= RBBIM_BACKGROUND;
		rbbi.hbmBack = hBackground;
		SetBandInfo(nBand, &rbbi);
	}

	void CRebar::SetBandColor(int nBand, COLORREF clrFore, COLORREF clrBack)
	{
		// Won't work with XP themes enabled
		// Won't work if a bitmap has been set
		REBARBANDINFO rbbi = {0};
		rbbi.fMask = RBBIM_COLORS;
		rbbi.clrFore = clrFore;
		rbbi.clrBack = clrBack;
		SetBandInfo(nBand, &rbbi);
	}

	BOOL CRebar::SetBandInfo(int nBand, LPREBARBANDINFO prbbi)
	{
		// REBARBANDINFO describes individual BAND characteristics
		prbbi->cbSize = sizeof(REBARBANDINFO);
		return (BOOL)::SendMessage(m_hWnd, RB_SETBANDINFO, nBand, (LPARAM)prbbi);
	}

	BOOL CRebar::SetBarInfo(LPREBARINFO prbi)
	{
		// REBARINFO associates an image list with the rebar
		// A band will also need to set RBBIM_IMAGE
		prbi->cbSize = sizeof(REBARINFO);
		return (BOOL)::SendMessage(m_hWnd, RB_SETBARINFO, 0, (LPARAM)prbi);
	}


	//////////////////////////////////
	// Definitions for the CFrame class
	//
	CMenubar::CMenubar()
	{
		m_bExitAfter	= FALSE;
		m_hTopMenu		= NULL;
		m_nHotItem		= -1;
		m_bSelPopup		= FALSE;
		m_hSelMenu		= NULL;
		m_bMenuActive	= FALSE;
		m_bKeyMode		= FALSE;
		m_nMaxedFlag	= 0;
		m_hPrevFocus	= NULL;
		m_nMDIButton    = 0;

		ZeroMemory(&m_MDIRect, 3*sizeof(RECT));

		m_pTLSData->pMenubar = this;
		Superclass(TOOLBARCLASSNAME, TEXT("Menubar"));
	}

	CMenubar::~CMenubar()
	{
		m_pTLSData->pMenubar = NULL;
	}

	void CMenubar::DoAltKey(WORD KeyCode)
	{
		//Handle key pressed with Alt held down
		UINT ID;
		if (::SendMessage(m_hWnd, TB_MAPACCELERATOR, KeyCode, (LPARAM) &ID))
		{
			GrabFocus();
			m_bKeyMode = TRUE;
			SetHotItem(ID);
			m_bMenuActive = TRUE;
			::PostMessage(m_hWnd, USER_POPUPMENU, 0, 0);
		}
		else
			::MessageBeep(MB_OK);
	}

	void CMenubar::DoPopupMenu()
	{
		if (m_bKeyMode)
			// Simulate a down arrow key press
			::PostMessage(m_hWnd, WM_KEYDOWN, VK_DOWN, 0);

		m_bKeyMode = FALSE;
		m_bExitAfter = FALSE;
		::GetCursorPos(&m_OldMousePos);

		// Retrieve the bounding rectangle for the toolbar button
		RECT rc = {0};
		::SendMessage(m_hWnd, TB_GETITEMRECT, m_nHotItem, (LPARAM) &rc);

		// convert rectangle to desktop coordinates
		::MapWindowPoints(m_hWnd, HWND_DESKTOP, (LPPOINT)&rc, 2);

		// Position popup above toolbar if it won't fit below
		TPMPARAMS tpm;
		tpm.cbSize = sizeof(TPMPARAMS);
		tpm.rcExclude = rc;

		// Load the submenu
		m_hPopupMenu = ::GetSubMenu(m_hTopMenu, m_nHotItem - m_nMaxedFlag);

		// Set the hot button
		::SendMessage(m_hWnd, TB_SETHOTITEM, m_nHotItem, 0);
		::SendMessage(m_hWnd, TB_PRESSBUTTON, m_nHotItem, MAKELONG(TRUE, 0));

		m_bSelPopup = FALSE;
		m_hSelMenu = NULL;
		m_bMenuActive = TRUE;

		// We hook mouse input to process mouse and keyboard input during
		//  the popup menu. Messages are sent to StaticMsgHook.

		// Remove any remaining hook first
		if (m_pTLSData->hMenuHook != NULL)
			::UnhookWindowsHookEx(m_pTLSData->hMenuHook);

		// Hook messages about to be processed by the shortcut menu
		m_pTLSData->hMenuHook = ::SetWindowsHookEx(WH_MSGFILTER, (HOOKPROC)StaticMsgHook, NULL, ::GetCurrentThreadId());;

		// Display the shortcut menu
		::TrackPopupMenuEx(m_hPopupMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
			rc.left, rc.bottom, m_hWnd, &tpm);

		// We get here once the TrackPopupMenuEx has ended
		m_bMenuActive = FALSE;

		// Remove the message hook
		::UnhookWindowsHookEx(m_pTLSData->hMenuHook);
		m_pTLSData->hMenuHook = NULL;

		// Resestablish Focus
		if (m_bKeyMode)
			GrabFocus();
	}

	void CMenubar::DrawMDIButtons(HDC hDC)
	{
		CFrame* pFrame = GetApp()->GetFrame();
		BOOL bMaxed = FALSE;

		if (!pFrame->IsMDIFrame())
			return;

		HWND MDIClient = pFrame->GetView()->GetHwnd();
		::SendMessage(MDIClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMaxed);

		if (bMaxed)
		{
			int cx = GetSystemMetrics(SM_CXSMICON);
			int cy = GetSystemMetrics(SM_CYSMICON);
			RECT rc;
			::GetClientRect(m_hWnd, &rc);

			// Assign values to each element of the RECT array
			for (int i = 0 ; i < 3 ; i++)
				::SetRect(&m_MDIRect[2 - i], rc.right - (i+1)*cx, rc.bottom/2 - cy/2 , rc.right - i*cx, rc.bottom/2 + cy/2);

			// Draw the MDI Min, Restore and Close buttons
			::DrawFrameControl(hDC, &m_MDIRect[0], DFC_CAPTION ,DFCS_CAPTIONMIN);
			::DrawFrameControl(hDC, &m_MDIRect[1], DFC_CAPTION ,DFCS_CAPTIONRESTORE);
			::DrawFrameControl(hDC, &m_MDIRect[2], DFC_CAPTION ,DFCS_CAPTIONCLOSE);
		}
	}

	void CMenubar::ExitMenu()
	{
		ReleaseFocus();
		m_bKeyMode = FALSE;
		m_bMenuActive = FALSE;
		::SendMessage(m_hWnd, TB_PRESSBUTTON, m_nHotItem, (LPARAM) MAKELONG (FALSE, 0));
		SetHotItem(-1);

		POINT pt = {0};
		::GetCursorPos(&pt);
		::ScreenToClient(m_hWnd, &pt);

		// Update mouse mouse position for hot tracking
		::SendMessage(m_hWnd, WM_MOUSEMOVE, 0, MAKELONG(pt.x, pt.y));

	}

	void CMenubar::GrabFocus()
	{
		if (::GetFocus() != m_hWnd)
			m_hPrevFocus = ::SetFocus(m_hWnd);
		::SetCapture(m_hWnd);
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}

	int CMenubar::HitTest()
	{
		POINT pt = {0};
		::GetCursorPos(&pt);
		::ScreenToClient(m_hWnd, &pt);

		//Returns button hit, or a value <0 if no button hit
		return (int) ::SendMessage(m_hWnd, TB_HITTEST, 0, (LPARAM)&pt);
	}

	//LRESULT CMenubar::OnNotify(WPARAM /* wParam */, LPARAM lParam)
	LRESULT CMenubar::OnNotifyReflect(WPARAM /* wParam */, LPARAM lParam)
	{
		switch (((LPNMHDR)lParam)->code)
		{
		case NM_CUSTOMDRAW:
			{
				LRESULT lResult = 0;
				OnCustomDraw((LPNMHDR) lParam, &lResult);
				return (BOOL)lResult;
			}

		case TBN_DROPDOWN:
			// Always use PostMessage for USER_POPUPMENU (not SendMessage)
			::PostMessage(m_hWnd, USER_POPUPMENU, 0, 0);
			break;

		case TBN_HOTITEMCHANGE:
			// This is the notification that a hot item change is about to occur
			// This is used to bring up a new popup menu when required
			{
				DWORD flag = ((LPNMTBHOTITEM)lParam)->dwFlags;
				if ((flag & (HICF_ENTERING | HICF_MOUSE )) && !(flag & HICF_LEAVING))
				{
					int nButton = ((LPNMTBHOTITEM)lParam)->idNew;

					if (nButton >= m_nMaxedFlag)
					{
						if ((m_bMenuActive) && (nButton != m_nHotItem))
						{
							::SendMessage(m_hWnd, TB_PRESSBUTTON, m_nHotItem, MAKELONG(FALSE, 0));
							m_nHotItem = nButton;
							::SendMessage(m_hWnd, WM_CANCELMODE, 0, 0);

							//Always use PostMessage for USER_POPUPMENU (not SendMessage)
							::PostMessage(m_hWnd, USER_POPUPMENU, 0, 0);
						}
						m_nHotItem = nButton;
					}
				}

				if ((flag & HICF_LEAVING) && m_bKeyMode)
				{
					m_nHotItem = ((LPNMTBHOTITEM)lParam)->idOld;
					::PostMessage(m_hWnd, TB_SETHOTITEM, m_nHotItem, 0);
					return -1; // Discard this HotItemChange now
				}

				if(flag == HICF_MOUSE)
				{
					::PostMessage(m_hWnd, TB_SETHOTITEM, m_nHotItem, 0);
					return -1; // Discard this HotItemChange now
				}

				break;
			} //case TBN_HOTITEMCHANGE:

		} // switch(((LPNMHDR)lParam)->code)
		return 0L;
	} // CMenubar::OnNotify(...)

	void CMenubar::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
	{
		// Use custom draw to draw a rectangle over the hot button
		LPNMTBCUSTOMDRAW lpNMCustomDraw = (LPNMTBCUSTOMDRAW)pNMHDR;
		switch (lpNMCustomDraw->nmcd.dwDrawStage)
		{
		// Begin paint cycle
		case CDDS_PREPAINT:
			// Send NM_CUSTOMDRAW item draw, and post-paint notification messages.
			*pResult = CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT ;
			break;

		// An item is about to be drawn
		case CDDS_ITEMPREPAINT:
			{
				HDC hDC = lpNMCustomDraw->nmcd.hdc;
				RECT rcRect = lpNMCustomDraw->nmcd.rc;
				int nState = lpNMCustomDraw->nmcd.uItemState;
				DWORD dwItem = (DWORD)lpNMCustomDraw->nmcd.dwItemSpec;

				// Leave a 1 pixel gap above the drawn rectangle
				rcRect.top = 1;

				if (nState & (CDIS_HOT | CDIS_SELECTED))
				{
					// Draw highlight rectangle
					HBRUSH hbHighlight = ::GetSysColorBrush(COLOR_HIGHLIGHT);
					::FillRect(hDC, &rcRect, hbHighlight);

					TCHAR str[80] = {0};
					int nLength = (int)::SendMessage(m_hWnd, TB_GETBUTTONTEXT, lpNMCustomDraw->nmcd.dwItemSpec, (LPARAM) NULL);
					if ((nLength > 0) && (nLength < 80))
						::SendMessage(m_hWnd, TB_GETBUTTONTEXT, lpNMCustomDraw->nmcd.dwItemSpec, (LPARAM)str);

					// Draw highlight text
					::SelectObject(hDC, (HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0, 0));
					::SetTextColor(hDC, ::GetSysColor(COLOR_HIGHLIGHTTEXT));
					::SetBkMode(hDC, TRANSPARENT);
					::DrawText(hDC, str, lstrlen(str), &rcRect, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

					*pResult = CDRF_SKIPDEFAULT;  // No further drawing
				}
				else if ((nState & CDIS_DISABLED) && (dwItem == 0))
				{
					CFrame* pFrame = GetApp()->GetFrame();
					HWND hMDIClient = pFrame->GetView()->GetHwnd();
					HWND hMDIChild = (HWND)::SendMessage(hMDIClient, WM_MDIGETACTIVE, 0, 0);
					HICON hIcon = (HICON)::SendMessage(hMDIChild, WM_GETICON, ICON_SMALL, 0);
					if (hIcon == NULL)
						hIcon = ::LoadIcon(NULL, IDI_APPLICATION);

					int cx = ::GetSystemMetrics (SM_CXSMICON);
					int cy = ::GetSystemMetrics (SM_CYSMICON);
					int y = 1 + (rcRect.bottom - rcRect.top - cy)/2;
					int x = 0;
					::DrawIconEx(hDC, x, y, hIcon, cx, cy, 0, NULL, DI_NORMAL);
					::SetRect(&m_IconRect, x, y, x + cx, y + cy);

					*pResult = CDRF_SKIPDEFAULT;  // No further drawing
				}
				else
					*pResult = CDRF_DODEFAULT ;   // Do default drawing
			}
			break;

		// Painting cycle has completed
		case CDDS_POSTPAINT:
			// Draw MDI Minimise, Restore and Close buttons
			{
				HDC hDC = lpNMCustomDraw->nmcd.hdc;
				DrawMDIButtons(hDC);
			}
			break;
		}
	}

	void CMenubar::OnKeyDown(WPARAM wParam, LPARAM /*lParam*/)
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			ExitMenu();
			break;

		case VK_SPACE:
			ExitMenu();
			// Bring up the system menu
			::SendMessage(GetAncestor(m_hWnd), WM_SYSCOMMAND, SC_KEYMENU, VK_SPACE);
			break;

		// Handle VK_DOWN,VK_UP and VK_RETURN together
		case VK_DOWN:
		case VK_UP:
		case VK_RETURN:
			// Always use PostMessage for USER_POPUPMENU (not SendMessage)
			::PostMessage(m_hWnd, USER_POPUPMENU, 0, 0);
			break;

		case VK_LEFT:
			// Move left to next topmenu item
			(m_nHotItem > m_nMaxedFlag)? SetHotItem(m_nHotItem -1) : SetHotItem(m_nButtonCount-1+m_nMaxedFlag);
			break;

		case VK_RIGHT:
			// Move right to next topmenu item
			(m_nHotItem < m_nButtonCount -1 + m_nMaxedFlag)? SetHotItem(m_nHotItem +1) : SetHotItem(m_nMaxedFlag);
			break;

		default:
			// Handle Accelerator keys with Alt toggled down
			if (m_bKeyMode)
			{
				UINT ID;
				if (::SendMessage(m_hWnd, TB_MAPACCELERATOR, wParam, (LPARAM) &ID))
				{
					m_nHotItem = ID;
					::PostMessage(m_hWnd, USER_POPUPMENU, 0, 0);
				}
				else
					::MessageBeep(MB_OK);
			}
			break;
		} // switch (wParam)
	}

	void CMenubar::OnLButtonDown(WPARAM /*wParam*/, LPARAM lParam)
	{
		GrabFocus();
		m_nMDIButton = 0;
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		CFrame* pFrame = GetApp()->GetFrame();
		if (pFrame->IsMDIFrame())
		{
			BOOL bMaxed;
			HWND MDIClient = pFrame->GetView()->GetHwnd();
			HWND MDIChild = (HWND)::SendMessage(MDIClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMaxed);

			if (bMaxed)
			{
				HDC hDC = ::GetDC(m_hWnd);

				// Draw the MDI button pressed down
				if (PtInRect(&m_MDIRect[0], pt))
				{
					::DrawFrameControl(hDC, &m_MDIRect[0], DFC_CAPTION, DFCS_CAPTIONMIN | DFCS_PUSHED);
					m_nMDIButton = MDI_MIN;
				}

				if (PtInRect(&m_MDIRect[1], pt))
				{
					::DrawFrameControl(hDC, &m_MDIRect[1], DFC_CAPTION, DFCS_CAPTIONRESTORE | DFCS_PUSHED);
					m_nMDIButton = MDI_RESTORE;
				}

				if (PtInRect(&m_MDIRect[2], pt))
				{
					::DrawFrameControl(hDC, &m_MDIRect[2], DFC_CAPTION, DFCS_CAPTIONCLOSE | DFCS_PUSHED);
					m_nMDIButton = MDI_CLOSE;
				}

				// Bring up the MDI Child window's system menu when the icon is pressed
				if (PtInRect(&m_IconRect, pt))
				{
					// Retrieve the MDI child system menu
					HMENU hChildMenu = ::GetSystemMenu(MDIChild, FALSE);

					// Retrieve the bounding rectangle for the toolbar button
					RECT rc = {0};
					GetWindowRect(m_hWnd, &rc);

					// Position popup above toolbar if it won't fit below
					TPMPARAMS tpm;
					tpm.cbSize = sizeof(TPMPARAMS);
					tpm.rcExclude = rc;

					// We hook mouse input to capture a double mouse button click
					// Its required for processing the default menu entry.
					// Messages are sent to StaticIconHook.

					// Remove any remaining hook first
					if (m_pTLSData->hMenuHook != NULL)
						::UnhookWindowsHookEx(m_pTLSData->hMenuHook);

					// Hook messages about to be processed by the shortcut menu
					m_pTLSData->hMenuHook = ::SetWindowsHookEx(WH_MSGFILTER, (HOOKPROC)StaticIconHook, NULL, ::GetCurrentThreadId());;

					// Display the popup
					UINT nID = ::TrackPopupMenuEx(hChildMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL | TPM_RETURNCMD,
						rc.left, rc.bottom, m_hWnd, &tpm);

					// Remove the message hook
					::UnhookWindowsHookEx(m_pTLSData->hMenuHook);
					m_pTLSData->hMenuHook = NULL;

					if (nID)
						::PostMessage(MDIChild, WM_SYSCOMMAND, nID, 0);
				}
				::ReleaseDC(m_hWnd, hDC);
			}
		}
	}

	void CMenubar::OnLButtonUp(WPARAM /*wParam*/, LPARAM lParam)
	{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		CFrame* pFrame = GetApp()->GetFrame();
		if (pFrame->IsMDIFrame())
		{
			BOOL bMaxed;
			HWND MDIClient = pFrame->GetView()->GetHwnd();
			HWND MDIChild = (HWND)::SendMessage(MDIClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMaxed);

			if (bMaxed)
			{
				POINT pt = {0};
				::GetCursorPos(&pt);
				::ScreenToClient(m_hWnd, &pt);

				// Process the MDI button action when the left mouse button is up
				if (PtInRect(&m_MDIRect[0], pt))
				{
					if (m_nMDIButton == MDI_MIN)
						::ShowWindow(MDIChild, SW_MINIMIZE);
				}

				if (PtInRect(&m_MDIRect[1], pt))
				{
					if (m_nMDIButton == MDI_RESTORE)
						::PostMessage(MDIClient, WM_MDIRESTORE, (WPARAM)MDIChild, 0);
				}

				if (PtInRect(&m_MDIRect[2], pt))
				{
					if (m_nMDIButton == MDI_CLOSE)
						::PostMessage(MDIChild, WM_CLOSE, 0, 0);
				}
			}
		}
		m_nMDIButton = 0;
		ExitMenu();
	}

	void CMenubar::OnMouseMove(WPARAM wParam, LPARAM lParam)
	{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		if (wParam == MK_LBUTTON)  // mouse moved with left mouse button is held down
		{
			CFrame* pFrame = GetApp()->GetFrame();
			if (pFrame->IsMDIFrame())
			{
				BOOL bMaxed;
				HWND MDIClient = pFrame->GetView()->GetHwnd();
				::SendMessage(MDIClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMaxed);

				if (bMaxed)
				{
					HDC hDC = ::GetDC(m_hWnd);
					static BOOL bButtonPushed = TRUE;

					// toggle the MDI button image pressed/unpressed as required
					if (PtInRect(&m_MDIRect[0], pt))
					{
						if (m_nMDIButton == MDI_MIN)
						{
							::DrawFrameControl(hDC, &m_MDIRect[0], DFC_CAPTION, DFCS_CAPTIONMIN | DFCS_PUSHED);
							bButtonPushed = TRUE;
						}
						else
						{
							::DrawFrameControl(hDC, &m_MDIRect[1], DFC_CAPTION, DFCS_CAPTIONRESTORE);
							::DrawFrameControl(hDC, &m_MDIRect[2], DFC_CAPTION, DFCS_CAPTIONCLOSE);
							bButtonPushed = FALSE;
						}
					}

					else if (PtInRect(&m_MDIRect[1], pt))
					{
						if (m_nMDIButton == MDI_RESTORE)
						{
							::DrawFrameControl(hDC, &m_MDIRect[1], DFC_CAPTION, DFCS_CAPTIONRESTORE | DFCS_PUSHED);
							bButtonPushed = TRUE;
						}
						else
						{
							::DrawFrameControl(hDC, &m_MDIRect[0], DFC_CAPTION, DFCS_CAPTIONMIN);
							::DrawFrameControl(hDC, &m_MDIRect[2], DFC_CAPTION, DFCS_CAPTIONCLOSE);
							bButtonPushed = FALSE;
						}
					}

					else if (PtInRect(&m_MDIRect[2], pt))
					{
						if (m_nMDIButton == MDI_CLOSE)
						{
							::DrawFrameControl(hDC, &m_MDIRect[2], DFC_CAPTION, DFCS_CAPTIONCLOSE | DFCS_PUSHED);
							bButtonPushed = TRUE;
						}
						else
						{
							::DrawFrameControl(hDC, &m_MDIRect[0], DFC_CAPTION, DFCS_CAPTIONMIN);
							::DrawFrameControl(hDC, &m_MDIRect[1], DFC_CAPTION, DFCS_CAPTIONRESTORE);
							bButtonPushed = FALSE;
						}
					}

					else if (bButtonPushed)
					{
						::DrawFrameControl(hDC, &m_MDIRect[0], DFC_CAPTION, DFCS_CAPTIONMIN);
						::DrawFrameControl(hDC, &m_MDIRect[1], DFC_CAPTION, DFCS_CAPTIONRESTORE);
						::DrawFrameControl(hDC, &m_MDIRect[2], DFC_CAPTION, DFCS_CAPTIONCLOSE);
						bButtonPushed = FALSE;
					}
				}
			}
		}
	}

	void CMenubar::OnMDISetMenu(WPARAM /*wParam*/, LPARAM lParam)
	{
		// Adds the additional menu items the the "Window" submenu when
		//  MDI child windows are created

		HMENU hMenuWindow = (HMENU)lParam;
		if (!IsMenu(hMenuWindow))
			return;

		// Delete previously appended items
		int nItems = ::GetMenuItemCount(hMenuWindow);
		UINT uLastID = ::GetMenuItemID(hMenuWindow, --nItems);
		if ((uLastID >= IDM_FIRSTCHILD) && (uLastID < IDM_FIRSTCHILD + 10))
		{
			while ((uLastID >= IDM_FIRSTCHILD) && (uLastID < IDM_FIRSTCHILD + 10))
			{
				::DeleteMenu(hMenuWindow, nItems, MF_BYPOSITION);
				uLastID = ::GetMenuItemID(hMenuWindow, --nItems);
			}
			//delete the separator too
			::DeleteMenu(hMenuWindow, nItems, MF_BYPOSITION);
		}

		// Append MDI Child windows
		CMDIFrame* pFrame = (CMDIFrame*)GetApp()->GetFrame();
		HWND hWndMDIClient = pFrame->GetView()->GetHwnd();
		TCHAR szTitle[25];
		TCHAR szString[30];
		int nWindow = 0;

		// Allocate an iterator for our MDIChild vector
		std::vector <CMDIChild*>::iterator v;

		for (v = pFrame->GetMDIChildVect().begin(); v < pFrame->GetMDIChildVect().end(); v++)
		{
			HWND hWndMDIChild = (*v)->GetHwnd();
			if (::IsWindowVisible(hWndMDIChild))
			{
				// Add Separator
				if (nWindow == 0)
					::AppendMenu(hMenuWindow, MF_SEPARATOR, 0, NULL);

				// Add a menu entry for each MDI child (up to 9)
				if (nWindow < 9)
				{
					::GetWindowText(hWndMDIChild, szTitle, 25);
					::wsprintf(szString, TEXT("&%d %s"), nWindow+1, szTitle);
					::AppendMenu(hMenuWindow, MF_STRING, IDM_FIRSTCHILD + nWindow, szString );

					if ((HWND)::SendMessage(hWndMDIClient, WM_MDIGETACTIVE, 0, 0) == hWndMDIChild)
						::CheckMenuItem(hMenuWindow, IDM_FIRSTCHILD+nWindow, MF_CHECKED);

					nWindow++;
				}
				else if (nWindow == 9)
				// For the 10th MDI child, add this menu item and return
				{
					::AppendMenu(hMenuWindow, MF_STRING, IDM_FIRSTCHILD + nWindow, TEXT("&Windows..."));
					return;
				}
			}
		}
	}

	void CMenubar::OnMenuChar(WPARAM wParam, LPARAM /* lParam */)
	{
		DoAltKey(LOWORD(wParam));
	}

	BOOL CMenubar::OnMenuInput(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch(uMsg)
		{
		case WM_KEYDOWN:
			m_bExitAfter = FALSE;
			{
				switch (wParam)
				{
				case VK_ESCAPE:
					// Use default processing if inside a Sub Menu
					if ((m_hSelMenu) &&(m_hSelMenu != m_hPopupMenu))
						return FALSE;

					m_bMenuActive = FALSE;
					m_bKeyMode = TRUE;
					::SendMessage(m_hWnd, WM_CANCELMODE, 0, 0);
					::SendMessage(m_hWnd, TB_PRESSBUTTON, m_nHotItem, MAKELONG(FALSE, 0));
					::SendMessage(m_hWnd, TB_SETHOTITEM, m_nHotItem, 0);
					break;

				case VK_LEFT:
					// Use default processing if inside a Sub Menu
				    if ((m_hSelMenu) &&(m_hSelMenu != m_hPopupMenu))
						return FALSE;

					::SendMessage(m_hWnd, TB_PRESSBUTTON, m_nHotItem, MAKELONG(FALSE, 0));

					// Move left to next topmenu item
					(m_nHotItem > m_nMaxedFlag)? m_nHotItem-- : m_nHotItem = m_nButtonCount-1 + m_nMaxedFlag;
					::SendMessage(m_hWnd, WM_CANCELMODE, 0, 0);

					// Always use PostMessage for USER_POPUPMENU (not SendMessage)
					::PostMessage(m_hWnd, USER_POPUPMENU, 0, 0);
					::PostMessage(m_hWnd, WM_KEYDOWN, VK_DOWN, 0);
					break;

				case VK_RIGHT:
					// Use default processing to open Sub Menu
					if (m_bSelPopup)
						return FALSE;

					::SendMessage(m_hWnd, TB_PRESSBUTTON, m_nHotItem, MAKELONG(FALSE, 0));

					// Move right to next topmenu item
					(m_nHotItem < m_nButtonCount -1 + m_nMaxedFlag)? m_nHotItem++ : m_nHotItem = m_nMaxedFlag;
					::SendMessage(m_hWnd, WM_CANCELMODE, 0, 0);

					// Always use PostMessage for USER_POPUPMENU (not SendMessage)
					::PostMessage(m_hWnd, USER_POPUPMENU, 0, 0);
					::PostMessage(m_hWnd, WM_KEYDOWN, VK_DOWN, 0);
					break;

				case VK_RETURN:
					m_bExitAfter = TRUE;
					break;

				} // switch (wParam)

			} // case WM_KEYDOWN

			return FALSE;

		case WM_CHAR:
			m_bExitAfter = TRUE;
			return FALSE;

		case WM_LBUTTONDOWN:
			{
				m_bExitAfter = TRUE;
				if (HitTest() >= 0)
				{
					// Cancel popup when we hit a button a second time
					::SendMessage(m_hWnd, WM_CANCELMODE, 0, 0);
					return TRUE;
				}
			}
			return FALSE;

		case WM_MENUSELECT:
			{
				// store info about selected item
				m_hSelMenu = (HMENU)lParam;
				m_bSelPopup = ((HIWORD(wParam) & MF_POPUP) != 0);

				// Reflect message back to the frame window
				::SendMessage(GetAncestor(m_hWnd), WM_MENUSELECT, wParam, lParam);
			}
			return TRUE;

		case WM_MOUSEMOVE:
			{
				POINT pt = {0};
				pt.x = GET_X_LPARAM(lParam);
				pt.y = GET_Y_LPARAM(lParam);

				// Skip if mouse hasn't moved
				if ((pt.x == m_OldMousePos.x) && (pt.y == m_OldMousePos.y))
					return FALSE;

				m_OldMousePos.x = pt.x;
				m_OldMousePos.y = pt.y;
				::ScreenToClient(m_hWnd, &pt);

				// Reflect messages back to the Menubar for hot tracking
				::SendMessage(m_hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(pt.x, pt.y));
			}
			break;

		}
		return FALSE;
	}

	void CMenubar::OnSysCommand(WPARAM wParam, LPARAM lParam)
	{
		if (wParam == SC_KEYMENU)
		{
			if (lParam == 0)
			{
				GrabFocus();
				m_bKeyMode = TRUE;
				SetHotItem(m_nMaxedFlag);
			}
			else
				DoAltKey((WORD)lParam);
		}
	}

	void CMenubar::OnWindowPosChanged()
	{
		::InvalidateRect(m_hWnd, &m_MDIRect[0], TRUE);
		::InvalidateRect(m_hWnd, &m_MDIRect[1], TRUE);
		::InvalidateRect(m_hWnd, &m_MDIRect[2], TRUE);
		{
			HDC hDC = ::GetDC(m_hWnd);
			DrawMDIButtons(hDC);
			::ReleaseDC(m_hWnd, hDC);
		}
	}

	void CMenubar::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | CCS_NODIVIDER | CCS_NORESIZE;;
		cs.lpszClass = TEXT("Menubar");
	}

	void CMenubar::ReleaseFocus()
	{
		if (m_hPrevFocus)
			::SetFocus(m_hPrevFocus);

		m_hPrevFocus = NULL;
		::ReleaseCapture();
	}

	void CMenubar::SetHotItem(int nHot)
	{
		m_nHotItem = nHot;
		::SendMessage(m_hWnd, TB_SETHOTITEM, m_nHotItem, 0);
	}

	void CMenubar::SetMenu(HMENU hMenu)
	{
		m_hTopMenu = hMenu;

		try
		{
			CFrame* pFrame = GetApp()->GetFrame();
			HWND MDIClient = pFrame->GetView()->GetHwnd();
			BOOL bMaxed = FALSE;
			::SendMessage(MDIClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMaxed);
			m_nMaxedFlag = bMaxed? 1: 0;
			m_nButtonCount = ::GetMenuItemCount(hMenu);

			// Send the TB_BUTTONSTRUCTSIZE message, which is required for backward compatibility.
			::SendMessage(m_hWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

			// Remove any existing buttons
			while (::SendMessage(m_hWnd, TB_BUTTONCOUNT,  0, 0) > 0)
			{
				if(!::SendMessage(m_hWnd, TB_DELETEBUTTON, 0, 0))
					break;
			}

			// Set the Bitmap size to zero
			::SendMessage(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(0, 0));

			if (m_nMaxedFlag == 1)
			{
				// Create a hidden disabled button for the MDI child system menu
				// Later we will custom draw the window icon over this button
				TBBUTTON tbb = {0};
				tbb.idCommand = 0;
				tbb.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE ;
				tbb.iString = (INT_PTR)"    ";
				LRESULT lr = ::SendMessage(m_hWnd, TB_INSERTBUTTON, (LPARAM)0, (WPARAM)&tbb);
				if (!lr)
					throw CWinException(TEXT("Menubar::SetMenu  TB_INSERTBUTTON failed"));
			}

			for (int i = 0 ; i < m_nButtonCount; i++)
			{
				// Add the menu title to the string table
				TCHAR szMenuName[MAX_MENU_STRING +1];
				int Result = ::GetMenuString(hMenu, i, szMenuName, MAX_MENU_STRING, MF_BYPOSITION);
				if (Result == 0)
					throw CWinException(TEXT("Menubar::SetMenu  GetMenuString failed"));
				int iIndex = (int)::SendMessage(m_hWnd, TB_ADDSTRING, 0,(LPARAM)szMenuName);

				// Assign the Toolbar Button struct
				TBBUTTON tbb = {0};
				tbb.idCommand = i  + m_nMaxedFlag;
				tbb.fsState = TBSTATE_ENABLED;
				tbb.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE | TBSTYLE_DROPDOWN;
				tbb.iString = iIndex;
				LRESULT lr = ::SendMessage(m_hWnd, TB_INSERTBUTTON, (LPARAM)i+1, (WPARAM)&tbb);
				if (!lr)
					throw CWinException(TEXT("Menubar::SetMenu  TB_INSERTBUTTON failed"));
			}
		}

		catch(const CWinException &e)
		{
			e.MessageBox();
		}

		catch(...)
		{
			DebugErrMsg(TEXT("Exception in CMenubar::SetMenu"));
		}
	}

	LRESULT CALLBACK CMenubar::StaticIconHook(int nCode, WPARAM wParam, LPARAM lParam)
	{
		MSG* pMsg = (MSG*)lParam;
		TLSData* pTLSData = (TLSData*)TlsGetValue(GetApp()->GetTlsIndex());
		CMenubar* pMenubar = (CMenubar*)pTLSData->pMenubar;

		if (pMenubar && nCode == MSGF_MENU)
		{
			switch (pMsg->message)
			{
				case WM_LBUTTONDBLCLK:
				{
					CFrame* pFrame = GetApp()->GetFrame();
					HWND MDIClient = pFrame->GetView()->GetHwnd();
					HWND MDIChild = (HWND)::SendMessage(MDIClient, WM_MDIGETACTIVE, 0, 0);
					HMENU hChildMenu = ::GetSystemMenu(MDIChild, FALSE);

					UINT nID = ::GetMenuDefaultItem(hChildMenu, FALSE, 0);
					if (nID)
						::PostMessage(MDIChild, WM_SYSCOMMAND, nID, 0);
				}
				break;
			}
		}

		return CallNextHookEx(pTLSData->hMenuHook, nCode, wParam, lParam);
	}

	LRESULT CALLBACK CMenubar::StaticMsgHook(int nCode, WPARAM wParam, LPARAM lParam)
	{
		MSG* pMsg = (MSG*)lParam;
		TLSData* pTLSData = (TLSData*)TlsGetValue(GetApp()->GetTlsIndex());
		CMenubar* pMenubar = (CMenubar*)pTLSData->pMenubar;

		if (pMenubar && nCode == MSGF_MENU)
		{
			// process menu message
			if (pMenubar->OnMenuInput(pMsg->message, pMsg->wParam, pMsg->lParam))
			{
				return TRUE;
			}
		}

		return CallNextHookEx(pTLSData->hMenuHook, nCode, wParam, lParam);
	}

	LRESULT CMenubar::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_EXITMENULOOP:
			if (m_bExitAfter)
				ExitMenu();
			break;
		case USER_POPUPMENU:
			DoPopupMenu();
			return 0L;
		case WM_KEYDOWN:
			OnKeyDown(wParam, lParam);
			return 0L;
		case WM_CHAR:
			return 0L;  // Discard these messages
		case WM_KILLFOCUS:
			ExitMenu();
			return 0L;
		case WM_LBUTTONDOWN:
			// Do default processing first
			CallPrevWindowProc(hwnd, uMsg, wParam, lParam);

			OnLButtonDown(wParam, lParam);
			return 0L;
		case WM_LBUTTONUP:
			OnLButtonUp(wParam, lParam);
			break;
		case WM_MOUSEMOVE:
			OnMouseMove(wParam, lParam);
			break;
		case WM_MDISETMENU:
			OnMDISetMenu(wParam, lParam);
			return 0L;
		case WM_MENUCHAR:
			// A key press with the Alt key held down
			if (!m_bMenuActive)
				OnMenuChar(wParam, lParam);
			return 0L;
		case WM_SYSCOMMAND:
			// F10 or Alt key press and release
			OnSysCommand(wParam, lParam);
			return 0L;
		case WM_SYSKEYDOWN:
			if ((wParam == VK_MENU) || (wParam == VK_F10))
				return 0L;
			break;
		case WM_SYSKEYUP:
			if ((wParam == VK_MENU) || (wParam == VK_F10))
			{
				ExitMenu();
				return 0L;
			}
			break;

		case WM_WINDOWPOSCHANGED:
			OnWindowPosChanged();
			break;
		} // switch (uMsg)

		return CWnd::WndProc(hwnd, uMsg, wParam, lParam);
	} // LRESULT CMenubar::WndProc(...)


	//////////////////////////////////
	// Definitions for the CFrame class
	//
	CFrame::CFrame() : m_bIsMDIFrame(FALSE), m_bUseMenubar(FALSE), m_bUseRebar(FALSE), m_bSupportRebars(FALSE),
		                  m_pView(NULL), m_ToolbarButtons(0), m_ToolbarData(NULL)
	{
		GetApp()->SetFrame(this);
		INITCOMMONCONTROLSEX InitStruct;
		InitStruct.dwSize=sizeof(INITCOMMONCONTROLSEX);
		InitStruct.dwICC=ICC_WIN95_CLASSES| ICC_COOL_CLASSES;

		// Do either InitCommonControls or InitCommonControlsEx
		LoadCommonControls(InitStruct);

		m_hMenu = ::LoadMenu(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(ID_MAIN));

		// Set up the default Toolbar data. To change this, create your own ToolbarData array
		// and call SetToolbarData in your frame's constructor
		BYTE ToolbarData[][2] =
		{
			{BUTTON,    IDM_FILE_NEW},
			{BUTTON,    IDM_FILE_OPEN},
			{BUTTON,    IDM_FILE_SAVE},
			{SEPARATOR, 0},
			{BUTTON,    IDM_EDIT_CUT},
			{BUTTON,    IDM_EDIT_COPY},
			{BUTTON,    IDM_EDIT_PASTE},
			{SEPARATOR, 0},
			{BUTTON,    IDM_FILE_PRINT},
			{SEPARATOR, 0},
			{BUTTON,    IDM_HELP_ABOUT}
		};

		int nButtons = sizeof(ToolbarData)/(2*sizeof(BYTE));

		SetToolbarData(nButtons, ToolbarData);
	}

	CFrame::~CFrame()
	{
		delete []m_ToolbarData;
		::DestroyMenu(m_hMenu);
	}

	void CFrame::AddMenubarBand(int Menubar_Height /* = MENUBAR_HEIGHT*/)
	{
   		REBARBANDINFO rbbi = {0};

		rbbi.cbSize     = sizeof(REBARBANDINFO);
		rbbi.fMask      = RBBIM_COLORS | RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_CHILD;
		rbbi.cyMinChild = Menubar_Height;
		rbbi.cyMaxChild = Menubar_Height;
		rbbi.fStyle     = RBBS_BREAK | RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS ;// | RBBS_NOGRIPPER;
		rbbi.clrFore    = GetSysColor(COLOR_BTNTEXT);
		rbbi.clrBack    = GetSysColor(COLOR_BTNFACE);
		rbbi.hwndChild  = GetMenubar().GetHwnd();

		GetRebar().InsertBand(-1, &rbbi);
	}

	void CFrame::AddToolbarBand(int Toolbar_Height /*= TOOLBAR_HEIGHT*/)
	{
   		REBARBANDINFO rbbi = {0};

		rbbi.cbSize     = sizeof(REBARBANDINFO);
		rbbi.fMask      = RBBIM_COLORS | RBBIM_CHILDSIZE | RBBIM_STYLE |  RBBIM_CHILD;
		rbbi.cyMinChild = Toolbar_Height;
		rbbi.cyMaxChild = Toolbar_Height;
		rbbi.fStyle     = RBBS_BREAK | RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS;
		rbbi.clrFore    = GetSysColor(COLOR_BTNTEXT);
		rbbi.clrBack    = GetSysColor(COLOR_BTNFACE);
		rbbi.hwndChild  = GetToolbar().GetHwnd();

		GetRebar().InsertBand(-1, &rbbi);
	}

	RECT CFrame::GetClientSize()
	{
		RECT rFrame  = {0};
		RECT rStatus = {0};
		RECT rTop  = {0};
		RECT rClientSize = {0};

		// Get the size of the client area of the frame window
		::GetClientRect(m_hWnd, &rFrame);

		// Get size of status bar window
		if (::IsWindowVisible(GetStatusbar().GetHwnd()))
			::GetWindowRect(GetStatusbar().GetHwnd(), &rStatus);

		// Get size of top rebar or toolbar
		if (m_bSupportRebars && m_bUseRebar)
			::GetWindowRect(GetRebar().GetHwnd(), &rTop);
		else
			if (IsWindowVisible(GetToolbar().GetHwnd()))
				::GetWindowRect(GetToolbar().GetHwnd(), &rTop);

		// Return client size less the rebar and status windows
		int nHeight = rTop.bottom - rTop.top;
		int top = rFrame.top + nHeight;
		int left = rFrame.left;
		int right = rFrame.right;
		int bottom = rFrame.bottom - rFrame.top - (rStatus.bottom - rStatus.top);
		if ((bottom <= top) ||( right <= left))
			top = left = right = bottom = 0;

		::SetRect(&rClientSize, left, top, right, bottom);

		return rClientSize;
	} // RECT CFrame::GetClientSize()

	void CFrame::LoadCommonControls(INITCOMMONCONTROLSEX InitStruct)
	{
		try
		{
			// Load the Common Controls DLL
			HMODULE hComCtl = ::LoadLibraryA("COMCTL32.DLL");
			if (!hComCtl)
				throw CWinException(TEXT("CFrame::LoadCommonControls ... Failed to load COMCTL32.DLL"));

			// Declare a pointer to function
			BOOL (STDAPICALLTYPE* pfnInit)(LPINITCOMMONCONTROLSEX InitStruct);

			// Store the address of the InitCommonControlEx function
			(FARPROC&)pfnInit = ::GetProcAddress(hComCtl, "InitCommonControlsEx");

			if (!pfnInit)
				// Can't call InitCommonControlsEx, so call InitCommonControls instead
				::InitCommonControls();
			else
			{
				// Call InitCommonControlsEx
				if(!((*pfnInit)(&InitStruct)))
					throw CWinException(TEXT("CFrame::LoadCommonControls ... InitCommonControlsEx failed"));
				m_bSupportRebars = TRUE;
				m_bUseRebar     = TRUE;
				m_bUseMenubar    = TRUE;
			}

			::FreeLibrary(hComCtl);
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CFrame::LoadCommonControls"));
		}
	}

	BOOL CFrame::OnCommand(UINT nID)
	{
		switch (nID)
		{
		case IDM_FILE_EXIT:
			::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
			break;
		case IDM_HELP_ABOUT:
			OnHelp();
			break;
		case IDM_VIEW_STATUSBAR:
			OnViewStatusbar();
			break;
		case IDM_VIEW_TOOLBAR:
			OnViewToolbar();
			break;
		} // switch cmd

		return 0;
	} // CFrame::OnCommand(...)

	void CFrame::OnCreate()
	{
		// Set the icon
		SetIconLarge(ID_MAIN);
		SetIconSmall(ID_MAIN);

		if (m_bSupportRebars && m_bUseRebar)
		{
			// Create the rebar
			GetRebar().Create(m_hWnd);

			// Create the menu
			if (m_bUseMenubar)
			{
				GetMenubar().Create(GetRebar().GetHwnd());
				GetMenubar().SetMenu(m_hMenu);
				AddMenubarBand();
			}

			// Create tool bar
			GetToolbar().Create(GetRebar().GetHwnd());
			AddToolbarBand();
		}
		else
			GetToolbar().Create(m_hWnd);

		if (!IsMenubarUsed())
			::SetMenu(m_hWnd, m_hMenu);

		// Create the status bar
		GetStatusbar().Create(m_hWnd);

		// Load the toolbar buttons
		SetButtons(m_ToolbarButtons, m_ToolbarData);

		// Create the view window
		m_pView->Create(m_hWnd);
	}

	void CFrame::OnHelp()
	{
		static BOOL IsHelpOpen = FALSE;
		if (!IsHelpOpen)
		{
			HWND PrevFocus = ::GetFocus();
			IsHelpOpen = TRUE;
			CDialog AboutDialog(IDD_ABOUT_DIALOG, m_hWnd);
			AboutDialog.DoModal();

			// Return the focus back to where it was
			::SetFocus(PrevFocus);
			IsHelpOpen = FALSE;
		}
	}

	void CFrame::OnKeyDown(WPARAM wParam, LPARAM /*lParam*/)
	{
		// Check CAPs lock, NUM lock and SCRL lock keys
		if((wParam == VK_CAPITAL) || (wParam == VK_NUMLOCK) ||(wParam == VK_SCROLL))
			SetStatusText();
	}

	void CFrame::OnMenuSelect(WPARAM wParam, LPARAM lParam)
	{
		// Set the Statusbar text when we hover over a menu
		// Only popup submenus have status strings

		int nID = LOWORD (wParam);
		HMENU hMenu = (HMENU) lParam;
		if ((!(hMenu == ::GetMenu(m_hWnd))) && (nID != 0))
			SetStatusText(LoadString(nID));
		else
			SetStatusText();
	}

	LRESULT CFrame::OnNotify(WPARAM wParam, LPARAM lParam)
	{
		switch (((LPNMHDR)lParam)->code)
		{
		case EN_MSGFILTER:  // Keydown event for RichEdit control
			{
				MSGFILTER* pmsgfilter = (MSGFILTER*) lParam;
				UINT uMsg = pmsgfilter->msg;
				if (uMsg == WM_KEYDOWN)
					OnKeyDown((WORD)pmsgfilter->wParam, (WORD)pmsgfilter->lParam);
			}
			break;

		case LVN_KEYDOWN:	// Keydown event for List control
		case TCN_KEYDOWN:	// Keydown event for Tab control
		case TVN_KEYDOWN:	// Keydown event for Tree control
			{
				LPNMLVKEYDOWN pnkd = (LPNMLVKEYDOWN) lParam;
				WORD wKey = pnkd->wVKey;
				OnKeyDown(wKey, 0);
			}
			break;

		case RBN_HEIGHTCHANGE:
			RecalcLayout();
			::InvalidateRect(m_hWnd, NULL, TRUE);
			break;

		// Quick method for updating status bar text from tool bar buttons (requires IE 4 or higher)
		case TBN_HOTITEMCHANGE:
			if (((LPNMHDR)lParam)->hwndFrom == GetToolbar().GetHwnd())
			{
				LPNMTBHOTITEM pHotItem = (LPNMTBHOTITEM)lParam;

				if (!(pHotItem->dwFlags & HICF_LEAVING))
					SetStatusText(LoadString(pHotItem->idNew));
				else
					SetStatusText(TEXT("Ready"));
			}
			break;

		// Display ToolTip
		case TTN_GETDISPINFO:
			{
				LPNMTTDISPINFO lpDispInfo = (LPNMTTDISPINFO)lParam;
				INT idButton = (INT)lpDispInfo->hdr.idFrom;
				lpDispInfo->lpszText = (LPTSTR)LoadString(idButton);

				// Slower method of updating the Status Bar text, waiting for tooltip
				// (works with IE version 3 and higher)
				if (!IsMenubarUsed())
					SetStatusText(LoadString(idButton));
			}
			break;

		case TTN_POP:
			if (((LPNMHDR)lParam)->hwndFrom != GetToolbar().GetHwnd()) break;
				// Sets the "slower method" status bar text back to default
				SetStatusText(TEXT("Ready"));
			break;

		} // switch LPNMHDR
		return CWnd::OnNotify(wParam, lParam);

	} // CFrame::OnNotify(...)

	void CFrame::OnSetFocus()
	{
		SetStatusText();
	}

	void CFrame::OnSysColorChange()
	{
		// Honor theme color changes
		for (int nBand = 0; nBand <= GetRebar().GetBandCount(); nBand++)
		{
			GetRebar().SetBandColor(nBand, GetSysColor(COLOR_BTNTEXT), GetSysColor(COLOR_BTNFACE));
		}

		//Reposition and redraw everything
		RecalcLayout();
		::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
	}

	void CFrame::OnViewStatusbar()
	{
		if (::IsWindowVisible(GetStatusbar().GetHwnd()))
		{
			if (IsMenubarUsed())
				::CheckMenuItem(GetMenubar().GetMenu(), IDM_VIEW_STATUSBAR, MF_UNCHECKED);
			else
			{
				::CheckMenuItem (m_hMenu, IDM_VIEW_STATUSBAR, MF_UNCHECKED);
				if (::GetMenu(m_hWnd) != m_hMenu)
					::CheckMenuItem (::GetMenu(m_hWnd), IDM_VIEW_STATUSBAR, MF_UNCHECKED);
			}

			::ShowWindow(GetStatusbar().GetHwnd(), SW_HIDE);
		}
		else
		{
			if (IsMenubarUsed())
				::CheckMenuItem(GetMenubar().GetMenu(), IDM_VIEW_STATUSBAR, MF_CHECKED);
			else
			{
				::CheckMenuItem (m_hMenu, IDM_VIEW_STATUSBAR, MF_CHECKED);
				if (::GetMenu(m_hWnd) != m_hMenu)
					::CheckMenuItem (::GetMenu(m_hWnd), IDM_VIEW_STATUSBAR, MF_CHECKED);
			}

			::ShowWindow(GetStatusbar().GetHwnd(), SW_SHOW);
		}

		// Reposition the Windows
		RecalcLayout();
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}

	void CFrame::OnViewToolbar()
	{
		if (::IsWindowVisible(GetToolbar().GetHwnd()))
		{
			if (IsMenubarUsed())
			{
				::CheckMenuItem(GetMenubar().GetMenu(), IDM_VIEW_TOOLBAR, MF_UNCHECKED);
				::SendMessage(GetRebar().GetHwnd(), RB_SHOWBAND, m_Rebar.GetBand(GetToolbar().GetHwnd()), FALSE);
			}
			else
			{
				::CheckMenuItem (m_hMenu, IDM_VIEW_TOOLBAR, MF_UNCHECKED);
				if (::GetMenu(m_hWnd) != m_hMenu)
					::CheckMenuItem (::GetMenu(m_hWnd), IDM_VIEW_TOOLBAR, MF_UNCHECKED);
				if (IsRebarUsed())
					::SendMessage(GetRebar().GetHwnd(), RB_SHOWBAND, m_Rebar.GetBand(GetToolbar().GetHwnd()), FALSE);
				else
					::ShowWindow(GetToolbar().GetHwnd(), SW_HIDE);
			}
		}
		else
		{
			if (IsMenubarUsed())
			{
				::CheckMenuItem(GetMenubar().GetMenu(), IDM_VIEW_TOOLBAR, MF_CHECKED);
				::SendMessage(GetRebar().GetHwnd(), RB_SHOWBAND, m_Rebar.GetBand(GetToolbar().GetHwnd()), TRUE);
			}
			else
			{
				::CheckMenuItem (m_hMenu, IDM_VIEW_TOOLBAR, MF_CHECKED);
				if (::GetMenu(m_hWnd) != m_hMenu)
					::CheckMenuItem (::GetMenu(m_hWnd), IDM_VIEW_TOOLBAR, MF_CHECKED);
				if (IsRebarUsed())
					::SendMessage(GetRebar().GetHwnd(), RB_SHOWBAND, m_Rebar.GetBand(GetToolbar().GetHwnd()), TRUE);
				else
					::ShowWindow(GetToolbar().GetHwnd(), SW_SHOW);
			}
		}

		RecalcLayout();
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}

  	void CFrame::PreCreate(CREATESTRUCT& cs)
	{
		try
		{
			if (m_pView == NULL)
				throw CWinException(TEXT("CFrame::PreCreate ... m_pView is NULL\n\nUse SetView to set the View Window"));

			// Set the Window Class
			cs.lpszClass = TEXT("Win32++ Frame");

			// Set the caption from the string resource
			cs.lpszName = LoadString(ID_MAIN);

			// Set the frame window styles
			cs.style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CFrame::PreCreate"));
		}
	}

	void CFrame::RecalcLayout()
	{
		// Resize the status bar
		::SendMessage(GetStatusbar().GetHwnd(), WM_SIZE, 0, 0);

		// Reposition the text
		SetStatusText();

		// Resize the rebar or toolbar
		if (IsRebarUsed())
			::SendMessage(GetRebar().GetHwnd(), WM_SIZE, 0, 0);
		else
			::SendMessage(GetToolbar().GetHwnd(), TB_AUTOSIZE, 0, 0);

		// Resize the View window
		RECT rClient = GetClientSize();

		if ((rClient.bottom - rClient.top) >= 0)
		{
			int x  = rClient.left;
			int y  = rClient.top;
			int cx = rClient.right - rClient.left;
			int cy = rClient.bottom - rClient.top;

			::SetWindowPos(m_pView->GetHwnd(), NULL, x, y, cx, cy, SWP_SHOWWINDOW );
		}

		::SendMessage(m_hWnd, USER_REARRANGED, 0, 0);
	}

	void CFrame::SetBackground(HBITMAP hBackground)
	{
		for (int nBands = 0; nBands < GetRebar().GetBandCount(); nBands++)
			GetRebar().SetBandBitmap(nBands, hBackground);
	}

	void CFrame::SetButtons(int iNumButtons, BYTE bButtonArray[][2])
	// Function arguements:
	// iNumButtons:-  The number of buttons
	// iButtonArray:- A two dimensional int array which looks like ...
	//  BUTTON,    ID_FILE_OPEN,
	//  BUTTON,    ID_FILE_SAVE,
	//  SEPARATOR, NULL,
	//  BUTTON,    ID_EDIT_COPY,
    //  BUTTON,    ID_EDIT_PASTE,
	{
		try
		{
			Button* Buttons = new Button [iNumButtons];
			for (int i = 0 ; i < iNumButtons ; i++)
			{
				Buttons[i].bStyle = bButtonArray[i][0];
				Buttons[i].Message = bButtonArray[i][1];
				Buttons[i].bState = ENABLED;
			}

			// Send the TB_BUTTONSTRUCTSIZE message, which is required for backward compatibility.
			::SendMessage(GetToolbar().GetHwnd(), TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

			// Remove any existing buttons
			while (::SendMessage(GetToolbar().GetHwnd(), TB_BUTTONCOUNT,  0, 0) > 0)
			{
				if(!::SendMessage(GetToolbar().GetHwnd(), TB_DELETEBUTTON, 0, 0))
					break;
			}

			// Add bitmap that contains button images to the toolbar
			TBADDBITMAP tbab;
			tbab.hInst = GetApp()->GetInstanceHandle();
			tbab.nID   = ID_MAIN;

			if (::SendMessage(GetToolbar().GetHwnd(), TB_ADDBITMAP, iNumButtons, (LPARAM) &tbab) == -1)
				throw (CWinException(TEXT("CFrame::SetButtons  .. TB_ADDBITMAP failed ")));

			// Load the TBBUTTON structure for each button in the toolbar
			TBBUTTON* tbb = new TBBUTTON [iNumButtons];

			int iBitMap = 0;
			for (int j = 0 ; j < iNumButtons; j++)
			{
				::ZeroMemory(&tbb[j], sizeof(TBBUTTON));

				if (Buttons[j].bStyle == SEPARATOR)
				{
					tbb[j].fsStyle = TBSTYLE_SEP;
				}
				else
				{
					tbb[j].iBitmap = iBitMap++;
					tbb[j].idCommand = Buttons[j].Message;
					tbb[j].fsState = Buttons[j].bState;
					tbb[j].fsStyle = Buttons[j].bStyle;
				}
			}
			// Add the buttons to the toolbar
			if (!::SendMessage(GetToolbar().GetHwnd(), TB_ADDBUTTONS, (WPARAM)iNumButtons, (LPARAM)tbb))
				throw (CWinException(TEXT("CFrame::SetButtons  .. TB_ADDBUTTONS failed ")));

			delete []tbb;
		}
		catch(const CWinException &e)
		{
			e.MessageBox();
		}
		catch(...)
		{
			DebugErrMsg(TEXT("Exception in CFrame::SetButtons"));
		}
	} 

	void CFrame::SetStatusText(LPCTSTR szText /*= "Ready"*/)
	{
		HWND hStatus = GetStatusbar().GetHwnd();
	
		if (IsWindow(hStatus))
		{
			// Get the coordinates of the parent window's client area.
			RECT rcClient;
			::GetClientRect(m_hWnd, &rcClient);

			// width = max(300, rcClient.right)
			int width = (300 > rcClient.right) ? 300 : rcClient.right;

			// Fill the vector with the pane sizes
			std::vector<int> StatusPaneSizes;
			StatusPaneSizes.push_back(width - 110);
			StatusPaneSizes.push_back(width - 80);
			StatusPaneSizes.push_back(width - 50);
			StatusPaneSizes.push_back(width - 20);
			
			// Set the pane sizes
			GetStatusbar().SetPaneSizes(StatusPaneSizes);

			// Fill the vector with text
			std::vector<LPCTSTR> StatusText;
			StatusText.push_back(szText);
			StatusText.push_back((::GetKeyState(VK_CAPITAL) & 0x0001)? TEXT("\tCAP") : TEXT(""));
			StatusText.push_back((::GetKeyState(VK_NUMLOCK) & 0x0001)? TEXT("\tNUM") : TEXT(""));
			StatusText.push_back((::GetKeyState(VK_SCROLL)  & 0x0001)? TEXT("\tSCRL"): TEXT(""));

			//Send text to the statusbar	
			GetStatusbar().SetText(StatusText);
		}
	}


	void CFrame::SetToolbarData(int nButtons, BYTE ToolbarData[][2])
	{
		try
		{
			if (nButtons <= 0)
				throw CWinException(TEXT("CFrame::SetToolbarData  Must have at least one toolbar button"));

			delete []m_ToolbarData;
			m_ToolbarButtons = nButtons;

			m_ToolbarData = new BYTE[nButtons][2];
			for ( int i = 0; i < nButtons; i++)
			{
				m_ToolbarData[i][0] = ToolbarData[i][0];
				m_ToolbarData[i][1] = ToolbarData[i][1];
			}
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CFrame::SetToolbarData"));
		}
	}

	void CFrame::SetView(CWnd& View)
	{
		// You call this function to assign the View window object to the frame
		m_pView = &View;
	}

	LRESULT CFrame::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		try
		{
			switch (uMsg)
			{
			case WM_DESTROY:
				::SetMenu(m_hWnd, NULL);
				::PostQuitMessage(0);	// Terminates the application
				return 0L;
			case WM_ERASEBKGND:
				return 0L;
			case WM_HELP:
				OnHelp();
				return 0L;
			case WM_KEYDOWN:
				OnKeyDown(wParam, lParam);
				return 0L;
			case WM_MENUCHAR:
				if (IsMenubarUsed())
				{
					if (LOWORD(wParam)!= VK_SPACE)
						::SendMessage(GetMenubar().GetHwnd(), WM_MENUCHAR, wParam, lParam);
					return -1;
				}
				break;
			case WM_MENUSELECT:
				OnMenuSelect(wParam, lParam);
				return 0L;
			case WM_SETFOCUS:
				OnSetFocus();
				break;
			case WM_SIZE:
				RecalcLayout();
				return 0L;
			case WM_SYSCOLORCHANGE:
				// Changing themes trigger this
				OnSysColorChange();
				return 0L;
			case WM_SYSCOMMAND:
				if ((wParam == SC_KEYMENU) && (lParam != VK_SPACE) && IsMenubarUsed())
				{
					::SendMessage(GetMenubar().GetHwnd(), WM_SYSCOMMAND, wParam, lParam);
					return 0L;
				}
				break;
			} // switch uMsg
			return CWnd::WndProc(hwnd, uMsg, wParam, lParam);
		} // try

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CFrame::WndProc"));
		}

		return 0L;
	} // LRESULT CFrame::WndProc(...)

} // namespace Win32xx

