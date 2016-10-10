/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]             (AppHelp.cpp) *
********************************************************************************
|                                                                              |
|                     Copyright (c) 2016, Robert C. Tausworthe                 |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
*==============================================================================*

	Contents: AppHelp class implementation file. This class provides a set
	of help functions whereby topics appearing in a compiled HTML (.chm)
	document pertaining to an  application may be displayed. The class is
	based on the Win32++ WINAPI interface, Copyright (c) 2005-2016 David
	Nash, under permissions granted therein. Further information on the
	usage of this class appears below.

        Caveats: The copyright displayed above extends only to the author's
        treatment of material that may have been extracted from cited sources.
	Unaltered portions of those materials retain their original copyright
	status. The author's additions and modifications may be used or altered
	by others for non-commercial use only without permission.  However,
	citation of the author's work should be included in such usages.

	These materials are provided "as is", without warranty of any kind,
	express or implied, including but not limited to: warranties of
	merchantability, fitness for a particular purpose, and non-infringement.
	In no event shall the authors or copyright holders be liable for any
	claim, damages, or other liability, whether in an action of contract,
	tort or otherwise, arising from, out of, or in connection with, these
	materials, the use thereof, or any other other dealings therewith.

	USAGE NOTES: This class provides access to help information in two
	forms: First, as a modeless dialog bearing whatever information, or
	"credits," the user desires to post there; this dialog is commonly
	called the "About Box", and it is made to appear whenever the system
	posts the IDW_ABOUT message defined in the Win32++ default_resource.h
	file. Typically, an application will invoke this form of help activating
	the Help->About topic on the main menu.

	The second form is the display of topical information held in a compiled
	HTML (.chm) file. Typically an application will invoke this kind of help
	by pressint the F1 key, by selecting a Help->Contents topic on the main
	menu, by clicking the mouse on a Help button on the display of one of
	the CCommonDialog objects, by pressing the Shift+F1 key, or by clicking
	the mouse on a context help button on the toolbar. The latter two cases
	initiate the context help state, signified by the presence of the help
	cursor (?), which extends until the user has clicked the mouse on
	another control. At this point the .chm help file is searched for a
	topic corresponding to the selected control; that topic is then
	displayed, if found.

	The suite of files comprising the suite of help functions is composed of

		AppHelp.h       declaration of the AppHelp class
		AppHelp.cpp     implementation of the AppHelp class
		AppHelpRC.h     resource definitions needed by the class
		AppHelp.rc      the AboutBox template

	It will be necessary to include the AppHelp.h file in the front matter
	of all files accessing the help functions. Typically, this includes the
	mainframe and view classes. The AppHelpRC.h file, which includes the
	numeric definitions for all help-specific resources, including those for
	dialogs derived from CCommonDialog, will need to be included in those
	files.  The AppHelp.rc file must be included among the application
	program resource templates.

	The first thing the user must do to use the AppHelp class is to define
	an object of this class in the application's mainframe class, such as

		AppHelp m_AppHelp;

	The name of the .chm help file and the attribution credits that appear
	on the Help->About menu item are then communicated to the help object
	using the statement

		m_AppHelp.ConnectAppHelp(const CString& chmName,
		    const CString& aboutCredits);

	at an appropriate point in the mainframe class when its arguments have
	been determined.

	Topics to be displayed are declared within the application prior to
	initiation of help requests, typically in the constructor of the main
	frame class. Such declarations take the form

		m_AppHelp.AddHelpTopic(UINT nID, const CString& topic);

	in which the selection of a control having the identifier nID will
	invoke the display of a string topic within the help guide file. A
	number of these, considered to be standard, are declared in the
	AppHelp::DefaultTopics() method, below.
	
	Assistance in peparing the help guide to contain the application's
	topics may be found in the file

		Adding HTML Help to MinGW.html

	which may be accessed at a number of sites using an internet search. The
	programmer should make sure that all controls that should have help
	topics are declared in the resource.h header file.

	A number of help numeric resources are typically defined, such as those
	implicit in the mainframe OnCommand() cases to be added:

	    case IDW_ABOUT:  // invoked by F1 and by the Help->About menu item
	    	return m_AppHelp.OnHelpAbout();

	    case IDC_HELP_COMDLG:  // Handle help requests from common dialogs
	    	return m_AppHelp.OnHelpID(LOWORD(lParam));

	    case IDM_HELP_CONTENT:  // invoked by Help->Content menu item
	    	return m_AppHelp.OnHelpID(nID);

	    case IDM_HELP_CONTEXT: // invoked by Shift+F1 and Help button
		return EngageContextHelp(); // initiates context help mode

	The mainframe's EngageContextHelp() should take the form

		  // if context help is already active, no need to re-engage
		if (m_AppHelp.IsActive())
			return FALSE;

		m_View.SetFocus();
		  // set the cursor(s) in all controls to help
		m_hCursor = ::LoadCursor(NULL, IDC_HELP);
	    	  // set help mode on
	    	m_AppHelp.EngageContextHelp();
		return TRUE;

	Once the context help mode is active, messages initiated by controls are
	viewed as help topic requests. Therefore, methods that process these
	messages must divert those topic requests to the AppHelp class so as to
	be displayed.  Thus the mainframe OnCommand() and WndProc WM_SYSCOMMAND
	processing, as well as the view OnCommand() processing, need to insert
	code to recognize such requests and process them accordingly. Messages
	that do not pass through these more usual message loops, such as those
	handling dialog messages (e.g., IDOK and IDCANCEL), also need to make
	such insertions.

	It is useful for the mainframe thus to implement the following function

		BOOL CMainFrame::DoContextHelp(WPARAM wParam)
		{
			if (!m_AppHelp.OnHelp(wParam))
				return FALSE;

			m_hCursor = ::LoadCursor(NULL, IDC_ARROW);
			Invalidate();
			return TRUE;
		}

	If the OnHelp() method returns TRUE, that means there was a response
	to a help request. Having dispatched this request, the help mode is
	terminated and the cursor restored to its regular frame value.
	Otherwise, the wParam message is not help-related and normal processing
	is allowed to take place.

	At the top of the mainframe's OnCommand() for example,

		if (DoContextHelp(wParam))
		return TRUE;

	and within the mainframe's WndProc WM_SYSCOMMAND
	processing, 		
	
	    case IDM_HELP_ACTIVE:
		return DoContextHelp(wParam);

	At the top of the view's OnCommand()

		if(::SendMessage(m_hParent, IDM_HELP_ACTIVE, wParam, 0))
			return TRUE;

	and, if the view handles IDOK and/or IDCANCEL, or other such dialog
	messages, place at the top of each handler code such as

		if(::SendMessage(m_hParent, IDM_HELP_ACTIVE, wParam, 0))
			return;

	In the above, the m_hParent handle may be set within the view class
	member
	
		CView:: Create(HWND hParent = 0) 
		{
			m_hParent = hParent;
			...
		}
	
	In instances where messages avoid message loops, such in as the IDOK
	case above, but be careful to use a meaningful return value, if one is
	required, so as not to create an unintentioned response. The low-order
	word of wParam should identify the numeric nID of the control.

	Finally, in the WndProc() procedure for the mainframe, field the cursor
	reset and CCommonDialog help messages,

		    case WM_SETCURSOR:
		    {
			SetCursor(m_hCursor);
			return TRUE;
		    }

		    case WM_HELP:
		    {     // Handle the F1 requests for help. The HELPINFO in lParam
			  // is of no use to the current context help class. The
			  // system also sends this message on Shift+F1, so it is
			  // necessary to distinguish which usage is in effect.
			if (!m_AppHelp.IsActive())
				m_AppHelp.OnHelpID(IDM_HELP_CONTENT);
		    	return TRUE;
		    }

	Other members of this class available for use in applications are

		IsActive() // returns TRUE when in context sensitive mode
		OnHelp(const CString &topic) // displays help for string topic

 	Acknowledgement:
		The author would like to thank and acknowledge the advice,
		critical review, insight, and assistance provided by David Nash
		in the development of this work.

	Programming Notes:
               The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	AppHelp Class Implementation

