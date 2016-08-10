/* (20-Jul-2016) [Tab/Indent: 8/8][Line/Box: 80/74]            (AppProlog.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description: Implementation of the CAppProlog class, which is
	used by the CApp class in an App/Frame/Doc/View architecture for
	supplying a set of commonly needed items within applications. It is
	based on the Win32++ Windows interface classes, Copyright (c) 2005-2015
	David Nash, under permissions granted therein.

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

	Usage: The declaration of an object of the AppProlog class within the
	CApp class automatically creates a number of program resources of
	possible use in an application. These are retrieved at any time using
	the functions
	
		CString& GetAboutBoxInfo()
		CString& GetAppDir()
		CString& GetAppName()
		CString& GetAppPath()
		CString& GetAppTitle()
		CString& GetArcvDir()
		CString& GetArcvPath()
		CString& GetCompiler()	
		CString& GetCompileDate()
		CString& GetDocExt()
		CString& GetFileFilter()
		CString& GetHelpDir()	
		CString& GetHelpPath()	
		BOOL     HasHelpFile() 
		UINT     GetMaxMRU() 

	Special Conventions:

 	Acknowledgement:
		The author would like to thank and acknowledge the advice,
		critical review, insight, and assistance provided by David Nash
		in the development of this work.

	Programming Notes:
               The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Implementation of the CAppProlog class

*******************************************************************************/

#include "stdafx.h"
#include "default_resource.h"

#include "AppProlog.h"
#include "AppPrologRC.h"

/*******************************************************************************

	CAppProlog class

*=============================================================================*/
	CAppProlog::
CAppProlog()                                                            /*

	Here are initialized public data strings containing the AboutBox
	information, the app path, the app directory, the app name, the
	app exe name, the archive file name, and the help file name.
*-----------------------------------------------------------------------------*/
{
          // extract the app name, directory, and  path names
	::GetModuleFileName(NULL, m_sAppPath.GetBuffer(FILENAME_MAX),
	    FILENAME_MAX);
	m_sAppPath.ReleaseBuffer();
	CFile f; // no file opened here, just using the name parsing parts
	f.SetFilePath(m_sAppPath.c_str());
	m_sAppDir   = f.GetFileDirectory();
	m_sAppTitle = f.GetFileTitle();
	m_sAppName  = f.GetFileNameWOExt();

	  // the help file path name
	m_sHelpDir  = m_sAppDir;
	m_sHelpPath = m_sHelpDir + _T("\\") + m_sAppName +
	    LoadString(IDS_HELP_FILE_EXT);

	  // determine the availability of the help file
	m_bHasHelpFile = (_taccess(m_sHelpPath.c_str(), 0x04) == 0);
	  // convey the a help file name to the AppHelp object (empty if none)
	if (!m_bHasHelpFile)
		m_sHelpPath.Empty();

	  // locate the archive file
	m_sArcvDir = MakeAppDataPath(LoadString(IDS_DATAPATH_SUBDIR) +
	    m_sAppName);

	  // form the archive file path name
 	m_sArcvPath  = m_sArcvDir + _T("\\") + m_sAppName +
	     LoadString(IDS_ARCHIVE_FILE_EXT);

	  // the document default extension
	m_sDocExt     = LoadString(IDS_DOC_DEFAULT_EXT);
	m_sFileFilter = LoadString(IDS_FILE_FILTER);
	
	  // the maximum allowed number of MRU entries
	m_nMaxMRUSlots = _ttoi(LoadString(IDS_MAX_MRU_ENTRIES).c_str());

	  // make Win32++ version string
	UINT ver = _WIN32XX_VER;
	m_sWin32Version.Format(_T("Win32++ Version %d.%d.%d"), ver / 0x100,
	    (ver % 0x100) / 0x10, (ver % 0x10));

	  // generate compiler information for the About box
#ifdef __GNUC__
	m_sCompiler.Format(_T("Gnu C++ %d.%d.%d"), __GNUC__, __GNUC_MINOR__,
	    __GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
	m_sCompiler.Format(_T("MS C++ %d.%d"), _MSC_VER / 100,
	    _MSC_VER % 100);
#else
	m_sCompiler      = _T("(unknown compiler name)");
#endif
	m_sCompiled_on        = __DATE__;

;	m_sAboutBoxInfo.Format(_T("%s\n\n(%s.exe)\n%s\n%s\ncompiled with ")
	    _T("%s on %s"), LoadString(IDW_MAIN).c_str(), m_sAppName.c_str(),
	    LoadString(IDS_APP_VERSION).c_str(), m_sWin32Version.c_str(),
	    m_sCompiler.c_str(), m_sCompiled_on.c_str());
}

/*============================================================================*/
	CAppProlog::
~CAppProlog()                                                            /*

*-----------------------------------------------------------------------------*/
{
}

/*******************************************************************************

	Static functions

*=============================================================================*/
	CString CAppProlog::
MakeAppDataPath(const CString& subpath)					/*

	Return a string consisting of the APPDATA environmental path with the
	given subpath appended.  Create this path if it does not exist. If
	an error is encountered, throw a user exception.
*-----------------------------------------------------------------------------*/
{
	::SetLastError(0);
	CString app_data_path = GetAppDataPath();

	int from, to, next;
	for (from = 0, to = subpath.GetLength(); from < to; from = ++next)
	{
		int 	nextbk  = subpath.Find(_T("\\"), from),
			nextfwd = subpath.Find(_T("/"), from);
		next    = MAX(nextbk, nextfwd);
		if (next < 0)
			next = to;

		CString add = subpath.Mid(from, next - from);
		app_data_path += _T("\\") + add;
		if (!SUCCEEDED(::CreateDirectory(app_data_path, 0)))
		{
			CString msg = app_data_path +
			    _T("\nDirectory creation error.");
			throw CUserException(msg);
		}
	}
	return app_data_path;
}


