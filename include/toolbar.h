// Win32++   Version 7.6.1 Beta
// 
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2014  David Nash
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


#ifndef _WIN32XX_TOOLBAR_H_
#define _WIN32XX_TOOLBAR_H_

#include "wincore.h"
#include "gdi.h"
#include "controls.h"


namespace Win32xx
{

	////////////////////////////////////
	// Declaration of the CToolBar class
	//
	class CToolBar : public CWnd
	{
	public:
		CToolBar();
		virtual ~CToolBar();

		// Operations
		virtual int  AddBitmap(UINT ToolBarID);
		virtual BOOL AddButton(UINT nID, BOOL bEnabled = TRUE, int iImage = -1);
		virtual void Destroy();
		virtual BOOL ReplaceBitmap(UINT NewToolBarID);
		virtual BOOL SetBitmap(UINT nID);
		virtual BOOL SetButtonText(int idButton, LPCTSTR szText);

		// Wrappers for Win32 API functions
		BOOL  AddButtons(UINT uNumButtons, LPTBBUTTON lpButtons) const;
		int   AddString(UINT nStringID) const;
		int   AddStrings(LPCTSTR lpszStrings) const;
		void  Autosize() const;
		void  CheckButton(int idButton, BOOL fCheck) const;
		int   CommandToIndex(int idButton) const;
		void  Customize() const;
		BOOL  DeleteButton(int iButton) const;
		BOOL  DisableButton(int idButton) const;
		BOOL  EnableButton(int idButton) const;
		BOOL  GetButton(int iButton, LPTBBUTTON lpButton) const;
		int   GetButtonCount() const;
		DWORD GetButtonSize() const;
		UINT  GetButtonState(int idButton) const;
		BYTE  GetButtonStyle(int idButton) const;
		CString GetButtonText(int idButton) const;
		int   GetCommandID(int iIndex) const;
		CImageList* GetDisabledImageList() const;
		int   GetHotItem() const;
		CImageList* GetHotImageList() const;
		CImageList* GetImageList() const;
		CRect GetItemRect(int iIndex) const;
		CSize GetMaxSize() const;
		DWORD GetPadding() const;
		CRect GetRect(int idButton) const;
		int   GetRows() const;
		int   GetTextRows() const;
		CToolTip*  GetToolTips() const;
		BOOL  HasText() const;
		BOOL  HideButton(int idButton, BOOL fShow) const;
		int   HitTest() const;
		BOOL  Indeterminate(int idButton, BOOL fIndeterminate) const;
		BOOL  InsertButton(int iButton, LPTBBUTTON lpButton) const;
		BOOL  IsButtonHidden(int idButton) const;
		BOOL  IsButtonHighlighted(int idButton) const;
		BOOL  IsButtonIndeterminate(int idButton) const;
		BOOL  IsButtonPressed(int idButton) const;
		int   MapAccelerator(TCHAR chAccel) const;
		BOOL  MarkButton(int idButton) const;
		BOOL  MoveButton(UINT uOldPos, UINT uNewPos) const;
		BOOL  PressButton(int idButton, BOOL fPress) const;
		void  SaveRestore(BOOL fSave, TBSAVEPARAMS* ptbsp) const;
		void  SetButtonInfo(int idButton, int idButtonNew, int iImage, BYTE Style = 0, BYTE State = 0) const;
		BOOL  SetBitmapSize(int cx, int cy) const;
		BOOL  SetButtonSize(int cx, int cy) const;
		BOOL  SetButtonState(int idButton, UINT State) const;
		BOOL  SetButtonStyle(int idButton, BYTE Style) const;
		BOOL  SetButtonWidth(int idButton, int nWidth) const;
		BOOL  SetCommandID(int iIndex, int idButton) const;
		CImageList* SetDisableImageList(CImageList* pNewDisabled) const;
		DWORD SetDrawTextFlags(DWORD dwMask, DWORD dwDTFlags) const;
		DWORD SetExtendedStyle(DWORD dwExStyle) const;
		CImageList* SetHotImageList(CImageList* pNewHot) const;
		int   SetHotItem(int iHot) const;
		CImageList* SetImageList(CImageList* pNew) const;
		BOOL  SetIndent(int iIndent) const;
		BOOL  SetMaxTextRows(int iMaxRows) const;
		BOOL  SetPadding(int cx, int cy) const;
		void  SetToolTips(CToolTip* pToolTip) const;

