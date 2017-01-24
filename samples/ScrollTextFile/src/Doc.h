/* (02-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                    (Doc.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2017, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

	Contents Description: Declaration of the basic CDoc class for the
	ScrollWin demo application using the Win32++ Windows interface classes,
	Copyright c) 2005-2017 David Nash, under permissions granted therein.

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


	Special Conventions:

	Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Network Planning
		and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef SDI_DOC_H
#define SDI_DOC_H

/*============================================================================*/
	class 
CDoc									/*

*-----------------------------------------------------------------------------*/
{
	public:

		CDoc();
		virtual ~CDoc();
		virtual void    AddRecord(const CString& entry);
		virtual LPCTSTR	GetFilter();
		virtual UINT	GetLength();
		virtual CSize	GetMaxExtent(const CDC& dc);
		virtual CString GetRecord(UINT, UINT left = 0,
				    UINT length = MAX_STRING_SIZE);
		virtual UINT	GetWidth();
		virtual BOOL    IsOpen();
		virtual BOOL    CloseDoc();
		virtual BOOL    OpenDoc(const CString&);
		virtual	void 	Serialize(CArchive& ar);

		  // public data members
		static CString	m_sCompiled_on;  // compilation date, mmm dd yyyy

	private:
		  // private data members
		CFile	m_fDoc_file;     // holds the document name, path, etc.
		BOOL    m_bDoc_is_dirty; // document has been altered since open
		BOOL	m_bDoc_is_open;	 // the document status
		UINT    m_stDoc_length;  // length, in records
		UINT    m_stDoc_width;   // width, in characters
		CString m_open_doc_path; // empty if closed
	     	LPCTSTR m_lpszFile_dlg_filter; // file dialog filter
	     	std::vector<CString> m_doclines; // the lines in the document

};
/*-----------------------------------------------------------------------------*/
#endif //SDI_DOC_H
