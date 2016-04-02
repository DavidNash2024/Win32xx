/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]              (MainFrm.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description: Implementation of the CMainFrame class for the
	CommonDialogs sample application using the Win32++ Windows interface
	classes, Copyright (c) 2005-2016 David Nash, under permissions granted
	therein.

	This particular frame class contains features a fixed-size form for the
	display, with no resizing gripper tool at the end of the status bar,
	and provisions for selection of client background color, selection of
	edit box font, use of external serialization files, MRU lists, and
	context help for all controls on the frame and client area.

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
	Citation of the author's work should be included in such usages.

	Special Conventions:

	Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Implementation of the CMainFrame class

*******************************************************************************/

#include "stdafx.h"

#include "resource.h"
#include "App.h"
#include "MRU.h"


/*******************************************************************************

	Macros and Local (static) default constants          		*/

  // latest file compilation date
CString CMainFrame::m_sCompiled_on = __DATE__;

static const	UINT    nMaxMRUSlots = 5;
  // the (fixed) window dimensions : {left, top, width, height}
static const	RECT    rcWindowPlacement = {100, 100, 490, 400};

/*******************************************************************************

	Implementation of the Main Frame class

*=============================================================================*/
	CMainFrame::
CMainFrame() 								/*

	Construct and initiallize the CMainFrame object from the IDD_FORM_DIALOG
	resource defined in resource.rc.
*-----------------------------------------------------------------------------*/
	: m_View(IDD_FORM_DIALOG)
{
	  // set normal cursor to the arrow
	m_hCursor = ::LoadCursor(NULL, IDC_ARROW);
	  // set the form initial default location and (fixed) size
	m_win_x  = rcWindowPlacement.left;
	m_win_y  = rcWindowPlacement.top;
	m_width  = rcWindowPlacement.right;
	m_height = rcWindowPlacement.bottom;
	ZeroMemory(&m_Wndpl, sizeof(WINDOWPLACEMENT));
	  // Set m_View as the view window of the frame
	SetView(m_View);
	  // inform the MRU object of the frame that owns it
	m_MRU.ConnectMRU(this, nMaxMRUSlots);
	  // define the context help topics and controls they service
	SetContextHelpMessages();
}

/*============================================================================*/
	void CMainFrame::
EngageContextHelp() 							/*

	Set the context help mode on and set cursors to the help cursor.
*-----------------------------------------------------------------------------*/
{
	  // if context help is already active, no need to engage further
	if (m_ContextHelp.IsActive())
		return;

	  // set the cursor(s) in all controls to help
	m_hCursor = ::LoadCursor(NULL, IDC_HELP);
    	  // set help mode on
    	m_ContextHelp.InitiateContextHelp();
}

/*============================================================================*/
	void    CMainFrame::
LoadPersistentData()                                                    /*

	Recover app, mainframe, MRU, and view serialized data from previous
	execution.
*-----------------------------------------------------------------------------*/
{
	try
	{
		CArchive ar(theApp.GetArcvFile(), CArchive::load);
	          // deserialize in the same order as serialized
		ar >> theApp;	// for the app
		ar >> *this;	// for the frame
		ar >> m_MRU;    // for the MRU list
		ar >> m_View;   // for the view, including control colors
		Invalidate();  // repaint the client with recovered colors

		  // if there is a MRU item at the top of the list, use it
		  // as the name of the document to open
		CString docfile = m_MRU.AccessMRUEntry(0);
		if (!docfile.IsEmpty())
			m_Doc.OnOpenDoc(docfile);
		UpdateToolbarMenuStatus();
	  // the ar object closes on destruction
	}
	catch (const CException& e)  // catch all CException events
	{
		  // Process the exception
		CString msg = "Previous settings could not be restored.\n";
		msg += e.GetText();
		::MessageBox(NULL, msg, A2T(e.what()), MB_OK | MB_ICONSTOP |
		    MB_TASKMODAL);
	}
	catch(...) // catch all other exception events
	{
		CString msg = _T("Previous settings could not be restored.\n");
		::MessageBox(NULL, msg.c_str(), _T("Unknown Exception"),
		    MB_OK | MB_ICONSTOP | MB_TASKMODAL);
	}
}

/*============================================================================*/
	void CMainFrame::
OnColorChoice()     		                                       	/*

        Show the color common dialog box and select the app backbround color.
*-----------------------------------------------------------------------------*/
{
	m_View.OnColorChoice();
	UpdateToolbarMenuStatus();
	Invalidate();
	UpdateWindow();
}

/*============================================================================*/
	BOOL CMainFrame::
OnCommand(WPARAM wParam, LPARAM lParam)					/*

	The framework calls this member method when the user selects an
	item from a menu, when a child control sends a notification message,
	or when an accelerator keystroke is translated. Here, we respond to
	menu selections, toolbar events, scrollbar actions, and accelerator
	keys in both action and help modes.

	The low-order word of wParam identifies the command ID of the menu
	or accelerator message. The high-order word is 1 if the message is
	from an accelerator, or 0 if the message is from the menu.

	The command ID may not always be sufficient to determine the action
	to be taken.  For example, the CBN_SELCHANGE message will be received
	when a user changes the current selection in the list box of a combo
	box, and appears in the high-order word of the wParam parameter.
	However, the CBN_SELCHANGE message value itself is 1, and is thus
	indistingishable from an accelerator event on this basis.  For this
	reason, the low-order word will also need to be queried to distinguish
	the actual combobox event.

	The method returns nonzero if it processes the message; otherwise it
	returns zero.

	In this particular case, the command message may be directed toward
	context help, in which case a separate method is invoked to display
	the help topic indicated.  Otherwise, the command messages are assumed
	to be for actions; these are routed to the more traditional message
	handling methods.
*-----------------------------------------------------------------------------*/
{
	  // if context help mode is active, display the help topic for wParam
	if (OnContextHelp(wParam))
		return TRUE;

	UINT nID = LOWORD(wParam);
	  // map all MRU file messages to one representative
	if(IDW_FILE_MRU_FILE1 <= nID &&
	    nID < IDW_FILE_MRU_FILE1 + nMaxMRUSlots)
		nID = IDW_FILE_MRU_FILE1;

	switch(nID)
	{
	    case IDM_FILE_NEW:
		m_Doc.OnFileNewDialog();
		return TRUE;

	    case IDM_FILE_OPEN:
		m_Doc.OnDocOpenDialog();
		return TRUE;

	    case IDM_FILE_SAVE:
	    	m_Doc.OnSaveDoc();
		return TRUE;

	    case IDM_FILE_SAVEAS:
	    	m_Doc.OnSaveDocAs();
		return TRUE;

	    case IDM_FILE_CLOSE:
		m_Doc.OnCloseDoc();
		return TRUE;

	    case IDM_FILE_PAGESETUP:
	    	m_Doc.OnPageSetup();
		return TRUE;

	    case IDM_FILE_PREVIEW:
	    	m_Doc.OnPrintPreview();
		return TRUE;

	    case IDM_FILE_PRINT:
	    	m_Doc.OnPrintDialog();
		return TRUE;

	    case IDM_FILE_EXIT:
		  // Issue a close request to the frame
		SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
		return TRUE;

	    case IDM_EDIT_UNDO:
	    	m_Doc.OnUndo();
		return TRUE;

	    case IDM_EDIT_REDO:
	    	m_Doc.OnRedo();
		return TRUE;

	    case IDM_EDIT_CUT:
	    	m_Doc.OnCut();
		return TRUE;

	    case IDM_EDIT_COPY:
	    	m_Doc.OnCopy();
		return TRUE;

	    case IDM_EDIT_PASTE:
	    	m_Doc.OnPaste();
		return TRUE;

	    case IDM_EDIT_DELETE:
	    	m_Doc.OnDelete();
		return TRUE;

	    case IDM_EDIT_FIND:
	    	m_Doc.OnFindDialog();
		return TRUE;

	    case IDM_EDIT_REPLACE:
	    	m_Doc.OnReplaceDialog();
		return TRUE;

	    case IDM_HELP_CONTENT:
	    	EngageContextHelp();
	    	return OnContextHelp(wParam);

	      // Handle all of the help request messages from common dialogs.
	    case IDC_HELP_COMDLG:
	    	EngageContextHelp();
	    	return OnContextHelp((WPARAM)lParam);

	    case IDW_ABOUT:
	    	return m_ContextHelp.OnHelpID(IDW_ABOUT, nMaxMRUSlots);

	    case IDM_HELP_CONTEXT:
		EngageContextHelp();
		return TRUE;

	    case IDW_VIEW_TOOLBAR:
	    	OnViewToolBar(); // toggle tool bar
		return TRUE;

	    case IDW_VIEW_STATUSBAR:
	    	OnViewStatusBar(); // toggle status bar
		return TRUE;

	    case IDM_COLOR_CHOICE:
		OnColorChoice();
		return TRUE;

	    case IDM_FONT_CHOICE:
		OnFontChoice();
		return TRUE;

	    case IDW_FILE_MRU_FILE1:
	    	OnProcessMRU(wParam, lParam);
	    	return TRUE;

	    default:
		break;
	}

	return FALSE;
}

/*============================================================================*/
	BOOL CMainFrame::
OnContextHelp(WPARAM wParam)						/*

	This method is invoked when the user selects an item from a menu, when
	a child control sends a notification message, or when an accelerator
	keystroke is translated. If the frame is not in context help mode,
	returns FALSE. When context help mode is active, the method returns
	TRUE.  The low-order word of wParam identifies the command nID of the
	menu or accelerator message. If this method is invoked from an
	OnCommand() method, the high-order word is 1 if the message is from
	an accelerator, or 0 if the message is from the menu. If it is invoked
	as the result of WM_COMMAND or WM_SYSCOMMAND, the high-order word
	carries a notification code.
*-----------------------------------------------------------------------------*/
{
	  // make sure context help is active
	if (!m_ContextHelp.IsActive())
		return FALSE;

	  // show the topic and cancel help mode
	m_ContextHelp.OnHelp(wParam, nMaxMRUSlots);
	  // context help is no longer active, so restore normal cursors
	m_hCursor = ::LoadCursor(NULL, IDC_ARROW);
	Invalidate();

	return TRUE;
}

/*============================================================================*/
	int CMainFrame::
OnCreate(CREATESTRUCT& rcs)                                            /*

	This member controls the way the frame is created. Overriding
	it is optional.
*-----------------------------------------------------------------------------*/
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::OnCreate is optional.
	// The default for the following variables is TRUE

	// m_UseIndicatorStatus = FALSE; // Don't show keyboard indicators
	// m_UseMenuStatus = FALSE;	 // Don't show menu descriptions
	// m_UseReBar = FALSE;		 // Don't use a ReBar
	// m_UseThemes = FALSE;		 // Don't use themes
	// m_UseToolBar = FALSE;	 // Don't use a ToolBar

	  // TODO: set CREATESTRUCT desired options here

	  // call the base class OnCreate() method with these options
	int rtn = CFrame::OnCreate(rcs);
	  // set theme colors, if supported
	if (IsReBarSupported())
	{
		//Set our theme colors
		SetThemeColors();
	}

	  // communicate help file name and about box contents to
	  // context help object
	RegisterHelpParameters();
	LoadPersistentData();
	  // weed out any MRU entries that have disappeared
//	m_MRU.ValidateMRU(); // TODO: uncomment this when we have actual files
	return rtn;
}

