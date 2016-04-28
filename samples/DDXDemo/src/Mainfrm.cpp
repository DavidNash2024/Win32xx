/* (12-Jun-2015) [Tab/Indent: 8/8][Line/Box: 80/74]              (MainFrm.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

	Contents Description: The DDXDemo CMainFrame class implementation.
	This class controls the appearance and frame-related actions of the
	DDXDemo sample program. This class is a modified version of that found
	in the FormDocView sample distributed with the Win32++ Windows interface
	classes, Copyright (c) 2005-2015 David Nash, used under permissions
	granted therein. The modified sample program was based on code provided
	by Lynn Allan. This program extends the given sample by application of
	Dialog Data Exchange and Validation (DDX/DDV). The NoResizeStatusBar
	class creates a status bar without the resizing gripper in the lower-
	right-hand corner.

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
		
	Acknowledgement:
	The author would like to thank and acknowledge the advice, critical
	review, insight, and assistance provided by David Nash in the development
	of this work.		

********************************************************************************

	Implementation of theCMainFrame class

*******************************************************************************/

#include "stdafx.h"
#include "resource.h"
#include "App.h"
#include "mainfrm.h"

static const LPCTSTR registryKeyName = _T("Win32++\\DDX-DDVDemo");

/*============================================================================*/
	CDoc&
TheDoc()                                                            /*

	Return a global reference to the CDoc object. 
*-----------------------------------------------------------------------------*/
{
	return TheApp().TheFrame()->TheDoc();
}

/*============================================================================*/
	CMainFrame::
CMainFrame()                                                            /*

	Construct the CMainFrame. Must be called after the CFrame constructor.
	Also, load the initial window position, toolbar/statusbar switches,
	and view parameters, which will have been saved here in previous
	invocations of the program.  These items occupy the registry key
	section 'Software\<key name>\Frame Settings'.  Also load the document's
	saved parameters from the 'Software\<key name>\Document Settings' key.

	Note: the <key name> used here should be a unique registry key name
	such as "CompanyName\\Application".
*-----------------------------------------------------------------------------*/
	: m_View(IDD_MAIN_DIALOG)
{
	  //Set m_View as the view window of the frame
	SetView(m_View);

	LoadRegistrySettings(registryKeyName);
}

/*============================================================================*/
	CMainFrame::
~CMainFrame()                                                            /*

	Destructor for CMainFrame.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
	BOOL CMainFrame::
LoadRegistrySettings(LPCTSTR szKeyName)					/*

	Load the main frame and document persistent data from the registry
	section marked by szKeyName. The frame values were automatically
	deposited into the registry by the base class SaveRegistrySettings(),
	which was called by the framework when the program was last terminated.
	The document data is recovered by a CDoc class method.
*-----------------------------------------------------------------------------*/
{
	  // reload previously saved window placement, toolbar/statusbar
	  // switches and view settings from the registry entries in key section
	  // 'Software\szKeyName\Frame Settings'
	CFrame::LoadRegistrySettings(szKeyName);
	  // load the saved document entries from the same key
	TheDoc().LoadDocRegistry(szKeyName);

	return TRUE;
}

/*============================================================================*/
	void CMainFrame::
OnClose()								/*

	The framework calls this member function as a signal that the CWnd or
	application is to terminate. Retrieve the data in the dialog controls
	and save it if all is well. If an error arises, return with the cursor
	in the offending control so the user can correct it.
*-----------------------------------------------------------------------------*/
{									
	if (UpdateDialog(TRUE))						
	{ 	// all is well
		TRACE("Verification passed\n");				
		TheView().UpdateParameters();				
	}
	else
	{	  // oops! there is a problem with some of the control data								
		TRACE("*** Verification failed ***\n");			
			return;	 // return control to user to fix this problem
	}

	  // save the document contents into the registry		
	TheDoc().SaveDocRegistry(GetRegistryKeyName().c_str());		
	TRACE("Data saved to registry\n");				
	  // the base class calls SaveRegistrySettings() and DestroyWindow()
	CFrame::OnClose();						
}									