*******************************************************************************/

#include "stdafx.h"
#include <htmlhelp.h>  // load after windows.h to get system #defines

#include "AppHelp.h"
#include "AppHelpRC.h"

/*============================================================================*/
	AppHelp::
AppHelp()                                                           /*

	Constructor: load topics those native to CWinAppEx and AboutBox dialog
	content.
*-----------------------------------------------------------------------------*/
	:   CDialog(IDW_ABOUT)  // the AppHelp class is based on the AboutBox
				// but extends to all context help.
{
	m_bHelpActive = FALSE;
	m_help_table.clear();
	  // load the topics table with
	DefaultTopics();
	  // generate about box credits (PUT YOUR OWN HERE)
	CString date = __DATE__;
	m_sCredits = _T("An Application\n\nCopyright (c) ") + date.Right(4) +
			_T(", Robert C. Tausworthe\n") +
			_T("robert.c.tausworthe@ieee.org\n") +
			_T("All Rights Reserved.\n\n Compiled: ") + date;
}

/*============================================================================*/
	BOOL AppHelp::
AddHelpTopic(UINT nID, const CString& topic) 				/*

	Add the (control-nID, string topic) pair to the help topic table.
	Complain if a duplicate id entered.
*-----------------------------------------------------------------------------*/
{
	UINT size = (UINT)m_help_table.size();
	help_message pair = {nID, topic};

 	for (UINT id = 0; id < size; id++)
	{
		help_message msg = m_help_table[id];
		if (msg.nID == nID)
		{
			CString err;
			err.Format( _T("Duplicate entry in\nAddHelpTopic")
			    _T("(%u, %s)\n"), nID, msg.topic.c_str());
			throw CUserException(err);
		}
	}
	  // no registered help topic found, so enter the pair
	m_help_table.push_back(pair);
	return ((UINT)m_help_table.size() == size + 1);
}