/*============================================================================*/
	void CMainFrame::
OnFileExit()								/*

	Perform whatever functions are necessary, other than Serialize(), as
	it is invoked in response to the WM_CLOSE message that is sent when
	the frame is close.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
	BOOL CMainFrame::
OnFileOpenMRU(UINT nIndex)						/*

	Open the MRU file at nIndex as the next document.
*-----------------------------------------------------------------------------*/
{
	if (m_Doc.IsOpen())
		m_Doc.OnCloseDoc();

	// get the MRU entry but don't move it to the top of the MRU list, yet
	CString str = m_MRU.AccessMRUEntry(nIndex);
	m_Doc.OnOpenDoc(str);
	UpdateToolbarMenuStatus();
	if (m_Doc.IsOpen())
	{         // now it's ok to move it
		m_MRU.AddMRUEntry(str);
		return TRUE;
	}

	return FALSE;
}

/*============================================================================*/
	void CMainFrame::
OnFontChoice()     		                                 	/*

        Select the view font typeface, characteristics, and color.
*-----------------------------------------------------------------------------*/
{
	m_View.OnFontChoice();
	Invalidate();
	UpdateToolbarMenuStatus();
	UpdateWindow();
}

/*============================================================================*/
	void CMainFrame::
OnInitialUpdate()                                                       /*

	The frame has now been created. Provide any desired main frame
	formatting.
*-----------------------------------------------------------------------------*/
{
	// TODO: Place any additional startup code here.

	  // Give keyboard focus to the view window:
	m_View.SetFocus();

	  // show initial button status
	UpdateToolbarMenuStatus();
	TRACE("Frame created\n");
}

