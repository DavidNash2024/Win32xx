// Win32++  Version 6.7
// Released: 6th November, 2009 by:
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


#ifndef _TOOLBAR_H_
#define _TOOLBAR_H_

#include "wincore.h"
#include "gdi.h"
#include "rebar.h"


namespace Win32xx
{

	struct ThemeToolbar
	{
		BOOL UseThemes;			// TRUE if themes are used
		COLORREF clrHot1;		// Colour 1 for hot button
		COLORREF clrHot2;		// Colour 2 for hot button
		COLORREF clrPressed1;	// Colour 1 for pressed button
		COLORREF clrPressed2;	// Colour 2 for pressed button
		COLORREF clrOutline;	// Colour for border outline
	};


	////////////////////////////////////
	// Declaration of the CToolbar class
	//
	class CToolbar : public CWnd
	{
	public:
		CToolbar();
		virtual ~CToolbar();
		virtual BOOL IsToolbar() const {return TRUE;}

	// Attributes
		void AddToolbarButton(UINT nID, BOOL bEnabled = TRUE);
		virtual void Destroy();
		int  CommandToIndex(int iButtonID) const;
		int  GetButtonCount() const;
		UINT GetButtonState(int iButtonID) const;
		BYTE GetButtonStyle(int iButtonID) const;
		int  GetCommandID(int iIndex) const;
		CRect GetItemRect(int iIndex) const;
		CSize GetMaxSize() const;
		std::vector<UINT>& GetToolbarData() const {return (std::vector <UINT> &)m_vToolbarData;}
		ThemeToolbar& GetToolbarTheme() {return m_Theme;}
		BOOL HasText() const;
		int  HitTest() const;
		void SetBitmap(UINT nID);
		void SetBitmapSize(int cx, int cy) const;
		int  SetButtons(const std::vector<UINT>& vToolbarData) const;
		void SetButtonSize(int cx, int cy) const;
		void SetButtonState(int iButtonID, UINT State) const;
		void SetButtonStyle(int iButtonID, BYTE Style) const;
		void SetButtonText(int iButtonID, LPCTSTR szText);
		void SetButtonWidth(int iButtonID, int nWidth) const;
		void SetCommandID(int iIndex, int iButtonID) const;
		void SetImages(COLORREF crMask, UINT ToolbarID, UINT ToolbarHotID, UINT ToolbarDisabledID);
		void SetToolbarTheme(ThemeToolbar& Theme);

	// Operations
		void AddBitmap(UINT ToolbarID);
		void DisableButton(int iButtonID) const;
		void EnableButton(int iButtonID) const;
		void ReplaceBitmap(UINT NewToolbarID);

