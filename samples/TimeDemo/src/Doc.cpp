/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]                  (Doc.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

	Contents Description:  Implementation of the CDoc class for a
	skeleton SDI application using the Win32++ Windows interface classes,
	Copyright (c) 2005-2015 David Nash, under permissions granted therein.

        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and  to the alterations,
	additions, deletions, and  other treatments of materials that may have
	been extracted from the cited sources.  Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any persion obtaining a copy of this treatment
	of the subject class and  any associated documentation composed by
	the author, to utilize this material, free of charge and  without
	restriction  or  limitation, subject to the following conditions:

		The above copyright notice, as well as that of David Nash
		and Win32++, together with the respective permission
		conditions shall be included in all copies  or  substantial
		portions of this material so copied, modified, merged,
		published, distributed,  or  otherwise held by others.

	These materials are provided "as is", without warranty of any kind,
	express  or  implied, including but not limited to: warranties of
	merchantability, fitness for a particular purpose, and non-infringement.
	In no event shall the authors  or  copyright holders be liable for any
	claim, damages,  or  other liability, whether in an action of contract,
	tort  or  otherwise, arising from, out of,  or in connection with, these
	materials, the use thereof,  or  any other other dealings therewith.

	Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Implementation of the CDoc class

	This particular formulation of the document class presumes that the
	document souce is a series of textual lines that are accessed and
	formatted by the methods of this class and passed to the CView object
	for display. In the present usage of this skeleton implementation,
	a document is simulated as the results of computations illustrating
	the capabilities of the CTime class, the numeric version information
	of the Windows system, and the command line parameters.

*******************************************************************************/

#include <io.h>

#include "stdafx.h"
#include "Doc.h"
#include "App.h"
#include "targetver.h"

  // local function for display of boolean value in demo
static CString Truth(bool b){return b ? _T("true") : _T("false");}

/*******************************************************************************

	Static constant initialization					*/

  // this file dialog extensions filter is used by the main frame when getting
  // the name of a document to open
LPCTSTR CDoc::m_file_dlg_filter =
    _T("Time Demo Files (*.tdf)\0")
    _T("*.tdf\0")
    _T("All Files (*.*)\0")
    _T("*.*\0")
    _T("\0");

  // static constant initiallization: latest CDoc.cpp file compilation date
const CString CDoc::m_sCompiled_on = __DATE__;

/*============================================================================*/
	CDoc::
CDoc() 									/*

	Construct the document object and load the document to be initially
	displayed at the start of operations.
*-----------------------------------------------------------------------------*/
{
	  // create the document object, set default values, and set the
	  // initial document state
	m_Doc_path.Empty();
	m_bDoc_is_open  = false;
	m_bDoc_is_dirty = false;
	m_stDoc_width   = 0;
	m_doc_content.clear();
	  // show initial document content for display
	InitialDoc();
}

/*============================================================================*/
	CDoc::
~CDoc()									/*

	Destructor
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
	bool CDoc::
CloseDoc()								/*

	Perform any cleanup necessary to close the document.
*-----------------------------------------------------------------------------*/
{
	bool rtn = true;
	if (m_bDoc_is_open && m_bDoc_is_dirty)
		rtn = SaveDoc();

	m_bDoc_is_open = m_bDoc_is_dirty = false;
	m_Doc_path.Empty();
	m_doc_content.clear();
	TheApp().TheFrame()->SetWindowTitle();
	return rtn;
}

/*============================================================================*/
	int CDoc::
GetDocLength() const							/*

	Return the document length, in records.
*----------------------------------------------------------------------------*/
{
	return (int)m_doc_content.size();
}

/*============================================================================*/
	CString CDoc::
GetDocOpenFileName(const CString &title) const				/*

	Bring up the open file dialog and get the path of the file to open.
	If none is given, return an empty path. Use the title as the dialog
	window title.
*-----------------------------------------------------------------------------*/
{
	// Bring up the dialog, and  open the file
	CFile File;
	CString str = File.OpenFileDialog(0, OFN_HIDEREADONLY |
	    OFN_OVERWRITEPROMPT, title, m_file_dlg_filter);
	if (str.IsEmpty())
		::MessageBox(NULL, _T("No document name was selected.\n")
		    _T("The document cannot be opened."),
		    _T("Information"), MB_OK | MB_ICONINFORMATION |
		    MB_TASKMODAL);
	return str;
}

/*============================================================================*/
	CString CDoc::
GetDocRecord(int rcd, int left /* = 0 */, int length
    /* = -1 */)	const						/*

	Return a CString containing the document rcd record, starting at
	the left position and continuing for length characters.
*----------------------------------------------------------------------------*/
{
	int size = GetDocLength();
	if (size == 0 || rcd >= size || rcd == -1)
		return _T("");
	  // CString Mid doesn't like lengths longer than that of the CString's
	  //  or  zero-length CStrings
	CString s  = m_doc_content[rcd];
	  // if left is beyond the length of s, return blank value
	if (s.GetLength() <= left)
		return _T("");

	  // compute length of text to extract
	length = MIN(length, s.GetLength() - left);
	  // extract length chars after left position using base class
	return s.Mid(left, length);
}

/*============================================================================*/
	CString CDoc::
GetDocSaveFileName(const CString &title) const				/*

	Bring up the file save dialog and get the path of file to save the
	document in. If nont is selected, return an empty path.  Use the title
	as the window title.
*-----------------------------------------------------------------------------*/
{
	CFile file;
	TCHAR extbuff[10];
	CString str = file.SaveFileDialog(0, OFN_HIDEREADONLY |
	    OFN_OVERWRITEPROMPT, title.c_str(), m_file_dlg_filter,
	    extbuff);
	if (str.IsEmpty())
		::MessageBox(NULL, _T("No document name was selected.\n")
		    _T("The current document will not be saved."),
		    _T("Information"), MB_OK | MB_ICONINFORMATION |
		    MB_TASKMODAL);
	return str;
}

/*============================================================================*/
	int CDoc::
GetDocWidth() const							/*

	Return the document width, in characters.
*----------------------------------------------------------------------------*/
{
	return m_stDoc_width;
}

/*============================================================================*/
	void CDoc::
InitialDoc() 								/*

	For this TimeDemo, develop the ad hoc array of strings that contain the
	results of various tests of the CTime class and Win32++ functions.
*-----------------------------------------------------------------------------*/
{
	CString s;
	s = _T("  -------------------------------------------------------");
	PushContent(s);
	s.Format(_T("    CTime functions test results"),
	    8 * sizeof(size_t));
	PushContent(s);
	s = _T("  -------------------------------------------------------");
	PushContent(s);
	PushContent(_T(""));
	CTime t1;
	PushContent(_T("   1. CTime t1;                                  ")
	    + t1.StdFormat());

	CTime t2(t1);
	PushContent(_T("   2. CTime t2(t1);                          UTC ")
	    + t2.FormatGmt(stdTForm));

	CTime t3(0);
	PushContent(_T("   3. CTime t3(0); as stdDate                    ")
	    + t3.FormatGmt(stdTDate));
	PushContent(_T("   4. CTime t3(0); as stdTHMS                    ")
	    + t3.FormatGmt(stdTHMS));

	t3 = CTime::GetCurrentTime();
	PushContent(_T("   5. CTime t3 = GetCurrentTime()                ")
	    + t3.StdFormat());

	CTime t4(2014, 3, 8, 2, 37, 40);
	PushContent(_T("   6. CTime t4(2014, 3, 8, 2, 37, 40);           ")
	    + t4.StdFormat());

	CTime t5(2014, 2, 9, 2, 37, 40);
	PushContent(_T("   7. CTime t5(2014, 2, 9, 2, 37, 40);           ")
	    + t5.StdFormat());

	CTime t6(2014, 3, 9, 3, 37, 40);
	PushContent(_T("   8. CTime t6(2014, 3, 9, 3, 37, 40);           ")
	    + t6.StdFormat());

	CTime t7(2014, 67, 3, 37, 40);
	PushContent(_T("   9. CTime t7(2014, 67, 3, 37, 40);             ")
	    + t7.StdFormat());

	CTime t8(2014, 68, 3, 37, 40);
	PushContent(_T("  10. CTime t8(2014, 68, 3, 37, 40);             ")
	    + t8.StdFormat());

	CString s9(_T("09-Mar-2014 3:37:40"));
	CTime t9(s9);  // a CString conversion
	PushContent(_T("  11. CTime t9(\"09-Mar-2014 3:37:40\")   CString  ")
	    + t9.StdFormat());

	CTime t10(_T("09-Mar-2014 3:37:40"));  // a LPCTTR conversion
	PushContent(_T("  12. CTime t10(\"09-Mar-2014 2:37:40\")  LPCTSTR  ")
	    + t10.StdFormat());

	CTime t11(2014, 3, 6, 2, 3, 37, 40);
	PushContent(_T("  13. CTime t11(2014, 3, 6, 2, 3, 37, 40);       ")
	    + t11.StdFormat());

	CTime t12(2014, 3, 6, 3, 3, 37, 40);
	PushContent(_T("  14. CTime t12(2014, 3, 6, 3, 3, 37, 40);       ")
	    + t12.StdFormat());

	SYSTEMTIME st;
	t12.GetAsSystemTime(st);
	CTime t13(st);
	PushContent(_T("  15. GetAsSystemTime(t12);                      ")
	    + t13.StdFormat());

	FILETIME ft;
	t12.GetAsFileTime(ft);
	CTime t14(ft);
	PushContent(_T("  16. GetAsFileTime(t12);                        ")
	    + t14.StdFormat());

	WORD fatdate, fattime;
	::FileTimeToDosDateTime(&ft, &fatdate, &fattime);
	CTime t15(fatdate, fattime);
	PushContent(_T("  17. t15(fatdate, fattime)                      ")
	    + t15.StdFormat());

	int 	yr = t15.GetYear(),
		mo = t15.GetMonth(),
		da = t15.GetDay(),
		hr = t15.GetHour(),
		mn = t15.GetMinute(),
		sc = t15.GetSecond(),
		dw = t15.GetDayOfWeek(),
		dy = t15.GetDayOfYear();
	CString fmt = _T("%04d-%02d-%02d %02d:%02d:%02d [%03d] (%d) ");
	s.Format(fmt + _T("local"), yr, mo, da, hr, mn, sc, dy, dw);
	PushContent(_T("  18. t15, yr-mo-day hr:min:sec [doy] (wk)       ")
	    + s);

 	yr = t15.GetYear(false);
	mo = t15.GetMonth(false);
	da = t15.GetDay(false);
	hr = t15.GetHour(false);
	mn = t15.GetMinute(false);
	sc = t15.GetSecond(false);
	dw = t15.GetDayOfWeek(false);
	dy = t15.GetDayOfYear(false);
	s.Format(fmt + _T("UTC"), yr, mo, da, hr, mn, sc, dy, dw);
	PushContent("  19. t15, yr-mo-day hr:min:sec [doy] (wk)       "
	    + s);

	CTimeSpan ts = t12 - t1;
	s = ts.Format(_T("%D days, %H:%M:%S"));
	PushContent(_T("  20. t12 - t1 =                                 ")
	    + s);

	s.Format(_T("%ld total hrs, %ld min, %ld sec"),
	    ts.GetTotalHours(), ts.GetTotalMinutes(), ts.GetTotalSeconds());
	PushContent(_T("  21. t12 - t1 =                                 ")
	    + s);

	ts = t1 - t12;
	s = ts.Format(_T("%D days, %H:%M:%S"));
	PushContent(_T("  22. t1 - t12 =                                 ")
	    + s);

	CTime t16 = t12 + ts;
	PushContent(_T("  23. t16 = t12 + (t1 - t12) =  t1               ")
	    + t16.StdFormat());

	t16 -= ts;
	PushContent(_T("  24. t16 -= (t1 - t12) =  t12                   ")
	    + t16.StdFormat());

	t16 += ts;
	PushContent(_T("  25. t16 += (t1 - t12) =  t1                    ")
	    + t16.StdFormat());

	t16 = t1 - ts;
	PushContent(_T("  26. t16 = t1 - (t1 - t12) =  t12               ")
	    + t16.StdFormat());

	ts = -ts;
	s = ts.Format(_T("%D days, %H:%M:%S"));
	PushContent(_T("  27. -(t1 - t12) =                              ")
	    + s);

	PushContent(_T("  28. t8 == t7;                                  ")
	    + Truth(t8 == t7));

	PushContent(_T("  29. t8 == t9;                                  ")
	    + Truth(t8 == t9));

	PushContent(_T("  30. t8 != t7;                                  ")
	    + Truth(t8 != t7));

	PushContent(_T("  31. t8 < t7;                                   ")
	    + Truth(t8 < t7));

	PushContent(_T("  32. t8 > t7;                                   ")
	    + Truth(t8 > t7));

	PushContent(_T(""));
	s.Format(_T("  -------------------------------------------------------"));
	PushContent(s);
	s = _T("    Platform values for this machine");
	PushContent(s);
	s.Format(_T("  -------------------------------------------------------"));
	PushContent(s);
	PushContent(_T(""));
	s.Format(_T("  WINVER        = 0x%x"), WINVER);
	PushContent(s);
	s.Format(_T("  WIN32_WINNT   = 0x%x"), _WIN32_WINNT);
	PushContent(s);
	s.Format(_T("  _WIN32_IE     = 0x%x"), _WIN32_IE);
	PushContent(s);
	s.Format(_T("  NTDDI_VERSION = 0x%x"), NTDDI_VERSION);
	PushContent(s);
	PushContent(_T(""));

	s = ::GetCommandLine();
	s.Format(_T("  Command line: %s"), s.c_str());
	PushContent(s);
	PushContent(_T(""));

	  // Here we process the Win32++ access to command line arguments, and
	  // display them among the other tests in this demo. GetCommandLineArgs
	  // retrieves our command line arguments and stores them in a vector
	  // of CString.
	std::vector<CString> args = GetCommandLineArgs();
	  // The second argument (if any) contains this app file name.
	for (size_t i = 0; i < args.size(); i++)
	{
		s.Format(_T("  argv(%d) = %s"), i, args[i].c_str());
		PushContent(s);
	}

	PushContent(_T(""));
	s.Format(_T("  -------------------------------------------------------"));
	PushContent(s);
	s.Format(_T("    C++ Integer data types on this %d-bit machine"),
	    8 * sizeof(size_t));
	PushContent(s);
	s.Format(_T("  -------------------------------------------------------"));
	PushContent(s);
	PushContent(_T(""));
	s.Format(_T("  char                     %lu bytes"), sizeof(char));
	PushContent(s);
	s.Format(_T("  short                    %lu bytes"), sizeof(short));
	PushContent(s);
	s.Format(_T("  int                      %lu bytes"), sizeof(int));
	PushContent(s);
	s.Format(_T("  long                     %lu bytes"), sizeof(long));
	PushContent(s);
	s.Format(_T("  LONGLONG                 %lu bytes"), sizeof(LONGLONG));
	PushContent(s);
	PushContent(_T(""));
	s.Format(_T("  UINT                     %lu bytes"), sizeof(UINT));
	PushContent(s);
	s.Format(_T("  TCHAR                    %lu bytes"), sizeof(TCHAR));
	PushContent(s);
	s.Format(_T("  WORD                     %lu bytes"), sizeof(WORD));
	PushContent(s);
	s.Format(_T("  DWORD                    %lu bytes"), sizeof(DWORD));
	PushContent(s);
	s.Format(_T("  size_t                   %lu bytes"), sizeof(size_t));
	PushContent(s);
	s.Format(_T("  LPTSTR                   %lu bytes"), sizeof(LPTSTR));
	PushContent(s);
	PushContent(_T(""));
 	  // indicate that the initial document has been opened as a new
	  // document, which has been written into, but as yet has no document
	  // file name
	m_bDoc_is_open = m_bDoc_is_dirty = true;
	m_Doc_path.Empty();
}

/*============================================================================*/
	bool CDoc::
IsDirty() const								/*

	Return true if the document has not been saved in its present
	condition, or false otherwise.
*----------------------------------------------------------------------------*/
{
	return m_bDoc_is_dirty;
}

/*============================================================================*/
	bool CDoc::
IsOpen() const								/*

	Return true if the document has been loaded successfully, or false
	otherwise.
*----------------------------------------------------------------------------*/
{
	return m_bDoc_is_open;
}

/*============================================================================*/
	bool CDoc::
OpenDoc(const CString &doc_file_name)					/*

	Open the document having the given doc_file_name and load its contents
	into the internal CString array.  Return true if the document was
	opened, or false if not.
*-----------------------------------------------------------------------------*/
{
	  // if there is a document already open with this file name,
	  // say so, and return
	if (m_bDoc_is_open && !m_Doc_path.IsEmpty() &&
	    m_Doc_path.CompareNoCase(doc_file_name) == 0)
	{
		CString s;
		s.Format(_T("This file is already open:\n    %s"),
		    doc_file_name.c_str());
		::MessageBox(NULL, s, _T("Error"), MB_OK |
		    MB_ICONEXCLAMATION | MB_TASKMODAL);
		return true;
	}

	  // if the name does not exist, return
	if (doc_file_name.IsEmpty())
		return false;

	  // if there is another document open, close it before proceeding
	if (m_bDoc_is_open)
		CloseDoc();
	try
	{
		CArchive ar(doc_file_name, CArchive::load);
		ar << *this;
	}
	catch (const CWinException &e)  // catch std::exception events
	{	  // Process the exception and  quit
		CString msg,
			what(e.what());
		msg.Format(_T("Error restoring the document.\n%s\n%s"),
		    e.GetErrorString(), what.c_str());
		::MessageBox(NULL, msg.c_str(), _T("Exception"),
		    MB_OK | MB_ICONSTOP | MB_TASKMODAL);
		return false;
	}
	catch(...) // catch everything else
	{
		CString msg = _T("Error restoring the document.\n");
		::MessageBox(NULL, msg.c_str(), _T("Exception"),
		    MB_OK | MB_ICONSTOP | MB_TASKMODAL);
		return false;
	}

	  // register the open document
	m_Doc_path = doc_file_name;
	m_bDoc_is_dirty = false;
	m_bDoc_is_open = true;
	  // show the file name in the window title
	TheApp().TheFrame()->SetWindowTitle(m_Doc_path);
	  // record the path in the MRU list
	TheApp().TheFrame()->TheMRU().AddMRUEntry(m_Doc_path);
	return true;
}

/*============================================================================*/
	void  CDoc::
PushContent(const CString &s)                				/*

	Insert the CString s at the bottom of the document content list  and
	adjust the document maximum width indicator to accommodate s, if
	necessary.
*-----------------------------------------------------------------------------*/
{
	m_doc_content.push_back(s);
	m_stDoc_width = MAX(m_stDoc_width, s.GetLength());
}

/*============================================================================*/
	bool CDoc::
SaveDoc()								/*

	Save current CString array of the document into the currently named
	source file path. Return true if able to do so, or false otherwise.
*-----------------------------------------------------------------------------*/
{
	  // if the current document was created as a new document that has
	  // not yet been saved, give it a name and then save it.
	if (m_bDoc_is_open && m_bDoc_is_dirty && m_Doc_path.IsEmpty())
	{
		m_Doc_path =
		    GetDocSaveFileName(_T("Save the current document as..."));
		  // show the new file name in the window title
		TheApp().TheFrame()->SetWindowTitle(m_Doc_path);
	}
	if (m_Doc_path.IsEmpty())
		return false;

	try
	{
		CArchive ar(m_Doc_path, CArchive::store);
		ar << *this;
	}

	catch (const CWinException &e)  // catch all std::exception events
	{	  // Process the exception and  quit
		CString msg,
			what(e.what());
		msg.Format(_T("Error while saving document:\n%s\n%s"),
		    e.GetErrorString(), what.c_str());
		::MessageBox(NULL, msg.c_str(), _T("Exception"),
		    MB_OK | MB_ICONSTOP | MB_TASKMODAL);
		return false;
	}
	catch(...)
	{
		CString msg = _T("Error while saving document:\n");
		::MessageBox(NULL, msg.c_str(), _T("Exception"),
		    MB_OK | MB_ICONSTOP | MB_TASKMODAL);
		return false;
	}


	  // record the path in the MRU list
	TheApp().TheFrame()->TheMRU().AddMRUEntry(m_Doc_path);
	  // mark the document in memory as having no unsaved changes
	m_bDoc_is_dirty = false;
	return true;
}

/*============================================================================*/
	bool CDoc::
SaveDocAs(void)							/*

	Get a new name for the document file and replace the old one with
	this.  Save the newly named document and retain it as the current
	document in memory. Return true if able to do so, false otherwise.
*-----------------------------------------------------------------------------*/
{
	CString str = GetDocSaveFileName( _T("Save file as..."));
	  // check for user bail-out
	if (str.IsEmpty())
		return false;

	  // register the new name and save the document under its new name
	m_Doc_path = str;
	m_bDoc_is_dirty = true;
	return SaveDoc();
}

/*============================================================================*/
	void CDoc::
Serialize(CArchive &ar)                                               	/*

        Serialize or deserialize the CDoc document from the archive ar,
	depending on the sense of ar.IsStoring().  Throw exceptions during 
	reading or writing if the process fails at any point.  Return true 
	if all goes well, or false if it didn't.
*-----------------------------------------------------------------------------*/
{
        if (ar.IsStoring())
        {	  // storing the document
		UINT n = m_doc_content.size();
		  // record the number of records to save
		ar << n;
		  // save the records
		for (UINT i = 0; i < n; i++)
			ar << m_doc_content[i];
	}
        else // recovering
        {
		  // recover the number of records
		UINT n;
		ar >> n;
		  // read in the individual records
		CString s;
		for (UINT i = 0; i < n; i++)
		{
			ar >> s;
			PushContent(s);
		}
	}
}

