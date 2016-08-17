/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]                  (Doc.cpp) *
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
	m_Doc_is_dirty = FALSE;
	m_Doc_length = 0;
	m_Doc_width = 0;
	m_Doc_file_ext.Empty();
	m_Doc_file_filter.Empty();
}

/*============================================================================*/
	CDoc::
~CDoc()									/*

	Destructor
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
	size_t CDoc::
GetLength()								/*

	Return the document length, in records.
*----------------------------------------------------------------------------*/
{
	if (!IsOpen())
		m_Doc_length = 0;

	  // TODO: compute the document length
	return m_Doc_length;
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
	CString CDoc::
GetRecord(size_t rcd, size_t left /* = 0 */, size_t length /* = (size_t)-1 */)
									/*
	Return a CString containing the document rcd record, starting at
	the left position and continuing for length characters, for use when the
	document is record-based and records are to be viewed.
*----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(rcd);
	
	CString rtn = _T("");
	if (!IsOpen() || m_Doc_length == 0)
		return rtn;

	// TODO: read the rcd-th record of the document into rtn 

	int count = MAX(MIN(rtn.GetLength() - (int)left, (int)length), 0);
	if (count > 0)
		rtn.Mid(left, count);
	return rtn;
}

/*============================================================================*/
	size_t CDoc::
GetWidth()								/*

	Return the document view width, in characters, for use when the
	document is record-based and records are to be viewed.
*----------------------------------------------------------------------------*/
{
	if (!IsOpen())
		m_Doc_width = 0;
		
	  // TODO: compute document width for view
	  
	return m_Doc_width;
}

/*============================================================================*/
	BOOL CDoc::
OnCloseDoc()								/*

	Perform any cleanup necessary to close the document, except for
	serialization chores, which are performed separately in the Serialize()
	member.
*-----------------------------------------------------------------------------*/
{
	  // save the document, or if the document cannot be saved, just exit
	if (!OnSaveDoc())
		return FALSE;

	// TODO: clean up as needed to close the document
	
	  // mark the document is not open, with zero length
	m_Doc_is_open = FALSE;
	m_Doc_length = 0;
	return m_Doc_file.Close();
}

/*============================================================================*/
	void CDoc::
OnCopy()								/*

	Copy the currently selected items in document into the clipboard.
*-----------------------------------------------------------------------------*/
{
	// TODO: app-dependent function
}

/*============================================================================*/
	void CDoc::
OnCut()									/*

	Cut the selected items from the document into the clipboard.
*-----------------------------------------------------------------------------*/
{
	// TODO: Add app-dependent
}

/*============================================================================*/
	void CDoc::
OnDelete()								/*

	Delete the currently selected items from the document.
*-----------------------------------------------------------------------------*/
{
	// TODO: app-dependent function
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
	CString msg,
		next = pFR->GetFindString(),
		match = pFR->MatchCase() ? _T("yes") : _T("no"),
		whole = pFR->MatchWholeWord() ? _T("yes") : _T("no"),
		updown = pFR->SearchDown() ? _T("down") : _T("up");

	// TODO: enter code to find next occurrence of 'next'
	msg.Format(_T("Find next occurrence of '%s'.\n")
		   _T("Match case? \t\t%s\n")
		   _T("Match whole word? \t%s\n")
		   _T("Search direction: \t%s"),
		   next.c_str(), match.c_str(), whole.c_str(), updown.c_str());
	::MessageBox(NULL, msg, _T("Information"), MB_OK | MB_ICONINFORMATION |
	    MB_TASKMODAL);
}

/*============================================================================*/
	void CDoc::
OnFRReplaceAll(MyFindReplaceDialog* pFR)           			/*

	Replace all occurrences of the string returned by the replace dialog box
	in the document by the string returned by that box.
*-----------------------------------------------------------------------------*/
{
	CString msg,
		next = pFR->GetFindString(),
		with = pFR->GetReplaceString(),
		match = pFR->MatchCase() ? _T("yes") : _T("no"),
		whole = pFR->MatchWholeWord() ? _T("yes") : _T("no"),
		updown = pFR->SearchDown() ? _T("down") : _T("up");

	// TODO: enter code to make replacements
	msg.Format(_T("Find all occurrences of '%s'.\n")
		   _T("replace all occurrences with: '%s'.\n")
		   _T("Match case? \t\t%s\n")
		   _T("Match whole word? \t%s\n")
		   _T("Search direction: \t%s"), next.c_str(), with.c_str(),
		   match.c_str(), whole.c_str(), updown.c_str());
	::MessageBox(NULL, msg, _T("Information"), MB_OK | MB_ICONINFORMATION |
	    MB_TASKMODAL);
}

/*============================================================================*/
	void CDoc::
OnFRReplaceCurrent(MyFindReplaceDialog* pFR)     			/*

	Replace the next occurrence of the string returned by the replace dialog
	box in the document by the string returned by that box.
*-----------------------------------------------------------------------------*/
{
	CString msg,
		next = pFR->GetFindString(),
		with = pFR->GetReplaceString(),
		match = pFR->MatchCase() ? _T("yes") : _T("no"),
		whole = pFR->MatchWholeWord() ? _T("yes") : _T("no"),
		updown = pFR->SearchDown() ? _T("down") : _T("up");
		
	// TODO: enter code to make the replacement
	msg.Format(_T("Find next occurrence of '%s'.\n")
		   _T("replace this occurrences with '%s'.\n")
		   _T("Match case? \t\t%s\n")
		   _T("Match whole word? \t%s\n")
		   _T("Search direction: \t%s"), next.c_str(), with.c_str(),
		   match.c_str(), whole.c_str(), updown.c_str());
	::MessageBox(NULL, msg, _T("Information"), MB_OK | MB_ICONINFORMATION |
	    MB_TASKMODAL);
}

/*============================================================================*/
	void CDoc::
OnFRTerminating(MyFindReplaceDialog* pFR)   				/*

	Perform any actions necessary to terminate the FindReplaceDialog box.
*-----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(pFR);

	::MessageBox(NULL, _T("Terminating find/replace."),
	    _T("Information"), MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
}

/*============================================================================*/
	BOOL CDoc::
OnNewDoc(const CString& filename)                                       /*

	Open a new document based on the given filename; return TRUE if able
	to do so, or FALSE otherwise.
*-----------------------------------------------------------------------------*/
{
//	UNREFERENCED_PARAMETER(filename); // this fails on const parameters
	  // dummy for above: TODO: remove when filename is used elsewhere
	{size_t i_07_04_1776 = sizeof(filename); i_07_04_1776 = i_07_04_1776;}

	  // if there is a document currently open, save and close it
	if (IsOpen())
		OnCloseDoc();

	// TODO: create the document using this filename
	
	m_Doc_is_open = TRUE; // for the demo
	m_Doc_is_dirty = FALSE;
	return IsOpen();
}

/*============================================================================*/
	BOOL CDoc::
OnOpenDoc(const CString &file)						/*

	Open the document from the given file. Previous state parameters that
	were serialized in the prior execution will have already been loaded.
	Return TRUE if file is open on return, FALSE if not.
*-----------------------------------------------------------------------------*/
{
	CString current_path = m_Doc_file.GetFilePath(),
		msg;
	if (file.CompareNoCase(current_path) == 0)
	{
		msg.Format(_T("Document file\n    '%s'\nis already open."),
		    current_path.c_str());
		::MessageBox(NULL, msg, _T("Information"), MB_OK |
		    MB_ICONINFORMATION | MB_TASKMODAL);
		  // not deemed a failure, as the file is open, as specified
		return TRUE;
	}
	  // if there is currently a document open, close it
	if (IsOpen())
		OnCloseDoc();

	  // regardless of whether it opens, it is not dirty
	m_Doc_is_dirty = FALSE;
	  // try to open (it should, as we know it exists, but still ...)
	try
	{
		  // for the demo, announce the path being opened
		msg = (CString)"Now opening document file:\n    " + file;
		::MessageBox(NULL, msg, _T("Information"), MB_OK |
		    MB_ICONINFORMATION | MB_TASKMODAL);
		m_Doc_file.Open(file, OPEN_EXISTING);
		  // if there was no throw, the document opened
		m_Doc_is_open = TRUE;
		  // for the demo, annouce success
		msg = (CString)"File opened successfully:\n    " + file;
		::MessageBox(NULL, msg, _T("Information"), MB_OK | MB_ICONINFORMATION |
		    MB_TASKMODAL);
	}
	catch (...)
	{
		msg.Format(_T("Document file\n    '%s'\ndid not open."),
		    file.c_str());
		::MessageBox(NULL, msg, _T("Information"), MB_OK |
		    MB_ICONINFORMATION | MB_TASKMODAL);
		m_Doc_is_open = FALSE;
		return FALSE;
	}
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
	// TODO: app dependent function
}

/*============================================================================*/
	void CDoc::
OnPrintDialog()								/*

	Invoke the CPrintDialog to get printing parameters and print the
	document.
*-----------------------------------------------------------------------------*/
{
	  // Choose the printer and printing parameters.
	MyPrinter PD(PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_SHOWHELP);
	PRINTDLG pd = PD.GetParameters();
 	pd.nFromPage = 1;
	pd.nToPage = 100;
	pd.nMinPage = 1;
	pd.nMaxPage = 1000;
	PD.SetPDTitle(_T("Choose Print Parameters"));
	PD.SetParameters(pd);			
	PD.DoModal(theApp.GetMainWnd());	

	// TODO: Add code here to print the document.  Note: control does not
	// return here until after OnOK() or OnCancel() have concluded.
}

/*============================================================================*/
	void CDoc::
OnPrintPreview()							/*

	Invoke the print preview dialog
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
	void CDoc::
OnRedo()								/*

	Reapply the last editing action that was undone.
*-----------------------------------------------------------------------------*/
{
	// TODO: app dependent function
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
	if (!IsOpen() || !m_Doc_is_dirty)
		return TRUE;

	  // document will not be dirty on exit, whatever its current state
	m_Doc_is_dirty = FALSE;

	  // make sure the file is ok to save
	if (m_Doc_file.GetFileName().IsEmpty() ||
	    m_Doc_file.GetHandle() == INVALID_HANDLE_VALUE)
	{
		CString msg = _T("Attempt to save an invalid file.");
		::MessageBox(NULL, msg, _T("Information"),
		    MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
		m_Doc_is_open = FALSE;
		m_Doc_length = 0;
		return FALSE;
	}

	// TODO: save the document into its file

	return TRUE;
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
OnUndo()								/*

	Undo the last editing action on the document.
*-----------------------------------------------------------------------------*/
{
	// TODO: app dependent function
}