/*============================================================================*/
	BOOL CMainFrame::
OnCommand(WPARAM wParam, LPARAM lParam)  				/*

	The framework calls this member function when the user selects an item
	from a menu, when a child control sends a notification message, or when
	an accelerator keystroke is translated. On recognizing a particular
	message, the appropriate member function is to be activated. This method
	allows your application to handle Windows messages. The parameters
	passed above reflect the parameters received by the framework when the
	message was received. If you call the base-class implementation of this
	function, that implementation will use the parameters originally passed
	with the message and not the parameters you supply to the function.
	The method should return nonzero if it processes a particular message;
	otherwise it should return 0.  The low-order word of wParam identifies
	the command ID of the menu item, control, or accelerator. The high-order
	word of wParam specifies the notification message if the message is from
	a control. If the message is from an accelerator, the high-order word
	is 1. If the message is from a menu, the high-order word is 0.  The
	lParam identifies the control that sends the message if the message is
	from a control. Otherwise, lParam is 0.
*-----------------------------------------------------------------------------*/
{
	UNREFERENCED_PARAMETER(lParam);

	WORD wpLo = LOWORD(wParam);
	switch(wpLo)
	{
	    case IDM_FILE_EXIT:
	    	OnFileExit();
		return TRUE;
		
	    case IDW_VIEW_STATUSBAR:
	    	OnViewStatusBar();
		return TRUE;
		
	    case IDW_VIEW_TOOLBAR:
		OnViewToolBar();
		return TRUE;
		
	    case IDM_HELP_ABOUT:
		OnHelp();
		return TRUE;

	    case IDC_RADIO_A:
	    case IDC_RADIO_B:
	    case IDC_RADIO_C:
		TheView().m_iRadioA = wpLo - IDC_RADIO_A;
		UpdateDialog(SENDTOCONTROL);
	    	return TRUE;

	    case IDC_CHECK_A:
		TheView().m_iCheckA = !TheView().m_iCheckA;
		UpdateDialog(SENDTOCONTROL);
		return TRUE;

	    case IDC_CHECK_B:
		TheView().m_iCheckB = !TheView().m_iCheckB;
		UpdateDialog(SENDTOCONTROL);
		return TRUE;

	    case IDC_CHECK_C:
		TheView().m_iCheckC = !TheView().m_iCheckC;
		UpdateDialog(SENDTOCONTROL);
		return TRUE;

	    case WM_CLOSE:
	    	return TRUE;
	}

	return FALSE;
} 

/*============================================================================*/
	int CMainFrame::
OnCreate(CREATESTRUCT& cs)  						/*

	Define the way the frame is created. Overriding CFrame::OnCreate is
	optional.
*-----------------------------------------------------------------------------*/
{
	// Uncomment the lines below to change frame options.

	// SetUseIndicatorStatus(FALSE); // Don't show keyboard indicators
	// SetUseMenuStatus(FALSE);	 // Don't show menu item descriptions
	// SetUseReBar(FALSE);		 // Don't use a ReBar
	// SetUseThemes(FALSE);		 // Don't use themes
	// SetUseToolBar(FALSE);	 // Don't use a ToolBar

	// call the base class function
	int rtn = CFrame::OnCreate(cs);  
	
	if (IsReBarSupported())
	{
		//Set our theme colors
		SetThemeColors();
	}

	  // just before the window appears, set the status box to show all
	  // of the dialog contents and the initial focus
	TheView().AdjustParameters();
	  // show the initial document and status
	UpdateDialog(SENDTOCONTROL);

	return rtn;
}

/*============================================================================*/
	void CMainFrame::
OnFileExit()                                           			/*

	Close the application.
*-----------------------------------------------------------------------------*/
{
	  // Issue a close request to the frame
	PostMessage(WM_CLOSE);
}

