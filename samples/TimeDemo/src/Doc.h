/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]                    (Doc.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

	Contents Description: Declaration of the basic CDoc class for a
	skeleton application using the Win32++ Windows interface classes,
	Copyright (c) 2005-2015 David Nash, under permissions granted therein.

        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and  to the alterations,
	additions, deletions, and  other treatments of materials that may have
	been extracted from the cited sources.  Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any person obtaining a copy of this treatment
	of the subject class and  any associated documentation composed by
	the author, to utilize this material, free of charge and  without
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

#include <vector>

/*============================================================================*/
	class 
CDoc : public CObject							/*
	Application document interface class
*-----------------------------------------------------------------------------*/
{
	public:

		CDoc();
		virtual ~CDoc();

		virtual bool    CloseDoc();
		virtual int	GetDocLength() const;
		virtual	CString GetDocOpenFileName(const CString&) const;
		virtual CString GetDocRecord(int, int left = 0,
				    int length = -1) const;
		virtual	CString GetDocSaveFileName(const CString&) const;
		virtual int	GetDocWidth() const;
		virtual void	InitialDocument();
		virtual	bool	IsDirty() const;
		virtual bool    IsOpen() const;
		virtual bool    OpenDoc(const CString &);
		virtual void	NewDocument();
		virtual void    PushContent(const CString&);
		virtual bool    SaveDoc();
		virtual bool    SaveDocAs(void);
		virtual void	Serialize(CArchive& ar);

	  // public data members

		static const 	CString m_sCompiled_on;  // date, mmm dd yyyy

	protected:
		CString	m_Doc_path;
		
	private:
		bool    m_bDoc_is_dirty; // document has been altered
		bool	m_bDoc_is_open;	 // the document status
		int	m_stDoc_width;   // length, in characters
		std::vector<CString> m_doc_content; // array of document lines

	  // static constants
		static LPCTSTR m_file_dlg_filter; // file dialog filter
};
/*----------------------------------------------------------------------------*/
#endif //SDI_DOC_H
