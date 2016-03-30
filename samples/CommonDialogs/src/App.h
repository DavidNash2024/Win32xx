/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]                    (App.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description: Declaration of a basic Single Document Interface
	(SDI) class for the BasicForm sample program using with the Win32++
	Windows interface classes, Copyright (c) 2005-2015  David Nash, under
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
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Declaration of the CApp class
	
*******************************************************************************/

#ifndef SDI_APP_H
#define SDI_APP_H

#include "Doc.h"
#include "View.h"
#include "MainFrm.h"

/*******************************************************************************

	Intra-Class Communication definitions                           */
	
#define theApp          TheApp()
#define theFrame        theApp.TheFrame()
#define theDoc          theFrame.TheDoc()

/*============================================================================*/
	class 
CApp : public CWinApp							/*

	This application's app class, a pattern for developing new apps.
*-----------------------------------------------------------------------------*/
{
	public:
		CApp(){}
		~CApp(){}

		BOOL 	    InitInstance(); // called from (CWinApp) Run()
		CMainFrame& TheFrame()    { return m_Frame;}
		CString&    GetCmdLine()  {return m_sCmdLine;}
		int	    GetCmdShow()  {return m_nCmdShow;}
		CString&    GetArcvFile() {return m_sArcvFile;}
		CString&    GetHelpFile() {return m_sHelpFile;}
		void	    SetCmdShow(int c) {m_nCmdShow = c;}

		  // TODO: add getters/setters according to application needs
		  
		  // static data members
	     	static CString m_sCompiled_on;	    // compile date, mmm dd yyyy
		static CString m_sCompiled_with;    // compiler and system info
		static CString m_sCredits; 	    // About box notices
		static CString m_sAppVersion; 	    // app version info
		static CString m_sLatest_compilation; // latest compile date

	protected:
		void Serialize(CArchive &ar);

	private:
		  // private data
 		CString    m_sCmdLine,   // commmand line, from WinMain()
			   m_sAppName, 	 // name of app: path minus directory
		           m_sAppDir,    // directory of this app
		           m_sAppPath,   // path to this app
		           m_sExeName,   // name of exe file, without extension
		           m_sHelpDir,   // directory holding the help file
		           m_sHelpFile,  // path to the application's help file
		           m_sArcvDir,   // directory holding the archive file
			   m_sArcvFile;  // initialization archive file name
		int        m_argc,       // number of command line arguments
			   m_nCmdShow;   // WinMain() entry nCmdShow argument
		std::vector<CString> m_argv; // list of command line arguments
		CMainFrame m_Frame;      // the main frame object

		  // static data
		static ULONG   DatInt(const CString &);
		static CString IntDat(ULONG);
};

/*============================================================================*/
	inline
CApp& TheApp()                                                  	/*

	Global access to this application object.
*----------------------------------------------------------------------------*/
{
	return (CApp&)GetApp();
}

/*-----------------------------------------------------------------------------*/
#endif // define SDI_APP_H

