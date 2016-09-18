/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]              (MainFrm.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description: Implementation of the CMainFrame class for the
	CommonDialogs demonstration sample application using the Win32++ Windows
	interface classes, Copyright (c) 2005-2016 David Nash, under permissions
	granted therein.

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

 	Acknowledgement:
		The author would like to thank and acknowledge the advice,
		critical review, insight, and assistance provided by David Nash
		in the development of this work.

	Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Implementation of the CMainFrame class

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"
#include <io.h>

/*******************************************************************************

	Macros and Local (static) default constants          		*/

  // the (fixed) window dimensions : {left, top, width, height}
static const RECT rcWindowPlacement = {100, 100, 780, 620};

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
	m_win_x   = rcWindowPlacement.left;
	m_win_y   = rcWindowPlacement.top;
	m_width   = rcWindowPlacement.right;
	m_height  = rcWindowPlacement.bottom;
	m_nMaxMRU = 0;
	m_bWrapText = TRUE;
	ZeroMemory(&m_Wndpl, sizeof(WINDOWPLACEMENT));
	  // Set m_View as the view window of the frame
	SetView(m_View);
	  // define the context help topics and controls they service
	SetContextHelpMessages();
}

/*============================================================================*/
	void CMainFrame::
ConnectAppHelp() 							/*

	Register the AppHelp object help .chm file name and the AppHelp About
	Box credits information string.
*-----------------------------------------------------------------------------*/
{
	  // inform context help of its .chm document name
	m_AppHelp.ConnectAppHelp(theApp.GetHelpFile(),
	    theAppGlobal.GetAboutBoxInfo());
}

/*============================================================================*/
	BOOL CMainFrame::
DoContextHelp(WPARAM wParam)						/*

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
	if (!m_AppHelp.OnHelp(wParam))
		return FALSE;

	m_hCursor = ::LoadCursor(NULL, IDC_ARROW);
	Invalidate();
	return TRUE;
}

/*============================================================================*/
	void CMainFrame::
EmptyMRUList()                                                          /*

	Remove all entries from the MRU list and  display the empty list
	on the MRU menu.
*-----------------------------------------------------------------------------*/
{
	  // use a separate list to get the entries
	const std::vector<CString>& MRUEntries = GetMRUEntries();
	  // then remove items from the MRU list one by one
	std::vector<CString>::const_iterator it;
	for (it = MRUEntries.begin(); it != MRUEntries.end(); ++it)
		RemoveMRUEntry(*it);
}

/*============================================================================*/
	BOOL CMainFrame::
EngageContextHelp() 							/*

	Set the context help mode on and set cursors to the help cursor.
*-----------------------------------------------------------------------------*/
{
	  // if context help is already active, no need to engage further
	if (m_AppHelp.IsActive())
		return FALSE;

	m_View.SetFocus();
	  // set the cursor(s) in all controls to help, and show immediately
	m_hCursor = ::LoadCursor(NULL, IDC_HELP);
	SendMessage(WM_SETCURSOR, 0, 0);
    	  // set help mode on
    	m_AppHelp.EngageContextHelp();
	return TRUE;
}

/*============================================================================*/
	void CMainFrame::
GetCtlColors(UINT nCtlColor, UINT nID, UINT& fg, UINT& bk,  UINT& bg) /*

	Return the text foreground fg, text background bk, and control
	background bg indexes for the control class nCtlColor whose color values
	are to be found in the color table at their respective color indexes.
*-----------------------------------------------------------------------------*/
{
	switch (nCtlColor)
	{
	    case WM_CTLCOLORDLG:
		fg = DlgTxFg;
		bk = DlgTxBg;
    		bg = DlgBg;
		break;

	    case WM_CTLCOLORBTN:
	    {
		  // As illustrated here, we can handle each button separately
		  // according to its nID. For the OK button case
	    	if (nID == IDOK)
	    	{
			fg = OKTxFg;
	    		bk = OKTxBg;
	    		bg = OKBg;
		}
		else
		{
			fg = BtnTxFg;
			bk = BtnTxBg;
	    		bg = BtnBg;
		}
		// ... ditto for other buttons
		break;
	    }

	    case WM_CTLCOLOREDIT:
		fg = EdtTxFg;
		bk = EdtTxBg;
    		bg = EdtBg;
		break;

	    case WM_CTLCOLORLISTBOX:
		fg = LBxTxFg;
		bk = LBxTxBg;
    		bg = LBxBg;
		break;

	    case WM_CTLCOLORSCROLLBAR:
		fg = SclTxFg;
		bk = SclTxBg;
    		bg = SclBg;
		break;

	    case WM_CTLCOLORSTATIC:
		fg = StcTxFg;
		bk = StcTxBg;
    		bg = StcBg;
		break;

	  // if there are custom colors for some controls, change them here

	    default:
		fg = bk = bg = DfltClr;
		break;
	}
}

/*============================================================================*/
	void CMainFrame::
