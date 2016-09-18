/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                  (Doc.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description:  Implementation of the CDoc class for the
	CommonDialogs SDI sample application using the Win32++ Windows interface
	classes, Copyright (c) 2005-2016 David Nash, under permissions granted
	therein.
	
        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and to the alterations,
	additions, deletions, and other treatments of materials that may have
	been extracted from the cited sources.  Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any person obtaining a copy of this treatment
	of the subject class and any associated documentation composed by
	the author, to utilize this material, free of charge and without
	restriction or limitation, subject to the following conditions:

		The above copyright notice, as well as that of David Nash
		and Win32++, together with the respective permission
		conditions shall be included in all copies or substantial
		portions of this material so copied, modified, merged,
		published, distributed, or otherwise held by others.

	These materials are provided "as is", without warranty of any kind,
	express or implied, including but not limited to: warranties of
	merchantability, fitness for a particular purpose, and non-infringement.
	In no event shall the authors or copyright holders be liable for any
	claim, damages, or other liability, whether in an action of contract,
	tort or otherwise, arising from, out of, or in connection with, these
	materials, the use thereof, or any other other dealings therewith.

	Programming notes: Documents in this sample program are simple text
	files of either ASCII or UNICODE character formats that conform with the
	character mode option set during compilation.  An open document has a
	file path name associated with it, which is maintained in the CString
	m_open_doc_path. The CFile object m_Doc_file is used to create or open
	this file and to read or write the contents of the rich edit view window
	and then to immediately close the CFile object. The document itself
	remains open until OnCloseDoc() is executed. At that point the path name
	is emptied.
	
 	Acknowledgement:
		The author would like to thank and acknowledge the advice,
		critical review, insight, and assistance provided by David Nash
		in the development of this work.

	Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Implementation of the CDoc class

*******************************************************************************/

#include "stdafx.h"	
#include <sys/types.h>
#include <sys/stat.h>

#include "StdApp.h"

/*============================================================================*/
	CDoc::
CDoc() 									/*

	Constructor.
*-----------------------------------------------------------------------------*/
    :	m_Doc_file(INVALID_HANDLE_VALUE)
{
	m_Doc_is_open = FALSE;
	m_Doc_file_ext.Empty();
	m_Doc_file_filter.Empty();
	m_open_doc_path.Empty();
}

/*============================================================================*/
	CDoc::
~CDoc()									/*

	Destructor
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
	CHARRANGE CDoc::
FindNext(const MyFindReplaceDialog& FR, CHARRANGE r)			/*

	Find the next occurrence of the string returned by the find-replace
	dialog box FR in the document starting with the given character range
	and return the found character range, or an end-of-text range otherwise.
*-----------------------------------------------------------------------------*/
{
	  // get find string that does not go out of scope
	m_find_next   = FR.GetFindString();
	  // set the search parmeters
	BOOL	match = FR.MatchCase(),
		whole = FR.MatchWholeWord(),
		down  = FR.SearchDown();
	DWORD dwFlags = (match ? FR_MATCHCASE : 0) |
			(whole ? FR_WHOLEWORD : 0) |
			(down  ? FR_DOWN : 0);
	  // Perform the search per the dwFlags and the FINDTEXEX contents:
	  // search down: end of current selection to end of text, or
	  // search up:   beginning of current selection to beginning of text
	r.cpMin = (down ? r.cpMax : r.cpMin);
	r.cpMax = (down ? -1 : 0);
	  // get current location or selection
	FINDTEXTEX ftx;
	ftx.chrg = r;
	ftx.lpstrText = m_find_next.c_str();
	GetREView().FindText(dwFlags, ftx);
	return ftx.chrgText;
}

/*============================================================================*/
	const CString& CDoc::
GetExt()                                                          	/*

	Get the document file extension; if no extension has been set, use the
	one from the AppGlobal object.
*----------------------------------------------------------------------------*/
{
	if (m_Doc_file_ext.IsEmpty())
		SetExt(theAppGlobal.GetDocExt());

	return m_Doc_file_ext;
}

/*============================================================================*/
	const CString& CDoc::
GetFilter()                                                       	/*

	Get the document file dialog filter; if no filter has been set, use the
	one from the AppGlobal object.
*----------------------------------------------------------------------------*/
{
	if (m_Doc_file_filter.IsEmpty())
		SetFilter(theAppGlobal.GetFileFilter());

	return m_Doc_file_filter;
}

/*============================================================================*/
	CRichEditView& CDoc::
GetREView()                                                       	/*

	Return a reference to the rich edit view object.
*----------------------------------------------------------------------------*/
{
	return GetView().GetREView();
}

/*============================================================================*/
	BOOL CDoc::
IsDirty()                                                               /*

	Indicate the modification state of the text in the rich edit view.
*-----------------------------------------------------------------------------*/
{
	return GetREView().GetModify();
}

/*============================================================================*/
	void CDoc::
NotFound(const MyFindReplaceDialog& FR)					/*

	Post a message box relating that the current search did not find the
	string sought for.
*-----------------------------------------------------------------------------*/
{
	BOOL	match = FR.MatchCase(),
		whole = FR.MatchWholeWord();
	CString msg;
	LPCTSTR wholeword = (whole ? _T("\nas a whole word") : _T("")),
		matchcase = (match ? _T("\nmatching case") : _T(""));
	msg.Format(_T("'%s'was not found%s%s."), m_find_next.c_str(),
	    wholeword, matchcase);
	::MessageBox(NULL, msg, _T("Information"), MB_OK |
	    MB_ICONINFORMATION | MB_TASKMODAL);
}

/*============================================================================*/
	void CDoc::
OnCloseDoc()								/*

	Perform any cleanup necessary to close the document, except for
	serialization chores, which are performed separately in the Serialize()
	member.
*-----------------------------------------------------------------------------*/
{
	  //Check for unsaved text
	CString msg;
	msg.Format(_T("Save changes to this document?\n    %s"),
	    GetFilePath().c_str());
	if (m_Doc_is_open && IsDirty() && (::MessageBox(NULL, msg,
	    _T("Question..."), MB_YESNO | MB_ICONQUESTION) == IDYES))
	{
		OnSaveDoc();
		return;
	}
	  // mark the document is not open, with zero length
	m_Doc_is_open = FALSE;
	m_open_doc_path.Empty();
	GetREView().Clean();
	GetView().NoDocOpen();
	GetFrame().SetWindowTitle(m_open_doc_path);
}

/*============================================================================*/
	void CDoc::
OnCopy()								/*

	Copy the currently selected items in document into the clipboard.
*-----------------------------------------------------------------------------*/
{
	GetREView().Copy();
}

/*============================================================================*/
	void CDoc::
OnCut()									/*

	Cut the selected items from the document into the clipboard.
*-----------------------------------------------------------------------------*/
{
	GetREView().Cut();
}

/*============================================================================*/
	void CDoc::
OnDelete()								/*

	Delete the currently selected items from the document.
*-----------------------------------------------------------------------------*/
{
	GetREView().Clear();
}

/*============================================================================*/
	void CDoc::
OnFindReplace(UINT message, WPARAM wParam, LPARAM lParam)		/*

	This method responds to the FINDMSGSTRING registered message sent by
	the find/replace dialog box. This is the callback function response
	method for all user interactions with the dialog boxes.
*-----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(wParam);

	MyFindReplaceDialog* pFR =
	    (MyFindReplaceDialog*)MyFindReplaceDialog::GetNotifier(lParam);
	assert(pFR != NULL);
	if (pFR->IsTerminating())
	{
		OnFRTerminating(pFR);
	}
	else if (pFR->FindNext())
	{
		OnFRFindNext(pFR);
	}
	else if (pFR->ReplaceCurrent())
	{
		OnFRReplaceCurrent(pFR);
	}
	else if (pFR->ReplaceAll())
	{
		OnFRReplaceAll(pFR);
	}
}

/*============================================================================*/
	void CDoc::
OnFRFindNext(MyFindReplaceDialog* pFR)      				/*

	Find the next occurrence of the string returned by the find dialog box
	in the document.
*-----------------------------------------------------------------------------*/
{
	  // get current location or selection
	CHARRANGE r;
	GetREView().GetSel(r);
	  // find the next occurrence
	r = FindNext(*pFR, r);
	if (r.cpMin >= 0)
	{
		  // select the text and set the focus to show it
		GetREView().SetSel(r);
		GetREView().SetFocus();
	}
	else
		NotFound(*pFR);
}

/*============================================================================*/
	void CDoc::
OnFRReplaceAll(MyFindReplaceDialog* pFR)           			/*

	Replace all occurrences of the string returned by the replace dialog box
	in the document by the string returned by that box.
*-----------------------------------------------------------------------------*/
{
	  // get replacement string that does not go out of scope
	m_replace_with = pFR->GetReplaceString();
	  // search the entire range, start at character 0
	CHARRANGE r  = {0, 0},
		  r0 = r;
	r = FindNext(*pFR, r);
	  // if not found, say so
	if (r.cpMin < 0)
	{
		NotFound(*pFR);
		return;
	}
	  // replace this occurrence, seek the next, and repeat until no more
	do
	{
		GetREView().SetSel(r);
		GetREView().ReplaceSel(m_replace_with.c_str(), TRUE);
		  // save the last replacement site and reset the search
		  // beginning point for the next
		r0 = r;
		r.cpMin = r.cpMax;
		r = FindNext(*pFR, r);
	} while (r.cpMin >= 0);
	  // show the last replacement
	r0.cpMax = r0.cpMin + m_replace_with.GetLength();
	GetREView().SetSel(r0);
	GetREView().SetFocus();
}

/*============================================================================*/
	void CDoc::
OnFRReplaceCurrent(MyFindReplaceDialog* pFR)     			/*

	Replace the next occurrence of the string returned by the replace dialog
	box in the document by the string returned by that box.
*-----------------------------------------------------------------------------*/
{
	  // get replacement string that does not go out of scope
	m_replace_with = pFR->GetReplaceString();
	  // get current location or selection
	CHARRANGE r;
	GetREView().GetSel(r);
	  // find the next occurrence
	r = FindNext(*pFR, r);
	if (r.cpMin >= 0)
	{
		  // select the text, make the replacement, and set the
		  // focus to show it
		GetREView().SetSel(r);
		GetREView().ReplaceSel(m_replace_with, TRUE);
		r.cpMax = r.cpMin + m_replace_with.GetLength();
		GetREView().SetSel(r);
		GetREView().SetFocus();
	}
	else
		NotFound(*pFR);
}

/*============================================================================*/
	void CDoc::
OnFRTerminating(MyFindReplaceDialog* pFR)   				/*

	Perform any actions necessary to terminate the FindReplaceDialog box.
*-----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(pFR);
}

/*============================================================================*/
	BOOL CDoc::
OnNewDoc(const CString& filename)                                       /*

	Open a new document with the given filename; return TRUE if able to do
	so, or FALSE otherwise.
*-----------------------------------------------------------------------------*/
{
	  // if there is a document currently open, save and close it
	if (IsOpen())
		OnCloseDoc();

	  // try to open 
	try
	{
		m_Doc_file.Open(filename, CREATE_NEW);
		GetREView().Clean();
		m_open_doc_path = GetFilePath();
		m_Doc_file.Close();
		m_Doc_is_open = TRUE; //if no throw, the document opened
		GetFrame().AddMRUEntry(m_open_doc_path);
	}
	catch (...) // if there was an error in opening the file
	{
		CString msg = (CString)"Document file did not open." + filename;
		::MessageBox(NULL, msg, _T("Information"), MB_OK |
		    MB_ICONINFORMATION | MB_TASKMODAL);
		m_open_doc_path.Empty();
		m_Doc_is_open = FALSE;
		  // if the_path was in the MRU list, remove it
		GetFrame().RemoveMRUEntry(filename);
		return FALSE;
	}
	SetDirty(FALSE);
	GetFrame().SetWindowTitle(m_open_doc_path);
	return m_Doc_is_open;
}

/*============================================================================*/
	BOOL CDoc::
OnOpenDoc(const CString &file)						/*

	Open the document from the given file. Previous state parameters that
	were serialized in the prior execution will have already been loaded.
	Return TRUE if file is open on return, FALSE if not.
*-----------------------------------------------------------------------------*/
{
	CString msg;
	if (file.CompareNoCase(m_open_doc_path) == 0)
	{
		msg.Format(_T("Document file\n    '%s'\nis already open."),
		    m_open_doc_path.c_str());
		::MessageBox(NULL, msg, _T("Information"), MB_OK |
		    MB_ICONINFORMATION | MB_TASKMODAL);
		  // not deemed a failure, as the file is open, as specified
		return TRUE;
	}
	  // if there is currently a document open, close it
	if (IsOpen())
		OnCloseDoc();

	  // try to open (it should, as we know it exists, but still ...)
	try
	{
		m_Doc_file.Open(file, OPEN_EXISTING);
		  // if there was no throw, the document opened
		GetREView().ReadFile(m_Doc_file);
		m_open_doc_path = GetFilePath();
		m_Doc_file.Close();
		m_Doc_is_open = TRUE;
		GetFrame().AddMRUEntry(file);
	}
	catch (...) // if there was an error in opening the file
	{
		msg.Format(_T("Document file\n    '%s'\ndid not open."),
		    file.c_str());
		::MessageBox(NULL, msg, _T("Information"), MB_OK |
		    MB_ICONINFORMATION | MB_TASKMODAL);
		m_Doc_is_open = FALSE;
		  // if the_path was in the MRU list, remove it
		GetFrame().RemoveMRUEntry(file);
		m_open_doc_path.Empty();
		return FALSE;
	}
	  // regardless of whether it opens, it is not dirty
	SetDirty(FALSE);
	GetFrame().SetWindowTitle(m_open_doc_path);
	return TRUE;
}

/*============================================================================*/
	void CDoc::
OnPageSetup()								/*

	Invoke the page setup dialog box and set printer parameters.
*-----------------------------------------------------------------------------*/
{
	MyPageSetup PSD(PSD_SHOWHELP);
	PSD.SetPSDTitle(_T("Page Parameter Setup"));		
	PSD.DoModal(theApp.GetMainWnd());

	// TODO: Add code here to set up the printer.  Note: control does not
	// return here until after OnOK() or OnCancel() have concluded.
}

/*============================================================================*/
	void CDoc::
OnPaste()								/*

	Paste the text in the clipboard into the document at the current
	location.
*-----------------------------------------------------------------------------*/
{
	GetREView().Paste();
}

/*============================================================================*/
	void CDoc::
OnPrintPreview()							/*

	Invoke the print preview dialog
*-----------------------------------------------------------------------------*/
{
	// TODO: fill in print preview
}

/*============================================================================*/
	void CDoc::
OnRedo()								/*

	Reapply the last editing action that was undone.
*-----------------------------------------------------------------------------*/
{
	GetREView().Redo();
}

/*============================================================================*/
	BOOL CDoc::
OnSaveDoc()								/*

	Save current values of the document back into the currently named
	source file. Return TRUE if the document was not open or not dirty, or
	is saved properly, or FALSE otherwise.
*-----------------------------------------------------------------------------*/
{
	  // if no document is open or, if open, not dirty
	if (!IsOpen() || !IsDirty())
		return TRUE;

	  // make sure the file is ok to save
	if (m_open_doc_path.IsEmpty())
	{
		CString msg = _T("Attempt to save an invalid file.");
		::MessageBox(NULL, msg, _T("Information"),
		    MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
		m_Doc_is_open = FALSE;
		return FALSE;
	}

	try
	{
		  // for the demo, announce the path being opened
		CString msg = (CString)"Saving document file:\n    " +
		    m_open_doc_path;
		::MessageBox(NULL, msg, _T("Information"), MB_OK |
		    MB_ICONINFORMATION | MB_TASKMODAL);
		m_Doc_file.Open(m_open_doc_path, CREATE_ALWAYS);
		  // if there was no throw, the document opened
		GetREView().WriteFile(m_Doc_file);
		m_Doc_file.Close();
		m_Doc_is_open = TRUE;
		GetFrame().AddMRUEntry(m_open_doc_path);
	}
	catch (...) // if there was an error 
	{
		CString msg = (CString)"Document file did not save." +
		     m_open_doc_path;
		::MessageBox(NULL, msg, _T("Information"), MB_OK |
		    MB_ICONINFORMATION | MB_TASKMODAL);
		m_Doc_is_open = FALSE;
		  // if the_path was in the MRU list, remove it
		GetFrame().RemoveMRUEntry(m_open_doc_path);
		m_open_doc_path.Empty();
		return FALSE;
	}
	  // document will not be dirty on exit, whatever its current state
	SetDirty(FALSE);
	return TRUE;
}

/*============================================================================*/
	void CDoc::
OnUndo()								/*

	Undo the last editing action on the document.
*-----------------------------------------------------------------------------*/
{
	GetREView().Undo();
}

/*============================================================================*/
	void CDoc::
Register(CMainFrame* pFm, CView* pVu)					/*

	Register the parent frame and view with this document.
*-----------------------------------------------------------------------------*/
{
	m_pParent = pFm;
	m_pView   = pVu;
}


/*============================================================================*/
        void CDoc::
Serialize(CArchive &ar)                                               	/*

        Called to serialize the document to or deserialize it from the
	archive ar, depending on the sense of IsStoring().  Leaves the
	archive open for for further operations.
*-----------------------------------------------------------------------------*/
{
	// TODO: save and restore document elements
	
	  // perform loading or storing
        if (ar.IsStoring())
        {
                  // each item serialized is written to the archive
                  // file as a char stream of the proper length,
                  // preceded by that length. In some cases, other forms os
		  // data are saved, from which the primary items are then
                  // reconstructed.

	}
        else    // recovering
        {
                  // each item deserialized from the archive is
                  // retrieved by first reading its length and then
                  // loading in that number of bytes into the data
                  // item saved in the archive, as above. Some items require
                  // additional converstion procedures.

	}
}

/*============================================================================*/
	void CDoc::
SetDirty(BOOL b)                                                        /*

	Set the real edit window text modification state to b.
*-----------------------------------------------------------------------------*/
{
	GetREView().SetModify(b);
}


