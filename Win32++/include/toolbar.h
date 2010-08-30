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

		// Operations
		virtual int  AddBitmap(UINT ToolbarID);
		virtual BOOL AddButton(UINT nID, BOOL bEnabled = TRUE);
		virtual void Destroy();
		virtual BOOL ReplaceBitmap(UINT NewToolbarID);
		virtual BOOL SetBitmap(UINT nID);
		virtual BOOL SetImages(COLORREF crMask, UINT ToolbarID, UINT ToolbarHotID, UINT ToolbarDisabledID);
		virtual int  SetButtons(const std::vector<UINT>& vToolbarData) const;
		virtual BOOL SetButtonText(int idButton, LPCTSTR szText);

		// Wrappers for Win32 API functions
		BOOL  AddButtons(UINT uNumButtons, LPTBBUTTON lpButtons) const;
		int   AddString(UINT nStringID) const;
		int   AddStrings(LPCTSTR lpszStrings) const;
		void  Autosize() const;
		void  CheckButton(int idButton, BOOL fCheck) const;
		int   CommandToIndex(int idButton) const;
		BOOL  DeleteButton(int iButton) const;
		BOOL  DisableButton(int idButton) const;
		BOOL  EnableButton(int idButton) const;
		BOOL  GetButton(int iButton, LPTBBUTTON lpButton) const;
		int   GetButtonCount() const;
		DWORD GetButtonSize() const;
		UINT  GetButtonState(int idButton) const;
		BYTE  GetButtonStyle(int idButton) const;
		LPCTSTR GetButtonText(int idButton) const;
		int   GetCommandID(int iIndex) const;
		HIMAGELIST GetDisabledImageList() const;
		int   GetHotItem() const;
		HIMAGELIST GetImageList() const;
		CRect GetItemRect(int iIndex) const;
		CSize GetMaxSize() const;
		DWORD GetPadding() const;
		CRect GetRect(int idButton) const;
		int   GetRows() const;
		int   GetTextRows() const;
		HWND  GetToolTips() const;
		BOOL  HasText() const;
		BOOL  HideButton(int idButton, BOOL fShow) const;
		int   HitTest() const;
		BOOL  Indeterminate(int idButton, BOOL fIndeterminate) const;
		BOOL  InsetButton(int iButton, LPTBBUTTON lpButton) const;
		BOOL  IsButtonHidden(int idButton) const;
		BOOL  IsButtonHighlighted(int idButton) const;
		BOOL  IsButtonIndeterminate(int idButton) const;
		BOOL  IsButtonPressed(int idButton) const;
		int   MapAccelerator(TCHAR chAccel) const;
		BOOL  MarkButton(int idButton) const;
		BOOL  MoveButton(UINT uOldPos, UINT uNewPos) const;
		BOOL  PressButton(int idButton, BOOL fPress) const;
		void  SaveRestore(BOOL fSave, TBSAVEPARAMS* ptbsp) const;
		BOOL  SetBitmapSize(int cx, int cy) const;
		BOOL  SetButtonSize(int cx, int cy) const;
		BOOL  SetButtonState(int idButton, UINT State) const;
		BOOL  SetButtonStyle(int idButton, BYTE Style) const;
		BOOL  SetButtonWidth(int idButton, int nWidth) const;
		BOOL  SetCommandID(int iIndex, int idButton) const;
		HIMAGELIST SetDisableImageList(HIMAGELIST himlNewDisabled) const;
		DWORD SetDrawTextFlags(DWORD dwMask, DWORD dwDTFlags) const;
		DWORD SetExtendedStyle(DWORD dwExStyle) const;
		HIMAGELIST SetHotImageList(HIMAGELIST himlNewHot) const;
		HIMAGELIST SetImageList(HIMAGELIST himlNew) const;
		BOOL SetIndent(int iIndent) const;
		BOOL SetMaxTextRows(int iMaxRows) const;
		BOOL SetPadding(int cx, int cy) const;
		void SetToolTips(HWND hwndToolTip) const;

		// Attributes
		std::vector<UINT>& GetToolbarData() const {return (std::vector <UINT> &)m_vToolbarData;}
		ToolbarTheme& GetToolbarTheme() {return m_Theme;}
		void SetToolbarTheme(ToolbarTheme& Theme);

	protected:
	// Overridables
		virtual void OnCreate();
		virtual void OnDestroy();
		virtual void OnWindowPosChanging(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnCustomDraw(NMHDR* pNMHDR);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual void PreRegisterClass(WNDCLASS &wc);
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		CToolbar(const CToolbar&);				// Disable copy construction
		CToolbar& operator = (const CToolbar&); // Disable assignment operator

		std::vector<UINT> m_vToolbarData;	// vector of resource IDs for toolbar buttons
		std::map<tString, int> m_StringMap;	// a map of strings used in SetButtonText
		UINT m_OldToolbarID;				// Bitmap Resource ID, used in AddBitmap/ReplaceBitmap
		ToolbarTheme m_Theme;				// The theme structure
		BOOL m_bDrawArrowBkgrnd;			// True if a seperate arrow background is to be drawn
		mutable std::vector<TCHAR> m_vTChar;// Used in string functions

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

	inline BOOL CToolbar::AddButton(UINT nID, BOOL bEnabled /* = TRUE */)
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

	inline BOOL CToolbar::AddButtons(UINT uNumButtons, LPTBBUTTON lpButtons) const
	// Adds one or more buttons to a toolbar.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_ADDBUTTONS, (LPARAM)uNumButtons, (WPARAM)lpButtons);
	}

	inline int CToolbar::AddString(UINT nStringID) const
	// Adds a new string, passed as a resource ID, to the toolbar's internal list of strings.
	{
		assert(::IsWindow(m_hWnd));
		return (int)SendMessage(TB_ADDSTRING, (LPARAM)GetApp()->GetResourceHandle(), (WPARAM)nStringID);
	}

	inline int CToolbar::AddStrings(LPCTSTR lpszStrings) const
	// Adds a new string or strings to the list of strings available for a toolbar control.
	// Strings in the buffer must be separated by a null character. You must ensure that the last string has two null terminators.
	{
		assert(::IsWindow(m_hWnd));
		return (int)SendMessage(TB_ADDSTRING, 0L, (WPARAM)lpszStrings);
	}

	inline void CToolbar::Autosize() const
	// Causes a toolbar to be resized.
	{
		assert(::IsWindow(m_hWnd));
		SendMessage(TB_AUTOSIZE, 0L, 0L);
	}

	inline void CToolbar::CheckButton(int idButton, BOOL fCheck) const
	// Checks or unchecks a given button in a toolbar.
	// When a button is checked, it is displayed in the pressed state.
	{
		assert(::IsWindow(m_hWnd));
		SendMessage(TB_CHECKBUTTON, (WPARAM)idButton, (LPARAM)MAKELONG(fCheck, 0));
	}

	inline int CToolbar::CommandToIndex(int idButton) const
	// Retrieves the zero-based index for the button associated with the specified command identifier
	{
		assert(::IsWindow(m_hWnd));

		// returns -1 on fail
		return (int)SendMessage(TB_COMMANDTOINDEX, (WPARAM)idButton, 0L);
	}

	inline BOOL CToolbar::DeleteButton(int iButton) const
	// Deletes a button from the toolbar.
	// iButton is the Zero-based index of the button to delete.
	{
		assert(::IsWindow(m_hWnd));
		return (int)SendMessage(TB_DELETEBUTTON, (WPARAM)iButton, 0L);
	}

	inline void CToolbar::Destroy()
	// Allows CToolbar to be reused after the window is destroyed
	{
		CWnd::Destroy();
		m_StringMap.clear();
	}

	inline BOOL CToolbar::DisableButton(int idButton) const
	// Disables the specified button in a toolbar
	// An example of idButton would be IDM_FILE_OPEN
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_ENABLEBUTTON, (WPARAM)idButton, (LPARAM) MAKELONG(FALSE, 0));
	}

	inline BOOL CToolbar::EnableButton(int idButton) const
	// Enables the specified button in a toolbar
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_ENABLEBUTTON, (WPARAM)idButton, (LPARAM) MAKELONG(TRUE,0 ));
	}

	inline BOOL CToolbar::GetButton(int iButton, LPTBBUTTON lpButton) const
	// Recieves the TBBUTTON structure information from the specified button
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_GETBUTTON, (LPARAM)iButton, (WPARAM)lpButton);
	}

	inline int CToolbar::GetButtonCount() const
	// Retrieves a count of the buttons currently in the toolbar
	{
		assert(::IsWindow(m_hWnd));
		return (int)SendMessage(TB_BUTTONCOUNT, 0L, 0L);
	}

	inline DWORD CToolbar::GetButtonSize() const
	// Retrieves the current width and height of toolbar buttons, in pixels.
	// Returns a DWORD value that contains the width and height values in the low word and high word, respectively.
	{
		assert(::IsWindow(m_hWnd));
		return (DWORD)SendMessage(TB_GETBUTTONSIZE, 0L, 0L);
	}

	inline UINT CToolbar::GetButtonState(int idButton) const
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

	inline BYTE CToolbar::GetButtonStyle(int idButton) const
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

		int iIndex = CommandToIndex(idButton);
		TBBUTTON tbb = {0};
		SendMessage(TB_GETBUTTON, iIndex, (LPARAM) &tbb);

		return tbb.fsStyle;
	}

	inline LPCTSTR CToolbar::GetButtonText(int idButton) const
	// Retrieves the display text of a button on a toolbar.
	{
		assert(::IsWindow(m_hWnd));

		int Length = (int)SendMessage(TB_GETBUTTONTEXT, idButton, 0);
		m_vTChar.assign(Length+1, _T('\0'));
		TCHAR* pTCharArray = &m_vTChar.front();

		SendMessage(TB_GETBUTTONTEXT, (LPARAM)idButton, (WPARAM)pTCharArray);
		return pTCharArray;
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

	inline HIMAGELIST CToolbar::GetDisabledImageList() const
	// Retrieves the image list that a toolbar control uses to display inactive buttons.
	{
		assert(::IsWindow(m_hWnd));
		return (HIMAGELIST)SendMessage(TB_GETDISABLEDIMAGELIST, 0L, 0L);
	}

	inline int CToolbar::GetHotItem() const
	// Retrieves the index of the hot item in a toolbar, or -1 if no hot item is set.
	{
		assert(::IsWindow(m_hWnd));
		return (int)SendMessage(TB_GETHOTITEM, 0L, 0L);
	}

	inline HIMAGELIST CToolbar::GetImageList() const
	// Retrieves the image list that a toolbar control uses to display buttons in their default state.
	{
		assert(::IsWindow(m_hWnd));
		return (HIMAGELIST)SendMessage(TB_GETIMAGELIST, 0L, 0L);
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

	inline DWORD CToolbar::GetPadding() const
	// Returns a DWORD value that contains the horizontal padding in the low word and the vertical padding in the high word, in pixels.
	{
		assert(::IsWindow(m_hWnd));
		return (DWORD)SendMessage(TB_GETPADDING, 0L, 0L);
	}

	inline CRect CToolbar::GetRect(int idButton) const
	// Retrieves the bounding rectangle for a specified toolbar button.
	{
		assert(::IsWindow(m_hWnd));
		CRect rc;
		SendMessage(TB_GETRECT, (WPARAM)idButton, (LPARAM)&rc);
		return rc;
	}

	inline int CToolbar::GetRows() const
	// Retrieves the number of rows of buttons in a toolbar with the TBSTYLE_WRAPABLE style.
	{
		assert(::IsWindow(m_hWnd));
		return (int)SendMessage(TB_GETROWS, 0L, 0L);
	}

	inline int CToolbar::GetTextRows() const
	// Retrieves the maximum number of text rows that can be displayed on a toolbar button.
	{
		assert(::IsWindow(m_hWnd));
		return (int)SendMessage(TB_GETTEXTROWS, 0L, 0L);
	}

	inline HWND CToolbar::GetToolTips() const
	// Retrieves the handle to the ToolTip control, if any, associated with the toolbar.
	{
		assert(::IsWindow(m_hWnd));
		return (HWND)SendMessage(TB_GETTOOLTIPS, 0L, 0L);
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

	inline BOOL CToolbar::HideButton(int idButton, BOOL fShow) const
	//Hides or shows the specified button in a toolbar.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_HIDEBUTTON, (WPARAM)idButton, (LPARAM)MAKELONG (fShow, 0));
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

	inline BOOL CToolbar::Indeterminate(int idButton, BOOL fIndeterminate) const
	//Hides or shows the specified button in a toolbar.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_INDETERMINATE, (WPARAM)idButton, (LPARAM)MAKELONG (fIndeterminate, 0));
	}

	inline BOOL CToolbar::InsetButton(int iButton, LPTBBUTTON lpButton) const
	// Inserts a button in a toolbar.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_INSERTBUTTON, (WPARAM)iButton, (LPARAM)lpButton);
	}

	inline BOOL CToolbar::IsButtonHidden(int idButton) const
	// Determines whether the specified button in a toolbar is hidden.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_ISBUTTONHIDDEN, (WPARAM)idButton, 0L);
	}

	inline BOOL CToolbar::IsButtonHighlighted(int idButton) const
	// Checks the highlight state of a toolbar button.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_ISBUTTONHIGHLIGHTED, (WPARAM)idButton, 0L);
	}

	inline BOOL CToolbar::IsButtonIndeterminate(int idButton) const
	// Determines whether the specified button in a toolbar is indeterminate.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_ISBUTTONINDETERMINATE, (WPARAM)idButton, 0L);
	}

	inline BOOL CToolbar::IsButtonPressed(int idButton) const
	// Determines whether the specified button in a toolbar is pressed.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_ISBUTTONPRESSED, (WPARAM)idButton, 0L);
	}

	inline int CToolbar::MapAccelerator(TCHAR chAccel) const
	// Determines whether the specified button in a toolbar is pressed.
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

	inline BOOL CToolbar::MarkButton(int idButton) const
	// Sets the highlight state of a given button in a toolbar control.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_MARKBUTTON, (WPARAM)idButton, 0L);
	}

	inline BOOL CToolbar::MoveButton(UINT uOldPos, UINT uNewPos) const
	// Moves a button from one index to another.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_MOVEBUTTON, (WPARAM)uOldPos, (LPARAM)uNewPos);
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
	}

	inline void CToolbar::PreRegisterClass(WNDCLASS &wc)
	{
		// Set the Window Class
		wc.lpszClassName =  TOOLBARCLASSNAME;
	}

	inline BOOL CToolbar::PressButton(int idButton, BOOL fPress) const
	// Presses or releases the specified button in a toolbar.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_PRESSBUTTON, (WPARAM)idButton, (LPARAM)MAKELONG(fPress, 0));
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

	inline void CToolbar::SaveRestore(BOOL fSave, TBSAVEPARAMS* ptbsp) const
	// Presses or releases the specified button in a toolbar.
	{
		assert(::IsWindow(m_hWnd));
		SendMessage(TB_PRESSBUTTON, (WPARAM)fSave, (LPARAM)ptbsp);
	}

	inline BOOL CToolbar::SetBitmap(UINT nID)
	// Set the button images
	{
		assert(::IsWindow(m_hWnd));

		HBITMAP hbm = LoadBitmap(MAKEINTRESOURCE(nID));
		assert (hbm);
		BITMAP bm = {0};

		int iResult = ::GetObject(hbm, sizeof(BITMAP), &bm);
		assert (iResult);
		UNREFERENCED_PARAMETER(iResult);

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

	inline BOOL CToolbar::SetButtonState(int idButton, UINT State) const
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

	inline BOOL CToolbar::SetButtonStyle(int idButton, BYTE Style) const
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
		return (BOOL)SendMessage(TB_SETBUTTONINFO, idButton, (LPARAM) &tbbi);
	}

	inline BOOL CToolbar::SetButtonText(int idButton, LPCTSTR szText)
	// This rather convoluted approach to setting toolbar button text supports
	// all versions of Windows, including Win95 with COMCTL32.DLL version 4.0
	{
		assert(::IsWindow(m_hWnd));
		int iIndex = CommandToIndex(idButton);
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
			Succeeded = (BOOL)SendMessage(TB_GETBUTTON, iIndex, (LPARAM)&tbb);

			tbb.iString = iString;

			// Turn off Toolbar drawing
			SendMessage(WM_SETREDRAW, FALSE, 0L);

			if (Succeeded)
				Succeeded = (BOOL)SendMessage(TB_DELETEBUTTON, iIndex, 0L);

			if (Succeeded)
				Succeeded = (BOOL)SendMessage(TB_INSERTBUTTON, iIndex, (LPARAM)&tbb);

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

	inline BOOL CToolbar::SetButtonWidth(int idButton, int nWidth) const
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
		BOOL bResult = (BOOL)SendMessage(TB_SETBUTTONINFO, (WPARAM)idButton, (LPARAM)&tbbi);

		// Send a changed message to the parent (used by the rebar)
		SIZE MaxSize = GetMaxSize();
		::SendMessage(GetParent(), UWM_TOOLBAR_RESIZE, (WPARAM)m_hWnd, (LPARAM)&MaxSize);

		return bResult;
	}

	inline BOOL CToolbar::SetCommandID(int iIndex, int idButton) const
	// Sets the command identifier of a toolbar button
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_SETCMDID, iIndex, idButton);
	}

	inline HIMAGELIST CToolbar::SetDisableImageList(HIMAGELIST himlNewDisabled) const
	// Sets the image list that the toolbar control will use to display disabled buttons.
	{
		assert(::IsWindow(m_hWnd));
		return (HIMAGELIST)SendMessage(TB_SETDISABLEDIMAGELIST, 0L, (LPARAM)himlNewDisabled);
	}

	inline DWORD CToolbar::SetDrawTextFlags(DWORD dwMask, DWORD dwDTFlags) const
	// Sets the text drawing flags for the toolbar.
	{
		assert(::IsWindow(m_hWnd));
		return (DWORD)SendMessage(TB_SETDRAWTEXTFLAGS, (WPARAM)dwMask, (LPARAM)dwDTFlags);
	}

	inline DWORD CToolbar::SetExtendedStyle(DWORD dwExStyle) const
	// Sets the text drawing flags for the toolbar.
	// Extended styles include: TBSTYLE_EX_DRAWDDARROWS, TBSTYLE_EX_HIDECLIPPEDBUTTONS, TBSTYLE_EX_DOUBLEBUFFER and TBSTYLE_EX_MIXEDBUTTONS
	{
		assert(::IsWindow(m_hWnd));
		return (DWORD)SendMessage(TB_SETEXTENDEDSTYLE, 0L, (LPARAM)dwExStyle);
	}

	inline HIMAGELIST CToolbar::SetHotImageList(HIMAGELIST himlNewHot) const
	// Sets the image list that the toolbar control will use to display hot buttons.
	{
		assert(::IsWindow(m_hWnd));
		return (HIMAGELIST)SendMessage(TB_SETHOTIMAGELIST, 0L, (LPARAM)himlNewHot);
	}

	inline HIMAGELIST CToolbar::SetImageList(HIMAGELIST himlNew) const
	// Sets the image list that the toolbar will use to display buttons that are in their default state.
	{
		assert(::IsWindow(m_hWnd));
		return (HIMAGELIST)SendMessage(TB_SETIMAGELIST, 0L, (LPARAM)himlNew);
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

		// Toolbar ImageLists require Comctl32.dll version 4.7 or later
		if (400 == GetComCtlVersion())
		{
			// We are using COMCTL32.DLL version 4.0, so we can't use an imagelist.
			// Instead we simply set the bitmap.
			return SetBitmap(ToolbarID);
		}

		int iNumButtons = 0;
		std::vector<UINT>::iterator iter;
		for (iter = GetToolbarData().begin(); iter < GetToolbarData().end(); ++iter)
			if ((*iter) != 0) ++iNumButtons;

		if (iNumButtons > 0)
		{
			// Set the button images
			HBITMAP hbm = LoadBitmap(MAKEINTRESOURCE(ToolbarID));
			assert(hbm);

			BITMAP bm = {0};
			int iResult = GetObject(hbm, sizeof(BITMAP), &bm);
			assert(iResult);
			UNREFERENCED_PARAMETER(iResult);

			int iImageWidth  = bm.bmWidth / iNumButtons;
			int iImageHeight = bm.bmHeight;

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

			if (ToolbarHotID)
			{
				hbm = LoadBitmap(MAKEINTRESOURCE(ToolbarHotID));
				assert(hbm);

				himlToolbarHot = ImageList_Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, iNumButtons, 0);
				assert(himlToolbarHot);

				ImageList_AddMasked(himlToolbarHot, hbm, crMask);
				SendMessage(TB_SETHOTIMAGELIST, 0L, (LPARAM)himlToolbarHot);

				::DeleteObject(hbm);
				hbm = NULL;
			}

			if (ToolbarDisabledID)
			{
				hbm = LoadBitmap(MAKEINTRESOURCE(ToolbarDisabledID));
				assert(hbm);

				himlToolbarDis = ImageList_Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, iNumButtons, 0);
				assert(himlToolbarDis);

				ImageList_AddMasked(himlToolbarDis, hbm, crMask);
				SendMessage(TB_SETDISABLEDIMAGELIST, 0L, (LPARAM)himlToolbarDis);
			}
			else
			{
				himlToolbarDis = CreateDisabledImageList(himlToolbar);
				SendMessage(TB_SETDISABLEDIMAGELIST, 0L, (LPARAM)himlToolbarDis);
			}

			// Inform the parent of the change (rebar needs this)
			SIZE MaxSize = GetMaxSize();
			::SendMessage(GetParent(), UWM_TOOLBAR_RESIZE, (WPARAM)m_hWnd, (LPARAM)&MaxSize);

			::DeleteObject(hbm);
		}

		return TRUE;
	}

	inline BOOL CToolbar::SetIndent(int iIndent) const
	// Sets the indentation for the first button in a toolbar control.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_SETINDENT, (WPARAM)iIndent, 0L);
	}

	inline BOOL CToolbar::SetMaxTextRows(int iMaxRows) const
	// Sets the maximum number of text rows displayed on a toolbar button.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_SETMAXTEXTROWS, (WPARAM)iMaxRows, 0L);
	}

	inline BOOL CToolbar::SetPadding(int cx, int cy) const
	// Sets the padding for a toolbar control.
	{
		assert(::IsWindow(m_hWnd));
		return (BOOL)SendMessage(TB_SETPADDING, 0L, (WPARAM)MAKELONG(cx, cy));
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

	inline void CToolbar::SetToolTips(HWND hwndToolTip) const
	// Associates a ToolTip control with a toolbar.
	{
		assert(::IsWindow(m_hWnd));
		SendMessage(TB_SETPADDING, (WPARAM)hwndToolTip, 0L);
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
