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


////////////////////////////////////////////////////////
// stdcontrols.h
//  Declaration of the CButton, CEdit, CListBox and CStatic classes

// Standard controls send notifications via WM_COMMAND
// Other controls send notifications via WM_NOTIFY


#ifndef _STDCONTROLS_H_
#define _STDCONTROLS_H_

#include "wincore.h"


namespace Win32xx
{
	class CButton : public CWnd
	{
	public:
		CButton() {}
		virtual ~CButton() {}
		virtual tString GetWindowType() const { return _T("CButton"); }

		// Attributes
		HBITMAP GetBitmap() const;
		UINT GetButtonStyle() const;
		int GetCheck() const;
		HCURSOR GetCursor() const;
		HICON GetIcon() const;
		UINT GetState() const;
		HBITMAP SetBitmap(HBITMAP hBitmap);
		void SetButtonStyle(DWORD dwStyle, BOOL bRedraw);
		void SetCheck(int nCheckState);
		HCURSOR SetCursor(HCURSOR hCursor);
		HICON SetIcon(HICON hIcon);
		void SetState(BOOL bHighlight);

	protected:
		// Overridables
		virtual void PreCreate(CREATESTRUCT& cs);
	};

	class CEdit : public CWnd
	{
	public:
		// Construction
		CEdit() {}
		virtual ~CEdit() {}
		virtual tString GetWindowType() const { return _T("CEdit"); }

		// Attributes
		BOOL CanUndo() const;
		int CharFromPos(CPoint pt) const;
		int GetFirstVisibleLine() const;
		HLOCAL GetHandle() const;
		UINT GetLimitText() const;
		int GetLine(int nIndex, LPTSTR lpszBuffer) const;
		int GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const;
		int GetLineCount() const;
		DWORD GetMargins() const;
		BOOL GetModify() const;
		TCHAR GetPasswordChar() const;
		void GetRect(LPRECT lpRect) const;
		void GetSel(int& nStartChar, int& nEndChar) const;
		DWORD GetSel() const;
		CPoint PosFromChar(UINT nChar) const;
		void SetHandle(HLOCAL hBuffer);
		void SetLimitText(UINT nMax);
		void SetMargins(UINT nLeft, UINT nRight);
		void SetModify(BOOL bModified = TRUE);

		// Operations
		void EmptyUndoBuffer();
		BOOL FmtLines(BOOL bAddEOL);
		void LimitText(int nChars = 0);
		int LineFromChar(int nIndex = -1) const;
		int LineIndex(int nLine = -1) const;
		int LineLength(int nLine = -1) const;
		void LineScroll(int nLines, int nChars = 0);
		void ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo);
		void SetPasswordChar(TCHAR ch);
		BOOL SetReadOnly(BOOL bReadOnly = TRUE);
		void SetRect(LPCRECT lpRect);
		void SetRectNP(LPCRECT lpRect);
		void SetSel(DWORD dwSelection, BOOL bNoScroll);
		void SetSel(int nStartChar, int nEndChar, BOOL bNoScroll);
		BOOL SetTabStops(int nTabStops, LPINT rgTabStops);
		void SetTabStops();
		BOOL SetTabStops(const int& cxEachStop);

		//Clipboard Operations
		void Clear();
		void Copy();
		void Cut();
		void Paste();
		void Undo();

