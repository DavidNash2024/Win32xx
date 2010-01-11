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


#include "stdcontrols.h"

namespace Win32xx
{

	////////////////////////////////////////
	// Definitions for the CButton class
	//
	HBITMAP CButton::GetBitmap() const
	{
		return (HBITMAP)SendMessage(BM_GETIMAGE, IMAGE_BITMAP, 0);
	}

	UINT CButton::GetButtonStyle() const
	{
		return (UINT)GetWindowLongPtr(GWL_STYLE) & 0xff;
	}

	int CButton::GetCheck() const
	{
		return SendMessage(BM_GETCHECK, 0, 0);
	}

	HCURSOR CButton::GetCursor() const
	{
		return (HCURSOR)::SendMessage(m_hWnd, BM_GETIMAGE, IMAGE_CURSOR, 0L);
	}

	HICON CButton::GetIcon() const
	{
		return (HICON)SendMessage(BM_GETIMAGE, IMAGE_ICON, 0);
	}

	UINT CButton::GetState() const
	{
		return SendMessage(BM_GETSTATE, 0, 0);
	}

	HBITMAP CButton::SetBitmap(HBITMAP hBitmap)
	{
		return (HBITMAP)SendMessage(BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
	}

	void CButton::SetButtonStyle(DWORD dwStyle, BOOL bRedraw)
	{
		SendMessage(BM_SETSTYLE, dwStyle, bRedraw);
	}

	void CButton::SetCheck(int nCheckState)
	{
		SendMessage(BM_SETCHECK, nCheckState, 0);
	}

	HCURSOR CButton::SetCursor(HCURSOR hCursor)
	{
		return (HCURSOR)SendMessage(STM_SETIMAGE, IMAGE_CURSOR, (LPARAM)hCursor);
	}

	HICON CButton::SetIcon(HICON hIcon)
	{
		return (HICON)SendMessage( BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
	}

	void CButton::SetState(BOOL bHighlight)
	{
		SendMessage(BM_SETSTATE, bHighlight, 0);
	}

	void CButton::PreCreate(CREATESTRUCT& cs)
	{
		cs.lpszClass = _T("Button");
	}


	////////////////////////////////////////
	// Definitions for the CEdit class
	//
	BOOL CEdit::CanUndo() const
	{
		return (BOOL)SendMessage(EM_CANUNDO, 0, 0);
	}

	int CEdit::CharFromPos(CPoint pt) const
	{
		return SendMessage(EM_CHARFROMPOS, 0, MAKELPARAM(pt.x, pt.y));
	}

	int CEdit::GetFirstVisibleLine() const
	{
		return (int)SendMessage(EM_GETFIRSTVISIBLELINE, 0, 0);
	}

	HLOCAL CEdit::GetHandle() const
	{
		return (HLOCAL)SendMessage(EM_GETHANDLE, 0, 0);
	}

	UINT CEdit::GetLimitText() const
	{
		return (UINT)SendMessage(EM_GETLIMITTEXT, 0, 0);
	}

	int CEdit::GetLine(int nIndex, LPTSTR lpszBuffer) const
	{
		return (int)::SendMessage(m_hWnd, EM_GETLINE, nIndex, (LPARAM)lpszBuffer);
	}

	int CEdit::GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const
	{
		*(LPWORD)lpszBuffer = (WORD)nMaxLength;
		return (int)SendMessage(EM_GETLINE, nIndex, (LPARAM)lpszBuffer);
	}

	int CEdit::GetLineCount() const
	{
		return (int)SendMessage(EM_GETLINECOUNT, 0, 0);
	}

	DWORD CEdit::GetMargins() const
	{
		return (DWORD)SendMessage(EM_GETMARGINS, 0, 0);
	}

	BOOL CEdit::GetModify() const
	{
		return (BOOL)SendMessage(EM_GETMODIFY, 0, 0);
	}

	TCHAR CEdit::GetPasswordChar() const
	{
		return (TCHAR)SendMessage(EM_GETPASSWORDCHAR, 0, 0);
	}

	void CEdit::GetRect(LPRECT lpRect) const
	{
		SendMessage(EM_GETRECT, 0, (LPARAM)lpRect);
	}

	void CEdit::GetSel(int& nStartChar, int& nEndChar) const
	{
		SendMessage(EM_GETSEL, (WPARAM)&nStartChar,(LPARAM)&nEndChar);
	}

	DWORD CEdit::GetSel() const
	{
		return SendMessage(EM_GETSEL, 0, 0);
	}

	CPoint CEdit::PosFromChar(UINT nChar) const
	{
		return CPoint( (DWORD)SendMessage(EM_POSFROMCHAR, nChar, 0));
	}

	void CEdit::SetHandle(HLOCAL hBuffer)
	{
		SendMessage(EM_SETHANDLE, (WPARAM)hBuffer, 0);
	}

	void CEdit::SetLimitText(UINT nMax)
	{
		SendMessage(EM_SETLIMITTEXT, (WPARAM)nMax, 0);
	}

	void CEdit::SetMargins(UINT nLeft, UINT nRight)
	{
		SendMessage(EM_SETMARGINS, EC_LEFTMARGIN|EC_RIGHTMARGIN, MAKELONG(nLeft, nRight));
	}

	void CEdit::SetModify(BOOL bModified)
	{
		SendMessage(EM_SETMODIFY, bModified, 0);
	}

	void CEdit::EmptyUndoBuffer()
	{
		SendMessage(EM_EMPTYUNDOBUFFER, 0, 0);
	}

	BOOL CEdit::FmtLines(BOOL bAddEOL)
	{
		return (BOOL)SendMessage(EM_FMTLINES, bAddEOL, 0);
	}

	void CEdit::LimitText(int nChars)
	{
		SendMessage(EM_LIMITTEXT, nChars, 0);
	}

	int CEdit::LineFromChar(int nIndex) const
	{
		return (int)SendMessage(EM_LINEFROMCHAR, (WPARAM)nIndex, 0);
	}

	int CEdit::LineIndex(int nLine) const
	{
		return (int)SendMessage(EM_LINEINDEX, (WPARAM)nLine, 0);
	}

	int CEdit::LineLength(int nLine) const
	{
		return (int)SendMessage(EM_LINELENGTH, (WPARAM)nLine, 0);
	}

	void CEdit::LineScroll(int nLines, int nChars)
	{
		SendMessage(EM_LINESCROLL, (WPARAM)nChars, (LPARAM)nLines);
	}

	void CEdit::ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo)
	{
		SendMessage(EM_REPLACESEL, (WPARAM) bCanUndo, (LPARAM)lpszNewText);
	}

	void CEdit::SetPasswordChar(TCHAR ch)
	{
		SendMessage(EM_SETPASSWORDCHAR, ch, 0);
	}

	BOOL CEdit::SetReadOnly(BOOL bReadOnly)
	{
		return (BOOL)SendMessage(EM_SETREADONLY, bReadOnly, 0);
	}

	void CEdit::SetRect(LPCRECT lpRect)
	{
		SendMessage(EM_SETRECT, 0, (LPARAM)lpRect);
	}

	void CEdit::SetRectNP(LPCRECT lpRect)
	{
		SendMessage(EM_SETRECTNP, 0, (LPARAM)lpRect);
	}

	void CEdit::SetSel(DWORD dwSelection, BOOL bNoScroll)
	{
		SendMessage(EM_SETSEL, LOWORD(dwSelection), HIWORD(dwSelection));
		if (!bNoScroll)
			SendMessage(EM_SCROLLCARET, 0, 0);
	}

	void CEdit::SetSel(int nStartChar, int nEndChar, BOOL bNoScroll)
	{
		SendMessage(m_hWnd, EM_SETSEL, nStartChar, nEndChar);
		if (!bNoScroll)
			SendMessage(EM_SCROLLCARET, 0, 0);
	}

	BOOL CEdit::SetTabStops(int nTabStops, LPINT rgTabStops)
	{
		return (BOOL)::SendMessage(m_hWnd, EM_SETTABSTOPS, nTabStops, (LPARAM)rgTabStops);
	}

	void CEdit::SetTabStops()
	{
		SendMessage( EM_SETTABSTOPS, 0, 0);
	}

	BOOL CEdit::SetTabStops(const int& cxEachStop)
	{
		return (BOOL)SendMessage(EM_SETTABSTOPS, 1, (LPARAM)(LPINT)&cxEachStop);
	}

	void CEdit::Clear()
	{
		SendMessage(WM_CLEAR, 0, 0);
	}

	void CEdit::Copy()
	{
		SendMessage(WM_COPY,  0, 0);
	}

	void CEdit::Cut()
	{
		SendMessage(WM_CUT,   0, 0);
	}

	void CEdit::Paste()
	{
		SendMessage(WM_PASTE, 0, 0);
	}

	void CEdit::Undo()
	{
		SendMessage(EM_UNDO,  0, 0);
	}

	void CEdit::PreCreate(CREATESTRUCT& cs)
	{
		cs.lpszClass = _T("Edit");
	}


	////////////////////////////////////////
	// Definitions for the CListbox class
	//
	int CListBox::GetCount() const
	{
		return (int)SendMessage(LB_GETCOUNT, 0, 0);
	}

	int CListBox::GetHorizontalExtent() const
	{
		return (int)SendMessage(LB_GETHORIZONTALEXTENT,	0, 0);
	}

	DWORD CListBox::GetItemData(int nIndex) const
	{
		return SendMessage(LB_GETITEMDATA, nIndex, 0);
	}

	void* CListBox::GetItemDataPtr(int nIndex) const
	{
		return (LPVOID)SendMessage(LB_GETITEMDATA, nIndex, 0);
	}

	int CListBox::GetItemHeight(int nIndex) const
	{
		return (int)SendMessage(LB_GETITEMHEIGHT, nIndex, 0L);
	}

	int CListBox::GetItemRect(int nIndex, LPRECT lpRect) const
	{
		return (int)SendMessage(LB_GETITEMRECT, nIndex, (LPARAM)lpRect);
	}

	LCID CListBox::GetLocale() const
	{
		return (LCID)::SendMessage(m_hWnd, LB_GETLOCALE, 0, 0);
	}

	int CListBox::GetSel(int nIndex) const
	{
		return (int)SendMessage(LB_GETSEL, nIndex, 0);
	}

	int CListBox::GetText(int nIndex, LPTSTR lpszBuffer) const
	{
		return (int)::SendMessage(m_hWnd, LB_GETTEXT, nIndex, (LPARAM)lpszBuffer);
	}

	int CListBox::GetTextLen(int nIndex) const
	{
		return (int)SendMessage( LB_GETTEXTLEN, nIndex, 0);
	}

	int CListBox::GetTopIndex() const
	{
		return (int)SendMessage(LB_GETTOPINDEX, 0, 0);
	}

	UINT CListBox::ItemFromPoint(CPoint pt, BOOL& bOutside) const
	{
		DWORD dw = (DWORD)::SendMessage(m_hWnd, LB_ITEMFROMPOINT, 0, MAKELPARAM(pt.x, pt.y));
		bOutside = !!HIWORD(dw);
		return LOWORD(dw);
	}

	void CListBox::SetColumnWidth(int cxWidth)
	{
		SendMessage(LB_SETCOLUMNWIDTH, cxWidth, 0);
	}

	void CListBox::SetHorizontalExtent(int cxExtent)
	{
		SendMessage(LB_SETHORIZONTALEXTENT, cxExtent, 0);
	}

	int CListBox::SetItemData(int nIndex, DWORD dwItemData)
	{
		return (int)SendMessage(LB_SETITEMDATA, nIndex, (LPARAM)dwItemData);
	}

	int CListBox::SetItemDataPtr(int nIndex, void* pData)
	{
		return SetItemData(nIndex, (DWORD)(LPVOID)pData);
	}

	int CListBox::SetItemHeight(int nIndex, UINT cyItemHeight)
	{
		return (int)SendMessage(LB_SETITEMHEIGHT, nIndex, MAKELONG(cyItemHeight, 0));
	}

	LCID CListBox::SetLocale(LCID nNewLocale)
	{
		return (LCID)::SendMessage(m_hWnd, LB_SETLOCALE, (WPARAM)nNewLocale, 0);
	}

	BOOL CListBox::SetTabStops(int nTabStops, LPINT rgTabStops)
	{
		return (BOOL)SendMessage(LB_SETTABSTOPS, nTabStops, (LPARAM)rgTabStops);
	}

	void CListBox::SetTabStops()
	{
		SendMessage(LB_SETTABSTOPS, 0, 0);
	}

	BOOL CListBox::SetTabStops(const int& cxEachStop)
	{
		return (BOOL)SendMessage(LB_SETTABSTOPS, 1, (LPARAM)(LPINT)&cxEachStop);
	}

	int CListBox::SetTopIndex(int nIndex)
	{
		return (int)SendMessage(LB_SETTOPINDEX, nIndex, 0);
	}

	int CListBox::GetCurSel() const
	{
		return (int)SendMessage(LB_GETCURSEL, 0, 0);
	}

	int CListBox::SetCurSel(int nSelect)
	{
		return (int)SendMessage(LB_SETCURSEL, nSelect, 0);
	}

	int CListBox::GetAnchorIndex() const
	{
		return (int)SendMessage(LB_GETANCHORINDEX, 0, 0);
	}

	int CListBox::GetCaretIndex() const
	{
		return (int)SendMessage(LB_GETCARETINDEX, 0, 0L);
	}

	int CListBox::GetSelCount() const
	{
		return (int)SendMessage(LB_GETSELCOUNT, 0, 0);
	}

	int CListBox::GetSelItems(int nMaxItems, LPINT rgIndex) const
	{
		return (int)SendMessage(LB_GETSELITEMS, nMaxItems, (LPARAM)rgIndex);
	}

	int CListBox::SelItemRange(BOOL bSelect, int nFirstItem, int nLastItem)
	{
		if (bSelect)
			return (int)SendMessage(LB_SELITEMRANGEEX, nFirstItem, nLastItem);
		else
			return (int)SendMessage(LB_SELITEMRANGEEX, nLastItem, nFirstItem);
	}

	void CListBox::SetAnchorIndex(int nIndex)
	{
		SendMessage(LB_SETANCHORINDEX, nIndex, 0);
	}

	int CListBox::SetCaretIndex(int nIndex, BOOL bScroll)
	{
		return (int)SendMessage(LB_SETCARETINDEX, nIndex, MAKELONG(bScroll, 0));
	}

	int CListBox::SetSel(int nIndex, BOOL bSelect)
	{
		return (int)SendMessage(LB_SETSEL, bSelect, nIndex);
	}

	int CListBox::AddString(LPCTSTR lpszItem)
	{
		return (int)SendMessage(LB_ADDSTRING, 0, (LPARAM)lpszItem);
	}

	int CListBox::DeleteString(UINT nIndex)
	{
		return (int)SendMessage(LB_DELETESTRING, nIndex, 0);
	}

	int CListBox::Dir(UINT attr, LPCTSTR lpszWildCard)
	{
		return (int)SendMessage(LB_DIR, attr, (LPARAM)lpszWildCard);
	}

	int CListBox::FindString(int nStartAfter, LPCTSTR lpszItem) const
	{
		return (int)SendMessage(LB_FINDSTRING, nStartAfter, (LPARAM)lpszItem);
	}

	int CListBox::FindStringExact(int nIndexStart, LPCTSTR lpszFind) const
	{
		return (int)SendMessage(LB_FINDSTRINGEXACT, nIndexStart, (LPARAM)lpszFind);
	}

	int CListBox::InsertString(int nIndex, LPCTSTR lpszItem)
	{
		return (int)SendMessage(LB_INSERTSTRING, nIndex, (LPARAM)lpszItem);
	}

	void CListBox::ResetContent()
	{
		SendMessage(LB_RESETCONTENT, 0, 0);
	}

	int CListBox::SelectString(int nStartAfter, LPCTSTR lpszItem)
	{
		return (int)::SendMessage(m_hWnd, LB_SELECTSTRING, nStartAfter, (LPARAM)lpszItem);
	}

	void CListBox::PreCreate(CREATESTRUCT& cs)
	{
		cs.lpszClass = _T("ListBox");
	}


	////////////////////////////////////////
	// Definitions for the CStatic class
	//
	HBITMAP CStatic::GetBitmap() const
	{
		return (HBITMAP)SendMessage(STM_GETIMAGE, IMAGE_BITMAP, 0);
	}

	HCURSOR CStatic::GetCursor() const
	{
		return (HCURSOR)SendMessage(STM_GETIMAGE, IMAGE_CURSOR, 0);
	}

	HENHMETAFILE CStatic::GetEnhMetaFile() const
	{
		return (HENHMETAFILE)SendMessage(STM_GETIMAGE, IMAGE_ENHMETAFILE, 0);
	}

	HICON CStatic::GetIcon() const
	{
		return (HICON)SendMessage(STM_GETIMAGE, IMAGE_ICON, 0);
	}

	HBITMAP CStatic::SetBitmap(HBITMAP hBitmap)
	{
		return (HBITMAP)SendMessage(STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
	}

	HCURSOR CStatic::SetCursor(HCURSOR hCursor)
	{
		return (HCURSOR)SendMessage(STM_SETIMAGE, IMAGE_CURSOR, (LPARAM)hCursor);
	}

	HENHMETAFILE CStatic::SetEnhMetaFile(HENHMETAFILE hMetaFile)
	{
		return (HENHMETAFILE)SendMessage(STM_SETIMAGE, IMAGE_ENHMETAFILE, (LPARAM)hMetaFile);
	}

	HICON CStatic::SetIcon(HICON hIcon)
	{
		return (HICON)SendMessage(STM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
	}

	void CStatic::PreCreate(CREATESTRUCT& cs)
	{
		cs.lpszClass = _T("Static");
	}

}
