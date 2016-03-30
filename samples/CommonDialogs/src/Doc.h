/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]                    (Doc.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description: Declaration of the CDoc class for the BasicForm
	sample  application using the Win32++ Windows interface classes,
	Copyright (c) 2005-2016 David Nash, under permissions granted therein.

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
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Declaration of the CDoc class

*******************************************************************************/

#ifndef SDI_DOC_H
#define SDI_DOC_H

// #include "wxx_commondlg.h"			// dn ...
// #include "wxx_printdialogs.h"		// dn ...
#include "MyFindReplaceDlg.h"
#include <sys/stat.h>                   // dn ...

/*============================================================================*/
	class
CDoc									/*

	This application's document class, a pattern for developing new apps.
*-----------------------------------------------------------------------------*/
{
	public:

		CDoc();
		virtual ~CDoc();
		virtual const CString&	GetExt() const
					    { return m_Doc_file_ext;}
		virtual const CString&	GetFilter() const
					    { return m_Doc_file_filter;}
		virtual size_t  GetLength();
		virtual CString GetRecord(size_t, size_t left = 0,
				    size_t length = (size_t)-1);
		virtual size_t  GetWidth();
		virtual BOOL    IsDirty(){return m_Doc_is_dirty;}
		virtual BOOL    IsOpen(){return m_Doc_is_open;}
		virtual BOOL    OnCloseDoc();
		virtual void    OnCopy();
		virtual void    OnCut();
		virtual void    OnDelete();
		virtual void    OnFileNewDialog();
		virtual void    OnDocOpenDialog();
		virtual void    OnFindDialog();
		virtual void 	OnFindReplace(UINT, WPARAM, LPARAM);
		virtual void 	OnFRFindNext(MyFindReplaceDialog*);
		virtual void 	OnFRReplaceAll(MyFindReplaceDialog*);
		virtual void 	OnFRReplaceCurrent(MyFindReplaceDialog*);
		virtual void 	OnFRTerminating(MyFindReplaceDialog*);
		virtual BOOL    OnNewDoc(const CString&);
		virtual BOOL    OnOpenDoc(const CString &);
		virtual void    OnPaste();
		virtual void    OnPrintDialog();
		virtual void    OnPrintPreview();
		virtual void    OnPageSetup();
		virtual void    OnRedo();
		virtual void    OnReplaceDialog();
		virtual BOOL    OnSaveDoc();
		virtual BOOL    OnSaveDocAs();
		virtual void    OnUndo();
		virtual void    SetExt(const CString& ext)
				    { m_Doc_file_ext = ext;}
		virtual void    SetFilter(const CString &s)
				    { m_Doc_file_filter = s;}

		  // public data members
		static CString	m_sCompiled_on; // compilation date, mmm dd yyyy

	protected:
			BOOL    GetFileStatus(const CString&);
		virtual	void 	Serialize(CArchive &ar);

	private:
		CFile   m_Doc_file;     // holds the document name, path, etc.
		BOOL    m_Doc_is_dirty; // document has been altered since open
		BOOL	m_Doc_is_open;	// the document status
		size_t  m_Doc_length;   // length, in records
		size_t  m_Doc_width;    // length, in records
	     	CString m_Doc_file_ext; // default file extension
	     	CString m_Doc_file_filter; // file dialog filter
		MyFindReplaceDialog m_FindRepDialog;  // find-replace dialog
		struct _stat	    m_Status;         // status information
};
/*-----------------------------------------------------------------------------*/
#endif //SDI_DOC_H
