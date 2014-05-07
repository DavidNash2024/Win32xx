// Win32++   Version 7.5
// Released: 7th May 2014
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


///////////////////////////////////////////////////////
// rich_edit.h
//  Declaration of the CRichEdit class

#ifndef _WIN32XX_RICH_EDIT_H_
#define _WIN32XX_RICH_EDIT_H_


#include "wincore.h"
#include "gdi.h"
#include <Richedit.h>
#include <Richole.h>


namespace Win32xx
{

#ifdef __GNUC__
   //	UndoName info (required by GNU compilers)
    typedef enum _undonameid
    {
        UID_UNKNOWN     = 0,
        UID_TYPING		= 1,
        UID_DELETE 		= 2,
        UID_DRAGDROP	= 3,
        UID_CUT			= 4,
        UID_PASTE		= 5,
        UID_AUTOCORRECT = 6
    } UNDONAMEID;
#endif

	///////////////////////////////////////////////
	// Declarations for the CRichEdit class
	//
	class CRichEdit : public CWnd
	{
	public:
		CRichEdit();
		virtual ~CRichEdit();

		BOOL 	CanPaste(UINT nFormat = 0) const;
		BOOL 	CanRedo() const;
		BOOL 	CanUndo() const;
		int 	CharFromPos(CPoint pt) const;
		void 	Clear() const;
		void 	Copy() const;
		void 	Cut() const;
		BOOL 	DisplayBand(LPRECT pDisplayRect) const;
		void 	EmptyUndoBuffer() const;
		long 	FindText(DWORD dwFlags, FINDTEXTEX* pFindTextEx) const;
		DWORD 	FindWordBreak(UINT nCode, DWORD nStart) const;
		long  	FormatRange(FORMATRANGE* pfr, BOOL bDisplay = TRUE) const;
		CPoint 	GetCharPos(long lChar) const;
		DWORD 	GetDefaultCharFormat(CHARFORMAT& cf) const;
		DWORD 	GetDefaultCharFormat(CHARFORMAT2& cf) const;
		long 	GetEventMask() const;
		int 	GetFirstVisibleLine() const;
		IRichEditOle* GetIRichEditOle() const;
		long 	GetLimitText() const;
		int 	GetLine(int nIndex, LPTSTR lpszBuffer) const;
		int 	GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const;
		int 	GetLineCount() const;
		BOOL 	GetModify() const;
		UINT 	GetOptions() const;
		DWORD 	GetParaFormat(PARAFORMAT& pf) const;
		DWORD 	GetParaFormat(PARAFORMAT2& pf) const;
		void 	GetRect(LPRECT lpRect) const;
		UNDONAMEID GetRedoName() const;
		void 	GetSel(CHARRANGE& cr) const;
		void 	GetSel(long& nStartChar, long& nEndChar) const;
		DWORD 	GetSelectionCharFormat(CHARFORMAT& cf) const;
		DWORD 	GetSelectionCharFormat(CHARFORMAT2& cf) const;
		WORD 	GetSelectionType() const;
		long 	GetSelText(LPSTR lpBuf) const;
		CString GetSelText() const;
		long 	GetTextLength() const;
		long 	GetTextLengthEx(DWORD dwFlags, UINT uCodePage = -1) const;
		UINT 	GetTextMode() const;
		int 	GetTextRange(int nFirst, int nLast, CString& refString) const;
		UNDONAMEID GetUndoName() const;
		void	 HideSelection(BOOL bHide, BOOL bPerm) const;
		void 	LimitText(long nChars = 0) const;
		long 	LineFromChar(long nIndex) const;
		int 	LineIndex(int nLine = -1) const;
		int 	LineLength(int nLine = -1) const;
		void 	LineScroll(int nLines, int nChars = 0) const;
		void 	Paste() const;
		void 	PasteSpecial(UINT nClipFormat, DWORD dwAspect = 0, HMETAFILE hMF = 0) const;
		CPoint 	PosFromChar(UINT nChar) const;
		BOOL 	Redo() const;
		void 	ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo = FALSE) const;
		void 	RequestResize() const;
		BOOL 	SetAutoURLDetect(BOOL bEnable = TRUE) const;
		COLORREF SetBackgroundColor(BOOL bSysColor, COLORREF cr) const;
		BOOL 	SetDefaultCharFormat(CHARFORMAT& cf) const;
		BOOL 	SetDefaultCharFormat(CHARFORMAT2& cf) const;
		DWORD 	SetEventMask(DWORD dwEventMask) const;
		void 	SetModify(BOOL bModified = TRUE) const;
		BOOL 	SetOLECallback(IRichEditOleCallback* pCallback) const;
		void 	SetOptions(WORD wOp, DWORD dwFlags) const;
		BOOL 	SetParaFormat(PARAFORMAT& pf) const;
		BOOL 	SetParaFormat(PARAFORMAT2& pf) const;
		BOOL 	SetPunctuation(UINT fType, PUNCTUATION* lpPunc) const;
		BOOL 	SetReadOnly(BOOL bReadOnly = TRUE) const;
		void 	SetRect(LPCRECT lpRect) const;
		void 	SetSel(long nStartChar, long nEndChar) const;
		void 	SetSel(CHARRANGE& cr) const;
		BOOL 	SetSelectionCharFormat(CHARFORMAT& cf) const;
		BOOL 	SetSelectionCharFormat(CHARFORMAT2& cf) const;
		BOOL 	SetTargetDevice(HDC hDC, long lLineWidth) const;
		BOOL 	SetTargetDevice(CDC& dc, long lLineWidth) const;
		BOOL 	SetTextMode(UINT fMode) const;
		UINT 	SetUndoLimit(UINT nLimit) const;
		BOOL 	SetWordCharFormat(CHARFORMAT& cf) const;
		BOOL 	SetWordCharFormat(CHARFORMAT2& cf) const;
		void 	StopGroupTyping() const;
		long 	StreamIn(int nFormat, EDITSTREAM& es) const;
		long 	StreamOut(int nFormat, EDITSTREAM& es) const;
		BOOL 	Undo() const;