InitCtlColors()								/*

	Populate the color table with the initial ctl_color triplets used in the
	list box of the CColorDialog object. These values are displayed in the
	controls on first execution of this program, and are overwritten by
	deserialization in subsequent executions.

	Note: Not all listed colors are actually displayed in this demo program.
	Unused color entries have been commented out or eliminated; However, on
	doing so, it is necessary to eliminate references elsewhere in this
	program to the nIDs of removed entries.
*-----------------------------------------------------------------------------*/
{
//	m_ColorChoice.AddColorChoice(DlgTxFg, _T("o  Dialog text FG"),
//	    COLOR_WHITE);
//	m_ColorChoice.AddColorChoice(DlgTxBg, _T("o  Dialog text BG"),
//	    COLOR_BLUE);
	m_ColorChoice.AddColorChoice(DlgBg, _T("o  Dialog BG"),
	    COLOR_LT_BLUE);

//	m_ColorChoice.AddColorChoice(BtnTxFg, _T("o  Button text FG"),
//	    COLOR_BLACK);
//	m_ColorChoice.AddColorChoice(BtnTxBg, _T("o  Button text BG"),
//	    COLOR_CYAN);
//	m_ColorChoice.AddColorChoice(BtnBg, _T("o  Button BG"),
//	    COLOR_LT_CYAN);

	m_ColorChoice.AddColorChoice(OKTxFg, _T("o  OK text FG"),
	    COLOR_BLACK);
	m_ColorChoice.AddColorChoice(OKTxBg, _T("o  OK text BG"),
	    COLOR_YELLOW);
	m_ColorChoice.AddColorChoice(OKBg, _T("o  OK BG"),
	    COLOR_LT_YELLOW);

//	m_ColorChoice.AddColorChoice(EdtTxFg, _T( "o  Edit text FG"),
//	    COLOR_WHITE);
//	m_ColorChoice.AddColorChoice(EdtTxBg, _T("o  Edit text BG"),
//	    COLOR_RED);
//	m_ColorChoice.AddColorChoice(EdtBg, _T("o  Edit control BG"),
//	    COLOR_LT_RED);

//	m_ColorChoice.AddColorChoice(LBxTxFg, _T("o  List box text FG"),
//	    COLOR_BLACK);
//	m_ColorChoice.AddColorChoice(LBxTxBg, _T("o  List box text BG"),
//	    COLOR_GREEN);
//	m_ColorChoice.AddColorChoice(LBxBg, _T("o  List box BG"),
//	    COLOR_LT_GREEN);

//	m_ColorChoice.AddColorChoice(SBTxFg, _T("o  Status bar text FG"),
//	    COLOR_WHITE);
//	m_ColorChoice.AddColorChoice(SBTxBg, _T("o  Status bar text BG"),
//	    COLOR_MAGENTA);
	m_ColorChoice.AddColorChoice(SBBg, _T("o  Status bar BG"),
	    GetStatusBarTheme().clrBkgnd1); // use current theme

//	m_ColorChoice.AddColorChoice(SclTxFg, _T("o  Scroll bar text FG"),
//	    COLOR_WHITE);
//	m_ColorChoice.AddColorChoice(SclTxBg, _T("o  Scroll bar text BG"),
//	    COLOR_MAGENTA);
//	m_ColorChoice.AddColorChoice(SclBg, _T("o  Scroll bar BG"),
//	    COLOR_LT_MAGENTA);

//	m_ColorChoice.AddColorChoice(StcTxFg, _T("o  Static box text FG"),
//	    COLOR_BLACK);
//	m_ColorChoice.AddColorChoice(StcTxBg, _T("o  Static box text BG"),
//	    COLOR_YELLOW);
//	m_ColorChoice.AddColorChoice(StcBg, _T("o  Static box BG"),
//	    COLOR_LT_YELLOW);

	  // richedit controls (these are set differently than the others)
	m_ColorChoice.AddColorChoice(REdTxFg, _T("o  RichEdit text FG"),
	    COLOR_WHITE);
	m_ColorChoice.AddColorChoice(REdTxBg, _T("o  RichEdit text BG"),
	    COLOR_RED);
	m_ColorChoice.AddColorChoice(REdBg, _T("o  RichEdit BG"),
	    COLOR_LT_RED);
}

/*============================================================================*/
	void    CMainFrame::
LoadPersistentData()                                                    /*

	Recover app, mainframe, MRU, and view serialized data from previous
	execution. MRU strings that are not valid file paths are discarded.
*-----------------------------------------------------------------------------*/
{
	try
	{
		CArchive ar(theApp.GetArcvFile(), CArchive::load);
	          // deserialize in the same order as serialized
		ar >> theApp;	// for the app
		ar >> *this;	// for the mainframe and base classes
		ar >> m_View;   // for the view, including control colors
		Invalidate();   // repaint the client with recovered colors

		  // weed out any MRU entries that have disappeared
		ValidateMRU(); // remove invalid file path names
		  // the ar object closes on destruction
	}
	catch(...) // catch all exceptions in trying to load the archive
	{
		CString msg = _T("Previous settings could not be restored.\n")
		    _T("Unable to read archived values.\n");
		::MessageBox(NULL, msg, _T("Exception"), MB_OK | MB_ICONSTOP |
		    MB_TASKMODAL);
	}
}

/*============================================================================*/
	void CMainFrame::
