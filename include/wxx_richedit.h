// Win32++   Version 8.5.1
// Release Date: TBA
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2017  David Nash
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
// wxx_rich_edit.h
//  Declaration of the CRichEdit class

#ifndef _WIN32XX_RICH_EDIT_H_
#define _WIN32XX_RICH_EDIT_H_


#include "wxx_wincore.h"
#include "wxx_gdi.h"
#include <Richedit.h>
#include <Richole.h>


namespace Win32xx
{

#ifdef __GNUC__
   //   UndoName info (required by GNU compilers)
    typedef enum _undonameid
    {
        UID_UNKNOWN     = 0,
        UID_TYPING      = 1,
        UID_DELETE      = 2,
        UID_DRAGDROP    = 3,
        UID_CUT         = 4,
        UID_PASTE       = 5,
        UID_AUTOCORRECT = 6
    } UNDONAMEID;
#endif

    ///////////////////////////////////////////////
    // The CRichEdit class provides the functionality a rich edit control.
    // It provides methods to create a rich edit control and modify its contents.
    class CRichEdit : public CWnd
    {
    public:
        CRichEdit();
        virtual ~CRichEdit();

        BOOL    CanPaste(UINT format = 0) const;
        BOOL    CanRedo() const;
        BOOL    CanUndo() const;
        int     CharFromPos(CPoint pt) const;
        void    Clear() const;
        void    Copy() const;
        void    Cut() const;
        BOOL    DisplayBand(const RECT& rc) const;
        void    EmptyUndoBuffer() const;
        long    FindText(DWORD flags, const FINDTEXTEX& findInfo) const;
        DWORD   FindWordBreak(UINT code, DWORD start) const;
        long    FormatRange(const FORMATRANGE& fr, BOOL display = TRUE) const;
        long    FormatRange(BOOL display = FALSE) const;
        CPoint  GetCharPos(long charIndex) const;
        DWORD   GetDefaultCharFormat(CHARFORMAT& format) const;
        DWORD   GetDefaultCharFormat(CHARFORMAT2& format) const;
        long    GetEventMask() const;
        int     GetFirstVisibleLine() const;
        IRichEditOle* GetIRichEditOle() const;
        long    GetLimitText() const;
        int     GetLine(int index, LPTSTR pBuffer) const;
        int     GetLine(int index, LPTSTR pBuffer, int maxLength) const;
        int     GetLineCount() const;
        BOOL    GetModify() const;
        UINT    GetOptions() const;
        DWORD   GetParaFormat(PARAFORMAT& format) const;
        DWORD   GetParaFormat(PARAFORMAT2& format) const;
        BOOL    GetPunctuation(UINT type, const PUNCTUATION& puncInfo) const;
        void    GetRect(RECT& rc) const;
        UNDONAMEID GetRedoName() const;
        void    GetSel(CHARRANGE& range) const;
        void    GetSel(long& startChar, long& endChar) const;
        DWORD   GetSelectionCharFormat(CHARFORMAT& cf) const;
        DWORD   GetSelectionCharFormat(CHARFORMAT2& cf) const;
        WORD    GetSelectionType() const;
        long    GetSelText(LPSTR pBuf) const;
        CString GetSelText() const;
        long    GetTextLength() const;
        long    GetTextLengthEx(DWORD flags, UINT codePage = -1) const;
        UINT    GetTextMode() const;
        CString GetTextRange(int first, int last) const;
        UNDONAMEID GetUndoName() const;
        void     HideSelection(BOOL hide, BOOL isPermanent) const;
        void    LimitText(long limit = 0) const;
        long    LineFromChar(long charIndex) const;
        int     LineIndex(int lineIndex = -1) const;
        int     LineLength(int charIndex = -1) const;
        void    LineScroll(int lines) const;
        void    Paste() const;
        void    PasteSpecial(UINT clipFormat, DWORD aspect = 0, HMETAFILE hMF = 0) const;
        CPoint  PosFromChar(UINT fromChar) const;
        BOOL    Redo() const;
        void    ReplaceSel(LPCTSTR pNewText, BOOL canUndo = FALSE) const;
        void    RequestResize() const;
        BOOL    SetAutoURLDetect(BOOL enable = TRUE) const;
        COLORREF SetBackgroundColor(BOOL isSysColor, COLORREF color) const;
        BOOL    SetDefaultCharFormat(CHARFORMAT& format) const;
        BOOL    SetDefaultCharFormat(CHARFORMAT2& format) const;
        DWORD   SetEventMask(DWORD eventMask) const;
        void    SetModify(BOOL isModified = TRUE) const;
        BOOL    SetOLECallback(IRichEditOleCallback* pCallback) const;
        void    SetOptions(WORD options, DWORD flags) const;
        BOOL    SetParaFormat(PARAFORMAT& pf) const;
        BOOL    SetParaFormat(PARAFORMAT2& pf) const;
        BOOL    SetPunctuation(UINT type, const PUNCTUATION& puncInfo) const;
        BOOL    SetReadOnly(BOOL isReadOnly = TRUE) const;
        void    SetRect(const RECT& rc) const;
        void    SetSel(long startChar, long endChar) const;
        void    SetSel(CHARRANGE& cr) const;
        BOOL    SetSelectionCharFormat(CHARFORMAT& cf) const;
        BOOL    SetSelectionCharFormat(CHARFORMAT2& cf) const;
        BOOL    SetTargetDevice(HDC hdc, long lineWidth) const;
        BOOL    SetTargetDevice(CDC& dc, long lineWidth) const;
        BOOL    SetTextMode(UINT mode) const;
        UINT    SetUndoLimit(UINT limit) const;
        BOOL    SetWordCharFormat(CHARFORMAT& format) const;
        BOOL    SetWordCharFormat(CHARFORMAT2& format) const;
        void    StopGroupTyping() const;
        long    StreamIn(int format, EDITSTREAM& stream) const;
        long    StreamOut(int format, EDITSTREAM& stream) const;
        BOOL    Undo() const;

