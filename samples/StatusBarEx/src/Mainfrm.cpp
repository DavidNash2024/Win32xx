/* [Tab/Indent: 8/8][Line/Box: 80/74]	                         (MainFrm.cpp) *
********************************************************************************

	Contents Description: Implementation of the CMainFrame class for a 
	sample program demonstrating the extended use of the status bar using 
	the Win32++ Windows framework, Copyright (c) 2005-2017 David Nash.

	This particular program was adapted from the StatusBar sample progam
	in the Win32++ distribution by Robert C. Tausworthe to illustrate the 
	definition of the status bar makeup by way of a data structure, in a
	similar fashion as that appearing in the Microsoft Foundation Classes 
	(MFC), and to override the Win32++ normal statusbar definition of four 
	parts to any number suitable for an application.

	The code changes to the StatusBar sample program only occur in the 
	CMainFrame and MyStatusBar classes. However, the entire program has
	been reformatted for better readability, per the Programming Notes,
	below.

	These materials are provided "as is", without warranty of any kind,
	express or implied, including but not limited to: warranties of
	merchantability, fitness for a particular purpose, and non-infringement.

	Programming Notes:
                The programming conventions used here roughly follow those 
		established by the 1997-1999 Jet Propulsion Laboratory Deep 
		Space Network Planning and Preparation Subsystem project for 
		C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"

/*============================================================================*/
	CMainFrame::
CMainFrame()								/*

*-----------------------------------------------------------------------------*/
{
	  //Set m_View as the view window of the frame
	SetView(m_View);
	  // Set the registry key name, and load the initial window position
	  // Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\StatusBarEx Sample"));
}

/*============================================================================*/
	CMainFrame::
~CMainFrame()								/*

*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
	BOOL CMainFrame::
OnCommand(WPARAM wParam, LPARAM lParam)					/*

*-----------------------------------------------------------------------------*/
{
	// OnCommand responds to menu and and toolbar input

	UNREFERENCED_PARAMETER(lParam);

	UINT nID = LOWORD(wParam);
	switch(nID)
	{
	    case IDM_FILE_OPEN:		return OnFileOpen();
	    case IDM_FILE_SAVE:		return OnFileSave();
	    case IDM_FILE_SAVEAS:	return OnFileSave();
	    case IDM_FILE_PRINT:	return OnFilePrint();
	    case IDM_FILE_EXIT:		return OnFileExit();
	    case IDW_VIEW_STATUSBAR:	return OnViewStatusBar();
	    case IDW_VIEW_TOOLBAR:	return OnViewToolBar();
	    case IDM_HELP_ABOUT:	return OnHelp();
	}
	return FALSE;
}

/*============================================================================*/
	int CMainFrame::
OnCreate(CREATESTRUCT& cs)						/*

*-----------------------------------------------------------------------------*/
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::OnCreate is optional.
	// Uncomment the lines below to change frame options.

	// SetUseIndicatorStatus(FALSE); // No keyboard status bar indicators 
	// SetUseMenuStatus(FALSE);	 // No menu staus bar descriptions 
	// SetUseReBar(FALSE);		 // Don't use a ReBar
	// SetUseThemes(FALSE);		 // Don't use themes
	// SetUseToolBar(FALSE);	 // Don't use a ToolBar
	  // call the base class function
	CFrame::OnCreate(cs);

	return 0;
}

/*============================================================================*/
	BOOL CMainFrame::
OnFileExit()								/*

*-----------------------------------------------------------------------------*/
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);

	return TRUE;
}

/*============================================================================*/
	void CMainFrame::
OnInitialUpdate()							/*

*-----------------------------------------------------------------------------*/
{
	// The frame is now created.
	// Place any additional startup code here.

	  // The content of the owner drawn status bar part can be set anywhere
	  // according to the needs of the app. We put it here for the demo. 
	m_CustomStatusBar.SetOwnerDrawText(_T(" Owner Draw"));
	TRACE("Frame created\n");
}

/*============================================================================*/
	BOOL CMainFrame::
OnFileOpen()								/*

*-----------------------------------------------------------------------------*/
{
	CFileDialog FileDlg(TRUE);

	// Bring up the file open dialog retrieve the selected filename
	if (FileDlg.DoModal(*this) == IDOK)
	{
		// TODO: Add your own code here. Refer to the tutorial for 
		// additional information
	}
	return TRUE;
}

/*============================================================================*/
	BOOL CMainFrame::
OnFileSave()								/*

*-----------------------------------------------------------------------------*/
{
	CFileDialog FileDlg(FALSE);

	// Bring up the file save dialog retrieve the selected filename
	if (FileDlg.DoModal(*this) == IDOK)
	{
		// TODO: Add your own code here. Refer to the tutorial for 
		// additional information
	}
	return TRUE;
}

/*============================================================================*/
	BOOL CMainFrame::
OnFilePrint()								/*

*-----------------------------------------------------------------------------*/
{
	// Bring up a dialog to choose the printer
	CPrintDialog Printdlg;

	try
	{
		INT_PTR Res = Printdlg.DoModal(*this);

		// Retrieve the printer DC
		// CDC dcPrinter = Printdlg.GetPrinterDC();

		// TODO: Add your own code here. Refer to the tutorial for 
		// additional information

		return (Res == IDOK);	// boolean expression
	}

	catch (const CWinException& /* e */)
	{
		// No default printer
		MessageBox(_T("Unable to display print dialog"), 
		    _T("Print Failed"), MB_OK);
		return FALSE;
	}
}

/*============================================================================*/
	LRESULT CMainFrame::
OnNotify(WPARAM wParam, LPARAM lParam)					/*

*-----------------------------------------------------------------------------*/
{
	// Process notification messages sent by child windows
//	switch(((LPNMHDR)lParam)->code)
//	{
//		Add case statements for each notification message here
//	}

	// Some notifications should return a value when handled
	return CFrame::OnNotify(wParam, lParam);
}

/*============================================================================*/
	void CMainFrame::
PreCreate(CREATESTRUCT& cs)						/*

*-----------------------------------------------------------------------------*/

{
	// This function is called before the frame is created.
	// It provides an opportunity to modify the various CREATESTRUCT
	// parameters used in the frame window's creation.

	  // Call base clase to set defaults
	CFrame::PreCreate(cs);
	  // default window size
	cs.cx = 640;
	cs.cy = 480;

	  // uncomment this for a fixed size window
//	cs.style &= ~WS_THICKFRAME;
}

/*============================================================================*/
	void CMainFrame::
SetStatusIndicators() 							/*

	Override the CFrame method in order to update the custom status bar
	parts.
*-----------------------------------------------------------------------------*/
{
	m_CustomStatusBar.SetStatusIndicators();
}

/*============================================================================*/
	void CMainFrame::
SetupToolBar()								/*

*-----------------------------------------------------------------------------*/
{
	// Set the Resource IDs for the toolbar buttons
	AddToolBarButton( IDM_FILE_NEW   );
	AddToolBarButton( IDM_FILE_OPEN  );
	AddToolBarButton( IDM_FILE_SAVE  );
	
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_EDIT_CUT,   FALSE );	// disabled button
	AddToolBarButton( IDM_EDIT_COPY,  FALSE );	// disabled button
	AddToolBarButton( IDM_EDIT_PASTE, FALSE );	// disabled button
	
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_FILE_PRINT );
	
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_HELP_ABOUT );
}

/*============================================================================*/
	LRESULT CMainFrame::
WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)			/*

*-----------------------------------------------------------------------------*/
{
//	switch (uMsg)
//	{
//		Add case statements for each message to be handled here
//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}