	protected:
	// Overridables
		virtual void OnAttach();
		virtual void OnDestroy();
		virtual LRESULT OnWindowPosChanging(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual void PreRegisterClass(WNDCLASS &wc);
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		CToolBar(const CToolBar&);				// Disable copy construction
		CToolBar& operator = (const CToolBar&); // Disable assignment operator

		std::map<CString, int> m_StringMap;	// a map of strings used in SetButtonText
		UINT m_OldToolBarID;				// Bitmap Resource ID, used in AddBitmap/ReplaceBitmap

	};  // class CToolBar

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{

	////////////////////////////////////
	// Definitions for the CToolBar class
	//
	inline CToolBar::CToolBar() : m_OldToolBarID(0)
	{
	}

	inline CToolBar::~CToolBar()
	{
	}

	inline int CToolBar::AddBitmap(UINT ToolBarID)
	// Adds one or more images to the list of button images available for a ToolBar.

	// Note: AddBitmap supports a maximum colour depth of 8 bits (256 colours)
	//       For more colours, use an ImageList instead
	{
		assert(::IsWindow(m_hWnd));

		CBitmap Bitmap(ToolBarID);
		assert (Bitmap.GetHandle());
		BITMAP bm = Bitmap.GetBitmapData();
		int iImageWidth  = MAX(bm.bmHeight, 16);
		int iImages = bm.bmWidth / iImageWidth;

		TBADDBITMAP tbab;
		ZeroMemory(&tbab, sizeof(TBADDBITMAP));
		tbab.hInst = GetApp()->GetResourceHandle();
		tbab.nID   = ToolBarID;
		int iResult = (int)SendMessage(TB_ADDBITMAP, iImages, (LPARAM)&tbab);

		if (-1 != iResult)
			m_OldToolBarID = ToolBarID;

		return iResult;
	}

	inline BOOL CToolBar::AddButton(UINT nID, BOOL bEnabled /* = TRUE */, int iImage /* = -1 */)
	// Adds buttons to the Toolbar. It provides a convenient alternative to AddButtons.
	// A resource ID of 0 is a separator.  iImage is the index of the image in the ImageList. 
	// The default is -1 in which case the image based on the button's position is chosen.
	{
		assert(::IsWindow(m_hWnd));

		// Count toolbar buttons with Command IDs
		int nImages = 0;

		if (iImage == -1)
		{
			// choose the image based on the number of buttons already used
			for (int i = 0; i < GetButtonCount(); ++i)
			{
				if (GetCommandID(i) > 0)
					nImages++;
			}
		}
		else
		{
			nImages = iImage;
		}

		// TBBUTTON structure for each button in the toolbar
		TBBUTTON tbb;
		ZeroMemory(&tbb, sizeof(TBBUTTON));

		if (0 == nID)
		{
			tbb.fsStyle = TBSTYLE_SEP;
		}
		else
		{
			tbb.iBitmap = nImages;
			tbb.idCommand = nID;
			tbb.fsState = bEnabled? TBSTATE_ENABLED : 0;
			tbb.fsStyle = TBSTYLE_BUTTON;
		}

		// Add the button to the toolbar
		return (BOOL)SendMessage(TB_ADDBUTTONS, 1L, (LPARAM)&tbb);
	}

	inline BOOL CToolBar::AddButtons(UINT uNumButtons, LPTBBUTTON lpButtons) const
	// Adds one or more buttons to a ToolBar.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_ADDBUTTONS, (LPARAM)uNumButtons, (WPARAM)lpButtons);
	}

	inline int CToolBar::AddString(UINT nStringID) const
	// Adds a new string, passed as a resource ID, to the ToolBar's internal list of strings.
	{
		assert(::IsWindow(m_hWnd));
		return (int)SendMessage(TB_ADDSTRING, (LPARAM)GetApp()->GetResourceHandle(), (WPARAM)nStringID);
	}

	inline int CToolBar::AddStrings(LPCTSTR lpszStrings) const
	// Adds a new string or strings to the list of strings available for a ToolBar control.
	// Strings in the buffer must be separated by a null character. You must ensure that the last string has two null terminators.
	{
		assert(::IsWindow(m_hWnd));
		return (int)SendMessage(TB_ADDSTRING, 0L, (WPARAM)lpszStrings);
	}

	inline void CToolBar::Autosize() const
	// Causes a ToolBar to be resized.
	{
		assert(::IsWindow(m_hWnd));
		SendMessage(TB_AUTOSIZE, 0L, 0L);
	}

	inline void CToolBar::CheckButton(int idButton, BOOL fCheck) const
	// Checks or unchecks a given button in a ToolBar.
	// When a button is checked, it is displayed in the pressed state.
	{
		assert(::IsWindow(m_hWnd));
		SendMessage(TB_CHECKBUTTON, (WPARAM)idButton, (LPARAM)MAKELONG(fCheck, 0));
	}

