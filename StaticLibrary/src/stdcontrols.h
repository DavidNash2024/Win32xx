// Win32++  Version 6.8 alpha
// Released: ?th November, 2010 by:
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
		void PreCreate(CREATESTRUCT& cs);
	};

	class CStatic : public CWnd
	{
	public:
		CStatic() {}
		virtual ~CStatic() {}

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


#endif