/*============================================================================*/
	void CMainFrame::
OnMenuUpdate(UINT nID)							/*

	This method is invoked for each item in a top-level menu item the mouse
	selects. Override the base class method to update the frame status
	as well as the usual tool bar menu items.
*-----------------------------------------------------------------------------*/
{
	  // Update the check state of the various menu items
	UpdateToolbarMenuStatus();
	CFrame::OnMenuUpdate(nID);
}

/*============================================================================*/
	LRESULT CMainFrame::
OnNotify(WPARAM wParam, LPARAM lParam)                                  /*

	Process messages that controls send to the main frame, events such
	as mouse clicks, changes in content and  selection, and  control
	background painting by sending a message to the parent.
**-----------------------------------------------------------------------------*/
{
	// TODO: Add code here to implement this member

	  // Process notification messages sent by child windows
//	switch(((LPNMHDR)lParam)->code)
//	{
//		// Add case statements for each notification message here
//	}

	// Some notifications should return a value when handled
	return CFrame::OnNotify(wParam, lParam);
}

/*============================================================================*/
	BOOL CMainFrame::
OnProcessMRU(WPARAM wParam, LPARAM lParam)                              /*

	One of the MRU entries has been selected.  Process accordingly.
*-----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(lParam);

	  // compute the MRU index, where IDW_FILE_MRU_FILE1 is index 0
	UINT nMRUIndex = LOWORD(wParam) - IDW_FILE_MRU_FILE1;
	OnFileOpenMRU(nMRUIndex);
	return TRUE;
}

/*============================================================================*/
	void CMainFrame::
