/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]                  (App.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description:  This file contains the WinMain() function and
	CApp Class implementation for the BasicForm SDI sample application using
	the Win32++ Windows interface classes, Copyright (c) 2005-2016 David
	Nash, under permissions granted therein.

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

	Programming Notes: The architectural members of this program set,
	viz., CApp, CDoc, CMainFrame, and CView, are meant to be edited and
	adapted to conform with the needs of each new application to which they
	are applied. The code provided in this sample is meant to serve as the
	template for applications that require context-sensitive help, full
	color selection of frame, controls, and client area, font selection for
	controls, standard file open and save-as dialogs, and most-recently used
	list (MRU), with persistent data stored in and retrieved from archive
	files.

        The programming standards roughly follow those established by the
	1997-1999 Jet Propulsion Laboratory Deep Space Network Planning and
	Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"

#include <sys\stat.h>
#include <io.h>

#include "App.h"

/*******************************************************************************

	Implementation of the CApp class

********************************************************************************

	Local extern and (static) default constants			*/

  // serialized data file type and help file type
static	const	LPCTSTR	archive_file_type = _T(".arc");
static	const	LPCTSTR	help_file_type    = _T(".chm");

  // semaphore name, instances, and handle: make sure the name
  // is unique to among applications being executed simultaneously
static	const	LPCTSTR	appSemaphoreName = _T("Win32++BasicFormApp");
static  const	int     nInstances = 1; // number of allowed instances
static	HANDLE 	m_hSem;

  // application version and compilation information
CString CApp::m_sAppVersion	= _T("Version 1.0\n");  // app version info
CString CApp::m_sCompiled_with 	= _T("TDM-GCC 4.8.1");  // your compiler here
CString CApp::m_sCompiled_on   	= __DATE__;
  // About box program description
CString CApp::m_sCredits = _T("%s\n\nCopyright (c) %s, Robert C. Tausworthe\n")
				 _T("All Rights Reserved.\n\n")
				 _T("Robert.C.Tausworthe@ieee.org\n\n")
				 _T("%s compiled with %s\n")
				 _T("Win32++ %d.%d.%d on %s.\n");
CString CApp::m_sLatest_compilation = _T("Hello World!");

/*******************************************************************************

	Implementation of the CApp class

*=============================================================================*/
	BOOL CApp::
InitInstance()								/*

	This method is immediately called from the base class (CWinApp) Run()
	method to create the frame, perform initialization of the app, and
	return TRUE on success. Returning FALSE terminates the program.

	Here, the About box information, command line arguments, app path,
	app directory, app name, app exe name, archive file name, and help
	file name are generated and saved as public data members of this object.
*-----------------------------------------------------------------------------*/
{

	  // Here we save the command line and its arguments

	m_sCmdLine = ::GetCommandLine();
	m_argv = GetCommandLineArgs();
	m_argc  = m_argv.size();
          // extract the app name, directory, and path names as TCHAR strings
	m_sAppPath = m_argv[0];
          // the directory ends at the final "\" or "/" (NOTE: the path
	  // separator may be presented as either '/' or '\' by gnu. In
	  // fact, regular execution has '\', while the debugger has '/'.)
	CString sPathSep = _T("\\");
        size_t finalPathSep = m_sAppPath.GetString().find_last_of(sPathSep);
        if (finalPathSep == (size_t)-1)
        {
        	sPathSep = _T("/");
       		finalPathSep = m_sAppPath.GetString().find_last_of(sPathSep);
		if (finalPathSep == (size_t)-1)
		{
			CString msg = _T("Path separation character error.");
			::MessageBox(NULL, msg, _T("Error"), MB_OK |
			    MB_ICONEXCLAMATION | MB_TASKMODAL);
			return FALSE;
		}
        }
	  // the directory is the string up to this last path separator
	m_sAppDir  = m_sAppPath.Mid(0, finalPathSep);
	  // the location of the archive file

	m_sAppName = m_sAppPath.Mid(finalPathSep + 1);
	  // find the app name, sans type
	size_t finalExtSep = m_sAppName.GetString().find_last_of(_T("."));
	m_sExeName = m_sAppName.Mid(0, finalExtSep);

	if (!GetAppDataPath().IsEmpty())
	{
		// Use the AppData folder for the archive and help file
		m_sArcvDir = GetAppDataPath() + _T("\\Win32++\\") + m_sExeName;
		m_sHelpDir = GetAppDataPath() + _T("\\Win32++\\") + m_sExeName;

		// Create the folders if required
		CreateDirectory(GetAppDataPath() + _T("\\Win32++"), NULL);
		CreateDirectory(GetAppDataPath() + _T("\\Win32++\\") + m_sExeName, NULL);
	}
	else
	{
		m_sArcvDir = m_sAppDir;
		m_sHelpDir = m_sAppDir;
	}



 	m_sArcvFile  = m_sArcvDir + _T("\\") + m_sExeName +
	    archive_file_type;
	  // the help file path name
	m_sHelpFile = m_sHelpDir + _T("\\") + m_sExeName + help_file_type;
	  // generate the About box static information
	ULONG compiled_on = DatInt(CApp::m_sCompiled_on); // CApp
	compiled_on = MAX(compiled_on, DatInt(CMainFrame::m_sCompiled_on));
	compiled_on = MAX(compiled_on, DatInt(CDoc::m_sCompiled_on));
	compiled_on = MAX(compiled_on, DatInt(CView::m_sCompiled_on));
	CString date = IntDat(compiled_on);
	m_sLatest_compilation.Format(m_sCredits, m_sExeName.c_str(),
	    date.Right(4).c_str(), m_sAppVersion.c_str(),
	    m_sCompiled_with.c_str(),
	    _WIN32XX_VER / 0x100, (_WIN32XX_VER % 0x100) / 0x10,
	    _WIN32XX_VER % 0x10, date.c_str());

	  //Create the Frame Window
	if (!m_Frame.Create())
	{	  //End the program if the frame window creation fails
		CString msg = _T("Failed to create Frame window");
		::MessageBox(NULL, msg, _T("Error"), MB_OK |
				    MB_ICONEXCLAMATION | MB_TASKMODAL);
		return FALSE;
	}
	  // initialize app-wide printer context to default.
	return TRUE;
}

/*******************************************************************************

	Static Members                                                  */

static const CString months = "Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec";

/*============================================================================*/
	ULONG CApp::
DatInt(const CString &date)                                             /*

	Convert the date, of form mmm dd yyyy, to a long integer of the form
	0xyyyymodd, where mmm is character based month, and mo is 0 (Jan) to
	11 (Dec).
*-----------------------------------------------------------------------------*/
{
	int 	yyyy = _ttoi(date.Mid(7, 4).c_str()),
		dd   = _ttoi(date.Mid(4, 2).c_str()),
		mo   = months.Find(date.Mid(0, 3)) / 4;
	ULONG	ans  = ((yyyy * 100 + mo) * 100) + dd;
	return  ans;
}

/*============================================================================*/
	CString CApp::
IntDat(ULONG hexdate)                                             	/*

	Convert the hex date, of form 0xyyyymodd, to a CString date of the form
	mmm dd yyyy,  where mmm is character based month, and mo is 0 (Jan) to
	11 (Dec).
*-----------------------------------------------------------------------------*/
{
	UINT	dd   = hexdate % 100,
		mo   = (hexdate / 100) % 100,
		yyyy = (hexdate / 10000);
	CString ans;
	ans.Format(_T("%s %02d, %u"),  months.Mid(4 * mo, 3).c_str(), dd, yyyy);
	return ans;
}

/*============================================================================*/
        void CApp::
Serialize(CArchive &ar)                                               	/*

        Called to serialize the application to or from the archive ar, depending
	on the sense of IsStoring().  Leaves the archive open for for further
	operations.
*-----------------------------------------------------------------------------*/
{
	  // perform loading or storing
        if (ar.IsStoring())
        {
                  // each item is written to the archive as a char stream of
		  // the proper length, preceded by that length.

	}
        else    // recovering
        {
                  // each item read from the archive is retrieved by first
		  // reading its byte length and then by loading in that number
		  // of bytes into the location of that item.

	}
}

/*******************************************************************************

	The WinMain() function invoked by the system to launch the application.

*=============================================================================*/
	int APIENTRY
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
    int nCmdShow)							/*

	Application entry point: hInstance is the handle to the current
	instance of the application; hPrevInstance is always 0; lpCmdLine
	is a pointer to a null-terminated string specifying the command line
	for the application, excluding the program name; nCmdShow specifies
	how the window is to be shown (consult WinMain() documentation for
	parameter values).
*-----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	  // set default return value
	int rtn = -1;
	  // Create and check the semaphore that limits the number of
	  // simultaneously executing instances of this application
	  // to m_nInstances.
        m_hSem = CreateSemaphore(NULL, nInstances, nInstances,
	    appSemaphoreName);
        if (WaitForSingleObject(m_hSem, 0) == WAIT_TIMEOUT)
        {
        	::MessageBox(NULL, _T("The allowed number of instances of this\n")
		    _T("application are already running."), _T("Stop"),
		    MB_OK | MB_ICONSTOP | MB_TASKMODAL);
	        CloseHandle(m_hSem);
	        return 0;
	}
	  // declare the CApp object and run the application
	try
	{
		CApp thisApp;
		  // save entry parameters
		thisApp.SetCmdShow(nCmdShow);
		  // Run the application
		rtn = thisApp.Run();
	}
	catch (std::exception &e)  // catch all std::exception events
	{
		  // Process the exception and quit
		CString msg = e.what() + (CString)"\nWinMain Goodbye...";;
		::MessageBox(NULL, msg.c_str(), _T("Standard Exception"),
		    MB_OK | MB_ICONSTOP | MB_TASKMODAL);
	}
	catch(CString s)        // catch CString events
	{
		CString msg = s + (CString)"\nWinMain Goodbye...";
		::MessageBox(NULL, msg.c_str(), _T("Registered Exception"),
		    MB_OK | MB_ICONSTOP | MB_TASKMODAL);
	}
	catch(LPCTSTR s)        // catch C string events
	{
		CString msg = s + (CString)"\nWinMain Goodbye...";
		::MessageBox(NULL, msg.c_str(), _T("Registered Exception"),
		    MB_OK | MB_ICONSTOP | MB_TASKMODAL);
	}
	catch(...)      // catch all other exception events
	{
		CString msg = _T("Unregistered exception event.\n")
		    _T("WinMain Goodbye...");
		::MessageBox(NULL, msg.c_str(), _T("Unknown Exception"),
		    MB_OK | MB_ICONSTOP | MB_TASKMODAL);
	}

	  // release the semaphore
        ReleaseSemaphore(m_hSem, 1, NULL);
        CloseHandle(m_hSem);
	return rtn;
}

