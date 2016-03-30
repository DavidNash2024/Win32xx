/* (15-Sep-2015) [Tab/Indent: 8/8][Line/Box: 80/74]          (ContextHelp.cpp) *
********************************************************************************
|                                                                              |
|                     Copyright (c) 2016, Robert C. Tausworthe                 |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
*==============================================================================*

	Contents: Generic ContextHelp class implementation file. This class
	provides the means for accessing help topics contained in compiled HTML
	(.chm) documents to applications that use the Win32++ WINAPI interface,
	Copyright (c) 2005-2016 David Nash, under permissions granted therein.
	Further information on the usage of this class appears below.

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

	Usage Notes: This class provides access to help information held in a
	compiled HTML file. Typically, an application will invoke context help
	using the F1 key or via a Context Help button on the toolbar with
	message handlers in the main frame's OnCommand() function, such as

		OnCommand() additions:

		    case IDW_ABOUT:
		    	return m_ContextHelp.OnHelpAbout();

		    case IDM_HELP_CONTEXT:
			EngageContextHelp();
			return TRUE;

		Additionally, if common dialogs are used in the application,
		then a handler for these is needed:

		      // Handle all of the help request messages from
		      // common dialogs.
		    case IDC_HELP_COMDLG:
		    	EngageContextHelp();
		    	return OnContextHelp((WPARAM)lParam);

	It will also be necessary to insert handlers in the main frame's
	message loop to set the cursor and to respond to the WM_HELP message:

		WndProc() alterations:

		    case WM_SETCURSOR:
		    {
			::SetCursor(m_hCursor);
			return TRUE;
		    }

		    case WM_HELP:
		    {     // Handle the F1 requests for help. The HELPINFO in
		    	  // lParam is of no use to the current context help class.
			m_ContextHelp.OnHelpID(IDM_HELP_CONTENT, nMaxMRUSlots);
		    	return TRUE;
		    }

	In the above, a ContextHelp object, here designated as m_ContextHelp
	for illustrative purposes, has been used to direct help requests to
	the ContextHelp class.  The application will also need a set of
	functions similar to those listed and described below:

		Frame class addtions:

		RegisterHelpParameters(const CString& chmName,
		    const CString& aboutCredits):

		    The ContextHelp class assumes that the body of context help
		    information is held in a compiled HTML document having
		    extension ".chm" and residing in a path communicated to the
		    class via the m_ContextHelp.SetHelpFile() method.
		    Information appearing within the application's AboutBox is
		    deposited using the m_ContextHelp.SetCredits() method.
		    These are assumed to have been loaded prior to seeking help
		    topics in the file or calling for the About information.
		    This procedure may be performed after the frame has been
		    created (e.g., in the frame's OnCreate() method, or in the
		    view's OnInitDialog() member).

		SetContextHelpMessages():

		    Use the m_ContextHelp.AddHelpTopic() method to register all
		    the {nID, _T("topic")} help_message pairs for controls
		    within the main frame having help topics in the .chm file.

		EngageContextHelp():

		    Invoke m_ContextHelp.InitiateContextHelp() to enter help
		    mode, set a declared HCURSOR m_hCursor to the help cursor,
		    and invoke m_ContextHelp.InitiateContextHelp().

		OnContextHelp(WPARAM wParam):

		    Invoke this method at the top of all message loops (e.g.,
		    OnCommand() and WndProc()) in which any one of the control
		    numeric identifiers appears in a help_message appended by a
		    AddHelpTopic() statement in the SetContextHelpMessages()
		    function above. If this method returns TRUE, then exit the
		    message loop with returned value TRUE. Otherwise, let the
		    message loop perform as intended in the application. In
		    other instances where messages avoid message loops, such as
		    the IDOK identifier, invoke this method in the same way (but
		    not returning a TRUE value if the method is not BOOL). The
		    low-order word of wParam identifies the numeric nID of the
		    control.

		    This function should check that help mode is active and,
		    if so,  should call m_ContextHelp.OnHelp(wParam, maxMRU) to
		    display the help topic and terminate the help mode,
		    whereupon it should then restore the cursor to its normal
		    shape, invalidate the frame for redrawing, and return TRUE.
		    Otherwise it should return FALSE if help mode is not active.

 	Programming Notes:
               The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	ContextHelp Class Implementation

*******************************************************************************/

#include "stdafx.h"
// #include "wxx_frame.h"			// dn ...
#ifndef __BORLANDC__    // not supported by Borland v5.5 compiler
#include <htmlhelp.h>   // load after wxx_ to get system #defines
#endif // __BORLANDC__

#include "ContextHelp.h"
// #include "ContextHelpRC.h"       // dn ...
#include "resource.h"               // dn ...

/*============================================================================*/
	ContextHelp::
ContextHelp()                                                           /*

	Constructor: add topics common to most apps and common dialog help
	box responses.
*-----------------------------------------------------------------------------*/
	:   CDialog(IDW_ABOUT)
{
	m_bHelpActive = FALSE;
	m_help_table.clear();

	DefaultTopics();

	CString date = __DATE__;
	m_sCredits = _T("An Application\n\nCopyright (c) ") + date.Right(4) +
			_T(", Robert C. Tausworthe\n") +
			_T("robert.c.tausworthe@ieee.org\n") +
			_T("All Rights Reserved.\n\n Compiled: ") + date;
}