UpdateToolbarMenuStatus()						/*

	Check the status of controls whose enabled/disabled or check/uncheck
	status needs to be changed and make changes as necessary. Caution:
	only use one SetXXXXStatus per control ID.
*-----------------------------------------------------------------------------*/
{
	// TODO: Add and modify code here to implement this member

	  // document status
	BOOL 	doc_is_ready = m_Doc.IsOpen();

	  // determine enabled status of controls (TODO: redo for actual app)
	BOOL	ok_to_cut         = TRUE;   // doc_is_ready;
	BOOL	ok_to_copy        = FALSE;  // doc_is_ready;
	BOOL	ok_to_delete      = FALSE;  // doc_is_ready;
	BOOL	ok_to_find        = TRUE;   // doc_is_ready;
	BOOL	ok_to_paste       = FALSE;  // doc_is_ready;
	BOOL	ok_to_print       = TRUE;   // doc_is_ready;
	BOOL	ok_to_preview     = TRUE;   // doc_is_ready;
	BOOL	ok_to_page_setup  = TRUE;   // doc_is_ready;
	BOOL	ok_to_redo        = FALSE;  // doc_is_ready;
	BOOL	ok_to_replace     = TRUE;   // doc_is_ready;
	BOOL 	ok_to_save        = doc_is_ready;
	BOOL	ok_to_saveas      = doc_is_ready;
	BOOL	ok_to_undo        = FALSE;  // doc_is_ready;

 	  // set the control button status
	SetControlStatus(IDM_EDIT_CUT, 		ok_to_cut, 	   both);
	SetControlStatus(IDM_EDIT_COPY, 	ok_to_copy, 	   both);
	SetControlStatus(IDM_EDIT_DELETE, 	ok_to_delete, 	   both);
	SetControlStatus(IDM_EDIT_FIND, 	ok_to_find, 	   both);
	SetControlStatus(IDM_FILE_PAGESETUP,	ok_to_page_setup,  mainmenu);
	SetControlStatus(IDM_EDIT_PASTE, 	ok_to_paste,	   both);
	SetControlStatus(IDM_FILE_PRINT, 	ok_to_print,  	   both);
	SetControlStatus(IDM_FILE_PREVIEW,      ok_to_preview,     mainmenu);
	SetControlStatus(IDM_EDIT_REDO, 	ok_to_redo,	   both);
	SetControlStatus(IDM_EDIT_REPLACE, 	ok_to_replace, 	   both);
	SetControlStatus(IDM_FILE_SAVE, 	ok_to_save,        both);
	SetControlStatus(IDM_FILE_SAVEAS, 	ok_to_saveas, 	   both);
	SetControlStatus(IDM_EDIT_UNDO, 	ok_to_undo, 	   both);
}