	protected:
	// Overridables
		virtual void OnCreate();
		virtual void OnDestroy();
		virtual void OnLButtonDblClk(WPARAM wParam, LPARAM lParam);
		virtual void OnWindowPosChanging(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnCustomDraw(NMHDR* pNMHDR);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		CToolbar(const CToolbar&);				// Disable copy construction
		CToolbar& operator = (const CToolbar&); // Disable assignment operator

		std::vector<UINT> m_vToolbarData;	// vector of resource IDs for toolbar buttons
		std::map<tString, int> m_StringMap;	// a map of strings used in SetButtonText
		UINT m_OldToolbarID;				// Bitmap Resource ID, used in AddBitmap/ReplaceBitmap
		ThemeToolbar m_Theme;				// The theme structure
		BOOL m_bDrawArrowBkgrnd;			// True if a seperate arrow background is to be drawn

	};  // class CToolbar

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{

	////////////////////////////////////
	// Definitions for the CToolbar class
	//
	inline CToolbar::CToolbar() : m_OldToolbarID(0), m_bDrawArrowBkgrnd(FALSE)
	{
		ZeroMemory(&m_Theme, sizeof(ThemeToolbar));
	}

	inline CToolbar::~CToolbar()
	{
	}

	inline void CToolbar::AddBitmap(UINT ToolbarID)
	// Adds one or more images to the list of button images available for a toolbar.

	// Note: AddBitmap supports a maximum colour depth of 8 bits (256 colours)
	//       For more colours, use an ImageList instead
	{
		int iNumButtons = 0;
		std::vector<UINT>::iterator iter;
		for (iter = GetToolbarData().begin(); iter < GetToolbarData().end(); ++iter)
			if ((*iter) != 0) ++iNumButtons;

		TBADDBITMAP tbab = {0};
		tbab.hInst = GetApp()->GetResourceHandle();
		tbab.nID   = ToolbarID;
		if (-1 == SendMessage(TB_ADDBITMAP, iNumButtons, (LPARAM)&tbab) )
			throw CWinException(_T("CToolbar::AddBitmap  TB_ADDBITMAP failed"));

		m_OldToolbarID = ToolbarID;
	}

	inline void CToolbar::AddToolbarButton(UINT nID, BOOL bEnabled /* = TRUE */)
	// Adds Resource IDs to toolbar buttons.
	// A resource ID of 0 is a separator
	{
		m_vToolbarData.push_back(nID);

		if(m_hWnd)
		{
			// TBBUTTON structure for each button in the toolbar
			TBBUTTON tbb = {0};

			std::vector<UINT>::iterator iter;
			int iImages = 0;
			for(iter = m_vToolbarData.begin(); iter < m_vToolbarData.end(); ++iter)
				if (0 != *iter) iImages++;

			ZeroMemory(&tbb, sizeof(TBBUTTON));

			if (0 == nID)
			{
				tbb.fsStyle = TBSTYLE_SEP;
			}
			else
			{
				tbb.dwData  = iImages -1;
				tbb.iBitmap = iImages -1;
				tbb.idCommand = nID;
				tbb.fsState = bEnabled? TBSTATE_ENABLED : 0;
				tbb.fsStyle = TBSTYLE_BUTTON;
			}

			// Add the button to the toolbar
			if (!SendMessage(TB_ADDBUTTONS, 1L, (LPARAM)&tbb))
				throw CWinException(_T("CToolbar::SetButtons  .. TB_ADDBUTTONS failed "));
		}
	}

	inline int CToolbar::CommandToIndex(int iButtonID) const
	// Retrieves the zero-based index for the button associated with the specified command identifier
	{
		// returns -1 on fail
		return (int)SendMessage(TB_COMMANDTOINDEX, (WPARAM)iButtonID, 0L);
	}

	inline void CToolbar::Destroy()
	// Allows CToolbar to be reused after the window is destroyed
	{
		CWnd::Destroy();
		m_StringMap.clear();
	}

	inline void CToolbar::DisableButton(int iButtonID) const
	// Disables the specified button in a toolbar
	{
		// An example of iButtonID would be IDM_FILE_OPEN
		if (!SendMessage(TB_ENABLEBUTTON, (WPARAM)iButtonID, (LPARAM) MAKELONG(FALSE, 0)))
			TRACE(_T("Disable button failed\n"));
	}

	inline void CToolbar::EnableButton(int iButtonID) const
	// Enables the specified button in a toolbar
	{
		if (!SendMessage(TB_ENABLEBUTTON, (WPARAM)iButtonID, (LPARAM) MAKELONG(TRUE,0 )))
			TRACE(_T("Enable button failed\n"));
	}

	inline int CToolbar::GetButtonCount() const
	// Retrieves a count of the buttons currently in the toolbar
	{
		return (int)SendMessage(TB_BUTTONCOUNT, 0L, 0L);
	}

	inline UINT CToolbar::GetButtonState(int iButtonID) const
	// Get the state of an individual button
	//	TBSTATE_CHECKED		The button has the TBSTYLE_CHECK style and is being clicked.
	//	TBSTATE_ELLIPSES	The button's text is cut off and an ellipsis is displayed.
	//	TBSTATE_ENABLED		The button accepts user input. A button that doesn't have this state is grayed.
	//	TBSTATE_HIDDEN		The button is not visible and cannot receive user input.
	//	TBSTATE_INDETERMINATE	The button is grayed.
	//	TBSTATE_MARKED		The button is marked. The interpretation of a marked item is dependent upon the application.
	//	TBSTATE_PRESSED		The button is being clicked.
	//	TBSTATE_WRAP		The button is followed by a line break.
	{
		LRESULT lResult= SendMessage(TB_GETSTATE, (WPARAM) iButtonID, 0L);
		if (-1L == lResult)
			throw CWinException(_T("CToolbar::GetButtonState failed"));

		return (UINT) lResult;
	}

	inline BYTE CToolbar::GetButtonStyle(int iButtonID) const
	//	Get the the style of the toolbar control. The following button styles are supported:
	//	TBSTYLE_BUTTON		Standard pushbutton (default)
	//	TBSTYLE_SEP			Separator
	//	TBSTYLE_CHECK		Auto check-box button
	//	TBSTYLE_GROUP		Marks the start of a group of buttons
	//	TBSTYLE_CHECKGROUP	Marks the start of a group of check-box buttons
	//	TBSTYLE_DROPDOWN	Creates a drop-down list button
	//	TBSTYLE_AUTOSIZE	The button's width will be calculated based on the text of the button, not on the size of the image
	//	TBSTYLE_NOPREFIX	The button text will not have an accelerator prefix associated with it
	{
		int iIndex = CommandToIndex(iButtonID);
		if (-1 == iIndex)
			throw CWinException(_T("CToolbar::GetButtonStyle failed to get command ID"));

		TBBUTTON tbb = {0};

		LRESULT lResult = SendMessage(TB_GETBUTTON, iIndex, (LPARAM) &tbb);
		if (-1L == lResult)
			throw CWinException(_T("CToolbar::GetButtonStyle failed"));

		return tbb.fsStyle;
	}

	inline int CToolbar::GetCommandID(int iIndex) const
	// Retrieves information about the specified button in a toolbar
	{
		TBBUTTON tbb = {0};
		SendMessage(TB_GETBUTTON, iIndex, (WPARAM) &tbb);

		// returns zero if failed
		return tbb.idCommand;
	}

	inline CRect CToolbar::GetItemRect(int iIndex) const
	// Retrieves the bounding rectangle of a button in a toolbar
	{
		CRect rc;
		int iCount = (int)SendMessage(TB_BUTTONCOUNT, 0L, 0L);

		if (iCount >= iIndex)
			SendMessage(TB_GETITEMRECT, (WPARAM)iIndex, (LPARAM)&rc);

		return rc;
	}

	inline CSize CToolbar::GetMaxSize() const
	// Retrieves the total size of all of the visible buttons and separators in the toolbar
	{
		CSize sz;
		SendMessage(TB_GETMAXSIZE, 0L, (LPARAM)&sz);

		// This fixes a Windows bug calculating the size when TBSTYLE_DROPDOWN is used.
		int xMaxSize = 0;
		for (int i= 0 ; i < GetButtonCount(); ++i)
		{
			xMaxSize += GetItemRect(i).Width();
		}

		sz.cx = xMaxSize;
		return sz;
	}

	inline BOOL CToolbar::HasText() const
	{
		BOOL bReturn = FALSE;

		for (int i = 0 ; i < GetButtonCount(); ++i)
		{
			if (SendMessage(TB_GETBUTTONTEXT, GetCommandID(i), 0L) != -1)
				bReturn = TRUE;
		}

		// return TRUE if any button has text
		return bReturn;
	}

	inline int CToolbar::HitTest() const
	// Determines where a point lies in a toolbar control.

	// We do our own hit test since TB_HITTEST is a bit buggy,
	// and also doesn't work at all on earliest versions of Win95
	{
		CPoint pt;
		::GetCursorPos(&pt);
		::ScreenToClient(m_hWnd, &pt);

		int nButtons = (int)SendMessage(TB_BUTTONCOUNT, 0L, 0L);
		int iButton = -1;

		for (int i = 0 ; i < nButtons; ++i)
		{
			CRect r = GetItemRect(i);
			if (::PtInRect(&r, pt))
				iButton = i;
		}

		return iButton;
	}

	inline void CToolbar::OnCreate()
	{
		// We must send this message before sending the TB_ADDBITMAP or TB_ADDBUTTONS message
		SendMessage(TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0L);

		// allows buttons to have a separate dropdown arrow
		// Note: TBN_DROPDOWN notification is sent by a toolbar control when the user clicks a dropdown button
		SendMessage(TB_SETEXTENDEDSTYLE, 0L, TBSTYLE_EX_DRAWDDARROWS);

		// Add extra styles for toolbars inside a rebar
		CWnd* pWnd = FromHandle(GetParent());
		if (pWnd && pWnd->IsRebar())
		{
			DWORD style = (DWORD)GetWindowLongPtr(GWL_STYLE);
			style |= CCS_NODIVIDER | CCS_NORESIZE;
			SetWindowLongPtr(GWL_STYLE, style);
		} 

		SetButtons(m_vToolbarData);
		
		// Set rows of text to zero
		SendMessage(TB_SETMAXTEXTROWS, 0L, 0L);	
	}

	inline LRESULT CToolbar::OnCustomDraw(NMHDR* pNMHDR)
	// With CustomDraw we manually control the drawing of each toolbar button
	{
		LPNMTBCUSTOMDRAW lpNMCustomDraw = (LPNMTBCUSTOMDRAW)pNMHDR;

		switch (lpNMCustomDraw->nmcd.dwDrawStage)
		{
		// Begin paint cycle
		case CDDS_PREPAINT:
			// Send NM_CUSTOMDRAW item draw, and post-paint notification messages.
			return CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT ;

		// An item is about to be drawn
		case CDDS_ITEMPREPAINT:
			{
				CDC DrawDC = lpNMCustomDraw->nmcd.hdc;
				CRect rcRect = lpNMCustomDraw->nmcd.rc;
				int nState = lpNMCustomDraw->nmcd.uItemState;
				DWORD dwItem = (DWORD)lpNMCustomDraw->nmcd.dwItemSpec;
				DWORD dwTBStyle = (DWORD)SendMessage(TB_GETSTYLE, 0L, 0L);
				int nStyle = GetButtonStyle(dwItem);

				int nButton = (int)SendMessage(TB_COMMANDTOINDEX, (WPARAM) dwItem, 0L);
				TBBUTTON tbb = {0};
				SendMessage(TB_GETBUTTON, nButton, (LPARAM)&tbb);
				int iImage = (int)tbb.dwData;

				// Calculate text size
				TCHAR szText[80] = _T("");
				CSize TextSize;
				if (HasText())	// Does any button have text?
				{
					DrawDC.AttachFont((HFONT)SendMessage(WM_GETFONT, 0L, 0L));
					if (SendMessage(TB_GETBUTTONTEXT, dwItem, (LPARAM)&szText)> 0)
					{
						TextSize = DrawDC.GetTextExtentPoint32(szText, lstrlen(szText));
					}
					// Detach the font so it doesn't get destroyed
					DrawDC.DetachFont();
				}

				// Draw outline rectangle
				if (nState & (CDIS_HOT | CDIS_SELECTED))
				{
					DrawDC.CreatePen(PS_SOLID, 1, m_Theme.clrOutline);
					DrawDC.MoveTo(rcRect.left, rcRect.top);
					DrawDC.LineTo(rcRect.left, rcRect.bottom-1);
					DrawDC.LineTo(rcRect.right-1, rcRect.bottom-1);
					DrawDC.LineTo(rcRect.right-1, rcRect.top);
					DrawDC.LineTo(rcRect.left, rcRect.top);
				}

				// Draw filled gradient background
				::InflateRect(&rcRect, -1, -1);
				if ((nState & CDIS_SELECTED) || (GetButtonState(dwItem) & TBSTATE_PRESSED))
				{
					DrawDC.GradientFill(m_Theme.clrPressed1, m_Theme.clrPressed2, rcRect, FALSE);
				}
				else if (nState & CDIS_HOT)
				{
					DrawDC.GradientFill(m_Theme.clrHot1, m_Theme.clrHot2, rcRect, FALSE);
				}

				// Get the appropriate image list depending on the button state
				HIMAGELIST himlToolbar;
				if (nState & CDIS_DISABLED)
				{
					himlToolbar = (HIMAGELIST)SendMessage(TB_GETDISABLEDIMAGELIST, 0L, 0L);
				}
				else if (nState & (CDIS_HOT | CDIS_SELECTED))
				{
					himlToolbar = (HIMAGELIST)SendMessage(TB_GETHOTIMAGELIST, 0L, 0L);
					if (0 == himlToolbar)
						himlToolbar = (HIMAGELIST)SendMessage(TB_GETIMAGELIST, 0L, 0L);
				}
				else
				{
					himlToolbar = (HIMAGELIST)SendMessage(TB_GETIMAGELIST, 0L, 0L);
				}

				BOOL IsWin95 = (1400 == (GetWinVersion()) || (2400 == GetWinVersion()));

				// Calculate image position
				int cxImage = 0;
				int cyImage = 0;
				ImageList_GetIconSize(himlToolbar, &cxImage, &cyImage);

				int yImage = (rcRect.bottom - rcRect.top - cyImage - TextSize.cy +2)/2;
				int xImage = (rcRect.right + rcRect.left - cxImage)/2 + ((nState & CDIS_SELECTED)? 1:0);
				if (dwTBStyle & TBSTYLE_LIST)
				{
					xImage = rcRect.left + (IsXPThemed()?2:4) + ((nState & CDIS_SELECTED)? 1:0);
					yImage = (rcRect.bottom -rcRect.top - cyImage +2)/2 + ((nState & CDIS_SELECTED)? 1:0);
				}

				// Handle the TBSTYLE_DROPDOWN and BTNS_WHOLEDROPDOWN styles
				if ((nStyle & TBSTYLE_DROPDOWN) || ((nStyle & 0x0080) && (!IsWin95)))
				{
					// Calculate the dropdown arrow position
					int xAPos = (nStyle & TBSTYLE_DROPDOWN)? rcRect.right -6 : (rcRect.right + rcRect.left + cxImage + 4)/2;
					int yAPos = (nStyle & TBSTYLE_DROPDOWN)? (rcRect.bottom - rcRect.top +1)/2 : (cyImage)/2;
					if (dwTBStyle & TBSTYLE_LIST)
					{
						xAPos = (nStyle & TBSTYLE_DROPDOWN)?rcRect.right -6:rcRect.right -5;
						yAPos =	(rcRect.bottom - rcRect.top +1)/2 + ((nStyle & TBSTYLE_DROPDOWN)?0:1);
					}

					xImage -= (nStyle & TBSTYLE_DROPDOWN)?((dwTBStyle & TBSTYLE_LIST)? (IsXPThemed()?-4:0):6):((dwTBStyle & TBSTYLE_LIST)? 0:4);

					// Draw separate background for dropdown arrow
					if ((m_bDrawArrowBkgrnd) && (nState & CDIS_HOT))
					{
						CRect rcArrowBkgnd = rcRect;
						rcArrowBkgnd.left = rcArrowBkgnd.right - 13;
						DrawDC.GradientFill(m_Theme.clrPressed1, m_Theme.clrPressed2, rcArrowBkgnd, FALSE);
					}

					m_bDrawArrowBkgrnd = FALSE;

					// Manually draw the dropdown arrow
					DrawDC.CreatePen(PS_SOLID, 1, RGB(0,0,0));
					for (int i = 2; i >= 0; --i)
					{
						DrawDC.MoveTo(xAPos -i-1, yAPos - i+1);
						DrawDC.LineTo(xAPos +i,   yAPos - i+1);
					}

					// Draw line between icon and dropdown arrow
					if ((nStyle & TBSTYLE_DROPDOWN) && ((nState & CDIS_SELECTED) || nState & CDIS_HOT))
					{
						DrawDC.CreatePen(PS_SOLID, 1, m_Theme.clrOutline);
						DrawDC.MoveTo(rcRect.right - 13, rcRect.top);
						DrawDC.LineTo(rcRect.right - 13, rcRect.bottom);
					}
				}

				// Draw the button image
				if (xImage > 0)
				{
					ImageList_Draw(himlToolbar, iImage, DrawDC, xImage, yImage, ILD_TRANSPARENT);
				}

				//Draw Text
				if (lstrlen(szText) > 0)
				{
					int iWidth = rcRect.right - rcRect.left - ((nStyle & TBSTYLE_DROPDOWN)?13:0);
					CRect rcText(0, 0, MIN(TextSize.cx, iWidth), TextSize.cy);

					int xOffset = (rcRect.right + rcRect.left - rcText.right + rcText.left - ((nStyle & TBSTYLE_DROPDOWN)? 11 : 1))/2;
					int yOffset = yImage + cyImage +1;

					if (dwTBStyle & TBSTYLE_LIST)
					{
						xOffset = rcRect.left + cxImage + ((nStyle & TBSTYLE_DROPDOWN)?(IsXPThemed()?10:6): 6) + ((nState & CDIS_SELECTED)? 1:0);
						yOffset = (2+rcRect.bottom - rcRect.top - rcText.bottom + rcText.top)/2 + ((nState & CDIS_SELECTED)? 1:0);
						rcText.right = MIN(rcText.right,  rcRect.right - xOffset);
					}

					OffsetRect(&rcText, xOffset, yOffset);

					int iMode = DrawDC.SetBkMode(TRANSPARENT);
					DrawDC.AttachFont((HFONT)SendMessage(WM_GETFONT, 0L, 0L));

					if (nState & (CDIS_DISABLED))
					{
						// Draw text twice for embossed look
						::OffsetRect(&rcText, 1, 1);
						DrawDC.SetTextColor(RGB(255,255,255));
						DrawDC.DrawText(szText, lstrlen(szText), rcText, DT_LEFT);
						::OffsetRect(&rcText, -1, -1);
						DrawDC.SetTextColor(GetSysColor(COLOR_GRAYTEXT));
						DrawDC.DrawText(szText, lstrlen(szText), rcText, DT_LEFT);
					}
					else
					{
						DrawDC.SetTextColor(GetSysColor(COLOR_BTNTEXT));
						DrawDC.DrawText(szText, lstrlen(szText), rcText, DT_LEFT | DT_END_ELLIPSIS);
					}
					DrawDC.SetBkMode(iMode);
					// Detach the font so it doesn't get destroyed
					DrawDC.DetachFont();
				}
				// Detach the DC so it doesn't get destroyed
				DrawDC.DetachDC();
			}
			return CDRF_SKIPDEFAULT;  // No further drawing
		}
		return 0L;
	}

	inline void CToolbar::OnDestroy()
	{
		HIMAGELIST himlToolbar    = (HIMAGELIST)SendMessage(TB_GETIMAGELIST,    0L, 0L);
		HIMAGELIST himlToolbarHot = (HIMAGELIST)SendMessage(TB_GETHOTIMAGELIST, 0L, 0L);
		HIMAGELIST himlToolbarDis = (HIMAGELIST)SendMessage(TB_GETDISABLEDIMAGELIST, 0L, 0L);
		ImageList_Destroy(himlToolbar);
		ImageList_Destroy(himlToolbarHot);
		ImageList_Destroy(himlToolbarDis);
	}

	inline void CToolbar::OnLButtonDblClk(WPARAM /*wParam*/, LPARAM lParam)
	// Doubleclicks on drop down buttons behave strangely because the popup
	//  menu eats the LeftButtonUp messages, so we put them back.
	{
		int iButton = HitTest();
		if (iButton >= 0)
		{
			DWORD nStyle = GetButtonStyle(GetCommandID(iButton));
			if (((nStyle & 0x0080) && (GetWinVersion() != 1400) && (GetWinVersion() != 2400)))
			{
				// DoubleClick on BTNS_WHOLEDROPDOWN button
				::mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			}

			if (nStyle & TBSTYLE_DROPDOWN)
			{
				CRect rcButton = GetItemRect(iButton);

				int xPos = GET_X_LPARAM(lParam);
				if (xPos >= rcButton.right -13)
				{
					// DoubleClick on dropdown part of TBSTYLE_DROPDOWN button
					::mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
				}
			}
		}
	}

	inline LRESULT CToolbar::OnNotifyReflect(WPARAM /* wParam */, LPARAM lParam)
	// Notifications sent to the parent window are reflected back here
	{
		switch (((LPNMHDR)lParam)->code)
		{
			case NM_CUSTOMDRAW:
			{
				if (m_Theme.UseThemes)
					return OnCustomDraw((LPNMHDR) lParam);
			}
			break;

			case TBN_DROPDOWN:
			{
				int iItem = ((LPNMTOOLBAR) lParam)->iItem;

				// a boolean expression
				m_bDrawArrowBkgrnd = (GetButtonStyle(iItem) & TBSTYLE_DROPDOWN);
			}
			break;
		}
		return 0L;
	}

	inline void CToolbar::OnWindowPosChanging(WPARAM /*wParam*/, LPARAM lParam)
	{
		// Adjust size for toolbars inside a rebar
		CWnd* pWnd = FromHandle(GetParent());
		if (pWnd && pWnd->IsRebar())
		{
			CRebar* pRebar = (CRebar*)pWnd;
			if (pRebar && (pRebar->GetRebarTheme().ShortBands))
			{
				LPWINDOWPOS pWinPos = (LPWINDOWPOS)lParam;
				pWinPos->cx = GetMaxSize().cx+2;
			}
		}
	}

	inline void CToolbar::PreCreate(CREATESTRUCT &cs)
	{
		// Sets the CREATESTRUCT parameters prior to window creation
		cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT;
		cs.lpszClass = TOOLBARCLASSNAME;
	}

	inline void CToolbar::ReplaceBitmap(UINT NewToolbarID)
	// Replaces an existing bitmap with a new bitmap.

	// Note: ReplaceBitmap supports a maximum colour depth of 8 bits (256 colours)
	//       For more colours, use an ImageList instead
	{
		int iNumButtons = 0;
		std::vector<UINT>::iterator iter;
		for (iter = GetToolbarData().begin(); iter < GetToolbarData().end(); ++iter)
			if ((*iter) != 0) ++iNumButtons;

		TBREPLACEBITMAP tbrb = {0};
		tbrb.hInstNew = GetApp()->GetResourceHandle();
		tbrb.hInstOld = GetApp()->GetResourceHandle();
		tbrb.nIDNew = NewToolbarID;
		tbrb.nIDOld = m_OldToolbarID;
		tbrb.nButtons  = iNumButtons;
		if (0 == SendMessage(TB_REPLACEBITMAP, iNumButtons, (LPARAM)&tbrb) )
			throw CWinException(_T("CToolbar::ReplaceBitmap  TB_REPLACEBITMAP failed"));

		m_OldToolbarID = NewToolbarID;
	}

	inline void CToolbar::SetBitmap(UINT nID)
	{
		// Set the button images
		HBITMAP hbm = LoadBitmap(MAKEINTRESOURCE(nID));
		if (!hbm)
			throw CWinException(_T("CToolbar::SetBitmap ... LoadBitmap failed "));

		BITMAP bm = {0};

		if (!::GetObject(hbm, sizeof(BITMAP), &bm))
			throw CWinException(_T("CToolbar::SetBitmap ... GetObject failed "));

		int iNumButtons = 0;
		std::vector<UINT>::iterator iter;
		for (iter = GetToolbarData().begin(); iter < GetToolbarData().end(); ++iter)
			if ((*iter) != 0) ++iNumButtons;
		
		int iImageWidth  = bm.bmWidth / iNumButtons;
		int iImageHeight = bm.bmHeight;

		// Set the bitmap size first
		SetBitmapSize(iImageWidth, iImageHeight);

		if (m_OldToolbarID)
			ReplaceBitmap(nID);
		else
			AddBitmap(nID);
	}

	inline void CToolbar::SetBitmapSize(int cx, int cy) const
	// Sets the size of the bitmapped images to be added to a toolbar.

	// Needs to be used when the image size is not the default 16 x 15
	// Call this function before using AddBitmap or ReplaceBitmap
	{
		if (!SendMessage(TB_SETBITMAPSIZE, 0L, MAKELONG(cx, cy)))
			TRACE(_T("CToolbar::SetBitmapSize  failed\n"));
	}

	inline int CToolbar::SetButtons(const std::vector<UINT>& vToolbarData) const
	// Assigns a resource ID to each toolbar button
	{
		int iImages = 0;
		size_t iNumButtons = vToolbarData.size();

		// Remove any existing buttons
		while (SendMessage(TB_BUTTONCOUNT,  0L, 0L) > 0)
		{
			if(!SendMessage(TB_DELETEBUTTON, 0L, 0L))
				break;
		}

		if (iNumButtons > 0)
		{
			// TBBUTTON structure for each button in the toolbar
			TBBUTTON tbb = {0};

			for (size_t j = 0 ; j < iNumButtons; ++j)
			{
				ZeroMemory(&tbb, sizeof(TBBUTTON));

				if (0 == vToolbarData[j])
				{
					tbb.fsStyle = TBSTYLE_SEP;
				}
				else
				{
					tbb.dwData  = iImages;
					tbb.iBitmap = iImages++;
					tbb.idCommand = vToolbarData[j];
					tbb.fsState = TBSTATE_ENABLED;
					tbb.fsStyle = TBSTYLE_BUTTON;
				}

				// Add the button to the toolbar
				if (!SendMessage(TB_ADDBUTTONS, 1L, (LPARAM)&tbb))
					throw CWinException(_T("CToolbar::SetButtons  .. TB_ADDBUTTONS failed "));
			}
		}

		return iImages;
	}

	inline void CToolbar::SetButtonSize(int cx, int cy) const
	// Sets the size of the buttons to be added to a toolbar
	// The size can be set only before adding any buttons to the toolbar
	{

		if (!SendMessage(TB_SETBUTTONSIZE, 0L, MAKELONG(cx, cy)))
			TRACE(_T("CToolbar::SetButtonSize failed\n"));
	}

	inline void CToolbar::SetButtonState(int iButtonID, UINT State) const
	{
	// Set the state of an individual button
	//	TBSTATE_CHECKED		The button has the TBSTYLE_CHECK style and is being clicked.
	//	TBSTATE_ELLIPSES	The button's text is cut off and an ellipsis is displayed.
	//	TBSTATE_ENABLED		The button accepts user input. A button that doesn't have this state is grayed.
	//	TBSTATE_HIDDEN		The button is not visible and cannot receive user input.
	//	TBSTATE_INDETERMINATE	The button is grayed.
	//	TBSTATE_MARKED		The button is marked. The interpretation of a marked item is dependent upon the application.
	//	TBSTATE_PRESSED		The button is being clicked.
	//	TBSTATE_WRAP		The button is followed by a line break.

		if (!SendMessage(TB_SETSTATE, (WPARAM) iButtonID, (LPARAM)MAKELONG (State, 0)))
			TRACE(_T("CToolbar::SetButtonState failed\n"));
 	}

	inline void CToolbar::SetButtonStyle(int iButtonID, BYTE Style) const
	//	The the style of the toolbar control. The following button styles are supported:
	//	TBSTYLE_BUTTON		Standard pushbutton (default)
	//	TBSTYLE_SEP			Separator
	//	TBSTYLE_CHECK		Auto check-box button
	//	TBSTYLE_GROUP		Marks the start of a group of buttons
	//	TBSTYLE_CHECKGROUP	Marks the start of a group of check-box buttons
	//	TBSTYLE_DROPDOWN	Creates a drop-down list button
	//	TBSTYLE_AUTOSIZE	The button's width will be calculated based on the text of the button, not on the size of the image
	//	TBSTYLE_NOPREFIX	The button text will not have an accelerator prefix associated with it
	{
		TBBUTTONINFO tbbi = {0};
		tbbi.cbSize = sizeof(TBBUTTONINFO);
		tbbi.dwMask = TBIF_STYLE;
		tbbi.fsStyle = Style;

		if (!SendMessage(TB_SETBUTTONINFO, iButtonID, (LPARAM) &tbbi))
			throw CWinException(_T("CToolbar::SetButtonStyle  failed"));

		// Note:  TB_SETBUTTONINFO requires comctl32.dll version 4.71 or later
		//        i.e. Win95 with IE4 / NT with IE4   or later
	}

	inline void CToolbar::SetButtonText(int iButtonID, LPCTSTR szText)
	// This rather convoluted approach to setting toolbar button text supports
	// all versions of Windows, including Win95 with COMCTL32.DLL version 4.0
	{
		int iIndex = CommandToIndex(iButtonID);
		if (-1 == iIndex)
			throw CWinException(_T("CToolbar::SetButtonText  failed to get Command ID"));

		tString sString = szText;
		std::map<tString, int>::iterator m;
		int iString;

		// Check to see if the string is already added
		m = m_StringMap.find(sString);
		if (m_StringMap.end() == m)
		{
			if (0 == m_StringMap.size())
			{
				// Place a blank string first in the string table, in case some
				// buttons don't have text
				TCHAR szString[3] = _T(" ");
				szString[2] = _T('\0');		// Double-null terminate
				SendMessage(TB_ADDSTRING, 0L, (LPARAM)szString);
			}

			// No index for this string exists, so create it now
			TCHAR szBuf[80] = _T("");
			lstrcpyn(szBuf, szText, 79);
			szBuf[lstrlen(szBuf)+1] = _T('\0');		// Double-null terminate

			iString = (int)SendMessage(TB_ADDSTRING, 0L, (LPARAM)szBuf);
			if (-1 == iString )
				throw CWinException(_T("CToolbar::SetButtonText  TB_ADDSTRING failed"));

			// Save the string its index in our map
			m_StringMap.insert(std::make_pair(sString, iString));
		}
		else
		{
			// String found, use the index from our map
			iString = m->second;
		}

		TBBUTTON tbb = {0};
		if (!SendMessage(TB_GETBUTTON, iIndex, (LPARAM)&tbb))
			throw CWinException(_T("CToolbar::SetButtonText  TB_GETBUTTON failed"));

		tbb.iString = iString;

		// Turn off Toolbar drawing
		SendMessage(WM_SETREDRAW, FALSE, 0L);

		if (!SendMessage(TB_DELETEBUTTON, iIndex, 0L))
			throw CWinException(_T("CToolbar::SetButtonText  TB_DELETEBUTTON failed"));

		if (!SendMessage(TB_INSERTBUTTON, iIndex, (LPARAM)&tbb))
			throw CWinException(_T("CToolbar::SetButtonText  TB_INSERTBUTTON failed"));

		// Ensure the button now includes some text rows
		if (0 == SendMessage(TB_GETTEXTROWS, 0L, 0L))
			SendMessage(TB_SETMAXTEXTROWS, 1L, 0L);

		// Turn on Toolbar drawing
		SendMessage(WM_SETREDRAW, TRUE, 0L);

		// Redraw button
		CRect r = GetItemRect(iIndex);
		InvalidateRect(&r, TRUE);
	}

	inline void CToolbar::SetButtonWidth(int iButtonID, int nWidth) const
	{
		// The set button width can adjust the width of the button after it is created.
		// This is useful when replacing a button with a ComboBox or other control.

		TBBUTTONINFO tbbi = {0};
		tbbi.cbSize = sizeof(TBBUTTONINFO);
		tbbi.dwMask = TBIF_SIZE;
		tbbi.cx = (WORD)nWidth;
		SendMessage(TB_SETBUTTONINFO, (WPARAM)iButtonID, (LPARAM)&tbbi);
		
		// Send a changed message to the parent (used by the rebar)
		SIZE MaxSize = GetMaxSize();
		::SendMessage(GetParent(), UWM_TOOLBAR_RESIZE, (WPARAM)m_hWnd, (LPARAM)&MaxSize);

		// Note:  TB_SETBUTTONINFO requires comctl32.dll version 4.71 or later
		//        i.e. Win95 with IE4 / NT with IE4   or later
	}

	inline void CToolbar::SetCommandID(int iIndex, int iButtonID) const
	// Sets the command identifier of a toolbar button
	{
		if (!SendMessage(TB_SETCMDID, iIndex, iButtonID))
			throw CWinException(_T("CToolbar::SetCommandID failed"));
	}

	inline void CToolbar::SetImages(COLORREF crMask, UINT ToolbarID, UINT ToolbarHotID, UINT ToolbarDisabledID)
	// Either sets the imagelist or adds/replaces bitmap depending on ComCtl32.dll version
	// Assumes the width of the button image = bitmap_size / buttons
	// Assumes buttons have been already been added via AdddToolbarButton
	// This colour mask is often grey RGB(192,192,192) or magenta (255,0,255);
	{
		int iNumButtons = 0;
		std::vector<UINT>::iterator iter;
		for (iter = GetToolbarData().begin(); iter < GetToolbarData().end(); ++iter)
			if ((*iter) != 0) ++iNumButtons;

		if (iNumButtons > 0)
		{
			// Set the button images
			HBITMAP hbm = LoadBitmap(MAKEINTRESOURCE(ToolbarID));
			if (!hbm)
				throw CWinException(_T("CToolbar::SetImages ... LoadBitmap failed "));

			BITMAP bm = {0};

			if (!::GetObject(hbm, sizeof(BITMAP), &bm))
				throw CWinException(_T("CToolbar::SetImages ... GetObject failed "));

			int iImageWidth  = bm.bmWidth / iNumButtons;
			int iImageHeight = bm.bmHeight;

			// Toolbar ImageLists require Comctl32.dll version 4.7 or later
			if (400 == GetComCtlVersion())
			{
				// We are using COMCTL32.DLL version 4.0, so we can't use an imagelist.
				// Instead we simply set the bitmap.
				SetBitmap(ToolbarID);
				return;
			}

			HIMAGELIST himlToolbar    = (HIMAGELIST)SendMessage(TB_GETIMAGELIST,    0L, 0L);
			HIMAGELIST himlToolbarHot = (HIMAGELIST)SendMessage(TB_GETHOTIMAGELIST, 0L, 0L);
			HIMAGELIST himlToolbarDis = (HIMAGELIST)SendMessage(TB_GETDISABLEDIMAGELIST, 0L, 0L);
			ImageList_Destroy(himlToolbar);
			ImageList_Destroy(himlToolbarHot);
			ImageList_Destroy(himlToolbarDis);

			himlToolbar = ImageList_Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, iNumButtons, 0);
			if (!himlToolbar)
				throw CWinException(_T("CToolbar::SetImages ... Create himlToolbar failed "));

			ImageList_AddMasked(himlToolbar, hbm, crMask);
			if(-1L == SendMessage(TB_SETIMAGELIST, 0L, (LPARAM)himlToolbar) )
				throw CWinException(_T("CToolbar::SetImages ... TB_SETIMAGELIST failed "));

			::DeleteObject(hbm);
			hbm = NULL;

			if (ToolbarHotID)
			{
				hbm = LoadBitmap(MAKEINTRESOURCE(ToolbarHotID));
				if (!hbm)
					throw CWinException(_T("CToolbar::SetImages ... LoadBitmap failed "));

				himlToolbarHot = ImageList_Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, iNumButtons, 0);
				if (!himlToolbarHot)
					throw CWinException(_T("CToolbar::SetImages ... Create himlToolbarHot failed "));

				ImageList_AddMasked(himlToolbarHot, hbm, crMask);

				if(-1L == SendMessage(TB_SETHOTIMAGELIST, 0L, (LPARAM)himlToolbarHot) )
					throw CWinException(_T("CToolbar::SetImages ... TB_SETHOTIMAGELIST failed "));

				::DeleteObject(hbm);
				hbm = NULL;
			}

			if (ToolbarDisabledID)
			{
				hbm = LoadBitmap(MAKEINTRESOURCE(ToolbarDisabledID));
				if (!hbm)
					throw CWinException(_T("CToolbar::SetImages ... LoadBitmap failed "));

				himlToolbarDis = ImageList_Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, iNumButtons, 0);
				if (!himlToolbarDis)
					throw CWinException(_T("CToolbar::SetImages ... Create himlToolbarDis failed "));

				ImageList_AddMasked(himlToolbarDis, hbm, crMask);
				if(-1L == SendMessage(TB_SETDISABLEDIMAGELIST, 0L, (LPARAM)himlToolbarDis) )
					throw CWinException(_T("CToolbar::SetImages ... TB_SETDISABLEDIMAGELIST failed "));
			}
			else
			{
				himlToolbarDis = CreateDisabledImageList(himlToolbar);
				if(-1L == SendMessage(TB_SETDISABLEDIMAGELIST, 0L, (LPARAM)himlToolbarDis) )
					throw CWinException(_T("CToolbar::SetImages ... TB_SETDISABLEDIMAGELIST failed "));
			}

