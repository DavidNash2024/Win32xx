/* (26-Mar-2025)                                                     (Doc.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description:  Implementation of the CDoc class for the
    CommonDialogs sample application using the Win32++ Windows interface
    classes.

    Programming notes: Documents in this sample program are assumed to derive
    from simple text files of single byte characters from the Windows standard
    UTF8 code page.  For a complete Unicode operation, see the NotePad sample
    function in this distribution.

    Internally, the document contents are managed (as well as viewed) by the
    RichEditView class.

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"


 CDoc::CDoc() : m_isOpen(FALSE), m_data(nullptr)
{
}

// Find the next occurrence of the string returned by the find - replace
// dialog box FR in the document starting with the given character range
// and return the found character range, or an end - of - text range otherwise.
CHARRANGE CDoc::FindNext(const MyFindReplaceDialog& FR, CHARRANGE r)
{
    // Get the find string that does not go out of scope.
    m_findNext   = FR.GetFindString();

    // Set the search parameters.
    BOOL match = FR.MatchCase();
    BOOL whole = FR.MatchWholeWord();
    BOOL down  = FR.SearchDown();
    DWORD dwFlags = (match ? FR_MATCHCASE : 0) |
            (whole ? FR_WHOLEWORD : 0)         |
            (down  ? FR_DOWN : 0);
    r.cpMin = (down ? r.cpMax : r.cpMin);
    r.cpMax = (down ? -1 : 0);

   // Perform the search as per the dwFlags and the FINDTEXEX contents.
    FINDTEXTEX ftx;
    ftx.chrg = r;
    ftx.lpstrText = const_cast<LPWSTR>(m_findNext.c_str());
    GetRichView().FindText(dwFlags, ftx);
    return ftx.chrgText;
}

// Returns the directory of the file associated with this object.
CString CDoc::GetDocDir() const
{
    CFile f; f.SetFilePath(m_docPath);
    return f.GetFileDirectory();
}

// Return a reference to the document data repository object.
CRichEditView& CDoc::GetRichView() const
{
    return m_data->GetRichView();
}

// Indicate the modification state of the document text.
BOOL CDoc::IsDirty() const
{
    return GetRichView().GetModify() && IsOpen();
}

// Returns TRUE if text is selected.
BOOL CDoc::IsSelected() const
{
    CHARRANGE range;
    GetRichView().GetSel(range);
    return m_isOpen && (range.cpMin != range.cpMax);
}

// Returns TRUE if text can be pasted.
 BOOL CDoc::CanPaste() const
{
    return m_isOpen && GetRichView().CanPaste(CF_TEXT);
}

// Open a new document with the given filename; return TRUE if able to do
// so, or FALSE otherwise.
BOOL CDoc::MakeNewDoc(LPCWSTR docPath)
{
    SetDirty(FALSE);
    m_docPath.Empty();
    m_isOpen = FALSE;
    try
    {
        CFile f(docPath, CREATE_NEW);
        f.Close();
    }

    catch (...)
    {
        CString msg = "Could not create document file:\n";
        msg += docPath;
        ::MessageBox(nullptr, msg, L"Information", MB_OK |
            MB_ICONINFORMATION | MB_TASKMODAL);
        return FALSE;
    }

    return TRUE;
}

// Post a message box relating that the current search did not find the
// string sought for.
void CDoc::NotFound(const MyFindReplaceDialog& FR)
{
    BOOL match = FR.MatchCase();
    BOOL whole = FR.MatchWholeWord();
    CString msg;
    LPCWSTR wholeword = (whole ? L"\nas a whole word" : L"");
    LPCWSTR matchcase = (match ? L"\nmatching case" : L"");
    msg.Format(L"'%s'was not found%s%s.", m_findNext.c_str(),
        wholeword, matchcase);
    ::MessageBox(nullptr, msg, L"Information", MB_OK |
        MB_ICONINFORMATION | MB_TASKMODAL);
}

// Perform any cleanup necessary to close the document.
void CDoc::OnCloseDoc()
{
    if (!IsOpen())
        return;

    // Check for unsaved text.
    CString msg;
    msg.Format(L"Save changes to this document?\n    %s",
        m_docPath.c_str());
    if (m_isOpen && IsDirty() && (::MessageBox(nullptr, msg,
        L"Question...", MB_YESNO | MB_ICONQUESTION) == IDYES))
    {
        OnSaveDoc();
        return;
    }

    // Mark the document is not open, with zero length.
    m_isOpen = FALSE;
    m_docPath.Empty();
}

// This method responds to the FINDMSGSTRING registered message sent by
// the find / replace dialog box. This is the callback function response
// method for all user interactions with the dialog boxes.
void CDoc::OnFindReplace(UINT, WPARAM, LPARAM lparam)
{
    MyFindReplaceDialog* fr =
        (MyFindReplaceDialog*)MyFindReplaceDialog::GetNotifier(lparam);

    assert(fr != nullptr);
    if (fr != nullptr)
    {
        if (fr->IsTerminating())
            OnFRTerminating(fr);
        else if (fr->FindNext())
            OnFRFindNext(fr);
        else if (fr->ReplaceCurrent())
            OnFRReplaceCurrent(fr);
        else if (fr->ReplaceAll())
            OnFRReplaceAll(fr);
    }
}

// Find the next occurrence of the string returned by the find dialog box
// in the document.
void CDoc::OnFRFindNext(MyFindReplaceDialog* fr)
{
    // Get current location or selection.
    CHARRANGE r;
    GetRichView().GetSel(r);

    // Find the next occurrence.
    r = FindNext(*fr, r);
    if (r.cpMin >= 0)
    {
        // Select the text and set the focus to show it.
        GetRichView().SetSel(r);
        GetRichView().SetFocus();
    }
    else
        NotFound(*fr);
}

// Replace all occurrences of the string returned by the replace dialog box
// in the document by the string returned by that box.
void CDoc::OnFRReplaceAll(MyFindReplaceDialog* fr)
{
    // Get replacement string that does not go out of scope.
    m_replaceWith = fr->GetReplaceString();

    // Search the entire range, start at character 0.
    CHARRANGE r = { 0, 0 };
    CHARRANGE r0 = r;
    r = FindNext(*fr, r);
    if (r.cpMin < 0)
    {
        NotFound(*fr);
        return;
    }

    // Replace this occurrence, seek the next, and repeat until no more.
    do
    {
        GetRichView().SetSel(r);
        GetRichView().ReplaceSel(m_replaceWith.c_str(), TRUE);

        // Save the last replacement site and reset the search beginning point
        // for the next.
        r0 = r;
        r.cpMin = r.cpMax;
        r = FindNext(*fr, r);
    } while (r.cpMin >= 0);

    // Show the last replacement.
    r0.cpMax = r0.cpMin + m_replaceWith.GetLength();
    GetRichView().SetSel(r0);
    GetRichView().SetFocus();
}

// Replace the next occurrence of the string returned by the replace dialog
// box in the document by the string returned by that box.
void CDoc::OnFRReplaceCurrent(MyFindReplaceDialog* fr)
{
    // Get replacement string that does not go out of scope.
    m_replaceWith = fr->GetReplaceString();

    // Get current location or selection.
    CHARRANGE r;
    GetRichView().GetSel(r);

    // Find the next occurrence.
    r = FindNext(*fr, r);
    if (r.cpMin >= 0)
    {
        // Select the text, make the replacement, and set the focus to show it.
        GetRichView().SetSel(r);
        GetRichView().ReplaceSel(m_replaceWith, TRUE);
        r.cpMax = r.cpMin + m_replaceWith.GetLength();
        GetRichView().SetSel(r);
        GetRichView().SetFocus();
    }
    else
        NotFound(*fr);
}

// Perform any actions necessary to terminate the FindReplaceDialog box.
void CDoc::OnFRTerminating(MyFindReplaceDialog*)
{
}

// Save current values of the document back into the currently named source
// file. Return TRUE if the document was not open or not dirty, or is saved
// properly, or FALSE otherwise.
BOOL CDoc::OnSaveDoc()
{
    assert(m_docPath.IsEmpty());
    if (!IsOpen() || !IsDirty())
        return TRUE;

    try
    {
        CFile f(m_docPath, CREATE_ALWAYS);  // Can throw.

        GetRichView().StreamOutFile(f);
        f.Close();
        m_isOpen = TRUE;
    }

    catch (...)
    {
        CString msg = "Document file did not save.";
        msg += m_docPath;
        ::MessageBox(nullptr, msg, L"Information", MB_OK |
            MB_ICONINFORMATION | MB_TASKMODAL);
        m_isOpen = FALSE;
        m_docPath.Empty();
        return FALSE;
    }

    // The document will not be dirty on exit, whatever its current state.
    SetDirty(FALSE);
    return TRUE;
}

// Open the document from the given path.
BOOL CDoc::OpenDoc(LPCWSTR docPath)
{
    CString msg;
    if (CString(docPath).CompareNoCase(m_docPath) == 0)
    {
        msg.Format(L"Document file\n    '%s'\nis already open.",
            m_docPath.c_str());
        ::MessageBox(nullptr, msg, L"Information", MB_OK |
            MB_ICONINFORMATION | MB_TASKMODAL);

        return TRUE;
    }

    try
    {
        CFile f(docPath, OPEN_EXISTING);  // Can throw
        GetRichView().StreamInFile(f);
        m_docPath = docPath;
        f.Close();
        m_isOpen = TRUE;
    }

    catch (...)
    {
        msg.Format(L"Document file\n    '%s'\ndid not open.", docPath);
        ::MessageBox(nullptr, msg, L"Information", MB_OK |
            MB_ICONINFORMATION | MB_TASKMODAL);
        m_isOpen = FALSE;
        m_docPath.Empty();
        return FALSE;
    }

    // Regardless of whether it opens, it is not dirty.
    SetDirty(FALSE);
    return TRUE;
}

// Register the path to the document data repository.
void CDoc::SetDataPath(CView* path)
{
    m_data = path;
}

// Set the real edit window text modification state to b.
void CDoc::SetDirty(BOOL b)
{
    GetRichView().SetModify(b);
}