/*============================================================================*/
	void AppHelp::
ConnectAppHelp(const CString& chmName, const CString& aboutCredits) /*

	Inform the context help object of the help file name and the credits
	information string that will appear in the AboutBox, when activated.
*-----------------------------------------------------------------------------*/
{
	m_sContextHelpFile = chmName;
	m_sCredits         = aboutCredits;
}

/*============================================================================*/
	void AppHelp::
DefaultTopics()                                               	/*

	Add topics common to most apps and common dialog help box responses.
	Override this method to provide customized defaults. Call this function
	within the override so as to include these topics.
*-----------------------------------------------------------------------------*/
{
	  // standard frame topics
	AddHelpTopic(IDW_MAIN, /* client area*/	_T("Introduction"));
	AddHelpTopic(SC_MINIMIZE, 		_T("MinimizeWindow"));
	AddHelpTopic(SC_MAXIMIZE, 		_T("MaximizeWindow"));
	AddHelpTopic(SC_RESTORE, 		_T("RestoreWindow"));
	AddHelpTopic(SC_CLOSE, 			_T("ExitTerminateProgram"));
	  // standard menu topics
	AddHelpTopic(IDW_VIEW_TOOLBAR, 		_T("ToolbarTopics"));
	AddHelpTopic(IDW_VIEW_STATUSBAR, 	_T("StatusbarTopics"));
	AddHelpTopic(IDW_ABOUT, 		_T("AboutThisProgram"));
	AddHelpTopic(IDW_FILE_MRU_FILE1, 	_T("MostRecentlyUsedList"));
}

/*============================================================================*/
	BOOL AppHelp::
EngageContextHelp()        						/*

	Set the m_bHelpActive switch to TRUE, which will divert mouse clicks
	and accelerator keys to selection of help topics and instantiation of
	the help documentation for those topics. Save the current keyboard
	focus and then terminate the focus, as a mouse selection of a control
	already having the current focus will not invoke messages to the view's
	OnCommand() handler.
*-----------------------------------------------------------------------------*/
{
	m_bHelpActive = TRUE;
	m_hHadFocus = ::GetFocus(); // save window with current keyboard focus
	::SetFocus(NULL);  // terminate focus
	return TRUE;
}

/*============================================================================*/
	BOOL    AppHelp::
DisplayHelpTopic(const CString& topic)                                  /*

	Display the context help topic, if present, and return TRUE, or show
	the About dialog if there is an error and return FALSE. Override this
	method if the context help is found in a medium other than a .chm file.
*-----------------------------------------------------------------------------*/
{
	CString help_path = m_sContextHelpFile,
	 	topic_url = topic.IsEmpty() ? topic : _T("::/") + topic +
		     _T(".htm"),
		seek_url = help_path + topic_url;
 	  // Be sure to load the htmlhelp.h and htmlhelp.lib precisely as
	  // described in Adding Compiled HTML Help to MinGW Win32 Apps.html.
	  // Add the link to the libhtmlhelp.a to use this function
 	  // (linker parameter -lhtmlhelp in DevC++). Ignore the compiler
	  //  warning message ".drectve `-defaultlib:uuid.lib ' unrecognized".
	HWND hWnd = HtmlHelp(HWND(), seek_url.c_str(), HH_DISPLAY_TOPIC,
	    (DWORD)NULL);
	return (hWnd != 0 ? TRUE : FALSE);
}

