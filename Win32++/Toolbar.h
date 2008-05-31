// Win32++  Version 6.1
// Released: 3rd June, 2008 by:
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


#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "WinCore.h"


namespace Win32xx
{

	struct ThemeToolbar
	{
		BOOL UseThemes;			// TRUE if themes are used
		COLORREF clrHot1;		// Color 1 for hot button
		COLORREF clrHot2;		// Color 2 for hot button
		COLORREF clrPressed1;	// Color 1 for pressed button
		COLORREF clrPressed2;	// Color 2 for pressed button
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

	// Attributes
		int  CommandToIndex(int iButtonID) const;
		int  GetButtonCount() const;
		UINT GetButtonState(int iButtonID) const;
		BYTE GetButtonStyle(int iButtonID) const;
		int  GetCommandID(int iIndex) const;
		CRect GetItemRect(int iIndex) const;
		CSize GetMaxSize() const;
		ThemeToolbar& GetToolbarTheme() {return m_Theme;}
		BOOL HasText() const;
		int  HitTest() const;
		void SetBitmap(int iNumButtons, UINT nID);
		void SetBitmapSize(int cx, int cy) const;
		int  SetButtons(const std::vector<UINT>& ToolbarData) const;
		void SetButtonSize(int cx, int cy) const;
		void SetButtonState(int iButtonID, UINT State) const;
		void SetButtonStyle(int iButtonID, BYTE Style) const;
		void SetButtonText(int iButtonID, LPCTSTR szText);
		void SetCommandID(int iIndex, int iButtonID) const;
		void SetToolbarTheme(ThemeToolbar& Theme);

	// Operations
		void AddBitmap(int iNumButtons, UINT ToolbarID);
		void DisableButton(int iButtonID) const;
		void EnableButton(int iButtonID) const;
		void ReplaceBitmap(int iNumButtons, UINT NewToolbarID);

	protected:
	// Overridables
		virtual void OnCreate();
		virtual void OnDestroy();
		virtual LRESULT OnCustomDraw(NMHDR* pNMHDR);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		std::map<tString, int> m_StringMap;	// a map of strings used in SetButtonText
		UINT m_OldToolbarID;				// Bitmap Resource ID, used in AddBitmap/ReplaceBitmap
		ThemeToolbar m_Theme;				// The theme structure
		BOOL m_bDrawArrowBkgrnd;			// True if a seperate arrow background is to be drawn

	};  // class CToolbar

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

	inline int CToolbar::CommandToIndex(int iButtonID) const
	// Retrieves the zero-based index for the button associated with the specified command identifier
	{
		// returns -1 on fail
		return (int)::SendMessage(m_hWnd, TB_COMMANDTOINDEX, (WPARAM)iButtonID, 0);
	}

	inline void CToolbar::AddBitmap(int iNumButtons, UINT ToolbarID)
	// Adds one or more images to the list of button images available for a toolbar.

	// Note: AddBitmap supports a maximum colour depth of 8 bits (256 colours)
	//       For more colours, use an ImageList instead
	{

		TBADDBITMAP tbab = {0};
		tbab.hInst = GetApp()->GetResourceHandle();
		tbab.nID   = ToolbarID;
		if (-1 == ::SendMessage(m_hWnd, TB_ADDBITMAP, iNumButtons, (LPARAM)&tbab) )
			throw CWinException(_T("CToolbar::AddBitmap  TB_ADDBITMAP failed"));

		m_OldToolbarID = ToolbarID;
	}

	inline void CToolbar::ReplaceBitmap(int iNumButtons, UINT NewToolbarID)
	// Replaces an existing bitmap with a new bitmap.

	// Note: ReplaceBitmap supports a maximum colour depth of 8 bits (256 colours)
	//       For more colours, use an ImageList instead
	{
		TBREPLACEBITMAP tbrb = {0};
		tbrb.hInstNew = GetApp()->GetResourceHandle();
		tbrb.hInstOld = GetApp()->GetResourceHandle();
		tbrb.nIDNew = NewToolbarID;
		tbrb.nIDOld = m_OldToolbarID;
		tbrb.nButtons  = iNumButtons;
		if (0 == ::SendMessage(m_hWnd, TB_REPLACEBITMAP, iNumButtons, (LPARAM)&tbrb) )
			throw CWinException(_T("CToolbar::ReplaceBitmap  TB_REPLACEBITMAP failed"));

		m_OldToolbarID = NewToolbarID;
	}

