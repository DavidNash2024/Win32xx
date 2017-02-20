////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_View as the view window of the frame
	SetView(m_View);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\Frame"));
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// OnCommand responds to menu and and toolbar input

	UNREFERENCED_PARAMETER(lParam);

	UINT nID = LOWORD(wParam);
	switch(nID)
	{
	case IDM_FILE_OPEN:			return OnFileOpen();
	case IDM_FILE_SAVE:			return OnFileSave();
	case IDM_FILE_SAVEAS:		return OnFileSave();
	case IDM_FILE_PRINT:		return OnFilePrint();
	case IDM_FILE_EXIT:			return OnFileExit();
	case IDW_VIEW_STATUSBAR:	return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:		return OnViewToolBar();
	case IDM_HELP_ABOUT:		return OnHelp();
	}

	return FALSE;
}

int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::OnCreate is optional.
	// The default for the following variables is TRUE

	// SetUseIndicatorStatus(FALSE);	// Don't show keyboard indicators in the StatusBar
	// SetUseMenuStatus(FALSE);			// Don't show menu descriptions in the StatusBar
	// SetUseReBar(FALSE);				// Don't use a ReBar
	// SetUseThemes(FALSE);				// Don't use themes
	// SetUseToolBar(FALSE);			// Don't use a ToolBar

	// call the base class function
	return CFrame::OnCreate(cs);
}

BOOL CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
	return TRUE;
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.


	TRACE("Frame created\n");
}

BOOL CMainFrame::OnFileOpen()
{
	CFileDialog FileDlg(TRUE);

	// Bring up the file open dialog retrieve the selected filename
	if (FileDlg.DoModal(*this) == IDOK)
	{
		GetDoc().FileLoad(FileDlg.GetPathName());
	}

	return TRUE;
}

BOOL CMainFrame::OnFileSave()
{
	CFileDialog FileDlg(FALSE);
	
	// Bring up the file save dialog retrieve the selected filename
	if (FileDlg.DoModal(*this) == IDOK)
	{
		GetDoc().FileStore(FileDlg.GetPathName());
	}

	return TRUE;
}

BOOL CMainFrame::OnFilePrint()
{
	// Bring up a dialog to choose the printer
	CPrintDialog Printdlg;

	try
	{
		INT_PTR Res = Printdlg.DoModal(*this);

		// Retrieve the printer DC
		// CDC dcPrinter = Printdlg.GetPrinterDC();

		// TODO:
		// Add your own code here. Refer to the tutorial for additional information

		return (Res == IDOK);	// boolean expression
	}

	catch (const CException& /* e */)
	{
		// No default printer
		MessageBox(_T("Unable to display print dialog"), _T("Print Failed"), MB_OK);
		return FALSE;
	}
}

LRESULT CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam)
{
	// Process notification messages sent by child windows
//	switch(((LPNMHDR)lParam)->code)
//	{
//		Add case statements for each notification message here
//	}

	// Some notifications should return a value when handled
	return CFrame::OnNotify(wParam, lParam);
}

void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
	// This function is called before the frame is created.
	// It provides an opportunity to modify the various CREATESTRUCT
	// parameters used in the frame window's creation.

	// The WS_EX_LAYOUTRTL style requires Windows 2000 or above in targetver.h
	// cs.dwExStyle = WS_EX_LAYOUTRTL;		// Set Right-To-Left Window Layout

	// Call base clase to set defaults
	CFrame::PreCreate(cs);

	// cs.style &= ~WS_VISIBLE;	// Remove the WS_VISIBLE style. The frame will be initially hidden.
}

void CMainFrame::SetupToolBar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolBarButton( IDM_FILE_NEW   );
	AddToolBarButton( IDM_FILE_OPEN  );
	AddToolBarButton( IDM_FILE_SAVE  );

	AddToolBarButton( 0 );						// Separator
	AddToolBarButton( IDM_EDIT_CUT,   FALSE );	// disabled button
	AddToolBarButton( IDM_EDIT_COPY,  FALSE );	// disabled button
	AddToolBarButton( IDM_EDIT_PASTE, FALSE );	// disabled button

	AddToolBarButton( 0 );						// Separator
	AddToolBarButton( IDM_FILE_PRINT );

	AddToolBarButton( 0 );						// Separator
	AddToolBarButton( IDM_HELP_ABOUT );
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//		Add case statements for each messages to be handled here
//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