/*============================================================================*/
	BOOL 	AppHelp::
OnInitDialog()                                                          /*

*-----------------------------------------------------------------------------*/
{
	  // do the base class initialization first
	CDialog::OnInitDialog();
	SetDlgItemText(IDC_ABOUT_CREDITS, m_sCredits);
	return TRUE;
}

/*============================================================================*/
	BOOL AppHelp::
OnHelp(const CString &topic)                                   		/*

	Display the application guide topic, if present, or show the topic
	in a message box if there is no such topic in the guide. Almost all
	help messages wind up using this method.
*-----------------------------------------------------------------------------*/
{
	BOOL rtn = DisplayHelpTopic(topic);
	m_bHelpActive = FALSE;
	  // terminate help mode
	::SetFocus(m_hHadFocus);  // restore focus to prev window containing it
	if (rtn)
		return TRUE;

	  // if no help guide topic was opened, show that this happened
	CString s,
		fmt = _T("Help topic could not be located:\n\n%s%s");
	CString add = (m_sContextHelpFile.IsEmpty() ?
		   _T("\n\nNo help guide exists.")
		    :
		   _T("\n\nMake sure the .chm file is in the .exe directory."));

	s.Format(fmt, topic.c_str(), add.c_str());
	::MessageBox(NULL, s, _T("Information"), MB_OK | MB_ICONINFORMATION |
	    MB_TASKMODAL);
	return TRUE;
}

/*============================================================================*/
	BOOL AppHelp::
OnHelp(WPARAM wParam) 							/*

	When invoked with context help active, display the application guide
	topic corresponding to the numeric identifier contained in the low-order
	word of wParam, if present in the help table, and if the topic exists
	in the guide. Otherwise show the "feature not described" dialog if there
	is an error. Return TRUE on success or FALSE otherwise. This mode is
	used to display topics invoked by common dialog HELP buttons.
*-----------------------------------------------------------------------------*/
{
	  // make sure context help is active
	if (!IsActive())
		return FALSE;

	UINT nID     = LOWORD(wParam),
	     nNotify = HIWORD(wParam);
	  // if the notification message is just to kill the focus, do not
	  // invoke the help topic for that control
    	if (nNotify == EN_KILLFOCUS)
		return TRUE;

	return OnHelpID(nID);
}

/*============================================================================*/
	BOOL AppHelp::
OnHelpAbout()                                                       	/*

	Display the application's About dialog box, which shows either the
	default credits string, or a credits string supplied by the applicaiton.
	This typically contains an application name, copyright information, and
	date of most recent compilation.
*-----------------------------------------------------------------------------*/
{
	  // Ensure that only one About dialog displays even for multiple
	  // activation commands
	if (!IsWindow())
		Create();  // make the AboutBox modeless

	::SetFocus(m_hHadFocus);  // restore focus to prev window containing it
	return TRUE;
}

/*============================================================================*/
	BOOL AppHelp::
OnHelpID(UINT nID)							/*

	Display the application guide topic corresponding to the numeric
	identifier nID, if present in the help table, and if the topic exists
	in the guide. Otherwise, show the "feature not described" dialog if
	there is an error. Return TRUE on success or FALSE otherwise. Context
	help mode need not be active upon invocation.
*-----------------------------------------------------------------------------*/
{
	if (nID == IDW_ABOUT)
		return OnHelpAbout();

	  // map all MRU file messages to one representative
	if(IDW_FILE_MRU_FILE1 <= (UINT)nID && (UINT)nID <= IDW_FILE_MRU_FILE16)
		nID = (int)IDW_FILE_MRU_FILE1;
	  // search the help table for the nID entry
	UINT nEntries = (UINT)m_help_table.size();
 	for (UINT id = 0; id < nEntries; id++)
	{
		help_message msg = m_help_table[id];
		if (msg.nID == nID)
			return OnHelp(msg.topic);
	}
	  // no registered help topic found, so display default manual topic
	return OnHelp(_T("feature not described"));
}