	inline void CToolbar::DisableButton(int iButtonID) const
	// Disables the specified button in a toolbar
	{
		// An example of iButtonID would be IDM_FILE_OPEN
		if (!::SendMessage(m_hWnd, TB_ENABLEBUTTON, (WPARAM)iButtonID, (LPARAM) MAKELONG(FALSE, 0)))
			TRACE(_T("Disable button failed\n"));
	}

	inline void CToolbar::EnableButton(int iButtonID) const
	// Enables the specified button in a toolbar
	{
		if (!::SendMessage(m_hWnd, TB_ENABLEBUTTON, (WPARAM)iButtonID, (LPARAM) MAKELONG(TRUE,0 )))
			TRACE(_T("Enable button failed\n"));
	}

	inline int CToolbar::GetButtonCount() const
	// Retrieves a count of the buttons currently in the toolbar
	{
		return (int)::SendMessage(m_hWnd, TB_BUTTONCOUNT, 0, 0);
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
		LRESULT lResult= ::SendMessage(m_hWnd, TB_GETSTATE, (WPARAM) iButtonID, 0);
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

		LRESULT lResult = ::SendMessage(m_hWnd, TB_GETBUTTON, iIndex, (LPARAM) &tbb);
		if (-1L == lResult)
			throw CWinException(_T("CToolbar::GetButtonStyle failed"));

		return tbb.fsStyle;
	}

	inline int CToolbar::GetCommandID(int iIndex) const
	// Retrieves information about the specified button in a toolbar
	{
		TBBUTTON tbb = {0};
		::SendMessage(m_hWnd, TB_GETBUTTON, iIndex, (WPARAM) &tbb);

		// returns zero if failed
		return tbb.idCommand;
	}

	inline CRect CToolbar::GetItemRect(int iIndex) const
	// Retrieves the bounding rectangle of a button in a toolbar
	{
		CRect rc;
		int iCount = (int)::SendMessage(m_hWnd, TB_BUTTONCOUNT, 0, 0);

		if (iCount >= iIndex)
			::SendMessage(m_hWnd, TB_GETITEMRECT, (WPARAM)iIndex, (LPARAM)&rc);

		return rc;
	}

	inline CSize CToolbar::GetMaxSize() const
	// Retrieves the total size of all of the visible buttons and separators in the toolbar
	{
		CSize sz;
		::SendMessage(m_hWnd, TB_GETMAXSIZE, 0, (LPARAM)&sz);

		// This fixes a Windows bug calculating the size when TBSTYLE_DROPDOWN is used.
		int xMaxSize = 0;
		for (int i= 0 ; i < GetButtonCount(); ++i)
		{
			xMaxSize += GetItemRect(i).right - GetItemRect(i).left;
		}

		sz.cx = xMaxSize;
		return sz;
	}