OnColorChoice()     		                                       	/*

        Show the CListBoxDlg color dialog box to select the color of a control's
	text foreground, text background, or control background. NOTE: colors
	selected for controls in the view are painted by the OnCtlColor()
	method invoked in the WndProc() loop each time the control is redrawn.
	The staus bar color has no message to redraw it, so it is set here.
*-----------------------------------------------------------------------------*/
{
	  // set color choice help messages to go to the the main frame,
	HWND hWndOwner = theApp.GetMainWnd();
	m_ColorChoice.DoModal(hWndOwner);  // calls the base class DoModal()
	  // reset the status bar color
	UINT selection = m_ColorChoice.GetSelectedColorID();
	if (selection == SBBg)
	{
		COLORREF sb = GetSBBkColor();
		SetSBBkColor(sb);
	}
	  // reset the rich edit control color
	if (selection == REdTxFg || selection == REdTxBg || selection == REdBg)
		OnRichEditColor();
		
	UpdateControlUIState();
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
	UINT nID = LOWORD(wParam);
	  // if context help mode is active, display the help topic for wParam
	  // and return TRUE; otherwise, just pass through
	if (DoContextHelp(wParam))
		return TRUE;

	  // map all MRU file messages to one representative
	if(IDW_FILE_MRU_FILE1 <= nID &&
	    nID < IDW_FILE_MRU_FILE1 + m_nMaxMRU)
		nID = IDW_FILE_MRU_FILE1;

	switch(nID)
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

	    case IDM_FILE_PAGESETUP:
		OnFilePageSetup();
		return TRUE;

	    case IDM_FILE_PREVIEW:
	    	OnFilePreview();
		return TRUE;

	    case IDM_FILE_PRINT:
	    	OnFilePrint();
		return TRUE;

	    case IDM_FILE_EXIT:
	    	m_Doc.OnCloseDoc();
		  // Issue a close request to the frame
		SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
		return TRUE;

	    case IDM_EDIT_UNDO:
	    	m_Doc.OnUndo();
 	    	UpdateControlUIState();
		return TRUE;

	    case IDM_EDIT_REDO:
	    	m_Doc.OnRedo();
 	    	UpdateControlUIState();
		return TRUE;

	    case IDM_EDIT_CUT:
 	    	m_Doc.OnCut();
 	    	UpdateControlUIState();
		return TRUE;

	    case IDM_EDIT_COPY:
	    	m_Doc.OnCopy();
 	    	UpdateControlUIState();
		return TRUE;

	    case IDM_EDIT_PASTE:
	    	m_Doc.OnPaste();
 	    	UpdateControlUIState();
		return TRUE;

	    case IDM_EDIT_DELETE:
	    	m_Doc.OnDelete();
 	    	UpdateControlUIState();
		return TRUE;

	    case IDM_EDIT_FIND:
	    	OnEditFind();
 	    	UpdateControlUIState();
		return TRUE;

	    case IDM_EDIT_REPLACE:
	    	OnEditReplace();
 	    	UpdateControlUIState();
		return TRUE;

	    case IDM_BOGUS_MRU:
	    	OnBogusMRU();
	    	return TRUE;
	    	
	    case IDW_ABOUT:         // invoked by F1 and Help->About menu item
	    	return m_AppHelp.OnHelpAbout();

	    case IDC_HELP_COMDLG:  // Handle help requests from common dialogs
	    	return m_AppHelp.OnHelpID(LOWORD(lParam)); 

	    case IDM_HELP_CONTENT:	// invoked by Help->Content menu item
	    	return m_AppHelp.OnHelpID(nID);

	    case IDM_HELP_CONTEXT: // invoked by Shift+F1 and Help button
		return EngageContextHelp(); // initiates context help mode

	    case IDW_VIEW_TOOLBAR:
	    	OnViewToolBar(); // toggle tool bar
		return TRUE;

	    case IDW_VIEW_STATUSBAR:
	    	OnViewStatusBar(); // toggle status bar
		return TRUE;

	    case IDM_RICHEDWRAP:
	    	OnWrapText();
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
	INT_PTR  CMainFrame::
OnCtlColor(HDC hDC, HWND hWnd, UINT nCtlColor)            		/*

	This member function is invoked when a child control is about to be
	drawn. It can be used to prepare the display context hDC for drawing
	the control using user-selected colors. Here, the SetTextColor() and
	SetBkColor() methods may be used with desired RGB values to set the
	text and background colors of almost all controls.  Here, hDC contains
	the handle to the display context for the child window, which may be
	temporary.  hWnd contains he handle to the control asking for the
	color, which also may be temporary. Finally, nCtlColor contains one
	of the messages specifying the type of control (see WndProc() for
	values). On termination the method returns a handle to the brush that
	is to be used for painting the control background.

	Note: This method is not to be used for the list box of a drop-down
	combo box because the drop-down list box is actually a child of the
	combo box and not a child of the window. To change the color of the
	drop-down list box, create a CComboBox with a similar OnCtlColor() that
	checks for CTLCOLOR_LISTBOX in the nCtlColor parameter. In that handler,
	the SetBkColor() member function must be used to set the background
	color for the text.
	
	It is also not used to change the colors of a rich edit control. See
	View::SetRichEditColors() for that process.
*-----------------------------------------------------------------------------*/
{
	  // declare default control colors IDs
	UINT fg = DfltClr,
	     bk = DfltClr,
	     bg = DfltClr;
	  // get the CtlColors of each control class first
	GetCtlColors(nCtlColor, ::GetDlgCtrlID(hWnd), fg, bk, bg);
	  // get the display context
	CDC dcCtl(hDC);
	dcCtl.SetTextColor(m_ColorChoice.GetTableColor(fg));
	dcCtl.SetBkColor(m_ColorChoice.GetTableColor(bk));
	m_br = m_ColorChoice.GetBrush(bg);
	  // returned brush handle must persist
        return (UINT_PTR)(HBRUSH)m_br;
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
	  // establish communications
	m_View.Register(&m_Doc);
	m_Doc.Register(this, &m_View);
	  // populate the initial control colors (will be overwritten by
	  // deserialized values)
	InitCtlColors();
	  // and set the initial flags to show the help box and all colors
	CHOOSECOLOR cc = m_ColorChoice.GetParameters();
	cc.Flags = CC_SHOWHELP | CC_FULLOPEN;
	cc.Flags |= CC_ANYCOLOR | CC_RGBINIT | CC_ENABLEHOOK;
	  // setup the CColorChoice object
	m_ColorChoice.SetParameters(cc);
	  // tell CFrame the max MRU size
	m_nMaxMRU = theAppGlobal.GetMaxMRU();
	SetMRULimit(m_nMaxMRU);
	  // set font choice help messages to go to the main frame,
	  // set the initial flags to show the help box and use the font style,
	CHOOSEFONT cf = m_FontChoice.GetParameters();
	cf.Flags |= CF_SHOWHELP | CF_USESTYLE;
	cf.lpszStyle = (LPTSTR)_T("Regular"); // initial font presumed regular
	m_FontChoice.SetParameters(cf);
	  // set the default font
	CFont f;
	f.CreatePointFont(100, _T("Courier New"));
	m_FontChoice.SetChoiceFont(f);
	m_FontChoice.SetColor(m_ColorChoice.GetTableColor(REdTxFg));
	m_View.SetEditFont(f);
	  // communicate help file name and about box contents to help object
	ConnectAppHelp();
	LoadPersistentData();
	  // set the default status bar color
	COLORREF sb = GetSBBkColor();
	SetSBBkColor(sb);
	  // set the edit box colors
	OnRichEditColor();
	return rtn;
}

/*============================================================================*/
	void CMainFrame::
OnBogusMRU()                                                            /*

	Demonstrate operation of the MRU list: for this demo, add 5 strings to
	the MRU list and open the topmost one. In an actual application, this
	function would not be present
*-----------------------------------------------------------------------------*/
{
	EmptyMRUList();
	AddMRUEntry(_T("This is MRU 5 and it is very, very, very, ")
	    _T("very, very, very, very, very, very, very, very, very, very, ")
	    _T("very, very, very long"));
	AddMRUEntry(_T("This is MRU 4"));
	AddMRUEntry(_T("This is MRU 3"));
	AddMRUEntry(_T("This is MRU 2"));
	AddMRUEntry(_T("This is MRU 1"));
	CString mru_top = GetMRUEntry(0);
	m_Doc.OnOpenDoc(mru_top);
	UpdateControlUIState();
}

/*============================================================================*/
	void CMainFrame::
OnEditFind()                                                            /*

   	Initiate the find non-modal dialog box and the messages sent to the
	CDoc::OnFindReplace() method by the CMainFrame::WndProc() message loop.
*-----------------------------------------------------------------------------*/
{
	m_FindRepDialog.SetBoxTitle(_T("Find a string..."));
	m_FindRepDialog.Create(TRUE, _T("Initial Text"), _T(""), FR_DOWN |
	    FR_ENABLEHOOK | FR_SHOWHELP, (HWND)*this);
}

/*============================================================================*/
	void CMainFrame::
OnEditReplace()                                                            /*

	Invoke the find-replace dialog and initiate the find non-modal dialog
	box and the messages sent to the CDoc::OnFindReplace() method by the
	the CMainFrame::WndProc() message loop.  Note: the replace dialog box
	does not have the direction up-down box that the find dialog box has.
	This is "by design."
*-----------------------------------------------------------------------------*/
{
	m_FindRepDialog.SetBoxTitle(_T("Find, then Replace"));
	m_FindRepDialog.Create(FALSE, _T("Initial Text"), _T("Replace Text"),
	    FR_DOWN | FR_SHOWHELP | FR_ENABLEHOOK, (HWND)*this);
}

/*============================================================================*/
	void CMainFrame::
OnFileNew()                                                            /*

	Prompt the user for a new document file name and, if valid, open a new
	document.
*-----------------------------------------------------------------------------*/
{
	MyFileDialog fd(TRUE,
	    m_Doc.GetExt(),  	 // extension defined by app
	    m_Doc.GetFilePath(), // current open file path
	    OFN_HIDEREADONLY |
	    OFN_SHOWHELP |
	    OFN_EXPLORER |
	    OFN_NONETWORKBUTTON |
	    OFN_ENABLESIZING,
	    m_Doc.GetFilter());
	fd.SetBoxTitle(_T("New document file..."));
	fd.SetDefExt(m_Doc.GetExt());
	CString msg;
	  // do not leave without a valid unused file name, unless cancelled
	while (fd.DoModal(*this) == IDOK)
 	{
 		CString new_path = fd.GetPathName();
		  // new_path should not exist
		if (::_taccess(new_path, 0x04) != 0)
		{  	  // for the demo, announce the file chosen
			msg.Format(_T("Open new document file\n    '%s'"),
			    new_path.c_str());
			::MessageBox(NULL, msg, _T("Information"),
			    MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);

			  // let the document class handle the new_path
			m_Doc.OnNewDoc(new_path);
			UpdateControlUIState();
			return;
		}
		  //
		msg.Format(_T("That document file\n    '%s'\n")
		    _T("already exists."), new_path.c_str());
		::MessageBox(NULL, msg, _T("Error"), MB_OK |
		    MB_ICONERROR | MB_TASKMODAL);
	}
	msg = _T("No name was entered, no action was taken.");
	::MessageBox(NULL, msg, _T("Information"),
	    MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
}

/*============================================================================*/
	void CMainFrame::
OnFileClose()                                                            /*

	Close the currently open document file.
*-----------------------------------------------------------------------------*/
{
	if (!m_Doc.IsOpen())
		return;
		
	CString current_file = m_Doc.GetFilePath(),
		mru_top_file = GetMRUEntry(0);
	  // these should be the same
	assert(current_file.CompareNoCase(mru_top_file) == 0);
	  // let the document class handle the closure
	m_Doc.OnCloseDoc();
	UpdateControlUIState();
}

/*============================================================================*/
	void CMainFrame::
OnFileOpen()                                                            /*

	Display the open file dialog to input the document file name and to
	open the corresponding document if that file exists.
*-----------------------------------------------------------------------------*/
{
	MyFileDialog fd(TRUE,
	    m_Doc.GetExt(),	 // extension defined by app
	    m_Doc.GetFilePath(), // current open file path
	    OFN_HIDEREADONLY |
	    OFN_SHOWHELP |
	    OFN_EXPLORER |
	    OFN_NONETWORKBUTTON |
	    OFN_FILEMUSTEXIST |	 // only exising files allowed
	    OFN_PATHMUSTEXIST |
	    OFN_ENABLEHOOK    |
	    OFN_ENABLESIZING,
	    m_Doc.GetFilter());
	fd.SetBoxTitle(_T("Open document file..."));
	fd.SetDefExt(m_Doc.GetExt());
	CString msg;
	if (fd.DoModal(*this) == IDOK)
 	{
 		CString the_path = fd.GetPathName();
		if (the_path.IsEmpty())
		    return;

		  // open the document based on this name
		m_Doc.OnOpenDoc(the_path);
		if (m_Doc.IsOpen())
			SetWindowTitle(the_path);
		UpdateControlUIState();
		return;
  	}
	msg = _T("No name was entered, no action was taken.");
	::MessageBox(NULL, msg, _T("Information"),
	    MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
}

/*============================================================================*/
	void CMainFrame::
OnFileOpenMRU(UINT nIndex)						/*

	Open the MRU file at nIndex as the next document.
*-----------------------------------------------------------------------------*/
{
	  // get the MRU entry
	CString mru_entry = GetMRUEntry(nIndex);
	m_Doc.OnOpenDoc(mru_entry);
	UpdateControlUIState();
}

/*============================================================================*/
	void CMainFrame::
OnFilePageSetup()                                                            /*

*-----------------------------------------------------------------------------*/
{
    	m_Doc.OnPageSetup();
}

/*============================================================================*/
	void CMainFrame::
OnFilePreview()                                                            /*

*-----------------------------------------------------------------------------*/
{
    	m_Doc.OnPrintPreview();
}

/*============================================================================*/
	void CMainFrame::
OnFilePrint()                                                            /*

*-----------------------------------------------------------------------------*/
{
    	m_View.OnPrintDocument();
}

/*============================================================================*/
	void CMainFrame::
OnFileSave()                                                            /*

	Save the current document.
*-----------------------------------------------------------------------------*/
{
	if (m_Doc.GetFilePath().IsEmpty())
		OnFileSaveAs();
	else
	    	m_Doc.OnSaveDoc();
}

/*============================================================================*/
	void CMainFrame::
OnFileSaveAs()                                                            /*

	Save the current document into a file named in a file dialog and make
	that file the current document.
*-----------------------------------------------------------------------------*/
{
	if (!m_Doc.IsOpen())
		return;

	  // declare the file dialog box
	MyFileDialog fd(FALSE,
	    m_Doc.GetExt(),	 // extension defined by app
	    m_Doc.GetFilePath(), // current open file path
	    OFN_HIDEREADONLY |
	    OFN_OVERWRITEPROMPT |
	    OFN_SHOWHELP |
	    OFN_EXPLORER |
	    OFN_ENABLEHOOK |
	    OFN_NONETWORKBUTTON,
	    m_Doc.GetFilter());  // filter defined by app
	fd.SetBoxTitle(_T("Save document file as"));
	CString current_path = m_Doc.GetFilePath(),
		msg;
	  // query user for the save-as file path name
	if (fd.DoModal(*this) == IDOK)
 	{	  // At this point, a file path has been chosen that is
	 	  // not empty and if it already exists has been approved by the
		   // user to be overwritten. Fetch the path from the dialog.
		CString new_path = fd.GetPathName();
		  // check if the input path is the one already open
		if (new_path.CompareNoCase(current_path) == 0)
		{         // the named paths are the same
			msg.Format(_T("Document file\n    '%s'\n is already ")
			    _T("open. No action taken"), new_path.c_str());
			::MessageBox(NULL, msg, _T("Information"),
			    MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
			return;
		}
		else
			  // save and close the current document
			m_Doc.OnCloseDoc();

		  // for the demo, announce the file chosen
		msg.Format(_T("Document file saved as:\n    '%s'."),
		    new_path.c_str());
		::MessageBox(NULL, msg, _T("Information"),
		    MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
		CopyFile(current_path, new_path, FALSE);
		if (!m_Doc.OnOpenDoc(new_path))
		{
			msg.Format(_T("Saved document file\n    '%s'")
			    _T(" could not be reopened."), new_path.c_str());
			::MessageBox(NULL, msg, _T("Information"),
			    MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
			  // reopen the current file at entry
			m_Doc.OnOpenDoc(current_path);
			UpdateControlUIState();
			return;
		}
		if (m_Doc.IsOpen())
		{
			AddMRUEntry(new_path);
			UpdateControlUIState();
		}
		return;
	}
	msg = _T("No name was entered, no action was taken.");
	::MessageBox(NULL, msg, _T("Information"),
	    MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
}

/*============================================================================*/
	void CMainFrame::
OnFontChoice()     		                                 	/*

        Select the view font typeface, characteristics, and color.
*-----------------------------------------------------------------------------*/
{
	  // set font choice help messages to go to the the main frame,
	HWND hOwnerWnd = GetApp().GetMainWnd();
          // open the dialog
	m_FontChoice.SetBoxTitle(_T("Select font for edit box"));
	CHOOSEFONT cf = m_FontChoice.GetParameters();
	cf.Flags |= CF_SCREENFONTS;
	m_FontChoice.SetParameters(cf);
	if(m_FontChoice.DoModal(hOwnerWnd))
	{
		  // bring choice elements into this view
		m_View.SetEditFont(m_FontChoice.GetChoiceFont());
    		m_ColorChoice.SetTableColor(REdTxFg, m_FontChoice.GetColor());
		  // reset the rich edit control color
		OnRichEditColor();
	}
	Invalidate();
	UpdateControlUIState();
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
	m_nMaxMRU = theAppGlobal.GetMaxMRU();
	DragAcceptFiles(TRUE);
	  // if there is a MRU item at the top of the list, use it
	  // as the name of the document to open
	CString docfile = GetMRUEntry(0),
		msg;
	msg.Format(_T("Open previous document?\n    %s"), docfile.c_str());
	if (!docfile.IsEmpty() &&  (::MessageBox(NULL, msg, _T("Question..."),
	    MB_YESNO | MB_ICONQUESTION) == IDYES))
	{
		m_Doc.OnOpenDoc(docfile);
	}
	UpdateControlUIState();
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
	UpdateControlUIState();
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
//	NMHDR* pNMH;
//	pNMH = (LPNMHDR) lParam;
//	switch (pNMH->code)
//	{
//	    default:
//	    	break;
//	}

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
	BOOL CMainFrame::
OnRichEditColor()                            /*

	Set the rich edit control colors to those found in the color table.
*-----------------------------------------------------------------------------*/
{
	COLORREF txfg = m_ColorChoice.GetTableColor(REdTxFg),
		 txbg = m_ColorChoice.GetTableColor(REdTxBg),
		 bg   = m_ColorChoice.GetTableColor(REdBg);
	m_View.SetRichEditColors(txfg, txbg, bg);
	return TRUE;
}

/*============================================================================*/
	void CMainFrame::
OnTerminate()								/*

	Perform whatever functions are necessary, other than Serialize(), as
	it is invoked in response to the WM_CLOSE message that is sent when
	the frame is close.
*-----------------------------------------------------------------------------*/
{
	m_Doc.OnCloseDoc();
}

/*============================================================================*/
	void CMainFrame::
OnWrapText()								/*

	Description
*-----------------------------------------------------------------------------*/
{
	m_bWrapText = !m_bWrapText;
	GetREView().SetWrapping(m_bWrapText);
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
		ar << *this;    // for the mainframe and base classes
		ar << m_View;   // for the view, including control colors
	}
	catch (const CWinException &e)  // catch all std::exception events
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
		::MessageBox(NULL, msg, _T("Unregistered Exception"), MB_OK |
		    MB_ICONSTOP | MB_TASKMODAL);
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
                  // save font parameters
		ar << m_FontChoice;
		  // save regular control class colors
		ar << m_ColorChoice;
		ar << m_bWrapText;
		  // record the number of MRU entries to write
		size_t i, nMRU = GetMRUSize();
		ar << nMRU;
		  // save this many entries (don't use a copied list)
		for (i = 0; i < nMRU; ++i)
		{
			ar << GetMRUEntries()[i];
		}
	}
        else    // recovering
        {
  		  // recover frame status and  tool bar base class switches
		BOOL showbar;
		ar >> showbar;
		ShowStatusBar(showbar);
		ar >> showbar;
		ShowToolBar(showbar);
                  // recover font parameters
		ar >> m_FontChoice;
		m_View.SetFont(m_FontChoice.GetChoiceFont(), TRUE);
		  // set font elements into view
		m_View.SetEditFont(m_FontChoice.GetChoiceFont());
    		m_ColorChoice.SetTableColor(EdtTxFg, m_FontChoice.GetColor());
		  // recover colors
		ar >> m_ColorChoice;
		  // reset the status bar color
		COLORREF sb = GetSBBkColor();
		SetSBBkColor(sb);
		ar >> m_bWrapText;
		GetREView().SetWrapping(m_bWrapText);
		  // read MRU values from archive
		EmptyMRUList();
		  // use dummy vector in case an exception occurs
		std::vector<CString> vMRUEntries;
		  // extract all the MRU entries that were archived to preserve
		  // the archive for subsequent use
		size_t i, nMRU;
		ar >> nMRU; // the number of entries to read in
		CString s;
		for (i = 0; i < nMRU; ++i)
		{
			ar >> s;
			if (i < m_nMaxMRU)  // keep only those within the limit
				vMRUEntries.push_back(s);
		}
		  // all successfully read in, so store them LIFO order into
		  // the MRU list for proper display
		for (i = 0; i < nMRU; ++i)
		{
			s = vMRUEntries[nMRU - 1 - i];
			AddMRUEntry(s);
		}
        }
}

/*============================================================================*/
	BOOL CMainFrame::
SetCheckStatus(UINT nID, BOOL bCheck, ControlBars where)		/*

*-----------------------------------------------------------------------------*/
{

	BOOL ok = FALSE;
	if (where == mainmenu || where == both)
	{
		if (bCheck > 1)
			ok = (GetFrameMenu().EnableMenuItem(nID, MF_BYCOMMAND |
			    MF_GRAYED) != 0xFFFFFFFF);
		else
			ok = (GetFrameMenu().CheckMenuItem(nID, MF_BYCOMMAND |
			    (bCheck ? MF_CHECKED : MF_UNCHECKED)) != 0xFFFFFFFF);
	}
	
	if (!ok)
		ok = TRUE;
	if (where == mainmenu || where == both)
	{
		if (bCheck > 1)
			ok = GetToolBar().SetButtonState(nID, TBSTATE_INDETERMINATE);
		else
		{
			GetToolBar().CheckButton(nID, bCheck);
			ok = TRUE;
		}
	}
	return ok;
}

/*============================================================================*/
	void CMainFrame::
SetContextHelpMessages() 						/*

	Define the set of all context help topics to be displayed for each of
	the control identifiers that are serviced in this app, except for those
	for CCommonDialog help boxes and the frame SC_MAXIMIZE, SC_MINIMIZE,
	and SC_CLOSE boxes (these are defaults in the AppHelp class).
*-----------------------------------------------------------------------------*/
{
	  // define the tool bar button and menu item topics
	m_AppHelp.AddHelpTopic(IDM_FILE_NEW, 	   _T("NewDocument"));
	m_AppHelp.AddHelpTopic(IDM_FILE_OPEN, 	   _T("OpenExistingDocument"));
	m_AppHelp.AddHelpTopic(IDM_FILE_SAVE, 	   _T("SaveCurrentDocument"));
	m_AppHelp.AddHelpTopic(IDM_FILE_SAVEAS,    _T("SaveAsAnotherDocument"));
	m_AppHelp.AddHelpTopic(IDM_FILE_CLOSE, 	    _T("CloseCurrentDocument"));
	m_AppHelp.AddHelpTopic(IDM_FILE_PAGESETUP,  _T("PageSetupForPrintout"));
	m_AppHelp.AddHelpTopic(IDM_FILE_PRINT, 	    _T("PrintDocument"));
	m_AppHelp.AddHelpTopic(IDM_FILE_PREVIEW,    _T("PreviewPrintout"));
	m_AppHelp.AddHelpTopic(IDM_FILE_EXIT,  	    _T("ExitTerminateProgram"));
	m_AppHelp.AddHelpTopic(IDM_EDIT_UNDO, 	    _T("UndoFunction"));
	m_AppHelp.AddHelpTopic(IDM_EDIT_REDO, 	    _T("RedoFunction"));
	m_AppHelp.AddHelpTopic(IDM_EDIT_CUT, 	    _T("CutFunction"));
	m_AppHelp.AddHelpTopic(IDM_EDIT_COPY, 	    _T("CopyFunction"));
	m_AppHelp.AddHelpTopic(IDM_EDIT_PASTE, 	    _T("PasteFunction"));
	m_AppHelp.AddHelpTopic(IDM_EDIT_DELETE,     _T("DeleteFunction"));
	m_AppHelp.AddHelpTopic(IDM_EDIT_FIND, 	    _T("FindInDocument"));
	m_AppHelp.AddHelpTopic(IDM_EDIT_REPLACE,    _T("ReplaceInDocument"));
	m_AppHelp.AddHelpTopic(IDM_HELP_CONTENT,    _T("Introduction"));
	m_AppHelp.AddHelpTopic(IDM_HELP_CONTEXT,    _T("Welcome"));
	m_AppHelp.AddHelpTopic(IDM_COLOR_CHOICE,    _T("ColorChoiceFunction"));
	m_AppHelp.AddHelpTopic(IDM_FONT_CHOICE,     _T("FontChoiceFunction"));
	  // define client area controls topics
	m_AppHelp.AddHelpTopic(IDC_RICHEDITBOX,	    _T("EditBoxUsage"));
	m_AppHelp.AddHelpTopic(IDOK, 		    _T("OKButtonUsage"));
	  // define common dialog help button topics
	m_AppHelp.AddHelpTopic(IDM_HELP_COLORDLG,    _T("ColorDialogHelp"));
	m_AppHelp.AddHelpTopic(IDM_HELP_FILEDLG_OPEN, _T("FileDialogOpenHelp"));
	m_AppHelp.AddHelpTopic(IDM_HELP_FILEDLG_NEW, _T("FileDialogNewHelp"));
	m_AppHelp.AddHelpTopic(IDM_HELP_FILEDLG_SAVEAS, _T("FileDialogSaveAsHelp"));
	m_AppHelp.AddHelpTopic(IDM_HELP_FONTDLG,    _T("FontChoiceDialogHelp"));
	m_AppHelp.AddHelpTopic(IDM_HELP_PAGESETDLG, _T("PageSetupDialogHelp"));
	m_AppHelp.AddHelpTopic(IDM_HELP_PRINTDLG,   _T("PrinterDialogHelp"));
	m_AppHelp.AddHelpTopic(IDM_HELP_FINDDLG,    _T("FindTextDialogHelp"));
	m_AppHelp.AddHelpTopic(IDM_HELP_REPLACEDLG, _T("ReplaceTextDialogHelp"));
	m_AppHelp.AddHelpTopic(IDM_HELP_LIST_BOX,   _T("ListBoxDialogHelp"));
}

/*============================================================================*/
	BOOL CMainFrame::
SetEnableStatus(UINT nID, BOOL status, ControlBars which)           	/*

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
	 void CMainFrame::
SetStatusbarMsg(CString status)             				/*

	Write the status message on the status bar.
*-----------------------------------------------------------------------------*/
{
	GetStatusBar().SetPartText(0, status);
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
SetWindowTitle(const CString& docfile)					/*

	Put the app title and docfile name in the main window text. Limit the
	displayed text length to that of the frame.
*-----------------------------------------------------------------------------*/
{
	  // the app title is in resources.rc
	CString appTitle = LoadString(IDW_MAIN);
	  // compute text lengths
	int applen = appTitle.GetLength(),
	    doclen = docfile.GetLength();
	  // only count "\\" as one character
	for (int i = 0; i < doclen; )
	{
		i = docfile.Find(_T("\\"), i);
		if (i < 0)
			break;

		--doclen;
		i++;
	}
	  // form the caption title
	if (!docfile.IsEmpty())
		appTitle  += _T(":    ") + docfile;;
	  // compute maximum characters in the caption and mid point of docfile
	int width = m_View.GetClientRect().Width(),
	    maxlen = (10 * width) / 68 - 3,  // empirical value minus "..."
	    mid    = applen + (maxlen - applen - 1) / 2;
	  // limit the caption line to that which will fit
	if (appTitle.GetLength() > maxlen)
	{
		  // eliminate middle if too long
		appTitle.Delete(mid, appTitle.GetLength() - maxlen);
		appTitle.Insert(mid, _T("..."));
	}
	SetWindowText(appTitle);
}

/*============================================================================*/
	void CMainFrame::
UpdateControlUIState()							/*

	Check the status of controls whose enabled/disabled or check/uncheck
	status needs to be changed and make changes as necessary. Caution:
	only use one SetXXXXStatus per control ID.
*-----------------------------------------------------------------------------*/
{
	// TODO: Add and modify code here to implement this member

	  // document status
	BOOL 	doc_is_ready = m_Doc.IsOpen();

	  // determine enabled status of controls (TODO: redo for actual app)
	BOOL	ok_to_cut         = doc_is_ready && GetREView().IsSelected();
	BOOL	ok_to_copy        = doc_is_ready && GetREView().IsSelected();
	BOOL	ok_to_delete      = doc_is_ready && GetREView().IsSelected();
	BOOL	ok_to_find        = doc_is_ready;
	BOOL	ok_to_paste       = doc_is_ready && GetREView().CanPaste();
	BOOL	ok_to_print       = doc_is_ready;
	BOOL	ok_to_preview     = FALSE;   // doc_is_ready; TBD
	BOOL	ok_to_page_setup  = TRUE;
	BOOL	ok_to_redo        = GetREView().CanRedo();
	BOOL	ok_to_replace     = doc_is_ready;
	BOOL 	ok_to_save        = doc_is_ready && GetREView().GetModify();
	BOOL	ok_to_saveas      = doc_is_ready;
	BOOL	ok_to_undo        = GetREView().CanUndo();
	BOOL    ok_to_close       = doc_is_ready;
	
 	  // set the control button status
	SetEnableStatus(IDM_EDIT_CUT, 		ok_to_cut, 	   both);
	SetEnableStatus(IDM_EDIT_COPY, 		ok_to_copy, 	   both);
	SetEnableStatus(IDM_EDIT_DELETE, 	ok_to_delete, 	   both);
	SetEnableStatus(IDM_EDIT_FIND, 		ok_to_find, 	   both);
	SetEnableStatus(IDM_FILE_PAGESETUP,	ok_to_page_setup,  mainmenu);
	SetEnableStatus(IDM_EDIT_PASTE, 	ok_to_paste,	   both);
	SetEnableStatus(IDM_FILE_PRINT, 	ok_to_print,  	   both);
	SetEnableStatus(IDM_FILE_PREVIEW,       ok_to_preview,     mainmenu);
	SetEnableStatus(IDM_EDIT_REDO, 		ok_to_redo,	   both);
	SetEnableStatus(IDM_EDIT_REPLACE, 	ok_to_replace, 	   both);
	SetEnableStatus(IDM_FILE_CLOSE, 	ok_to_close,       mainmenu);
	SetEnableStatus(IDM_FILE_SAVE, 		ok_to_save,        both);
	SetEnableStatus(IDM_FILE_SAVEAS, 	ok_to_saveas, 	   both);
	SetEnableStatus(IDM_EDIT_UNDO, 		ok_to_undo, 	   both);
	SetCheckStatus(IDM_RICHEDWRAP, 		m_bWrapText, 	   mainmenu);
}

/*============================================================================*/
	void CMainFrame::
UpdateMRUMenu()                          				/*

	Override the base class method to truncate long MRU list entries using
	a '...' splice at the midpoint, rather than at the beginning, as does
	the base class. This will be called by the framework whenever a menu
	element is changed.
*-----------------------------------------------------------------------------*/
{
	  // find in the leftmost submenu (i.e., the one with index 0)
	CMenu fileMenu = GetFrameMenu().GetSubMenu(0);
	  // compute the index of the last entry in the MRU list
	int iLast = (int)MIN(GetMRUSize(), m_nMaxMRU) -  1;
	  // if there is no leftmost submenu, or if there are no entries to
	  // post, or if we cannot modify the first entry to indicate an empty
	  // MRU list, we cannot proceed
	if (!fileMenu.GetHandle())
	{	  // just refresh the frame menu bar and  leave
		DrawMenuBar();
		return;
	}
	   // insert the empty MRU list label in the top slot
	fileMenu.ModifyMenu(IDW_FILE_MRU_FILE1, MF_BYCOMMAND,
	    IDW_FILE_MRU_FILE1, _T("Recent Files"));
	fileMenu.EnableMenuItem(IDW_FILE_MRU_FILE1, MF_BYCOMMAND | MF_GRAYED);

	  // remove all the other MRU Menu entries
	for (int i = IDW_FILE_MRU_FILE2; i <= IDW_FILE_MRU_FILE1 +
	    (int)m_nMaxMRU; ++i)
		fileMenu.DeleteMenu(i, MF_BYCOMMAND);
	  // if the list is not empty, there's more to do
	if (iLast >= 0)
	{
		  // create the MRU "show" list, which contains only strings
		  // of limited length, chars removed at the midpoint, as needed
		int 	maxlen = MAX_MENU_STRING - 10,
			mid    = maxlen / 2;
		CString strMRUShow[16];
		for (int i = 0; i <= iLast; i++)
		{
			CString s = GetMRUEntries()[i];
			if (s.GetLength() > maxlen)
			{
				  // eliminate middle if too long
				s.Delete(mid, s.GetLength() - maxlen);
				s.Insert(mid, _T("..."));
			}
			// Prefix with its number
			CString v;
			v.Format(_T("%d "), i + 1);
			strMRUShow[i] = v + s;
		}

		  // display the MRU items: start by replacing the first item
		  // in the the list with the last MRU item
		fileMenu.ModifyMenu(IDW_FILE_MRU_FILE1, MF_BYCOMMAND,
		    IDW_FILE_MRU_FILE1 + iLast, strMRUShow[iLast]);
		  // now insert the remaining items in reverse order, starting
		  // at the next-to-iLast entry and  pushing all the others
		  // down in the menu (entries thus end up in the correct order)
		for (int j = iLast - 1 ; j >= 0; iLast--, j--)
			fileMenu.InsertMenu(IDW_FILE_MRU_FILE1 + iLast,
			    MF_BYCOMMAND, IDW_FILE_MRU_FILE1 + j,
			    strMRUShow[j]);
	}
	  // refresh the frame menu bar and  leave
	DrawMenuBar();
}

/*============================================================================*/
	void CMainFrame::
ValidateMRU()                                                 		/*
	Validate the that the MRU list entries, if there are any, correspond
	to actual file paths. Remove any that do not.
*-----------------------------------------------------------------------------*/
{
	  // get a copy of the MRU list entries
	const std::vector<CString>& MRUEntries = GetMRUEntries();
	  // check them one by one as a valid file path
	std::vector<CString>::const_iterator it;
	for (it = MRUEntries.begin(); it != MRUEntries.end(); ++it)
	{
		  // check whether the current entry exists
		CString s = *it;
		if (_taccess(s, 4) != 0)
		{
			  // for the demo, announce removal
			::MessageBox(NULL, s, _T("Removing invalid MRU entry..."),
			    MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
			RemoveMRUEntry(s);
		}
	}
}

/*============================================================================*/
	LRESULT CMainFrame::
WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)                        /*

	This is the main frame message loop. By default, it handles the
	normal housekeeping functions (see Win32++\include\frame.h).
*-----------------------------------------------------------------------------*/
{
	  // can't use a case statement on this ::RegisterWindowMessage() value
	if (uMsg == UWM_FINDMSGSTRING)
	{
		m_Doc.OnFindReplace(uMsg, wParam, lParam);
		return TRUE;
	}

	switch (uMsg)
	{
	    case WM_CTLCOLORBTN:
	    case WM_CTLCOLOREDIT:
	    case WM_CTLCOLORDLG:
	    case WM_CTLCOLORLISTBOX:
	    case WM_CTLCOLORSCROLLBAR:
	    case WM_CTLCOLORSTATIC:
		return OnCtlColor((HDC)wParam, (HWND)lParam, uMsg);

	    case WM_SETCURSOR:
	    {
		::SetCursor(m_hCursor);
		return TRUE;
	    }

	    case IDM_UPDATECONTROLUISTATE:
	    	UpdateControlUIState();
		return TRUE;
		
	    case WM_HELP:
	    {     // Handle the F1 requests for help. The HELPINFO in lParam
		  // is of no use to the current context help class. The
		  // system also sends this message on Shift+F1, so it is
		  // necessary to distinguish which usage is in effect.
		if (!m_AppHelp.IsActive())
			m_AppHelp.OnHelpID(IDM_HELP_CONTENT);
	    	return TRUE;
	    }

	    case IDM_HELP_ACTIVE:
		return DoContextHelp(wParam);

	    case WM_SYSCOMMAND:
	    {
		  // if in help mode, let it handle the wParam message
	    	if (DoContextHelp(wParam))
			return TRUE;

		  // else process requests for action
		switch (LOWORD(wParam))
		{
		    case SC_CLOSE:
			OnTerminate();
			break;  // let default process this further
		}
	    }
	}
	  // pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

