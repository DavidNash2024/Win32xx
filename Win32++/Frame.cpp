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
// Frame.cpp
// Definitions for the CFrame, CStatusbar, CRebar, CToolbar and
// CMenubar classes:


#include "Frame.h"
#include <windowsx.h>
#include <shlwapi.h>
#include <uxtheme.h>
#include "Default_Resource.h"


namespace Win32xx
{

	//////////////////////////////////////
	// Definitions for the CStatusbar class
	//
	CStatusbar::CStatusbar()
	{
	}

	void CStatusbar::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CCS_BOTTOM | SBARS_SIZEGRIP;
		cs.lpszClass = STATUSCLASSNAME;
	}

	void CStatusbar::CreatePanes(int iPanes, const int iPaneWidths[]) const
	{
		// If an element of iPaneWidths is -1, the right edge of the corresponding part extends
		//  to the border of the window
		if (::IsWindow(m_hWnd))
		{
			if (iPanes > 256)
				throw CWinException (_T("CStatusbar::CreatePanes ... Too many panes"));

			// Create the statusbar panes
			if (!::SendMessage(m_hWnd, SB_SETPARTS, iPanes, (LPARAM)iPaneWidths))
				throw CWinException(_T("CStatusbar::CreatePanes failed"));
		}
	}

	tString CStatusbar::GetPaneText(int iPane)
	{
		tString PaneText;
		try
		{
			if (::IsWindow(m_hWnd))
			{
				// Get size of Text array
				int iChars = LOWORD (::SendMessage(m_hWnd, SB_GETTEXTLENGTH, iPane, 0));

				// Get the Text
				TCHAR* szText = new TCHAR[iChars +1 ];

				// Some MS compilers (including VS2003 under some circumstances) return NULL instead of throwing
				//  an exception when new fails. We make sure an exception gets thrown!
				if (NULL == szText)
					throw std::bad_alloc();

				szText[0] = _T('\0');
				::SendMessage(m_hWnd, SB_GETTEXT, iPane, (LPARAM)szText);

				//Store the text in the member variable
				PaneText = szText;
				delete []szText;
			}
		}

		catch (const std::bad_alloc &)
		{
			DebugErrMsg(_T("Failed to allocate memory in CStatusbar::GetPaneText"));
			PaneText = _T("");
			// Not a critical problem, so no need to rethrow
		}

		return PaneText;
	}

	void CStatusbar::SetPaneText(int iPane, LPCTSTR szText, UINT Style) const
	// Available Styles: Combinations of ...
	//0					The text is drawn with a border to appear lower than the plane of the window.
	//SBT_NOBORDERS		The text is drawn without borders.
	//SBT_OWNERDRAW		The text is drawn by the parent window.
	//SBT_POPOUT		The text is drawn with a border to appear higher than the plane of the window.
	//SBT_RTLREADING	The text will be displayed in the opposite direction to the text in the parent window.
	{
		if (::IsWindow(m_hWnd))
		{
			if (::SendMessage(m_hWnd, SB_GETPARTS, 0, 0) >= iPane)
			{
				if (!::SendMessage(m_hWnd, SB_SETTEXT, iPane | Style, (LPARAM)szText))
					throw CWinException(_T("Failed to set status bar text"));
			}
		}
	}

	void CStatusbar::SetPaneWidth(int iPane, int iWidth) const
	{
		// This changes the width of an existing pane, or creates a new pane
		// with the specified width
		try
		{
			if (::IsWindow(m_hWnd))
			{
				if ((iPane > 256) || (iWidth < 0))
					throw CWinException (_T("CStatusbar::SetPaneWidth ... Invalid parameters"));

				if (iPane < 0) iPane = 0;

				int iParts = (int)::SendMessage(m_hWnd, SB_GETPARTS, 0, 0);
				int* iPaneWidths = new int[iParts];

				// Some MS compilers (including VS2003 under some circumstances) return NULL instead of throwing
				//  an exception when new fails. We make sure an exception gets thrown!
				if (NULL == iPaneWidths)
					throw std::bad_alloc();

				::SendMessage(m_hWnd, SB_GETPARTS, iParts, (LPARAM)iPaneWidths);

				int iNewParts = max(iPane+1, iParts);
				int* iNewPaneWidths = new int[iNewParts];
				if (NULL == iNewPaneWidths)
					throw std::bad_alloc();

				ZeroMemory(iNewPaneWidths, iNewParts*sizeof(int));

				for (int i = 0; i < iParts; i++)
					iNewPaneWidths[i] = iPaneWidths[i];

				if (0 == iPane)
					iNewPaneWidths[iPane] = iWidth;
				else
					iNewPaneWidths[iPane] = iNewPaneWidths[iPane -1] + iWidth;

				if (!::SendMessage(m_hWnd, SB_SETPARTS, iNewParts, (LPARAM)iNewPaneWidths))
					throw CWinException(_T("CStatusbar::SetPaneWidth failed"));

				delete []iNewPaneWidths;
				delete []iPaneWidths;
			}
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (const std::bad_alloc &)
		{
			DebugErrMsg(_T("Exception in CStatusbar::SetPaneWidth"));
			// Not a critical problem, so no need to rethrow
		}
	}


	////////////////////////////////////
	// Definitions for the CToolbar class
	//
	CToolbar::CToolbar() : m_hImageList(NULL), m_hImageListHot(NULL), m_hImageListDis(NULL),
		                    m_OldToolbarID(0), m_bDrawArrowBkgrnd(FALSE)
	{
		ZeroMemory(&m_Theme, sizeof(ThemeToolbar));
	}

	CToolbar::~CToolbar()
	{
		if (m_hImageList)     ::DeleteObject(m_hImageList);
		if (m_hImageListHot)  ::DeleteObject(m_hImageListHot);
		if (m_hImageListDis)  ::DeleteObject(m_hImageListDis);
	}

	int CToolbar::CommandToIndex(int iButtonID) const
	// Retrieves the zero-based index for the button associated with the specified command identifier
	{
		// returns -1 on fail
		return (int)::SendMessage(m_hWnd, TB_COMMANDTOINDEX, (WPARAM)iButtonID, 0);
	}

	void CToolbar::AddBitmap(int iNumButtons, UINT ToolbarID)
	// Note: AddBitmap supports a maximum colour depth of 8 bits (256 colours)
	//       For more colours, use SetImageList instead
	{

		TBADDBITMAP tbab = {0};
		tbab.hInst = GetApp()->GetResourceHandle();
		tbab.nID   = ToolbarID;
		if (-1 == ::SendMessage(m_hWnd, TB_ADDBITMAP, iNumButtons, (LPARAM)&tbab) )
			throw CWinException(_T("CToolbar::AddBitmap  TB_ADDBITMAP failed"));

		m_OldToolbarID = ToolbarID;
	}

	void CToolbar::ReplaceBitmap(int iNumButtons, UINT NewToolbarID)
	// Note: ReplaceBitmap supports a maximum colour depth of 8 bits (256 colours)
	//       For more colours, use SetImageList instead
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

	void CToolbar::DisableButton(int iButtonID) const
	// Disables the specified button in a toolbar
	{
		// An example of iButtonID would be IDM_FILE_OPEN
		if (!::SendMessage(m_hWnd, TB_ENABLEBUTTON, (WPARAM)iButtonID, (LPARAM) MAKELONG(FALSE, 0)))
			TRACE(_T("Disable button failed\n"));
	}

	void CToolbar::EnableButton(int iButtonID) const
	// Enables the specified button in a toolbar
	{
		if (!::SendMessage(m_hWnd, TB_ENABLEBUTTON, (WPARAM)iButtonID, (LPARAM) MAKELONG(TRUE,0 )))
			TRACE(_T("Enable button failed\n"));
	}

	int CToolbar::GetButtonCount() const
	// Retrieves a count of the buttons currently in the toolbar
	{
		return (int)::SendMessage(m_hWnd, TB_BUTTONCOUNT, 0, 0);
	}

	UINT CToolbar::GetButtonState(int iButtonID) const
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

	BYTE CToolbar::GetButtonStyle(int iButtonID) const
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

	int CToolbar::GetCommandID(int iIndex) const
	// Retrieves information about the specified button in a toolbar
	{
		TBBUTTON tbb = {0};
		::SendMessage(m_hWnd, TB_GETBUTTON, iIndex, (WPARAM) &tbb);

		// returns zero if failed
		return tbb.idCommand;
	}

	RECT CToolbar::GetItemRect(int iIndex) const
	// Retrieves the bounding rectangle of a button in a toolbar
	{
		RECT rc = {0};
		int iCount = (int)::SendMessage(m_hWnd, TB_BUTTONCOUNT, 0, 0);

		if (iCount >= iIndex)
			::SendMessage(m_hWnd, TB_GETITEMRECT, (WPARAM)iIndex, (LPARAM)&rc);

		return rc;
	}

	SIZE CToolbar::GetMaxSize() const
	// Retrieves the total size of all of the visible buttons and separators in the toolbar
	{
		SIZE sz = {0};
		::SendMessage(m_hWnd, TB_GETMAXSIZE, 0, (LPARAM)&sz);

		// This fixes a Windows bug calculating the size when TBSTYLE_DROPDOWN is used.
		int xMaxSize = 0;
		for (int i= 0 ; i < GetButtonCount(); i++)
		{
			xMaxSize += GetItemRect(i).right - GetItemRect(i).left;
		}

		sz.cx = xMaxSize;
		return sz;
	}

	BOOL CToolbar::HasText() const
	{
		BOOL bReturn = FALSE;

		for (int i = 0 ; i < GetButtonCount(); i++)
		{
			if (::SendMessage(m_hWnd, TB_GETBUTTONTEXT, GetCommandID(i), 0) != -1)
				bReturn = TRUE;
		}

		// return TRUE if any button has text
		return bReturn;
	}

	int CToolbar::HitTest() const
	{
		// We do our own hit test since TB_HITTEST is a bit buggy,
		// and also doesn't work at all on earliest versions of Win95

		POINT pt = {0};
		::GetCursorPos(&pt);
		::ScreenToClient(m_hWnd, &pt);

		int nButtons = (int)::SendMessage(m_hWnd, TB_BUTTONCOUNT, 0, 0);
		int iButton = -1;

		for (int i = 0 ; i < nButtons; i++)
		{
			RECT r = GetItemRect(i);
			if (::PtInRect(&r, pt))
				iButton = i;
		}

		return iButton;
	}

	void CToolbar::OnCreate()
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
			DWORD style = ::GetWindowLong(m_hWnd, GWL_STYLE);
			style |= CCS_NODIVIDER | CCS_NORESIZE;
			::SetWindowLong(m_hWnd, GWL_STYLE, style);
		}
	}

	LRESULT CToolbar::OnCustomDraw(NMHDR* pNMHDR)
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
				RECT rcRect = lpNMCustomDraw->nmcd.rc;
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
				SIZE TextSize = {0};
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

				HIMAGELIST hImageList = (nState & CDIS_DISABLED)? m_hImageListDis : (m_hImageListHot? m_hImageListHot: m_hImageList);
				BOOL IsWin95 = (1400 == (GetWinVersion()) || (2400 == GetWinVersion()));

				// Calculate image position
				int cxImage = 0;
				int cyImage = 0;
				ImageList_GetIconSize(hImageList, &cxImage, &cyImage);

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
						RECT rcArrowBkgnd = {0};
						::CopyRect(&rcArrowBkgnd, &rcRect);
						rcArrowBkgnd.left = rcArrowBkgnd.right - 13;
						GradientFill(DrawDC, m_Theme.clrPressed1, m_Theme.clrPressed2, &rcArrowBkgnd, FALSE);
					}

					m_bDrawArrowBkgrnd = FALSE;

					// Manually draw the dropdown arrow
					DrawDC.CreatePen(PS_SOLID, 1, RGB(0,0,0));
					for (int i = 2; i >= 0; i--)
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
					ImageList_Draw(hImageList, iImage, DrawDC, xImage, yImage, ILD_TRANSPARENT);
				}

				//Draw Text
				if (lstrlen(szText) > 0)
				{
					int iWidth = rcRect.right - rcRect.left - ((nStyle & TBSTYLE_DROPDOWN)?13:0);
					RECT rcText = {0, 0, min(TextSize.cx, iWidth), TextSize.cy};

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

	LRESULT CToolbar::OnNotifyReflect(WPARAM /* wParam */, LPARAM lParam)
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

	void CToolbar::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT;
		cs.lpszClass = TOOLBARCLASSNAME;
	}

	void CToolbar::SetBitmapSize(int cx, int cy) const
	// Needs to be used when the image size is not the default 16 x 15
	// Call this function before using AddBitmap or ReplaceBitmap
	{
		if (!::SendMessage(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELONG(cx, cy)))
			TRACE(_T("CToolbar::SetBitmapSize  failed\n"));
	}

	int CToolbar::SetButtons(const std::vector<UINT>& ToolbarData) const
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

			for (int j = 0 ; j < iNumButtons; j++)
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

	void CToolbar::SetButtonSize(int cx, int cy) const
	// Sets the size of the buttons to be added to a toolbar
	// The size can be set only before adding any buttons to the toolbar
	{

		if (!::SendMessage(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELONG(cx, cy)))
			TRACE(_T("CToolbar::SetButtonSize failed\n"));
	}

	void CToolbar::SetButtonState(int iButtonID, UINT State) const
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

	void CToolbar::SetButtonStyle(int iButtonID, BYTE Style) const
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

	void CToolbar::SetButtonText(int iButtonID, LPCTSTR szText)
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
		RECT r = GetItemRect(iIndex);
		::InvalidateRect(m_hWnd, &r, TRUE);
	}

	void CToolbar::SetCommandID(int iIndex, int iButtonID) const
	// Sets the command identifier of a toolbar button
	{
		if (!::SendMessage(m_hWnd, TB_SETCMDID, iIndex, iButtonID))
			throw CWinException(_T("CToolbar::SetCommandID failed"));
	}

	void CToolbar::SetImageList(int iNumButtons, COLORREF crMask, UINT ToolbarID, UINT ToolbarHotID /*= 0*/, UINT ToolbarDisabledID /*= 0*/)
	// Either sets the imagelist or adds/replaces bitmap depending on ComCtl32.dll version
	// Assumes the width of the button image = bitmap_size / buttons
	// This colour mask is often grey RGB(192,192,192) or magenta (255,0,255);
	{

		if (m_hImageList)    ::ImageList_Destroy(m_hImageList);
		if (m_hImageListDis) ::ImageList_Destroy(m_hImageListDis);
		if (m_hImageListHot) ::ImageList_Destroy(m_hImageListHot);
		m_hImageList = 0;
		m_hImageListDis = 0;
		m_hImageListHot = 0;

		if (iNumButtons > 0)
		{
			// Set the button images
			HBITMAP hbm = LoadBitmap(MAKEINTRESOURCE(ToolbarID));
			if (!hbm)
				throw CWinException(_T("CToolbar::SetImageList ... LoadBitmap failed "));

			BITMAP bm = {0};

			if (!::GetObject(hbm, sizeof(BITMAP), &bm))
				throw CWinException(_T("CToolbar::SetImageList ... GetObject failed "));

			int iImageWidth  = bm.bmWidth / iNumButtons;
			int iImageHeight = bm.bmHeight;

			// Toolbar ImageLists require Comctl32.dll version 4.7 or later
			if (400 == GetComCtlVersion())
			{
				// We are using COMCTL32.DLL version 4.0, so we can't use an imagelist.
				// Instead we simply add/replace the bitmap.

				// Set the bitmap size first
				SetBitmapSize(iImageWidth, iImageHeight);

				if (m_OldToolbarID)
					ReplaceBitmap(iNumButtons, ToolbarID);
				else
					AddBitmap(iNumButtons, ToolbarID);
				return;
			}

			m_hImageList = ImageList_Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, iNumButtons, 0);
			if (!m_hImageList)
				throw CWinException(_T("CToolbar::SetImageList ... Create m_hImageList failed "));

			ImageList_AddMasked(m_hImageList, hbm, crMask);
			if(-1L == SendMessage(m_hWnd, TB_SETIMAGELIST, 0, (LPARAM)m_hImageList) )
				throw CWinException(_T("CToolbar::SetImageList ... TB_SETIMAGELIST failed "));

			::DeleteObject(hbm);
			hbm = NULL;

			if (ToolbarHotID)
			{
				hbm = LoadBitmap(MAKEINTRESOURCE(ToolbarHotID));
				if (!hbm)
					throw CWinException(_T("CToolbar::SetImageList ... LoadBitmap failed "));

				m_hImageListHot = ImageList_Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, iNumButtons, 0);
				if (!m_hImageListHot)
					throw CWinException(_T("CToolbar::SetImageList ... Create m_hImageListHot failed "));

				ImageList_AddMasked(m_hImageListHot, hbm, crMask);

				if(-1L == SendMessage(m_hWnd, TB_SETHOTIMAGELIST, 0, (LPARAM)m_hImageListHot) )
					throw CWinException(_T("CToolbar::SetImageList ... TB_SETHOTIMAGELIST failed "));

				::DeleteObject(hbm);
				hbm = NULL;
			}

			if (ToolbarDisabledID)
			{
				hbm = LoadBitmap(MAKEINTRESOURCE(ToolbarDisabledID));
				if (!hbm)
					throw CWinException(_T("CToolbar::SetImageList ... LoadBitmap failed "));

				m_hImageListDis = ImageList_Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, iNumButtons, 0);
				if (!m_hImageListDis)
					throw CWinException(_T("CToolbar::SetImageList ... Create m_hImageListDis failed "));

				ImageList_AddMasked(m_hImageListDis, hbm, crMask);
				if(-1L == SendMessage(m_hWnd, TB_SETDISABLEDIMAGELIST, 0, (LPARAM)m_hImageListDis) )
					throw CWinException(_T("CToolbar::SetImageList ... TB_SETDISABLEDIMAGELIST failed "));
			}
			else
			{
				m_hImageListDis = CreateDisabledImageList(m_hImageList);
			}

			::DeleteObject(hbm);
		}
	}

	void CToolbar::SetToolbarTheme(ThemeToolbar& Theme)
	{
		m_Theme.UseThemes   = Theme.UseThemes;
		m_Theme.clrHot1     = Theme.clrHot1;
		m_Theme.clrHot2     = Theme.clrHot2;
		m_Theme.clrPressed1 = Theme.clrPressed1;
		m_Theme.clrPressed2 = Theme.clrPressed2;
		m_Theme.clrOutline  = Theme.clrOutline;

		::InvalidateRect(m_hWnd, NULL, TRUE);
	}

	LRESULT CToolbar::WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_WINDOWPOSCHANGING:
			{
				CFrame* pFrame = (CFrame*)FromHandle(GetAncestor(m_hWnd));
				LPWINDOWPOS pWinPos = (LPWINDOWPOS)lParam;
				if (pFrame->IsRebarUsed())
				{
					ThemeRebar rb = pFrame->GetRebar().GetRebarTheme();
					if (rb.UseThemes && rb.ShortBands)
					{
						pWinPos->cx = GetMaxSize().cx+2;
					}
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
						RECT rcButton = GetItemRect(iButton);
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
		}

		// pass unhandled messages on for default processing
		return CWnd::WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

	///////////////////////////////////
	// Definitions for the CRebar class
	//
	CRebar::CRebar() : m_bIsDragging(FALSE)
	{
		ZeroMemory(&m_Theme, sizeof(ThemeRebar));
	}

	CRebar::~CRebar()
	{
	}

	void CRebar::DeleteBand(int nBand) const
	{
		if (!::SendMessage(m_hWnd, RB_DELETEBAND, nBand, 0))
			throw CWinException(_T("Failed to delete rebar band"));
	}

	int CRebar::GetBand(HWND hWnd) const
	// Returns the zero based band number for this window handle
	{
		int nResult = -1;
		if (NULL == hWnd) return nResult;

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

	RECT CRebar::GetBandBorders(int nBand) const
	{
		RECT rc = {0};
		::SendMessage(m_hWnd, RB_GETBANDBORDERS, nBand, (LPARAM)&rc);
		return rc;
	}

	int CRebar::GetBandCount() const
	{
		return (int)::SendMessage(m_hWnd, RB_GETBANDCOUNT, 0, 0);
	}

	void CRebar::GetBandInfo(int nBand, LPREBARBANDINFO prbbi) const
	{
		// REBARBANDINFO describes individual BAND characteristics
		if (nBand < 0)
			throw CWinException(_T("Invalid Rebar band number"));

		prbbi->cbSize = sizeof(REBARBANDINFO);
		if(!::SendMessage(m_hWnd, RB_GETBANDINFO, nBand, (LPARAM)prbbi))
			throw CWinException(_T("Failed to get rebar band info"));
	}

	RECT CRebar::GetBandRect(int i) const
	{
		RECT rc = {0};
		::SendMessage(m_hWnd, RB_GETRECT, i, (LPARAM)&rc);
		return rc;
	}

	void CRebar::GetBarInfo(LPREBARINFO prbi) const
	{
		// REBARINFO describes overall rebar control characteristics
		prbi->cbSize = sizeof(REBARINFO);
		if (!::SendMessage(m_hWnd, RB_GETBARINFO, 0, (LPARAM)prbi))
			throw CWinException(_T("Failed to get rebar info"));
	}

	int CRebar::GetRowHeight(int nRow) const
	{
		return (int)::SendMessage(m_hWnd, RB_GETROWHEIGHT, nRow, 0);
	}

	void CRebar::InsertBand(int nBand, LPREBARBANDINFO prbbi) const
	{
		if ( !::SendMessage(m_hWnd, RB_INSERTBAND, nBand, (LPARAM)(LPREBARBANDINFO)prbbi))
			throw CWinException(_T("Failed to insert rebar band"));
	}

	BOOL CRebar::IsBandVisible(int nBand) const
	{
		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(REBARBANDINFO);
		rbbi.fMask = RBBIM_STYLE;
		GetBandInfo(nBand, &rbbi);

		return !(rbbi.fStyle & RBBS_HIDDEN);
	}

	BOOL CRebar::OnEraseBkgnd(HDC hDC)
	{
		if (!m_Theme.UseThemes)
			return FALSE;

		RECT rc;
		GetClientRect(m_hWnd, &rc);
		int BarWidth = rc.right - rc.left;
		int BarHeight = rc.bottom - rc.top;

		// Create and set up our memory DC
		CDC MemDC = ::CreateCompatibleDC(hDC);
		MemDC.CreateCompatibleBitmap(hDC, BarWidth, BarHeight);

		// Draw to Rebar background to the memory DC
		rc.right = 600;
		GradientFill(MemDC, m_Theme.clrBkgnd1, m_Theme.clrBkgnd2, &rc, TRUE);
		if (BarWidth >= 600)
		{
			rc.left = 600;
			rc.right = BarWidth;
			SolidFill(MemDC, m_Theme.clrBkgnd2, &rc);
		}


		if (m_Theme.clrBand1 || m_Theme.clrBand2)
		{
			// Draw the individual band backgrounds
			for (int nBand = 0 ; nBand < GetBandCount(); nBand++)
			{
				if (IsBandVisible(nBand))
				{
					CFrame* pFrame = (CFrame*)FromHandle(m_hWndParent);
					HWND hwndMenubar = pFrame->GetMenubar();
					if (nBand != GetBand(hwndMenubar))
					{
						// Determine the size of this band
						RECT rcBand = GetBandRect(nBand);

						// Determine the size of the child window
						REBARBANDINFO rbbi = {0};
						rbbi.cbSize = sizeof(REBARBANDINFO);
						rbbi.fMask = RBBIM_CHILD ;
						GetBandInfo(nBand, &rbbi);
						RECT rcChild;
						::GetWindowRect(rbbi.hwndChild, &rcChild);
						int ChildWidth = rcChild.right - rcChild.left;

						// Determine our drawing rectangle
						RECT rcDraw = {0};
						CopyRect(&rcDraw, &rcBand);
						rcDraw.bottom = rcDraw.top + (rcBand.bottom - rcBand.top)/2;
						int xPad = IsXPThemed()? 2: 0;
						rcDraw.left -= xPad;

						// Fill the Source CDC with the band's background
						CDC SourceDC = ::CreateCompatibleDC(hDC);
						SourceDC.CreateCompatibleBitmap(hDC, BarWidth, BarHeight);
						RECT rcBorder = GetBandBorders(nBand);
						rcDraw.right = rcBand.left + ChildWidth + rcBorder.left;
						SolidFill(SourceDC, m_Theme.clrBand1, &rcDraw);
						rcDraw.top = rcDraw.bottom;
						rcDraw.bottom = rcBand.bottom;
						GradientFill(SourceDC, m_Theme.clrBand1, m_Theme.clrBand2, &rcDraw, FALSE);

						// Set Curve amount for rounded edges
						int Curve = m_Theme.RoundBorders? 12 : 0;

						// Create our mask for rounded edges using RoundRect
						CDC MaskDC = ::CreateCompatibleDC(hDC);
						MaskDC.CreateCompatibleBitmap(hDC, BarWidth, BarHeight);

						rcDraw.top = rcBand.top;
						if (!m_Theme.FlatStyle)
							::InflateRect(&rcDraw, 1, 1);

						int left = rcDraw.left;
						int right = rcDraw.right;
						int top = rcDraw.top;
						int bottom = rcDraw.bottom;
						int cx = rcDraw.right - rcBand.left + xPad;
						int cy = rcDraw.bottom - rcBand.top;

						if (m_Theme.FlatStyle)
						{
							SolidFill(MaskDC, RGB(0,0,0), &rcDraw);
							::BitBlt(MaskDC, left, top, cx, cy, MaskDC, left, top, PATINVERT);
							::RoundRect(MaskDC, left, top, right, bottom, Curve, Curve);
						}
						else
						{
							SolidFill(MaskDC, RGB(0,0,0), &rcDraw);
							::RoundRect(MaskDC, left, top, right, bottom, Curve, Curve);
							::BitBlt(MaskDC, left, top, cx, cy, MaskDC, left, top, PATINVERT);
						}

						// Copy Source DC to Memory DC using the RoundRect mask
						::BitBlt(MemDC, left, top, cx, cy, SourceDC, left, top, SRCINVERT);
						::BitBlt(MemDC, left, top, cx, cy, MaskDC,   left, top, SRCAND);
						::BitBlt(MemDC, left, top, cx, cy, SourceDC, left, top, SRCINVERT);

						// Extra drawing to prevent jagged edge while moving bands
						if (m_bIsDragging)
						{
							CDC RebarDC = ::GetDC(m_hWnd);
							::BitBlt(RebarDC, rcDraw.right - ChildWidth, rcDraw.top, ChildWidth, cy, MemDC, rcDraw.right - ChildWidth, rcDraw.top, SRCCOPY);
						}
					}
				}
			}
		}

		if (m_Theme.UseLines)
		{
			// Draw lines between bands
			for (int j = 0; j < GetBandCount()-1; j++)
			{
				rc = GetBandRect(j);
				rc.left = max(0, rc.left - 4);
				rc.bottom +=2;
				::DrawEdge(MemDC, &rc, EDGE_ETCHED, BF_BOTTOM | BF_ADJUST);
			}
		}

		// Copy the Memory DC to the window's DC
		::BitBlt(hDC, 0, 0, BarWidth, BarHeight, MemDC, 0, 0, SRCCOPY);

		return TRUE;
	}

	void CRebar::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
                         CCS_NODIVIDER | RBS_VARHEIGHT | RBS_BANDBORDERS ;

		cs.cy = 100;
		cs.lpszClass = REBARCLASSNAME;
	}

	void CRebar::MoveBandsLeft()
	{
		int OldrcTop = -1;
		for (int nBand = GetBandCount() -1; nBand >= 0; nBand--)
		{
			RECT rc = GetBandRect(nBand);
			if (rc.top != OldrcTop)
			{
				// Maximize the last band on each row
				if (IsBandVisible(nBand))
					::SendMessage(GetHwnd(), RB_MAXIMIZEBAND, nBand, 0);

				OldrcTop = rc.top;
			}
		}
	}

	void CRebar::ResizeBand(int nBand, SIZE sz) const
	{
		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(rbbi);
		rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_SIZE;

		GetBandInfo(nBand, &rbbi);
		rbbi.cx         = sz.cx + 2;
		rbbi.cxMinChild = sz.cx + 2;
		rbbi.cyMinChild = sz.cy;
		rbbi.cyMaxChild = sz.cy;
		SetBandInfo(nBand, &rbbi );
	}

	void CRebar::SetBandBitmap(int nBand, HBITMAP hBackground) const
	{
		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(REBARBANDINFO);
		rbbi.fMask  = RBBIM_STYLE;
		GetBandInfo(nBand, &rbbi);
		rbbi.fMask  |= RBBIM_BACKGROUND;
		rbbi.hbmBack = hBackground;
		::SendMessage(m_hWnd, RB_SETBANDINFO, nBand, (LPARAM)&rbbi);
	}

	void CRebar::SetBandColor(int nBand, COLORREF clrFore, COLORREF clrBack) const
	{
		// Won't work with XP themes enabled
		// Won't work if a bitmap has been set
		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(REBARBANDINFO);
		rbbi.fMask = RBBIM_COLORS;
		rbbi.clrFore = clrFore;
		rbbi.clrBack = clrBack;
		::SendMessage(m_hWnd, RB_SETBANDINFO, nBand, (LPARAM)&rbbi);
	}

	void CRebar::SetBandInfo(int nBand, LPREBARBANDINFO prbbi) const
	{
		if (nBand < 0)
			throw CWinException(_T("Invalid Rebar band number"));

		// REBARBANDINFO describes individual BAND characteristics
		prbbi->cbSize = sizeof(REBARBANDINFO);
		if(!::SendMessage(m_hWnd, RB_SETBANDINFO, nBand, (LPARAM)prbbi))
			throw CWinException(_T("Failed to set rebar band info"));
	}

	void CRebar::SetBarInfo(LPREBARINFO prbi) const
	{
		// REBARINFO associates an image list with the rebar
		// A band will also need to set RBBIM_IMAGE
		prbi->cbSize = sizeof(REBARINFO);
		if (!::SendMessage(m_hWnd, RB_SETBARINFO, 0, (LPARAM)prbi))
			throw CWinException(_T("Failed to set rebar info"));
	}

	void CRebar::SetRebarTheme(ThemeRebar& Theme)
	{
		m_Theme.UseThemes    = Theme.UseThemes;
		m_Theme.clrBkgnd1    = Theme.clrBkgnd1;
		m_Theme.clrBkgnd2    = Theme.clrBkgnd2;
		m_Theme.clrBand1     = Theme.clrBand1;
		m_Theme.clrBand2     = Theme.clrBand2;
		m_Theme.KeepBandsLeft= Theme.KeepBandsLeft;
		m_Theme.LockMenuBand = Theme.LockMenuBand;
		m_Theme.ShortBands   = Theme.ShortBands;
		m_Theme.UseLines     = Theme.UseLines;
		m_Theme.FlatStyle    = Theme.FlatStyle;
		m_Theme.RoundBorders = Theme.RoundBorders;

		CFrame* pFrame = (CFrame*)FromHandle(m_hWndParent);
		CMenubar& MB = pFrame->GetMenubar();
		if (m_Theme.LockMenuBand)
			ShowGripper(GetBand(MB), FALSE);
		else
			ShowGripper(GetBand(MB), TRUE);

		::InvalidateRect(m_hWnd, NULL, TRUE);
	}

	BOOL CRebar::ShowBand(int nBand, BOOL fShow) const
	// Show or hide a band
	{
		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(REBARBANDINFO);
		rbbi.fMask = RBBIM_STYLE;
		GetBandInfo(nBand, &rbbi);
		if (fShow)
		{
			rbbi.fStyle &=  ~RBBS_HIDDEN;
			SetBandInfo(nBand, &rbbi);
		}
		else
		{
			rbbi.fStyle |= RBBS_HIDDEN;
			SetBandInfo(nBand, &rbbi);
		}
		return fShow;
	}

	void CRebar::ShowGripper(int nBand, BOOL fShow) const
	{
		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(REBARBANDINFO);
		rbbi.fMask = RBBIM_STYLE;
		GetBandInfo(nBand, &rbbi);
		if (fShow)
		{
			rbbi.fStyle |= RBBS_GRIPPERALWAYS;
			rbbi.fStyle &= ~RBBS_FIXEDSIZE;
		}
		else
		{
			rbbi.fStyle &= ~RBBS_GRIPPERALWAYS;
			rbbi.fStyle |= RBBS_FIXEDSIZE;
		}
		SetBandInfo(nBand, &rbbi);
	}


	LRESULT CRebar::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		static LPARAM Orig_lParam;
		switch (uMsg)
		{
		case WM_MOUSEMOVE:
			if (m_Theme.UseThemes && m_Theme.LockMenuBand)
			{
				// We want to lock the first row in place, but allow other bands to move!
				// Use move messages to limit the resizing of bands
				int y = GET_Y_LPARAM(lParam);

				if (y <= GetRowHeight(0))
					return 0L;	// throw this message away
			}
			break;
		case WM_LBUTTONDOWN:
			Orig_lParam = lParam;	// Store the x,y position
			m_bIsDragging = TRUE;
			break;
		case WM_LBUTTONUP:
			if (m_Theme.UseThemes && m_Theme.LockMenuBand)
			{
				// Use move messages to limit the resizing of bands
				int y = GET_Y_LPARAM(lParam);

				if (y <= GetRowHeight(0))
				{
					// Use x,y from WM_LBUTTONDOWN for WM_LBUTTONUP position
					lParam = Orig_lParam;
				}
			}
			m_bIsDragging = FALSE;
			break;
		case WM_ERASEBKGND:
			if (OnEraseBkgnd((HDC)wParam))
				return TRUE;
			break;
		}

		// pass unhandled messages on for default processing
		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}


	/////////////////////////////////////
	// Definitions for the CMenubar class
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
		m_hPrevFocus	= NULL;
		m_nMDIButton    = 0;

		ZeroMemory(&m_ThemeMenu, sizeof(ThemeMenu));
	}

	CMenubar::~CMenubar()
	{
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

		HWND hMaxMDIChild = NULL;
		if (IsMDIChildMaxed())
			hMaxMDIChild = GetActiveMDIChild();

		// Load the submenu
		int nMaxedOffset = IsMDIChildMaxed()? 1:0;
		m_hPopupMenu = ::GetSubMenu(m_hTopMenu, m_nHotItem - nMaxedOffset);
		if (IsMDIChildMaxed() && (0 == m_nHotItem) )
			m_hPopupMenu = ::GetSystemMenu(hMaxMDIChild, FALSE);

        // Retrieve the bounding rectangle for the toolbar button
		RECT rc = GetItemRect(m_nHotItem);

		// convert rectangle to desktop coordinates
		::MapWindowPoints(m_hWnd, HWND_DESKTOP, (LPPOINT)&rc, 2);

		// Position popup above toolbar if it won't fit below
		TPMPARAMS tpm;
		tpm.cbSize = sizeof(TPMPARAMS);
		tpm.rcExclude = rc;

		// Set the hot button
		::SendMessage(m_hWnd, TB_SETHOTITEM, m_nHotItem, 0);
		::SendMessage(m_hWnd, TB_PRESSBUTTON, m_nHotItem, MAKELONG(TRUE, 0));

		m_bSelPopup = FALSE;
		m_hSelMenu = NULL;
		m_bMenuActive = TRUE;

		// We hook mouse input to process mouse and keyboard input during
		//  the popup menu. Messages are sent to StaticMsgHook.

		// Remove any remaining hook first
		TLSData* pTLSData = (TLSData*)::TlsGetValue(GetApp()->GetTlsIndex());
		pTLSData->pMenubar = this;
		if (pTLSData->hMenuHook != NULL)
			::UnhookWindowsHookEx(pTLSData->hMenuHook);

		// Hook messages about to be processed by the shortcut menu
		pTLSData->hMenuHook = ::SetWindowsHookEx(WH_MSGFILTER, (HOOKPROC)StaticMsgHook, NULL, ::GetCurrentThreadId());

		// Display the shortcut menu
		UINT nID = ::TrackPopupMenuEx(m_hPopupMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
			rc.left, rc.bottom, m_hWnd, &tpm);

		// We get here once the TrackPopupMenuEx has ended
		m_bMenuActive = FALSE;

		// Remove the message hook
		::UnhookWindowsHookEx(pTLSData->hMenuHook);
		pTLSData->hMenuHook = NULL;

		// Process MDI Child system menu
		if (IsMDIChildMaxed())
		{
			if (::GetSystemMenu(hMaxMDIChild, FALSE) == m_hPopupMenu )
			{
				if (nID)
					::SendMessage(hMaxMDIChild, WM_SYSCOMMAND, nID, 0);
			}
		}

		// Resestablish Focus
		if (m_bKeyMode)
			GrabFocus();
	}

	void CMenubar::DrawAllMDIButtons(CDC& DrawDC)
	{
		if (!IsMDIFrame())
			return;

		if (IsMDIChildMaxed())
		{
			int cx = GetSystemMetrics(SM_CXSMICON);
			int cy = GetSystemMetrics(SM_CYSMICON);
			RECT rc;
			::GetClientRect(m_hWnd, &rc);

			// Assign values to each element of the RECT array
			for (int i = 0 ; i < 3 ; i++)
			{
				int left = rc.right - (i+1)*cx - 4*(i+1);
				int top = rc.bottom/2 - cy/2;
				int right = rc.right - i*cx - 4*(i+1);
				int bottom = rc.bottom/2 + cy/2;
				::SetRect(&m_MDIRect[2 - i], left, top, right, bottom);
			}

			// Hide the MDI button if it won't fit
			for (int k = 0 ; k <= 2 ; k++)
			{

				if (m_MDIRect[k].left < GetMaxSize().cx)
				{
					::SetRectEmpty(&m_MDIRect[k]);
				}
			}

			DrawMDIButton(DrawDC, MDI_MIN, 0);
			DrawMDIButton(DrawDC, MDI_RESTORE, 0);
			DrawMDIButton(DrawDC, MDI_CLOSE, 0);
		}
	}

	void CMenubar::DrawMDIButton(CDC& DrawDC, int iButton, UINT uState)
	{
		if (!IsRectEmpty(&m_MDIRect[iButton]))
		{
			switch (uState)
			{
			case 0:
				{
					// Draw a grey outline
					DrawDC.CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNFACE));
					::MoveToEx(DrawDC, m_MDIRect[iButton].left, m_MDIRect[iButton].bottom, NULL);
					::LineTo(DrawDC, m_MDIRect[iButton].right, m_MDIRect[iButton].bottom);
					::LineTo(DrawDC, m_MDIRect[iButton].right, m_MDIRect[iButton].top);
					::LineTo(DrawDC, m_MDIRect[iButton].left, m_MDIRect[iButton].top);
					::LineTo(DrawDC, m_MDIRect[iButton].left, m_MDIRect[iButton].bottom);
				}
				break;
			case 1:
				{
					// Draw outline, white at top, black on bottom
					DrawDC.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					::MoveToEx(DrawDC, m_MDIRect[iButton].left, m_MDIRect[iButton].bottom, NULL);
					::LineTo(DrawDC, m_MDIRect[iButton].right, m_MDIRect[iButton].bottom);
					::LineTo(DrawDC, m_MDIRect[iButton].right, m_MDIRect[iButton].top);
					DrawDC.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
					::LineTo(DrawDC, m_MDIRect[iButton].left, m_MDIRect[iButton].top);
					::LineTo(DrawDC, m_MDIRect[iButton].left, m_MDIRect[iButton].bottom);
				}

				break;
			case 2:
				{
					// Draw outline, black on top, white on bottom
					DrawDC.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
					::MoveToEx(DrawDC, m_MDIRect[iButton].left, m_MDIRect[iButton].bottom, NULL);
					::LineTo(DrawDC, m_MDIRect[iButton].right, m_MDIRect[iButton].bottom);
					::LineTo(DrawDC, m_MDIRect[iButton].right, m_MDIRect[iButton].top);
					DrawDC.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					::LineTo(DrawDC, m_MDIRect[iButton].left, m_MDIRect[iButton].top);
					::LineTo(DrawDC, m_MDIRect[iButton].left, m_MDIRect[iButton].bottom);
				}
				break;
			}

			DrawDC.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

			switch (iButton)
			{
			case MDI_MIN:
				// Manually Draw Minimise button
				::MoveToEx(DrawDC, m_MDIRect[0].left + 4, m_MDIRect[0].bottom -4, NULL);
				::LineTo(DrawDC, m_MDIRect[0].right - 4, m_MDIRect[0].bottom - 4);

				::MoveToEx(DrawDC, m_MDIRect[0].left + 4, m_MDIRect[0].bottom -5, NULL);
				::LineTo(DrawDC, m_MDIRect[0].right - 4, m_MDIRect[0].bottom - 5);
				break;
			case MDI_RESTORE:
				// Manually Draw Restore Button
				::MoveToEx(DrawDC, m_MDIRect[1].left + 3, m_MDIRect[1].top + 7, NULL);
				::LineTo(DrawDC, m_MDIRect[1].left + 3, m_MDIRect[1].bottom -4);
				::LineTo(DrawDC, m_MDIRect[1].right - 6, m_MDIRect[1].bottom -4);
				::LineTo(DrawDC, m_MDIRect[1].right - 6, m_MDIRect[1].top + 7);
				::LineTo(DrawDC, m_MDIRect[1].left + 3, m_MDIRect[1].top + 7);

				::MoveToEx(DrawDC, m_MDIRect[1].left + 3, m_MDIRect[1].top + 8, NULL);
				::LineTo(DrawDC, m_MDIRect[1].right - 6, m_MDIRect[1].top + 8);

				::MoveToEx(DrawDC, m_MDIRect[1].left + 5, m_MDIRect[1].top + 7, NULL);
				::LineTo(DrawDC, m_MDIRect[1].left + 5, m_MDIRect[1].top + 4);
				::LineTo(DrawDC, m_MDIRect[1].right - 4, m_MDIRect[1].top + 4);
				::LineTo(DrawDC, m_MDIRect[1].right - 4, m_MDIRect[1].bottom -6);
				::LineTo(DrawDC, m_MDIRect[1].right - 6, m_MDIRect[1].bottom -6);

				::MoveToEx(DrawDC, m_MDIRect[1].left + 5, m_MDIRect[1].top + 5, NULL);
				::LineTo(DrawDC, m_MDIRect[1].right - 4, m_MDIRect[1].top + 5);
				break;
			case MDI_CLOSE:
				// Manually Draw Close Button
				::MoveToEx(DrawDC, m_MDIRect[2].left + 4, m_MDIRect[2].top +5, NULL);
				::LineTo(DrawDC, m_MDIRect[2].right - 4, m_MDIRect[2].bottom -3);

				::MoveToEx(DrawDC, m_MDIRect[2].left + 5, m_MDIRect[2].top +5, NULL);
				::LineTo(DrawDC, m_MDIRect[2].right - 4, m_MDIRect[2].bottom -4);

				::MoveToEx(DrawDC, m_MDIRect[2].left + 4, m_MDIRect[2].top +6, NULL);
				::LineTo(DrawDC, m_MDIRect[2].right - 5, m_MDIRect[2].bottom -3);

				::MoveToEx(DrawDC, m_MDIRect[2].right -5, m_MDIRect[2].top +5, NULL);
				::LineTo(DrawDC, m_MDIRect[2].left + 3, m_MDIRect[2].bottom -3);

				::MoveToEx(DrawDC, m_MDIRect[2].right -5, m_MDIRect[2].top +6, NULL);
				::LineTo(DrawDC, m_MDIRect[2].left + 4, m_MDIRect[2].bottom -3);

				::MoveToEx(DrawDC, m_MDIRect[2].right -6, m_MDIRect[2].top +5, NULL);
				::LineTo(DrawDC, m_MDIRect[2].left + 3, m_MDIRect[2].bottom -4);
				break;
			}
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

	HWND CMenubar::GetActiveMDIChild()
	{
		HWND hwndMDIChild = NULL;
		if (IsMDIFrame())
		{
			hwndMDIChild = (HWND)::SendMessage(m_pFrame->GetView()->GetHwnd(), WM_MDIGETACTIVE, 0, 0);
		}

		return hwndMDIChild;
	}

	void CMenubar::GrabFocus()
	{
		if (::GetFocus() != m_hWnd)
			m_hPrevFocus = ::SetFocus(m_hWnd);
		::SetCapture(m_hWnd);
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}

	BOOL CMenubar::IsMDIChildMaxed() const
	{
		BOOL bMaxed = FALSE;

		if (IsMDIFrame())
		{
			::SendMessage(m_pFrame->GetView()->GetHwnd(), WM_MDIGETACTIVE, 0, (LPARAM)&bMaxed);
		}

		return bMaxed;
	}

	BOOL CMenubar::IsMDIFrame() const
	{
		return m_pFrame->IsMDIFrame();
	}

	void CMenubar::MenuChar(WPARAM wParam, LPARAM /* lParam */)
	{
		if (!m_bMenuActive)
			DoAltKey(LOWORD(wParam));
	}

	void CMenubar::OnCreate()
	{
		// We must send this message before sending the TB_ADDBITMAP or TB_ADDBUTTONS message
		::SendMessage(m_hWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

		m_pFrame = (CFrame*)FromHandle(GetAncestor(m_hWnd));
	}

	LRESULT CMenubar::OnCustomDraw(NMHDR* pNMHDR)
	// CustomDraw is used to render the Menubar's toolbar buttons
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
				RECT rcRect = lpNMCustomDraw->nmcd.rc;
				int nState = lpNMCustomDraw->nmcd.uItemState;
				DWORD dwItem = (DWORD)lpNMCustomDraw->nmcd.dwItemSpec;

				// Leave a 2 pixel gap above the drawn rectangle
				rcRect.top = 2;

				if (IsMDIChildMaxed() && (0 == dwItem))
				// Draw over MDI Max button
				{
					HICON hIcon = (HICON)::SendMessage(GetActiveMDIChild(), WM_GETICON, ICON_SMALL, 0);
					if (NULL == hIcon)
						hIcon = ::LoadIcon(NULL, IDI_APPLICATION);

					int cx = ::GetSystemMetrics (SM_CXSMICON);
					int cy = ::GetSystemMetrics (SM_CYSMICON);
					int y = 1 + (rcRect.bottom - rcRect.top - cy)/2;
					int x = 0;
					::DrawIconEx(DrawDC, x, y, hIcon, cx, cy, 0, NULL, DI_NORMAL);

					// Detach the DC so it doesn't get destroyed
					DrawDC.DetachDC();
					return CDRF_SKIPDEFAULT;  // No further drawing
				}

				else if (nState & (CDIS_HOT | CDIS_SELECTED))
				{
					if (m_ThemeMenu.UseThemes)
					{
						if ((nState & CDIS_SELECTED) || (GetButtonState(dwItem) & TBSTATE_PRESSED))
						{
							GradientFill(DrawDC, m_ThemeMenu.clrPressed1, m_ThemeMenu.clrPressed2, &rcRect, FALSE);
						}
						else if (nState & CDIS_HOT)
						{
							GradientFill(DrawDC, m_ThemeMenu.clrHot1, m_ThemeMenu.clrHot2, &rcRect, FALSE);
						}

						// Draw border
						DrawDC.CreatePen(PS_SOLID, 1, m_ThemeMenu.clrOutline);
						::MoveToEx(DrawDC, rcRect.left, rcRect.bottom -1, NULL);
						::LineTo(DrawDC, rcRect.left, rcRect.top);
						::LineTo(DrawDC, rcRect.right-1, rcRect.top);
						::LineTo(DrawDC, rcRect.right-1, rcRect.bottom);
						if (!(nState & CDIS_SELECTED))
						{
							::MoveToEx(DrawDC, rcRect.right-1, rcRect.bottom-1, NULL);
							::LineTo(DrawDC, rcRect.left, rcRect.bottom-1);
						}
					}
					else
					{
						// Draw highlight rectangle
						DrawDC.CreatePen(PS_SOLID, 1, m_ThemeMenu.clrOutline);
						HBRUSH hbHighlight = ::GetSysColorBrush(COLOR_HIGHLIGHT);
						::FillRect(DrawDC, &rcRect, hbHighlight);
					}

					TCHAR str[80] = _T("");
					int nLength = (int)::SendMessage(m_hWnd, TB_GETBUTTONTEXT, lpNMCustomDraw->nmcd.dwItemSpec, (LPARAM) NULL);
					if ((nLength > 0) && (nLength < 80))
						::SendMessage(m_hWnd, TB_GETBUTTONTEXT, lpNMCustomDraw->nmcd.dwItemSpec, (LPARAM)str);

					// Draw highlight text
					DrawDC.AttachFont((HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0, 0));
					if (!m_ThemeMenu.UseThemes)
						::SetTextColor(DrawDC, ::GetSysColor(COLOR_HIGHLIGHTTEXT));

					rcRect.bottom += 1;
					int iMode = ::SetBkMode(DrawDC, TRANSPARENT);
					::DrawText(DrawDC, str, lstrlen(str), &rcRect, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

					::SetBkMode(DrawDC, iMode);
					DrawDC.DetachFont();
					DrawDC.DetachDC();
					return CDRF_SKIPDEFAULT;  // No further drawing
				}
				// Detach the DC so it doesn't get destroyed
				DrawDC.DetachDC();
			}
			return CDRF_DODEFAULT ;   // Do default drawing

		// Painting cycle has completed
		case CDDS_POSTPAINT:
			// Draw MDI Minimise, Restore and Close buttons
			{
				CDC DrawDC = lpNMCustomDraw->nmcd.hdc;
				DrawAllMDIButtons(DrawDC);
				// Detach the DC so it doesn't get destroyed
				DrawDC.DetachDC();
			}
			break;
		}
		return 0L;
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
			::PostMessage(GetAncestor(m_hWnd), WM_SYSCOMMAND, SC_KEYMENU, VK_SPACE);
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
			(m_nHotItem > 0)? SetHotItem(m_nHotItem -1) : SetHotItem(GetButtonCount()-1);
			break;

		case VK_RIGHT:
			// Move right to next topmenu item
			(m_nHotItem < GetButtonCount() -1)? SetHotItem(m_nHotItem +1) : SetHotItem(0);
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

		if (IsMDIFrame())
		{
			if (IsMDIChildMaxed())
			{
				CDC MenubarDC = ::GetDC(m_hWnd);
				m_nMDIButton = -1;

				if (PtInRect(&m_MDIRect[0], pt)) m_nMDIButton = 0;
				if (PtInRect(&m_MDIRect[1], pt)) m_nMDIButton = 1;
				if (PtInRect(&m_MDIRect[2], pt)) m_nMDIButton = 2;

				if (m_nMDIButton >= 0)
				{
					DrawMDIButton(MenubarDC, MDI_MIN,     (0 == m_nMDIButton)? 2 : 0);
					DrawMDIButton(MenubarDC, MDI_RESTORE, (1 == m_nMDIButton)? 2 : 0);
					DrawMDIButton(MenubarDC, MDI_CLOSE,   (2 == m_nMDIButton)? 2 : 0);
				}

				// Bring up the MDI Child window's system menu when the icon is pressed
				if (0 == HitTest())
				{
					m_nHotItem = 0;
					::PostMessage(m_hWnd, USER_POPUPMENU, 0, 0);
				}
			}
		}
	}

	void CMenubar::OnLButtonUp(WPARAM /*wParam*/, LPARAM lParam)
	{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		if (IsMDIFrame())
		{
			HWND MDIClient = m_pFrame->GetView()->GetHwnd();
			HWND MDIChild = GetActiveMDIChild();

			if (IsMDIChildMaxed())
			{
				POINT pt = {0};
				::GetCursorPos(&pt);
				::ScreenToClient(m_hWnd, &pt);

				// Process the MDI button action when the left mouse button is up
				if (PtInRect(&m_MDIRect[0], pt))
				{
					if (MDI_MIN == m_nMDIButton)
						::ShowWindow(MDIChild, SW_MINIMIZE);
				}

				if (PtInRect(&m_MDIRect[1], pt))
				{
					if (MDI_RESTORE == m_nMDIButton)
					::PostMessage(MDIClient, WM_MDIRESTORE, (WPARAM)MDIChild, 0);
				}

				if (PtInRect(&m_MDIRect[2], pt))
				{
					if (MDI_CLOSE == m_nMDIButton)
						::PostMessage(MDIChild, WM_CLOSE, 0, 0);
				}
			}
		}
		m_nMDIButton = 0;
		ExitMenu();
	}

	BOOL CMenubar::OnMenuInput(UINT uMsg, WPARAM wParam, LPARAM lParam)
	// When a popup menu is active, StaticMsgHook directs all menu messages here
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
					(m_nHotItem > 0)? m_nHotItem-- : m_nHotItem = GetButtonCount()-1;
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
					(m_nHotItem < GetButtonCount()-1)? m_nHotItem++ : m_nHotItem = 0;
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

		case WM_LBUTTONDBLCLK:
			// Perform default action for DblClick on MDI Maxed icon
			if (IsMDIChildMaxed() && (0 == HitTest()))
			{
				HWND MDIChild = GetActiveMDIChild();
				HMENU hChildMenu = ::GetSystemMenu(MDIChild, FALSE);

				UINT nID = ::GetMenuDefaultItem(hChildMenu, FALSE, 0);
				if (nID)
					::PostMessage(MDIChild, WM_SYSCOMMAND, nID, 0);
			}

			m_bExitAfter = TRUE;
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

	void CMenubar::OnMouseLeave()
	{
		if (IsMDIFrame())
		{
			if (IsMDIChildMaxed())
			{
				CDC MenubarDC = ::GetDC(m_hWnd);

				DrawMDIButton(MenubarDC, MDI_MIN,     0);
				DrawMDIButton(MenubarDC, MDI_RESTORE, 0);
				DrawMDIButton(MenubarDC, MDI_CLOSE,   0);
			}
		}
	}

	void CMenubar::OnMouseMove(WPARAM wParam, LPARAM lParam)
	{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		if (IsMDIFrame())
		{
			if (IsMDIChildMaxed())
			{
				CDC MenubarDC = ::GetDC(m_hWnd);
				int MDIButton = -1;
				if (PtInRect(&m_MDIRect[0], pt)) MDIButton = 0;
				if (PtInRect(&m_MDIRect[1], pt)) MDIButton = 1;
				if (PtInRect(&m_MDIRect[2], pt)) MDIButton = 2;

				if (MK_LBUTTON == wParam)  // mouse moved with left mouse button is held down
				{
					// toggle the MDI button image pressed/unpressed as required
					if (MDIButton >= 0)
					{
						DrawMDIButton(MenubarDC, MDI_MIN,     ((0 == MDIButton) && (0 == m_nMDIButton))? 2 : 0);
						DrawMDIButton(MenubarDC, MDI_RESTORE, ((1 == MDIButton) && (1 == m_nMDIButton))? 2 : 0);
						DrawMDIButton(MenubarDC, MDI_CLOSE,   ((2 == MDIButton) && (2 == m_nMDIButton))? 2 : 0);
					}
					else
					{
						DrawMDIButton(MenubarDC, MDI_MIN,     0);
						DrawMDIButton(MenubarDC, MDI_RESTORE, 0);
						DrawMDIButton(MenubarDC, MDI_CLOSE,   0);
					}
				}
				else	// mouse moved without left mouse button held down
				{
					if (MDIButton >= 0)
					{
						DrawMDIButton(MenubarDC, MDI_MIN,     (0 == MDIButton)? 1 : 0);
						DrawMDIButton(MenubarDC, MDI_RESTORE, (1 == MDIButton)? 1 : 0);
						DrawMDIButton(MenubarDC, MDI_CLOSE,   (2 == MDIButton)? 1 : 0);
					}
					else
					{
						DrawMDIButton(MenubarDC, MDI_MIN,     0);
						DrawMDIButton(MenubarDC, MDI_RESTORE, 0);
						DrawMDIButton(MenubarDC, MDI_CLOSE,   0);
					}
				}
			}
		}
	}

	LRESULT CMenubar::OnNotifyReflect(WPARAM /* wParam */, LPARAM lParam)
	{
		switch (((LPNMHDR)lParam)->code)
		{
		case NM_CUSTOMDRAW:
			{
				return OnCustomDraw((LPNMHDR) lParam);
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
				if ((flag & HICF_MOUSE) && !(flag & HICF_LEAVING))
				{
					int nButton = HitTest();
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

				// Handle escape from popup menu
				if ((flag & HICF_LEAVING) && m_bKeyMode)
				{
					m_nHotItem = ((LPNMTBHOTITEM)lParam)->idOld;
					::PostMessage(m_hWnd, TB_SETHOTITEM, m_nHotItem, 0);
				}

				break;
			} //case TBN_HOTITEMCHANGE:

		} // switch(((LPNMHDR)lParam)->code)
		return 0L;
	} // CMenubar::OnNotify(...)

	void CMenubar::OnWindowPosChanged()
	{
		::InvalidateRect(m_hWnd, &m_MDIRect[0], TRUE);
		::InvalidateRect(m_hWnd, &m_MDIRect[1], TRUE);
		::InvalidateRect(m_hWnd, &m_MDIRect[2], TRUE);
		{
			CDC MenubarDC = ::GetDC(m_hWnd);
			DrawAllMDIButtons(MenubarDC);
		}
	}

	void CMenubar::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | CCS_NODIVIDER | CCS_NORESIZE;
		cs.lpszClass = TOOLBARCLASSNAME;
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
		int nMaxedOffset = (IsMDIChildMaxed()? 1:0);

		// Remove any existing buttons
		while (::SendMessage(m_hWnd, TB_BUTTONCOUNT,  0, 0) > 0)
		{
			if(!::SendMessage(m_hWnd, TB_DELETEBUTTON, 0, 0))
				break;
		}

		// Set the Bitmap size to zero
		::SendMessage(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(0, 0));

		if (IsMDIChildMaxed())
		{
			// Create an extra button for the MDI child system menu
			// Later we will custom draw the window icon over this button
			TBBUTTON tbb = {0};
			tbb.fsState = TBSTATE_ENABLED;
			tbb.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE ;
			tbb.iString = (INT_PTR)_T(" ");
			if(!::SendMessage(m_hWnd, TB_ADDBUTTONS, 1, (WPARAM)&tbb))
				throw CWinException(_T("Menubar::SetMenu  TB_ADDBUTTONS failed"));

			SetButtonText(0, _T("    "));
		}

		for (int i = 0 ; i < ::GetMenuItemCount(hMenu); i++)
		{
			// Assign the Toolbar Button struct
			TBBUTTON tbb = {0};
			tbb.idCommand = i  + nMaxedOffset;	// Each button needs a unique ID
			tbb.fsState = TBSTATE_ENABLED;
			tbb.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE | TBSTYLE_DROPDOWN;
			tbb.iString = (INT_PTR)_T(" ");
			if (!::SendMessage(m_hWnd, TB_ADDBUTTONS, 1, (WPARAM)&tbb))
				throw CWinException(_T("Menubar::SetMenu  TB_ADDBUTTONS failed"));

			// Add the menu title to the string table
			TCHAR szMenuName[MAX_MENU_STRING +1] = _T("");

			if (0 == ::GetMenuString(hMenu, i, szMenuName, MAX_MENU_STRING, MF_BYPOSITION) )
				throw CWinException(TEXT("Menubar::SetMenu  GetMenuString failed"));

			SetButtonText(i  + nMaxedOffset, szMenuName);
		}

	}

	void CMenubar::SetMenubarTheme(ThemeMenu& Theme)
	{
		m_ThemeMenu.UseThemes   = Theme.UseThemes;
		m_ThemeMenu.clrHot1     = Theme.clrHot1;
		m_ThemeMenu.clrHot2     = Theme.clrHot2;
		m_ThemeMenu.clrPressed1 = Theme.clrPressed1;
		m_ThemeMenu.clrPressed2 = Theme.clrPressed2;
		m_ThemeMenu.clrOutline  = Theme.clrOutline;

		::InvalidateRect(m_hWnd, NULL, TRUE);
	}

	LRESULT CALLBACK CMenubar::StaticMsgHook(int nCode, WPARAM wParam, LPARAM lParam)
	{
		MSG* pMsg = (MSG*)lParam;
		TLSData* pTLSData = (TLSData*)TlsGetValue(GetApp()->GetTlsIndex());
		CMenubar* pMenubar = (CMenubar*)pTLSData->pMenubar;

		if (pMenubar && (MSGF_MENU == nCode))
		{
			// process menu message
			if (pMenubar->OnMenuInput(pMsg->message, pMsg->wParam, pMsg->lParam))
			{
				return TRUE;
			}
		}

		return CallNextHookEx(pTLSData->hMenuHook, nCode, wParam, lParam);
	}

	void CMenubar::SysCommand(WPARAM wParam, LPARAM lParam)
	{
		if (SC_KEYMENU == wParam)
		{
			if (0 == lParam)
			{
				// Alt/F10 key toggled
				GrabFocus();
				m_bKeyMode = TRUE;
				int nMaxedOffset = (IsMDIChildMaxed()? 1:0);
				SetHotItem(nMaxedOffset);
			}
			else
				// Handle key pressed with Alt held down
				DoAltKey((WORD)lParam);
		}
	}

	LRESULT CMenubar::WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CHAR:
			return 0L;  // Discard these messages
		case WM_DRAWITEM:
			m_pFrame->OnDrawItem(wParam, lParam);
			return TRUE; // handled
		case WM_EXITMENULOOP:
			if (m_bExitAfter)
				ExitMenu();
			m_pFrame->OnExitMenuLoop();
			break;
		case WM_INITMENUPOPUP:
			m_pFrame->OnInitMenuPopup(wParam, lParam);
			break;
		case WM_KEYDOWN:
			OnKeyDown(wParam, lParam);
			return 0L;	// Discard these messages
		case WM_KILLFOCUS:
			ExitMenu();
			return 0L;
		case WM_LBUTTONDBLCLK:
			// Convert double left click to single left click
			::mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			return 0L;	// Discard these messages
		case WM_LBUTTONDOWN:
			// Do default processing first
			CallPrevWindowProc(hWnd, uMsg, wParam, lParam);

			OnLButtonDown(wParam, lParam);
			return 0L;
		case WM_LBUTTONUP:
			OnLButtonUp(wParam, lParam);
			break;
		case WM_MEASUREITEM:
			m_pFrame->OnMeasureItem(wParam, lParam);
			return TRUE; // handled
		case WM_MOUSELEAVE:
			OnMouseLeave();
			break;
		case WM_MOUSEMOVE:
			OnMouseMove(wParam, lParam);
			break;
		case USER_POPUPMENU:
			DoPopupMenu();
			return 0L;
		case WM_SYSKEYDOWN:
			if ((VK_MENU == wParam) || (VK_F10 == wParam))
				return 0L;
			break;
		case WM_SYSKEYUP:
			if ((VK_MENU == wParam) || (VK_F10 == wParam))
			{
				ExitMenu();
				return 0L;
			}
			break;
		case WM_WINDOWPOSCHANGED:
			OnWindowPosChanged();
			break;
		case WM_WINDOWPOSCHANGING:
			// Bypass CToolbar::WndProcDefault for this message
			return CWnd::WndProcDefault(hWnd, uMsg, wParam, lParam);

		} // switch (uMsg)

		return CToolbar::WndProcDefault(hWnd, uMsg, wParam, lParam);
	} // LRESULT CMenubar::WndProc(...)



	///////////////////////////////////
	// Definitions for the CFrame class
	//
	CFrame::CFrame() :  m_bIsMDIFrame(FALSE), m_bShowIndicatorStatus(TRUE), m_bShowMenuStatus(TRUE),
		                m_bUseRebar(FALSE), m_bUseThemes(TRUE), m_bUpdateTheme(FALSE), m_StatusText(_T("Ready")),
						m_hImageList(NULL), m_hImageListDis(NULL), m_pAboutDialog(NULL), m_hMenu(NULL), m_pView(NULL)
	{

		ZeroMemory(&m_ThemeMenu, sizeof(m_ThemeMenu));
		GetApp()->SetFrame(this);

		INITCOMMONCONTROLSEX InitStruct;
		InitStruct.dwSize = sizeof(INITCOMMONCONTROLSEX);
		InitStruct.dwICC = ICC_WIN95_CLASSES| ICC_COOL_CLASSES;

		// Do either InitCommonControls or InitCommonControlsEx
		LoadCommonControls(InitStruct);

		// By default, we use the rebar if we can
		if (GetComCtlVersion() >= 470)
			m_bUseRebar = TRUE;

		for (int i = 0 ; i < 3 ; i++)
			m_OldStatus[i] = _T('\0');

		// Place this code in CMainFrame's constructor
/*
		// Define the resource IDs for the toolbar
		m_ToolbarData.clear();
		m_ToolbarData.push_back ( IDM_FILE_NEW   );
		m_ToolbarData.push_back ( IDM_FILE_OPEN  );
		m_ToolbarData.push_back ( IDM_FILE_SAVE  );
		m_ToolbarData.push_back ( 0 );				// Separator
		m_ToolbarData.push_back ( IDM_EDIT_CUT   );
		m_ToolbarData.push_back ( IDM_EDIT_COPY  );
		m_ToolbarData.push_back ( IDM_EDIT_PASTE );
		m_ToolbarData.push_back ( 0 );				// Separator
		m_ToolbarData.push_back ( IDM_FILE_PRINT );
		m_ToolbarData.push_back ( 0 );				// Separator
		m_ToolbarData.push_back ( IDM_HELP_ABOUT );
*/
	}

	CFrame::~CFrame()
	{
		for (int nItem = 0; nItem < (int)m_vpItemData.size(); nItem++)
		{
			// These are normally deleted in OnExitMenuLoop
			delete m_vpItemData[nItem];
		}

		if (m_hMenu)
			::DestroyMenu(m_hMenu);
	}

	BOOL CFrame::AddMenuIcon(int nID_MenuItem, HICON hIcon, int cx /*= 16*/, int cy /*= 16*/)
	{
		// Get ImageList image size
		int cxOld;
		int cyOld;
		ImageList_GetIconSize(m_hImageList, &cxOld, &cyOld );

		// Create a new ImageList if required
		if ((cx != cxOld) || (cy != cyOld) || (NULL == m_hImageList))
		{
			if (m_hImageList) ImageList_Destroy(m_hImageList);
			m_hImageList = ImageList_Create(cx, cy, ILC_COLOR32 | ILC_MASK, 1, 0);
			m_MenuData.clear();
		}

        if (ImageList_AddIcon(m_hImageList, hIcon) != -1)
		{
			m_MenuData.push_back(nID_MenuItem);

			// Recreate the Disabled imagelist
			if (m_hImageListDis) ImageList_Destroy(m_hImageListDis);
			m_hImageListDis = NULL;
			m_hImageListDis = CreateDisabledImageList(m_hImageList);

			return TRUE;
		}

		return FALSE;
	}

	int CFrame::AddMenuIcons(const std::vector<UINT>& MenuData, UINT nID_Image, COLORREF crMask)
	// Adds the icons from a bitmap resouce to an internal ImageList for use with popup menu items.
	// Note:  If existing are a different size to the new ones, the old ones will be removed!
	{
		// Count the MenuData entries excluding seperators
		int iImages = 0;
		for (unsigned int i = 0 ; i < MenuData.size(); i++)
		{
			if (MenuData[i] != 0)	// Don't count seperators
			{
				iImages++;
			}
		}

		// Load the button images from Resouce ID
		HBITMAP hbm = LoadBitmap(MAKEINTRESOURCE(nID_Image));

		if ((0 == iImages) || (NULL == hbm))
			return (int)m_MenuData.size();	// No valid images, so nothing to do!

		BITMAP bm = {0};
		::GetObject(hbm, sizeof(BITMAP), &bm);
		int iImageWidth  = bm.bmWidth / iImages;
		int iImageHeight = bm.bmHeight;

		// Create the ImageList if required
		if (NULL == m_hImageList)
		{
			m_hImageList = ImageList_Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, iImages, 0);
			m_MenuData.clear();
		}
		else
		{
			int Oldcx;
			int Oldcy;

			ImageList_GetIconSize(m_hImageList, &Oldcx, &Oldcy);
			if ((iImageWidth != Oldcx) || (iImageHeight != Oldcy))
			{
				// The new icons are a different size to the old ones in m_hImageList,
				//  so we throw the old ones away.
				ImageList_Destroy(m_hImageList);
				m_hImageList = ImageList_Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, iImages, 0);
				m_MenuData.clear();
			}
		}

		// Add the resource IDs to the m_MenuData vector
		for (unsigned int j = 0 ; j < MenuData.size(); j++)
		{
			if (MenuData[j] != 0)
			{
				m_MenuData.push_back(MenuData[j]);
			}
		}

		// Add the images to the ImageList
		ImageList_AddMasked(m_hImageList, hbm, crMask);
		::DeleteObject(hbm);

		// Create the Disabled imagelist
		if (m_hImageListDis) ImageList_Destroy(m_hImageListDis);
		m_hImageListDis = NULL;
		m_hImageListDis = CreateDisabledImageList(m_hImageList);

		// return the number of menu icons
		return (int)m_MenuData.size();
	}

	void CFrame::AddMenubarBand(int Menubar_Height /* = MENUBAR_HEIGHT*/)
	{
		try
		{
			// Adds a Menubar to the rebar control
   			REBARBANDINFO rbbi = {0};
			SIZE sz = GetMenubar().GetMaxSize();

			rbbi.cbSize     = sizeof(REBARBANDINFO);
			rbbi.fMask      = RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_CHILD | RBBIM_SIZE;
			rbbi.cxMinChild = sz.cx;
			rbbi.cx         = sz.cx;
			rbbi.cyMinChild = Menubar_Height;
			rbbi.cyMaxChild = Menubar_Height;
			rbbi.fStyle     = RBBS_BREAK | RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS ;
			rbbi.hwndChild  = GetMenubar();

			GetRebar().InsertBand(-1, &rbbi);
			SetMenubarBandSize();
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}
	}

	void CFrame::AddToolbarBand(CToolbar& TB, const std::vector<UINT>& TBData, COLORREF clrMask, UINT ID_Normal, UINT ID_HOT, UINT ID_Disabled)
	{
		// Adds a Toolbar to the rebar control

		if (0 == TBData.size())
		{
			DebugErrMsg(_T("Toolbar must have some data"));
			return;
		}

		try
		{
			// Create the Toolbar Window
			TB.Create(GetRebar());
			int nButtons = TB.SetButtons(TBData);
			TB.SetImageList(nButtons, clrMask, ID_Normal, ID_HOT, ID_Disabled);

			// Fill the REBARBAND structure
			REBARBANDINFO rbbi = {0};
			SIZE sz = TB.GetMaxSize();

			rbbi.cbSize     = sizeof(REBARBANDINFO);
			rbbi.fMask      = RBBIM_CHILDSIZE | RBBIM_STYLE |  RBBIM_CHILD | RBBIM_SIZE;
			rbbi.cyMinChild = sz.cy;
			rbbi.cyMaxChild = sz.cy;
			rbbi.cx         = sz.cx +2;
			rbbi.cxMinChild = sz.cx +2;

			rbbi.fStyle     = RBBS_BREAK | RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS;
			rbbi.hwndChild  = TB;

			GetRebar().InsertBand(-1, &rbbi);
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}
	}



	void CFrame::DrawCheckmark(LPDRAWITEMSTRUCT pdis)
	// Draws the checkmark or radiocheck transparently
	{
		CDC DrawDC = pdis->hDC;
		RECT rc = pdis->rcItem;
		UINT fType = ((ItemData*)pdis->itemData)->fType;

		// Draw the checkmark's background rectangle first
		if (m_ThemeMenu.UseThemes)
		{
			int Iconx, Icony;
			ImageList_GetIconSize(m_hImageList, &Iconx, &Icony);
			int offset = -1 + (rc.bottom - rc.top - Icony)/2;
			int height = rc.bottom - rc.top;
			RECT rcBk;
			::SetRect(&rcBk, rc.left, rc.top, rc.left + height, rc.bottom);
			::InflateRect(&rcBk, -offset, -offset);
			DrawDC.CreateSolidBrush(m_ThemeMenu.clrHot2);
			DrawDC.CreatePen(PS_SOLID, 1, m_ThemeMenu.clrOutline);

			// Draw the checkmark's background rectangle
			::Rectangle(DrawDC, rcBk.left, rcBk.top, rcBk.right, rcBk.bottom);
		}

		CDC MemDC = ::CreateCompatibleDC(pdis->hDC);
		int cxCheck = ::GetSystemMetrics(SM_CXMENUCHECK);
		int cyCheck = ::GetSystemMetrics(SM_CYMENUCHECK);
		MemDC.CreateBitmap(cxCheck, cyCheck, 1, 1, NULL);
		RECT rCheck = { 0, 0, cxCheck, cyCheck };

		// Copy the check mark bitmap to hdcMem
		if (MFT_RADIOCHECK == fType)
			::DrawFrameControl(MemDC, &rCheck, DFC_MENU, DFCS_MENUBULLET);
		else
			::DrawFrameControl(MemDC, &rCheck, DFC_MENU, DFCS_MENUCHECK);

		int offset = (rc.bottom - rc.top - ::GetSystemMetrics(SM_CXMENUCHECK))/2;
		if (m_ThemeMenu.UseThemes)
			rc.left += 2;

		// Draw a white or black check mark as required
		// Unfortunately MaskBlt isn't supported on Win95, 98 or ME, so we do it the hard way
		CDC MaskDC = ::CreateCompatibleDC(pdis->hDC);
		MaskDC.CreateCompatibleBitmap(pdis->hDC, cxCheck, cyCheck);

		::BitBlt(MaskDC, 0, 0, cxCheck, cyCheck, MaskDC, 0, 0, WHITENESS);
		if ((pdis->itemState & ODS_SELECTED) && (!m_ThemeMenu.UseThemes))
		{
			// Draw a white checkmark
			::BitBlt(MemDC, 0, 0, cxCheck, cyCheck, MemDC, 0, 0, DSTINVERT);
			::BitBlt(MaskDC, 0, 0, cxCheck, cyCheck, MemDC, 0, 0, SRCAND);
			::BitBlt(DrawDC, rc.left + offset, rc.top + offset, cxCheck, cyCheck, MaskDC, 0, 0, SRCPAINT);
		}
		else
		{
			// Draw a black checkmark
			int BullitOffset = ((MFT_RADIOCHECK == fType) && m_ThemeMenu.UseThemes)? 1 : 0;
			::BitBlt(MaskDC, -BullitOffset, BullitOffset, cxCheck, cyCheck, MemDC, 0, 0, SRCAND);
			::BitBlt(DrawDC, rc.left + offset, rc.top + offset, cxCheck, cyCheck, MaskDC, 0, 0, SRCAND);
		}
		// Detach the DC so it doesn't get destroyed
		DrawDC.DetachDC();
	}

	void CFrame::DrawMenuIcon(LPDRAWITEMSTRUCT pdis, BOOL bDisabled)
	{
		if (!m_hImageList)
			return;
		// Get icon size
		int Iconx;
		int Icony;
		ImageList_GetIconSize(m_hImageList, &Iconx, &Icony);

		// get the drawing rectangle
		CDC DrawDC = pdis->hDC;
		RECT rc = pdis->rcItem;
		int offset = (rc.bottom - rc.top - Icony)/2;
		int height = rc.bottom - rc.top;
		::SetRect(&rc, rc.left, rc.top, rc.left + height, rc.bottom);
		::InflateRect(&rc, -offset, -offset);

		// get the icon's location in the imagelist
		int iImage = -1;
		for (int i = 0 ; i < (int)m_MenuData.size(); i++)
		{
			if (pdis->itemID == m_MenuData[i])
				iImage = i;
		}

		// draw the image
		if (iImage >= 0 )
		{
			if ((bDisabled) && (m_hImageListDis))
				ImageList_Draw(m_hImageListDis, iImage, DrawDC, rc.left, rc.top, ILD_TRANSPARENT);
			else
				ImageList_Draw(m_hImageList, iImage, DrawDC, rc.left, rc.top, ILD_TRANSPARENT);
		}
		// Detach the DC so it doesn't get destroyed
		DrawDC.DetachDC();
	}

	void CFrame::DrawMenuText(HDC DrawDC, LPCTSTR ItemText, RECT rc, COLORREF colorText)
	{
		// find the position of tab character
		int nTab = -1;
		for(int i = 0; i < lstrlen(ItemText); i++)
		{
			if(_T('\t') == ItemText[i])
			{
				nTab = i;
				break;
			}
		}

		// Draw the item text
		::SetTextColor(DrawDC, colorText);
		::DrawText(DrawDC, ItemText, nTab, &rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

		// Draw text after tab, right aligned
		if(nTab != -1)
			::DrawText(DrawDC, &ItemText[nTab + 1], -1, &rc, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
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
		if (::IsWindowVisible(GetStatusbar()))
			::GetWindowRect(GetStatusbar(), &rStatus);

		// Get size of top rebar or toolbar
		if (IsRebarSupported() && m_bUseRebar)
			::GetWindowRect(GetRebar(), &rTop);
		else
			if (IsWindowVisible(GetToolbar()))
				::GetWindowRect(GetToolbar(), &rTop);

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

	int CFrame::GetMenuItemPos(HMENU hMenu, LPCTSTR szItem)
	// Returns the position of the menu item, given it's name
	{
		int nMenuItemCount = GetMenuItemCount(hMenu);
		MENUITEMINFO mii = {0};

		// For Win95 and NT, cbSize needs to be 44
		if (1400 == (GetWinVersion()) || (2400 == GetWinVersion()))
			mii.cbSize = 44;
		else
			mii.cbSize = sizeof(MENUITEMINFO);

		for (int nItem = 0 ; nItem < nMenuItemCount; nItem++)
		{
			TCHAR szStr[MAX_MENU_STRING +1] = _T("");
			TCHAR szStripped[MAX_MENU_STRING +1] = _T("");

			mii.fMask      = MIIM_TYPE;
			mii.fType      = MFT_STRING;
			mii.dwTypeData = szStr;
			mii.cch        = MAX_MENU_STRING;

			// Fill the contents of szStr from the menu item
			if (::GetMenuItemInfo(hMenu, nItem, TRUE, &mii) && (lstrlen(szStr) <= MAX_MENU_STRING))
			{
				// Strip out any & characters
				int j = 0;
				for (int i = 0; i < lstrlen(szStr); i++)
				{
					if (szStr[i] != _T('&'))
						szStripped[j++] = szStr[i];
				}
				szStripped[j] = _T('\0');	// Append null tchar

				// Compare the strings
				if (0 == lstrcmp(szStripped, szItem))
					return nItem;
			}
		}

		return -1;
	}

	void CFrame::LoadCommonControls(INITCOMMONCONTROLSEX InitStruct)
	{
		HMODULE hComCtl;

		try
		{
			// Load the Common Controls DLL
			hComCtl = ::LoadLibrary(_T("COMCTL32.DLL"));
			if (!hComCtl)
				throw CWinException(_T("CFrame::LoadCommonControls ... Failed to load COMCTL32.DLL"));

			if (GetComCtlVersion() > 470)
			{
				// Declare a pointer to the InItCommonControlsEx function
				typedef BOOL WINAPI INIT_EX(INITCOMMONCONTROLSEX*);
				INIT_EX* pfnInit = (INIT_EX*)::GetProcAddress(hComCtl, "InitCommonControlsEx");

				// Call InitCommonControlsEx
				if(!((*pfnInit)(&InitStruct)))
					throw CWinException(_T("CFrame::LoadCommonControls ... InitCommonControlsEx failed"));
			}
			else
			{
				::InitCommonControls();
			}

			::FreeLibrary(hComCtl);
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
			if (hComCtl)
				::FreeLibrary(hComCtl);
		}
	}

	BOOL CFrame::OnCommandFrame(WPARAM wParam, LPARAM /*lParam*/)
	{
		// Handle the View Statusbar and Toolbar menu items
		switch (LOWORD(wParam))
		{
		case IDW_VIEW_STATUSBAR:
			OnViewStatusbar();
			return TRUE;
		case IDW_VIEW_TOOLBAR:
			OnViewToolbar();
			return TRUE;
		} // switch cmd

		return FALSE;
	}

	void CFrame::OnCreate()
	{
		// This is called when the frame window is being created.
		// Override this in CMainFrame if you wish to modify what happens here

		// Set the icon
		SetIconLarge(IDW_MAIN);
		SetIconSmall(IDW_MAIN);

		// Set the menu
		SetFrameMenu(IDW_MAIN);

		// Set the accelerator table and HWND for translated messages
		GetApp()->SetAccelerators(IDW_MAIN, GetHwnd());

		if (IsRebarSupported() && m_bUseRebar)
		{
			// Create the rebar
			GetRebar().Create(m_hWnd);

			// Create the menu inside rebar
			GetMenubar().Create(GetRebar());
			GetMenubar().SetMenu(m_hMenu);
			AddMenubarBand();

			// Create the toolbar inside rebar
			AddToolbarBand(GetToolbar(), m_ToolbarData, RGB(192, 192, 192), IDW_MAIN);
		}
		else
		{
			// Create the toolbar without a rebar
			GetToolbar().Create(m_hWnd);

			// Set the ImageList for the toolbar
			// A mask of 192,192,192 is compatible with AddBitmap (for Win95)
			int iButtons = GetToolbar().SetButtons(m_ToolbarData);
			GetToolbar().SetImageList(iButtons, RGB(192,192,192), IDW_MAIN, 0, 0);
		}

		// Add the icons for popup menu items
		AddMenuIcons(m_ToolbarData, IDW_MAIN, RGB(192, 192, 192));

		if (!IsMenubarUsed())
			::SetMenu(m_hWnd, m_hMenu);

		if (m_bUseThemes)
			SetTheme();

		// Create the status bar
		GetStatusbar().Create(m_hWnd);

		// Create the view window
		m_pView->Create(m_hWnd);

		// Reposition the child windows
		RecalcLayout();

		// Start timer for Status updates
		if (m_bShowIndicatorStatus || m_bShowMenuStatus)
			::SetTimer(m_hWnd, ID_STATUS_TIMER, 200, NULL);
	}

	void CFrame::OnDrawItem(WPARAM /*wParam*/, LPARAM lParam)
	// OwnerDraw is used to render the popup menu items
	{
		LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT) lParam;

		RECT rc = pdis->rcItem;
		ItemData* pmd = (ItemData*)pdis->itemData;
		CDC DrawDC = pdis->hDC;

		int Iconx;
		int Icony;
		ImageList_GetIconSize(m_hImageList, &Iconx, &Icony);
		int BarWidth = m_ThemeMenu.UseThemes? Iconx + 6 : 0;

		// Draw the side bar
		if (m_ThemeMenu.UseThemes)
		{
			RECT rcBar = rc;
			rcBar.right = BarWidth;
			GradientFill(DrawDC, m_ThemeMenu.clrPressed1, m_ThemeMenu.clrPressed2, &rcBar, TRUE);
		}

		if (pmd->fType & MFT_SEPARATOR)
		{
			// draw separator
			RECT rcSep = rc;
			rcSep.left = BarWidth;
			SolidFill(DrawDC, RGB(255,255,255), &rcSep);
			rcSep.top += (rc.bottom - rc.top)/2;
			rcSep.left = BarWidth + 2;
			::DrawEdge(DrawDC, &rcSep,  EDGE_ETCHED, BF_TOP);
		}
		else
		{
			// draw menu item
			BOOL bDisabled = pdis->itemState & ODS_GRAYED;
			BOOL bSelected = pdis->itemState & ODS_SELECTED;
			BOOL bChecked  = pdis->itemState & ODS_CHECKED;
			RECT rcDraw = rc;

			if ((bSelected) && (!bDisabled))
			{
				// draw selected item background
				if (m_ThemeMenu.UseThemes)
				{
					DrawDC.CreateSolidBrush(m_ThemeMenu.clrHot1);
					DrawDC.CreatePen(PS_SOLID, 1, m_ThemeMenu.clrOutline);
					Rectangle(DrawDC, rcDraw.left, rcDraw.top, rcDraw.right, rcDraw.bottom);
				}
				else
					SolidFill(DrawDC, GetSysColor(COLOR_HIGHLIGHT), &rcDraw);
			}
			else
			{
				// draw non-selected item background
				rcDraw.left = BarWidth;
				SolidFill(DrawDC, RGB(255,255,255), &rcDraw);
			}

			if (bChecked)
				DrawCheckmark(pdis);
			else
				DrawMenuIcon(pdis, bDisabled);

			// Calculate the text rect size
			rc.left  = rc.bottom - rc.top + 2;
			if (_tcschr(pmd->Text, _T('\t')))
				rc.right -= POST_TEXT_GAP;	// Add POST_TEXT_GAP if the text includes a tab

			// Draw the text
			int iMode = ::SetBkMode(DrawDC, TRANSPARENT);
			COLORREF colorText;
			if (m_ThemeMenu.UseThemes)
			{
				rc.left += 8;
				colorText = GetSysColor(bDisabled ?  COLOR_GRAYTEXT : COLOR_MENUTEXT);
			}
			else
				colorText = GetSysColor(bDisabled ?  COLOR_GRAYTEXT : bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_MENUTEXT);

			DrawMenuText(DrawDC, pmd->Text, rc, colorText);
			::SetBkMode(DrawDC, iMode);
		}
		// Detach the DC so it doesn't get destroyed
		DrawDC.DetachDC();
	}

	void CFrame::OnExitMenuLoop()
	{
		for (int nItem = 0; nItem < (int)m_vpItemData.size(); nItem++)
		{
			// Undo OwnerDraw and put the text back
			MENUITEMINFO mii = {0};

			// For Win95 and NT, cbSize needs to be 44
			if ((1400 == GetWinVersion()) || (2400 == GetWinVersion()))
				mii.cbSize = 44;
			else
				mii.cbSize = sizeof(MENUITEMINFO);

			mii.fMask = MIIM_TYPE | MIIM_DATA;
			mii.fType = m_vpItemData[nItem]->fType;
			mii.dwTypeData = m_vpItemData[nItem]->Text;
			mii.cch = lstrlen(m_vpItemData[nItem]->Text);
			mii.dwItemData = 0;
			::SetMenuItemInfo(m_vpItemData[nItem]->hMenu, m_vpItemData[nItem]->nPos, TRUE, &mii);

			// Delete the ItemData object, then erase the vector item
			delete m_vpItemData[nItem];
		}
		m_vpItemData.clear();
	}

	void CFrame::OnHelp()
	{
		if (NULL == m_pAboutDialog)
		{
			// Store the window handle that currently has keyboard focus
			HWND hPrevFocus = ::GetFocus();
			if (hPrevFocus == GetMenubar())
				hPrevFocus = m_hWnd;

			m_pAboutDialog = new CDialog(IDW_ABOUT, m_hWnd);
			m_pAboutDialog->DoModal();
			
			// Clean up
			delete m_pAboutDialog;
			m_pAboutDialog = NULL;
			::SetFocus(hPrevFocus);
		}
	}

	void CFrame::OnInitMenuPopup(WPARAM wParam, LPARAM lParam)
	{
		// The system menu shouldn't be owner drawn
		if (HIWORD(lParam)) return;

		HMENU hMenu = (HMENU)wParam;

		for (int i = 0; i < ::GetMenuItemCount(hMenu) ; i++)
		{
			MENUITEMINFO mii = {0};

			// For Win95 and NT, cbSize needs to be 44
			if ((1400 == GetWinVersion()) || (2400 == GetWinVersion()))
				mii.cbSize = 44;
			else
				mii.cbSize = sizeof(MENUITEMINFO);

			TCHAR szMenuItem[MAX_MENU_STRING] = _T("");

			// Use old fashioned MIIM_TYPE instead of MIIM_FTYPE for MS VC6 compatibility
			mii.fMask  = MIIM_TYPE | MIIM_DATA | MIIM_SUBMENU;
			mii.dwTypeData = szMenuItem;
			mii.cch = MAX_MENU_STRING -1;

			// Specify owner-draw for the menu item type
			if (::GetMenuItemInfo(hMenu, i, TRUE, &mii))
			{
				if (0 == mii.dwItemData)
				{
					ItemData* pItem = new ItemData;		// deleted in OnExitMenuLoop

					// Some MS compilers (including VS2003 under some circumstances) return NULL instead of throwing
					//  an exception when new fails. We make sure an exception gets thrown!
					if (NULL == pItem)
						throw std::bad_alloc();

					ZeroMemory(pItem, sizeof(ItemData));
					pItem->hMenu = hMenu;
					pItem->nPos = i;
					pItem->fType = mii.fType;
					pItem->hSubMenu = mii.hSubMenu;
					mii.fType |= MFT_OWNERDRAW;
					lstrcpyn(pItem->Text, szMenuItem, MAX_MENU_STRING);
					mii.dwItemData = (DWORD_PTR)pItem;

					m_vpItemData.push_back(pItem);			// Store pItem in m_vpItemData
					::SetMenuItemInfo(hMenu, i, TRUE, &mii);// Store pItem in mii
				}
			}
		}
	}

	void CFrame::OnMeasureItem(WPARAM /*wParam*/, LPARAM lParam)
	// Called before the Popup menu is displayed, so that the MEASUREITEMSTRUCT
	//  values can be assigned with the menu item's dimensions.
	{
		LPMEASUREITEMSTRUCT pmis = (LPMEASUREITEMSTRUCT) lParam;

		ItemData* pmd = (ItemData *) pmis->itemData;

		if (pmd->fType & MFT_SEPARATOR)
		{
			pmis->itemHeight = 7;
			pmis->itemWidth  = 0;
		}

		else
		{
			CDC DesktopDC = GetDC(NULL);

			// Get the font used in menu items
			NONCLIENTMETRICS info = {0};
			info.cbSize = sizeof(info);
			SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(info), &info, 0);
			// Default menu items are bold, so take this into account
			if ((INT)::GetMenuDefaultItem(pmd->hMenu, TRUE, GMDI_USEDISABLED) != -1)
				info.lfMenuFont.lfWeight = FW_BOLD;

			DesktopDC.CreateFontIndirect(&info.lfMenuFont);

			// Calculate the size of the text
			SIZE size;
			GetTextExtentPoint32(DesktopDC, pmd->Text, lstrlen(pmd->Text), &size);

			// Calculate the size of the icon
			int Iconx = 0;
			int Icony = 0;
			ImageList_GetIconSize(m_hImageList, &Iconx, &Icony);

			pmis->itemHeight = 2+max(max(size.cy, GetSystemMetrics(SM_CYMENU)-2), Icony+2);
			pmis->itemWidth = size.cx + max(::GetSystemMetrics(SM_CXMENUSIZE), Iconx+2);

			// Allow extra width if the text includes a tab
			if (_tcschr(pmd->Text, _T('\t')))
				pmis->itemWidth += POST_TEXT_GAP;

			// Allow extra width if the menu item has a sub menu
			if (pmd->hSubMenu)
				pmis->itemWidth += 10;

			// Allow extra width for themed menu
			if (m_ThemeMenu.UseThemes)
				pmis->itemWidth += 8;
		}
	}

	void CFrame::OnMenuSelect(WPARAM wParam, LPARAM lParam)
	{
		// Set the Statusbar text when we hover over a menu
		// Only popup submenus have status strings
		if (m_bShowMenuStatus)
		{
			int nID = LOWORD (wParam);
			HMENU hMenu = (HMENU) lParam;

			if ((hMenu != ::GetMenu(m_hWnd)) && (nID != 0) && !(HIWORD(wParam) & MF_POPUP))
				m_StatusText = LoadString(nID);
			else
				m_StatusText = _T("Ready");

			SetStatusText();
		}
	}

	LRESULT CFrame::OnNotifyFrame(WPARAM /*wParam*/, LPARAM lParam)
	{

#ifndef RBN_MINMAX
  #define RBN_MINMAX RBN_FIRST - 21
#endif

		switch (((LPNMHDR)lParam)->code)
		{
		case RBN_HEIGHTCHANGE:
			RecalcLayout();
			::InvalidateRect(m_hWnd, NULL, TRUE);
			break;
		case RBN_LAYOUTCHANGED:
			if (GetRebar().GetRebarTheme().UseThemes && GetRebar().GetRebarTheme().KeepBandsLeft)
				GetRebar().MoveBandsLeft();
			break;
		case RBN_MINMAX:
			if (GetRebar().GetRebarTheme().UseThemes && GetRebar().GetRebarTheme().ShortBands)
				return 1L;	// Supress maximise or minimise rebar band
			break;

		// Display tooltips for the toolbar
		case TTN_GETDISPINFO:
			{
				int iIndex =  GetToolbar().HitTest();
				LPNMTTDISPINFO lpDispInfo = (LPNMTTDISPINFO)lParam;
				if (iIndex >= 0)
				{
					int nID = GetToolbar().GetCommandID(iIndex);
					if (nID > 0) lpDispInfo->lpszText = (LPTSTR)LoadString(nID);
				}
			}
			break;
		} // switch LPNMHDR

		return 0L;

	} // CFrame::OnNotifyFrame(...)

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

		if (m_bUpdateTheme) SetTheme();

		//Reposition and redraw everything
		RecalcLayout();
		::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
	}

	void CFrame::OnTimer(WPARAM wParam)
	{
		if (ID_STATUS_TIMER == wParam)
		{
			if (m_bShowMenuStatus)
			{
				static int nOldID = -1;
				CToolbar& tb = GetToolbar();
				POINT pt = {0};
				::GetCursorPos(&pt);

				// Is the mouse hovering over the toolbar?
				if (WindowFromPoint(pt) == tb)
				{
					// Which toolbar button is the mouse cursor hovering over?
					int nButton = tb.HitTest();
					if (nButton >= 0)
					{
						int nID = GetToolbar().GetCommandID(nButton);
						// Only update the statusbar if things have changed
						if (nID != nOldID)
						{
							if (nID != 0)
								m_StatusText = LoadString(nID);
							else
								m_StatusText = _T("Ready");

							SetStatusText();
						}
						nOldID = nID;
					}
				}
				else
				{
					if (nOldID != -1)
					{
						m_StatusText = _T("Ready");
						SetStatusText();
					}
					nOldID = -1;
				}
			}

			if (m_bShowIndicatorStatus)
				SetStatusIndicators();
		}
	}

	void CFrame::OnViewStatusbar()
	{
		if (::IsWindowVisible(GetStatusbar()))
			::ShowWindow(GetStatusbar(), SW_HIDE);
		else
			::ShowWindow(GetStatusbar(), SW_SHOW);

		UpdateCheckMarks();

		// Reposition the Windows
		RecalcLayout();
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}

	void CFrame::OnViewToolbar()
	{
		if (::IsWindowVisible(GetToolbar()))
		{
			if (IsMenubarUsed())
				::SendMessage(GetRebar(), RB_SHOWBAND, m_Rebar.GetBand(GetToolbar()), FALSE);
			else
				::ShowWindow(GetToolbar(), SW_HIDE);
		}
		else
		{
			if (IsMenubarUsed())
				::SendMessage(GetRebar(), RB_SHOWBAND, m_Rebar.GetBand(GetToolbar()), TRUE);
			else
				::ShowWindow(GetToolbar(), SW_SHOW);
		}

		UpdateCheckMarks();

		// Reposition the Windows
		RecalcLayout();
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}

  	void CFrame::PreCreate(CREATESTRUCT& cs)
	{
		if (NULL == m_pView)
			throw CWinException(_T("CFrame::PreCreate ... m_pView is NULL\n\nUse SetView to set the View Window"));

		// Set the Window Class
		cs.lpszClass = _T("Win32++ Frame");

		// Set the caption from the string resource
		cs.lpszName = LoadString(IDW_MAIN);

		// Set the frame window styles
		cs.style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	}

	void CFrame::RecalcLayout()
	{
		if ((!m_pView) || (!m_pView->GetHwnd()))
			return;

		// Resize the status bar
		HWND hStatusbar = m_Statusbar;
		if (hStatusbar)
		{
			::SendMessage(hStatusbar, WM_SIZE, 0, 0);
			::InvalidateRect(hStatusbar, NULL, TRUE);
		}

		// Reposition the text
		SetStatusText();

		// Resize the rebar or toolbar
		if (IsRebarUsed())
		{
			::SendMessage(m_Rebar, WM_SIZE, 0, 0);
		}
		else
			::SendMessage(m_Toolbar, TB_AUTOSIZE, 0, 0);

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

		if (m_Rebar.GetRebarTheme().UseThemes && m_Rebar.GetRebarTheme().KeepBandsLeft)
			m_Rebar.MoveBandsLeft();

		if (IsMenubarUsed())
			SetMenubarBandSize();

		::SendMessage(m_hWnd, USER_REARRANGED, 0, 0);
	}


	void CFrame::SetFrameMenu(INT ID_MENU)
	{
		if (m_hMenu)
			::DestroyMenu(m_hMenu);

		m_hMenu = ::LoadMenu(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(ID_MENU));

		if(!m_hMenu)
			TRACE(_T("Load Menu failed\n"));
 	}

	void CFrame::SetMenuTheme(ThemeMenu& Theme)
	{
		m_ThemeMenu.UseThemes   = Theme.UseThemes;
		m_ThemeMenu.clrHot1     = Theme.clrHot1;
		m_ThemeMenu.clrHot2     = Theme.clrHot2;
		m_ThemeMenu.clrPressed1 = Theme.clrPressed1;
		m_ThemeMenu.clrPressed2 = Theme.clrPressed2;
		m_ThemeMenu.clrOutline  = Theme.clrOutline;

		::InvalidateRect(m_hWnd, NULL, TRUE);
	}

	void CFrame::SetMenubarBandSize()
	{
		// Sets the minimum width of the Menubar band to the width of the rebar
		// This prevents other bands from moving to this Menubar's row.

		RECT rc = {0};
		GetClientRect(GetRebar(), &rc);
		int Width = rc.right - rc.left;
		CRebar& RB = GetRebar();
		int nBand = RB.GetBand(GetMenubar());
		rc = RB.GetBandBorders(nBand);
		Width = Width - rc.left - rc.right - 2;

		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(REBARBANDINFO);
		rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_SIZE;
		RB.GetBandInfo(nBand, &rbbi);
		if (m_Rebar.GetRebarTheme().UseThemes)
		{
			rbbi.cxMinChild = Width;
			rbbi.cx         = Width;
		}

		RB.SetBandInfo(nBand, &rbbi);
	}

	void CFrame::SetStatusIndicators()
	{
		if (::IsWindow(GetStatusbar()))
		{
			LPCTSTR Status1 = (::GetKeyState(VK_CAPITAL) & 0x0001)? _T("\tCAP") : _T("");
			LPCTSTR Status2 = (::GetKeyState(VK_NUMLOCK) & 0x0001)? _T("\tNUM") : _T("");
			LPCTSTR Status3 = (::GetKeyState(VK_SCROLL)  & 0x0001)? _T("\tSCRL"): _T("");

			// Only update indictors if the text has changed
			if (Status1 != m_OldStatus[0]) 	GetStatusbar().SetPaneText(1, (Status1));
			if (Status2 != m_OldStatus[1])  GetStatusbar().SetPaneText(2, (Status2));
			if (Status3 != m_OldStatus[2])  GetStatusbar().SetPaneText(3, (Status3));

			m_OldStatus[0] = Status1;
			m_OldStatus[1] = Status2;
			m_OldStatus[2] = Status3;
		}
	}

	void CFrame::SetStatusText()
	{
		if (::IsWindow(GetStatusbar()))
		{
			// Get the coordinates of the parent window's client area.
			RECT rcClient;
			::GetClientRect(m_hWnd, &rcClient);

			// width = max(300, rcClient.right)
			int width = (300 > rcClient.right) ? 300 : rcClient.right;
			int iPaneWidths[] = {width - 110, width - 80, width - 50, width - 20};

			if (m_bShowIndicatorStatus)
			{
				// Create 4 panes
				GetStatusbar().CreatePanes(4, iPaneWidths);

				// Or you could create the 4 panes this way
				//	GetStatusbar().SetPaneWidth(0, width - 110);
				//	GetStatusbar().SetPaneWidth(1, 30);
				//	GetStatusbar().SetPaneWidth(2, 30);
				//	GetStatusbar().SetPaneWidth(3, 30);

				SetStatusIndicators();
			}

			// Place text in the 1st pane
			GetStatusbar().SetPaneText(0, m_StatusText.c_str());
		}
	}

	void CFrame::SetTheme()
	{
		// Note: To modify theme colors, override this function in CMainframe,
		//        and make any modifications there.

		// Set a flag redo SetTheme when the theme changes
		m_bUpdateTheme = TRUE;

		// Detect the XP theme name
		WCHAR Name[30] = L"";
		HMODULE hMod = ::LoadLibrary(_T("uxtheme.dll"));
		if(hMod)
		{
			typedef HRESULT (__stdcall *PFNGETCURRENTTHEMENAME)(LPWSTR pszThemeFileName, int cchMaxNameChars,
				LPWSTR pszColorBuff, int cchMaxColorChars, LPWSTR pszSizeBuff, int cchMaxSizeChars);

			PFNGETCURRENTTHEMENAME pfn = (PFNGETCURRENTTHEMENAME)GetProcAddress(hMod, "GetCurrentThemeName");

			(*pfn)(0, 0, Name, 30, 0, 0);

			::FreeLibrary(hMod);
		}

		enum Themetype{ Blue, Silver, Olive };

		int Theme = Blue;
		if (0 == wcscmp(L"NormalColor", Name))	Theme = Blue;
		if (0 == wcscmp(L"Metallic", Name))		Theme = Silver;
		if (0 == wcscmp(L"HomeStead", Name))	Theme = Olive;

		BOOL T = TRUE;
		BOOL F = FALSE;

		switch (Theme)
		{
		case Blue:
			{
				// Use this theme by default
				ThemeToolbar tt = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(192, 128, 255)};
				ThemeRebar tr = {T, RGB(150,190,245), RGB(196,215,250), RGB(220,230,250), RGB( 70,130,220), F, T, T, T, T, F};
				ThemeMenu tm = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(220,230,250), RGB(150,190,245), RGB(128, 128, 200)};

				GetToolbar().SetToolbarTheme(tt);
				SetMenuTheme(tm); // Sets the theme for popup menus
				if (m_bUseRebar)
				{
					GetRebar().SetRebarTheme(tr);
					GetMenubar().SetMenubarTheme(tm); // Sets the theme for Menubar buttons
				}
			}
			break;

		case Silver:
			{
				ThemeToolbar tt = {T, RGB(192, 210, 238), RGB(192, 210, 238), RGB(152, 181, 226), RGB(152, 181, 226), RGB(49, 106, 197)};
				ThemeRebar tr = {T, RGB(225, 220, 240), RGB(240, 240, 245), RGB(245, 240, 255), RGB(160, 155, 180), F, T, T, T, T, F};
				ThemeMenu tm = {T, RGB(196, 215, 250), RGB( 120, 180, 220), RGB(240, 240, 245), RGB(170, 165, 185), RGB(128, 128, 150)};

				GetToolbar().SetToolbarTheme(tt);
				SetMenuTheme(tm); // Sets the theme for popup menus
				if (m_bUseRebar)
				{
					GetRebar().SetRebarTheme(tr);
					GetMenubar().SetMenubarTheme(tm); // Sets the theme for Menubar buttons
				}
			}
			break;

		case Olive:
			{
				ThemeRebar tr = {T, RGB(215, 216, 182), RGB(242, 242, 230), RGB(249, 255, 227), RGB(178, 191, 145), F, T, T, T, T, F};
				ThemeToolbar tt = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(200, 128, 128)};
				ThemeMenu tm = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(249, 255, 227), RGB(178, 191, 145), RGB(128, 128, 128)};

				GetToolbar().SetToolbarTheme(tt);
				SetMenuTheme(tm); // Sets the theme for popup menus
				if (m_bUseRebar)
				{
					GetRebar().SetRebarTheme(tr);
					GetMenubar().SetMenubarTheme(tm); // Sets the theme for Menubar buttons
				}
			}
			break;
		}

		RecalcLayout();
	}

	void CFrame::SetView(CWnd& View)
	{
		// You call this function to assign the View window object to the frame
		m_pView = &View;
	}

	void CFrame::UpdateCheckMarks()
	{
		if (::IsWindowVisible(GetToolbar()))
		// Show toolbar check mark
		{
			::CheckMenuItem (m_hMenu, IDW_VIEW_TOOLBAR, MF_CHECKED);
			if (IsMenubarUsed())
				::CheckMenuItem(GetMenubar().GetMenu(), IDW_VIEW_TOOLBAR, MF_CHECKED);
		}
		else
		// Hide toolbar check mark
		{
			::CheckMenuItem (m_hMenu, IDW_VIEW_TOOLBAR, MF_UNCHECKED);
			if (IsMenubarUsed())
				::CheckMenuItem(GetMenubar().GetMenu(), IDW_VIEW_TOOLBAR, MF_UNCHECKED);
		}

		if (::IsWindowVisible(GetStatusbar()))
		// Show status bar check mark
		{
			::CheckMenuItem (m_hMenu, IDW_VIEW_STATUSBAR, MF_CHECKED);
			if (IsMenubarUsed())
				::CheckMenuItem(GetMenubar().GetMenu(), IDW_VIEW_STATUSBAR, MF_CHECKED);
		}
		else
		// Hide status bar check mark
		{
			::CheckMenuItem (m_hMenu, IDW_VIEW_STATUSBAR, MF_UNCHECKED);
			if (IsMenubarUsed())
				::CheckMenuItem(GetMenubar().GetMenu(), IDW_VIEW_STATUSBAR, MF_UNCHECKED);
		}
	}

	LRESULT CFrame::WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_DESTROY:
			::SetMenu(m_hWnd, NULL);
			::KillTimer(m_hWnd, ID_STATUS_TIMER);
			::PostQuitMessage(0);	// Terminates the application
			return 0L;
		case WM_ERASEBKGND:
			return 0L;
		case WM_HELP:
			OnHelp();
			return 0L;
		case WM_MENUCHAR:
			if ((IsMenubarUsed()) && (LOWORD(wParam)!= VK_SPACE))
			{
				// Activate Menubar for key pressed with Alt key held down
				GetMenubar().MenuChar(wParam, lParam);
				return -1L;
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
			if ((SC_KEYMENU == wParam) && (VK_SPACE != lParam) && IsMenubarUsed())
			{
				GetMenubar().SysCommand(wParam, lParam);
				return 0L;
			}
			break;
		case WM_TIMER:
			OnTimer(wParam);
			return 0L;
		case WM_DRAWITEM:
			// Owner draw menu itmes
			OnDrawItem(wParam, lParam);
			return TRUE; // handled
		case WM_INITMENUPOPUP:
			OnInitMenuPopup(wParam, lParam);
			break;
		case WM_MEASUREITEM:
			OnMeasureItem(wParam, lParam);
			return TRUE; // handled
		case WM_EXITMENULOOP:
			OnExitMenuLoop();
			break;
		} // switch uMsg

		return CWnd::WndProcDefault(hWnd, uMsg, wParam, lParam);
	} // LRESULT CFrame::WndProcDefault(...)

} // namespace Win32xx