    protected:
        void    PreRegisterClass(WNDCLASS& wc);

    private:
        HMODULE m_hmodRich1;
        HMODULE m_hmodRich2;
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
        // Load RichEdit version 1.0
        // Note: Many dialogs use RichEdit version 1.0
        m_hmodRich1 = LoadLibrary(_T("riched32.dll"));

        if (m_hmodRich1 == 0)
            throw CNotSupportedException(_T("Failed to load RICHED32.DLL"));

        // Load RichEdit version 2.0 or 3.0 (if we can)
        m_hmodRich2 = LoadLibrary(_T("riched20.dll"));
    }


    inline CRichEdit::~CRichEdit()
    {
        // Destroy the window before freeing the DLL
        Destroy();

        ::FreeLibrary(m_hmodRich1);
        if (m_hmodRich2)
            ::FreeLibrary(m_hmodRich2);
    }


    // Set the window class
    inline void CRichEdit::PreRegisterClass(WNDCLASS& wc)
    {
        wc.lpszClassName =  RICHEDIT_CLASS;
        wc.style = ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP;
    }


    // Determines whether a rich edit control can paste a specified clipboard format.
    inline BOOL CRichEdit::CanPaste(UINT /* nFormat = 0 */) const
    {
        assert(IsWindow());
        return (0 != SendMessage(EM_CANPASTE, 0, 0));
    }


    // Determines whether there are any actions in the control redo queue.
    inline BOOL CRichEdit::CanRedo() const
    {
        assert(IsWindow());
        return (0 != SendMessage(EM_CANREDO, 0, 0));
    }


    // Determines whether there are any actions in an edit control's undo queue.
    inline BOOL CRichEdit::CanUndo() const
    {
        assert(IsWindow());
        return (0 != SendMessage(EM_CANUNDO, 0, 0));
    }


