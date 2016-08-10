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
	one from the AppProlog object.
*----------------------------------------------------------------------------*/
{
	if (m_Doc_file_ext.IsEmpty())
		SetExt(theAppProlog.GetDocExt());

	return m_Doc_file_ext;
}


/*============================================================================*/
	BOOL CDoc::
GetFileStatus(const CString& filename)                                  /*

	Return TRUE if the status information for the specified file is
	successfully obtained; otherwise, FALSE. On success, deposit the status
	of the file in the m_Status structure.
*----------------------------------------------------------------------------*/
{
	return (::_tstat(filename.c_str(), &m_Status) == 0);
}

/*============================================================================*/
	const CString& CDoc::
GetFilter()                                                       	/*

	Get the document file dialog filter; if no filter has been set, use the
	one from the AppProlog object.
*----------------------------------------------------------------------------*/
{
	if (m_Doc_file_filter.IsEmpty())
		SetFilter(theAppProlog.GetFileFilter());

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
	serialization chores, which are performed separately, in the Serialize()
	member.
*-----------------------------------------------------------------------------*/
{
	  // for this demo, just remove an MRU entry, and open the top one,
	  // if any
	CString s = theFrame.AccessMRUEntry(0);
	theFrame.RemoveMRUEntry(s.c_str());
	if (theFrame.GetMRUSize() > 0)
	{
		s = theFrame.AccessMRUEntry(0);
		OnOpenDoc(s);
	}
	theFrame.UpdateToolbarMenuStatus();

	m_Doc_is_open = FALSE;
	m_Doc_length = 0;
	if (m_Doc_file.GetHandle() == INVALID_HANDLE_VALUE)
		return FALSE;

	// TODO: clean up as needed to close the document
	
	return m_Doc_file.Close();
}

/*============================================================================*/
	void CDoc::
OnCopy()								/*

	Copy the currently selected text in document into the clipboard.
*-----------------------------------------------------------------------------*/
{
	// TODO: app dependent function
}

/*============================================================================*/
	void CDoc::
OnCut()									/*

	Cut the selected text from the document into the clipboard.
*-----------------------------------------------------------------------------*/
{

	// TODO: Add code here to implement this member
	
	// for this demo, add 5 strings to the MRU list and open the topmost one
	theFrame.EmptyMRUList();
	theFrame.AddMRUEntry(_T("This is MRU 5"));
	theFrame.AddMRUEntry(_T("This is MRU 4"));
	theFrame.AddMRUEntry(_T("This is MRU 3"));
	theFrame.AddMRUEntry(_T("This is MRU 2"));
	theFrame.AddMRUEntry(_T("This is MRU 1 and it is very, very, very, ")
	    _T("very, very, very, very, very, very, very, very, very, very, ")
	    _T("very, very, very long"));
	OnOpenDoc(theFrame.AccessMRUEntry(0));
 	theFrame.UpdateToolbarMenuStatus();
}

/*============================================================================*/
	void CDoc::
OnDelete()								/*

	Delete the currently selected text from the document.
*-----------------------------------------------------------------------------*/
{
	// TODO: app dependent function
}

/*============================================================================*/
	void CDoc::
OnDocOpenDialog()             						/*

	Display the open file dialog to input the document file name and to
	open the corresponding document if that file exists.
*-----------------------------------------------------------------------------*/
{
	MyFileDialog fd(TRUE, _T(""), _T(""),
	    OFN_HIDEREADONLY |
	    OFN_SHOWHELP |
	    OFN_EXPLORER |
	    OFN_NONETWORKBUTTON |
	    OFN_FILEMUSTEXIST |
	    OFN_PATHMUSTEXIST |
	    OFN_ENABLEHOOK    |
	    OFN_ENABLESIZING,
	    GetFilter());
	fd.SetBoxTitle(_T("Open document file..."));
	fd.SetDefExt(theDoc.GetExt());
	if (fd.DoModal(theApp.GetMainWnd()) == IDOK)
 	{
 		CString fname = fd.GetPathName();
		if (fname.IsEmpty())
		    return;
		    
		  // for the demo, show the file chosen
		CString msg;
		msg.Format(_T("File chosen: '%s'"),
		    fd.GetFileName().c_str());
		::MessageBox(NULL, msg, _T("Information"),
		    MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
		    
		  // open the document based on this name
		OnOpenDoc(fname);
		if (IsOpen())
			theFrame.AddMRUEntry(fname);
	}
	theFrame.UpdateToolbarMenuStatus();
}

/*============================================================================*/
	void CDoc::
OnFileNewDialog()     							/*

	Open a new document based on an input file name
*-----------------------------------------------------------------------------*/
{
	MyFileDialog fd(TRUE, _T(""), _T(""),
	    OFN_HIDEREADONLY |
	    OFN_SHOWHELP |
	    OFN_EXPLORER |
	    OFN_NONETWORKBUTTON |
	    OFN_ENABLESIZING,
	    GetFilter());
	fd.SetBoxTitle(_T("New document file..."));
	fd.SetDefExt(GetExt());
	if (fd.DoModal(theApp.GetMainWnd()) == IDOK)	
 	{
 		CString fname = fd.GetPathName();

		  // for the demo, show the file chosen
		CString msg;
		msg.Format(_T("File chosen: '%s'"), fname.c_str());
		::MessageBox(NULL, msg, _T("Information"),
		    MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);

		OnNewDoc(fname);
		if (IsOpen())
			theFrame.AddMRUEntry(fname);
	}
}

/*============================================================================*/
	void CDoc::
OnFindDialog()								/*

	Initiate the find non-modal dialog box and the messages sent to the
	OnFindReplace() method by the main window message loop.
*-----------------------------------------------------------------------------*/
{
	m_FindRepDialog.SetBoxTitle(_T("Find a string..."));
	m_FindRepDialog.Create(TRUE, _T("Initial Text"), _T(""), FR_DOWN |
	    FR_ENABLEHOOK | FR_SHOWHELP, (HWND)theApp.GetMainWnd());
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
	if (GetFileStatus(filename))
	{
		::MessageBox(NULL, _T("That document file already exists."),
		    _T("Error"), MB_OK | MB_ICONERROR |
		    MB_TASKMODAL);
		return FALSE;
	}
	if (IsOpen())
		OnCloseDoc();

	// TODO: create the document using this filename
	
	m_Doc_is_open = TRUE; // for the demo

	return IsOpen();
}

/*============================================================================*/
	BOOL CDoc::
OnOpenDoc(const CString &file)						/*

	Open the document from the given file. Previous state parameters that
	were serialized in the prior execution will have already been loaded.
*-----------------------------------------------------------------------------*/
{
	if (file.CompareNoCase(m_Doc_file.GetFilePath()) == 0)
	{
		::MessageBox(NULL, _T("That document file is already open."),
		    _T("Information"), MB_OK | MB_ICONINFORMATION |
		    MB_TASKMODAL);
		return FALSE;
	}
	if (IsOpen())
		OnCloseDoc();
	  // TODO: try to open the file and check that it opened

	  // here we just have some dummy code that announces the file name
	  // we are trying to open
	CString msg = (CString)"Trying to open file:\n" + file;
	::MessageBox(NULL, msg, _T("Information"), MB_OK | MB_ICONINFORMATION |
	    MB_TASKMODAL);
	if (file.IsEmpty() || !GetFileStatus(file)) // test for unopen file
	{
		CString s;
		s.Format(_T("Unable to open file\n    %s"), file.c_str());
		::MessageBox(NULL, s, _T("Error"), MB_OK | MB_ICONEXCLAMATION |
		    MB_TASKMODAL);
		m_Doc_is_open = FALSE;
		return FALSE;
	}
	
	m_Doc_is_open = TRUE;
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
	void CDoc::
OnReplaceDialog()							/*

	Invoke the find-replace dialog.  Note: the replace dialog box does not
	have the direction up-down box that the find dialog box has.  This is
	by design.
*-----------------------------------------------------------------------------*/
{
	m_FindRepDialog.SetBoxTitle(_T("Find, then Replace"));
	m_FindRepDialog.Create(FALSE, _T("Initial Text"), _T("Replace Text"),
	    FR_DOWN | FR_SHOWHELP | FR_ENABLEHOOK, (HWND)theApp.GetMainWnd());	
}

/*============================================================================*/
	BOOL CDoc::
OnSaveDoc()								/*

	Save current values of the document back into the currently named
	source file. Return TRUE if able to do so, FALSE otherwise.
*-----------------------------------------------------------------------------*/
{
	// get the file name
	if (!IsOpen() || m_Doc_file.GetFileName().IsEmpty() ||
	    m_Doc_file.GetHandle() == INVALID_HANDLE_VALUE)
		return FALSE;

	// TODO: restore proper file saving function

	m_Doc_is_dirty = FALSE;
	return TRUE;
}

/*============================================================================*/
	BOOL CDoc::
OnSaveDocAs()								/*

	Save the current document into a document based on the file named in
	a file dialog box and make that file the current document. Return TRUE
	if able to do so, FALSE otherwise.
*-----------------------------------------------------------------------------*/
{
	if (!IsOpen())
		return FALSE;

	MyFileDialog fd(FALSE, _T(""), _T(""),
	    OFN_HIDEREADONLY |
	    OFN_OVERWRITEPROMPT |
	    OFN_SHOWHELP |
	    OFN_EXPLORER |
	    OFN_ENABLEHOOK |	
	    OFN_NONETWORKBUTTON,
	    GetFilter());

	fd.SetBoxTitle(_T("Save document file as"));
	if (fd.DoModal(theApp.GetMainWnd()) == IDOK)
 	{	  // save the file
		CString fname = fd.GetPathName();
		  // test whether file name exists
		if (fname.IsEmpty() || !GetFileStatus(fname))
			return FALSE;

		  // if the file name is the same as that opened
		if (fname.CompareNoCase(m_Doc_file.GetFilePath()))
			return OnSaveDoc();

		// TODO: save the document as being based on fname

		  // for the demo, show the file chosen
		CString msg;
		msg.Format(_T("File to save: '%s'."), fname.c_str());
		::MessageBox(NULL, msg, _T("Information"),
		    MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);

		m_Doc_is_dirty = FALSE;
		if (IsOpen())
			theFrame.AddMRUEntry(fname);
		return TRUE;
	}
	return FALSE;
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