	inline int CToolBar::CommandToIndex(int idButton) const
	// Retrieves the zero-based index for the button associated with the specified command identifier
	{
		assert(::IsWindow(m_hWnd));

		// returns -1 on fail
		return (int)SendMessage(TB_COMMANDTOINDEX, (WPARAM)idButton, 0L);
	}

	inline void CToolBar::Customize() const
	// Displays the Customize Toolbar dialog box. The parent must handle the TBN_QUERYINSERT 
	// and TBN_QUERYDELETE notifications for the Customize Toolbar dialog box to appear.
	// Requires the CCS_ADJUSTABLE style to be set when the toolbar is created.
	{
		assert(::IsWindow(m_hWnd));
		SendMessage(TB_CUSTOMIZE, 0, 0);
	}

	inline BOOL CToolBar::DeleteButton(int iButton) const
	// Deletes a button from the ToolBar.
	// iButton is the Zero-based index of the button to delete.
	{
		assert(::IsWindow(m_hWnd));
		return (int)SendMessage(TB_DELETEBUTTON, (WPARAM)iButton, 0L);
	}

	inline void CToolBar::Destroy()
	// Allows CToolBar to be reused after the window is destroyed
	{
		CWnd::Destroy();
		m_StringMap.clear();
	}

	inline BOOL CToolBar::DisableButton(int idButton) const
	// Disables the specified button in a ToolBar
	// An example of idButton would be IDM_FILE_OPEN
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_ENABLEBUTTON, (WPARAM)idButton, (LPARAM) MAKELONG(FALSE, 0));
	}

	inline BOOL CToolBar::EnableButton(int idButton) const
	// Enables the specified button in a ToolBar
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_ENABLEBUTTON, (WPARAM)idButton, (LPARAM) MAKELONG(TRUE,0 ));
	}

	inline BOOL CToolBar::GetButton(int iButton, LPTBBUTTON lpButton) const
	// Receives the TBBUTTON structure information from the specified button
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_GETBUTTON, (LPARAM)iButton, (WPARAM)lpButton);
	}

	inline int CToolBar::GetButtonCount() const
	// Retrieves a count of the buttons currently in the ToolBar
	{
		assert(::IsWindow(m_hWnd));
		return (int)SendMessage(TB_BUTTONCOUNT, 0L, 0L);
	}

	inline DWORD CToolBar::GetButtonSize() const
	// Retrieves the current width and height of ToolBar buttons, in pixels.
	// Returns a DWORD value that contains the width and height values in the low word and high word, respectively.
	{
		assert(::IsWindow(m_hWnd));
		return (DWORD)SendMessage(TB_GETBUTTONSIZE, 0L, 0L);
	}

	inline UINT CToolBar::GetButtonState(int idButton) const
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
		return (UINT)SendMessage(TB_GETSTATE, (WPARAM) idButton, 0L);
	}

	inline BYTE CToolBar::GetButtonStyle(int idButton) const
	//	Get the the style of the ToolBar control. The following button styles are supported:
	//	TBSTYLE_BUTTON		Standard push button (default)
	//	TBSTYLE_SEP			Separator
	//	TBSTYLE_CHECK		Auto check-box button
	//	TBSTYLE_GROUP		Marks the start of a group of buttons
	//	TBSTYLE_CHECKGROUP	Marks the start of a group of check-box buttons
	//	TBSTYLE_DROPDOWN	Creates a drop-down list button
	//	TBSTYLE_AUTOSIZE	The button's width will be calculated based on the text of the button, not on the size of the image
	//	TBSTYLE_NOPREFIX	The button text will not have an accelerator prefix associated with it
	{
		assert(::IsWindow(m_hWnd));

		int iIndex = CommandToIndex(idButton);
		TBBUTTON tbb;
		ZeroMemory(&tbb, sizeof(TBBUTTON));
		SendMessage(TB_GETBUTTON, iIndex, (LPARAM) &tbb);

		return tbb.fsStyle;
	}

	inline CString CToolBar::GetButtonText(int idButton) const
	// Retrieves the display text of a button on a ToolBar.
	{
		assert(::IsWindow(m_hWnd));

		int Length = (int)SendMessage(TB_GETBUTTONTEXT, idButton, 0);
		CString str;
		LPTSTR szStr = str.GetBuffer(Length +1);
		SendMessage(TB_GETBUTTONTEXT, (LPARAM)idButton, (WPARAM)szStr);
		str.ReleaseBuffer();
		return str;
	}

	inline int CToolBar::GetCommandID(int iIndex) const
	// Retrieves information about the specified button in a ToolBar
	{
		assert(::IsWindow(m_hWnd));
		TBBUTTON tbb;
		ZeroMemory(&tbb, sizeof(TBBUTTON));
		SendMessage(TB_GETBUTTON, iIndex, (WPARAM) &tbb);

		// returns zero if failed
		return tbb.idCommand;
	}

	inline CImageList* CToolBar::GetDisabledImageList() const
	// Retrieves the image list that a ToolBar control uses to display inactive buttons.
	{
		assert(::IsWindow(m_hWnd));
		return CImageList::FromHandle( (HIMAGELIST)SendMessage(TB_GETDISABLEDIMAGELIST, 0L, 0L) );
	}

	inline CImageList* CToolBar::GetHotImageList() const
	// Retrieves the image list that a ToolBar control uses to display hot buttons.
	{
		assert(::IsWindow(m_hWnd));
		return CImageList::FromHandle( (HIMAGELIST)SendMessage(TB_GETHOTIMAGELIST, 0L, 0L) );
	}

	inline int CToolBar::GetHotItem() const
	// Retrieves the index of the hot item in a ToolBar, or -1 if no hot item is set.
	{
		assert(::IsWindow(m_hWnd));
		return (int)SendMessage(TB_GETHOTITEM, 0L, 0L);
	}

	inline CImageList* CToolBar::GetImageList() const
	// Retrieves the image list that a ToolBar control uses to display buttons in their default state.
	{
		assert(::IsWindow(m_hWnd));
		return CImageList::FromHandle( (HIMAGELIST)SendMessage(TB_GETIMAGELIST, 0L, 0L) );
	}

	inline CRect CToolBar::GetItemRect(int iIndex) const
	// Retrieves the bounding rectangle of a button in a ToolBar
	{
		assert(::IsWindow(m_hWnd));
		CRect rc;
		int iCount = (int)SendMessage(TB_BUTTONCOUNT, 0L, 0L);

		if (iCount >= iIndex)
			SendMessage(TB_GETITEMRECT, (WPARAM)iIndex, (LPARAM)&rc);

		return rc;
	}

	inline CSize CToolBar::GetMaxSize() const
	// Retrieves the total size of all of the visible buttons and separators in the ToolBar
	{
		assert(::IsWindow(m_hWnd));
		CSize sz;
		SendMessage(TB_GETMAXSIZE, 0L, (LPARAM)&sz);

		// This fixes a Windows bug calculating the size when TBSTYLE_DROPDOWN is used.
		int cxMaxSize = 0;
		int cyMaxSize = 0;
		for (int i= 0 ; i < GetButtonCount(); ++i)
		{
			CRect rcItem = GetItemRect(i);
			cxMaxSize += rcItem.Width();
			cyMaxSize += rcItem.Height();
		}

		if (GetWindowLongPtr(GWL_STYLE) & CCS_VERT)
			sz.cy = cyMaxSize;
		else
			sz.cx = cxMaxSize;

		return sz;
	}

	inline DWORD CToolBar::GetPadding() const
	// Returns a DWORD value that contains the horizontal padding in the low word and the vertical padding in the high word, in pixels.
	{
		assert(::IsWindow(m_hWnd));
		return (DWORD)SendMessage(TB_GETPADDING, 0L, 0L);
	}

	inline CRect CToolBar::GetRect(int idButton) const
	// Retrieves the bounding rectangle for a specified ToolBar button.
	{
		assert(::IsWindow(m_hWnd));
		CRect rc;
		SendMessage(TB_GETRECT, (WPARAM)idButton, (LPARAM)&rc);
		return rc;
	}

	inline int CToolBar::GetRows() const
	// Retrieves the number of rows of buttons in a ToolBar with the TBSTYLE_WRAPABLE style.
	{
		assert(::IsWindow(m_hWnd));
		return (int)SendMessage(TB_GETROWS, 0L, 0L);
	}

	inline int CToolBar::GetTextRows() const
	// Retrieves the maximum number of text rows that can be displayed on a ToolBar button.
	{
		assert(::IsWindow(m_hWnd));
		return (int)SendMessage(TB_GETTEXTROWS, 0L, 0L);
	}

	inline CToolTip* CToolBar::GetToolTips() const
	// Retrieves the handle to the ToolTip control, if any, associated with the ToolBar.
	{
		assert(::IsWindow(m_hWnd));
		return static_cast<CToolTip*>(FromHandlePermanent((HWND)SendMessage(TB_GETTOOLTIPS, 0L, 0L)));
	}

	inline BOOL CToolBar::HasText() const
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

	inline BOOL CToolBar::HideButton(int idButton, BOOL fShow) const
	//Hides or shows the specified button in a ToolBar.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_HIDEBUTTON, (WPARAM)idButton, (LPARAM)MAKELONG (fShow, 0));
	}

	inline int CToolBar::HitTest() const
	// Determines where a point lies in a ToolBar control.

	// We do our own hit test since TB_HITTEST is a bit buggy,
	// and also doesn't work at all on earliest versions of Win95
	{
		assert(::IsWindow(m_hWnd));
		CPoint pt = GetCursorPos();
		ScreenToClient(pt);

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

	inline BOOL CToolBar::Indeterminate(int idButton, BOOL fIndeterminate) const
	// Sets or clears the indeterminate state of the specified button in a toolbar.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_INDETERMINATE, (WPARAM)idButton, (LPARAM)MAKELONG (fIndeterminate, 0));
	}

	inline BOOL CToolBar::InsertButton(int iButton, LPTBBUTTON lpButton) const
	// Inserts a button to the left of iButton.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_INSERTBUTTON, (WPARAM)iButton, (LPARAM)lpButton);
	}

	inline BOOL CToolBar::IsButtonHidden(int idButton) const
	// Determines whether the specified button in a ToolBar is hidden.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_ISBUTTONHIDDEN, (WPARAM)idButton, 0L);
	}

	inline BOOL CToolBar::IsButtonHighlighted(int idButton) const
	// Checks the highlight state of a ToolBar button.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_ISBUTTONHIGHLIGHTED, (WPARAM)idButton, 0L);
	}

	inline BOOL CToolBar::IsButtonIndeterminate(int idButton) const
	// Determines whether the specified button in a ToolBar is indeterminate.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_ISBUTTONINDETERMINATE, (WPARAM)idButton, 0L);
	}

	inline BOOL CToolBar::IsButtonPressed(int idButton) const
	// Determines whether the specified button in a ToolBar is pressed.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_ISBUTTONPRESSED, (WPARAM)idButton, 0L);
	}

	inline int CToolBar::MapAccelerator(TCHAR chAccel) const
	// Determines whether the specified button in a ToolBar is pressed.
	{
		assert(::IsWindow(m_hWnd));
		int uButtonID;
		int idButton;
		if (SendMessage(TB_MAPACCELERATOR, (WPARAM)chAccel, (LPARAM)&uButtonID))
			idButton = uButtonID;
		else
			idButton = -1;

		return idButton;
	}

	inline BOOL CToolBar::MarkButton(int idButton) const
	// Sets the highlight state of a given button in a ToolBar control.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_MARKBUTTON, (WPARAM)idButton, 0L);
	}

	inline BOOL CToolBar::MoveButton(UINT uOldPos, UINT uNewPos) const
	// Moves a button from one index to another.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_MOVEBUTTON, (WPARAM)uOldPos, (LPARAM)uNewPos);
	}


	inline void CToolBar::OnAttach()
	{
		// We must send this message before sending the TB_ADDBITMAP or TB_ADDBUTTONS message
		SendMessage(TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0L);

		// allows buttons to have a separate drop-down arrow
		// Note: TBN_DROPDOWN notification is sent by a ToolBar control when the user clicks a drop-down button
		SendMessage(TB_SETEXTENDEDSTYLE, 0L, TBSTYLE_EX_DRAWDDARROWS);
	}

	inline void CToolBar::OnDestroy()
	{
	}

	inline LRESULT CToolBar::OnWindowPosChanging(WPARAM wParam, LPARAM lParam)
	{
	//	Used by ReBar controls to adjust ToolBar window size
	
		if ( GetParent()->SendMessage(UWM_TBWINPOSCHANGING, (WPARAM)m_hWnd, lParam) )
		{
			LPWINDOWPOS pWinPos = (LPWINDOWPOS)lParam;
			pWinPos->cx = GetMaxSize().cx;
			pWinPos->cy = GetMaxSize().cy;
		}

		return FinalWindowProc(WM_WINDOWPOSCHANGING, wParam, lParam);
	}

	inline void CToolBar::PreCreate(CREATESTRUCT &cs)
	{
		// Sets the CREATESTRUCT parameters prior to window creation
		cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | TBSTYLE_TOOLTIPS | TBSTYLE_FLAT;
	}

	inline void CToolBar::PreRegisterClass(WNDCLASS &wc)
	{
		// Set the Window Class
		wc.lpszClassName =  TOOLBARCLASSNAME;
	}

	inline BOOL CToolBar::PressButton(int idButton, BOOL fPress) const
	// Presses or releases the specified button in a ToolBar.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_PRESSBUTTON, (WPARAM)idButton, (LPARAM)MAKELONG(fPress, 0));
	}

	inline BOOL CToolBar::ReplaceBitmap(UINT NewToolBarID)
	// Replaces an existing bitmap with a new bitmap.

	// Note: ReplaceBitmap supports a maximum colour depth of 8 bits (256 colours)
	//       For more colours, use an ImageList instead
	{
		assert(::IsWindow(m_hWnd));

		CBitmap Bitmap(NewToolBarID);
		assert (Bitmap.GetHandle());
		BITMAP bm = Bitmap.GetBitmapData();
		int iImageWidth  = MAX(bm.bmHeight, 16);
		int iImages = bm.bmWidth / iImageWidth;

		TBREPLACEBITMAP tbrb;
		ZeroMemory(&tbrb, sizeof(TBREPLACEBITMAP));
		tbrb.hInstNew = GetApp()->GetResourceHandle();
		tbrb.hInstOld = GetApp()->GetResourceHandle();
		tbrb.nIDNew = NewToolBarID;
		tbrb.nIDOld = m_OldToolBarID;
		tbrb.nButtons  = iImages;

		BOOL bResult = (BOOL)SendMessage(TB_REPLACEBITMAP, iImages, (LPARAM)&tbrb);
		if (bResult)
			m_OldToolBarID = NewToolBarID;

		return bResult;
	}

	inline void CToolBar::SaveRestore(BOOL fSave, TBSAVEPARAMS* ptbsp) const
	// Presses or releases the specified button in a ToolBar.
	{
		assert(::IsWindow(m_hWnd));
		SendMessage(TB_PRESSBUTTON, (WPARAM)fSave, (LPARAM)ptbsp);
	}

	inline BOOL CToolBar::SetBitmap(UINT nID)
	// Set the button images
	{
		assert(::IsWindow(m_hWnd));

		CBitmap Bitmap(nID);
		assert (Bitmap.GetHandle());
		BITMAP bm = Bitmap.GetBitmapData();

		int iImageHeight = bm.bmHeight;
		int iImageWidth  = MAX(bm.bmHeight, 16);

		// Set the bitmap size first
		SetBitmapSize(iImageWidth, iImageHeight);

		BOOL bResult = FALSE;
		if (m_OldToolBarID)
			bResult = ReplaceBitmap(nID);
		else
			bResult = (BOOL)AddBitmap(nID);

		return bResult;
	}

	inline BOOL CToolBar::SetBitmapSize(int cx, int cy) const
	// Sets the size of the bitmapped images to be added to a ToolBar.

	// Needs to be used when the image size is not the default 16 x 15
	// Call this function before using AddBitmap or ReplaceBitmap
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_SETBITMAPSIZE, 0L, MAKELONG(cx, cy));
	}

	inline BOOL CToolBar::SetButtonSize(int cx, int cy) const
	// Sets the size of the buttons to be added to a ToolBar
	// The size can be set only before adding any buttons to the ToolBar
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_SETBUTTONSIZE, 0L, MAKELONG(cx, cy));
	}

	inline void CToolBar::SetButtonInfo(int idButton, int idButtonNew, int iImage, BYTE Style /* = 0 */, BYTE State /* = 0 */) const
	// Use this to change the buttons image and ID.
	// The ID must be changed for the image to be changed.
	{
		// Retrieve existing state and style
		TBBUTTON tb;
		ZeroMemory(&tb, sizeof(TBBUTTON));
		BOOL bSucceeded = GetButton(CommandToIndex(idButton), &tb);
		assert(bSucceeded);

        if (bSucceeded)
        {
            TBBUTTONINFO tbbi;
			ZeroMemory(&tbbi, sizeof(TBBUTTONINFO));
            tbbi.cbSize = sizeof(TBBUTTONINFO);
            tbbi.dwMask = TBIF_COMMAND | TBIF_IMAGE | TBIF_STYLE | TBIF_STATE;
            tbbi.idCommand = idButtonNew;
            tbbi.iImage = iImage;
            tbbi.fsStyle = Style ? Style : tb.fsStyle;
            tbbi.fsState = State ? State : tb.fsState;

            SendMessage(TB_SETBUTTONINFO, idButton, (LPARAM) &tbbi);
        }
	}

	inline BOOL CToolBar::SetButtonState(int idButton, UINT State) const
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
		return (BOOL)SendMessage(TB_SETSTATE, (WPARAM) idButton, (LPARAM)MAKELONG (State, 0));
 	}

	inline BOOL CToolBar::SetButtonStyle(int idButton, BYTE Style) const
	//	The the style of the ToolBar control. The following button styles are supported:
	//	TBSTYLE_BUTTON		Standard push button (default)
	//	TBSTYLE_SEP			Separator
	//	TBSTYLE_CHECK		Auto check-box button
	//	TBSTYLE_GROUP		Marks the start of a group of buttons
	//	TBSTYLE_CHECKGROUP	Marks the start of a group of check-box buttons
	//	TBSTYLE_DROPDOWN	Creates a drop-down list button
	//	TBSTYLE_AUTOSIZE	The button's width will be calculated based on the text of the button, not on the size of the image
	//	TBSTYLE_NOPREFIX	The button text will not have an accelerator prefix associated with it
	{
		assert(::IsWindow(m_hWnd));

		TBBUTTONINFO tbbi;
		ZeroMemory(&tbbi, sizeof(TBBUTTONINFO));
		tbbi.cbSize = sizeof(TBBUTTONINFO);
		tbbi.dwMask = TBIF_STYLE;
		tbbi.fsStyle = Style;

		// Note:  TB_SETBUTTONINFO requires comctl32.dll version 4.71 or later
		//        i.e. Win95 with IE4 / NT with IE4   or later
		return (BOOL)SendMessage(TB_SETBUTTONINFO, idButton, (LPARAM) &tbbi);
	}

	inline BOOL CToolBar::SetButtonText(int idButton, LPCTSTR szText)
	// This rather convoluted approach to setting ToolBar button text supports
	// all versions of Windows, including Win95 with COMCTL32.DLL version 4.0
	{
		assert(::IsWindow(m_hWnd));
		int iIndex = CommandToIndex(idButton);
		assert(-1 != iIndex);

		BOOL Succeeded = TRUE;
		CString sString = szText;
		std::map<CString, int>::iterator m;
		int iString;

		// Check to see if the string is already added
		m = m_StringMap.find(sString);
		if (m_StringMap.end() == m)
		{
			CString str;
			if (0 == m_StringMap.size())
			{
				// Place a blank string first in the string table, in case some
				// buttons don't have text

				str = _T(" ");
				str += _T('\0');	// Double-null terminate
				AddStrings(str);
			}

			// No index for this string exists, so create it now
			str = szText;
			str += _T('\0');		// Double-null terminate

			iString = AddStrings(str);
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
			TBBUTTON tbb;
			ZeroMemory(&tbb, sizeof(TBBUTTON));
			Succeeded = (BOOL)SendMessage(TB_GETBUTTON, iIndex, (LPARAM)&tbb);

			tbb.iString = iString;

			// Turn off ToolBar drawing
			SetRedraw(FALSE);

			if (Succeeded)
				Succeeded = (BOOL)SendMessage(TB_DELETEBUTTON, iIndex, 0L);

			if (Succeeded)
				Succeeded = (BOOL)SendMessage(TB_INSERTBUTTON, iIndex, (LPARAM)&tbb);

			// Ensure the button now includes some text rows
			if (0 == SendMessage(TB_GETTEXTROWS, 0L, 0L))
				SendMessage(TB_SETMAXTEXTROWS, 1L, 0L);

			// Turn on ToolBar drawing
			SetRedraw(TRUE);
		}
		// Redraw button
		CRect r = GetItemRect(iIndex);
		InvalidateRect(&r, TRUE);

		return Succeeded;
	}

	inline BOOL CToolBar::SetButtonWidth(int idButton, int nWidth) const
	// The set button width can adjust the width of the button after it is created.
	// This is useful when replacing a button with a ComboBox or other control.
	// Note:  TB_SETBUTTONINFO requires comctl32.dll version 4.71 or later
	//        i.e. Win95 with IE4 / NT with IE4   or later
	{
		assert(::IsWindow(m_hWnd));

		TBBUTTONINFO tbbi;
		ZeroMemory(&tbbi, sizeof(TBBUTTONINFO));
		tbbi.cbSize = sizeof(TBBUTTONINFO);
		tbbi.dwMask = TBIF_SIZE;
		tbbi.cx = (WORD)nWidth;
		BOOL bResult = (BOOL)SendMessage(TB_SETBUTTONINFO, (WPARAM)idButton, (LPARAM)&tbbi);

		// Send a changed message to the parent (used by the ReBar)
		SIZE MaxSize = GetMaxSize();
		GetParent()->SendMessage(UWM_TOOLBARRESIZE, (WPARAM)m_hWnd, (LPARAM)&MaxSize);

		return bResult;
	}

	inline BOOL CToolBar::SetCommandID(int iIndex, int idButton) const
	// Sets the command identifier of a ToolBar button
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_SETCMDID, iIndex, idButton);
	}

	inline CImageList* CToolBar::SetDisableImageList(CImageList* pNewDisabled) const
	// Sets the ImageList that the ToolBar control will use to display disabled buttons.
	{
		assert(::IsWindow(m_hWnd));
		HIMAGELIST himlNewDisabled = pNewDisabled ? pNewDisabled->GetHandle() : 0;
		return CImageList::FromHandle( (HIMAGELIST)SendMessage(TB_SETDISABLEDIMAGELIST, 0L, (LPARAM)himlNewDisabled) );
	}

	inline DWORD CToolBar::SetDrawTextFlags(DWORD dwMask, DWORD dwDTFlags) const
	// Sets the text drawing flags for the ToolBar.
	{
		assert(::IsWindow(m_hWnd));
		return (DWORD)SendMessage(TB_SETDRAWTEXTFLAGS, (WPARAM)dwMask, (LPARAM)dwDTFlags);
	}

	inline DWORD CToolBar::SetExtendedStyle(DWORD dwExStyle) const
	// Sets the text drawing flags for the ToolBar.
	// Extended styles include: TBSTYLE_EX_DRAWDDARROWS, TBSTYLE_EX_HIDECLIPPEDBUTTONS, TBSTYLE_EX_DOUBLEBUFFER and TBSTYLE_EX_MIXEDBUTTONS
	{
		assert(::IsWindow(m_hWnd));
		return (DWORD)SendMessage(TB_SETEXTENDEDSTYLE, 0L, (LPARAM)dwExStyle);
	}

	inline CImageList* CToolBar::SetHotImageList(CImageList* pNewHot) const
	// Sets the image list that the ToolBar control will use to display hot buttons.
	{
		assert(::IsWindow(m_hWnd));
		HIMAGELIST himlNewHot = pNewHot ? pNewHot->GetHandle() : 0;
		return CImageList::FromHandle( (HIMAGELIST)SendMessage(TB_SETHOTIMAGELIST, 0L, (LPARAM)himlNewHot) );
	}

	inline int CToolBar::SetHotItem(int iHot) const
	// Sets the hot item in a ToolBar.
	{
		assert(::IsWindow(m_hWnd));
		return (int)SendMessage(TB_SETHOTITEM, (WPARAM)iHot, 0L);
	}

	inline CImageList* CToolBar::SetImageList(CImageList* pNew) const
	// Sets the image list that the ToolBar will use to display buttons that are in their default state.
	{
		assert(::IsWindow(m_hWnd));
		HIMAGELIST himlNew = pNew ? pNew->GetHandle() : 0;
		return CImageList::FromHandle( (HIMAGELIST)SendMessage(TB_SETIMAGELIST, 0L, (LPARAM)himlNew) );
	}

	inline BOOL CToolBar::SetIndent(int iIndent) const
	// Sets the indentation for the first button in a ToolBar control.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_SETINDENT, (WPARAM)iIndent, 0L);
	}

	inline BOOL CToolBar::SetMaxTextRows(int iMaxRows) const
	// Sets the maximum number of text rows displayed on a ToolBar button.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_SETMAXTEXTROWS, (WPARAM)iMaxRows, 0L);
	}

	inline BOOL CToolBar::SetPadding(int cx, int cy) const
	// Sets the padding for a ToolBar control.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_SETPADDING, 0L, (WPARAM)MAKELONG(cx, cy));
	}

	inline void CToolBar::SetToolTips(CToolTip* pToolTip) const
	// Associates a ToolTip control with a ToolBar. This should be done before
	//  adding any buttons to the ToolBar.
	{
		assert(::IsWindow(m_hWnd));
		HWND hToolTip = pToolTip? pToolTip->GetHwnd() : (HWND)0;
		SendMessage(TB_SETTOOLTIPS, (WPARAM)hToolTip, 0L);
	}

	inline LRESULT CToolBar::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_WINDOWPOSCHANGING:	return OnWindowPosChanging(wParam, lParam);
		}

		// pass unhandled messages on for default processing
		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	}

} // namespace Win32xx

#endif // #ifndef _WIN32XX_TOOLBAR_H_
 