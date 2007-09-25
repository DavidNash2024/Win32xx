// Win32++  Version 5.5
// Released: 4th October, 2007 by:
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
// Frame.cpp
// Definitions for the following classes:
// CFrame, CStatusbar and CToolbar classes
//

#include "Frame.h"
#include <windowsx.h>
#include <shlwapi.h>
#include "MDI.h"
#include "Default_Resource.h"


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

	void CStatusbar::CreatePanes(int iPanes, const int iPaneWidths[])
	{
		// If an element of iPaneWidths is -1, the right edge of the corresponding part extends
		//  to the border of the window
		try
		{
			if (::IsWindow(m_hWnd))
			{
				if (iPanes > 256)
					throw CWinException (_T("CStatusbar::CreatePanes ... Too many panes"));

				// Create the statusbar panes
				if (!::SendMessage(m_hWnd, SB_SETPARTS, iPanes, (LPARAM)iPaneWidths))
					throw CWinException(_T("CStatusbar::CreatePanes failed"));
			}
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(_T("Exception in CStatusbar::CreatePanes"));
		}
	}

	LPCTSTR CStatusbar::GetPaneText(int iPane)
	{
		if (::IsWindow(m_hWnd))
		{
			// Get size of Text array
			int iChars = LOWORD (::SendMessage(m_hWnd, SB_GETTEXTLENGTH, iPane, 0));

			// Get the Text
			TCHAR* szText = new TCHAR[iChars +1 ];
			::SendMessage(m_hWnd, SB_GETTEXT, iPane, (LPARAM)szText);

			//Store the text in the member variable
			lstrcpyn(m_szText, szText, 80);
			delete []szText;
		}

		return m_szText;
	}

	void CStatusbar::SetPaneText(int iPane, LPCTSTR szText, UINT Style)
	// Available Styles: Combinations of ...
	//0					The text is drawn with a border to appear lower than the plane of the window.
	//SBT_NOBORDERS		The text is drawn without borders.
	//SBT_OWNERDRAW		The text is drawn by the parent window.
	//SBT_POPOUT		The text is drawn with a border to appear higher than the plane of the window.
	//SBT_RTLREADING	The text will be displayed in the opposite direction to the text in the parent window.
	{
		try
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

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(_T("Exception in CStatusbar::SetPaneText"));
		}
	}

	void CStatusbar::SetPaneWidth(int iPane, int iWidth)
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
				::SendMessage(m_hWnd, SB_GETPARTS, iParts, (LPARAM)iPaneWidths);

				int iNewParts = max(iPane+1, iParts);
				int* iNewPaneWidths = new int[iNewParts];
				ZeroMemory(iNewPaneWidths, iNewParts*sizeof(int));

				for (int i = 0; i < iParts; i++)
					iNewPaneWidths[i] = iPaneWidths[i];

				if (iPane == 0)
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

		catch (...)
		{
			DebugErrMsg(_T("Exception in CStatusbar::SetPaneWidth"));
		}
	}


	////////////////////////////////////
	// Definitions for the CToolbar class
	//
	CToolbar::CToolbar() : m_hImageList(NULL), m_hImageListHot(NULL), m_hImageListDis(NULL),
		                    m_OldToolbarID(0), m_bDrawArrowBkgrnd(FALSE)
	{
		ZeroMemory(&m_Theme, sizeof(TOOLBARTHEME));
	}

	CToolbar::~CToolbar()
	{
		if (m_hImageList)     ::DeleteObject(m_hImageList);
		if (m_hImageListHot)  ::DeleteObject(m_hImageListHot);
		if (m_hImageListDis)  ::DeleteObject(m_hImageListDis);
	}

	int CToolbar::CommandToIndex(int iButtonID)
	// Retrieves the zero-based index for the button associated with the specified command identifier
	{
		// returns -1 on fail
		return (int)::SendMessage(m_hWnd, TB_COMMANDTOINDEX, (WPARAM)iButtonID, 0);
	}

	void CToolbar::AddBitmap(int iNumButtons, UINT ToolbarID)
	// Note: AddBitmap supports a maximum colour depth of 8 bits (256 colours)
	//       For more colours, use SetImageList instead
	{
		try
		{
			TBADDBITMAP tbab = {0};
			tbab.hInst = GetApp()->GetInstanceHandle();
			tbab.nID   = ToolbarID;
			if (::SendMessage(m_hWnd, TB_ADDBITMAP, iNumButtons, (LPARAM)&tbab) == -1)
				throw CWinException(_T("CToolbar::AddBitmap  TB_ADDBITMAP failed"));

			m_OldToolbarID = ToolbarID;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(_T("Exception in CToolbar::AddBitmap"));
		}
	}

	void CToolbar::ReplaceBitmap(int iNumButtons, UINT NewToolbarID)
	// Note: ReplaceBitmap supports a maximum colour depth of 8 bits (256 colours)
	//       For more colours, use SetImageList instead
	{
		try
		{
			TBREPLACEBITMAP tbrb = {0};
			tbrb.hInstNew = GetApp()->GetInstanceHandle();
			tbrb.hInstOld = GetApp()->GetInstanceHandle();
			tbrb.nIDNew = NewToolbarID;
			tbrb.nIDOld = m_OldToolbarID;
			tbrb.nButtons  = iNumButtons;
			if (::SendMessage(m_hWnd, TB_REPLACEBITMAP, iNumButtons, (LPARAM)&tbrb) == 0)
				throw CWinException(_T("CToolbar::AddBitmap  TB_REPLACEBITMAP failed"));

			m_OldToolbarID = NewToolbarID;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(_T("Exception in CToolbar::ReplaceBitmap"));
		}
	}

	void CToolbar::CreateDisabledImageList()
	{
		if (m_hImageListDis)
		{
			ImageList_Destroy(m_hImageListDis);
			m_hImageListDis = NULL;
		}

		int cx, cy;
		int nCount = ImageList_GetImageCount(m_hImageList);
		if (nCount == 0)
			return;

		ImageList_GetIconSize(m_hImageList, &cx, &cy);

		// Create the destination ImageList
		m_hImageListDis = ImageList_Create(cx, cy, ILC_COLOR32 | ILC_MASK, nCount, 0);

		// Process each image in the ImageList
		for (int i = 0 ; i < nCount; i++)
		{
			HDC hdcToolbar = ::GetDC(m_hWnd);
			HDC hdcMem = ::CreateCompatibleDC(NULL);
			HBITMAP hbmMem = ::CreateCompatibleBitmap(hdcToolbar, cx, cx);
			HBITMAP hbmMemOld = (HBITMAP)::SelectObject(hdcMem, hbmMem);
			RECT rc;
			SetRect(&rc, 0, 0, cx, cx);

			// Set the mask color to magenta for the new ImageList
			COLORREF crMask = RGB(255,0,255);
			SolidFill(hdcMem, crMask, &rc);

			// Draw the image on the memory DC
			ImageList_Draw(m_hImageList, i, hdcMem, 0, 0, ILD_TRANSPARENT);

			// Convert colored pixels to gray
			for (int x = 0 ; x < cx; x++)
			{
				for (int y = 0; y < cy; y++)
				{
					COLORREF clr = ::GetPixel(hdcMem, x, y);

					if (clr != crMask)
					{
						BYTE btGray = 63 + (GetRValue(clr) *3 + GetGValue(clr)*6 + GetBValue(clr))/15;
						::SetPixel(hdcMem, x, y, RGB(btGray, btGray, btGray));
					}
				}
			}

			::SelectObject(hdcMem, hbmMemOld);
			ImageList_AddMasked(m_hImageListDis, hbmMem, crMask);

			// Cleanup the GDI objects
			::DeleteObject(hbmMem);
			::DeleteDC(hdcMem);
			::ReleaseDC(m_hWnd, hdcToolbar);
		}
	}

	void CToolbar::DisableButton(int iButtonID)
	// Disables the specified button in a toolbar
	{
		// An example of iButtonID would be IDM_FILE_OPEN
		if (!::SendMessage(m_hWnd, TB_ENABLEBUTTON, (WPARAM)iButtonID, (LPARAM) MAKELONG(FALSE, 0)))
			DebugWarnMsg(_T("Disable button failed"));
	}

	void CToolbar::EnableButton(int iButtonID)
	// Enables the specified button in a toolbar
	{
		if (!::SendMessage(m_hWnd, TB_ENABLEBUTTON, (WPARAM)iButtonID, (LPARAM) MAKELONG(TRUE,0 )))
			DebugWarnMsg(_T("Enable button failed"));
	}

	int CToolbar::GetButtonCount()
	// Retrieves a count of the buttons currently in the toolbar
	{
		return (int)::SendMessage(m_hWnd, TB_BUTTONCOUNT, 0, 0);
	}

	UINT CToolbar::GetButtonState(int iButtonID)
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
		try
		{
			LRESULT lResult= ::SendMessage(m_hWnd, TB_GETSTATE, (WPARAM) iButtonID, 0);
			if (lResult == -1)
				throw CWinException(_T("CToolbar::GetButtonState failed"));

			return (UINT) lResult;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(_T("Exception in CToolbar::GetButtonState"));
		}
		return 0;
	}

	BYTE CToolbar::GetButtonStyle(int iButtonID)
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
		try
		{
			int iIndex = CommandToIndex(iButtonID);
			if (iIndex == -1)
				throw CWinException(_T("CToolbar::GetButtonStyle failed to get command ID"));

			TBBUTTON tbb = {0};

			LRESULT lResult = ::SendMessage(m_hWnd, TB_GETBUTTON, iIndex, (LPARAM) &tbb);
			if (lResult == -1)
				throw CWinException(_T("CToolbar::GetButtonStyle failed"));

			return tbb.fsStyle;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(_T("Exception in CToolbar::GetButtonStyle"));
		}

		return 0;
	}

	int CToolbar::GetCommandID(int iIndex)
	// Retrieves information about the specified button in a toolbar
	{
		TBBUTTON tbb = {0};
		::SendMessage(m_hWnd, TB_GETBUTTON, iIndex, (WPARAM) &tbb);

		// returns zero if failed
		return tbb.idCommand;
	}

	RECT CToolbar::GetItemRect(int iIndex)
	// Retrieves the bounding rectangle of a button in a toolbar
	{
		RECT rc = {0};
		int iCount = (int)::SendMessage(m_hWnd, TB_BUTTONCOUNT, 0, 0);

		if (iCount >= iIndex)
			::SendMessage(m_hWnd, TB_GETITEMRECT, (WPARAM)iIndex, (LPARAM)&rc);

		return rc;
	}

	SIZE CToolbar::GetMaxSize()
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

	BOOL CToolbar::HasText()
	{
		BOOL bReturn = FALSE;

		for (int i = 0 ; i < GetButtonCount(); i++)
		{
			if (::SendMessage(m_hWnd, TB_GETBUTTONTEXT, GetCommandID(i), 0) != -1)
				bReturn = TRUE;
		}

		return bReturn;
	}


	int CToolbar::HitTest()
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
				HDC hDC = lpNMCustomDraw->nmcd.hdc;
				RECT rcRect = lpNMCustomDraw->nmcd.rc;
				int nState = lpNMCustomDraw->nmcd.uItemState;
				DWORD dwItem = (DWORD)lpNMCustomDraw->nmcd.dwItemSpec;
				DWORD dwTBStyle = (DWORD)::SendMessage(m_hWnd, TB_GETSTYLE, 0, 0);
				int nStyle = GetButtonStyle(dwItem);

				int nButton = (int)::SendMessage(m_hWnd, TB_COMMANDTOINDEX, (WPARAM) dwItem, 0);
				TBBUTTON tb = {0};
				::SendMessage(m_hWnd, TB_GETBUTTON, nButton, (LPARAM)&tb);
				int iImage = (int)tb.dwData;

				// Calculate text size
				TCHAR szText[80] = _T("");
				SIZE TextSize = {0};
				if (HasText())	// Does any button have text?
				{
					HFONT hFont = (HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0, 0);
					HFONT hOldFont = (HFONT)::SelectObject(hDC, hFont);
					if (::SendMessage(m_hWnd, TB_GETBUTTONTEXT, dwItem, (LPARAM)&szText)> 0)
					{
						::GetTextExtentPoint32(hDC, szText, lstrlen(szText), &TextSize);
					}

					::SelectObject(hDC, hOldFont);
				}

				// Draw outline rectangle
				if (nState & (CDIS_HOT | CDIS_SELECTED))
				{
					HPEN hPen = ::CreatePen(PS_SOLID, 1, m_Theme.clrOutline);
					HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
					::MoveToEx(hDC, rcRect.left, rcRect.top, NULL);
					::LineTo(hDC, rcRect.left, rcRect.bottom-1);
					::LineTo(hDC, rcRect.right-1, rcRect.bottom-1);
					::LineTo(hDC, rcRect.right-1, rcRect.top);
					::LineTo(hDC, rcRect.left, rcRect.top);
					::SelectObject(hDC, hOldPen);
					::DeleteObject(hPen);
				}

				// Draw filled gradient background
				::InflateRect(&rcRect, -1, -1);
				if ((nState & CDIS_SELECTED) || (GetButtonState(dwItem) & TBSTATE_PRESSED))
				{
					GradientFill(hDC, m_Theme.clrPressed1, m_Theme.clrPressed2, &rcRect, FALSE);
				}
				else if (nState & CDIS_HOT)
				{
					GradientFill(hDC, m_Theme.clrHot1, m_Theme.clrHot2, &rcRect, FALSE);
				}

				HIMAGELIST hImageList = (nState & CDIS_DISABLED)? m_hImageListDis : (m_hImageListHot? m_hImageListHot: m_hImageList);
				BOOL IsWin95 = ((GetWinVersion() == 1400) || (GetWinVersion() == 2400));
				
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
						GradientFill(hDC, m_Theme.clrPressed1, m_Theme.clrPressed2, &rcArrowBkgnd, FALSE);
					}

					m_bDrawArrowBkgrnd = FALSE;

					// Manually draw the dropdown arrow
					for (int i = 2; i >= 0; i--)
					{
						::MoveToEx(hDC, xAPos -i-1, yAPos - i+1, NULL);
						::LineTo  (hDC, xAPos +i,   yAPos - i+1);
					}

					// Draw line between icon and dropdown arrow
					if ((nStyle & TBSTYLE_DROPDOWN) && ((nState & CDIS_SELECTED) || nState & CDIS_HOT))
					{
						HPEN hPen = ::CreatePen(PS_SOLID, 1, m_Theme.clrOutline);
						HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
						::MoveToEx(hDC, rcRect.right - 13, rcRect.top, NULL);
						::LineTo(hDC, rcRect.right - 13, rcRect.bottom);
						::DeleteObject(::SelectObject(hDC, hOldPen));
					}
				}

				// Draw the button image
				if (xImage > 0)
				{
					ImageList_Draw(hImageList, iImage, hDC, xImage, yImage, ILD_TRANSPARENT);
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

					::SetBkMode(hDC, TRANSPARENT);
					HFONT hFont = (HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0, 0);
					HFONT hOldFont = (HFONT)::SelectObject(hDC, hFont);

					if (nState & (CDIS_DISABLED))
					{
						// Draw text twice for embossed look
						::OffsetRect(&rcText, 1, 1);
						::SetTextColor(hDC, RGB(255,255,255));
						::DrawTextEx(hDC, szText, lstrlen(szText), &rcText, DT_LEFT, NULL);
						::OffsetRect(&rcText, -1, -1);
						::SetTextColor(hDC, GetSysColor(COLOR_GRAYTEXT));
						::DrawTextEx(hDC, szText, lstrlen(szText), &rcText, DT_LEFT, NULL);
					}
					else
					{
						::SetTextColor(hDC, GetSysColor(COLOR_BTNTEXT));
						::DrawTextEx(hDC, szText, lstrlen(szText), &rcText, DT_LEFT | DT_END_ELLIPSIS, NULL);
					}
					::SelectObject(hDC, hOldFont);
				}
			}
			return CDRF_SKIPDEFAULT;  // No further drawing
		//	return CDRF_DODEFAULT;
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

		// Add extra styles for toolbars inside a rebar
		CFrame* pFrame = GetApp()->GetFrame();
		if (pFrame->IsRebarUsed())
			cs.style |= CCS_NODIVIDER | CCS_NORESIZE;
	}

	void CToolbar::SetBitmapSize(int cx, int cy)
	// Needs to be used when the image size is not the default 16 x 15
	// Call this function before using AddBitmap or ReplaceBitmap
	{
		if (!::SendMessage(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELONG(cx, cy)))
			DebugWarnMsg(_T("CToolbar::SetBitmapSize  failed"));
	}

	int CToolbar::SetButtons(const std::vector<UINT> ToolbarData)
	// Assigns a resource ID to each toolbar button
	{
		try
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
				TBBUTTON tbb;

				for (int j = 0 ; j < iNumButtons; j++)
				{
					ZeroMemory(&tbb, sizeof(TBBUTTON));

					if (ToolbarData[j] == 0)
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
						throw (CWinException(_T("CToolbar::SetButtons  .. TB_ADDBUTTONS failed ")));
				}

				// Set rows of text to zero
				::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, 0, 0);
			}
			else
				DebugWarnMsg(_T("No Resource IDs for Toolbar"));

			return iImages;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(_T("Exception in CToolbar::SetButtons"));
		}

		return 0;
	}

	void CToolbar::SetButtonSize(int cx, int cy)
	// Sets the size of the buttons to be added to a toolbar
	// The size can be set only before adding any buttons to the toolbar
	{

		if (!::SendMessage(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELONG(cx, cy)))
			DebugWarnMsg(_T("CToolbar::SetButtonSize failed"));
	}

	void CToolbar::SetButtonState(int iButtonID, UINT State)
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
			DebugWarnMsg(_T("CToolbar::SetButtonState failed"));
 	}

	void CToolbar::SetButtonStyle(int iButtonID, BYTE Style)
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
		try
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

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(_T("Exception in CToolbar::SetButtonStyle"));
		}
	}

	void CToolbar::SetButtonText(int iButtonID, LPCTSTR szText)
	// This rather convoluted approach to setting toolbar button text supports
	// all versions of Windows, including Win95 with COMCTL32.DLL version 4.0
	{
		try
		{
			int iIndex = CommandToIndex(iButtonID);
			if (iIndex == -1)
				throw CWinException(_T("CToolbar::SetButtonText  failed to get Command ID"));

			tString sString = szText;
			std::map<tString, int>::iterator m;
			int iString;

			// Check to see if the string is already added
			m = m_StringMap.find(sString);
			if (m == m_StringMap.end())
			{
				if (m_StringMap.size() == 0)
				{
					// Place a blank string first in the string table, in case some
					// buttons don't have text
					TCHAR szString[3] = _T(" ");
					szString[2] = _T('\0');		// Double-null terminate
					::SendMessage(m_hWnd, TB_ADDSTRING, 0, (LPARAM)szString);
				}

				// No index for this string exists, so create it now
				TCHAR szBuf[80];
				lstrcpyn(szBuf, szText, 79);
				szBuf[lstrlen(szBuf)+1] = _T('\0');		// Double-null terminate

				iString = (int)::SendMessage(m_hWnd, TB_ADDSTRING, 0, (LPARAM)szBuf);
				if (iString == -1)
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
			if (::SendMessage(m_hWnd, TB_GETTEXTROWS, 0, 0) == 0)
				::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, 1, 0);

			// Turn on Toolbar drawing
			::SendMessage(m_hWnd, WM_SETREDRAW, TRUE, 0);

			// Redraw button
			RECT r = GetItemRect(iIndex);
			::InvalidateRect(m_hWnd, &r, TRUE);
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(_T("Exception in CToolbar::SetButtonText"));
		}
	}

	void CToolbar::SetCommandID(int iIndex, int iButtonID)
	// Sets the command identifier of a toolbar button
	{
		try
		{
			if (!::SendMessage(m_hWnd, TB_SETCMDID, iIndex, iButtonID))
				throw CWinException(_T("CToolbar::SetCommandID failed"));
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(_T("Exception in CToolbar::SetCommandID"));
		}
	}

	void CToolbar::SetImageList(int iNumButtons, COLORREF crMask, UINT ToolbarID, UINT ToolbarHotID /*= 0*/, UINT ToolbarDisabledID /*= 0*/)
	// Either sets the imagelist or adds/replaces bitmap depending on ComCtl32.dll version
	// Assumes the width of the button image = bitmap_size / buttons
	// This colour mask is often grey RGB(192,192,192) or magenta (255,0,255);
	{
		HBITMAP hbm = NULL;

		try
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
				hbm = LoadBitmap(MAKEINTRESOURCE(ToolbarID));
				if (!hbm)
					throw CWinException(_T("CToolbar::SetImageList ... LoadBitmap failed "));

				BITMAP bm = {0};

				if (!::GetObject(hbm, sizeof(BITMAP), &bm))
					throw CWinException(_T("CToolbar::SetImageList ... GetObject failed "));

				int iImageWidth  = bm.bmWidth / iNumButtons;
				int iImageHeight = bm.bmHeight;

				// Toolbar ImageLists require Comctl32.dll version 4.7 or later
				if (GetComCtlVersion() == 400)
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
				if(SendMessage(m_hWnd, TB_SETIMAGELIST, 0, (LPARAM)m_hImageList) == -1)
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

					if(SendMessage(m_hWnd, TB_SETHOTIMAGELIST, 0, (LPARAM)m_hImageListHot) == -1)
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
					if(SendMessage(m_hWnd, TB_SETDISABLEDIMAGELIST, 0, (LPARAM)m_hImageListDis) == -1)
						throw CWinException(_T("CToolbar::SetImageList ... TB_SETDISABLEDIMAGELIST failed "));
				}
				else
					CreateDisabledImageList();

				::DeleteObject(hbm);
				hbm = NULL;
			}
		}

		catch (const CWinException &e)
		{
			if (hbm) ::DeleteObject(hbm);
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(_T("Exception in CToolbar::SetImageList"));
		}
	}

	void CToolbar::SetTheme(TOOLBARTHEME& Theme)
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
				LPWINDOWPOS pWinPos = (LPWINDOWPOS)lParam;
				if (GetApp()->GetFrame()->IsRebarUsed())
				{
					REBARTHEME& rb = GetApp()->GetFrame()->GetRebar().GetTheme();
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
	CRebar::CRebar()
	{
		ZeroMemory(&m_Theme, sizeof(REBARTHEME));
	}

	CRebar::~CRebar()
	{
	}

	BOOL CRebar::DeleteBand(const int nBand)
	{
		return (BOOL)::SendMessage(m_hWnd, RB_DELETEBAND, nBand, 0);
	}

	int CRebar::GetBand(const HWND hWnd) const
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

	RECT CRebar::GetBandBorders(int nBand)
	{
		RECT rc = {0};
		::SendMessage(m_hWnd, RB_GETBANDBORDERS, nBand, (LPARAM)&rc);
		return rc;
	}

	int CRebar::GetBandCount() const
	{
		return (int)::SendMessage(m_hWnd, RB_GETBANDCOUNT, 0, 0);
	}

	BOOL CRebar::GetBandInfo(const int nBand, LPREBARBANDINFO prbbi) const
	{
		// REBARBANDINFO describes individual BAND characteristics
		prbbi->cbSize = sizeof(REBARBANDINFO);
		return (BOOL)::SendMessage(m_hWnd, RB_GETBANDINFO, nBand, (LPARAM)prbbi);
	}

	RECT CRebar::GetBandRect(int i)
	{
		RECT rc = {0};
		::SendMessage(m_hWnd, RB_GETRECT, i, (LPARAM)&rc);
		return rc;
	}

	BOOL CRebar::GetBarInfo(LPREBARINFO prbi) const
	{
		// REBARINFO describes overall rebar control characteristics
		prbi->cbSize = sizeof(REBARINFO);
		return (BOOL)::SendMessage(m_hWnd, RB_GETBARINFO, 0, (LPARAM)prbi);
	}

	int CRebar::GetRowHeight(int nRow) const
	{
		return (int)::SendMessage(m_hWnd, RB_GETROWHEIGHT, nRow, 0);
	}

	BOOL CRebar::InsertBand(const int nBand, LPREBARBANDINFO prbbi)
	{
		return (BOOL)::SendMessage(m_hWnd, RB_INSERTBAND, nBand, (LPARAM)(LPREBARBANDINFO)prbbi);
	}

	BOOL CRebar::IsBandVisible(int nBand)
	{
		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(REBARBANDINFO);
		rbbi.fMask = RBBIM_STYLE;
		GetBandInfo(nBand, &rbbi);

		return !(rbbi.fStyle & RBBS_HIDDEN);
	}

	BOOL CRebar::OnEraseBkGnd(HDC hDC)
	{
		if (!m_Theme.UseThemes)
			return FALSE;

		RECT rc;
		GetClientRect(m_hWnd, &rc);
		int BarWidth = rc.right - rc.left;
		int BarHeight = rc.bottom - rc.top;

		// Create and set up our memory DC
		HDC hdcMem = ::CreateCompatibleDC(hDC);
		HBITMAP hbmMem = ::CreateCompatibleBitmap(hDC, BarWidth, BarHeight);
		HBITMAP hbmMemOld = (HBITMAP)::SelectObject(hdcMem, (HBITMAP)hbmMem);

		// Draw to Rebar background to the memory DC
		rc.right = 600;
		GradientFill(hdcMem, m_Theme.clrBkGnd1, m_Theme.clrBkGnd2, &rc, TRUE);
		if (BarWidth >= 600)
		{
			rc.left = 600;
			rc.right = BarWidth;
			SolidFill(hdcMem, m_Theme.clrBkGnd2, &rc);
		}

		if (m_Theme.clrBand1 || m_Theme.clrBand2)
		{
			// Draw the individual band backgrounds
			for (int nBand = 0 ; nBand < GetBandCount(); nBand++)
			{
				if (IsBandVisible(nBand))
				{
					HWND hwndMenubar = GetApp()->GetFrame()->GetMenubar().GetHwnd();
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

						// Fill the Source HDC with the band's background
						HBITMAP hbmSource = ::CreateCompatibleBitmap(hDC, BarWidth, BarHeight);
						HDC hdcSource = ::CreateCompatibleDC(hDC);
						HBITMAP hbmSourceOld = (HBITMAP)::SelectObject(hdcSource, (HBITMAP)hbmSource);
						RECT rcBorder = GetBandBorders(nBand);
						rcDraw.right = rcBand.left + ChildWidth + rcBorder.left;
						SolidFill(hdcSource, m_Theme.clrBand1, &rcDraw);
						rcDraw.top = rcDraw.bottom;
						rcDraw.bottom = rcBand.bottom;
						GradientFill(hdcSource, m_Theme.clrBand1, m_Theme.clrBand2, &rcDraw, FALSE);

						// Set Curve amount for rounded edges
						int Curve = m_Theme.RoundBorders? 12 : 0;

						// Create our mask for rounded edges using RoundRect
						HBITMAP hbmMask   = ::CreateCompatibleBitmap(hDC, BarWidth, BarHeight);
						HDC hdcMask = ::CreateCompatibleDC(hDC);
						HBITMAP hbmMaskOld = (HBITMAP)::SelectObject(hdcMask, (HBITMAP)hbmMask);

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
							SolidFill(hdcMask, RGB(0,0,0), &rcDraw);
							::BitBlt(hdcMask, left, top, cx, cy, hdcMask, left, top, PATINVERT);
							::RoundRect(hdcMask, left, top, right, bottom, Curve, Curve);
						}
						else
						{
							SolidFill(hdcMask, RGB(0,0,0), &rcDraw);
							::RoundRect(hdcMask, left, top, right, bottom, Curve, Curve);
							::BitBlt(hdcMask, left, top, cx, cy, hdcMask, left, top, PATINVERT);
						}

						// Copy Source DC to Memory DC using the RoundRect mask
						::BitBlt(hdcMem, left, top, cx, cy, hdcSource, left, top, SRCINVERT);
						::BitBlt(hdcMem, left, top, cx, cy, hdcMask,   left, top, SRCAND);
						::BitBlt(hdcMem, left, top, cx, cy, hdcSource, left, top, SRCINVERT);

						::DeleteObject(::SelectObject(hdcMask, hbmMaskOld));
						::DeleteDC(hdcMask);
						::DeleteObject(::SelectObject(hdcSource, hbmSourceOld));
						::DeleteDC(hdcSource);

						// Extra drawing to prevent jagged edge while moving bands
						HDC hdcRebar = ::GetDC(m_hWnd);
						::BitBlt(hdcRebar, rcDraw.right - ChildWidth, rcDraw.top, ChildWidth, cy, hdcMem, rcDraw.right - ChildWidth, rcDraw.top, SRCCOPY);
						::ReleaseDC(m_hWnd, hdcRebar);
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
				::DrawEdge(hdcMem, &rc, EDGE_ETCHED, BF_BOTTOM | BF_ADJUST);
			}
		}

		// Copy the Memory DC to the window's DC
		::BitBlt(hDC, 0, 0, BarWidth, BarHeight, hdcMem, 0, 0, SRCCOPY);

		// Cleanup
		::DeleteObject(SelectObject(hdcMem, hbmMemOld));
		::DeleteDC(hdcMem);

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
				::SendMessage(GetHwnd(), RB_MAXIMIZEBAND, nBand, 0);
				OldrcTop = rc.top;
			}
		}
	}

	void CRebar::ResizeBand(const int nBand, SIZE sz)
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

	void CRebar::SetBandBitmap(const int nBand, const HBITMAP hBackground)
	{
		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(REBARBANDINFO);
		rbbi.fMask  = RBBIM_STYLE;
		GetBandInfo(nBand, &rbbi);
		rbbi.fMask  |= RBBIM_BACKGROUND;
		rbbi.hbmBack = hBackground;
		::SendMessage(m_hWnd, RB_SETBANDINFO, nBand, (LPARAM)&rbbi);
	}

	void CRebar::SetBandColor(const int nBand, const COLORREF clrFore, const COLORREF clrBack)
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

	BOOL CRebar::SetBandInfo(const int nBand, LPREBARBANDINFO prbbi)
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

	void CRebar::SetTheme(REBARTHEME& Theme)
	{
		m_Theme.UseThemes    = Theme.UseThemes;
		m_Theme.clrBkGnd1    = Theme.clrBkGnd1;
		m_Theme.clrBkGnd2    = Theme.clrBkGnd2;
		m_Theme.clrBand1     = Theme.clrBand1;
		m_Theme.clrBand2     = Theme.clrBand2;
		m_Theme.KeepBandsLeft= Theme.KeepBandsLeft;
		m_Theme.LockMenuBand = Theme.LockMenuBand;
		m_Theme.ShortBands   = Theme.ShortBands;
		m_Theme.UseLines     = Theme.UseLines;
		m_Theme.FlatStyle    = Theme.FlatStyle;
		m_Theme.RoundBorders = Theme.RoundBorders;


		CMenubar& MB = GetApp()->GetFrame()->GetMenubar();
		if (m_Theme.LockMenuBand)
			ShowGripper(GetBand(MB.GetHwnd()), FALSE);
		else
			ShowGripper(GetBand(MB.GetHwnd()), TRUE);

		::InvalidateRect(m_hWnd, NULL, TRUE);
	}

	BOOL CRebar::ShowBand(int nBand, BOOL fShow)
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

	void CRebar::ShowGripper(int nBand, BOOL fShow)
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
			break;
		case WM_ERASEBKGND:
			if (OnEraseBkGnd((HDC)wParam))
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
		m_hImageList	= NULL;
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
			hMaxMDIChild = ((CMDIFrame*)GetApp()->GetFrame())->GetActiveMDIChild();

		// Load the submenu
		int nMaxedOffset = IsMDIChildMaxed()? 1:0;
		m_hPopupMenu = ::GetSubMenu(m_hTopMenu, m_nHotItem - nMaxedOffset);
		if (IsMDIChildMaxed() && (m_nHotItem == 0))
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
		if (m_pTLSData->hMenuHook != NULL)
			::UnhookWindowsHookEx(m_pTLSData->hMenuHook);

		// Hook messages about to be processed by the shortcut menu
		m_pTLSData->hMenuHook = ::SetWindowsHookEx(WH_MSGFILTER, (HOOKPROC)StaticMsgHook, NULL, ::GetCurrentThreadId());;

		// Display the shortcut menu
		UINT nID = ::TrackPopupMenuEx(m_hPopupMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
			rc.left, rc.bottom, m_hWnd, &tpm);

		// We get here once the TrackPopupMenuEx has ended
		m_bMenuActive = FALSE;

		// Remove the message hook
		::UnhookWindowsHookEx(m_pTLSData->hMenuHook);
		m_pTLSData->hMenuHook = NULL;

		RevertPopupMenu(0);

		// Process MDI Child system menu
		if (IsMDIChildMaxed())
		{
			if (m_hPopupMenu == ::GetSystemMenu(hMaxMDIChild, FALSE))
			{
				if (nID)
					::SendMessage(hMaxMDIChild, WM_SYSCOMMAND, nID, 0);
			}
		}

		// Resestablish Focus
		if (m_bKeyMode)
			GrabFocus();
	}

	void CMenubar::DrawAllMDIButtons(HDC hDC)
	{
		CMDIFrame* pMDIFrame = (CMDIFrame*)GetApp()->GetFrame();

		if (!pMDIFrame->IsMDIFrame())
			return;

		if (pMDIFrame->IsMDIChildMaxed())
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

			DrawMDIButton(hDC, MDI_MIN, 0);
			DrawMDIButton(hDC, MDI_RESTORE, 0);
			DrawMDIButton(hDC, MDI_CLOSE, 0);
		}
	}

	void CMenubar::DrawMDIButton(HDC hDC, int iButton, UINT uState)
	{
		// uState: Normal = 0, Hot = 1, Pressed = 2
		// iButton: MDI_MIN = 0, MDI_RESTORE = 1, MDI_CLOSE = 2

		if (!IsRectEmpty(&m_MDIRect[iButton]))
		{
			switch (uState)
			{
			case 0:
				{
					// Draw a grey outline
					HPEN hPen = ::CreatePen(PS_SOLID, 1, GetSysColor(COLOR_BTNFACE));
					HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
					::MoveToEx(hDC, m_MDIRect[iButton].left, m_MDIRect[iButton].bottom, NULL);
					::LineTo(hDC, m_MDIRect[iButton].right, m_MDIRect[iButton].bottom);
					::LineTo(hDC, m_MDIRect[iButton].right, m_MDIRect[iButton].top);
					::LineTo(hDC, m_MDIRect[iButton].left, m_MDIRect[iButton].top);
					::LineTo(hDC, m_MDIRect[iButton].left, m_MDIRect[iButton].bottom);
					SelectObject(hDC, hOldPen);
					DeleteObject(hPen);
				}
				break;
			case 1:
				{
					// Draw outline, white at top, black on bottom
					HPEN hWhitePen = ::CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
					HPEN hBlackPen = ::CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					HPEN hOldPen = (HPEN)::SelectObject(hDC, hBlackPen);
					::MoveToEx(hDC, m_MDIRect[iButton].left, m_MDIRect[iButton].bottom, NULL);
					::LineTo(hDC, m_MDIRect[iButton].right, m_MDIRect[iButton].bottom);
					::LineTo(hDC, m_MDIRect[iButton].right, m_MDIRect[iButton].top);
					::SelectObject(hDC, hWhitePen);
					::LineTo(hDC, m_MDIRect[iButton].left, m_MDIRect[iButton].top);
					::LineTo(hDC, m_MDIRect[iButton].left, m_MDIRect[iButton].bottom);
					SelectObject(hDC, hOldPen);
					DeleteObject(hWhitePen);
					DeleteObject(hBlackPen);
				}

				break;
			case 2:
				{
					// Draw outline, black on top, white on bottom
					HPEN hWhitePen = ::CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
					HPEN hBlackPen = ::CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
					HPEN hOldPen = (HPEN)::SelectObject(hDC, hWhitePen);
					::MoveToEx(hDC, m_MDIRect[iButton].left, m_MDIRect[iButton].bottom, NULL);
					::LineTo(hDC, m_MDIRect[iButton].right, m_MDIRect[iButton].bottom);
					::LineTo(hDC, m_MDIRect[iButton].right, m_MDIRect[iButton].top);
					::SelectObject(hDC, hBlackPen);
					::LineTo(hDC, m_MDIRect[iButton].left, m_MDIRect[iButton].top);
					::LineTo(hDC, m_MDIRect[iButton].left, m_MDIRect[iButton].bottom);
					SelectObject(hDC, hOldPen);
					DeleteObject(hWhitePen);
					DeleteObject(hBlackPen);
				}
				break;
			}

			switch (iButton)
			{
			case MDI_MIN:
				// Manually Draw Minimise button
				::MoveToEx(hDC, m_MDIRect[0].left + 4, m_MDIRect[0].bottom -4, NULL);
				::LineTo(hDC, m_MDIRect[0].right - 4, m_MDIRect[0].bottom - 4);

				::MoveToEx(hDC, m_MDIRect[0].left + 4, m_MDIRect[0].bottom -5, NULL);
				::LineTo(hDC, m_MDIRect[0].right - 4, m_MDIRect[0].bottom - 5);
				break;
			case MDI_RESTORE:
				// Manually Draw Restore Button
				::MoveToEx(hDC, m_MDIRect[1].left + 3, m_MDIRect[1].top + 7, NULL);
				::LineTo(hDC, m_MDIRect[1].left + 3, m_MDIRect[1].bottom -4);
				::LineTo(hDC, m_MDIRect[1].right - 6, m_MDIRect[1].bottom -4);
				::LineTo(hDC, m_MDIRect[1].right - 6, m_MDIRect[1].top + 7);
				::LineTo(hDC, m_MDIRect[1].left + 3, m_MDIRect[1].top + 7);

				::MoveToEx(hDC, m_MDIRect[1].left + 3, m_MDIRect[1].top + 8, NULL);
				::LineTo(hDC, m_MDIRect[1].right - 6, m_MDIRect[1].top + 8);

				::MoveToEx(hDC, m_MDIRect[1].left + 5, m_MDIRect[1].top + 7, NULL);
				::LineTo(hDC, m_MDIRect[1].left + 5, m_MDIRect[1].top + 4);
				::LineTo(hDC, m_MDIRect[1].right - 4, m_MDIRect[1].top + 4);
				::LineTo(hDC, m_MDIRect[1].right - 4, m_MDIRect[1].bottom -6);
				::LineTo(hDC, m_MDIRect[1].right - 6, m_MDIRect[1].bottom -6);

				::MoveToEx(hDC, m_MDIRect[1].left + 5, m_MDIRect[1].top + 5, NULL);
				::LineTo(hDC, m_MDIRect[1].right - 4, m_MDIRect[1].top + 5);
				break;
			case MDI_CLOSE:
				// Manually Draw Close Button
				::MoveToEx(hDC, m_MDIRect[2].left + 4, m_MDIRect[2].top +5, NULL);
				::LineTo(hDC, m_MDIRect[2].right - 4, m_MDIRect[2].bottom -3);

				::MoveToEx(hDC, m_MDIRect[2].left + 5, m_MDIRect[2].top +5, NULL);
				::LineTo(hDC, m_MDIRect[2].right - 4, m_MDIRect[2].bottom -4);

				::MoveToEx(hDC, m_MDIRect[2].left + 4, m_MDIRect[2].top +6, NULL);
				::LineTo(hDC, m_MDIRect[2].right - 5, m_MDIRect[2].bottom -3);

				::MoveToEx(hDC, m_MDIRect[2].right -5, m_MDIRect[2].top +5, NULL);
				::LineTo(hDC, m_MDIRect[2].left + 3, m_MDIRect[2].bottom -3);

				::MoveToEx(hDC, m_MDIRect[2].right -5, m_MDIRect[2].top +6, NULL);
				::LineTo(hDC, m_MDIRect[2].left + 4, m_MDIRect[2].bottom -3);

				::MoveToEx(hDC, m_MDIRect[2].right -6, m_MDIRect[2].top +5, NULL);
				::LineTo(hDC, m_MDIRect[2].left + 3, m_MDIRect[2].bottom -4);
				break;
			}
		}
	}

	void CMenubar::DrawMenuText(HDC hDC, LPCTSTR ItemText, RECT rc, COLORREF colorText)
	{
		// find the position of tab character
		int nTab = -1;
		for(int i = 0; i < lstrlen(ItemText); i++)
		{
			if(ItemText[i] == _T('\t'))
			{
				nTab = i;
				break;
			}
		}

		::SetTextColor(hDC, colorText);
		::DrawText(hDC, ItemText, nTab, &rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

		// Draw text after tab right aligned
		if(nTab != -1)
			::DrawText(hDC, &ItemText[nTab + 1], -1, &rc, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
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

	BOOL CMenubar::IsMDIChildMaxed()
	{
		BOOL bMaxed = FALSE;
		CFrame* pFrame = GetApp()->GetFrame();

		if (pFrame->IsMDIFrame())
		{
			bMaxed = ((CMDIFrame*)pFrame)->IsMDIChildMaxed();
		}

		return bMaxed;
	}

	void CMenubar::MenuChar(WPARAM wParam, LPARAM /* lParam */)
	{
		if (!m_bMenuActive)
			DoAltKey(LOWORD(wParam));
	}

	void CMenubar::DrawBackground(HDC hDC, RECT rc)
	{
		HBRUSH hbHighlight = ::GetSysColorBrush(COLOR_HIGHLIGHT);
		::FillRect(hDC, &rc, hbHighlight);
	}

	void CMenubar::DrawCheckmark(LPDRAWITEMSTRUCT pdis)
	// Copy the checkmark or radiocheck transparently
	{
		HDC hdcMem = ::CreateCompatibleDC(pdis->hDC);
		if (hdcMem)
		{
			int cxCheck = ::GetSystemMetrics(SM_CXMENUCHECK);
			int cyCheck = ::GetSystemMetrics(SM_CYMENUCHECK);
			HBITMAP hbmMono = ::CreateBitmap(cxCheck, cyCheck, 1, 1, NULL);
			if (hbmMono)
			{
				HBITMAP hbmPrev = (HBITMAP)::SelectObject(hdcMem, hbmMono);
				if (hbmPrev)
				{
					RECT rCheck = { 0, 0, cxCheck, cyCheck };

					// Copy the check mark bitmap to hdcMem
					if (((ItemData*)pdis->itemData)->fType == MFT_RADIOCHECK)
						::DrawFrameControl(hdcMem, &rCheck, DFC_MENU, DFCS_MENUBULLET);
					else
						::DrawFrameControl(hdcMem, &rCheck, DFC_MENU, DFCS_MENUCHECK);

					RECT rc = pdis->rcItem;
					int offset = (rc.bottom - rc.top - ::GetSystemMetrics(SM_CXMENUCHECK))/2;

					if (pdis->itemState & ODS_SELECTED)
					// Draw a white check mark for a selected item.
					// Unfortunately MaskBlt isn't supported on Win95, 98 or ME, so we do it the hard way
					{
						HDC hdcMask = ::CreateCompatibleDC(pdis->hDC);
						if (hdcMask)
						{
							HBITMAP hbmMask = ::CreateCompatibleBitmap(pdis->hDC, cxCheck, cyCheck);
							if (hbmMask)
							{
								HBITMAP hbmPrevMask = (HBITMAP)::SelectObject(hdcMask, hbmMask);
								if (hbmPrevMask)
								{
									// Set the colour of the check mark to white
									SetBkColor(hdcMask, RGB(255, 255, 255));
									::BitBlt(hdcMask, 0, 0, cxCheck, cyCheck, hdcMask, 0, 0, PATCOPY);

									// Invert the check mark bitmap
									::BitBlt(hdcMem, 0, 0, cxCheck, cyCheck, hdcMem, 0, 0, DSTINVERT);

									// Use the mask to copy the check mark to Menu's device context
									::BitBlt(hdcMask, 0, 0, cxCheck, cyCheck, hdcMem, 0, 0, SRCAND);
									::BitBlt(pdis->hDC, rc.left + offset, rc.top + offset, cxCheck, cyCheck, hdcMask, 0, 0, SRCPAINT);

									::SelectObject(hdcMask, hbmPrevMask);
								}
							}
							::DeleteObject(hbmMask);
						}
						::DeleteDC(hdcMask);
					}
					else
						// Draw a black check mark for an unselected item
						::BitBlt(pdis->hDC, rc.left + offset, rc.top + offset, cxCheck, cyCheck, hdcMem, 0, 0, SRCAND);

					::SelectObject(hdcMem, hbmPrev);
				}
				::DeleteObject(hbmMono);
			}
			::DeleteDC(hdcMem);
		}
	}

	void CMenubar::DrawIcon(LPDRAWITEMSTRUCT pdis)
	{
		if (!m_hImageList)
			return;

		int Iconx;
		int Icony;
		ImageList_GetIconSize(m_hImageList, &Iconx, &Icony);
		HDC hDC = pdis->hDC;
		RECT rc = pdis->rcItem;
		int offset = (rc.bottom - rc.top - Icony)/2;
		int height = rc.bottom - rc.top;
		::SetRect(&rc, rc.left, rc.top, rc.left + height, rc.bottom);
		::InflateRect(&rc, -offset, -offset);

		int iImage = -1;
		for (int i = 0 ; i < (int)m_ImageData.size(); i++)
		{
			if (m_ImageData[i] == pdis->itemID)
				iImage = i;
		}

		if (iImage >= 0 )
			ImageList_Draw(m_hImageList, iImage, hDC, rc.left, rc.top, ILD_TRANSPARENT);
	}

	void CMenubar::OnCreate()
	{
		m_pTLSData->pMenubar = this;

		// We must send this message before sending the TB_ADDBITMAP or TB_ADDBUTTONS message
		::SendMessage(m_hWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	}

	LRESULT CMenubar::OnCustomDraw(NMHDR* pNMHDR)
	// CustomDraw is used to render the Menubar's toolbar buttons
	{
		// Use custom draw to draw a rectangle over the hot button
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
				HDC hDC = lpNMCustomDraw->nmcd.hdc;
				RECT rcRect = lpNMCustomDraw->nmcd.rc;
				int nState = lpNMCustomDraw->nmcd.uItemState;
				DWORD dwItem = (DWORD)lpNMCustomDraw->nmcd.dwItemSpec;

				// Leave a 1 pixel gap above the drawn rectangle
				rcRect.top = 1;

				if (IsMDIChildMaxed() && (dwItem == 0))
				// Draw over MDI Max button
				{
					CMDIFrame* pMDIFrame = (CMDIFrame*)GetApp()->GetFrame();
					HICON hIcon = (HICON)::SendMessage(pMDIFrame->GetActiveMDIChild(), WM_GETICON, ICON_SMALL, 0);
					if (hIcon == NULL)
						hIcon = ::LoadIcon(NULL, IDI_APPLICATION);

					int cx = ::GetSystemMetrics (SM_CXSMICON);
					int cy = ::GetSystemMetrics (SM_CYSMICON);
					int y = 1 + (rcRect.bottom - rcRect.top - cy)/2;
					int x = 0;
					::DrawIconEx(hDC, x, y, hIcon, cx, cy, 0, NULL, DI_NORMAL);

					return CDRF_SKIPDEFAULT;  // No further drawing
				}

				else if (nState & (CDIS_HOT | CDIS_SELECTED))
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

					return CDRF_SKIPDEFAULT;  // No further drawing
				}
			}
			return CDRF_DODEFAULT ;   // Do default drawing

		// Painting cycle has completed
		case CDDS_POSTPAINT:
			// Draw MDI Minimise, Restore and Close buttons
			{
				DrawAllMDIButtons(lpNMCustomDraw->nmcd.hdc);
			}
			break;
		}
		return 0L;
	}

	BOOL CMenubar::OnDrawItem(WPARAM /*wParam*/, LPARAM lParam)
	// CustomDraw is used to render the popup menu items
	{
		LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT) lParam;

		RECT rc = pdis->rcItem;
		ItemData* pmd = (ItemData*)pdis->itemData;
		HDC hDC = pdis->hDC;

		if (pmd->fType & MFT_SEPARATOR)
		{
			// draw separator
			rc.top += (rc.bottom - rc.top)/2;
			::DrawEdge(hDC, &rc,  EDGE_ETCHED, BF_TOP);
		}
		else
		{
			BOOL bDisabled = pdis->itemState & ODS_GRAYED;
			BOOL bSelected = pdis->itemState & ODS_SELECTED;
			BOOL bChecked  = pdis->itemState & ODS_CHECKED;

			if (bSelected)
				DrawBackground(hDC, rc);
			else
				::FillRect(hDC, &rc, ::GetSysColorBrush(COLOR_MENU));

			// Draw checkmark or icon
			if (bChecked)
				DrawCheckmark(pdis);
			else
				DrawIcon(pdis);

			// Calculate the text rect size
			rc.left  = rc.bottom - rc.top + 2;
			if (_tcschr(pmd->Text, _T('\t')))
				rc.right -= POST_TEXT_GAP;	// Add POST_TEXT_GAP if the text includes a tab

			// Draw the text
			::SetBkMode(hDC, TRANSPARENT);
			COLORREF colorText = GetSysColor(bDisabled ?  COLOR_GRAYTEXT : bSelected ? COLOR_HIGHLIGHTTEXT : COLOR_MENUTEXT);
			DrawMenuText(hDC, pmd->Text, rc, colorText);
		}

		return TRUE;
	}

	void CMenubar::OnInitMenuPopup(WPARAM wParam, LPARAM /*lParam*/)
	{
		HWND hMaxMDIChild = NULL;
		if (IsMDIChildMaxed())
			hMaxMDIChild = ((CMDIFrame*)GetApp()->GetFrame())->GetActiveMDIChild();

		HMENU hMenu = (HMENU)wParam;
		if (hMenu == ::GetSystemMenu(hMaxMDIChild, FALSE))
			return;	// No OwnerDraw for system menu

		// Reverse any previous OwnerDraw for this menu (required for submenus)
		RevertPopupMenu(hMenu);

		for (int i = 0; i < ::GetMenuItemCount(hMenu) ; i++)
		{
			MENUITEMINFO mii = {0};

			// For Win95 and NT, cbSize needs to be 44
			if ((GetWinVersion() == 1400) || (GetWinVersion() == 2400))
				mii.cbSize = 44;
			else
				mii.cbSize = sizeof(MENUITEMINFO);

			TCHAR szMenuItem[MAX_MENU_STRING];

			// Use old fashioned MIIM_TYPE instead of MIIM_FTYPE for MS VC6 compatibility
			mii.fMask  = MIIM_TYPE | MIIM_DATA;
			mii.dwTypeData = szMenuItem;
			mii.cch = MAX_MENU_STRING -1;

			if (::GetMenuItemInfo(hMenu, i, TRUE, &mii))
			{
				ItemData* pItem = new ItemData;		// deleted in DoPopupMenu
				pItem->hMenu = hMenu;
				pItem->nPos = i;
				pItem->fType = mii.fType;
				mii.fType |= MFT_OWNERDRAW;
				lstrcpyn(pItem->Text, szMenuItem, MAX_MENU_STRING);
				mii.dwItemData = (DWORD_PTR)pItem;

				m_vpItemData.push_back(pItem);			// Store pItem in m_vpItemData
				::SetMenuItemInfo(hMenu, i, TRUE, &mii);// Store pItem in mii
			}

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

		CMDIFrame* pMDIFrame = (CMDIFrame *)GetApp()->GetFrame();
		if (pMDIFrame->IsMDIFrame())
		{
			if (pMDIFrame->IsMDIChildMaxed())
			{
				HDC hdcMenubar = ::GetDC(m_hWnd);
				if (hdcMenubar)
				{
					m_nMDIButton = -1;
					if (PtInRect(&m_MDIRect[0], pt)) m_nMDIButton = 0;
					if (PtInRect(&m_MDIRect[1], pt)) m_nMDIButton = 1;
					if (PtInRect(&m_MDIRect[2], pt)) m_nMDIButton = 2;

					if (m_nMDIButton >= 0)
					{
						DrawMDIButton(hdcMenubar, MDI_MIN,     (m_nMDIButton == 0)? 2 : 0);
						DrawMDIButton(hdcMenubar, MDI_RESTORE, (m_nMDIButton == 1)? 2 : 0);
						DrawMDIButton(hdcMenubar, MDI_CLOSE,   (m_nMDIButton == 2)? 2 : 0);
					}

					::ReleaseDC(m_hWnd, hdcMenubar);
				}

				// Bring up the MDI Child window's system menu when the icon is pressed
				if (HitTest() == 0)
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

		CMDIFrame* pMDIFrame = (CMDIFrame*)GetApp()->GetFrame();
		if (pMDIFrame->IsMDIFrame())
		{
			HWND MDIClient = pMDIFrame->GetMDIClient().GetHwnd();
			HWND MDIChild = pMDIFrame->GetActiveMDIChild();

			if (IsMDIChildMaxed())
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

		// Append MDI Child windows
		CMDIFrame* pMDIFrame = (CMDIFrame*)GetApp()->GetFrame();
		TCHAR szTitle[25];
		TCHAR szString[30];
		int nWindow = 0;

		// Allocate an iterator for our MDIChild vector
		std::vector <CMDIChild*>::iterator v;

		for (v = pMDIFrame->GetMDIChildVect().begin(); v < pMDIFrame->GetMDIChildVect().end(); v++)
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
					::wsprintf(szString, _T("&%d %s"), nWindow+1, szTitle);
					::AppendMenu(hMenuWindow, MF_STRING, IDW_FIRSTCHILD + nWindow, szString );

					if (pMDIFrame->GetActiveMDIChild() == hWndMDIChild)
						::CheckMenuItem(hMenuWindow, IDW_FIRSTCHILD+nWindow, MF_CHECKED);

					nWindow++;
				}
				else if (nWindow == 9)
				// For the 10th MDI child, add this menu item and return
				{
					::AppendMenu(hMenuWindow, MF_STRING, IDW_FIRSTCHILD + nWindow, _T("&Windows..."));
					return;
				}
			}
		}
	}

	BOOL CMenubar::OnMeasureItem(WPARAM /*wParam*/, LPARAM lParam)
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
			HDC hdcMenubar = GetDC(m_hWnd);
			if (hdcMenubar)
			{
				HFONT hfntOld = (HFONT)::SelectObject(hdcMenubar, (HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0, 0));
				SIZE size;
				int Iconx = 0;
				int Icony = 0;

				ImageList_GetIconSize(m_hImageList, &Iconx, &Icony);
				GetTextExtentPoint32(hdcMenubar, pmd->Text, lstrlen(pmd->Text), &size);

				pmis->itemHeight = max(max(size.cy, GetSystemMetrics(SM_CYMENU)-2), Icony+2);
				pmis->itemWidth = size.cx + max(::GetSystemMetrics(SM_CXMENUSIZE), Iconx+2);

				// Allow extra width if the text includes a tab
				if (_tcschr(pmd->Text, _T('\t')))
					pmis->itemWidth += POST_TEXT_GAP;

				::SelectObject(hdcMenubar, hfntOld);
				::ReleaseDC(m_hWnd, hdcMenubar);
			}
		}

		return TRUE;
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
			if (IsMDIChildMaxed() && (HitTest() == 0))
			{
				CMDIFrame* pMDIFrame = (CMDIFrame*)GetApp()->GetFrame();
				HWND MDIChild = pMDIFrame->GetActiveMDIChild();
				HMENU hChildMenu = ::GetSystemMenu(MDIChild, FALSE);

				UINT nID = ::GetMenuDefaultItem(hChildMenu, FALSE, 0);
				if (nID)
					::PostMessage(MDIChild, WM_SYSCOMMAND, nID, 0);
				m_bExitAfter = TRUE;
				return FALSE;
			}
			return TRUE;

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
		CMDIFrame* pMDIFrame = (CMDIFrame*)GetApp()->GetFrame();
		if (pMDIFrame->IsMDIFrame())
		{
			if (pMDIFrame->IsMDIChildMaxed())
			{
				HDC hdcMenubar = ::GetDC(m_hWnd);
				if (hdcMenubar)
				{
					DrawMDIButton(hdcMenubar, MDI_MIN,     0);
					DrawMDIButton(hdcMenubar, MDI_RESTORE, 0);
					DrawMDIButton(hdcMenubar, MDI_CLOSE,   0);

					::ReleaseDC(m_hWnd, hdcMenubar);
				}
			}
		}
	}

	void CMenubar::OnMouseMove(WPARAM wParam, LPARAM lParam)
	{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		CMDIFrame* pMDIFrame = (CMDIFrame*)GetApp()->GetFrame();
		if (pMDIFrame->IsMDIFrame())
		{
			if (pMDIFrame->IsMDIChildMaxed())
			{
				HDC hdcMenubar = ::GetDC(m_hWnd);
				if (hdcMenubar)
				{
					int MDIButton = -1;
					if (PtInRect(&m_MDIRect[0], pt)) MDIButton = 0;
					if (PtInRect(&m_MDIRect[1], pt)) MDIButton = 1;
					if (PtInRect(&m_MDIRect[2], pt)) MDIButton = 2;

					if (wParam == MK_LBUTTON)  // mouse moved with left mouse button is held down
					{
						// toggle the MDI button image pressed/unpressed as required
						if (MDIButton >= 0)
						{
							DrawMDIButton(hdcMenubar, MDI_MIN,     ((MDIButton == 0) && (m_nMDIButton == 0))? 2 : 0);
							DrawMDIButton(hdcMenubar, MDI_RESTORE, ((MDIButton == 1) && (m_nMDIButton == 1))? 2 : 0);
							DrawMDIButton(hdcMenubar, MDI_CLOSE,   ((MDIButton == 2) && (m_nMDIButton == 2))? 2 : 0);
						}
						else
						{
							DrawMDIButton(hdcMenubar, MDI_MIN,     0);
							DrawMDIButton(hdcMenubar, MDI_RESTORE, 0);
							DrawMDIButton(hdcMenubar, MDI_CLOSE,   0);
						}
					}
					else	// mouse moved without left mouse button held down
					{
						if (MDIButton >= 0)
						{
							DrawMDIButton(hdcMenubar, MDI_MIN,     (MDIButton == 0)? 1 : 0);
							DrawMDIButton(hdcMenubar, MDI_RESTORE, (MDIButton == 1)? 1 : 0);
							DrawMDIButton(hdcMenubar, MDI_CLOSE,   (MDIButton == 2)? 1 : 0);
						}
						else
						{
							DrawMDIButton(hdcMenubar, MDI_MIN,     0);
							DrawMDIButton(hdcMenubar, MDI_RESTORE, 0);
							DrawMDIButton(hdcMenubar, MDI_CLOSE,   0);
						}
					}
					::ReleaseDC(m_hWnd, hdcMenubar);
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

	void CMenubar::RevertPopupMenu(HMENU hMenu)
	{
		int nItem = (int)m_vpItemData.size() -1;

		while (nItem >= 0)
		{
			if ((m_vpItemData[nItem]->hMenu == hMenu) || (hMenu == NULL))
			{
				// Undo OwnerDraw and put the text back
				MENUITEMINFO mii = {0};
				// For Win95, cbSize needs to be 44
				mii.cbSize = (GetWinVersion() == 1400)? 44 : sizeof(MENUITEMINFO);
				mii.fMask = MIIM_TYPE;
				mii.fType = m_vpItemData[nItem]->fType;
				mii.dwTypeData = m_vpItemData[nItem]->Text;
				mii.cch = lstrlen(m_vpItemData[nItem]->Text);
				::SetMenuItemInfo(m_vpItemData[nItem]->hMenu, m_vpItemData[nItem]->nPos, TRUE, &mii);

				// Delete the ItemData object, then erase the vector item
				delete m_vpItemData[nItem];
				m_vpItemData.erase(m_vpItemData.begin() + nItem);
			}
			nItem--;
		}
	}

	void CMenubar::OnWindowPosChanged()
	{
		::InvalidateRect(m_hWnd, &m_MDIRect[0], TRUE);
		::InvalidateRect(m_hWnd, &m_MDIRect[1], TRUE);
		::InvalidateRect(m_hWnd, &m_MDIRect[2], TRUE);
		{
			HDC hdcMenubar = ::GetDC(m_hWnd);
			if (hdcMenubar)
			{
				DrawAllMDIButtons(hdcMenubar);
				::ReleaseDC(m_hWnd, hdcMenubar);
			}
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

		try
		{
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
				TCHAR szMenuName[MAX_MENU_STRING +1];

				if (::GetMenuString(hMenu, i, szMenuName, MAX_MENU_STRING, MF_BYPOSITION) == 0)
					throw CWinException(TEXT("Menubar::SetMenu  GetMenuString failed"));

				SetButtonText(i  + nMaxedOffset, szMenuName);
			}
		}

		catch(const CWinException &e)
		{
			e.MessageBox();
		}

		catch(...)
		{
			DebugErrMsg(_T("Exception in CMenubar::SetMenu"));
		}
	}

	void CMenubar::SetIcons(const std::vector<UINT> ImageData, UINT nID_Image, COLORREF crMask)
	// Set the drop-down icons using a bitmap resource
	{
		// Remove any existing imagelist
		if (m_hImageList)
		{
			ImageList_Destroy(m_hImageList);
			m_hImageList = NULL;
		}
		m_ImageData.clear();

		if (ImageData.size() == 0)
			return;

		int iImages = 0;
		for (unsigned int i = 0 ; i < ImageData.size(); i++)
		{
			if (ImageData[i] != 0)
			{
				m_ImageData.push_back(ImageData[i]);
				iImages++;
			}
		}

		// Set the button images
		HBITMAP hbm = LoadBitmap(MAKEINTRESOURCE(nID_Image));
		BITMAP bm = {0};

		::GetObject(hbm, sizeof(BITMAP), &bm);
		int iImageWidth  = bm.bmWidth / (int)m_ImageData.size();
		int iImageHeight = bm.bmHeight;

		m_hImageList = ImageList_Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, iImages, 0);
		ImageList_AddMasked(m_hImageList, hbm, crMask);
		::DeleteObject(hbm);
	}

    void CMenubar::SetIcons(const std::vector<UINT> ImageData, HIMAGELIST hImageList)
	// Set the drop-down icons from an existing image list.
	// The image list could be a collection of icons
    {
        // Remove any existing imagelist
        if (m_hImageList)
        {
            ImageList_Destroy(m_hImageList);
            m_hImageList = NULL;
        }
        m_ImageData.clear();
        if (ImageData.size() == 0)
            return;

		int iImages = 0;
        for (unsigned int i = 0 ; i < ImageData.size(); i++)
        {
            if (ImageData[i] != 0)
            {
                m_ImageData.push_back(ImageData[i]);
                iImages++;
            }
        }

		// Set the button images
        if (ImageList_GetImageCount(hImageList) == iImages)
            m_hImageList = hImageList;
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

	void CMenubar::SysCommand(WPARAM wParam, LPARAM lParam)
	{
		if (wParam == SC_KEYMENU)
		{
			if (lParam == 0)
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
			if (OnDrawItem(wParam, lParam))
				return TRUE; // handled
			break;
		case WM_EXITMENULOOP:
			if (m_bExitAfter)
				ExitMenu();
			break;
		case WM_INITMENUPOPUP:
			OnInitMenuPopup(wParam, lParam);
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
		case WM_MDISETMENU:
			OnMDISetMenu(wParam, lParam);
			return 0L;
		case WM_MEASUREITEM:
			if (OnMeasureItem(wParam, lParam))
				return TRUE; // handled
			break;
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
		case WM_WINDOWPOSCHANGING:
			// Bypass CToolbar::WndProcDefault for this message
			return CWnd::WndProcDefault(hWnd, uMsg, wParam, lParam);

		} // switch (uMsg)

		return CToolbar::WndProcDefault(hWnd, uMsg, wParam, lParam);
	} // LRESULT CMenubar::WndProc(...)


	//////////////////////////////////
	// Definitions for the CFrame class
	//
	CFrame::CFrame() :  m_bIsMDIFrame(FALSE), m_bShowIndicatorStatus(TRUE), m_bShowMenuStatus(TRUE),
		                m_bUseRebar(FALSE), m_StatusText(_T("Ready")), m_hMenu(NULL), m_pView(NULL)//,
	{
		GetApp()->SetFrame(this);

		INITCOMMONCONTROLSEX InitStruct;
		InitStruct.dwSize = sizeof(INITCOMMONCONTROLSEX);
		InitStruct.dwICC = ICC_WIN95_CLASSES| ICC_COOL_CLASSES;

		// Do either InitCommonControls or InitCommonControlsEx
		LoadCommonControls(InitStruct);

		// By default, we use the rebar if we can
		if (GetComCtlVersion() >= 470)
			m_bUseRebar = TRUE;

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
		if (m_hMenu)
			::DestroyMenu(m_hMenu);
	}

	void CFrame::AddMenubarBand(int Menubar_Height /* = MENUBAR_HEIGHT*/)
	{
   		REBARBANDINFO rbbi = {0};
		SIZE sz = GetMenubar().GetMaxSize();

		rbbi.cbSize     = sizeof(REBARBANDINFO);
		rbbi.fMask      = RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_CHILD | RBBIM_SIZE;
		rbbi.cxMinChild = sz.cx;
		rbbi.cx         = sz.cx;
		rbbi.cyMinChild = Menubar_Height;
		rbbi.cyMaxChild = Menubar_Height;
		rbbi.fStyle     = RBBS_BREAK | RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS ;
		rbbi.hwndChild  = GetMenubar().GetHwnd();

		GetRebar().InsertBand(-1, &rbbi);
		SetMenubarBandSize();
	}

	void CFrame::AddToolbarBand(CToolbar& TB, std::vector<UINT> TBData, COLORREF clrMask, UINT ID_Normal, UINT ID_HOT, UINT ID_Disabled)
	{
		if (TBData.size() == 0)
		{
			DebugErrMsg(_T("Toolbar must have some data"));
			return;
		}

		// Create the Toolbar Window
		TB.Create(GetRebar().GetHwnd());
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
		rbbi.hwndChild  = TB.GetHwnd();

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
		if (IsRebarSupported() && m_bUseRebar)
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

	int CFrame::GetMenuItemPos(HMENU hMenu, LPCTSTR szItem)
	// Returns the position of the menu item, given it's name
	{
		int nMenuItemCount = GetMenuItemCount(hMenu);
		MENUITEMINFO mii = {0};

		// For Win95 and NT, cbSize needs to be 44
		if ((GetWinVersion() == 1400) || (GetWinVersion() == 2400))
			mii.cbSize = 44;
		else
			mii.cbSize = sizeof(MENUITEMINFO);

		for (int nItem = 0 ; nItem < nMenuItemCount; nItem++)
		{
			TCHAR szStr[MAX_MENU_STRING +1];
			TCHAR szStripped[MAX_MENU_STRING +1];

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
				if (lstrcmp(szStripped, szItem) == 0)
					return nItem;
			}
		}

		return -1;
	}

	void CFrame::LoadCommonControls(INITCOMMONCONTROLSEX InitStruct)
	{
		try
		{
			// Load the Common Controls DLL
			HMODULE hComCtl = ::LoadLibrary(_T("COMCTL32.DLL"));
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
		}

		catch (...)
		{
			DebugErrMsg(_T("Exception in CFrame::LoadCommonControls"));
		}
	}

	BOOL CFrame::OnCommandFrame(WPARAM wParam, LPARAM /*lParam*/)
	{
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
			GetMenubar().Create(GetRebar().GetHwnd());
			GetMenubar().SetMenu(m_hMenu);
			AddMenubarBand();

			// Create the toolbar inside rebar
			AddToolbarBand(GetToolbar(), m_ToolbarData, RGB(192, 192, 192), IDW_MAIN);

			// Set the icons for popup menu items
			GetMenubar().SetIcons(m_ToolbarData, IDW_MAIN, RGB(192, 192, 192));
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

		if (!IsMenubarUsed())
			::SetMenu(m_hWnd, m_hMenu);

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

	void CFrame::OnHelp()
	{
		static BOOL IsHelpOpen = FALSE;
		if (!IsHelpOpen)
		{
			// Store the window handle that currently has keyboard focus
			HWND hPrevFocus = ::GetFocus();
			if (hPrevFocus == GetMenubar().GetHwnd())
				hPrevFocus = m_hWnd;

			IsHelpOpen = TRUE;
			CDialog AboutDialog(IDW_ABOUT, m_hWnd);
			AboutDialog.DoModal();

			// Return the keyboard focus back to the window that had it
			::SetFocus(hPrevFocus);
			IsHelpOpen = FALSE;
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
			if (GetRebar().GetTheme().UseThemes && GetRebar().GetTheme().KeepBandsLeft)
				GetRebar().MoveBandsLeft();
			break;
		case RBN_MINMAX:
			if (GetRebar().GetTheme().UseThemes && GetRebar().GetTheme().ShortBands)
				return 1L;	// Supress maximise or minimise rebar band
			break;
	//	case RBN_CHILDSIZE :
	//		if (GetRebar().GetTheme().UseThemes && GetRebar().GetTheme().ShortBands)
	//			GetRebar().MoveBandsLeft();
	//		break;

		// Display toolips for the toolbar
		case TTN_GETDISPINFO:
			{
				int iIndex =  GetToolbar().HitTest();
				LPNMTTDISPINFO lpDispInfo = (LPNMTTDISPINFO)lParam;
				if (iIndex >= 0)
				{
					int nID = GetToolbar().GetCommandID(iIndex);
					if (nID > 0)
						lpDispInfo->lpszText = (LPTSTR)LoadString(nID);
				}
			}
			break;
		} // switch LPNMHDR

		return 0;

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

		//Reposition and redraw everything
		RecalcLayout();
		::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);
	}

	void CFrame::OnTimer(WPARAM wParam)
	{
		if (wParam == ID_STATUS_TIMER)
		{
			if (m_bShowMenuStatus)
			{
				static int nOldID = -1;
				CToolbar& tb = GetToolbar();
				POINT pt = {0};
				::GetCursorPos(&pt);

				int nButton;
				if (WindowFromPoint(pt) == tb.GetHwnd())
				{
					nButton = tb.HitTest();
					if (nButton >= 0)
					{
						int nID = GetToolbar().GetCommandID(nButton);
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
		if (::IsWindowVisible(GetStatusbar().GetHwnd()))
		{
			::ShowWindow(GetStatusbar().GetHwnd(), SW_HIDE);
		}
		else
		{
			::ShowWindow(GetStatusbar().GetHwnd(), SW_SHOW);
		}

		UpdateCheckMarks();

		// Reposition the Windows
		RecalcLayout();
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}

	void CFrame::OnViewToolbar()
	{
		if (::IsWindowVisible(GetToolbar().GetHwnd()))
		{
			if (IsMenubarUsed())
				::SendMessage(GetRebar().GetHwnd(), RB_SHOWBAND, m_Rebar.GetBand(GetToolbar().GetHwnd()), FALSE);
			else
				::ShowWindow(GetToolbar().GetHwnd(), SW_HIDE);
		}
		else
		{
			if (IsMenubarUsed())
				::SendMessage(GetRebar().GetHwnd(), RB_SHOWBAND, m_Rebar.GetBand(GetToolbar().GetHwnd()), TRUE);
			else
				::ShowWindow(GetToolbar().GetHwnd(), SW_SHOW);
		}

		UpdateCheckMarks();

		// Reposition the Windows
		RecalcLayout();
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}

  	void CFrame::PreCreate(CREATESTRUCT& cs)
	{
		try
		{
			if (m_pView == NULL)
				throw CWinException(_T("CFrame::PreCreate ... m_pView is NULL\n\nUse SetView to set the View Window"));

			// Set the Window Class
			cs.lpszClass = _T("Win32++ Frame");

			// Set the caption from the string resource
			cs.lpszName = LoadString(IDW_MAIN);

			// Set the frame window styles
			cs.style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(_T("Exception in CFrame::PreCreate"));
		}
	}

	void CFrame::RecalcLayout()
	{
		if ((!m_pView) || (!m_pView->GetHwnd()))
			return;

		// Resize the status bar
		HWND hStatusbar = GetStatusbar().GetHwnd();
		if (hStatusbar)
		{
			::SendMessage(hStatusbar, WM_SIZE, 0, 0);
			::InvalidateRect(hStatusbar, NULL, TRUE);
		}

		// Reposition the text
		SetStatusText();

		// Resize the rebar or toolbar
		CRebar& RB = GetRebar();
		if (IsRebarUsed())
		{
			::SendMessage(RB.GetHwnd(), WM_SIZE, 0, 0);
		}
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

		if (RB.GetTheme().UseThemes && RB.GetTheme().KeepBandsLeft)
			RB.MoveBandsLeft();

		if (IsMenubarUsed())
			SetMenubarBandSize();

		::SendMessage(m_hWnd, USER_REARRANGED, 0, 0);
	}

	void CFrame::SetBackground(HBITMAP hBackground)
	{
		for (int nBands = 0; nBands < GetRebar().GetBandCount(); nBands++)
			GetRebar().SetBandBitmap(nBands, hBackground);
	}

	void CFrame::SetFrameMenu(INT ID_MENU)
	{
		if (m_hMenu)
			::DestroyMenu(m_hMenu);

		m_hMenu = ::LoadMenu(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(ID_MENU));

		if(!m_hMenu)
			DebugWarnMsg(_T("Load Menu failed"));
 	}

	void CFrame::SetMenubarBandSize()
	{
		// Sets the minimum width of the Menubar band to the width of the rebar
		// This prevents other bands from moving to this Menubar's row.

		RECT rc = {0};
		GetClientRect(GetRebar().GetHwnd(), &rc);
		int Width = rc.right - rc.left;
		CRebar& RB = GetRebar();
		int nBand = RB.GetBand(GetMenubar().GetHwnd());
		rc = RB.GetBandBorders(nBand);
		Width = Width - rc.left - rc.right - 2;

		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(REBARBANDINFO);
		rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_SIZE;
		RB.GetBandInfo(nBand, &rbbi);
		if (GetRebar().GetTheme().UseThemes)
		{
			rbbi.cxMinChild = Width;
			rbbi.cx         = Width;
		}

		RB.SetBandInfo(nBand, &rbbi);
	}

	void CFrame::SetStatusIndicators()
	{
		if (::IsWindow(GetStatusbar().GetHwnd()))
		{
			static LPCTSTR OldStatus1 = NULL;
			static LPCTSTR OldStatus2 = NULL;
			static LPCTSTR OldStatus3 = NULL;

			LPCTSTR Status1 = (::GetKeyState(VK_CAPITAL) & 0x0001)? _T("\tCAP") : _T("");
			LPCTSTR Status2 = (::GetKeyState(VK_NUMLOCK) & 0x0001)? _T("\tNUM") : _T("");
			LPCTSTR Status3 = (::GetKeyState(VK_SCROLL)  & 0x0001)? _T("\tSCRL"): _T("");

			// Only update indictors if the text has changed
			if (Status1 != OldStatus1) 	GetStatusbar().SetPaneText(1, (Status1));
			if (Status2 != OldStatus2)  GetStatusbar().SetPaneText(2, (Status2));
			if (Status3 != OldStatus3)  GetStatusbar().SetPaneText(3, (Status3));

			OldStatus1 = Status1;
			OldStatus2 = Status2;
			OldStatus3 = Status3;
		}
	}

	void CFrame::SetStatusText()
	{
		if (::IsWindow(GetStatusbar().GetHwnd()))
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

	void CFrame::SetView(CWnd& View)
	{
		// You call this function to assign the View window object to the frame
		m_pView = &View;
	}

	void CFrame::UpdateCheckMarks()
	{
		if (::IsWindowVisible(GetToolbar().GetHwnd()))
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

		if (::IsWindowVisible(GetStatusbar().GetHwnd()))
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
		try
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
					GetMenubar().SysCommand(wParam, lParam);
					return 0L;
				}
				break;
			case WM_TIMER:
				OnTimer(wParam);
				return 0L;
			} // switch uMsg

		} // try

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(_T("Exception in CFrame::WndProc"));
		}

		return CWnd::WndProcDefault(hWnd, uMsg, wParam, lParam);
	} // LRESULT CFrame::WndProcDefault(...)

} // namespace Win32xx

