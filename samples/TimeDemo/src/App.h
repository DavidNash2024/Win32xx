/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]                    (App.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

	Contents Description: Declaration of a skeleton Single Document
	Interface (SDI) CApp class, for use with the Win32++ Windows interface
	classes, Copyright (c) 2005-2015 David Nash, under permissions
	granted therein.

        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and  to the alterations,
	additions, deletions, and  other treatments of materials that may have
	been extracted from the cited sources.  Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any persion obtaining a copy of this treatment
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

	Declaration of the CApp class

*******************************************************************************/

#ifndef SDI_APP_H
#define SDI_APP_H

/******************************************************************************/

#include "Doc.h"
#include "View.h"
#include "MainFrm.h"

/*============================================================================*/
	class 
CApp : public CWinApp							/*

*-----------------------------------------------------------------------------*/
{
	public:
		CApp();
		virtual ~CApp();

		virtual BOOL		InitInstance();
			CMainFrame*	TheFrame() {return &m_Frame;}

		  // public data members
                TCHAR   **m_argv;       // list of command line arguments
                int       m_argc;       // number of command line arguments
		CString   m_sAppName, 	// name of app: path minus directory
		          m_sAppDir,    // directory of this app
		          m_sAppPath,   // path to this app
		          m_sExeName,   // name of exe file, without extension
			  m_sIniFile,   // serialization file name
			  m_sCmdLine;   // commmand line, sans app name

		CString   m_sApp_title; // base title of app
		
	     	static const CString m_sCompiled_on;   // compile date
		static const CString m_sCompiled_with; // compiler + system info
		static const CString m_sVersion;       // app version info
		static const CString m_sCredits;       // About box notices

		static CString m_sAboutStatement; // latest compile date
		
	protected:

	private:
		CMainFrame m_Frame; // the mainframe

		  // static methods
		static ULONG   DatInt(const CString &);
		static CString IntDat(ULONG);

		  // static constants
		static const CString m_months;
		static const CString m_ini_file_extension;
};

/*============================================================================*/
	inline
CApp& TheApp()                                                  	/*

	Global function for accesss to this application.
*----------------------------------------------------------------------------*/
{
	return (CApp&)GetApp();
}
/*-----------------------------------------------------------------------------*/
#endif // define SDI_APP_H

