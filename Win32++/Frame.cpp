// Win32++  Version 5.11 beta
// Released: 27th February, 2007 by:
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
					throw CWinException (TEXT("CStatusbar::CreatePanes ... Too many panes"));

				// Create the statusbar panes
				if (!::SendMessage(m_hWnd, SB_SETPARTS, iPanes, (LPARAM)iPaneWidths))
					throw CWinException(TEXT("CStatusbar::CreatePanes failed"));
			}
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CStatusbar::CreatePanes"));
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
						throw CWinException(TEXT("Failed to set status bar text"));
				}
			}
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CStatusbar::SetPaneText"));
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
					throw CWinException (TEXT("CStatusbar::SetPaneWidth ... Invalid parameters"));

				if (iPane < 0) iPane = 0;

				int iParts = (int)::SendMessage(m_hWnd, SB_GETPARTS, 0, 0);
				int* iPaneWidths = new int[iParts];
				::SendMessage(m_hWnd, SB_GETPARTS, iParts, (LPARAM)iPaneWidths);

				//int iNewParts = max(iPane+1, iParts);
				int iNewParts = (iPane+1 > iParts) ? iPane+1 : iParts;

				int* iNewPaneWidths = new int[iNewParts];
				ZeroMemory(iNewPaneWidths, iNewParts*sizeof(int));

				for (int i = 0; i < iParts; i++)
					iNewPaneWidths[i] = iPaneWidths[i];

				if (iPane == 0)
					iNewPaneWidths[iPane] = iWidth;
				else
					iNewPaneWidths[iPane] = iNewPaneWidths[iPane -1] + iWidth;

				if (!::SendMessage(m_hWnd, SB_SETPARTS, iNewParts, (LPARAM)iNewPaneWidths))
					throw CWinException(TEXT("CStatusbar::SetPaneWidth failed"));

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
			DebugErrMsg(TEXT("Exception in CStatusbar::SetPaneWidth"));
		}
	}


	////////////////////////////////////
	// Definitions for the CToolbar class
	//
	CToolbar::CToolbar() : m_hImageList(NULL), m_hImageListHot(NULL), m_hImageListDis(NULL)
	{
		Superclass(TOOLBARCLASSNAME, TEXT("Toolbar"));
	}

	CToolbar::~CToolbar()
	{
		if (m_hImageList)     ::DeleteObject(m_hImageList);
		if (m_hImageListHot)  ::DeleteObject(m_hImageListHot);
		if (m_hImageListDis)  ::DeleteObject(m_hImageListDis);
	}

	int CToolbar::CommandToIndex(int iButtonID)
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
				throw CWinException(TEXT("CToolbar::AddBitmap  TB_ADDBITMAP failed"));
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CToolbar::AddBitmap"));
		}
	}

	void CToolbar::DisableButton(int iButtonID)
	{
		// An example of iButtonID would be IDM_FILE_OPEN
		if (!::SendMessage(m_hWnd, TB_ENABLEBUTTON, (WPARAM)iButtonID, (LPARAM) MAKELONG(FALSE, 0)))
			DebugWarnMsg(TEXT("Disable button failed"));
	}

	void CToolbar::EnableButton(int iButtonID)
	{
		if (!::SendMessage(m_hWnd, TB_ENABLEBUTTON, (WPARAM)iButtonID, (LPARAM) MAKELONG(TRUE,0 )))
			DebugWarnMsg(TEXT("Enable button failed"));
	}

	int CToolbar::GetButtonCount()
	{
		return (int)::SendMessage(m_hWnd, TB_BUTTONCOUNT, 0, 0);
	}

	UINT CToolbar::GetButtonState(int iButtonID)
	{
		try
		{
			LRESULT lResult= ::SendMessage(m_hWnd, TB_GETSTATE, (WPARAM) iButtonID, 0);
			if (lResult == -1)
				throw CWinException(TEXT("CToolbar::GetButtonState failed"));

			return (UINT) lResult;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CToolbar::GetButtonState"));
		}
		return 0;
	}

	BYTE CToolbar::GetButtonStyle(int iButtonID)
	{
		try
		{
			int iIndex = CommandToIndex(iButtonID);
			if (iIndex == -1)
				throw CWinException(TEXT("CToolbar::GetButtonStyle failed to get command ID"));

			TBBUTTON tbb = {0};

			LRESULT lResult = ::SendMessage(m_hWnd, TB_GETBUTTON, iIndex, (LPARAM) &tbb);
			if (lResult == -1)
				throw CWinException(TEXT("CToolbar::GetButtonStyle failed"));

			return tbb.fsStyle;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CToolbar::GetButtonStyle"));
		}

		return 0;
	}

	int CToolbar::GetCommandID(int iIndex)
	{
		TBBUTTON tbb = {0};
		::SendMessage(m_hWnd, TB_GETBUTTON, iIndex, (WPARAM) &tbb);

		// returns zero if failed
		return tbb.idCommand;
	}

	void CToolbar::GetItemRect(int iIndex, RECT* lpRect)
	{
		ZeroMemory(lpRect, sizeof(RECT));
		int iCount = (int)::SendMessage(m_hWnd, TB_BUTTONCOUNT, 0, 0);

		if (iCount >= iIndex)
			::SendMessage(m_hWnd, TB_GETITEMRECT, (WPARAM)iIndex, (LPARAM)lpRect);
	}

	int CToolbar::HitTest()
	{
		// We do our own hit test since TB_HITTEST is a bit buggy,
		// and also doesn't work at all on earliest versions of Win95

		POINT pt = {0};
		::GetCursorPos(&pt);
		::ScreenToClient(m_hWnd, &pt);
		RECT r;

		int nButtons = (int)::SendMessage(m_hWnd, TB_BUTTONCOUNT, 0, 0);
		int iButton = -1;

		for (int i = 0 ; i < nButtons; i++)
		{
			::SendMessage(m_hWnd, TB_GETITEMRECT, i, (LPARAM) &r);
			if (::PtInRect(&r, pt))
				iButton = i;
		}

		return iButton;
	}

	void CToolbar::OnInitialUpdate()
	{
		// We must send this message before sending the TB_ADDBITMAP or TB_ADDBUTTONS message
		::SendMessage(m_hWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

		// allows buttons to have a separate dropdown arrow
		// Note: TBN_DROPDOWN notification is sent by a toolbar control when the user clicks a dropdown button
		::SendMessage(m_hWnd, TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_DRAWDDARROWS);
	}

	void CToolbar::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT;
		cs.lpszClass = TEXT("Toolbar");

		// Add extra styles for toolbars inside a rebar
		CFrame* pFrame = GetApp()->GetFrame();
		if (pFrame->IsRebarUsed())
			cs.style |= CCS_NODIVIDER | CCS_NORESIZE;
	}

	void CToolbar::SetBitmapSize(int cx, int cy)
	// Call this function when the image size is not the default 16 x 15
	// Call this function before adding a bitmap
	{
		if (!::SendMessage(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELONG(cx, cy)))
			DebugWarnMsg(TEXT("CToolbar::SetBitmapSize  failed"));
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
						tbb.iBitmap = iImages++;
						tbb.idCommand = ToolbarData[j];
						tbb.fsState = TBSTATE_ENABLED;
						tbb.fsStyle = TBSTYLE_BUTTON;
					}

					// Add the button to the toolbar
					if (!::SendMessage(m_hWnd, TB_ADDBUTTONS, 1, (LPARAM)&tbb))
						throw (CWinException(TEXT("CToolbar::SetButtons  .. TB_ADDBUTTONS failed ")));
				}

				// Set rows of text to zero
				::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, 0, 0);
			}
			else
				DebugWarnMsg(TEXT("No Resource IDs for Toolbar"));

			return iImages;
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CToolbar::SetButtons"));
		}

		return 0;
	}

	void CToolbar::SetButtonSize(int cx, int cy)
	{

		if (!::SendMessage(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELONG(cx, cy)))
			DebugWarnMsg(TEXT("CToolbar::SetButtonSize failed"));

		// Resize the rebar band containing the toolbar
		if (GetApp()->GetFrame()->IsRebarUsed())
		{
			CRebar* rb = (CRebar*) GetCWndObject(m_hWndParent);
			rb->ResizeBand(rb->GetBand(GetHwnd()), cy+2);
		}

		::InvalidateRect(m_hWnd, NULL, TRUE);
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
			DebugWarnMsg(TEXT("CToolbar::SetButtonState failed"));
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
				throw CWinException(TEXT("CToolbar::SetButtonStyle  failed"));
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CToolbar::SetButtonStyle"));
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
				throw CWinException(TEXT("CToolbar::SetButtonText  failed to get Command ID"));

			std::basic_string<TCHAR> sString = szText;
			std::map<std::basic_string<TCHAR>, int>::iterator m;
			int iString;

			// Check to see if the string is already added
			m = m_StringMap.find(sString);
			if (m == m_StringMap.end())
			{
				if (m_StringMap.size() == 0)
				{
					// Place a blank string first in the string table, in case some
					// buttons don't have text
					TCHAR szString[2] = TEXT("");
					szString[1] = TEXT('\0');
					::SendMessage(m_hWnd, TB_ADDSTRING, 0, (LPARAM)szString);
				}

				// No index for this string exists, so create it now
				TCHAR szBuf[80];
				lstrcpyn(szBuf, szText, 79);
				szBuf[lstrlen(szBuf)+1] = TEXT('\0');		// Double-null terminate

				iString = (int)::SendMessage(m_hWnd, TB_ADDSTRING, 0, (LPARAM)szBuf);
				if (iString == -1)
					throw CWinException(TEXT("CToolbar::SetButtonText  TB_ADDSTRING failed"));

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
				throw CWinException(TEXT("CToolbar::SetButtonText  TB_GETBUTTON failed"));

			tbb.iString = iString;

			// Turn off Toolbar drawing
			::SendMessage(m_hWnd, WM_SETREDRAW, FALSE, 0);

			if (!::SendMessage(m_hWnd, TB_DELETEBUTTON, iIndex, 0))
				throw CWinException(TEXT("CToolbar::SetButtonText  TB_DELETEBUTTON failed"));

			if (!::SendMessage(m_hWnd, TB_INSERTBUTTON, iIndex, (LPARAM)&tbb))
				throw CWinException(TEXT("CToolbar::SetButtonText  TB_INSERTBUTTON failed"));

			// Ensure the button now includes some text rows (5 rows should be plenty)
			if (::SendMessage(m_hWnd, TB_GETTEXTROWS, 0, 0) == 0)
				::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, 5, 0);

			// Turn on Toolbar drawing
			::SendMessage(m_hWnd, WM_SETREDRAW, TRUE, 0);

			// Redraw button
			RECT r;
			if (::SendMessage(m_hWnd, TB_GETITEMRECT, iIndex, (LPARAM)&r))
				::InvalidateRect(m_hWnd, &r, TRUE);
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CToolbar::SetButtonText"));
		}
	}

	void CToolbar::SetCommandID(int iIndex, int iButtonID)
	{
		try
		{
			if (!::SendMessage(m_hWnd, TB_SETCMDID, iIndex, iButtonID))
				throw CWinException(TEXT("CToolbar::SetCommandID failed"));
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CToolbar::SetButtonText"));
		}
	}

	void CToolbar::SetImageList(int iNumButtons, COLORREF crMask, UINT ToolbarID, UINT ToolbarHotID, UINT ToolbarDisabledID)
	// This function assumes the width of the button image = bitmap_size / buttons
	// This colour mask is often grey RGB(192,192,192) or magenta (255,0,255);
	// ToolbarHotID and ToolbarDisabledID can be 0
	{
		try
		{
			if (iNumButtons > 0)
			{
				// Toolbar ImageLists require Comctl32.dll version 4.7 or later
				if (!GetApp()->GetFrame()->m_bSupportRebars)
				{
					// We are using COMCTL32.DLL version 4.0, so we can't use an imagelist.
					// Instead we simply add the bitmap.
					AddBitmap(iNumButtons, ToolbarID);
					return;
				}

				// Set the button images
				HBITMAP hbm = ::LoadBitmap(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(ToolbarID));
				BITMAP bm = {0};

				::GetObject(hbm, sizeof(BITMAP), &bm);
				int iImageWidth  = bm.bmWidth / iNumButtons;
				int iImageHeight = bm.bmHeight;

				m_hImageList = ImageList_Create(iImageWidth, iImageHeight, ILC_COLORDDB | ILC_MASK, iNumButtons, 0);
				ImageList_AddMasked(m_hImageList, hbm, crMask);
				::DeleteObject(hbm);

				if(SendMessage(m_hWnd, TB_SETIMAGELIST, 0, (LPARAM)m_hImageList) == -1)
					throw CWinException(TEXT("CToolbar::SetImageList ... TB_SETIMAGELIST failed "));

				if (ToolbarHotID)
				{
					hbm = ::LoadBitmap(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(ToolbarHotID));
					m_hImageListHot = ImageList_Create(iImageWidth, iImageHeight, ILC_COLORDDB | ILC_MASK, iNumButtons, 0);
					ImageList_AddMasked(m_hImageListHot, hbm, crMask);
					::DeleteObject(hbm);
					if(SendMessage(m_hWnd, TB_SETHOTIMAGELIST, 0, (LPARAM)m_hImageListHot) == -1)
						throw CWinException(TEXT("CToolbar::SetImageList ... TB_SETHOTIMAGELIST failed "));
				}

				if (ToolbarDisabledID)
				{
					hbm = ::LoadBitmap(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(ToolbarDisabledID));
					m_hImageListDis = ImageList_Create(iImageWidth, iImageHeight, ILC_COLORDDB | ILC_MASK, iNumButtons, 0);
					ImageList_AddMasked(m_hImageListDis, hbm, crMask);
					::DeleteObject(hbm);
					if(SendMessage(m_hWnd, TB_SETDISABLEDIMAGELIST, 0, (LPARAM)m_hImageListDis) == -1)
						throw CWinException(TEXT("CToolbar::SetImageList ... TB_SETDISABLEDIMAGELIST failed "));
				}
			}
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CToolbar::SetImageList"));
		}
	}

	LRESULT CToolbar::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		CFrame* pFrame = GetApp()->GetFrame();

		switch (uMsg)
		{
		case WM_MOUSEMOVE:
			// Tell the frame which button the mouse is over
			if (pFrame->m_bShowMenuStatus)
				pFrame->ToolbarNotify(HitTest());
			break;

		case WM_MOUSELEAVE:
			if (pFrame->m_bShowMenuStatus)
				pFrame->ToolbarNotify(-1);
			break;
		}
		return CWnd::WndProc(hwnd, uMsg, wParam, lParam);
	}

	///////////////////////////////////
	// Definitions for the CRebar class
	//
	CRebar::CRebar()
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

	BOOL CRebar::GetBarInfo(LPREBARINFO prbi) const
	{
		// REBARINFO describes overall rebar control characteristics
		prbi->cbSize = sizeof(REBARINFO);
		return (BOOL)::SendMessage(m_hWnd, RB_GETBARINFO, 0, (LPARAM)prbi);
	}

	BOOL CRebar::InsertBand(const int nBand, LPREBARBANDINFO prbbi)
	{
		return (BOOL)::SendMessage(m_hWnd, RB_INSERTBAND, nBand, (LPARAM)(LPREBARBANDINFO)prbbi);
	}

	void CRebar::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS |
                         CCS_NODIVIDER | RBS_VARHEIGHT | RBS_BANDBORDERS;

		cs.lpszClass = REBARCLASSNAME;
	}

	void CRebar::ResizeBand(const int nBand, const int nSize)
	{
		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof(rbbi);
		rbbi.fMask = RBBIM_CHILDSIZE;

		// Alter the maximum size of the band
		GetBandInfo(nBand, &rbbi);
		rbbi.cyMinChild = nSize;
		rbbi.cyMaxChild = nSize;
		SetBandInfo(nBand, &rbbi );

		// Resize the band to the new maximum size
		::SendMessage(m_hWnd, RB_MAXIMIZEBAND, nBand, 0);
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
		CMDIFrame* pMDIFrame = (CMDIFrame*)GetApp()->GetFrame();
		BOOL bMaxed = FALSE;

		if (!pMDIFrame->IsMDIFrame())
			return;

		pMDIFrame->GetActiveChild(&bMaxed);

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

	void CMenubar::MenuChar(WPARAM wParam, LPARAM /* lParam */)
	{
		if (!m_bMenuActive)
			DoAltKey(LOWORD(wParam));
	}

	void CMenubar::OnInitialUpdate()
	{
		// We must send this message before sending the TB_ADDBITMAP or TB_ADDBUTTONS message
		::SendMessage(m_hWnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	}

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
					CMDIFrame* pMDIFrame = (CMDIFrame*)GetApp()->GetFrame();
					HICON hIcon = (HICON)::SendMessage(pMDIFrame->GetActiveChild(), WM_GETICON, ICON_SMALL, 0);
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

		CMDIFrame* pMDIFrame = (CMDIFrame *)GetApp()->GetFrame();
		if (pMDIFrame->IsMDIFrame())
		{
			BOOL bMaxed;
			HWND MDIChild = pMDIFrame->GetActiveChild(&bMaxed);

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

		CMDIFrame* pMDIFrame = (CMDIFrame*)GetApp()->GetFrame();
		if (pMDIFrame->IsMDIFrame())
		{
			BOOL bMaxed;
			HWND MDIClient = pMDIFrame->GetMDIClient().GetHwnd();
			HWND MDIChild = pMDIFrame->GetActiveChild(&bMaxed);

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
					::wsprintf(szString, TEXT("&%d %s"), nWindow+1, szTitle);
					::AppendMenu(hMenuWindow, MF_STRING, IDW_FIRSTCHILD + nWindow, szString );

					if (pMDIFrame->GetActiveChild() == hWndMDIChild)
						::CheckMenuItem(hMenuWindow, IDW_FIRSTCHILD+nWindow, MF_CHECKED);

					nWindow++;
				}
				else if (nWindow == 9)
				// For the 10th MDI child, add this menu item and return
				{
					::AppendMenu(hMenuWindow, MF_STRING, IDW_FIRSTCHILD + nWindow, TEXT("&Windows..."));
					return;
				}
			}
		}
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

	void CMenubar::OnMouseMove(WPARAM wParam, LPARAM lParam)
	{
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		if (wParam == MK_LBUTTON)  // mouse moved with left mouse button is held down
		{
			CMDIFrame* pMDIFrame = (CMDIFrame*)GetApp()->GetFrame();
			if (pMDIFrame->IsMDIFrame())
			{
				BOOL bMaxed;
				pMDIFrame->GetActiveChild(&bMaxed);

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
				tbb.iString = (INT_PTR)TEXT(" ");
				if(!::SendMessage(m_hWnd, TB_ADDBUTTONS, 1, (WPARAM)&tbb))
					throw CWinException(TEXT("Menubar::SetMenu  TB_ADDBUTTONS failed"));

				SetButtonText(0, TEXT("    "));
			}

			for (int i = 0 ; i < m_nButtonCount; i++)
			{
				// Assign the Toolbar Button struct
				TBBUTTON tbb = {0};
				tbb.idCommand = i  + m_nMaxedFlag;
				tbb.fsState = TBSTATE_ENABLED;
				tbb.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE | TBSTYLE_DROPDOWN;
				tbb.iString = (INT_PTR)TEXT(" ");
				if (!::SendMessage(m_hWnd, TB_ADDBUTTONS, 1, (WPARAM)&tbb))
					throw CWinException(TEXT("Menubar::SetMenu  TB_ADDBUTTONS failed"));

				// Add the menu title to the string table
				TCHAR szMenuName[MAX_MENU_STRING +1];
				if (::GetMenuString(hMenu, i, szMenuName, MAX_MENU_STRING, MF_BYPOSITION) == 0)
					throw CWinException(TEXT("Menubar::SetMenu  GetMenuString failed"));

				SetButtonText(i  + m_nMaxedFlag, szMenuName);
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
					CMDIFrame* pMDIFrame = (CMDIFrame*)GetApp()->GetFrame();
					HWND MDIChild = pMDIFrame->GetActiveChild();
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

	void CMenubar::SysCommand(WPARAM wParam, LPARAM lParam)
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
		case WM_MDISETMENU:
			OnMDISetMenu(wParam, lParam);
			return 0L;
		case WM_MOUSEMOVE:
			OnMouseMove(wParam, lParam);
			break;
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

		// Don't call CToolbar::WndProc. Call CWnd::Wndproc instead
		return CWnd::WndProc(hwnd, uMsg, wParam, lParam);
	} // LRESULT CMenubar::WndProc(...)


	//////////////////////////////////
	// Definitions for the CFrame class
	//
	CFrame::CFrame() :  m_bShowIndicatorStatus(TRUE), m_bShowMenuStatus(TRUE), m_bUseRebar(FALSE),
		                m_StatusText(TEXT("Ready")), m_bIsMDIFrame(FALSE), m_bSupportRebars(FALSE),
						m_pView(NULL)
	{
		GetApp()->SetFrame(this);
		INITCOMMONCONTROLSEX InitStruct;
		InitStruct.dwSize = sizeof(INITCOMMONCONTROLSEX);
		InitStruct.dwICC = ICC_WIN95_CLASSES| ICC_COOL_CLASSES;

		// Do either InitCommonControls or InitCommonControlsEx
		LoadCommonControls(InitStruct);

		m_hMenu = ::LoadMenu(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDW_MAIN));

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
		::DestroyMenu(m_hMenu);
	}

	void CFrame::AddMenubarBand(int Menubar_Height /* = MENUBAR_HEIGHT*/)
	{
   		REBARBANDINFO rbbi = {0};

		rbbi.cbSize     = sizeof(REBARBANDINFO);
		rbbi.fMask      = RBBIM_COLORS | RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_CHILD;
		rbbi.cyMinChild = Menubar_Height;
		rbbi.cyMaxChild = Menubar_Height;
		rbbi.fStyle     = RBBS_BREAK | RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS ;
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
				// We are using COMCTL32.dll version 4.0
				::InitCommonControls();
			else
			{
				// We are using COMCTL32.dll version 4.7 or later
				// Call InitCommonControlsEx
				if(!((*pfnInit)(&InitStruct)))
					throw CWinException(TEXT("CFrame::LoadCommonControls ... InitCommonControlsEx failed"));
				m_bSupportRebars = TRUE;
				m_bUseRebar     = TRUE;
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
		case IDW_VIEW_STATUSBAR:
			OnViewStatusbar();
			break;
		case IDW_VIEW_TOOLBAR:
			OnViewToolbar();
			break;
		} // switch cmd

		return 0;
	} // CFrame::OnCommand(...)

	void CFrame::OnCreate()
	{
		// Set the icon
		SetIconLarge(IDW_MAIN);
		SetIconSmall(IDW_MAIN);

		if (m_bSupportRebars && m_bUseRebar)
		{
			// Create the rebar
			GetRebar().Create(m_hWnd);

			// Create the menu
			GetMenubar().Create(GetRebar().GetHwnd());
			GetMenubar().SetMenu(m_hMenu);
			AddMenubarBand();

			// Create tool bar
			GetToolbar().Create(GetRebar().GetHwnd());
			AddToolbarBand();
		}
		else
			GetToolbar().Create(m_hWnd);

		if (!IsMenubarUsed())
			::SetMenu(m_hWnd, m_hMenu);

		// Load the toolbar buttons
		SetButtons(m_ToolbarData);

		// Create the status bar
		GetStatusbar().Create(m_hWnd);

		// Create the view window
		m_pView->Create(m_hWnd);

		// Start timer for Status Indicator updates
		if (m_bShowIndicatorStatus)
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
			if ((hMenu != ::GetMenu(m_hWnd)) && (nID != 0))
				m_StatusText = LoadString(nID);
			else
				m_StatusText = TEXT("Ready");
			SetStatusText();
		}
	}

	LRESULT CFrame::OnNotify(WPARAM wParam, LPARAM lParam)
	{
		switch (((LPNMHDR)lParam)->code)
		{
		case RBN_HEIGHTCHANGE:
			RecalcLayout();
			::InvalidateRect(m_hWnd, NULL, TRUE);
			break;

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
				::CheckMenuItem(GetMenubar().GetMenu(), IDW_VIEW_STATUSBAR, MF_UNCHECKED);
			else
			{
				::CheckMenuItem (m_hMenu, IDW_VIEW_STATUSBAR, MF_UNCHECKED);
				if (::GetMenu(m_hWnd) != m_hMenu)
					::CheckMenuItem (::GetMenu(m_hWnd), IDW_VIEW_STATUSBAR, MF_UNCHECKED);
			}

			::ShowWindow(GetStatusbar().GetHwnd(), SW_HIDE);
		}
		else
		{
			if (IsMenubarUsed())
				::CheckMenuItem(GetMenubar().GetMenu(), IDW_VIEW_STATUSBAR, MF_CHECKED);
			else
			{
				::CheckMenuItem (m_hMenu, IDW_VIEW_STATUSBAR, MF_CHECKED);
				if (::GetMenu(m_hWnd) != m_hMenu)
					::CheckMenuItem (::GetMenu(m_hWnd), IDW_VIEW_STATUSBAR, MF_CHECKED);
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
				::CheckMenuItem(GetMenubar().GetMenu(), IDW_VIEW_TOOLBAR, MF_UNCHECKED);
				::SendMessage(GetRebar().GetHwnd(), RB_SHOWBAND, m_Rebar.GetBand(GetToolbar().GetHwnd()), FALSE);
			}
			else
			{
				::CheckMenuItem (m_hMenu, IDW_VIEW_TOOLBAR, MF_UNCHECKED);
				if (::GetMenu(m_hWnd) != m_hMenu)
					::CheckMenuItem (::GetMenu(m_hWnd), IDW_VIEW_TOOLBAR, MF_UNCHECKED);
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
				::CheckMenuItem(GetMenubar().GetMenu(), IDW_VIEW_TOOLBAR, MF_CHECKED);
				::SendMessage(GetRebar().GetHwnd(), RB_SHOWBAND, m_Rebar.GetBand(GetToolbar().GetHwnd()), TRUE);
			}
			else
			{
				::CheckMenuItem (m_hMenu, IDW_VIEW_TOOLBAR, MF_CHECKED);
				if (::GetMenu(m_hWnd) != m_hMenu)
					::CheckMenuItem (::GetMenu(m_hWnd), IDW_VIEW_TOOLBAR, MF_CHECKED);
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
			DebugErrMsg(TEXT("Exception in CFrame::PreCreate"));
		}
	}

	void CFrame::RecalcLayout()
	{
		// Resize the status bar
		::SendMessage(GetStatusbar().GetHwnd(), WM_SIZE, 0, 0);
		::InvalidateRect(GetStatusbar().GetHwnd(), NULL, TRUE);

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

	void CFrame::SetButtons(const std::vector<UINT> ToolbarData)
	// Define the resource IDs for the toolbar like this in the Frame's constructor
	// m_ToolbarData.clear();
	// m_ToolbarData.push_back ( IDM_FILE_NEW   );
	// m_ToolbarData.push_back ( IDM_FILE_OPEN  );
	// m_ToolbarData.push_back ( IDM_FILE_SAVE  );
	// m_ToolbarData.push_back ( 0 );				// Separator
	// m_ToolbarData.push_back ( IDM_HELP_ABOUT );
	{
		int iButtons = GetToolbar().SetButtons(ToolbarData);
		GetToolbar().SetImageList(iButtons, RGB(192,192,192), IDW_MAIN, 0, 0);
	}

	void CFrame::SetStatusIndicators()
	{
		if (::IsWindow(GetStatusbar().GetHwnd()))
		{
			static LPCTSTR OldStatus1 = NULL;
			static LPCTSTR OldStatus2 = NULL;
			static LPCTSTR OldStatus3 = NULL;

			LPCTSTR Status1 = (::GetKeyState(VK_CAPITAL) & 0x0001)? TEXT("\tCAP") : TEXT("");
			LPCTSTR Status2 = (::GetKeyState(VK_NUMLOCK) & 0x0001)? TEXT("\tNUM") : TEXT("");
			LPCTSTR Status3 = (::GetKeyState(VK_SCROLL)  & 0x0001)? TEXT("\tSCRL"): TEXT("");

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

	void CFrame::ToolbarNotify(int nButton)
	// Called in CToolbar::WndProc
	{
		// Updates the statusbar when the mouse hovers over a toolbar buttton
		static int nOldID = -1;
		if (nButton >= 0)
		{
			int nID = GetToolbar().GetCommandID(nButton);
			if (nID != nOldID)
			{
				if (nID != 0)
					m_StatusText = LoadString(nID);
				else
					m_StatusText = TEXT("Ready");

				SetStatusText();
			}
			nOldID = nID;
		}
		else
		{
			if (nOldID != -1)
			{
				m_StatusText = TEXT("Ready");
				SetStatusText();
			}
			nOldID = -1;
		}
	}

	LRESULT CFrame::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
				if (wParam == ID_STATUS_TIMER)
					SetStatusIndicators();
				return 0L;
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

