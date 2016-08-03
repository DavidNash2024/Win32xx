/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]              (MainFrm.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

	Contents Description:  Implementation of the cMainFrm class for a
	skeleton SDI application using the Win32++ Windows interface classes,
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
	Citation of the author's work should be included in such usages.

	Special Conventions:

	Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

	Acknowledgement:
	The author would like to thank and acknowledge the advice, critical
	review, insight, and assistance provided by David Nash in the development
	of this work.		
		
********************************************************************************

	Implementation of the CMainFrame class

*******************************************************************************/

#include "stdafx.h"
#include "Resource.h"
#include "App.h"
#include "MainFrm.h"

/*******************************************************************************

	Static constants initialization					*/

  // maximum number of MRU entries allowed
const size_t CMainFrame::m_nMaxMRUEntries = 5;
  // latest main frame file compilation date
const CString CMainFrame::m_sCompiled_on = __DATE__;

/*============================================================================*/
	CDoc&
TheDoc()								/*

	Return a reference to the current document object.  This function has
	global scope in order to access the CDoc class from anywhere outside
	the CMainFrame class. It is defined here because TheApp() is known in
	this context and not in Doc.h.
*-----------------------------------------------------------------------------*/
{
	return TheApp().TheFrame()->ThisDoc();
}

/*============================================================================*/
	CMainFrame::
CMainFrame() 								/*

	Construct and initiallize the CMainFrame object.
*-----------------------------------------------------------------------------*/
{
	  // set screen default position and  size
	m_win_y = 100;
	m_win_x = 100;
	m_win_width  = 800;
	m_win_height = 700;
	SetCursorShape(arrow);
	m_hCursor = ::LoadCursor(0, IDC_ARROW);
	ZeroMemory(&m_Wndpl, sizeof(WINDOWPLACEMENT));

	  // Set m_View as the view window of the frame
	SetView(m_View);
	  // inform the MRU object of the frame that owns it
	m_MRU.ConnectMRU(this, m_nMaxMRUEntries);
}

/*============================================================================*/
	CMainFrame::
~CMainFrame()                                                           /*

	 Destructor for CMainFrame.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
	void CMainFrame::
OnColorChoice()     		                                       	/*

        Select the view's backbround color.
*-----------------------------------------------------------------------------*/
{
	CColorDialog ColorDlg(m_View.m_rgbBkColor, CC_RGBINIT | CC_ANYCOLOR);
	ColorDlg.SetCustomColors(m_View.m_rgbCustomColors);
	if (ColorDlg.DoModal(m_View) == IDOK)
	{
		m_View.m_rgbBkColor = ColorDlg.GetColor();
	}
	
	UpdateFrame();
}

/*============================================================================*/
	BOOL CMainFrame::
OnCommand(WPARAM wParam, LPARAM lParam)					/*

	The framework calls this member function when the user selects an
	item from a menu, when a child control sends a notification message,
	 or  when an accelerator keystroke is translated. Here, we respond to
	menu selections, toolbar events, scrollbar actions, and  accelerator
	keys.

	Here, the actions are divided into two groups: those that invoke help
	actions, and  those that program invoke actions.

	The method returns nonzero if it processes the message; otherwise it
	returns zero.
*-----------------------------------------------------------------------------*/
{
	return (m_help_mode ?
	    OnCommandHelp(wParam, lParam) :
	    OnCommandAction(wParam, lParam));
}

/*============================================================================*/
	BOOL CMainFrame::
OnCommandAction(WPARAM wParam, LPARAM lParam)				/*

	Process the action specified in the low-order word of wParam, which may
	be a command ID of a menu or control, or an accelerator message. The
	high-order word is 1 if the message is from an accelerator, or 0 if
	the message is from the menu.

	The command ID may not always be sufficient to determine the action
	to be taken.  For example, the CBN_SELCHANGE message will be received
	when a user changes the current selection in the list box of a combo
	box, and  appears in the high-order word of the wParam parameter.
	However, the CBN_SELCHANGE message value itself is 1, and  is thus
	indistingishable from an accelerator event on this basis.  For this
	reason, the low-order word will also need to be queried to distinguish
	the actual combobox event.
*-----------------------------------------------------------------------------*/
{
	UINT lowParam = LOWORD(wParam);
	  // map all MRU file messages to one representative
	if(IDW_FILE_MRU_FILE1 <= lowParam &&
	    lowParam < IDW_FILE_MRU_FILE1 + m_nMaxMRUEntries)
		lowParam = IDW_FILE_MRU_FILE1;

	switch(lowParam)
	{
	    case IDM_FILE_NEW:
		OnFileNew();
		return TRUE;

	    case IDM_FILE_OPEN:
		OnFileOpen();
		return TRUE;

	    case IDM_FILE_SAVE:
	    	OnFileSave();
		return TRUE;

	    case IDM_FILE_SAVEAS:
	    	OnFileSaveAs();
		return TRUE;

	    case IDM_FILE_CLOSE:
		 OnFileClose();
		return TRUE;

	    case IDM_FILE_PRINT:
	    	 OnFilePrint();
		return TRUE;

	    case IDM_FILE_PRINT_PREVIEW:
	    	 OnFilePrintPreview();
		return TRUE;

	    case IDM_FILE_EXIT:
		OnFileExit();
		return TRUE;

	    case IDM_EDIT_UNDO:
	    	OnEditUndo();
		return TRUE;

	    case IDM_EDIT_REDO:
	    	OnEditRedo();
		return TRUE;

	    case IDM_EDIT_CUT:
	    	OnEditCut();
		return TRUE;

	    case IDM_EDIT_COPY:
	    	OnEditCopy();
		return TRUE;

	    case IDM_EDIT_PASTE:
	    	OnEditPaste();
		return TRUE;

	    case IDM_EDIT_DELETE:
	    	OnEditDelete();
		return TRUE;

	    case IDM_EDIT_FIND:
	    	OnEditFind();
		return TRUE;

	    case IDM_EDIT_REPLACE:
	    	OnEditReplace();
		return TRUE;

	    case IDM_HELP_CONTENT:
	    	OnHelp();
		return TRUE;

	    case IDM_HELP_ABOUT:
	    	OnHelpAbout();
		return TRUE;

	    case IDM_HELP_CONTEXT:
	    	OnHelpContext();
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
	}
	return FALSE;
}

/*============================================================================*/
	BOOL CMainFrame::
OnCommandHelp(WPARAM wParam, LPARAM lParam)				/*

	This procedure handles messages to engage the help system for items
	selected while the help cursor is shown. Normally each of the selections
	would connect to a .chm file via a OnHelp("topic") linkage, where topic"
	is one of the topics appearing in the help file table of contents.
	This capability, however, is not included here.

	TODO: supply meaningful help tips to topics below.
*-----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(lParam);
	// TODO: replace InfoMessageBox() instances by OnHelp("topic")
	// invocations as the given topic becomes available in the .chm
	// help document.
	UINT lowParam = LOWORD(wParam),
	     hiwParam = HIWORD(wParam);

	  // check the combo box for an item selection event
	if(hiwParam == CBN_SELCHANGE && lowParam == 0)
	{
		OnHelp(_T("No help for this topic."));
	}
	else
	{
		switch(lowParam)
		{
		    case IDW_MAIN:
			OnHelp(_T("No help for client area."));
			break;

		    case IDM_EDIT_COPY:
			OnHelp(_T("No help for COPY."));
			break;

		    case IDM_EDIT_CUT:
			OnHelp(_T("Remove the topmost MRU list item."));
			break;

		    case IDM_EDIT_DELETE:
			OnHelp(_T("No help for DELETE."));
			break;

		    case IDM_EDIT_FIND:
			OnHelp(_T("No help for FIND."));
			break;

		    case IDM_EDIT_PASTE:
			OnHelp(_T("No help for PASTE."));
			break;

		    case IDM_EDIT_REDO:
			OnHelp(_T("No help for REDO."));
			break;

		    case IDM_EDIT_REPLACE:
			OnHelp(_T("No help for REPLACE."));
			break;

		    case IDM_EDIT_UNDO:
			OnHelp(_T("No help for UNDO."));

		    case IDM_FILE_EXIT:
			OnHelp(_T("No help for EXIT."));
			break;

		    case IDM_FILE_NEW:
			OnHelp(_T("Recompute a new CFile Test array."));
			break;

		    case IDM_FILE_OPEN:
			OnHelp(_T("No help for OPEN FILE."));
			break;

		    case IDM_FILE_PRINT:
			OnHelp(_T("No help for PRINT."));
			break;

		    case IDM_FILE_PRINT_PREVIEW:
			OnHelp(_T("No help for PRINT PREVIEW."));
			break;

		    case IDM_FILE_SAVE:
			OnHelp(_T("No help for FILE SAVE."));
			break;

		    case IDM_FILE_SAVEAS:
			OnHelp(_T("No help for FILE SAVEAS..."));
			break;

		    case IDM_HELP_ABOUT:
			OnHelp(_T("No help for HELP ABOUT."));
			break;

		    case IDM_HELP_CONTENT:
			OnHelp(_T("No help for HELP CONTENT."));
			break;

		    case IDM_HELP_CONTEXT:
			OnHelp(_T("No help for CONTEXT HELP."));
			break;

		    case IDW_VIEW_STATUSBAR:
			OnHelp(_T("No help for VIEW STATUSBAR."));
			break;

		    case IDW_VIEW_TOOLBAR:
			OnHelp(_T("No help for VIEW TOOLBAR."));
			break;

		    case SC_MINIMIZE:
			OnHelp(_T("No help for MINIMIZE."));
			break;

		    case SC_MAXIMIZE:
			OnHelp(_T("No help for MAXIMIZE."));
			break;

	    	    case IDM_COLOR_CHOICE:
			OnHelp(_T("No help for COLOR CHOICE."));
			break;

		    case IDM_FONT_CHOICE:
			OnHelp(_T("No help for FONT CHOICE."));
			break;

	    	    case IDW_FILE_MRU_FILE1:
	    		break;

		    case SC_CLOSE:
			OnHelp(_T("No help for CLOSE WINDOW."));
			break;

		    default:
			OnHelp(_T("No help for frame boxes."));
		}
	}
	SetCursorShape(arrow); // cancels context help mode
	return TRUE;
}

/*============================================================================*/
	int CMainFrame::
OnCreate(CREATESTRUCT& cs)						/*

	This method controls the way the frame is created.
*-----------------------------------------------------------------------------*/
{
	  //The default for each of the following variables is TRUE:
//	    m_bShowIndicatorStatus = FALSE; // Don't show statusbar indicators
//	    m_bShowMenuStatus = FALSE;	    // Don't show toolbar or menu status
//	    m_bUseReBar = FALSE;	    // Don't use rebars
//	    m_bUseThemes = FALSE; 	    // Don't use themes
//	    m_bUseToolBar = FALSE;	    // Don't use a toolbar

	  // call the base class function
	int rtn = CFrame::OnCreate(cs);  
	  // make sure it created the frame
	if (rtn != 0)
		return rtn;

	  // get archved values
	try
	{
		  // open the application's saved parameter archive
		CArchive ar(TheApp().m_sArcvFile, CArchive::load);
		  // recover the frame saved parameters
		ar >> *this;
	}
	catch (const CException& e)
	{
		  // Process the exception and  quit
		CString msg,
			what(e.what());
		msg.Format(_T("Error restoring previous parameters.\n%s\n%s"),
		    e.GetText(), e.GetErrorString());
		::MessageBox(NULL, msg.c_str(), what.c_str(),
		    MB_OK | MB_ICONSTOP | MB_TASKMODAL);
		  // remove the corrupted application archive file
		::DeleteFile(TheApp().m_sArcvFile);
	}
	catch(...)
	{
		CString msg = _T("Error restoring previous parameters.\n");
		::MessageBox(NULL, msg.c_str(), _T("Exception"),
		    MB_OK | MB_ICONSTOP | MB_TASKMODAL);
	}
	  // recover the window placement, if read in ok
	if (m_Wndpl.length != 0)
	{
		m_Wndpl.length = sizeof(WINDOWPLACEMENT);
		SetWindowPlacement(m_Wndpl);
	}

	  // weed out any MRU entries that have disappeared
	m_MRU.ValidateMRU();
	// Show the gripper in the ToolBar
	GetReBar().ShowGripper(GetReBar().GetBand(GetToolBar()), TRUE);
	return rtn;
}

/*============================================================================*/
	void CMainFrame::
OnDestroy()								/*

	Save the mainframe, MRU, and view status parameters before termination.
*-----------------------------------------------------------------------------*/
{
	  // if there is an unsaved document, save it now
  	OnFileClose();
  	try
	{
		CArchive ar(TheApp().m_sArcvFile, CArchive::store);
		  // no serialization on Open() error
		ar << *this;  // for the frame
	}
	catch (const CException& e)
	{
		  // Process the exception and  quit
		CString msg,
			what(e.what());
		msg.Format(_T("Error while saving program settings:\n%s\n%s"),
		    e.GetText(), e.GetErrorString());
		::MessageBox(NULL, msg.c_str(), what.c_str(),
		    MB_OK | MB_ICONSTOP | MB_TASKMODAL);
	}
	catch(...)
	{
		CString msg = _T("Error while saving program settings:\n");
		::MessageBox(NULL, msg.c_str(), _T("Exception"),
		    MB_OK | MB_ICONSTOP | MB_TASKMODAL);
	}

	CFrame::OnDestroy();
}

/*============================================================================*/
	void CMainFrame::
OnEditCopy()								/*

*-----------------------------------------------------------------------------*/
{
	// TODO: Add code here to implement this member

}

/*============================================================================*/
	void CMainFrame::
OnEditCut()								/*

	For this demo, the topmost MRU item is removed, when one exists.
*-----------------------------------------------------------------------------*/
{
	// TODO: Add code here to implement this member

	CString s = m_MRU.AccessMRUEntry(0);
	if (!s.IsEmpty())
		m_MRU.RemoveMRUEntry(s);
	m_MRU.UpdateMRUMenu();
}

/*============================================================================*/
	void CMainFrame::
OnEditDelete()								/*

*-----------------------------------------------------------------------------*/
{
	// TODO: Add code here to implement this member

}

/*============================================================================*/
	void CMainFrame::
OnEditFind()								/*

*-----------------------------------------------------------------------------*/
{
	// TODO: Add code here to implement this member

}

/*============================================================================*/
	void CMainFrame::
OnEditPaste()								/*

*-----------------------------------------------------------------------------*/
{
	// TODO: Add code here to implement this member

}

/*============================================================================*/
	void CMainFrame::
OnEditRedo()								/*

*-----------------------------------------------------------------------------*/
{
	// TODO: Add code here to implement this member

}

/*============================================================================*/
	void CMainFrame::
OnEditReplace()								/*

*-----------------------------------------------------------------------------*/
{
	// TODO: Add code here to implement this member

}

/*============================================================================*/
	void CMainFrame::
OnEditUndo()								/*

*-----------------------------------------------------------------------------*/
{
	// TODO: Add code here to implement this member

}

/*============================================================================*/
	void CMainFrame::
OnFileClose()								/*

*-----------------------------------------------------------------------------*/
{
	m_Doc.CloseDoc();
	UpdateFrame();

}

/*============================================================================*/
	void CMainFrame::
OnFileExit()								/*

	Perform whatever functions are necessary, other than Serialize(), as
	it is invoked in response to the WM_CLOSE message that is sent when
	the frame is close.
*-----------------------------------------------------------------------------*/
{
	  // Issue a close request to the frame
	SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
}

/*============================================================================*/
	void CMainFrame::
OnFileNew()								/*

*-----------------------------------------------------------------------------*/
{
	  // close the current document
	m_Doc.CloseDoc();
	  // TODO: Add code here to implement this member. For this demo,
	  // refill the document with the initial document and empty the
	  // document file name
	m_Doc.NewDocument();
	UpdateFrame();
}

/*============================================================================*/
	void CMainFrame::
OnFileOpen()                                                            /*

	Bring up the open file dialog to choose a document file to open.
*-----------------------------------------------------------------------------*/
{
	// Bring up the dialog, and  open the file
	CString str =
	    m_Doc.GetDocOpenFileName(_T("Name the file to open..."));
	if (str.IsEmpty())
		return;

	m_Doc.OpenDoc(str);

	if (m_Doc.IsOpen())
		m_MRU.AddMRUEntry(str);
}

/*============================================================================*/
	bool CMainFrame::
OnFileOpenMRU(UINT nIndex)						/*

	Open the MRU file at nIndex as the next document.
*-----------------------------------------------------------------------------*/
{
	  // get the MRU entry if there is one (str will be empty if not)
	CString str = m_MRU.AccessMRUEntry(nIndex);
	if (str.IsEmpty())
		return false;
		
	if (m_Doc.OpenDoc(str))
	{         // now it's ok to add it to the top of the MRU list
		m_MRU.AddMRUEntry(str);
		return true;
	}
	else // if it could not be opened, remove the entry from the MRU list
		m_MRU.RemoveMRUEntry(str.c_str());

	return false;
}

/*============================================================================*/
	void CMainFrame::
OnFilePrint()                                                           /*

	Bring up the print dialog box and  choose the printing parameters. The
	rest is up to the particular needs of the application.
*-----------------------------------------------------------------------------*/
{
	// Bring up a dialog to choose the printer
	CPrintDialog Printdlg;

	try
	{
		Printdlg.DoModal(*this);

		// Retrieve the printer DC
		// CDC dcPrinter = Printdlg.GetPrinterDC();

		// TODO:
		// Add your own code here. Refer to the tutorial for
		// additional information
	}

	catch (const CException& /* e */)
	{
		// No default printer
		MessageBox(_T("Unable to display print dialog"),
		    _T("Print Failed"), MB_OK);
	}
}
/*============================================================================*/
	void CMainFrame::
OnFilePrintPreview()               					/*

	Show the document printed in preview form.
*-----------------------------------------------------------------------------*/
{
	// TODO: Add code here to implement this member

}

/*============================================================================*/
	void CMainFrame::
OnFileSave()                                                            /*

	Save the current document.
*-----------------------------------------------------------------------------*/
{
	m_Doc.SaveDoc();
	UpdateFrame();
}

/*============================================================================*/
	void CMainFrame::
OnFileSaveAs()                                                       	/*

	Engage the file save dialog to obtain the file name to be used in
	saving the current document and  to retain that new document as the
	current one.
*-----------------------------------------------------------------------------*/
{
	m_Doc.SaveDocAs();
}

/*============================================================================*/
	void CMainFrame::
OnFontChoice()     		                                 	/*

        Select the app font typeface, characteristics, and  color. The font
        background color is always the same as the client area background.
*-----------------------------------------------------------------------------*/
{
	LOGFONT lf;
	m_View.m_font.GetObject(sizeof(LOGFONT), &lf);
	DWORD dwFlags = CF_EFFECTS | CF_SCREENFONTS;
	
	CFontDialog FontDlg(&lf, dwFlags);
	FontDlg.SetColor(m_View.m_rgbTxColor);
	
	if (FontDlg.DoModal(m_View) == IDOK)
	{
		lf = FontDlg.GetLogFont();
		CFont f;
		if (f.CreateFontIndirect(lf))
			m_View.m_font = f;
		else
			::MessageBox(NULL, _T("Font creation error."),
			    _T("Error"), MB_OK | MB_ICONEXCLAMATION |
			    MB_TASKMODAL);

		m_View.m_rgbTxColor = FontDlg.GetColor();
	}

	m_View.SaveFontSize();
	UpdateFrame();
}

/*============================================================================*/
	BOOL CMainFrame::
OnHelp()                                   				/*

	Override the CFrame::OnHelp(), which calls the basic IDW_ABOUT dialog
	whenever the F1 key is depressed. We want to prevent this from
	happening here, but rather display the application's default help
	information.
*-----------------------------------------------------------------------------*/
{
	return OnHelp(_T(""));
}

/*============================================================================*/
	BOOL CMainFrame::
OnHelp(const CString &topic)                                   		/*

	Display the help documentation on the given topic, if not an empty
	string, or show the app's About dialog if it is empty.
*-----------------------------------------------------------------------------*/
{
	  // Programming note: this feature would normally display information
	  // on the given topic as extracted from a separate help manual, such
	  // as a Microsoft htmlhelp document (.chm).  That capability is not
	  // implemented here.

	  // if there is no topic, just show the app's About dialog
	if (topic.IsEmpty())
		return OnHelpAbout();

	SetCursorShape(arrow);   // terminate help mode
	::SetFocus(m_hadFocus);  // restore focus to prev window containing it
	::MessageBox(NULL, topic.c_str(), _T("Help"),
	    MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
	return TRUE;
}

/*============================================================================*/
	BOOL CMainFrame::
OnHelpAbout()                                                       	/*

	Display the application's help dialog box, which contains the app
	name, copyright information, and  date of most recent compilation.
*-----------------------------------------------------------------------------*/
{
	// Ensure that only one About dialog displays even for multiple
	// activation commands
	if (!m_AboutDialog.IsWindow())
	{
		// Store the window handle that currently has keyboard focus
		HWND hPrevFocus = ::GetFocus();
		if (hPrevFocus == GetMenuBar().GetHwnd())
			hPrevFocus = m_hWnd;

		  // make the AboutBox modeless
		m_AboutDialog.Create(*this);

		::SetFocus(hPrevFocus);
	}
	return TRUE;
}

/*============================================================================*/
	BOOL CMainFrame::
OnHelpContext()                                                       	/*

	When the help mode is use_context, show the help cursor and  set the
	m_help_context switch to true, which will divert mouse clicks  and
	accelerator keys to selection of help topics and  instantiation of
	the help documentation for that topic.
*-----------------------------------------------------------------------------*/
{
	SetCursorShape(help);
	m_hadFocus = ::GetFocus(); // save window with current keyboard focus
	SetFocus();  // terminate focus in the window it might have been in
	return TRUE;
}

/*============================================================================*/
	void CMainFrame::
OnInitialUpdate()                                                       /*

	The frame has now been created. Provide any desired main frame
	formatting.
*-----------------------------------------------------------------------------*/
{
	// TODO: Place any additional startup code here.

	  // give keyboard focus to the view window:
	TheView().SetFocus();
	  // open the most recently used document, if there was one
	OnFileOpenMRU(0);
	  // show initial button status
	OnUpdateStatus();
	TRACE("Frame created\n");
}

/*============================================================================*/
	void CMainFrame::
OnMenuUpdate(UINT nID)                                                  /*

	Update the state of menu entries, enabling or disabling them as
	appropriate.
 *-----------------------------------------------------------------------------*/
{
	switch (nID)
	{
	case IDM_FILE_SAVE:
		SetControlStatus(nID, ThisDoc().IsDirty(), mainmenu);
		break;
	case IDM_FILE_SAVEAS:
		SetControlStatus(nID, ThisDoc().IsOpen(), mainmenu);
		break;
	case IDM_FILE_PRINT:
		SetControlStatus(nID, false, mainmenu);
		break;
	case IDM_FILE_PRINT_PREVIEW:
		SetControlStatus(nID, false, mainmenu);
		break;
	case IDM_EDIT_UNDO:
		SetControlStatus(nID, false, mainmenu);
		break;
	case IDM_EDIT_REDO:
		SetControlStatus(nID, false, mainmenu);
		break;
	case IDM_EDIT_CUT:
		SetControlStatus(nID, ThisDoc().IsOpen(), mainmenu);
		break;
	case IDM_EDIT_COPY:
		SetControlStatus(nID, false, mainmenu);
		break;
	case IDM_EDIT_PASTE:
		SetControlStatus(nID, false, mainmenu);
		break;
	case IDM_EDIT_FIND:
		SetControlStatus(nID, false, mainmenu);
		break;
	case IDM_EDIT_REPLACE:
		SetControlStatus(nID, false, mainmenu);
		break;
	case IDM_EDIT_DELETE:
		SetControlStatus(nID, false, mainmenu);
			break;

	}	// switch (nID)

	// Call the base function to update Tool Bar and Status Bar checkmarks.
	CFrame::OnMenuUpdate(nID);
}

/*============================================================================*/
	LRESULT CMainFrame::
OnNotify(WPARAM wParam, LPARAM lParam)                                  /*

	Process messages that controls sent to the main frame of events such
	as mouse clicks, changes in content and  selection, and  control
	background painting by sending a message to the parent.
*-----------------------------------------------------------------------------*/
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
OnUpdateStatus()							/*

	Check the status of controls whose enabled/disabled or check/uncheck
	status needs to be changed and  make changes as necessary. Caution:
	only use one SetXXXXStatus per control ID.
*-----------------------------------------------------------------------------*/
{
	// TODO: Add code here to determine status of the controls

	  // document status
	bool 	doc_is_ready 	    = m_Doc.IsOpen(),
		doc_is_dirty 	    = m_Doc.IsDirty();
	  // determine enabled status of controls
	bool 	ok_to_save          = doc_is_dirty;
	bool	ok_to_saveas        = doc_is_ready;
	bool	ok_to_print         = false;
	bool	ok_to_print_preview = false;
	bool	ok_to_undo          = false;
	bool	ok_to_redo          = false;
	bool	ok_to_cut           = doc_is_ready;
	bool	ok_to_copy          = false;
	bool	ok_to_paste         = false;
	bool	ok_to_find          = false;
	bool	ok_to_replace       = false;
	bool	ok_to_delete        = false;

	  // set the control button status determined above
	SetControlStatus(IDM_FILE_SAVE, 	ok_to_save, 	      both);
	SetControlStatus(IDM_FILE_SAVEAS, 	ok_to_saveas, 	      both);
	SetControlStatus(IDM_FILE_PRINT, 	ok_to_print, 	      both);
	SetControlStatus(IDM_FILE_PRINT_PREVIEW, ok_to_print_preview, both);
	SetControlStatus(IDM_EDIT_UNDO, 	ok_to_undo, 	      both);
	SetControlStatus(IDM_EDIT_REDO, 	ok_to_redo,	      both);
	SetControlStatus(IDM_EDIT_CUT, 		ok_to_cut, 	      both);
	SetControlStatus(IDM_EDIT_COPY, 	ok_to_copy, 	      both);
	SetControlStatus(IDM_EDIT_PASTE, 	ok_to_paste, 	      both);
	SetControlStatus(IDM_EDIT_FIND, 	ok_to_find, 	      both);
	SetControlStatus(IDM_EDIT_REPLACE, 	ok_to_replace, 	      both);
	SetControlStatus(IDM_EDIT_DELETE, 	ok_to_delete, 	      toolbar);
}

/*============================================================================*/
	void CMainFrame::
PreCreate(CREATESTRUCT& cs)                                             /*

	Set cs members to select window frame parameters desired. This gets
	executed before CView::PreCreate(). Use the deserialized position
	 and  size information to set the frame positioning and  size.
*-----------------------------------------------------------------------------*/
{
	  // do the base class stuff
	CFrame::PreCreate(cs);
	cs.x  = m_win_x;  // set to deserialized or default values
	cs.y  = m_win_y;
	cs.cx = m_win_width;
	cs.cy = m_win_height;
	   // specify a title bar and  border with a window-menu on the title bar
	cs.style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE
	    | WS_MINIMIZEBOX    // adds the minimize box
	    | WS_MAXIMIZEBOX    // adds the maximize box
	    | WS_THICKFRAME     // same as WS_SIZEBOX, enables resizing
	    ;
}

/*============================================================================*/
	BOOL CMainFrame::
PreTranslateMessage(MSG& Msg)						/*

	Pretranslates the system message uMsg from the mainframe message loop
	before they are dispatched to theTranslateMessage and  DispatchMessage
	Windows functions in the message loop and  before they are dispached
	to the PreTranslateMessage function of CWnd. MSG contains the message
	to process. Return a nonzero if the message was translated and  should
	not be dispatched; return 0 if the message was not translated  and
	should be dispatched.
*-----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(Msg);
//	HWND   hwnd	= Msg.hwnd;  
//	UINT   message	= Msg.message;  
//	WPARAM wParam	= Msg.wParam;  
//	LPARAM lParam	= Msg.lParam;  
//	DWORD  time	= Msg.time;  
//	CPoint  pt	= Msg.pt;  

	  // return 0 if the message was not handled here
	return 0;
}

/*============================================================================*/
        void CMainFrame::
Serialize(CArchive &ar)                                               /*

        Called serialize or deserialize the frame to and  from the archive ar,
        depending on the sense of IsStoring(). 
*-----------------------------------------------------------------------------*/
{
	  // perform loading or storing
        if (ar.IsStoring())
        {
			  // each item serialized is written to the archive
			  // file as a char stream of the proper length,
			  // preceded by that length. In some cases, other forms of
			  // data are saved, from with the primary items are then
			  // reconstructed.

			  // save current window placement information
			ZeroMemory(&m_Wndpl, sizeof(WINDOWPLACEMENT));
			m_Wndpl.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(m_Wndpl);
			ArchiveObject w(&m_Wndpl, m_Wndpl.length);
			ar << w;
			  // save the base class frame status and  tool bar switches:
			  // these control the display of the StatusBar and  ToolBar

			BOOL showbar = GetStatusBar().IsWindow() && GetStatusBar().IsWindowVisible();
			ar << showbar;
			showbar = GetToolBar().IsWindow() && GetToolBar().IsWindowVisible();
			ar << showbar;
			  // save MRU list and view (including scrollbars)
			ar << m_MRU;
			ar << m_View;
		}
        else    // recovering
        {
			  // each item deserialized from the archive is
			  // retrieved by first reading its length and  then
			  // loading in that number of bytes into the data
			  // item saved in the archive, as above. Some items require
			  // additional converstion procedures, as shown below.

			  // recover window frame placement, but do not invoke
			  // SetWindowPlacement(), as the window is not yet created.
			ArchiveObject w(&m_Wndpl, sizeof(WINDOWPLACEMENT));
			ar >> w;
			  // recover frame status and  tool bar base class switches
			BOOL showbar;
			ar >> showbar;
			ShowStatusBar(showbar);
			ar >> showbar;
			ShowToolBar(showbar);
			  // no exception having been raised, set frame parameters
			RECT rc = m_Wndpl.rcNormalPosition;
			m_win_x = rc.left;
			m_win_y = rc.top;
			m_win_width  = rc.right  - rc.left;
			m_win_height = rc.bottom - rc.top;
			  // load MRU and View parameters
			ar >> m_MRU;
			ar >> m_View;
        }
}

/*============================================================================*/
	BOOL CMainFrame::
SetControlStatus(UINT nID, bool status, ControlBars which)           	/*

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
SetCursorShape(CursorShape shape)	             			/*

	Set the main frame cursor to the new shape and  return the old one.
*-----------------------------------------------------------------------------*/
{
	m_cursor_shape =  shape;
	m_help_mode = (shape == help);
	m_hCursor = ::LoadCursor(0,
	    shape == arrow ? IDC_ARROW :
	    shape == help  ? IDC_HELP  :
	    shape == cur_wait  ? IDC_WAIT  :
	    /* else */       IDC_NO);
	if (TheView().GetHwnd())
		TheView().SetCursorShape(shape);
}

/*============================================================================*/
	 void CMainFrame::
SetStatusbarMsg(CString status)             				/*

*-----------------------------------------------------------------------------*/
{
	GetStatusBar().SetPartText(0, status.c_str());
}

 /*============================================================================*/
 void CMainFrame::
	 SetupMenuIcons()                                                          /*

Called from the CFrame::OnCreate() function to load the menu icons.
*-----------------------------------------------------------------------------*/
 {
	   // add the default set of menu icons from the toolbar
	 CFrame::SetupMenuIcons();

	   // add menu icons for color and  font choice items
	 AddMenuIcon(IDM_COLOR_CHOICE, TheApp().LoadIcon(IDM_COLOR_CHOICE));
	 AddMenuIcon(IDM_FONT_CHOICE, TheApp().LoadIcon(IDM_FONT_CHOICE));
 }

/*============================================================================*/
	void CMainFrame::
SetupToolBar()                                                          /*

	Called from the CFrame::CreateToolBar() function to load the toolbar
	bitmaps, to connect the tool bar buttons to Resource IDs of the
	toolbar buttons, and  to define the order of appearance of the buttons
	on the toolbar at runtime.
*-----------------------------------------------------------------------------*/
{
 	  // Connect button IDs to button icons, show enabled status,  and
	  // give the explicit image index iImage of each button in the bitmap.
	  // Add the toolbar buttons in the order they are to appear at runtime.
	AddToolBarButton(IDM_FILE_NEW,     TRUE,  0, 0);
	AddToolBarButton(IDM_FILE_OPEN,    TRUE,  0, 1);
	AddToolBarButton(IDM_FILE_SAVE,    TRUE,  0, 2);
	AddToolBarButton(0);  // Separator
	AddToolBarButton(IDM_EDIT_CUT,     TRUE,  0, 3);
	AddToolBarButton(IDM_EDIT_COPY,    FALSE, 0, 4);
	AddToolBarButton(IDM_EDIT_PASTE,   FALSE, 0, 5);
	AddToolBarButton(0);  // Separator
	AddToolBarButton(IDM_FILE_PRINT,   FALSE, 0, 6);
	AddToolBarButton(0);  // Separator
	AddToolBarButton(IDM_HELP_CONTEXT, TRUE,  0, 7);

}

/*============================================================================*/
        void CMainFrame::
SetWindowTitle(const CString &docpath /* = _T("") */)			/*

	Set the window title to the application base title plus the document
	file name.
*-----------------------------------------------------------------------------*/
{
	CString s = TheApp().m_sAppName + _T(":   ") + docpath;
	SetTitle(s);
	OnUpdateStatus();
	UpdateFrame();
}

/*============================================================================*/
	void CMainFrame::
UpdateFrame()								/*

	Invalidate the frame and redraw the window.
*-----------------------------------------------------------------------------*/
{
	OnUpdateStatus();
	Invalidate();
	UpdateWindow();
}

/*============================================================================*/
	LRESULT CMainFrame::
WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)                        /*

	This is the main frame message loop. By default, it handles the
	normal housekeeping functions (see Win32++\include\frame.h).
*-----------------------------------------------------------------------------*/
{
	switch (uMsg)
	{
		// Add case statements for each message to be handled here

	    case WM_SETCURSOR:
		if (m_help_mode)
		{
			SetCursor(m_hCursor);
			return TRUE;
		}
		break;

	    case WM_SYSCOMMAND:
	    {
		switch (LOWORD(wParam))
		{
		    case SC_MINIMIZE:
		    	if (m_help_mode)
				return OnCommandHelp(LOWORD(wParam),
				    HIWORD(wParam));

			  // save current window placement data before minimize
			m_Wndpl.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(m_Wndpl);
			break;

		    case SC_MAXIMIZE:
		    	if (m_help_mode)
				return OnCommandHelp(LOWORD(wParam),
				    HIWORD(wParam));

			  // save current window placement data before maximize
			m_Wndpl.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(m_Wndpl);
			break;

		    case SC_RESTORE:
			  // restore window parameters before minimize/maximize
			m_Wndpl.length = sizeof(WINDOWPLACEMENT);
			  // if restoring from maximized state, restore
			  // normal placement indicators
			if (IsZoomed())
			{
				m_Wndpl.flags  = WPF_SETMINPOSITION;
				m_Wndpl.showCmd = SW_SHOWNORMAL;
			}
			return SetWindowPlacement(m_Wndpl);

		    case SC_CLOSE:
			  // OnFileExit() or the [X] button brought us here,
			  // but we don't close down if in context help mode
	    		if (m_help_mode)
				return OnCommandHelp(LOWORD(wParam),
				    HIWORD(wParam));
			break;  // let default process this further
		}
	    }
	}
	  // pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

