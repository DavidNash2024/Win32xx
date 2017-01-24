/* (02-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                    (App.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2017, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

	Contents Description: Declaration of a basic Single Document Interface
	(SDI) CApp class for the ScrollWin demo application using the Win32++
	Windows interface classes, Copyright c) 2005-2017 David Nash, under
	permissions granted therein.

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

#ifndef SDI_APP_H
#define SDI_APP_H

/*============================================================================*/
	class 
CApp : public CWinApp							/*

*-----------------------------------------------------------------------------*/
{
	public:
		CApp();
		virtual ~CApp();

		virtual BOOL 	InitInstance();
		CString&        GetAboutBoxInfo() { return m_sAboutBoxInfo;}
		CString&        GetArcvPath() { return m_sArcvPath;}
		CMainFrame&	GetFrame()    { return m_Frame;}
		UINT            GetMaxMRUSlots() { return m_nMaxMRUSlots;}
		CMainFrame& 	TheFrame()    { return m_Frame;}

	protected:
		void Serialize(CArchive &ar);

	private:
		  // static methods
		static	CString	MakeAppDataPath(const CString& subpath);

		  // private data
		CString    	m_sArcvPath,  // archive file name
				m_sAboutBoxInfo; // credits
		CMainFrame 	m_Frame;      // the main frame object
		UINT		m_nMaxMRUSlots;
		  // static data
		static ULONG   DatInt(LPCTSTR);
		static CString IntDat(ULONG);
};

/*============================================================================*/
	inline
CApp& TheApp()                                                  	/*

	Handy global function
*----------------------------------------------------------------------------*/
{
	return (CApp&)GetApp();
}


/*-----------------------------------------------------------------------------*/
#endif // define SDI_APP_H