/*============================================================================*/
	BOOL ContextHelp::
AddHelpTopic(UINT nID, const CString& topic) 				/*

	Add the (control-nID, string topic) pair to the help topic table.
*-----------------------------------------------------------------------------*/
{
	UINT size = m_help_table.size();
	help_message pair;
	pair.nID = nID;				// dn ...
	pair.topic = topic;			// dn ...
	m_help_table.push_back(pair);
	return (UINT)m_help_table.size() == size + 1;
}

/*============================================================================*/
	void ContextHelp::
DefaultTopics(void)                                               	/*

	Add topics common to most apps and common dialog help box responses.
	Override this method to provide customized defaults.
*-----------------------------------------------------------------------------*/
{
	  // load frame topics
	AddHelpTopic(IDW_MAIN, /* client area*/	_T("Introduction"));
	AddHelpTopic(SC_MINIMIZE, 		_T("MinimizeWindow"));
	AddHelpTopic(SC_MAXIMIZE, 		_T("MaximizeWindow"));
	AddHelpTopic(SC_RESTORE, 		_T("RestoreWindow"));
	AddHelpTopic(SC_CLOSE, 			_T("TerminateFunction"));
	  // standard menu topics
	AddHelpTopic(IDW_VIEW_TOOLBAR, 		_T("ToolbarTopics"));
	AddHelpTopic(IDW_VIEW_STATUSBAR, 	_T("StatusbarTopics"));
}

/*============================================================================*/
	BOOL ContextHelp::
InitiateContextHelp()        						/*

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
	BOOL    ContextHelp::
LookupAndDisplay(const CString& topic)                                  /*

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
	HWND hWnd = 0;
#ifndef __BORLANDC__        // not supported by Borland v5.5 compiler
	hWnd = HtmlHelp(HWND(), seek_url.c_str(), HH_DISPLAY_TOPIC,
	    (DWORD)NULL);
#endif
	return (hWnd != 0 ? TRUE : FALSE);
}

/*============================================================================*/
	BOOL 	ContextHelp::
OnInitDialog()                                                          /*

*-----------------------------------------------------------------------------*/
{
	  // do the base class initialization first
	CDialog::OnInitDialog();
	SetDlgItemText(IDC_ABOUT_BOX_CREDITS, m_sCredits.c_str());
	return TRUE;
}

/*============================================================================*/
	BOOL ContextHelp::
OnHelp(const CString &topic)                                   		/*

	Display the application guide topic, if present, or show the About
	dialog if there is an error.
*-----------------------------------------------------------------------------*/
{
	BOOL rtn = LookupAndDisplay(topic);
	m_bHelpActive = FALSE;   // terminate help mode
	::SetFocus(m_hHadFocus);  // restore focus to prev window containing it
	if (rtn)
		return TRUE;

	  // if no help window was opened, show that there was an error
	OnHelpAbout();
	::MessageBox(NULL, _T("Help topic could not be opened.\nMake sure that")
	    _T("the BasicForm.chm file is in the BasicForm.exe directory."),
	    _T("Error"), MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
	return TRUE;
}

/*============================================================================*/
	BOOL ContextHelp::
OnHelp(WPARAM wParam, UINT nMaxMRUSlots /* = 0 */) 			/*

	Display the application guide topic corresponding to the numeric
	identifier contained in the low-order word of wParam, if present in the
	help table, and if the topic exists in the guide. Otherwise show the
	"feature not described" dialog if there is an error. Return TRUE on
	success or FALSE otherwise. It is assumed that context help mode is
	active upon invocation.
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

	return OnHelpID(nID, nMaxMRUSlots);
}

/*============================================================================*/
	BOOL ContextHelp::
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
	{
		  // make the AboutBox modeless
		Create();
	}
	return TRUE;
}

/*============================================================================*/
	BOOL ContextHelp::
OnHelpID(UINT nID, UINT nMaxMRUSlots /* = 0*/)				/*

	Display the application guide topic corresponding to the numeric
	identifier nID, if present in the help table, and if the topic exists
	in the guide. Otherwise, show the "feature not described" dialog if
	there is an error. Return TRUE on success or FALSE otherwise. Context
	help mode need not be activeupon invocation.
*-----------------------------------------------------------------------------*/
{
	if (nID == IDW_ABOUT)
		return OnHelpAbout();

	  // map all MRU file messages to one representative
	if(IDW_FILE_MRU_FILE1 <= (UINT)nID &&
	    (UINT)nID < IDW_FILE_MRU_FILE1 + nMaxMRUSlots)
		nID = (int)IDW_FILE_MRU_FILE1;

	for (UINT id = 0; id < (UINT)m_help_table.size(); id++)
	{
		help_message msg = m_help_table[id];
		if (msg.nID == nID)
			return OnHelp(msg.topic);
	}
	  // no registered help topic found, so display default manual topic
	return OnHelp(_T("feature not described"));
}