	protected:
		// Overridables
		virtual void PreCreate(CREATESTRUCT& cs);
	};

	class CListBox : public CWnd
	{
	public:
		CListBox() {}
		virtual ~CListBox() {}
		virtual tString GetWindowType() const { return _T("CListBox"); }

		// General Operations
		int  GetCount() const;
		int  GetHorizontalExtent() const;
		DWORD GetItemData(int nIndex) const;
		void* GetItemDataPtr(int nIndex) const;
		int  GetItemHeight(int nIndex) const;
		int  GetItemRect(int nIndex, LPRECT lpRect) const;
		LCID GetLocale() const;
		int  GetSel(int nIndex) const;
		int  GetText(int nIndex, LPTSTR lpszBuffer) const;
		int  GetTextLen(int nIndex) const;
		int  GetTopIndex() const;
		UINT ItemFromPoint(CPoint pt, BOOL& bOutside ) const;
		void SetColumnWidth(int cxWidth);
		void SetHorizontalExtent(int cxExtent);
		int  SetItemData(int nIndex, DWORD dwItemData);
		int  SetItemDataPtr(int nIndex, void* pData);
		int  SetItemHeight(int nIndex, UINT cyItemHeight);
		LCID SetLocale(LCID nNewLocale);
		BOOL SetTabStops(int nTabStops, LPINT rgTabStops);
		void SetTabStops();
		BOOL SetTabStops(const int& cxEachStop);
		int  SetTopIndex(int nIndex);

		// Single-Selection Operations
		int  GetCurSel() const;
		int  SetCurSel(int nSelect);

		// Multiple-Selection Operations
		int  GetAnchorIndex() const;
		int  GetCaretIndex() const;
		int  GetSelCount() const;
		int  GetSelItems(int nMaxItems, LPINT rgIndex) const;
		int  SelItemRange(BOOL bSelect, int nFirstItem, int nLastItem);
		void SetAnchorIndex(int nIndex);
		int  SetCaretIndex(int nIndex, BOOL bScroll);
		int  SetSel(int nIndex, BOOL bSelect);

		// String Operations
		int  AddString(LPCTSTR lpszItem);
		int  DeleteString(UINT nIndex);
		int  Dir(UINT attr, LPCTSTR lpszWildCard);
		int  FindString(int nStartAfter, LPCTSTR lpszItem) const;
		int  FindStringExact(int nIndexStart, LPCTSTR lpszFind) const;
		int  InsertString(int nIndex, LPCTSTR lpszItem);
		void ResetContent();
		int  SelectString(int nStartAfter, LPCTSTR lpszItem);

	protected:
		// Overridables
		virtual void PreCreate(CREATESTRUCT& cs);
	};

	class CStatic : public CWnd
	{
	public:
		CStatic() {}
		virtual ~CStatic() {}
		virtual tString GetWindowType() const { return _T("CStatic"); }

		// Operations
		HBITMAP  GetBitmap() const;
		HCURSOR GetCursor() const;
		HENHMETAFILE GetEnhMetaFile() const;
		HICON  GetIcon() const;
		HBITMAP SetBitmap(HBITMAP hBitmap);
		HCURSOR SetCursor(HCURSOR hCursor);
		HENHMETAFILE SetEnhMetaFile(HENHMETAFILE hMetaFile);
		HICON SetIcon(HICON hIcon);

	protected:
		// Overridables
		virtual void PreCreate(CREATESTRUCT& cs);

	};

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{

	////////////////////////////////////////
	// Definitions for the CButton class
	//
	inline HBITMAP CButton::GetBitmap() const
	{
		// returns the handle to the bitmap associated with the button
		return (HBITMAP)SendMessage(BM_GETIMAGE, IMAGE_BITMAP, 0);
	}

	inline UINT CButton::GetButtonStyle() const
	{
		// returns the style of the button
		return (UINT)GetWindowLongPtr(GWL_STYLE) & 0xff;
	}

	inline int CButton::GetCheck() const
	{
		// returns the check state of the button
		return (int)SendMessage(BM_GETCHECK, 0, 0);
	}

	inline HCURSOR CButton::GetCursor() const
	{
		// returns the handle to the cursor associated withe the button
		return (HCURSOR)::SendMessage(m_hWnd, BM_GETIMAGE, IMAGE_CURSOR, 0L);
	}

	inline HICON CButton::GetIcon() const
	{
		// returns the handle to the icon associated withe the button
		return (HICON)SendMessage(BM_GETIMAGE, IMAGE_ICON, 0);
	}

	inline UINT CButton::GetState() const
	{
		// returns the state of the button
		return (UINT)SendMessage(BM_GETSTATE, 0, 0);
	}

	inline HBITMAP CButton::SetBitmap(HBITMAP hBitmap)
	{
		// sets the bitmap associated with the button
		return (HBITMAP)SendMessage(BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
	}

	inline void CButton::SetButtonStyle(DWORD dwStyle, BOOL bRedraw)
	{
		// sets the button style
		SendMessage(BM_SETSTYLE, dwStyle, bRedraw);
	}

	inline void CButton::SetCheck(int nCheckState)
	{
		// sets the button check state
		SendMessage(BM_SETCHECK, nCheckState, 0);
	}

	inline HCURSOR CButton::SetCursor(HCURSOR hCursor)
	{
		// sets the cursor associated with the button
		return (HCURSOR)SendMessage(STM_SETIMAGE, IMAGE_CURSOR, (LPARAM)hCursor);
	}

	inline HICON CButton::SetIcon(HICON hIcon)
	{
		// sets the icon associated with the button
		return (HICON)SendMessage( BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
	}

	inline void CButton::SetState(BOOL bHighlight)
	{
		// sets the button state
		SendMessage(BM_SETSTATE, bHighlight, 0);
	}

	inline void CButton::PreCreate(CREATESTRUCT& cs)
	{
		cs.lpszClass = _T("Button");
	}


	////////////////////////////////////////
	// Definitions for the CEdit class
	//
	inline BOOL CEdit::CanUndo() const
	{
		// Returns TRUE if the edit control operation can be undone.
		return (BOOL)SendMessage(EM_CANUNDO, 0, 0);
	}

	inline int CEdit::CharFromPos(CPoint pt) const
	{
		// Returns the character index and line index of the character nearest the specified point.
		return (int)SendMessage(EM_CHARFROMPOS, 0, MAKELPARAM(pt.x, pt.y));
	}

	inline int CEdit::GetFirstVisibleLine() const
	{
		// Returns the zero-based index of the first visible character in a single-line edit control 
		//  or the zero-based index of the uppermost visible line in a multiline edit control.
		return (int)SendMessage(EM_GETFIRSTVISIBLELINE, 0, 0);
	}

	inline HLOCAL CEdit::GetHandle() const
	{
		// Returns a handle identifying the buffer containing the multiline edit control's text. 
		//  It is not processed by single-line edit controls.
		return (HLOCAL)SendMessage(EM_GETHANDLE, 0, 0);
	}

	inline UINT CEdit::GetLimitText() const
	{
		// Returns the current text limit, in characters.
		return (UINT)SendMessage(EM_GETLIMITTEXT, 0, 0);
	}

	inline int CEdit::GetLine(int nIndex, LPTSTR lpszBuffer) const
	{
		// Copies characters to a buffer and returns the number of characters copied.
		return (int)::SendMessage(m_hWnd, EM_GETLINE, nIndex, (LPARAM)lpszBuffer);
	}

	inline int CEdit::GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const
	{
		// Copies characters to a buffer and returns the number of characters copied.
		*(LPWORD)lpszBuffer = (WORD)nMaxLength;
		return (int)SendMessage(EM_GETLINE, nIndex, (LPARAM)lpszBuffer);
	}

	inline int CEdit::GetLineCount() const
	{
		// Returns the number of lines in the edit control.
		return (int)SendMessage(EM_GETLINECOUNT, 0, 0);
	}

	inline DWORD CEdit::GetMargins() const
	{
		// Returns the widths of the left and right margins.
		return (DWORD)SendMessage(EM_GETMARGINS, 0, 0);
	}

	inline BOOL CEdit::GetModify() const
	{
		// Returns a flag indicating whether the content of an edit control has been modified.
		return (BOOL)SendMessage(EM_GETMODIFY, 0, 0);
	}

	inline TCHAR CEdit::GetPasswordChar() const
	{
		// Returns the character that edit controls use in conjunction with the ES_PASSWORD style.
		return (TCHAR)SendMessage(EM_GETPASSWORDCHAR, 0, 0);
	}

	inline void CEdit::GetRect(LPRECT lpRect) const
	{
		// Returns the coordinates of the formatting rectangle in an edit control.
		SendMessage(EM_GETRECT, 0, (LPARAM)lpRect);
	}

	inline void CEdit::GetSel(int& nStartChar, int& nEndChar) const
	{
		// Returns the starting and ending character positions of the current selection in the edit control.
		SendMessage(EM_GETSEL, (WPARAM)&nStartChar,(LPARAM)&nEndChar);
	}

	inline DWORD CEdit::GetSel() const
	{
		// Returns the starting and ending character positions of the current selection in the edit control.
		return (DWORD)SendMessage(EM_GETSEL, 0, 0);
	}

	inline CPoint CEdit::PosFromChar(UINT nChar) const
	{
		// Returns the client coordinates of the specified character.
		return CPoint( (DWORD)SendMessage(EM_POSFROMCHAR, nChar, 0));
	}

	inline void CEdit::SetHandle(HLOCAL hBuffer)
	{
		// Sets a handle to the memory used as a text buffer, empties the undo buffer, 
		//  resets the scroll positions to zero, and redraws the window.
		SendMessage(EM_SETHANDLE, (WPARAM)hBuffer, 0);
	}

	inline void CEdit::SetLimitText(UINT nMax)
	{
		// Sets the maximum number of characters the user may enter in the edit control. 
		SendMessage(EM_SETLIMITTEXT, (WPARAM)nMax, 0);
	}

	inline void CEdit::SetMargins(UINT nLeft, UINT nRight)
	{
		// Sets the widths of the left and right margins, and redraws the edit control to reflect the new margins.
		SendMessage(EM_SETMARGINS, EC_LEFTMARGIN|EC_RIGHTMARGIN, MAKELONG(nLeft, nRight));
	}

	inline void CEdit::SetModify(BOOL bModified)
	{
		// Sets or clears the modification flag to indicate whether the edit control has been modified.
		SendMessage(EM_SETMODIFY, bModified, 0);
	}

	inline void CEdit::EmptyUndoBuffer()
	{
		// Empties the undo buffer and sets the undo flag retrieved by the EM_CANUNDO message to FALSE.
		SendMessage(EM_EMPTYUNDOBUFFER, 0, 0);
	}

	inline BOOL CEdit::FmtLines(BOOL bAddEOL)
	{
		// Adds or removes soft line-break characters (two carriage returns and a line feed) to the ends of wrapped lines 
		//  in a multiline edit control. It is not processed by single-line edit controls.
		return (BOOL)SendMessage(EM_FMTLINES, bAddEOL, 0);
	}

	inline void CEdit::LimitText(int nChars)
	{
		// Sets the text limit of an edit control. The text limit is the maximum amount of text, in TCHARs, 
		//  that the user can type into the edit control.
		SendMessage(EM_LIMITTEXT, nChars, 0);
	}

	inline int CEdit::LineFromChar(int nIndex) const
	{
		// Returns the zero-based number of the line in a multiline edit control that contains a specified character index.
		//  This message is the reverse of the EM_LINEINDEX message.
		return (int)SendMessage(EM_LINEFROMCHAR, (WPARAM)nIndex, 0);
	}

	inline int CEdit::LineIndex(int nLine) const
	{
		// Returns the character of a line in a multiline edit control. 
		//  This message is the reverse of the EM_LINEFROMCHAR message.
		return (int)SendMessage(EM_LINEINDEX, (WPARAM)nLine, 0);
	}

	inline int CEdit::LineLength(int nLine) const
	{
		// Returns the length, in characters, of a single-line edit control. In a multiline edit control, 
		//	returns the length, in characters, of a specified line.
		return (int)SendMessage(EM_LINELENGTH, (WPARAM)nLine, 0);
	}

	inline void CEdit::LineScroll(int nLines, int nChars)
	{
		// Scrolls the text vertically in a single-line edit control or horizontally in a multiline edit control. 
		SendMessage(EM_LINESCROLL, (WPARAM)nChars, (LPARAM)nLines);
	}

	inline void CEdit::ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo)
	{
		// Replaces the current selection with the text in an application-supplied buffer, sends the parent window 
		//  EN_UPDATE and EN_CHANGE messages, and updates the undo buffer.
		SendMessage(EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpszNewText);
	}

	inline void CEdit::SetPasswordChar(TCHAR ch)
	{
		// Defines the character that edit controls use in conjunction with the ES_PASSWORD style.
		SendMessage(EM_SETPASSWORDCHAR, ch, 0);
	}

	inline BOOL CEdit::SetReadOnly(BOOL bReadOnly)
	{
		// Sets or removes the read-only style (ES_READONLY) in an edit control.
		return (BOOL)SendMessage(EM_SETREADONLY, bReadOnly, 0);
	}

	inline void CEdit::SetRect(LPCRECT lpRect)
	{
		// Sets the formatting rectangle for the multiline edit control and redraws the window.
		SendMessage(EM_SETRECT, 0, (LPARAM)lpRect);
	}

	inline void CEdit::SetRectNP(LPCRECT lpRect)
	{
		// Sets the formatting rectangle for the multiline edit control but does not redraw the window.
		SendMessage(EM_SETRECTNP, 0, (LPARAM)lpRect);
	}

	inline void CEdit::SetSel(DWORD dwSelection, BOOL bNoScroll)
	{
		// Selects a range of characters in the edit control by setting the starting and ending positions to be selected.
		SendMessage(EM_SETSEL, LOWORD(dwSelection), HIWORD(dwSelection));
		if (!bNoScroll)
			SendMessage(EM_SCROLLCARET, 0, 0);
	}

	inline void CEdit::SetSel(int nStartChar, int nEndChar, BOOL bNoScroll)
	{
		// Selects a range of characters in the edit control by setting the starting and ending positions to be selected.
		SendMessage(m_hWnd, EM_SETSEL, nStartChar, nEndChar);
		if (!bNoScroll)
			SendMessage(EM_SCROLLCARET, 0, 0);
	}

	inline BOOL CEdit::SetTabStops(int nTabStops, LPINT rgTabStops)
	{
		// Sets tab-stop positions in the multiline edit control.
		return (BOOL)::SendMessage(m_hWnd, EM_SETTABSTOPS, nTabStops, (LPARAM)rgTabStops);
	}

	inline void CEdit::SetTabStops()
	{
		// Sets tab-stop positions in the multiline edit control.
		SendMessage( EM_SETTABSTOPS, 0, 0);
	}

	inline BOOL CEdit::SetTabStops(const int& cxEachStop)
	{
		// Sets tab-stop positions in the multiline edit control.
		return (BOOL)SendMessage(EM_SETTABSTOPS, 1, (LPARAM)(LPINT)&cxEachStop);
	}

	inline void CEdit::Clear()
	{
		// Clears the current selection, if any, in an edit control.
		SendMessage(WM_CLEAR, 0, 0);
	}

	inline void CEdit::Copy()
	{
		// Copies text to the clipboard unless the style is ES_PASSWORD, in which case the message returns zero.
		SendMessage(WM_COPY,  0, 0);
	}

	inline void CEdit::Cut()
	{
		// Cuts the selection to the clipboard, or deletes the character to the left of the cursor if there is no selection.
		SendMessage(WM_CUT,   0, 0);
	}

	inline void CEdit::Paste()
	{
		// Pastes text from the clipboard into the edit control window at the caret position.
		SendMessage(WM_PASTE, 0, 0);
	}

	inline void CEdit::Undo()
	{
		// Removes any text that was just inserted or inserts any deleted characters and sets the selection to the inserted text.
		SendMessage(EM_UNDO,  0, 0);
	}

	inline void CEdit::PreCreate(CREATESTRUCT& cs)
	{
		cs.lpszClass = _T("Edit");
	}


	////////////////////////////////////////
	// Definitions for the CListbox class
	//
	inline int CListBox::GetCount() const
	{
		// Returns the number of items in the list box.
		return (int)SendMessage(LB_GETCOUNT, 0, 0);
	}

	inline int CListBox::GetHorizontalExtent() const
	{
		// Returns the scrollable width, in pixels, of a list box.
		return (int)SendMessage(LB_GETHORIZONTALEXTENT,	0, 0);
	}

	inline DWORD CListBox::GetItemData(int nIndex) const
	{
		// Returns the value associated with the specified item.
		return (DWORD)SendMessage(LB_GETITEMDATA, nIndex, 0);
	}

	inline void* CListBox::GetItemDataPtr(int nIndex) const
	{
		// Returns the value associated with the specified item.
		return (LPVOID)SendMessage(LB_GETITEMDATA, nIndex, 0);
	}

	inline int CListBox::GetItemHeight(int nIndex) const
	{
		// Returns the height, in pixels, of an item in a list box.
		return (int)SendMessage(LB_GETITEMHEIGHT, nIndex, 0L);
	}

	inline int CListBox::GetItemRect(int nIndex, LPRECT lpRect) const
	{
		// Retrieves the client coordinates of the specified list box item.
		return (int)SendMessage(LB_GETITEMRECT, nIndex, (LPARAM)lpRect);
	}

	inline LCID CListBox::GetLocale() const
	{
		// Retrieves the locale of the list box. The high-order word contains the country/region code 
		//  and the low-order word contains the language identifier.
		return (LCID)::SendMessage(m_hWnd, LB_GETLOCALE, 0, 0);
	}

	inline int CListBox::GetSel(int nIndex) const
	{
		// Returns the selection state of a list box item.
		return (int)SendMessage(LB_GETSEL, nIndex, 0);
	}

	inline int CListBox::GetText(int nIndex, LPTSTR lpszBuffer) const
	{
		// Retrieves the string associated with a specified item and the length of the string.
		return (int)::SendMessage(m_hWnd, LB_GETTEXT, nIndex, (LPARAM)lpszBuffer);
	}

	inline int CListBox::GetTextLen(int nIndex) const
	{
		// Returns the length, in characters, of the string associated with a specified item.
		return (int)SendMessage( LB_GETTEXTLEN, nIndex, 0);
	}

	inline int CListBox::GetTopIndex() const
	{
		// Returns the index of the first visible item in a list box.
		return (int)SendMessage(LB_GETTOPINDEX, 0, 0);
	}

	inline UINT CListBox::ItemFromPoint(CPoint pt, BOOL& bOutside) const
	{
		// Retrieves the zero-based index of the item nearest the specified point in a list box.
		DWORD dw = (DWORD)::SendMessage(m_hWnd, LB_ITEMFROMPOINT, 0, MAKELPARAM(pt.x, pt.y));
		bOutside = !!HIWORD(dw);
		return LOWORD(dw);
	}

	inline void CListBox::SetColumnWidth(int cxWidth)
	{
		// Sets the width, in pixels, of all columns in a list box.
		SendMessage(LB_SETCOLUMNWIDTH, cxWidth, 0);
	}

	inline void CListBox::SetHorizontalExtent(int cxExtent)
	{
		// Sets the scrollable width, in pixels, of a list box.
		SendMessage(LB_SETHORIZONTALEXTENT, cxExtent, 0);
	}

	inline int CListBox::SetItemData(int nIndex, DWORD dwItemData)
	{
		// Associates a value with a list box item.
		return (int)SendMessage(LB_SETITEMDATA, nIndex, (LPARAM)dwItemData);
	}

	inline int CListBox::SetItemDataPtr(int nIndex, void* pData)
	{
		// Associates a value with a list box item.
		return SetItemData(nIndex, (DWORD)pData);
	}

	inline int CListBox::SetItemHeight(int nIndex, UINT cyItemHeight)
	{
		// Sets the height, in pixels, of an item or items in a list box.
		return (int)SendMessage(LB_SETITEMHEIGHT, nIndex, MAKELONG(cyItemHeight, 0));
	}

	inline LCID CListBox::SetLocale(LCID nNewLocale)
	{
		// Sets the locale of a list box and returns the previous locale identifier.
		return (LCID)::SendMessage(m_hWnd, LB_SETLOCALE, (WPARAM)nNewLocale, 0);
	}

	inline BOOL CListBox::SetTabStops(int nTabStops, LPINT rgTabStops)
	{
		// Sets the tab stops to those specified in a specified array.
		return (BOOL)SendMessage(LB_SETTABSTOPS, nTabStops, (LPARAM)rgTabStops);
	}

	inline void CListBox::SetTabStops()
	{
		// Sets the tab stops to those specified in a specified array.
		SendMessage(LB_SETTABSTOPS, 0, 0);
	}

	inline BOOL CListBox::SetTabStops(const int& cxEachStop)
	{
		// Sets the tab stops to those specified in a specified array.
		return (BOOL)SendMessage(LB_SETTABSTOPS, 1, (LPARAM)(LPINT)&cxEachStop);
	}

	inline int CListBox::SetTopIndex(int nIndex)
	{
		// Scrolls the list box so the specified item is at the top of the visible range.
		return (int)SendMessage(LB_SETTOPINDEX, nIndex, 0);
	}

	inline int CListBox::GetCurSel() const
	{
		// Returns the index of the currently selected item.
		return (int)SendMessage(LB_GETCURSEL, 0, 0);
	}

	inline int CListBox::SetCurSel(int nSelect)
	{
		// Selects a specified list box item.
		return (int)SendMessage(LB_SETCURSEL, nSelect, 0);
	}

	inline int CListBox::GetAnchorIndex() const
	{
		// Returns the index of the item that the mouse last selected.
		return (int)SendMessage(LB_GETANCHORINDEX, 0, 0);
	}

	inline int CListBox::GetCaretIndex() const
	{
		// Returns the index of the item that has the focus rectangle.
		return (int)SendMessage(LB_GETCARETINDEX, 0, 0L);
	}

	inline int CListBox::GetSelCount() const
	{
		// Returns the number of selected items in a multiple-selection list box.
		return (int)SendMessage(LB_GETSELCOUNT, 0, 0);
	}

	inline int CListBox::GetSelItems(int nMaxItems, LPINT rgIndex) const
	{
		// Creates an array of the indexes of all selected items in a multiple-selection list box 
		//  and returns the total number of selected items.
		return (int)SendMessage(LB_GETSELITEMS, nMaxItems, (LPARAM)rgIndex);
	}

	inline int CListBox::SelItemRange(BOOL bSelect, int nFirstItem, int nLastItem)
	{
		// Selects a specified range of items in a list box.
		if (bSelect)
			return (int)SendMessage(LB_SELITEMRANGEEX, nFirstItem, nLastItem);
		else
			return (int)SendMessage(LB_SELITEMRANGEEX, nLastItem, nFirstItem);
	}

	inline void CListBox::SetAnchorIndex(int nIndex)
	{
		// Sets the item that the mouse last selected to a specified item.
		SendMessage(LB_SETANCHORINDEX, nIndex, 0);
	}

	inline int CListBox::SetCaretIndex(int nIndex, BOOL bScroll)
	{
		// Sets the focus rectangle to a specified list box item.
		return (int)SendMessage(LB_SETCARETINDEX, nIndex, MAKELONG(bScroll, 0));
	}

	inline int CListBox::SetSel(int nIndex, BOOL bSelect)
	{
		// Selects an item in a multiple-selection list box.
		return (int)SendMessage(LB_SETSEL, bSelect, nIndex);
	}

	inline int CListBox::AddString(LPCTSTR lpszItem)
	{
		// Adds a string to a list box and returns its index.
		return (int)SendMessage(LB_ADDSTRING, 0, (LPARAM)lpszItem);
	}

	inline int CListBox::DeleteString(UINT nIndex)
	{
		// Removes a string from a list box and returns the number of strings remaining in the list.
		return (int)SendMessage(LB_DELETESTRING, nIndex, 0);
	}

	inline int CListBox::Dir(UINT attr, LPCTSTR lpszWildCard)
	{
		// Adds a list of filenames to a list box and returns the index of the last filename added.
		return (int)SendMessage(LB_DIR, attr, (LPARAM)lpszWildCard);
	}

	inline int CListBox::FindString(int nStartAfter, LPCTSTR lpszItem) const
	{
		// Returns the index of the first string in the list box that begins with a specified string.
		return (int)SendMessage(LB_FINDSTRING, nStartAfter, (LPARAM)lpszItem);
	}

	inline int CListBox::FindStringExact(int nIndexStart, LPCTSTR lpszFind) const
	{
		// Returns the index of the string in the list box that is equal to a specified string.
		return (int)SendMessage(LB_FINDSTRINGEXACT, nIndexStart, (LPARAM)lpszFind);
	}

	inline int CListBox::InsertString(int nIndex, LPCTSTR lpszItem)
	{
		// Inserts a string at a specified index in a list box.
		return (int)SendMessage(LB_INSERTSTRING, nIndex, (LPARAM)lpszItem);
	}

	inline void CListBox::ResetContent()
	{
		// Removes all items from a list box.
		SendMessage(LB_RESETCONTENT, 0, 0);
	}

	inline int CListBox::SelectString(int nStartAfter, LPCTSTR lpszItem)
	{
		// Selects the first string it finds that matches a specified prefix.
		return (int)::SendMessage(m_hWnd, LB_SELECTSTRING, nStartAfter, (LPARAM)lpszItem);
	}

	inline void CListBox::PreCreate(CREATESTRUCT& cs)
	{
		cs.lpszClass = _T("ListBox");
	}


	////////////////////////////////////////
	// Definitions for the CStatic class
	//
	inline HBITMAP CStatic::GetBitmap() const
	{
		// Returns the handle to the bitmap for the static control
		return (HBITMAP)SendMessage(STM_GETIMAGE, IMAGE_BITMAP, 0);
	}

	inline HCURSOR CStatic::GetCursor() const
	{
		// Returns the handle to the icon for the static control
		return (HCURSOR)SendMessage(STM_GETIMAGE, IMAGE_CURSOR, 0);
	}

	inline HENHMETAFILE CStatic::GetEnhMetaFile() const
	{
		// Returns the handle to the enhanced metafile for the static control
		return (HENHMETAFILE)SendMessage(STM_GETIMAGE, IMAGE_ENHMETAFILE, 0);
	}

	inline HICON CStatic::GetIcon() const
	{
		// Returns the handle to the icon for the static control
		return (HICON)SendMessage(STM_GETIMAGE, IMAGE_ICON, 0);
	}

	inline HBITMAP CStatic::SetBitmap(HBITMAP hBitmap)
	{
		// Sets the handle to the bitmap for the static control
		return (HBITMAP)SendMessage(STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
	}

	inline HCURSOR CStatic::SetCursor(HCURSOR hCursor)
	{
		// Sets the handle to the cursor for the static control
		return (HCURSOR)SendMessage(STM_SETIMAGE, IMAGE_CURSOR, (LPARAM)hCursor);
	}

	inline HENHMETAFILE CStatic::SetEnhMetaFile(HENHMETAFILE hMetaFile)
	{
		// Sets the handle to the enhanced metafile for the static control
		return (HENHMETAFILE)SendMessage(STM_SETIMAGE, IMAGE_ENHMETAFILE, (LPARAM)hMetaFile);
	}

	inline HICON CStatic::SetIcon(HICON hIcon)
	{
		// Sets the handle to the icon for the static control
		return (HICON)SendMessage(STM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
	}

	inline void CStatic::PreCreate(CREATESTRUCT& cs)
	{
		cs.lpszClass = _T("Static");
	}

}

#endif