    // Gets information about the character closest to a specified point in the client area of an edit control.
    inline int CRichEdit::CharFromPos(CPoint pt) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(EM_CHARFROMPOS, reinterpret_cast<WPARAM>(&pt), 0));
    }


    // Delete (clear) the current selection
    inline void CRichEdit::Clear() const
    {
        assert(IsWindow());
        SendMessage(WM_CLEAR, 0, 0);
    }


    // Copy the current selection to the clipboard in CF_TEXT format.
    inline void CRichEdit::Copy() const
    {
        assert(IsWindow());
        SendMessage(WM_COPY, 0, 0);
    }


    // Delete (cut) the current selection, if any, in the edit control and copy the deleted text to the clipboard in CF_TEXT format.
    inline void CRichEdit::Cut() const
    {
        assert(IsWindow());
        SendMessage(WM_CUT, 0, 0);
    }


    // Displays a portion of the contents of a rich edit control, as previously formatted for a device using the EM_FORMATRANGE message.
    inline BOOL CRichEdit::DisplayBand(const RECT& rc) const
    {
        assert(IsWindow());
        return (0 != SendMessage(EM_DISPLAYBAND, 0, reinterpret_cast<LPARAM>(&rc)));
    }


    // Resets the undo flag of the rich edit control. The undo flag is set whenever an operation within the edit control can be undone.
    inline void CRichEdit::EmptyUndoBuffer() const
    {
        assert(IsWindow());
        SendMessage(EM_EMPTYUNDOBUFFER, 0, 0);
    }


    // Finds text within the rich edit control.
    inline long CRichEdit::FindText(DWORD flags, const FINDTEXTEX& findInfo) const
    {
        assert(IsWindow());

    #ifdef UNICODE
        UINT em_findText = EM_FINDTEXTEXW;
     #else
         UINT em_findText = EM_FINDTEXTEX;
     #endif

        return static_cast<long>(SendMessage(em_findText, flags,  reinterpret_cast<LPARAM>(&findInfo)));
    }


    // Finds the next word break before or after the specified character position or retrieves information
    // about the character at that position.
    inline DWORD CRichEdit::FindWordBreak(UINT code, DWORD start) const
    {
        assert(IsWindow());
        return static_cast<DWORD>(SendMessage(EM_FINDWORDBREAK, code, start));
    }


    // Formats a range of text in a rich edit control for a specific device (e.g. printer).
    inline long CRichEdit::FormatRange(const FORMATRANGE& fr, BOOL display /* = TRUE */) const
    {
        assert(IsWindow());
        return static_cast<long>(SendMessage(EM_FORMATRANGE, display, reinterpret_cast<LPARAM>(&fr)));
    }


    // Free format information cached by the control.
    inline long CRichEdit::FormatRange(BOOL display /* = FALSE */) const
    {
        assert(IsWindow());
        return static_cast<long>(SendMessage(EM_FORMATRANGE, display, 0));
    }


    // Retrieves the client area coordinates of a specified character.
    inline CPoint CRichEdit::GetCharPos(long charIndex) const
    {
        assert(IsWindow());
        CPoint pt;
        SendMessage(EM_POSFROMCHAR, reinterpret_cast<WPARAM>(&pt), charIndex);
        return pt;
    }


    // Retrieves the current default character formatting attributes.
    inline DWORD CRichEdit::GetDefaultCharFormat(CHARFORMAT& format) const
    {
        assert(IsWindow());
        return static_cast<DWORD>(SendMessage(EM_GETCHARFORMAT, SCF_DEFAULT, reinterpret_cast<LPARAM>(&format)));
    }


    // Retrieves the current default character formatting attributes.
    inline DWORD CRichEdit::GetDefaultCharFormat(CHARFORMAT2& cf) const
    {
        assert(IsWindow());
        return static_cast<DWORD>(SendMessage(EM_GETCHARFORMAT, SCF_DEFAULT, reinterpret_cast<LPARAM>(&cf)));
    }


    // Retrieves the event mask. The event mask specifies which notification messages the control sends to its parent window.
    inline long CRichEdit::GetEventMask() const
    {
        assert(IsWindow());
        return static_cast<long>(SendMessage(EM_GETEVENTMASK, 0, 0));
    }


    // Gets the zero-based index of the uppermost visible line.
    inline int CRichEdit::GetFirstVisibleLine() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(EM_GETFIRSTVISIBLELINE, 0, 0));
    }


    // Retrieves an IRichEditOle object that a client can use to access the rich edit control's Component Object Model (COM) functionality.
    inline IRichEditOle* CRichEdit::GetIRichEditOle() const
    {
        assert(IsWindow());

        IRichEditOle* pRichEditOle = NULL;
        SendMessage(EM_GETOLEINTERFACE, 0, reinterpret_cast<LPARAM>(pRichEditOle));
        return pRichEditOle;
    }


    // Gets the current text limit for the edit control.
    inline long CRichEdit::GetLimitText() const
    {
        assert(IsWindow());
        return static_cast<long>(SendMessage(EM_GETLIMITTEXT, 0, 0));
    }


    // Copies a line of text from the rich edit control and places it in the specified buffer.
    // lpszBuffer is a pointer to the buffer that receives a copy of the line. Before sending the message,
    // set the first word of this buffer to the size, in TCHARs, of the buffer.
    inline int CRichEdit::GetLine(int index, LPTSTR pBuffer) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(EM_GETLINE, index, reinterpret_cast<LPARAM>(pBuffer)));
    }


    // Copies a line of text from the rich edit control and places it in the specified buffer.
    inline int CRichEdit::GetLine(int index, LPTSTR pBuffer, int maxLength) const
    {
        assert(IsWindow());
        pBuffer[0] = (TCHAR)maxLength;
        return static_cast<int>(SendMessage(EM_GETLINE, index, reinterpret_cast<LPARAM>(pBuffer)));
    }


    // Gets the number of lines in a multiline edit control.
    inline int CRichEdit::GetLineCount() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(EM_GETLINECOUNT, 0, 0));
    }


    // Retrieves a flag than indicates whether the contents of the edit control have been modified.
    inline BOOL CRichEdit::GetModify() const
    {
        assert(IsWindow());
        return (0 != SendMessage(EM_GETMODIFY, 0, 0));
    }


    // Retrieves the rich edit control options.
    inline UINT CRichEdit::GetOptions() const
    {
        assert(IsWindow());
        return static_cast<UINT>(SendMessage(EM_GETOPTIONS, 0, 0));
    }


    // Retrieves the paragraph formatting of the current selection.
    inline DWORD CRichEdit::GetParaFormat(PARAFORMAT& format) const
    {
        assert(IsWindow());
        return static_cast<DWORD>(SendMessage(EM_GETPARAFORMAT, 0, reinterpret_cast<LPARAM>(&format)));
    }


    // Retrieves the paragraph formatting of the current selection.
    inline DWORD CRichEdit::GetParaFormat(PARAFORMAT2& pf) const
    {
        assert(IsWindow());
        return static_cast<DWORD>(SendMessage(EM_GETPARAFORMAT, 0, reinterpret_cast<LPARAM>(&pf)));
    }


    // Retrieves the current punctuation characters for the rich edit control.
    // This is available only in Asian-language versions of the operating system.
    inline BOOL CRichEdit::GetPunctuation(UINT type, const PUNCTUATION& puncInfo) const
    {
        assert(IsWindow());
        return (0 != SendMessage(EM_GETPUNCTUATION, type, reinterpret_cast<LPARAM>(&puncInfo)));
    }


    // Retrieves the formatting rectangle. The formatting rectangle is the limiting
    // rectangle into which text can be drawn.
    inline void CRichEdit::GetRect(RECT& rc) const
    {
        assert(IsWindow());
        SendMessage(EM_GETRECT, 0, reinterpret_cast<LPARAM>(&rc));
    }


    // Retrieves the type of the next action, if any, in the control's redo queue.
    inline UNDONAMEID CRichEdit::GetRedoName() const
    {
        assert(IsWindow());
        return static_cast<UNDONAMEID>(SendMessage(EM_GETREDONAME, 0, 0));
    }


    // Retrieves the starting and ending character positions of the selection.
    inline void CRichEdit::GetSel(CHARRANGE& range) const
    {
        assert(IsWindow());
        SendMessage(EM_EXGETSEL, 0, reinterpret_cast<LPARAM>(&range));
    }


    // Gets the starting and ending positions of the current selection.
    inline void CRichEdit::GetSel(long& startChar, long& endChar) const
    {
        assert(IsWindow());
        CHARRANGE range;

        SendMessage(EM_EXGETSEL, 0, reinterpret_cast<LPARAM>(&range));
        startChar = range.cpMin;
        endChar   = range.cpMax;
    }


    // Retrieves the character formatting attributes in the current selection.
    inline DWORD CRichEdit::GetSelectionCharFormat(CHARFORMAT& format) const
    {
        assert(IsWindow());
        return static_cast<DWORD>(SendMessage(EM_GETCHARFORMAT, SCF_SELECTION, reinterpret_cast<LPARAM>(&format)));
    }


    // Retrieves the character formatting attributes in the current selection.
    inline DWORD CRichEdit::GetSelectionCharFormat(CHARFORMAT2& format) const
    {
        assert(IsWindow());
        return static_cast<DWORD>(SendMessage(EM_GETCHARFORMAT, SCF_SELECTION, reinterpret_cast<LPARAM>(&format)));
    }


    // Retrieves the type of contents in the current selection.
    inline WORD CRichEdit::GetSelectionType() const
    {
        assert(IsWindow());
        return static_cast<WORD>(SendMessage(EM_SELECTIONTYPE, 0, 0));
    }


    // Gets the text of the current selection.
    inline long CRichEdit::GetSelText(LPSTR pBuf) const
    {
        assert(IsWindow());
        return static_cast<long>(SendMessage(EM_GETSELTEXT, 0, reinterpret_cast<LPARAM>(pBuf)));
    }


    // Gets the text of the current selection.
    inline CString CRichEdit::GetSelText() const
    {
        assert(IsWindow());

        CString str;
        int nBuff = static_cast<int>(SendMessage(EM_GETSELTEXT, 0, 0));
        SendMessage(EM_GETSELTEXT, 0, reinterpret_cast<LPARAM>(str.GetBuffer(nBuff)));
        str.ReleaseBuffer();
        return str;
    }


    // Retrieves the length of the text, in characters. Does not include the terminating null character.
    inline long CRichEdit::GetTextLength() const
    {
        assert(IsWindow());
        return static_cast<long>(SendMessage(WM_GETTEXTLENGTH, 0, 0));
    }


    // Returns the number of TCHARs in the rich edit control, depending on the flags specified.
    inline long CRichEdit::GetTextLengthEx(DWORD flags, UINT codePage /* = -1 */) const
    {
        assert(IsWindow());
        GETTEXTLENGTHEX gtle;
        gtle.flags = flags;
        gtle.codepage = codePage;

        return static_cast<long>(SendMessage(EM_GETTEXTLENGTHEX, reinterpret_cast<WPARAM>(&gtle), 0));
    }


    // Retrieves the current text mode and undo level.
    inline UINT CRichEdit::GetTextMode() const
    {
        assert(IsWindow());
        return static_cast<UINT>(SendMessage(EM_GETTEXTMODE, 0, 0));
    }


    // Retrieves the specified range of text.
    inline CString CRichEdit::GetTextRange(int first, int last) const
    {
        assert(IsWindow());
        CHARRANGE range;
        range.cpMin = first;
        range.cpMax = last;

        CString refString;
        TEXTRANGE tr;
        tr.chrg = range;
        tr.lpstrText = refString.GetBuffer(last - first + 1);
        SendMessage(EM_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(&tr));
        refString.ReleaseBuffer();

        return refString;
    }


    // Retrieves the type of the next undo action, if any.
    inline UNDONAMEID CRichEdit::GetUndoName() const
    {
        assert(IsWindow());
        return static_cast<UNDONAMEID>(SendMessage(EM_GETREDONAME, 0, 0));
    }


    // Shows or hides the current selection.
    inline void CRichEdit::HideSelection(BOOL hide, BOOL isPermanent) const
    {
        assert(IsWindow());
        if (isPermanent)
        {
            SetOptions(ECOOP_SET, ECO_NOHIDESEL);
        }

        SendMessage(EM_HIDESELECTION, hide, 0);
    }


    // Limits the amount of text a user can enter.
    inline void CRichEdit::LimitText(long limit /* = 0 */) const
    {
        assert(IsWindow());
        SendMessage(EM_EXLIMITTEXT, 0, limit);
    }


    // Determines which line contains the given character.
    inline long CRichEdit::LineFromChar(long charIndex) const
    {
        assert(IsWindow());
        return static_cast<long>(SendMessage(EM_EXLINEFROMCHAR, 0, charIndex));
    }


    // Retrieves the character index of a given line.
    // index -   Specifies the zero-based line number. 
    //           A value of –1 specifies the current line number.  
    inline int CRichEdit::LineIndex(int lineIndex /* = -1 */) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(EM_LINEINDEX, lineIndex, 0));
    }


    // Retrieves the length of a given line.
    // charIndex - Specifies the character index of a character in the line whose length is to be retrieved.
    //             This parameter can be –1. In this case, the message returns the number of unselected
    //             characters on lines containing selected characters.
    inline int CRichEdit::LineLength(int charIndex /* = -1 */) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(EM_LINELENGTH, charIndex, 0));
    }


    // Scrolls the text.
    inline void CRichEdit::LineScroll(int lines) const
    {
        assert(IsWindow());
        SendMessage(EM_LINESCROLL, 0, lines);
    }


    // Inserts the contents of the Clipboard.
    inline void CRichEdit::Paste() const
    {
        assert(IsWindow());
        SendMessage(WM_PASTE, 0, 0);
    }


    // Inserts the contents of the Clipboard in the specified data format.
    inline void CRichEdit::PasteSpecial(UINT clipFormat, DWORD aspect /* = 0 */, HMETAFILE hMF /* = 0 */) const
    {
        assert(IsWindow());

        REPASTESPECIAL rps;
        rps.dwAspect = aspect;
        rps.dwParam = reinterpret_cast<DWORD_PTR>(hMF);
        SendMessage(EM_PASTESPECIAL, clipFormat, reinterpret_cast<LPARAM>(&rps));
    }


    // Retrieves the client area coordinates of a specified character.
    inline CPoint CRichEdit::PosFromChar(UINT fromChar) const
    {
        assert(IsWindow());
        DWORD dwPos = static_cast<DWORD>(SendMessage(EM_POSFROMCHAR, 0, fromChar));
        return CPoint(LOWORD(dwPos), HIWORD(dwPos));
    }


    // Redoes the next action in the control's redo queue.
    inline BOOL CRichEdit::Redo() const
    {
        assert(IsWindow());
        return (0 != SendMessage(EM_REDO, 0, 0));
    }


    // Replaces the current selection with specified text.
    inline void CRichEdit::ReplaceSel(LPCTSTR pNewText, BOOL canUndo /* = FALSE */) const
    {
        assert(IsWindow());
        SendMessage(EM_REPLACESEL, canUndo, reinterpret_cast<LPARAM>(pNewText));
    }


    // Forces the sending of a request resize notifications.
    inline void CRichEdit::RequestResize() const
    {
        assert(IsWindow());
        SendMessage(EM_REQUESTRESIZE, 0, 0);
    }


    // Indicates if the auto URL detection is active.
    inline BOOL CRichEdit::SetAutoURLDetect(BOOL enable /* = TRUE */) const
    {
        assert(IsWindow());
        return (0 != SendMessage(EM_AUTOURLDETECT, enable, 0));
    }


    // Sets the background color.
    inline COLORREF CRichEdit::SetBackgroundColor(BOOL isSysColor, COLORREF color) const
    {
        assert(IsWindow());
        return static_cast<COLORREF>(SendMessage(EM_SETBKGNDCOLOR, isSysColor, color));
    }


    // Sets the current default character formatting attributes.
    inline BOOL CRichEdit::SetDefaultCharFormat(CHARFORMAT& format) const
    {
        assert(IsWindow());
        return (0 != SendMessage(EM_SETCHARFORMAT, 0, reinterpret_cast<LPARAM>(&format)));
    }


    // Sets the current default character formatting attributes.
    inline BOOL CRichEdit::SetDefaultCharFormat(CHARFORMAT2& format) const
    {
        assert(IsWindow());
        return (0 != SendMessage(EM_SETCHARFORMAT, 0, reinterpret_cast<LPARAM>(&format)));
    }


    // Sets the event mask. The event mask specifies which notification messages the control sends to its parent window.
    inline DWORD CRichEdit::SetEventMask(DWORD mask) const
    {
        assert(IsWindow());
        return static_cast<DWORD>(SendMessage(EM_SETEVENTMASK, 0, mask));
    }


    // Sets or clears the modification flag. The modification flag indicates whether the text has been modified.
    inline void CRichEdit::SetModify(BOOL isModified /* = TRUE */) const
    {
        assert(IsWindow());
        SendMessage(EM_SETMODIFY, isModified, 0);
    }


    // Sets the IRichEditOleCallback COM object.
    inline BOOL CRichEdit::SetOLECallback(IRichEditOleCallback* pCallback) const
    {
        assert(IsWindow());
        return (0 != SendMessage(EM_SETOLECALLBACK, 0, reinterpret_cast<LPARAM>(pCallback)));
    }


    // Sets the options.
    // Possible options values: ECOOP_SET, ECOOP_OR, ECOOP_AND, ECOOP_XOR
    // Possible flags:   ECO_AUTOWORDSELECTION, ECO_AUTOVSCROLL, ECO_AUTOHSCROLL, ECO_NOHIDESEL
    //                   ECO_READONLY, ECO_WANTRETURN, ECO_SELECTIONBAR, ECO_VERTICAL
    inline void CRichEdit::SetOptions(WORD options, DWORD flags) const
    {
        assert(IsWindow());
        SendMessage(EM_SETOPTIONS, options, flags);
    }


    // Sets the paragraph formatting attributes in the current selection.
    inline BOOL CRichEdit::SetParaFormat(PARAFORMAT& format) const
    {
        assert(IsWindow());
        return (0 != SendMessage(EM_SETPARAFORMAT, 0, reinterpret_cast<LPARAM>(&format)));
    }


    // Sets the paragraph formatting attributes in the current selection.
    inline BOOL CRichEdit::SetParaFormat(PARAFORMAT2& pf) const
    {
        assert(IsWindow());
        return (0 != SendMessage(EM_SETPARAFORMAT, 0, reinterpret_cast<LPARAM>(&pf)));
    }


    // Sets the current punctuation characters for the rich edit control.
    // This is available only in Asian-language versions of the operating system.
    inline BOOL CRichEdit::SetPunctuation(UINT type, const PUNCTUATION& puncInfo) const
    {
        assert(IsWindow());
        return (0 != SendMessage(EM_SETPUNCTUATION, type, reinterpret_cast<LPARAM>(&puncInfo)));
    }


    // Sets or removes the read-only style.
    inline BOOL CRichEdit::SetReadOnly(BOOL isReadOnly /* = TRUE*/) const
    {
        assert(IsWindow());
        return (0 != SendMessage(EM_SETREADONLY, isReadOnly, 0));
    }


    // Sets the formatting rectangle. The formatting rectangle is the limiting rectangle into
    // which the control draws the text.
    inline void CRichEdit::SetRect(const RECT& rc) const
    {
        assert(IsWindow());
        SendMessage(EM_SETRECT, 0, reinterpret_cast<LPARAM>(&rc));
    }


    // Selects a range of characters.
    inline void CRichEdit::SetSel(long startChar, long endChar) const
    {
        assert(IsWindow());

        CHARRANGE cr;
        cr.cpMin = startChar;
        cr.cpMax = endChar;
        SendMessage(EM_EXSETSEL, 0, reinterpret_cast<LPARAM>(&cr));
    }


    // Selects a range of characters.
    inline void CRichEdit::SetSel(CHARRANGE& range) const
    {
        assert(IsWindow());
        SendMessage(EM_EXSETSEL, 0, reinterpret_cast<LPARAM>(&range));
    }


    // Sets the character formatting attributes in the current selection.
    inline BOOL CRichEdit::SetSelectionCharFormat(CHARFORMAT& format) const
    {
        assert(IsWindow());
        return (0 != SendMessage(EM_SETCHARFORMAT, 0, reinterpret_cast<LPARAM>(&format)));
    }


    // Sets the character formatting attributes in the current selection.
    inline BOOL CRichEdit::SetSelectionCharFormat(CHARFORMAT2& cf) const
    {
        assert(IsWindow());
        return (0 != SendMessage(EM_SETCHARFORMAT, 0, reinterpret_cast<LPARAM>(&cf)));
    }


    // Sets the target output device and line width used for "what you see is what you get" (WYSIWYG) formatting.
    inline BOOL CRichEdit::SetTargetDevice(HDC hDC, long lLineWidth) const
    {
        assert(IsWindow());
        return (0 != SendMessage(EM_SETTARGETDEVICE, reinterpret_cast<WPARAM>(hDC), lLineWidth));
    }


    // Sets the target output device and line width used for "what you see is what you get" (WYSIWYG) formatting.
    inline BOOL CRichEdit::SetTargetDevice(CDC& dc, long lLineWidth) const
    {
        assert(IsWindow());
        return (0 != SendMessage(EM_SETTARGETDEVICE, reinterpret_cast<WPARAM>(dc.GetHDC()), lLineWidth));
    }


    // Sets the text mode or undo level of the rich edit control. The message fails if the control contains text.
    inline BOOL CRichEdit::SetTextMode(UINT fMode) const
    {
        assert(IsWindow());
        return (0 != SendMessage(EM_SETTEXTMODE, fMode, 0));
    }


    // Sets the maximum number of actions that can stored in the undo queue.
    // This member function fails if the control contains text.
    inline UINT CRichEdit::SetUndoLimit(UINT nLimit) const
    {
        assert(IsWindow());
        return static_cast<UINT>(SendMessage(EM_SETUNDOLIMIT, nLimit, 0));
    }


    // Sets the character formatting attributes in the current word.
    inline BOOL CRichEdit::SetWordCharFormat(CHARFORMAT& format) const
    {
        assert(IsWindow());
        return (SendMessage(EM_SETCHARFORMAT, 0, reinterpret_cast<LPARAM>(&format)) != 0);
    }


    // Sets the character formatting attributes in the current word.
    inline BOOL CRichEdit::SetWordCharFormat(CHARFORMAT2& cf) const
    {
        assert(IsWindow());
        return (SendMessage(EM_SETCHARFORMAT, 0, reinterpret_cast<LPARAM>(&cf)) != 0);
    }


    // Stops the control from collecting additional typing actions into the current undo action.
    // The control stores the next typing action, if any, into a new action in the undo queue.
    inline void CRichEdit::StopGroupTyping() const
    {
        assert(IsWindow());
        SendMessage(EM_STOPGROUPTYPING, 0, 0);
    }


    // Replaces text with text from the specified input stream.
    inline long CRichEdit::StreamIn(int nFormat, EDITSTREAM& es) const
    {
        assert(IsWindow());
        return static_cast<long>(SendMessage(EM_STREAMIN, nFormat, reinterpret_cast<LPARAM>(&es)));
    }


    // Stores text into an output stream.
    inline long CRichEdit::StreamOut(int nFormat, EDITSTREAM& es) const
    {
        assert(IsWindow());
        return static_cast<long>(SendMessage(EM_STREAMOUT, nFormat, reinterpret_cast<LPARAM>(&es)));
    }


    // Reverses the last editing operation.
    inline BOOL CRichEdit::Undo() const
    {
        assert(IsWindow());
        return (0 != SendMessage(EM_UNDO, 0, 0));
    }

}

#endif   // _WIN32XX_RICHEDIT_H_
