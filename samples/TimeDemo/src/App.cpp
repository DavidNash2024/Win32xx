/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]                  (App.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

	Contents Description:  The WinMain() function and  CApp Class
	implementation for a skeleton SDI application using the Win32++
	Windows interface classes, Copyright (c) 2005-2015 David Nash,
	under permissions granted therein.

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

	Implementation of the CApp class.  Also contains WinMain()

*******************************************************************************/

#include "stdafx.h"
#include "App.h"

/*******************************************************************************

	Static constant and variable initialization			*/

  // About box dialog and  other information
CString CApp::m_sAboutStatement = _T("Hello World!");
const CString CApp::m_sCredits  = _T("CTime, MRU, CArchive, and Scrolling Demo");
const CString CApp::m_sVersion	= _T("Version 1.0 (2015)");
const CString CApp::m_sCompiled_with = _T("(TODO: name your compiler here)");
const CString CApp::m_sCompiled_on = __DATE__;
const CString CApp::m_months    =
		_T("Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec");
  // serialized data file type (here: "time-demo info")
const CString CApp::m_ini_file_extension  = _T(".tdi");

/*============================================================================*/
	int APIENTRY 
WinMain(HINSTANCE, HINSTANCE, LPSTR, int)				/*

	Application entry point. None of the entry parameters are used and
	are therefore omitted above.
*-----------------------------------------------------------------------------*/
{
	  // semaphore name, instances, and  handle: make sure the name is 
	  // unique to this application
	static const CString szSemaphoreName = _T("Unique name: TimeDemo");
	   // number of allowed instances: here, 1 avoids archive file clashes
	static const int nInstances = 1;
	  // set default return value
	int rtn = -1; 
	  // Create and  check the semaphore that limits the number of
	  // simultaneously executing instances of this application
	  // to m_nInstances.
	static	HANDLE 	m_hSemaphore;
        if ((m_hSemaphore = CreateSemaphore(NULL, nInstances, nInstances,
	    szSemaphoreName)) != NULL)
	{
	        if (WaitForSingleObject(m_hSemaphore, 0) == WAIT_TIMEOUT)
	        {
			::MessageBox(NULL,
			    _T("The allowed number of instances of this\n")
			    _T("application are already running."),
			    _T("Stop"), MB_OK | MB_ICONSTOP | MB_TASKMODAL);
		        return 0;
		}
		try
		{
			CApp thisApp;

			  // Run the application
			rtn = thisApp.Run();
		}
		
		catch(const CWinException e)
		{
			CString msg,
				what(e.what());
			msg.Format(_T("%s\n%s,%s"), e.GetErrorString(),
			    what.c_str(), _T("\nWinMain Goodbye..."));
			::MessageBox(NULL, msg.c_str(), _T("Exception"),
			    MB_OK | MB_ICONSTOP | MB_TASKMODAL);
		}
		catch (const CException &e) // catch all other CException events  
		{
			// Display the exception and quit
			MessageBox(NULL, e.GetText(), A2T(e.what()), MB_ICONERROR);

			return -1;
		}
		catch(...)      // catch all other exception events
		{
			::MessageBox(NULL, _T("WinMain Goodbye..."),
			    _T("Error"),
			    MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
		}

		  // release the semaphore
	        ReleaseSemaphore(m_hSemaphore, 1, NULL);
	        CloseHandle(m_hSemaphore);
        }
	return rtn;
}

/*******************************************************************************

	CApp class methods

*=============================================================================*/
	CApp::
CApp()									/*

	Default constructor.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
	CApp::
~CApp()									/*

	Default destructor.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
	BOOL CApp::
InitInstance()								/*

	This method is immediately called from the Run() method above to
	create the frame, perform initialization of the app, and  return
	TRUE on success. Returning FALSE terminates the program.
	
	Here, the About box information, command line arguments, app path,
	app directory, app name, app exe name, archive file name, and  help
	file name are generated and saved as public data members of this object.
*-----------------------------------------------------------------------------*/
{
	  // load the nominal app title
	m_sApp_title.LoadString(IDW_MAIN);
          // extract the app name, directory, and  path names
	TCHAR namebuffer[FILENAME_MAX];
	::GetModuleFileName(NULL, namebuffer, FILENAME_MAX);
	m_sAppPath = namebuffer;
          // the directory ends at the final "\" or "/" (NOTE: the path
	  // separator may be presented as either '/' or '\' by gnu. In
	  // fact, regular execution has '\', while the debugger has '/'.)
	LPCTSTR sPathSep = _T("\\");
        int findloc = m_sAppPath.ReverseFind(sPathSep);
        if (findloc == -1)
        {
        	sPathSep = (LPCTSTR)_T("/");
       		findloc = m_sAppPath.ReverseFind(sPathSep);
		if (findloc == -1)
		{
			::MessageBox(NULL,
			    _T("Path separation character error."),
			    _T("Error"), MB_OK | MB_ICONEXCLAMATION |
			    MB_TASKMODAL);
			return FALSE;
		}
        }
	  // the directory is the string up to this last "\"
	m_sAppDir  = m_sAppPath.Mid(0, findloc);
	  // the app name follows the "\"
	m_sAppName = m_sAppPath.Mid(findloc + 1);
	  // form the archive file name
	findloc = m_sAppName.ReverseFind(_T("."));
	  // the app name, sans type
	m_sExeName = m_sAppName.Mid(0, findloc);
	  // the archive path name
	if (!GetAppDataPath().IsEmpty())
	{
		m_sIniFile = GetAppDataPath() + _T("\\Win32++\\") + m_sExeName + sPathSep + m_sExeName + m_ini_file_extension;

		// create the folder if required
		::CreateDirectory(GetAppDataPath() + _T("\\Win32++"), NULL);
		::CreateDirectory(GetAppDataPath() + _T("\\Win32++\\") + m_sExeName, NULL);
	}
	else
 		m_sIniFile = m_sAppDir + sPathSep + m_sExeName + m_ini_file_extension;


	  // generate the About box static information: first the latest
	  // date one of the main stream files was compiled
	ULONG compiled_on = DatInt(CApp::m_sCompiled_on);
	compiled_on = MAX(compiled_on, DatInt(CDoc::m_sCompiled_on));
	compiled_on = MAX(compiled_on, DatInt(CMainFrame::m_sCompiled_on));
	compiled_on = MAX(compiled_on, DatInt(CView::m_sCompiled_on));
	m_sAboutStatement.Format(_T("%s\n\n(%s.exe)\n%s\ncompiled with ")
	    _T("%s\non %s"), m_sCredits.c_str(), m_sExeName.c_str(),
	    m_sVersion.c_str(), m_sCompiled_with.c_str(),
	    IntDat(compiled_on).c_str());
	  //Create the Frame Window
	if (!m_Frame.Create())
	{	  //End the program if the frame window creation fails
		::MessageBox(NULL, _T("Failed to create Frame window"),
		    _T("Error"), MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
		return FALSE;
	}

	return TRUE;
}

/*=============================================================================*

	Static Members                                                  */

/*============================================================================*/
	ULONG CApp::
DatInt(const CString &date)                                             /*

	Convert the date, of form mmm dd yyyy, to a long integer of the form
	0xyyyymodd, where mmm is character based month, and  mo is 0 (Jan) to
	11 (Dec).
*-----------------------------------------------------------------------------*/
{
	int 	yyyy = _ttoi(date.Mid(7, 4).c_str()),
		dd   = _ttoi(date.Mid(4, 2).c_str()),
		mo   = CApp::m_months.Find(date.Mid(0, 3)) / 4;
	ULONG	ans  = ((yyyy * 100 + mo) * 100) + dd;
	return  ans;
}

/*============================================================================*/
	CString CApp::
IntDat(ULONG hexdate)                                             	/*

	Convert the hex date, of form 0xyyyymodd, to a CString date of the form
	mmm dd yyyy,  where mmm is character based month, and  mo is 0 (Jan) to
	11 (Dec).
*-----------------------------------------------------------------------------*/
{
	UINT	dd   = (hexdate % 100),
		mo   = (hexdate / 100) % 100,
		yyyy = (hexdate / 10000);
	CString ans;
	ans.Format(_T("%s %02d, %u"), CApp::m_months.Mid(4 * mo, 3).c_str(), 
	    dd, yyyy);
	return ans;
}

