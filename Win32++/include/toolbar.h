// Win32++  Version 7.0 (Prerelease version)
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2010  David Nash
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

	struct ToolbarTheme
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
		virtual tString GetWindowType() const { return _T("CToolbar"); }

	// Attributes
		BOOL AddToolbarButton(UINT nID, BOOL bEnabled = TRUE);
		virtual void Destroy();
		int  CommandToIndex(int iButtonID) const;
		int  GetButtonCount() const;
		UINT GetButtonState(int iButtonID) const;
		BYTE GetButtonStyle(int iButtonID) const;
		int  GetCommandID(int iIndex) const;
		CRect GetItemRect(int iIndex) const;
		CSize GetMaxSize() const;
		std::vector<UINT>& GetToolbarData() const {return (std::vector <UINT> &)m_vToolbarData;}
		ToolbarTheme& GetToolbarTheme() {return m_Theme;}
		BOOL HasText() const;
		int  HitTest() const;
		BOOL SetBitmap(UINT nID);
		BOOL SetBitmapSize(int cx, int cy) const;
		int  SetButtons(const std::vector<UINT>& vToolbarData) const;
		BOOL SetButtonSize(int cx, int cy) const;
		BOOL SetButtonState(int iButtonID, UINT State) const;
		BOOL SetButtonStyle(int iButtonID, BYTE Style) const;
		BOOL SetButtonText(int iButtonID, LPCTSTR szText);
		BOOL SetButtonWidth(int iButtonID, int nWidth) const;
		BOOL SetCommandID(int iIndex, int iButtonID) const;
		BOOL SetImages(COLORREF crMask, UINT ToolbarID, UINT ToolbarHotID, UINT ToolbarDisabledID);
		void SetToolbarTheme(ToolbarTheme& Theme);

	// Operations
		int  AddBitmap(UINT ToolbarID);
		BOOL DisableButton(int iButtonID) const;
		BOOL EnableButton(int iButtonID) const;
		BOOL ReplaceBitmap(UINT NewToolbarID);

	protected:
	// Overridables
		virtual void OnCreate();
		virtual void OnDestroy();
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
		ToolbarTheme m_Theme;				// The theme structure
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
		ZeroMemory(&m_Theme, sizeof(ToolbarTheme));
	}

	inline CToolbar::~CToolbar()
	{
	}

	inline int CToolbar::AddBitmap(UINT ToolbarID)
	// Adds one or more images to the list of button images available for a toolbar.

	// Note: AddBitmap supports a maximum colour depth of 8 bits (256 colours)
	//       For more colours, use an ImageList instead
	{
		assert(::IsWindow(m_hWnd));

		int iNumButtons = 0;
		std::vector<UINT>::iterator iter;
		for (iter = GetToolbarData().begin(); iter < GetToolbarData().end(); ++iter)
			if ((*iter) != 0) ++iNumButtons;

		TBADDBITMAP tbab = {0};
		tbab.hInst = GetApp()->GetResourceHandle();
		tbab.nID   = ToolbarID;
		int iResult = (int)SendMessage(TB_ADDBITMAP, iNumButtons, (LPARAM)&tbab);

		if (-1 != iResult)
			m_OldToolbarID = ToolbarID;

		return iResult;
	}

	inline BOOL CToolbar::AddToolbarButton(UINT nID, BOOL bEnabled /* = TRUE */)
	// Adds Resource IDs to toolbar buttons.
	// A resource ID of 0 is a separator
	{
		assert(::IsWindow(m_hWnd));

		m_vToolbarData.push_back(nID);

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
		return (BOOL)SendMessage(TB_ADDBUTTONS, 1L, (LPARAM)&tbb);		
	}

	inline int CToolbar::CommandToIndex(int iButtonID) const
	// Retrieves the zero-based index for the button associated with the specified command identifier
	{
		assert(::IsWindow(m_hWnd));
		
		// returns -1 on fail
		return (int)SendMessage(TB_COMMANDTOINDEX, (WPARAM)iButtonID, 0L);
	}

	inline void CToolbar::Destroy()
	// Allows CToolbar to be reused after the window is destroyed
	{
		CWnd::Destroy();
		m_StringMap.clear();
	}

	inline BOOL CToolbar::DisableButton(int iButtonID) const
	// Disables the specified button in a toolbar
	// An example of iButtonID would be IDM_FILE_OPEN
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_ENABLEBUTTON, (WPARAM)iButtonID, (LPARAM) MAKELONG(FALSE, 0));
	}

	inline BOOL CToolbar::EnableButton(int iButtonID) const
	// Enables the specified button in a toolbar
	{
		assert(::IsWindow(m_hWnd));	
		return (BOOL)SendMessage(TB_ENABLEBUTTON, (WPARAM)iButtonID, (LPARAM) MAKELONG(TRUE,0 ));
	}

	inline int CToolbar::GetButtonCount() const
	// Retrieves a count of the buttons currently in the toolbar
	{
		assert(::IsWindow(m_hWnd));
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
		assert(::IsWindow(m_hWnd));
		return (UINT)SendMessage(TB_GETSTATE, (WPARAM) iButtonID, 0L);
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
		assert(::IsWindow(m_hWnd));
		
		int iIndex = CommandToIndex(iButtonID);
		TBBUTTON tbb = {0};
		SendMessage(TB_GETBUTTON, iIndex, (LPARAM) &tbb);
		
		return tbb.fsStyle;
	}

	inline int CToolbar::GetCommandID(int iIndex) const
	// Retrieves information about the specified button in a toolbar
	{
		assert(::IsWindow(m_hWnd));
		TBBUTTON tbb = {0};
		SendMessage(TB_GETBUTTON, iIndex, (WPARAM) &tbb);

		// returns zero if failed
		return tbb.idCommand;
	}

	inline CRect CToolbar::GetItemRect(int iIndex) const
	// Retrieves the bounding rectangle of a button in a toolbar
	{
		assert(::IsWindow(m_hWnd));
		CRect rc;
		int iCount = (int)SendMessage(TB_BUTTONCOUNT, 0L, 0L);

		if (iCount >= iIndex)
			SendMessage(TB_GETITEMRECT, (WPARAM)iIndex, (LPARAM)&rc);

		return rc;
	}

	inline CSize CToolbar::GetMaxSize() const
	// Retrieves the total size of all of the visible buttons and separators in the toolbar
	{
		assert(::IsWindow(m_hWnd));
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
		assert(::IsWindow(m_hWnd));
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
		assert(::IsWindow(m_hWnd));
		CPoint pt = GetCursorPos();
		::ScreenToClient(m_hWnd, &pt);

		int nButtons = (int)SendMessage(TB_BUTTONCOUNT, 0L, 0L);
		int iButton = -1;

		for (int i = 0 ; i < nButtons; ++i)
		{
			CRect rc = GetItemRect(i);
			if (rc.PtInRect(pt))
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

		// Turn of Double click processing (i.e. treat a double click as two single clicks)
		DWORD dwStyle = (DWORD)GetClassLongPtr(GCL_STYLE);
		dwStyle &= 	~CS_DBLCLKS;
		SetClassLongPtr(GCL_STYLE, dwStyle);

		// Add extra styles for toolbars inside a rebar
		CWnd* pWnd = FromHandle(GetParent());
		if (pWnd && (pWnd->GetWindowType() == _T("CRebar")))
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
				rcRect.InflateRect(-1, -1);
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
						rcText.OffsetRect(1, 1);
						DrawDC.SetTextColor(RGB(255,255,255));
						DrawDC.DrawText(szText, lstrlen(szText), rcText, DT_LEFT);
						rcText.OffsetRect(-1, -1);
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

	inline LRESULT CToolbar::OnNotifyReflect(WPARAM wParam, LPARAM lParam)
	// Notifications sent to the parent window are reflected back here
	{
		UNREFERENCED_PARAMETER(wParam);

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

	inline void CToolbar::OnWindowPosChanging(WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(wParam);

		// Adjust size for toolbars inside a rebar
		CWnd* pParent = FromHandle(GetParent());
		if (_T("CRebar") == pParent->GetWindowType())
		{
			RebarTheme* pTheme = (RebarTheme*)pParent->SendMessage(UWM_GETREBARTHEME, 0, 0);
			
			if (pTheme && pTheme->UseThemes && pTheme->ShortBands)
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

	inline BOOL CToolbar::ReplaceBitmap(UINT NewToolbarID)
	// Replaces an existing bitmap with a new bitmap.

	// Note: ReplaceBitmap supports a maximum colour depth of 8 bits (256 colours)
	//       For more colours, use an ImageList instead
	{
		assert(::IsWindow(m_hWnd));

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

		BOOL bResult = (BOOL)SendMessage(TB_REPLACEBITMAP, iNumButtons, (LPARAM)&tbrb);
		if (bResult) 
			m_OldToolbarID = NewToolbarID;

		return bResult;
	}

	inline BOOL CToolbar::SetBitmap(UINT nID)
	// Set the button images
	{
		assert(::IsWindow(m_hWnd));
		
		HBITMAP hbm = LoadBitmap(MAKEINTRESOURCE(nID));
		assert (hbm);
		BITMAP bm = {0};

		int Result = ::GetObject(hbm, sizeof(BITMAP), &bm);
		assert (Result);

		int iNumButtons = 0;
		std::vector<UINT>::iterator iter;
		for (iter = GetToolbarData().begin(); iter < GetToolbarData().end(); ++iter)
			if ((*iter) != 0) ++iNumButtons;
		
		int iImageWidth  = bm.bmWidth / iNumButtons;
		int iImageHeight = bm.bmHeight;

		// Set the bitmap size first
		SetBitmapSize(iImageWidth, iImageHeight);

		BOOL bResult = FALSE;
		if (m_OldToolbarID)
			bResult = ReplaceBitmap(nID);
		else
			bResult = (BOOL)AddBitmap(nID);

		return bResult;
	}

	inline BOOL CToolbar::SetBitmapSize(int cx, int cy) const
	// Sets the size of the bitmapped images to be added to a toolbar.

	// Needs to be used when the image size is not the default 16 x 15
	// Call this function before using AddBitmap or ReplaceBitmap
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_SETBITMAPSIZE, 0L, MAKELONG(cx, cy));
	}

	inline int CToolbar::SetButtons(const std::vector<UINT>& vToolbarData) const
	// Assigns a resource ID to each toolbar button
	{
		assert(::IsWindow(m_hWnd));

		int iImages = 0;
		UINT iNumButtons = (UINT)vToolbarData.size();

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

			for (UINT j = 0 ; j < iNumButtons; ++j)
			{
				ZeroMemory(&tbb, sizeof(TBBUTTON));

				if (0 == vToolbarData[j])
				{
					tbb.fsStyle = TBSTYLE_SEP;
				}
				else
				{
					tbb.dwData  = iImages;
					tbb.iBitmap = iImages;
					tbb.idCommand = vToolbarData[j];
					tbb.fsState = TBSTATE_ENABLED;
					tbb.fsStyle = TBSTYLE_BUTTON;
				}

				// Add the button to the toolbar
				if (SendMessage(TB_ADDBUTTONS, 1L, (LPARAM)&tbb))
					iImages++;
				else
					break;
			}
		}

		return iImages;
	}

	inline BOOL CToolbar::SetButtonSize(int cx, int cy) const
	// Sets the size of the buttons to be added to a toolbar
	// The size can be set only before adding any buttons to the toolbar
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_SETBUTTONSIZE, 0L, MAKELONG(cx, cy));
	}

	inline BOOL CToolbar::SetButtonState(int iButtonID, UINT State) const
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

		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_SETSTATE, (WPARAM) iButtonID, (LPARAM)MAKELONG (State, 0));
 	}

	inline BOOL CToolbar::SetButtonStyle(int iButtonID, BYTE Style) const
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
		assert(::IsWindow(m_hWnd));

		TBBUTTONINFO tbbi = {0};
		tbbi.cbSize = sizeof(TBBUTTONINFO);
		tbbi.dwMask = TBIF_STYLE;
		tbbi.fsStyle = Style;

		// Note:  TB_SETBUTTONINFO requires comctl32.dll version 4.71 or later
		//        i.e. Win95 with IE4 / NT with IE4   or later
		return (BOOL)SendMessage(TB_SETBUTTONINFO, iButtonID, (LPARAM) &tbbi);
	}

	inline BOOL CToolbar::SetButtonText(int iButtonID, LPCTSTR szText)
	// This rather convoluted approach to setting toolbar button text supports
	// all versions of Windows, including Win95 with COMCTL32.DLL version 4.0
	{
		assert(::IsWindow(m_hWnd));
		int iIndex = CommandToIndex(iButtonID);
		assert(-1 != iIndex);

		BOOL Succeeded = TRUE;
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
				Succeeded = FALSE;

			// Save the string its index in our map
			m_StringMap.insert(std::make_pair(sString, iString));
		}
		else
		{
			// String found, use the index from our map
			iString = m->second;
		}

		if (Succeeded)
		{
			TBBUTTON tbb = {0};
			Succeeded = SendMessage(TB_GETBUTTON, iIndex, (LPARAM)&tbb);

			tbb.iString = iString;

			// Turn off Toolbar drawing
			SendMessage(WM_SETREDRAW, FALSE, 0L);

			if (Succeeded)
				Succeeded = SendMessage(TB_DELETEBUTTON, iIndex, 0L);

			if (Succeeded)
				Succeeded = SendMessage(TB_INSERTBUTTON, iIndex, (LPARAM)&tbb);

			// Ensure the button now includes some text rows
			if (0 == SendMessage(TB_GETTEXTROWS, 0L, 0L))
				SendMessage(TB_SETMAXTEXTROWS, 1L, 0L);

			// Turn on Toolbar drawing
			SendMessage(WM_SETREDRAW, TRUE, 0L);
		}
		// Redraw button
		CRect r = GetItemRect(iIndex);
		InvalidateRect(&r, TRUE);

		return Succeeded;
	}

	inline BOOL CToolbar::SetButtonWidth(int iButtonID, int nWidth) const
	// The set button width can adjust the width of the button after it is created.
	// This is useful when replacing a button with a ComboBox or other control.
	// Note:  TB_SETBUTTONINFO requires comctl32.dll version 4.71 or later
	//        i.e. Win95 with IE4 / NT with IE4   or later
	{
		assert(::IsWindow(m_hWnd));
		
		TBBUTTONINFO tbbi = {0};
		tbbi.cbSize = sizeof(TBBUTTONINFO);
		tbbi.dwMask = TBIF_SIZE;
		tbbi.cx = (WORD)nWidth;
		BOOL bResult = SendMessage(TB_SETBUTTONINFO, (WPARAM)iButtonID, (LPARAM)&tbbi);
		
		// Send a changed message to the parent (used by the rebar)
		SIZE MaxSize = GetMaxSize();
		::SendMessage(GetParent(), UWM_TOOLBAR_RESIZE, (WPARAM)m_hWnd, (LPARAM)&MaxSize);

		return bResult;
	}

	inline BOOL CToolbar::SetCommandID(int iIndex, int iButtonID) const
	// Sets the command identifier of a toolbar button
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_SETCMDID, iIndex, iButtonID);
	}

	inline BOOL CToolbar::SetImages(COLORREF crMask, UINT ToolbarID, UINT ToolbarHotID, UINT ToolbarDisabledID)
	// Either sets the imagelist or adds/replaces bitmap depending on ComCtl32.dll version
	// Assumes the width of the button image = bitmap_size / buttons
	// Assumes buttons have been already been added via AdddToolbarButton
	// The colour mask is often grey RGB(192,192,192) or magenta (255,0,255);
	// The color mask is ignored for 32bit bitmap resources
	// The Hot and disiabled bitmap resources can be 0
	{
		assert(::IsWindow(m_hWnd));

		int iNumButtons = 0;
		std::vector<UINT>::iterator iter;
		for (iter = GetToolbarData().begin(); iter < GetToolbarData().end(); ++iter)
			if ((*iter) != 0) ++iNumButtons;

		BOOL Succeeded = TRUE;

		if (iNumButtons > 0)
		{
			// Set the button images
			HBITMAP hbm = LoadBitmap(MAKEINTRESOURCE(ToolbarID));
			assert(hbm);

			BITMAP bm = {0};
			int iResult = GetObject(hbm, sizeof(BITMAP), &bm);
			assert(iResult);

			int iImageWidth  = bm.bmWidth / iNumButtons;
			int iImageHeight = bm.bmHeight;

			// Toolbar ImageLists require Comctl32.dll version 4.7 or later
			if (400 == GetComCtlVersion())
			{
				// We are using COMCTL32.DLL version 4.0, so we can't use an imagelist.
				// Instead we simply set the bitmap.
				return SetBitmap(ToolbarID);
			}

			HIMAGELIST himlToolbar    = (HIMAGELIST)SendMessage(TB_GETIMAGELIST,    0L, 0L);
			HIMAGELIST himlToolbarHot = (HIMAGELIST)SendMessage(TB_GETHOTIMAGELIST, 0L, 0L);
			HIMAGELIST himlToolbarDis = (HIMAGELIST)SendMessage(TB_GETDISABLEDIMAGELIST, 0L, 0L);
			ImageList_Destroy(himlToolbar);
			ImageList_Destroy(himlToolbarHot);
			ImageList_Destroy(himlToolbarDis);

			himlToolbar = ImageList_Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, iNumButtons, 0);
			assert(himlToolbar);

			ImageList_AddMasked(himlToolbar, hbm, crMask);
			SendMessage(TB_SETIMAGELIST, 0L, (LPARAM)himlToolbar);

			::DeleteObject(hbm);
			hbm = NULL;

			if (ToolbarHotID && Succeeded)
			{
				hbm = LoadBitmap(MAKEINTRESOURCE(ToolbarHotID));
				assert(hbm);

				himlToolbarHot = ImageList_Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, iNumButtons, 0);
				assert(himlToolbarHot);

				ImageList_AddMasked(himlToolbarHot, hbm, crMask);
				Succeeded = SendMessage(TB_SETHOTIMAGELIST, 0L, (LPARAM)himlToolbarHot);

				::DeleteObject(hbm);
				hbm = NULL;
			}

			if (Succeeded)
			{
				if (ToolbarDisabledID)
				{
					hbm = LoadBitmap(MAKEINTRESOURCE(ToolbarDisabledID));
					assert(hbm);

					himlToolbarDis = ImageList_Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, iNumButtons, 0);
					assert(himlToolbarDis);

					ImageList_AddMasked(himlToolbarDis, hbm, crMask);
					Succeeded = SendMessage(TB_SETDISABLEDIMAGELIST, 0L, (LPARAM)himlToolbarDis);
				}
				else
				{
					himlToolbarDis = CreateDisabledImageList(himlToolbar);
					Succeeded = (BOOL)SendMessage(TB_SETDISABLEDIMAGELIST, 0L, (LPARAM)himlToolbarDis);
				}
			}

			// Inform the parent of the change (rebar needs this)
			SIZE MaxSize = GetMaxSize();
			::SendMessage(GetParent(), UWM_TOOLBAR_RESIZE, (WPARAM)m_hWnd, (LPARAM)&MaxSize);

			::DeleteObject(hbm);
		}

		return Succeeded;
	}

	inline void CToolbar::SetToolbarTheme(ToolbarTheme& Theme)
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
		case UWM_GETTOOLBARTHEME:
			{
				ToolbarTheme& tt = GetToolbarTheme();
				return (LRESULT)&tt;
			}
		case WM_WINDOWPOSCHANGING:
			OnWindowPosChanging(wParam, lParam);
			break;
		}

		// pass unhandled messages on for default processing
		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	}

} // namespace Win32xx

#endif // #ifndef _TOOLBAR_H_