	protected:
		void	PreRegisterClass(WNDCLASS &wc);

	private:
		HMODULE m_hmodRich;
	};

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{

	///////////////////////////////////////////////
	// Definitions of the CRichEdit class
	//

	inline CRichEdit::CRichEdit()
	{
		// Load RichEdit 2.0 or 3.0
		m_hmodRich = LoadLibrary(_T("riched20.dll"));
		assert(m_hmodRich);
	}

	inline CRichEdit::~CRichEdit()
	{
		// Destroy the window before freeing the DLL
		Destroy();

		FreeLibrary(m_hmodRich);
	}

	inline void CRichEdit::PreRegisterClass(WNDCLASS &wc)
	{
		// Set the Window Class
		wc.lpszClassName =  RICHEDIT_CLASS;
		wc.style = ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP;
	}

	inline BOOL CRichEdit::CanPaste(UINT /* nFormat = 0 */) const
	// Determines whether a rich edit control can paste a specified clipboard format.
	{
		assert(IsWindow());
		return SendMessage(EM_CANPASTE, 0, 0);
	}

	inline BOOL CRichEdit::CanRedo() const
	// Determines whether there are any actions in the control redo queue.
	{
		assert(IsWindow());
		return SendMessage(EM_CANREDO, 0, 0);
	}

	inline BOOL CRichEdit::CanUndo() const
	// Determines whether there are any actions in an edit control's undo queue.
	{
		assert(IsWindow());
		return SendMessage(EM_CANUNDO, 0, 0);
	}

	inline int CRichEdit::CharFromPos(CPoint pt) const
	// Gets information about the character closest to a specified point in the client area of an edit control.
	{
		assert(IsWindow());
		return SendMessage(EM_CHARFROMPOS, (WPARAM)&pt, 0);
	}

	inline void CRichEdit::Clear() const
	// Delete (clear) the current selection
	{
		assert(IsWindow());
		SendMessage(WM_CLEAR, 0, 0);
	}

	inline void CRichEdit::Copy() const
	// Copy the current selection to the clipboard in CF_TEXT format.
	{
		assert(IsWindow());
		SendMessage(WM_COPY, 0, 0);
	}

	inline void CRichEdit::Cut() const
	// Delete (cut) the current selection, if any, in the edit control and copy the deleted text to the clipboard in CF_TEXT format.
	{
		assert(IsWindow());
		SendMessage(WM_CUT, 0, 0);
	}

	inline BOOL CRichEdit::DisplayBand(LPRECT pDisplayRect) const
	// Displays a portion of the contents of a rich edit control, as previously formatted for a device using the EM_FORMATRANGE message.
	{
		assert(IsWindow());
		return SendMessage(EM_DISPLAYBAND, 0, (LPARAM)pDisplayRect);
	}

	inline void CRichEdit::EmptyUndoBuffer() const
	// Resets the undo flag of the rich edit control. The undo flag is set whenever an operation within the edit control can be undone.
	{
		assert(IsWindow());
		SendMessage(EM_EMPTYUNDOBUFFER, 0, 0);
	}

	inline long CRichEdit::FindText(DWORD dwFlags, FINDTEXTEX* pFindTextEx) const
	// Finds text within the rich edit control.
	{
		assert(IsWindow());
		return SendMessage(EM_FINDTEXTEX, (WPARAM)dwFlags,  (LPARAM)pFindTextEx);
	}

	inline DWORD CRichEdit::FindWordBreak(UINT nCode, DWORD nStart) const
	// Finds the next word break before or after the specified character position or retrieves information
	// about the character at that position.
	{
		assert(IsWindow());
		return SendMessage(EM_FINDWORDBREAK, (WPARAM)nCode, (LPARAM)nStart);
	}

	inline long CRichEdit::FormatRange(FORMATRANGE* pfr, BOOL bDisplay /* = TRUE */) const
	// Formats a range of text in a rich edit control for a specific device (e.g. printer).

	{
		assert(IsWindow());
		return SendMessage(EM_FORMATRANGE, (WPARAM)bDisplay, (LPARAM)pfr);
	}

	inline CPoint CRichEdit::GetCharPos(long lChar) const
	// Retrieves the client area coordinates of a specified character.
	{
		assert(IsWindow());
		CPoint pt;
		SendMessage(EM_POSFROMCHAR, (WPARAM)&pt, (LPARAM)lChar);
		return pt;
	}

	inline DWORD CRichEdit::GetDefaultCharFormat(CHARFORMAT& cf) const
	// Retrieves the current default character formatting attributes.
	{
		assert(IsWindow());
		return SendMessage(EM_GETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cf);
	}

	inline DWORD CRichEdit::GetDefaultCharFormat(CHARFORMAT2& cf) const
	// Retrieves the current default character formatting attributes.
	{
		assert(IsWindow());
		return SendMessage(EM_GETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cf);
	}

	inline long CRichEdit::GetEventMask() const
	// Retrieves the event mask. The event mask specifies which notification messages the control sends to its parent window.
	{
		assert(IsWindow());
		return SendMessage(EM_GETEVENTMASK, 0, 0);
	}

	inline int CRichEdit::GetFirstVisibleLine() const
	// Gets the zero-based index of the uppermost visible line.
	{
		assert(IsWindow());
		return SendMessage(EM_GETFIRSTVISIBLELINE, 0, 0);
	}

	inline IRichEditOle* CRichEdit::GetIRichEditOle() const
	// Retrieves an IRichEditOle object that a client can use to access the rich edit control's Component Object Model (COM) functionality.
	{
		assert(IsWindow());

		IRichEditOle* pRichEditOle;
		SendMessage(EM_GETOLEINTERFACE, 0, (LPARAM)pRichEditOle);
		return pRichEditOle;
	}

	inline long CRichEdit::GetLimitText() const
	// Gets the current text limit for the edit control.
	{
		assert(IsWindow());
		return SendMessage(EM_GETLIMITTEXT, 0, 0);
	}

	inline int CRichEdit::GetLine(int nIndex, LPTSTR lpszBuffer) const
	// Copies a line of text from the rich edit control and places it in the specified buffer.
	// lpszBuffer is a pointer to the buffer that receives a copy of the line. Before sending the message,
	// set the first word of this buffer to the size, in TCHARs, of the buffer.
	{
		assert(IsWindow());
		return SendMessage(EM_GETLINE, (WPARAM)nIndex, (LPARAM)lpszBuffer);
	}

	inline int CRichEdit::GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const
	// Copies a line of text from the rich edit control and places it in the specified buffer.
	{
		assert(IsWindow());
		lpszBuffer[0] = (TCHAR)nMaxLength;
		return SendMessage(EM_GETLINE, (WPARAM)nIndex, (LPARAM)lpszBuffer);
	}

	inline int CRichEdit::GetLineCount() const
	// Gets the number of lines in a multiline edit control.
	{
		assert(IsWindow());
		return SendMessage(EM_GETLINECOUNT, 0, 0);
	}

	inline BOOL CRichEdit::GetModify() const
	// Retrieves a flag than indicates whether the contents of the edit control have been modified.
	{
		assert(IsWindow());
		return SendMessage(EM_GETMODIFY, 0, 0);
	}

	inline UINT CRichEdit::GetOptions() const
	// Retrieves the rich edit control options.
	{
		assert(IsWindow());
		return SendMessage(EM_GETOPTIONS, 0, 0);
	}

	inline DWORD CRichEdit::GetParaFormat(PARAFORMAT& pf) const
	// Retrieves the paragraph formatting of the current selection.
	{
		assert(IsWindow());
		return SendMessage(EM_GETPARAFORMAT, 0, (LPARAM)&pf);
	}

	inline DWORD CRichEdit::GetParaFormat(PARAFORMAT2& pf) const
	// Retrieves the paragraph formatting of the current selection.
	{
		assert(IsWindow());
		return SendMessage(EM_GETPARAFORMAT, 0, (LPARAM)&pf);
	}

	inline void CRichEdit::GetRect(LPRECT lpRect) const
	// Retrieves the formatting rectangle. The formatting rectangle is the limiting
	// rectangle into which text can be drawn.
	{
		assert(IsWindow());
		SendMessage(EM_GETRECT, 0, (LPARAM)lpRect);
	}

	inline UNDONAMEID CRichEdit::GetRedoName() const
	// Retrieves the type of the next action, if any, in the control's redo queue.
	{
		assert(IsWindow());
		return (UNDONAMEID)SendMessage(EM_GETREDONAME, 0, 0);
	}

	inline void CRichEdit::GetSel(CHARRANGE& cr) const
	// Retrieves the starting and ending character positions of the selection.
	{
		assert(IsWindow());
		SendMessage(EM_EXGETSEL, 0, (LPARAM)&cr);
	}

	inline void CRichEdit::GetSel(long& nStartChar, long& nEndChar) const
	// Gets the starting and ending positions of the current selection.
	{
		assert(IsWindow());
		CHARRANGE cr;

		SendMessage(EM_EXGETSEL, 0, (LPARAM)&cr);
		nStartChar = cr.cpMin;
		nEndChar   = cr.cpMax;
	}

	inline DWORD CRichEdit::GetSelectionCharFormat(CHARFORMAT& cf) const
	// Retrieves the character formatting attributes in the current selection.
	{
		assert(IsWindow());
		return SendMessage(EM_GETCHARFORMAT, (WPARAM)SCF_SELECTION, (LPARAM)&cf);
	}

	inline DWORD CRichEdit::GetSelectionCharFormat(CHARFORMAT2& cf) const
	// Retrieves the character formatting attributes in the current selection.
	{
		assert(IsWindow());
		return SendMessage(EM_GETCHARFORMAT, (WPARAM)SCF_SELECTION, (LPARAM)&cf);
	}


	inline WORD CRichEdit::GetSelectionType() const
	// Retrieves the type of contents in the current selection.
	{
		assert(IsWindow());
		return (WORD)SendMessage(EM_SELECTIONTYPE, 0, 0);
	}

	inline long CRichEdit::GetSelText(LPSTR lpBuf) const
	// Gets the text of the current selection.
	{
		assert(IsWindow());
		return SendMessage(EM_GETSELTEXT, 0, (LPARAM)lpBuf);
	}

	inline CString CRichEdit::GetSelText() const
	// Gets the text of the current selection.
	{
		assert(IsWindow());

		CString str;
		int nBuff = SendMessage(EM_GETSELTEXT, 0, 0);
		SendMessage(EM_GETSELTEXT, 0, (LPARAM)str.GetBuffer(nBuff));
		str.ReleaseBuffer();
		return str;
	}

	inline long CRichEdit::GetTextLength() const
	// Retrieves the length of the text, in characters. Does not include the terminating null character.
	{
		assert(IsWindow());
		return SendMessage(WM_GETTEXTLENGTH, 0, 0);
	}

	inline long CRichEdit::GetTextLengthEx(DWORD dwFlags, UINT uCodePage /* = -1 */) const
	// Returns the number of TCHARs in the rich edit control, depending on the flags specified.
	{
		assert(IsWindow());
		GETTEXTLENGTHEX gtle;
		gtle.flags = dwFlags;
		gtle.codepage = uCodePage;

		return SendMessage(EM_GETTEXTLENGTHEX, (WPARAM)&gtle, 0);
	}

	inline UINT CRichEdit::GetTextMode() const
	// Retrieves the current text mode and undo level.
	{
		assert(IsWindow());
		return SendMessage(EM_GETTEXTMODE, 0, 0);
	}

	inline int CRichEdit::GetTextRange(int nFirst, int nLast, CString& refString) const
	// Retrieves the specified range of text.
	{
		assert(IsWindow());
		CHARRANGE chrg;
		chrg.cpMin = nFirst;
		chrg.cpMax = nLast;

		TEXTRANGE tr;
		tr.chrg = chrg;
		tr.lpstrText = refString.GetBuffer(nLast - nFirst + 1);
		SendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&tr);
		refString.ReleaseBuffer();

		return refString.GetLength();
	}

	inline UNDONAMEID CRichEdit::GetUndoName() const
	// Retrieves the type of the next undo action, if any.
	{
		assert(IsWindow());
		return (UNDONAMEID)SendMessage(EM_GETREDONAME, 0, 0);
	}

	inline void CRichEdit::HideSelection(BOOL bHide, BOOL bPerm) const
	// Shows or hides the current selection.
	{
		assert(IsWindow());
		if (bPerm)
		{
			SetOptions(ECOOP_SET, ECO_NOHIDESEL);
		}

		SendMessage(EM_HIDESELECTION, (WPARAM)bHide, 0);
	}

	inline void CRichEdit::LimitText(long nChars /* = 0 */) const
	// Limits the amount of text a user can enter.
	{
		assert(IsWindow());
		SendMessage(EM_EXLIMITTEXT, 0, (LPARAM)nChars);
	}

	inline long CRichEdit::LineFromChar(long nIndex) const
	// Determines which line contains the given character.
	{
		assert(IsWindow());
		return (long)SendMessage(EM_EXLINEFROMCHAR, 0, (LPARAM)nIndex);
	}

	inline int CRichEdit::LineIndex(int nLine /* = -1 */) const
	// Retrieves the character index of a given line.
	{
		assert(IsWindow());
		return SendMessage(EM_LINEINDEX, nLine, 0);
	}

	inline int CRichEdit::LineLength(int nLine /* = -1 */) const
	// Retrieves the length of a given line.
	{
		assert(IsWindow());
		return SendMessage(EM_LINELENGTH, nLine, 0);
	}

	inline void CRichEdit::LineScroll(int nLines, int nChars /* = 0 */) const
	// Scrolls the text.
	{
		assert(IsWindow());
		SendMessage(EM_LINESCROLL, (WPARAM)nChars, (LPARAM)nLines);
	}

	inline void CRichEdit::Paste() const
	// Inserts the contents of the Clipboard.
	{
		assert(IsWindow());
		SendMessage(WM_PASTE, 0, 0);
	}

	inline void CRichEdit::PasteSpecial(UINT nClipFormat, DWORD dwAspect /* = 0 */, HMETAFILE hMF /* = 0 */) const
	// Inserts the contents of the Clipboard in the specified data format.
	{
		assert(IsWindow());

		REPASTESPECIAL rps;
		rps.dwAspect = dwAspect;
		rps.dwParam = (DWORD_PTR)hMF;
		SendMessage(EM_PASTESPECIAL, (WPARAM)nClipFormat, (LPARAM)&rps);
	}

	inline CPoint CRichEdit::PosFromChar(UINT nChar) const
	// Retrieves the client area coordinates of a specified character.
	{
		assert(IsWindow());
		DWORD dwPos = SendMessage(EM_POSFROMCHAR, 0, (LPARAM)nChar);
		return CPoint(LOWORD(dwPos), HIWORD(dwPos));
	}

	inline BOOL CRichEdit::Redo() const
	// Redoes the next action in the control's redo queue.
	{
		assert(IsWindow());
		return SendMessage(EM_REDO, 0, 0);
	}

	inline void CRichEdit::ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo /* = FALSE */) const
	// Replaces the current selection with specified text.
	{
		assert(IsWindow());
		SendMessage(EM_REPLACESEL, (WPARAM)bCanUndo, (LPARAM)lpszNewText);
	}

	inline void CRichEdit::RequestResize() const
	// Forces the sending of a request resize notifications.
	{
		assert(IsWindow());
		SendMessage(EM_REQUESTRESIZE, 0, 0);
	}

	inline BOOL CRichEdit::SetAutoURLDetect(BOOL bEnable /* = TRUE */) const
	// Indicates if the auto URL detection is active.
	{
		assert(IsWindow());
		return SendMessage(EM_AUTOURLDETECT, (WPARAM)bEnable, 0);
	}

	inline COLORREF CRichEdit::SetBackgroundColor(BOOL bSysColor, COLORREF cr) const
	// Sets the background color.
	{
		assert(IsWindow());
		return SendMessage(EM_SETBKGNDCOLOR, (WPARAM)bSysColor, (LPARAM)cr);
	}

	inline BOOL CRichEdit::SetDefaultCharFormat(CHARFORMAT& cf) const
	// Sets the current default character formatting attributes.
	{
		assert(IsWindow());
		return SendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&cf);
	}

	inline BOOL CRichEdit::SetDefaultCharFormat(CHARFORMAT2& cf) const
	// Sets the current default character formatting attributes.
	{
		assert(IsWindow());
		return SendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&cf);
	}

	inline DWORD CRichEdit::SetEventMask(DWORD dwEventMask) const
	// Sets the event mask. The event mask specifies which notification messages the control sends to its parent window.
	{
		assert(IsWindow());
		return SendMessage(EM_SETEVENTMASK, 0, dwEventMask);
	}

	inline void CRichEdit::SetModify(BOOL bModified /* = TRUE */) const
	// Sets or clears the modification flag. The modification flag indicates whether the text has been modified.
	{
		assert(IsWindow());
		SendMessage(EM_SETMODIFY, bModified, 0);
	}

	inline BOOL CRichEdit::SetOLECallback(IRichEditOleCallback* pCallback) const
	// Sets the IRichEditOleCallback COM object.
	{
		assert(IsWindow());
		return SendMessage(EM_SETOLECALLBACK, 0, (LPARAM)pCallback);
	}

	inline void CRichEdit::SetOptions(WORD wOp, DWORD dwFlags) const
	// Sets the options.
	// Possible wOp values: ECOOP_SET, ECOOP_OR, ECOOP_AND, ECOOP_XOR
	// Possible dwFlags: ECO_AUTOWORDSELECTION, ECO_AUTOVSCROLL, ECO_AUTOHSCROLL, ECO_NOHIDESEL
	//                   ECO_READONLY, ECO_WANTRETURN, ECO_SELECTIONBAR, ECO_VERTICAL
	{
		assert(IsWindow());
		SendMessage(EM_SETOPTIONS, (WPARAM)wOp, (LPARAM)dwFlags);
	}

	inline BOOL CRichEdit::SetParaFormat(PARAFORMAT& pf) const
	// Sets the paragraph formatting attributes in the current selection.
	{
		assert(IsWindow());
		return SendMessage(EM_SETPARAFORMAT, 0, (LPARAM)&pf);
	}

	inline BOOL CRichEdit::SetParaFormat(PARAFORMAT2& pf) const
	// Sets the paragraph formatting attributes in the current selection.
	{
		assert(IsWindow());
		return SendMessage(EM_SETPARAFORMAT, 0, (LPARAM)&pf);
	}

	inline BOOL CRichEdit::SetReadOnly(BOOL bReadOnly /* = TRUE*/) const
	// Sets or removes the read-only style.
	{
		assert(IsWindow());
		return SendMessage(EM_SETREADONLY, (WPARAM)bReadOnly, 0);
	}

	inline void CRichEdit::SetRect(LPCRECT lpRect) const
	// Sets the formatting rectangle. The formatting rectangle is the limiting rectangle into which the control draws the text.
	{
		assert(IsWindow());
		SendMessage(EM_SETRECT, 0, (LPARAM)lpRect);
	}

	inline void CRichEdit::SetSel(long nStartChar, long nEndChar) const
	// Selects a range of characters.
	{
		assert(IsWindow());

		CHARRANGE cr;
		cr.cpMin = nStartChar;
		cr.cpMax = nEndChar;
		SendMessage(EM_EXSETSEL, 0, (LPARAM)&cr);
	}

	inline void CRichEdit::SetSel(CHARRANGE& cr) const
	// Selects a range of characters.
	{
		assert(IsWindow());
		SendMessage(EM_EXSETSEL, 0, (LPARAM)&cr);
	}

	inline BOOL CRichEdit::SetSelectionCharFormat(CHARFORMAT& cf) const
	// Sets the character formatting attributes in the current selection.
	{
		assert(IsWindow());
		return SendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&cf);
	}

	inline BOOL CRichEdit::SetSelectionCharFormat(CHARFORMAT2& cf) const
	// Sets the character formatting attributes in the current selection.
	{
		assert(IsWindow());
		return SendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&cf);
	}

	inline BOOL CRichEdit::SetTargetDevice(HDC hDC, long lLineWidth) const
	// Sets the target output device and line width used for "what you see is what you get" (WYSIWYG) formatting.
	{
		assert(IsWindow());
		return SendMessage(EM_SETTARGETDEVICE, (WPARAM)hDC, (LPARAM)lLineWidth);
	}

	inline BOOL CRichEdit::SetTargetDevice(CDC& dc, long lLineWidth) const
	// Sets the target output device and line width used for "what you see is what you get" (WYSIWYG) formatting.
	{
		assert(IsWindow());
		return SendMessage(EM_SETTARGETDEVICE, (WPARAM)dc.GetHDC(), (LPARAM)lLineWidth);
	}

	inline BOOL CRichEdit::SetTextMode(UINT fMode) const
	// Sets the text mode or undo level of the rich edit control. The message fails if the control contains text.
	{
		assert(IsWindow());
		return SendMessage(EM_SETTEXTMODE, (WPARAM)fMode, 0);
	}

	inline UINT CRichEdit::SetUndoLimit(UINT nLimit) const
	// Sets the maximum number of actions that can stored in the undo queue.
	// This member function fails if the control contains text.
	{
		assert(IsWindow());
		return (UINT)SendMessage(EM_SETUNDOLIMIT, (WPARAM)nLimit, 0);
	}

	inline BOOL CRichEdit::SetWordCharFormat(CHARFORMAT& cf) const
	// Sets the character formatting attributes in the current word.
	{
		assert(IsWindow());
		return (BOOL)SendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&cf);
	}

	inline BOOL CRichEdit::SetWordCharFormat(CHARFORMAT2& cf) const
	// Sets the character formatting attributes in the current word.
	{
		assert(IsWindow());
		return (BOOL)SendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&cf);
	}

	inline void CRichEdit::StopGroupTyping() const
	// Stops the control from collecting additional typing actions into the current undo action.
	// The control stores the next typing action, if any, into a new action in the undo queue.
	{
		assert(IsWindow());
		SendMessage(EM_STOPGROUPTYPING, 0, 0);
	}

	inline long CRichEdit::StreamIn(int nFormat, EDITSTREAM& es) const
	// Replaces text with text from the specified input stream.
	{
		assert(IsWindow());
		return SendMessage(EM_STREAMIN, (WPARAM)nFormat, (LPARAM)&es);
	}

	inline long CRichEdit::StreamOut(int nFormat, EDITSTREAM& es) const
	// Stores text into an output stream.
	{
		assert(IsWindow());
		return SendMessage(EM_STREAMOUT, (WPARAM)nFormat, (LPARAM)&es);
	}

	inline BOOL CRichEdit::Undo() const
	// Reverses the last editing operation.
	{
		assert(IsWindow());
		return SendMessage(EM_UNDO, 0, 0);
	}

}

#endif   // _WIN32XX_RICHEDIT_H_