			// Inform the parent of the change (rebar needs this)
			SIZE MaxSize = GetMaxSize();
			::SendMessage(GetParent(), UWM_TOOLBAR_RESIZE, (WPARAM)m_hWnd, (LPARAM)&MaxSize);

			::DeleteObject(hbm);
		}
	}

	inline void CToolbar::SetToolbarTheme(ThemeToolbar& Theme)
	{
		m_Theme.UseThemes   = Theme.UseThemes;
		m_Theme.clrHot1     = Theme.clrHot1;
		m_Theme.clrHot2     = Theme.clrHot2;
		m_Theme.clrPressed1 = Theme.clrPressed1;
		m_Theme.clrPressed2 = Theme.clrPressed2;
		m_Theme.clrOutline  = Theme.clrOutline;

		if (IsWindow())
			Invalidate();
	}

	inline LRESULT CToolbar::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_DESTROY:
			OnDestroy();
			break;
		case WM_LBUTTONDBLCLK:
			OnLButtonDblClk(wParam, lParam);
			return 0;
		case WM_WINDOWPOSCHANGING:
			OnWindowPosChanging(wParam, lParam);
			break;
		}

		// pass unhandled messages on for default processing
		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	}

} // namespace Win32xx

#endif // #ifndef _TOOLBAR_H_
