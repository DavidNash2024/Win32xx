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


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{

	////////////////////////////////////////
	// Definitions for the CButton class
	//
	inline HBITMAP CButton::GetBitmap() const
	{
		return (HBITMAP)SendMessage(BM_GETIMAGE, IMAGE_BITMAP, 0);
	}

	inline UINT CButton::GetButtonStyle() const
	{
		return (UINT)GetWindowLongPtr(GWL_STYLE) & 0xff;
	}

	inline int CButton::GetCheck() const
	{
		return SendMessage(BM_GETCHECK, 0, 0);
	}

	inline HCURSOR CButton::GetCursor() const
	{
		return (HCURSOR)::SendMessage(m_hWnd, BM_GETIMAGE, IMAGE_CURSOR, 0L);
	}

	inline HICON CButton::GetIcon() const
	{
		return (HICON)SendMessage(BM_GETIMAGE, IMAGE_ICON, 0);
	}

	inline UINT CButton::GetState() const
	{
		return SendMessage(BM_GETSTATE, 0, 0);
	}

	inline HBITMAP CButton::SetBitmap(HBITMAP hBitmap)
	{
		return (HBITMAP)SendMessage(BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
	}

	inline void CButton::SetButtonStyle(DWORD dwStyle, BOOL bRedraw)
	{
		SendMessage(BM_SETSTYLE, dwStyle, bRedraw);
	}

	inline void CButton::SetCheck(int nCheckState)
	{
		SendMessage(BM_SETCHECK, nCheckState, 0);
	}

	inline HCURSOR CButton::SetCursor(HCURSOR hCursor)
	{
		return (HCURSOR)SendMessage(STM_SETIMAGE, IMAGE_CURSOR, (LPARAM)hCursor);
	}

	inline HICON CButton::SetIcon(HICON hIcon)
	{
		return (HICON)SendMessage( BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
	}

	inline void CButton::SetState(BOOL bHighlight)
	{
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
		return (BOOL)SendMessage(EM_CANUNDO, 0, 0);
	}

	inline int CEdit::CharFromPos(CPoint pt) const
	{
		return SendMessage(EM_CHARFROMPOS, 0, MAKELPARAM(pt.x, pt.y));
	}

	inline int CEdit::GetFirstVisibleLine() const
	{
		return (int)SendMessage(EM_GETFIRSTVISIBLELINE, 0, 0);
	}

	inline HLOCAL CEdit::GetHandle() const
	{
		return (HLOCAL)SendMessage(EM_GETHANDLE, 0, 0);
	}

	inline UINT CEdit::GetLimitText() const
	{
		return (UINT)SendMessage(EM_GETLIMITTEXT, 0, 0);
	}

	inline int CEdit::GetLine(int nIndex, LPTSTR lpszBuffer) const
	{
		return (int)::SendMessage(m_hWnd, EM_GETLINE, nIndex, (LPARAM)lpszBuffer);
	}

	inline int CEdit::GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const
	{
		*(LPWORD)lpszBuffer = (WORD)nMaxLength;
		return (int)SendMessage(EM_GETLINE, nIndex, (LPARAM)lpszBuffer);
	}

	inline int CEdit::GetLineCount() const
	{
		return (int)SendMessage(EM_GETLINECOUNT, 0, 0);
	}

	inline DWORD CEdit::GetMargins() const
	{
		return (DWORD)SendMessage(EM_GETMARGINS, 0, 0);
	}

	inline BOOL CEdit::GetModify() const
	{
		return (BOOL)SendMessage(EM_GETMODIFY, 0, 0);
	}

	inline TCHAR CEdit::GetPasswordChar() const
	{
		return (TCHAR)SendMessage(EM_GETPASSWORDCHAR, 0, 0);
	}

	inline void CEdit::GetRect(LPRECT lpRect) const
	{
		SendMessage(EM_GETRECT, 0, (LPARAM)lpRect);
	}

	inline void CEdit::GetSel(int& nStartChar, int& nEndChar) const
	{
		SendMessage(EM_GETSEL, (WPARAM)&nStartChar,(LPARAM)&nEndChar);
	}

	inline DWORD CEdit::GetSel() const
	{
		return SendMessage(EM_GETSEL, 0, 0);
	}

	inline CPoint CEdit::PosFromChar(UINT nChar) const
	{
		return CPoint( (DWORD)SendMessage(EM_POSFROMCHAR, nChar, 0));
	}

	inline void CEdit::SetHandle(HLOCAL hBuffer)
	{
		SendMessage(EM_SETHANDLE, (WPARAM)hBuffer, 0);
	}

	inline void CEdit::SetLimitText(UINT nMax)
	{
		SendMessage(EM_SETLIMITTEXT, (WPARAM)nMax, 0);
	}

	inline void CEdit::SetMargins(UINT nLeft, UINT nRight)
	{
		SendMessage(EM_SETMARGINS, EC_LEFTMARGIN|EC_RIGHTMARGIN, MAKELONG(nLeft, nRight));
	}

	inline void CEdit::SetModify(BOOL bModified)
	{
		SendMessage(EM_SETMODIFY, bModified, 0);
	}

	inline void CEdit::EmptyUndoBuffer()
	{
		SendMessage(EM_EMPTYUNDOBUFFER, 0, 0);
	}

	inline BOOL CEdit::FmtLines(BOOL bAddEOL)
	{
		return (BOOL)SendMessage(EM_FMTLINES, bAddEOL, 0);
	}

	inline void CEdit::LimitText(int nChars)
	{
		SendMessage(EM_LIMITTEXT, nChars, 0);
	}

	inline int CEdit::LineFromChar(int nIndex) const
	{
		return (int)SendMessage(EM_LINEFROMCHAR, (WPARAM)nIndex, 0);
	}

	inline int CEdit::LineIndex(int nLine) const
	{
		return (int)SendMessage(EM_LINEINDEX, (WPARAM)nLine, 0);
	}

	inline int CEdit::LineLength(int nLine) const
	{
		return (int)SendMessage(EM_LINELENGTH, (WPARAM)nLine, 0);
	}

	inline void CEdit::LineScroll(int nLines, int nChars)
	{
		SendMessage(EM_LINESCROLL, (WPARAM)nChars, (LPARAM)nLines);
	}

	inline void CEdit::ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo)
	{
		SendMessage(EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpszNewText);
	}

	inline void CEdit::SetPasswordChar(TCHAR ch)
	{
		SendMessage(EM_SETPASSWORDCHAR, ch, 0);
	}

	inline BOOL CEdit::SetReadOnly(BOOL bReadOnly)
	{
		return (BOOL)SendMessage(EM_SETREADONLY, bReadOnly, 0);
	}

	inline void CEdit::SetRect(LPCRECT lpRect)
	{
		SendMessage(EM_SETRECT, 0, (LPARAM)lpRect);
	}

	inline void CEdit::SetRectNP(LPCRECT lpRect)
	{
		SendMessage(EM_SETRECTNP, 0, (LPARAM)lpRect);
	}

	inline void CEdit::SetSel(DWORD dwSelection, BOOL bNoScroll)
	{
		SendMessage(EM_SETSEL, LOWORD(dwSelection), HIWORD(dwSelection));
		if (!bNoScroll)
			SendMessage(EM_SCROLLCARET, 0, 0);
	}

	inline void CEdit::SetSel(int nStartChar, int nEndChar, BOOL bNoScroll)
	{
		SendMessage(m_hWnd, EM_SETSEL, nStartChar, nEndChar);
		if (!bNoScroll)
			SendMessage(EM_SCROLLCARET, 0, 0);
	}

	inline BOOL CEdit::SetTabStops(int nTabStops, LPINT rgTabStops)
	{
		return (BOOL)::SendMessage(m_hWnd, EM_SETTABSTOPS, nTabStops, (LPARAM)rgTabStops);
	}

	inline void CEdit::SetTabStops()
	{
		SendMessage( EM_SETTABSTOPS, 0, 0);
	}

	inline BOOL CEdit::SetTabStops(const int& cxEachStop)
	{
		return (BOOL)SendMessage(EM_SETTABSTOPS, 1, (LPARAM)(LPINT)&cxEachStop);
	}

	inline void CEdit::Clear()
	{
		SendMessage(WM_CLEAR, 0, 0);
	}

	inline void CEdit::Copy()
	{
		SendMessage(WM_COPY,  0, 0);
	}

	inline void CEdit::Cut()
	{
		SendMessage(WM_CUT,   0, 0);
	}

	inline void CEdit::Paste()
	{
		SendMessage(WM_PASTE, 0, 0);
	}

	inline void CEdit::Undo()
	{
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
		return (int)SendMessage(LB_GETCOUNT, 0, 0);
	}

	inline int CListBox::GetHorizontalExtent() const
	{
		return (int)SendMessage(LB_GETHORIZONTALEXTENT,	0, 0);
	}

	inline DWORD CListBox::GetItemData(int nIndex) const
	{
		return SendMessage(LB_GETITEMDATA, nIndex, 0);
	}

	inline void* CListBox::GetItemDataPtr(int nIndex) const
	{
		return (LPVOID)SendMessage(LB_GETITEMDATA, nIndex, 0);
	}

	inline int CListBox::GetItemHeight(int nIndex) const
	{
		return (int)SendMessage(LB_GETITEMHEIGHT, nIndex, 0L);
	}

	inline int CListBox::GetItemRect(int nIndex, LPRECT lpRect) const
	{
		return (int)SendMessage(LB_GETITEMRECT, nIndex, (LPARAM)lpRect);
	}

	inline LCID CListBox::GetLocale() const
	{
		return (LCID)::SendMessage(m_hWnd, LB_GETLOCALE, 0, 0);
	}

	inline int CListBox::GetSel(int nIndex) const
	{
		return (int)SendMessage(LB_GETSEL, nIndex, 0);
	}

	inline int CListBox::GetText(int nIndex, LPTSTR lpszBuffer) const
	{
		return (int)::SendMessage(m_hWnd, LB_GETTEXT, nIndex, (LPARAM)lpszBuffer);
	}

	inline int CListBox::GetTextLen(int nIndex) const
	{
		return (int)SendMessage( LB_GETTEXTLEN, nIndex, 0);
	}

	inline int CListBox::GetTopIndex() const
	{
		return (int)SendMessage(LB_GETTOPINDEX, 0, 0);
	}

	inline UINT CListBox::ItemFromPoint(CPoint pt, BOOL& bOutside) const
	{
		DWORD dw = (DWORD)::SendMessage(m_hWnd, LB_ITEMFROMPOINT, 0, MAKELPARAM(pt.x, pt.y));
		bOutside = !!HIWORD(dw);
		return LOWORD(dw);
	}

	inline void CListBox::SetColumnWidth(int cxWidth)
	{
		SendMessage(LB_SETCOLUMNWIDTH, cxWidth, 0);
	}

	inline void CListBox::SetHorizontalExtent(int cxExtent)
	{
		SendMessage(LB_SETHORIZONTALEXTENT, cxExtent, 0);
	}

	inline int CListBox::SetItemData(int nIndex, DWORD dwItemData)
	{
		return (int)SendMessage(LB_SETITEMDATA, nIndex, (LPARAM)dwItemData);
	}

	inline int CListBox::SetItemDataPtr(int nIndex, void* pData)
	{
		return SetItemData(nIndex, (DWORD)(LPVOID)pData);
	}

	inline int CListBox::SetItemHeight(int nIndex, UINT cyItemHeight)
	{
		return (int)SendMessage(LB_SETITEMHEIGHT, nIndex, MAKELONG(cyItemHeight, 0));
	}

	inline LCID CListBox::SetLocale(LCID nNewLocale)
	{
		return (LCID)::SendMessage(m_hWnd, LB_SETLOCALE, (WPARAM)nNewLocale, 0);
	}

	inline BOOL CListBox::SetTabStops(int nTabStops, LPINT rgTabStops)
	{
		return (BOOL)SendMessage(LB_SETTABSTOPS, nTabStops, (LPARAM)rgTabStops);
	}

	inline void CListBox::SetTabStops()
	{
		SendMessage(LB_SETTABSTOPS, 0, 0);
	}

	inline BOOL CListBox::SetTabStops(const int& cxEachStop)
	{
		return (BOOL)SendMessage(LB_SETTABSTOPS, 1, (LPARAM)(LPINT)&cxEachStop);
	}

	inline int CListBox::SetTopIndex(int nIndex)
	{
		return (int)SendMessage(LB_SETTOPINDEX, nIndex, 0);
	}

	inline int CListBox::GetCurSel() const
	{
		return (int)SendMessage(LB_GETCURSEL, 0, 0);
	}

	inline int CListBox::SetCurSel(int nSelect)
	{
		return (int)SendMessage(LB_SETCURSEL, nSelect, 0);
	}

	inline int CListBox::GetAnchorIndex() const
	{
		return (int)SendMessage(LB_GETANCHORINDEX, 0, 0);
	}

	inline int CListBox::GetCaretIndex() const
	{
		return (int)SendMessage(LB_GETCARETINDEX, 0, 0L);
	}

	inline int CListBox::GetSelCount() const
	{
		return (int)SendMessage(LB_GETSELCOUNT, 0, 0);
	}

	inline int CListBox::GetSelItems(int nMaxItems, LPINT rgIndex) const
	{
		return (int)SendMessage(LB_GETSELITEMS, nMaxItems, (LPARAM)rgIndex);
	}

	inline int CListBox::SelItemRange(BOOL bSelect, int nFirstItem, int nLastItem)
	{
		if (bSelect)
			return (int)SendMessage(LB_SELITEMRANGEEX, nFirstItem, nLastItem);
		else
			return (int)SendMessage(LB_SELITEMRANGEEX, nLastItem, nFirstItem);
	}

	inline void CListBox::SetAnchorIndex(int nIndex)
	{
		SendMessage(LB_SETANCHORINDEX, nIndex, 0);
	}

	inline int CListBox::SetCaretIndex(int nIndex, BOOL bScroll)
	{
		return (int)SendMessage(LB_SETCARETINDEX, nIndex, MAKELONG(bScroll, 0));
	}

	inline int CListBox::SetSel(int nIndex, BOOL bSelect)
	{
		return (int)SendMessage(LB_SETSEL, bSelect, nIndex);
	}

	inline int CListBox::AddString(LPCTSTR lpszItem)
	{
		return (int)SendMessage(LB_ADDSTRING, 0, (LPARAM)lpszItem);
	}

	inline int CListBox::DeleteString(UINT nIndex)
	{
		return (int)SendMessage(LB_DELETESTRING, nIndex, 0);
	}

	inline int CListBox::Dir(UINT attr, LPCTSTR lpszWildCard)
	{
		return (int)SendMessage(LB_DIR, attr, (LPARAM)lpszWildCard);
	}

	inline int CListBox::FindString(int nStartAfter, LPCTSTR lpszItem) const
	{
		return (int)SendMessage(LB_FINDSTRING, nStartAfter, (LPARAM)lpszItem);
	}

	inline int CListBox::FindStringExact(int nIndexStart, LPCTSTR lpszFind) const
	{
		return (int)SendMessage(LB_FINDSTRINGEXACT, nIndexStart, (LPARAM)lpszFind);
	}

	inline int CListBox::InsertString(int nIndex, LPCTSTR lpszItem)
	{
		return (int)SendMessage(LB_INSERTSTRING, nIndex, (LPARAM)lpszItem);
	}

	inline void CListBox::ResetContent()
	{
		SendMessage(LB_RESETCONTENT, 0, 0);
	}

	inline int CListBox::SelectString(int nStartAfter, LPCTSTR lpszItem)
	{
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
		return (HBITMAP)SendMessage(STM_GETIMAGE, IMAGE_BITMAP, 0);
	}

	inline HCURSOR CStatic::GetCursor() const
	{
		return (HCURSOR)SendMessage(STM_GETIMAGE, IMAGE_CURSOR, 0);
	}

	inline HENHMETAFILE CStatic::GetEnhMetaFile() const
	{
		return (HENHMETAFILE)SendMessage(STM_GETIMAGE, IMAGE_ENHMETAFILE, 0);
	}

	inline HICON CStatic::GetIcon() const
	{
		return (HICON)SendMessage(STM_GETIMAGE, IMAGE_ICON, 0);
	}

	inline HBITMAP CStatic::SetBitmap(HBITMAP hBitmap)
	{
		return (HBITMAP)SendMessage(STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
	}

	inline HCURSOR CStatic::SetCursor(HCURSOR hCursor)
	{
		return (HCURSOR)SendMessage(STM_SETIMAGE, IMAGE_CURSOR, (LPARAM)hCursor);
	}

	inline HENHMETAFILE CStatic::SetEnhMetaFile(HENHMETAFILE hMetaFile)
	{
		return (HENHMETAFILE)SendMessage(STM_SETIMAGE, IMAGE_ENHMETAFILE, (LPARAM)hMetaFile);
	}

	inline HICON CStatic::SetIcon(HICON hIcon)
	{
		return (HICON)SendMessage(STM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
	}

	inline void CStatic::PreCreate(CREATESTRUCT& cs)
	{
		cs.lpszClass = _T("Static");
	}

}

#endif