/*============================================================================*/
	void CMainFrame::
PreCreate(CREATESTRUCT& cs)                                             /*

	Set cs members to select window frame parameters desired. This gets
	executed before CView::PreCreate(). Use the serialized position
	and size information to set the frame positioning and size.
*-----------------------------------------------------------------------------*/
{
	  // do the base class stuff
	CFrame::PreCreate(cs);
	cs.x  = m_win_x;  // set initial window placement
	cs.y  = m_win_y;
	cs.cx = m_width;
	cs.cy = m_height;
	   // specify a title bar and border with a window-menu on the title bar
	cs.style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE
	    | WS_MINIMIZEBOX    // adds the minimize box
	    | WS_MAXIMIZEBOX    // adds the maximize box
//	    | WS_THICKFRAME     // same as WS_SIZEBOX, enables resizing
	    ;
	cs.dwExStyle = WS_EX_CLIENTEDGE
	    | WS_EX_CONTROLPARENT       // TAB key navigation
//	    | WS_EX_CONTEXTHELP   	// doesn't work if WS_MINIMIZEBOX
					// or WS_MAXIMIZEBOX is specified
	    ;
}

/*============================================================================*/
	BOOL CMainFrame::
PreTranslateMessage(MSG& Msg)                                           /*

	Pretranslates the system message uMsg from the mainframe message loop
	before they are dispatched to theTranslateMessage and DispatchMessage
	Windows functions in the message loop and before they are dispached
	to the PreTranslateMessage method of CWnd. MSG contains the message
	to process. Return a nonzero if the message was translated and should
	not be dispatched; return 0 if the message was not translated and
	should be dispatched.
*-----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(Msg);
//	HWND   hwnd	= Msg->hwnd;
//	UINT   message	= Msg->message;
//	WPARAM wParam	= Msg->wParam;
//	LPARAM lParam	= Msg->lParam;
//	DWORD  time	= Msg->time;
//	CPoint  pt	= Msg->pt;

	  // return 0 if the message was NOT handled here
	return 0;
}

/*============================================================================*/
	BOOL CMainFrame::
SaveRegistrySettings()                                                  /*

	This member is called in response to a WM_CLOSE message for the
	frame.  The base class saves settings into the registry. In this
	override, the archive member is called to save everything into
	archive files.
*-----------------------------------------------------------------------------*/
{
	try
	{
		CArchive ar(theApp.GetArcvFile(), CArchive::store);
	         // serialize in the following order
		ar << theApp;   // for the App
		ar << *this;    // for the main frame
		ar << m_MRU;    // for the MRU list
		ar << m_View;   // for the view, including control colors
	}
	catch (const CException& e)  // catch all std::exception events
	{
		  // Process the exception and quit
		CString msg;
		msg.Format(_T("%s\n%s\n%s"),
		    _T("Program settings could not be saved."),
		    e.GetErrorString(), e.what());
		::MessageBox(NULL, msg, _T("Exception"), MB_OK | MB_ICONSTOP |
		    MB_TASKMODAL);
	}
	catch(...)
	{
		CString msg = _T("\nProgram settings could not be saved.\n    ");
		::MessageBox(NULL, msg.c_str(), _T("Unregistered Exception"),
		    MB_OK | MB_ICONSTOP | MB_TASKMODAL);
	}
	return TRUE;
}

/*============================================================================*/
        void CMainFrame::