/*============================================================================*/
	void CMainFrame::
OnInitialUpdate()							/*

	This method is called by the framework after the view is first attached
	to the document, but before the view is initially displayed. Add code
	here to perform any one-time initialization.
*-----------------------------------------------------------------------------*/
{
	// The frame is now created.
	// Place any additional startup code here.
}

/*============================================================================*/
	void CMainFrame::
PreCreate(CREATESTRUCT& cs)   						/*

	This method is called by the framework before the creation of the
	window attached to this CWnd object.  Use it to modify the CREATESTRUCT
	structure before the window is created.
*-----------------------------------------------------------------------------*/
{
	  // set default window placement
	cs.x  = 100;
	cs.y  = 100;
	cs.cx = 950;
	cs.cy = 675;
	  // reset frame values with those from the registry, if they exist
	CFrame::PreCreate(cs);

	   // specify a title bar and border with a window-menu on the title bar
	cs.style = WS_VISIBLE	// must be present, or screen is blank
//	    | WS_OVERLAPPED     // implied, the default 0x00000000L.
	    | WS_CAPTION        // redundant, but put in anyway
	    | WS_SYSMENU        // adds close box at top right
	    | WS_MINIMIZEBOX    // adds minimize box to close box
//	    | WS_THICKFRAME     // enables resizing, unwanted here
	    ;
	cs.dwExStyle = WS_EX_CLIENTEDGE;
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
	SetReBarColors(RGB(150,190,245), RGB(196,215,250), RGB(220,230,250),
	    RGB( 70,130,220));

	StatusBarTheme sbt = {TRUE, RGB(150,190,245), RGB(196,215,250)};
	SetStatusBarTheme (sbt);

	ToolBarTheme tbt = {TRUE, RGB(255, 230, 190), RGB(255, 190, 100),
	    RGB(255, 140, 40), RGB(255, 180, 80), RGB(128, 128, 255)};
	SetToolBarTheme(tbt);

	MenuTheme mt = {TRUE, RGB(255, 230, 190), RGB(255, 190, 100),
	    RGB(150,190,245), RGB(220,230,250), RGB(128, 128, 200)};
	SetMenuTheme(mt);
	RecalcLayout();
	return TRUE;
}

/*============================================================================*/
	void CMainFrame::
SetupToolBar()                                           		/*

	Add the specific buttons to the tool bar.
*-----------------------------------------------------------------------------*/
{
	  // Set the Resource IDs for the toolbar buttons
	AddToolBarButton( IDM_FILE_NEW   );
	AddToolBarButton( IDM_FILE_OPEN  );
	AddToolBarButton( IDM_FILE_SAVE  );
	AddToolBarButton( 0 );        // Separator
	AddToolBarButton( IDM_EDIT_CUT   );
	AddToolBarButton( IDM_EDIT_COPY  );
	AddToolBarButton( IDM_EDIT_PASTE );
	AddToolBarButton( 0 );        // Separator
	AddToolBarButton( IDM_FILE_PRINT );
	AddToolBarButton( 0 );        // Separator
	AddToolBarButton( IDM_HELP_ABOUT );
}

/*============================================================================*/
	BOOL CMainFrame::
UpdateDialog(BOOL bReadFromControl /* = SENDTOCONTROL */)		/*
    
	Update data items in memory (bReadFromControl is READFROMCONTROL) or in
	the dialog controls (bReadFromControl is SENDTOCONTROL). The list of
	affected controls is specified in the CView::DoDataExchange() method.
*-----------------------------------------------------------------------------*/
{
	return TheView().UpdateDialog(bReadFromControl);
}

/*============================================================================*/
	LRESULT CMainFrame::
WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)			/*

	All CWnd windows direct their messages here. This function can process
	some of these and redirect the remaining messages to the base class
	default handler.
*-----------------------------------------------------------------------------*/
{
//	switch (uMsg)
//	{
//		// Add case statements for each messages to be handled here
//	}

	  // pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