	inline BOOL CToolbar::HasText() const
	{
		BOOL bReturn = FALSE;

		for (int i = 0 ; i < GetButtonCount(); ++i)
		{
			if (::SendMessage(m_hWnd, TB_GETBUTTONTEXT, GetCommandID(i), 0) != -1)
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

		int nButtons = (int)::SendMessage(m_hWnd, TB_BUTTONCOUNT, 0, 0);
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
		::SendMessage(m_hWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

		// allows buttons to have a separate dropdown arrow
		// Note: TBN_DROPDOWN notification is sent by a toolbar control when the user clicks a dropdown button
		::SendMessage(m_hWnd, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_DRAWDDARROWS);

		// Add extra styles for toolbars inside a rebar
		TCHAR ClassName[32];
		::GetClassName(m_hWndParent, ClassName, 32);
		if (0 == lstrcmp(ClassName, REBARCLASSNAME))
		{
			DWORD style = (DWORD)::GetWindowLongPtr(m_hWnd, GWL_STYLE);
			style |= CCS_NODIVIDER | CCS_NORESIZE;
			::SetWindowLongPtr(m_hWnd, GWL_STYLE, style);
		}
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
				DWORD dwTBStyle = (DWORD)::SendMessage(m_hWnd, TB_GETSTYLE, 0, 0);
				int nStyle = GetButtonStyle(dwItem);

				int nButton = (int)::SendMessage(m_hWnd, TB_COMMANDTOINDEX, (WPARAM) dwItem, 0);
				TBBUTTON tbb = {0};
				::SendMessage(m_hWnd, TB_GETBUTTON, nButton, (LPARAM)&tbb);
				int iImage = (int)tbb.dwData;

				// Calculate text size
				TCHAR szText[80] = _T("");
				CSize TextSize;
				if (HasText())	// Does any button have text?
				{
					DrawDC.AttachFont((HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0, 0));
					if (::SendMessage(m_hWnd, TB_GETBUTTONTEXT, dwItem, (LPARAM)&szText)> 0)
					{
						::GetTextExtentPoint32(DrawDC, szText, lstrlen(szText), &TextSize);
					}
					// Detach the font so it doesn't get destroyed
					DrawDC.DetachFont();
				}

				// Draw outline rectangle
				if (nState & (CDIS_HOT | CDIS_SELECTED))
				{
					DrawDC.CreatePen(PS_SOLID, 1, m_Theme.clrOutline);
					::MoveToEx(DrawDC, rcRect.left, rcRect.top, NULL);
					::LineTo(DrawDC, rcRect.left, rcRect.bottom-1);
					::LineTo(DrawDC, rcRect.right-1, rcRect.bottom-1);
					::LineTo(DrawDC, rcRect.right-1, rcRect.top);
					::LineTo(DrawDC, rcRect.left, rcRect.top);
				}

				// Draw filled gradient background
				::InflateRect(&rcRect, -1, -1);
				if ((nState & CDIS_SELECTED) || (GetButtonState(dwItem) & TBSTATE_PRESSED))
				{
					GradientFill(DrawDC, m_Theme.clrPressed1, m_Theme.clrPressed2, &rcRect, FALSE);
				}
				else if (nState & CDIS_HOT)
				{
					GradientFill(DrawDC, m_Theme.clrHot1, m_Theme.clrHot2, &rcRect, FALSE);
				}

				// Get the appropriate image list depending on the button state
				HIMAGELIST himlToolbar;
				if (nState & CDIS_DISABLED)
				{
					himlToolbar = (HIMAGELIST)::SendMessage(m_hWnd, TB_GETDISABLEDIMAGELIST, 0, 0);
				}
				else if (nState & (CDIS_HOT | CDIS_SELECTED))
				{
					himlToolbar = (HIMAGELIST)::SendMessage(m_hWnd, TB_GETHOTIMAGELIST, 0, 0);
					if (0 == himlToolbar)
						himlToolbar = (HIMAGELIST)::SendMessage(m_hWnd, TB_GETIMAGELIST, 0, 0);
				}
				else
				{
					himlToolbar = (HIMAGELIST)::SendMessage(m_hWnd, TB_GETIMAGELIST, 0, 0);
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
						CRect rcArrowBkgnd;
						::CopyRect(&rcArrowBkgnd, &rcRect);
						rcArrowBkgnd.left = rcArrowBkgnd.right - 13;
						GradientFill(DrawDC, m_Theme.clrPressed1, m_Theme.clrPressed2, &rcArrowBkgnd, FALSE);
					}

					m_bDrawArrowBkgrnd = FALSE;

					// Manually draw the dropdown arrow
					DrawDC.CreatePen(PS_SOLID, 1, RGB(0,0,0));
					for (int i = 2; i >= 0; --i)
					{
						::MoveToEx(DrawDC, xAPos -i-1, yAPos - i+1, NULL);
						::LineTo  (DrawDC, xAPos +i,   yAPos - i+1);
					}

					// Draw line between icon and dropdown arrow
					if ((nStyle & TBSTYLE_DROPDOWN) && ((nState & CDIS_SELECTED) || nState & CDIS_HOT))
					{
						DrawDC.CreatePen(PS_SOLID, 1, m_Theme.clrOutline);
						::MoveToEx(DrawDC, rcRect.right - 13, rcRect.top, NULL);
						::LineTo(DrawDC, rcRect.right - 13, rcRect.bottom);
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
					CRect rcText(0, 0, min(TextSize.cx, iWidth), TextSize.cy);

					int xOffset = (rcRect.right + rcRect.left - rcText.right + rcText.left - ((nStyle & TBSTYLE_DROPDOWN)? 11 : 1))/2;
					int yOffset = yImage + cyImage +1;

					if (dwTBStyle & TBSTYLE_LIST)
					{
						xOffset = rcRect.left + cxImage + ((nStyle & TBSTYLE_DROPDOWN)?(IsXPThemed()?10:6): 6) + ((nState & CDIS_SELECTED)? 1:0);
						yOffset = (2+rcRect.bottom - rcRect.top - rcText.bottom + rcText.top)/2 + ((nState & CDIS_SELECTED)? 1:0);
						rcText.right = min(rcText.right,  rcRect.right - xOffset);
					}

					OffsetRect(&rcText, xOffset, yOffset);

					int iMode = ::SetBkMode(DrawDC, TRANSPARENT);
					DrawDC.AttachFont((HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0, 0));

					if (nState & (CDIS_DISABLED))
					{
						// Draw text twice for embossed look
						::OffsetRect(&rcText, 1, 1);
						::SetTextColor(DrawDC, RGB(255,255,255));
						::DrawTextEx(DrawDC, szText, lstrlen(szText), &rcText, DT_LEFT, NULL);
						::OffsetRect(&rcText, -1, -1);
						::SetTextColor(DrawDC, GetSysColor(COLOR_GRAYTEXT));
						::DrawTextEx(DrawDC, szText, lstrlen(szText), &rcText, DT_LEFT, NULL);
					}
					else
					{
						::SetTextColor(DrawDC, GetSysColor(COLOR_BTNTEXT));
						::DrawTextEx(DrawDC, szText, lstrlen(szText), &rcText, DT_LEFT | DT_END_ELLIPSIS, NULL);
					}
					::SetBkMode(DrawDC, iMode);
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
		HIMAGELIST himlToolbar    = (HIMAGELIST)::SendMessage(m_hWnd, TB_GETIMAGELIST,    0, 0);
		HIMAGELIST himlToolbarHot = (HIMAGELIST)::SendMessage(m_hWnd, TB_GETHOTIMAGELIST, 0, 0);
		HIMAGELIST himlToolbarDis = (HIMAGELIST)::SendMessage(m_hWnd, TB_GETDISABLEDIMAGELIST, 0, 0);
		ImageList_Destroy(himlToolbar);
		ImageList_Destroy(himlToolbarHot);
		ImageList_Destroy(himlToolbarDis);
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

	inline void CToolbar::PreCreate(CREATESTRUCT &cs)
	{
		// Sets the CREATESTRUCT parameters prior to window creation
		cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT;
		cs.lpszClass = TOOLBARCLASSNAME;
	}

	inline void CToolbar::SetBitmap(int iNumButtons, UINT nID)
	{
		// Set the button images
		HBITMAP hbm = LoadBitmap(MAKEINTRESOURCE(nID));
		if (!hbm)
			throw CWinException(_T("CToolbar::SetBitmap ... LoadBitmap failed "));

		BITMAP bm = {0};

		if (!::GetObject(hbm, sizeof(BITMAP), &bm))
			throw CWinException(_T("CToolbar::SetBitmap ... GetObject failed "));

		int iImageWidth  = bm.bmWidth / iNumButtons;
		int iImageHeight = bm.bmHeight;

		// Set the bitmap size first
		SetBitmapSize(iImageWidth, iImageHeight);

		if (m_OldToolbarID)
			ReplaceBitmap(iNumButtons, nID);
		else
			AddBitmap(iNumButtons, nID);
	}

	inline void CToolbar::SetBitmapSize(int cx, int cy) const
	// Sets the size of the bitmapped images to be added to a toolbar.

	// Needs to be used when the image size is not the default 16 x 15
	// Call this function before using AddBitmap or ReplaceBitmap
	{
		if (!::SendMessage(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELONG(cx, cy)))
			TRACE(_T("CToolbar::SetBitmapSize  failed\n"));
	}

	inline int CToolbar::SetButtons(const std::vector<UINT>& ToolbarData) const
	// Assigns a resource ID to each toolbar button
	{
		int iImages = 0;
		int iNumButtons = (int)ToolbarData.size();

		// Remove any existing buttons
		while (::SendMessage(m_hWnd, TB_BUTTONCOUNT,  0, 0) > 0)
		{
			if(!::SendMessage(m_hWnd, TB_DELETEBUTTON, 0, 0))
				break;
		}

		if (iNumButtons > 0)
		{
			// TBBUTTON structure for each button in the toolbar
			TBBUTTON tbb = {0};

			for (int j = 0 ; j < iNumButtons; ++j)
			{
				ZeroMemory(&tbb, sizeof(TBBUTTON));

				if (0 == ToolbarData[j])
				{
					tbb.fsStyle = TBSTYLE_SEP;
				}
				else
				{
					tbb.dwData  = iImages;
					tbb.iBitmap = iImages++;
					tbb.idCommand = ToolbarData[j];
					tbb.fsState = TBSTATE_ENABLED;
					tbb.fsStyle = TBSTYLE_BUTTON;
				}

				// Add the button to the toolbar
				if (!::SendMessage(m_hWnd, TB_ADDBUTTONS, 1, (LPARAM)&tbb))
					throw CWinException(_T("CToolbar::SetButtons  .. TB_ADDBUTTONS failed "));
			}

			// Set rows of text to zero
			::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, 0, 0);
		}
		else
			TRACE(_T("No Resource IDs for Toolbar\n"));

		return iImages;
	}

	inline void CToolbar::SetButtonSize(int cx, int cy) const
	// Sets the size of the buttons to be added to a toolbar
	// The size can be set only before adding any buttons to the toolbar
	{

		if (!::SendMessage(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELONG(cx, cy)))
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

		if (!::SendMessage(m_hWnd, TB_SETSTATE, (WPARAM) iButtonID, (LPARAM)MAKELONG (State, 0)))
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

		if (!::SendMessage(m_hWnd, TB_SETBUTTONINFO, iButtonID, (LPARAM) &tbbi))
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
				::SendMessage(m_hWnd, TB_ADDSTRING, 0, (LPARAM)szString);
			}

			// No index for this string exists, so create it now
			TCHAR szBuf[80] = _T("");
			lstrcpyn(szBuf, szText, 79);
			szBuf[lstrlen(szBuf)+1] = _T('\0');		// Double-null terminate

			iString = (int)::SendMessage(m_hWnd, TB_ADDSTRING, 0, (LPARAM)szBuf);
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
		if (!::SendMessage(m_hWnd, TB_GETBUTTON, iIndex, (LPARAM)&tbb))
			throw CWinException(_T("CToolbar::SetButtonText  TB_GETBUTTON failed"));

		tbb.iString = iString;

		// Turn off Toolbar drawing
		::SendMessage(m_hWnd, WM_SETREDRAW, FALSE, 0);

		if (!::SendMessage(m_hWnd, TB_DELETEBUTTON, iIndex, 0))
			throw CWinException(_T("CToolbar::SetButtonText  TB_DELETEBUTTON failed"));

		if (!::SendMessage(m_hWnd, TB_INSERTBUTTON, iIndex, (LPARAM)&tbb))
			throw CWinException(_T("CToolbar::SetButtonText  TB_INSERTBUTTON failed"));

		// Ensure the button now includes some text rows
		if (0 == ::SendMessage(m_hWnd, TB_GETTEXTROWS, 0, 0))
			::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, 1, 0);

		// Turn on Toolbar drawing
		::SendMessage(m_hWnd, WM_SETREDRAW, TRUE, 0);

		// Redraw button
		CRect r = GetItemRect(iIndex);
		::InvalidateRect(m_hWnd, &r, TRUE);
	}

	inline void CToolbar::SetCommandID(int iIndex, int iButtonID) const
	// Sets the command identifier of a toolbar button
	{
		if (!::SendMessage(m_hWnd, TB_SETCMDID, iIndex, iButtonID))
			throw CWinException(_T("CToolbar::SetCommandID failed"));
	}

	inline void CToolbar::SetToolbarTheme(ThemeToolbar& Theme)
	{
		m_Theme.UseThemes   = Theme.UseThemes;
		m_Theme.clrHot1     = Theme.clrHot1;
		m_Theme.clrHot2     = Theme.clrHot2;
		m_Theme.clrPressed1 = Theme.clrPressed1;
		m_Theme.clrPressed2 = Theme.clrPressed2;
		m_Theme.clrOutline  = Theme.clrOutline;

		::InvalidateRect(m_hWnd, NULL, TRUE);
	}

	inline LRESULT CToolbar::WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_WINDOWPOSCHANGING:
			{
				// Adjust size for toolbars inside a rebar
				TCHAR ClassName[32];
				::GetClassName(m_hWndParent, ClassName, 32);

				if (0 == lstrcmp(ClassName, REBARCLASSNAME))
				{
					LPWINDOWPOS pWinPos = (LPWINDOWPOS)lParam;
					pWinPos->cx = GetMaxSize().cx+2;
				}
			}
			break;
		case WM_LBUTTONDBLCLK:
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
						return 0L;
					}

					if (nStyle & TBSTYLE_DROPDOWN)
					{
						CRect rcButton = GetItemRect(iButton);

						int xPos = GET_X_LPARAM(lParam);
						if (xPos >= rcButton.right -13)
						{
							// DoubleClick on dropdown part of TBSTYLE_DROPDOWN button
							::mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
							return 0L;
						}
					}
				}
			}

			break;

		case WM_DESTROY:
			OnDestroy();
			break;
		}

		// pass unhandled messages on for default processing
		return CWnd::WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

} // namespace Win32xx

#endif // #ifndef TOOLBAR_H