Serialize(CArchive &ar)                                               /*

        Called serialize or deserialize the frame to and from the archive ar,
        depending on the sense of IsStoring().  Leave the archive open for
        further serializations.
*-----------------------------------------------------------------------------*/
{
	  // perform loading or storing
        if (ar.IsStoring())
        {
 		  // save the base class frame status and tool bar switches:
		  // these control the display of the StatusBar and ToolBar
		BOOL showbar = GetStatusBar().IsWindowVisible();
		ar << showbar;
		showbar = GetToolBar().IsWindowVisible();
		ar << showbar;
                  // save current window placement information
		ZeroMemory(&m_Wndpl, sizeof(WINDOWPLACEMENT));
		m_Wndpl.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(m_Wndpl);
		ArchiveObject w(&m_Wndpl, m_Wndpl.length);
		ar << w;
	}
        else    // recovering
        {
  		  // recover frame status and  tool bar base class switches
		BOOL showbar;
		ar >> showbar;
		ShowStatusBar(showbar);
		ar >> showbar;
		ShowToolBar(showbar);
                   // recover window placement information
		ArchiveObject w(&m_Wndpl, sizeof(WINDOWPLACEMENT));
		ar >> w;
		if (m_Wndpl.length != 0)
		{
			m_Wndpl.length = sizeof(WINDOWPLACEMENT);
			SetWindowPlacement(m_Wndpl);
		}
        }
}

/*============================================================================*/
	void CMainFrame::
SetContextHelpMessages() 						/*

	Define the set of all context help topics to be displayed for each of
	the control identifiers that are serviced in this app, except for those
	for CCommonDialog help boxes and the frame SC_MAXIMIZE, SC_MINIMIZE,
	and SC_CLOSE boxes (these are defaults in the ContextHelp class).
*-----------------------------------------------------------------------------*/
{
	  // define the tool bar button and menu item topics
	m_ContextHelp.AddHelpTopic(IDM_FILE_NEW, 	_T("NewDocument"));
	m_ContextHelp.AddHelpTopic(IDM_FILE_OPEN, 	_T("OpenExistingDocument"));
	m_ContextHelp.AddHelpTopic(IDM_FILE_SAVE, 	_T("SaveCurrentDocument"));
	m_ContextHelp.AddHelpTopic(IDM_FILE_SAVEAS, 	_T("SaveAsAnotherDocument"));
	m_ContextHelp.AddHelpTopic(IDM_FILE_CLOSE, 	_T("CloseCurrentDocument"));
	m_ContextHelp.AddHelpTopic(IDM_FILE_PAGESETUP,  _T("PageSetupForPrintout"));
	m_ContextHelp.AddHelpTopic(IDM_FILE_PRINT, 	_T("PrintDocument"));
	m_ContextHelp.AddHelpTopic(IDM_FILE_PREVIEW,    _T("PreviewPrintout"));
	m_ContextHelp.AddHelpTopic(IDM_FILE_EXIT,  	_T("ExitTerminateProgram"));
	m_ContextHelp.AddHelpTopic(IDW_FILE_MRU_FILE1, 	_T("MostRecentlyUsedList"));
	m_ContextHelp.AddHelpTopic(IDM_EDIT_UNDO, 	_T("UndoFunction"));
	m_ContextHelp.AddHelpTopic(IDM_EDIT_REDO, 	_T("RedoFunction"));
	m_ContextHelp.AddHelpTopic(IDM_EDIT_CUT, 	_T("CutFunction"));
	m_ContextHelp.AddHelpTopic(IDM_EDIT_COPY, 	_T("CopyFunction"));
	m_ContextHelp.AddHelpTopic(IDM_EDIT_PASTE, 	_T("PasteFunction"));
	m_ContextHelp.AddHelpTopic(IDM_EDIT_DELETE, 	_T("DeleteFunction"));
	m_ContextHelp.AddHelpTopic(IDM_EDIT_FIND, 	_T("FindInDocument"));
	m_ContextHelp.AddHelpTopic(IDM_EDIT_REPLACE,	_T("ReplaceInDocument"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_CONTENT, 	_T("Introduction"));
	m_ContextHelp.AddHelpTopic(IDW_ABOUT, 		_T("AboutThisProgram"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_CONTEXT,	_T("Welcome"));
	m_ContextHelp.AddHelpTopic(IDW_VIEW_TOOLBAR, 	_T("ToolbarTopics"));
	m_ContextHelp.AddHelpTopic(IDW_VIEW_STATUSBAR, 	_T("StatusbarTopics"));
	m_ContextHelp.AddHelpTopic(IDM_COLOR_CHOICE, 	_T("ColorChoiceFunction"));
	m_ContextHelp.AddHelpTopic(IDM_FONT_CHOICE, 	_T("FontChoiceFunction"));
	  // define client area controls topics
	m_ContextHelp.AddHelpTopic(IDM_EDITBOX, 	_T("EditBoxUsage"));
	m_ContextHelp.AddHelpTopic(IDOK, 		_T("OKButtonUsage"));
	  // define common dialog help button topics
	m_ContextHelp.AddHelpTopic(IDM_HELP_COLORDLG, 	_T("ColorDialogHelp"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_FILEDLG_OPEN, _T("FileDialogOpenHelp"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_FILEDLG_NEW, _T("FileDialogNewHelp"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_FILEDLG_SAVEAS, _T("FileDialogSaveAsHelp"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_FONTDLG,    _T("FontChoiceDialogHelp"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_PAGESETDLG, _T("PageSetupDialogHelp"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_PRINTDLG, 	_T("PrinterDialogHelp"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_FINDDLG,	_T("FindTextDialogHelp"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_REPLACEDLG, _T("ReplaceTextDialogHelp"));
	m_ContextHelp.AddHelpTopic(IDM_HELP_LIST_BOX, 	_T("ListBoxDialogHelp"));
}

/*============================================================================*/
	BOOL CMainFrame::
SetControlStatus(UINT nID, BOOL status, ControlBars which)           	/*

	Set the control having the given nID to the enable/disable
	status, on the toolbar, main menu, or both, as indicated by which.
*-----------------------------------------------------------------------------*/
{
	UINT action = MF_BYCOMMAND | (status ? MF_ENABLED : MF_GRAYED);
	if (which == mainmenu)
		return (GetFrameMenu().EnableMenuItem(nID, action)
		    != 0xFFFFFFFF);
	else if (which == toolbar)
		return (status ? GetToolBar().EnableButton(nID) :
		    GetToolBar().DisableButton(nID));
	  // else must be both
	BOOL rtn = (GetFrameMenu().EnableMenuItem(nID, action)
	    != 0xFFFFFFFF);
	rtn |= (status ? GetToolBar().EnableButton(nID) :
	    GetToolBar().DisableButton(nID));
	return rtn;
}

/*============================================================================*/
	 void CMainFrame::
SetStatusbarMsg(CString status)             /*

	Write the status message on the status bar.
*-----------------------------------------------------------------------------*/
{
	GetStatusBar().SetPartText(0, status.c_str());
}

/*============================================================================*/
	void CMainFrame::
SetReBarColors(COLORREF clrBkGnd1, COLORREF clrBkGnd2, COLORREF clrBand1,
    COLORREF clrBand2)  						/*

	Set the colors to be used in the rebar theme.
*-----------------------------------------------------------------------------*/
{
	if (IsReBarSupported())
	{
		ReBarTheme rt;
		ZeroMemory(&rt, sizeof(ReBarTheme));
		rt.UseThemes    = TRUE;
		rt.clrBkgnd1    = clrBkGnd1;
		rt.clrBkgnd2    = clrBkGnd2;
		rt.clrBand1     = clrBand1;
		rt.clrBand2     = clrBand2;
		rt.FlatStyle    = FALSE;
		rt.BandsLeft    = TRUE;
		rt.LockMenuBand = TRUE;
		rt.RoundBorders = TRUE;
		rt.ShortBands   = TRUE;
		rt.UseLines     = TRUE;

		SetReBarTheme(rt);
	}
}

/*============================================================================*/
	BOOL CMainFrame::
SetThemeColors()                                                      	/*

	Set the colors of each rebar in the frame, i.e., the theme colors.
	These were taken from the Win32++ Themes sample program recommended
	colors for Windows XP, case IDM_BLUE.
*-----------------------------------------------------------------------------*/
{
	SetReBarColors(RGB(150, 190, 245), RGB(196, 215, 250),
	    RGB(220, 230, 250), RGB( 70,130,220));

	StatusBarTheme sbt = {TRUE, RGB(150, 190, 245), RGB(196, 215, 250)};
	SetStatusBarTheme(sbt);

	ToolBarTheme tbt = {TRUE, RGB(255, 230, 190), RGB(255, 190, 100),
	    RGB(255, 140, 40), RGB(255, 180, 80), RGB(128, 128, 255)};
	SetToolBarTheme(tbt);

	MenuTheme mt = {TRUE, RGB(255, 230, 190), RGB(255, 190, 100),
	    RGB(150, 190, 245), RGB(220, 230, 250), RGB(128, 128, 200)};
	SetMenuTheme(mt);
	RecalcLayout();
	return TRUE;
}

/*============================================================================*/
	void CMainFrame::
SetupToolBar()                                                          /*

	Called from the CFrame::CreateToolBar() method to load the toolbar
	bitmaps, to connect the tool bar buttons to resource IDs of the
	toolbar buttons, and to define the order of appearance of the buttons
	on the toolbar at runtime.
*-----------------------------------------------------------------------------*/
{
          // start out with a separator just to give some space at the
          // left of the toolbar
	AddToolBarButton(0);  // Separator
	  // Connect button IDs to button icons, show enabled status, and
	  // give the explicit image index iImage of each button in the bitmap.
	  // Add the toolbar buttons in the order they are to appear at runtime.
	AddToolBarButton(IDM_FILE_NEW,      TRUE, 0, 0);
	AddToolBarButton(IDM_FILE_OPEN,     TRUE, 0, 1);
	AddToolBarButton(IDM_FILE_SAVE,     TRUE, 0, 2);
	AddToolBarButton(0);  // Separator
	AddToolBarButton(IDM_EDIT_CUT,      TRUE, 0, 3);
	AddToolBarButton(IDM_EDIT_COPY,     TRUE, 0, 4);
	AddToolBarButton(IDM_EDIT_PASTE,    TRUE, 0, 5);
	AddToolBarButton(0);  // Separator
	AddToolBarButton( IDM_EDIT_FIND,    TRUE, 0, 6);
	AddToolBarButton( IDM_EDIT_REPLACE, TRUE, 0, 7);
	AddToolBarButton(0);  // Separator
	AddToolBarButton(IDM_FILE_PRINT,    TRUE, 0, 8);
	AddToolBarButton(0);  // Separator
	AddToolBarButton(IDM_COLOR_CHOICE,  TRUE, 0, 9);
	AddToolBarButton(IDM_FONT_CHOICE,   TRUE, 0, 10);
	AddToolBarButton(0);  // Separator
	AddToolBarButton(IDM_HELP_CONTEXT,  TRUE, 0, 11);
	  // Set the toolbar image list: use defaults for hot and disabled
	SetToolBarImages(RGB(255, 0, 255), IDW_MAIN, 0, 0);
	  // Set icons for color and font choice menu items
	AddMenuIcon(IDM_COLOR_CHOICE, theApp.LoadIcon(IDI_COLOR_CHOICE));
	AddMenuIcon(IDM_FONT_CHOICE,  theApp.LoadIcon(IDI_FONT_CHOICE));
}

/*============================================================================*/
	void CMainFrame::
RegisterHelpParameters() 						/*

	Inform the ContextHelp object of the help .chm file name and ContextHelp
	of its credits information string.
*-----------------------------------------------------------------------------*/
{
	  // inform context help of its .chm document name (perhaps again,
	  // but maybe not
	m_ContextHelp.SetHelpFile(theApp.GetHelpFile());
	  // set the about box application information (perhaps again, in
	  // case the file fails
	m_ContextHelp.SetCredits(CApp::m_sLatest_compilation);
}

/*============================================================================*/
	LRESULT CMainFrame::
WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)                        /*

	This is the main frame message loop. By default, it handles the
	normal housekeeping functions (see Win32++\include\frame.h).
*-----------------------------------------------------------------------------*/
{
	  // can't use a case statement on this:
	if (uMsg == UWM_FINDMSGSTRING)
	{
		m_Doc.OnFindReplace(uMsg, wParam, lParam);
		return TRUE;
	}

	switch (uMsg)
	{
		// Add case statements for each message to be handled here

	    case WM_SETCURSOR:
	    {
		::SetCursor(m_hCursor);
		return TRUE;
	    }

	    case WM_HELP:
	    {     // Handle the F1 requests for help. The HELPINFO in lParam
		  // is of no use to the current context help class.
		m_ContextHelp.OnHelpID(IDM_HELP_CONTENT, nMaxMRUSlots);
	    	return TRUE;
	    }

	    case WM_SYSCOMMAND:
	    {
		  // if in help mode, let it handle the wParam message
	    	if (OnContextHelp(wParam))
			return TRUE;

		  // else process requests for action
		switch (LOWORD(wParam))
		{
		    case SC_CLOSE:
			OnFileExit();
			break;  // let default process this further
		}
	    }
	}
	  // pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

