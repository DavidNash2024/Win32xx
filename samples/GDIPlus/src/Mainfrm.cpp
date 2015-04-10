////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "resource.h"
#include "mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_View as the view window of the frame
	SetView(m_View);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\GDIPlus"));
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// OnCommand responds to menu and and toolbar input

	UNREFERENCED_PARAMETER(lParam);

	switch(LOWORD(wParam))
	{
	case IDM_FILE_OPEN:		 return OnFileOpen();
	case IDM_FILE_SAVE:		 return OnFileSave();
	case IDM_FILE_SAVEAS:	 return OnFileSave();
	case IDM_FILE_PRINT:	 return OnFilePrint();
	case IDM_FILE_EXIT:		 return OnFileExit();
	case IDW_VIEW_STATUSBAR: return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:	 return OnViewToolBar();
	case IDM_HELP_ABOUT:	 return OnHelp();
	}

	return FALSE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT pcs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// Uncomment the lines below to change frame options.

	// SetUseIndicatorStatus(FALSE);	// Don't show keyboard indicators in the StatusBar
	// SetUseMenuStatus(FALSE);			// Don't show menu descriptions in the StatusBar
	// SetUseReBar(FALSE);				// Don't use a ReBar
	// SetUseThemes(FALSE);				// Don't use themes
	// SetUseToolBar(FALSE);			// Don't use a ToolBar

	// call the base class function
	return CFrame::OnCreate(pcs);
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.

	TRACE("Frame created\n");
}

BOOL CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
	return TRUE;
}

BOOL CMainFrame::OnFileOpen()
{
	// Bring up the file open dialog
	CFile File;
	CString str = File.OpenFileDialog(0, 0, 0, 0);

	// TODO:
	// Add your own code here. Refer to the tutorial for additional information

	return TRUE;
}

BOOL CMainFrame::OnFileSave()
{
	// Bring up the file save dialog.
	CFile File;
	CString str = File.SaveFileDialog(0, 0, 0, 0, 0);

	// TODO:
	// Add your own code here. Refer to the tutorial for additional information

	return TRUE;
}

BOOL CMainFrame::OnFilePrint()
{
	// Bring up a dialog to choose the printer
	PRINTDLG pd;
	ZeroMemory(&pd, sizeof(PRINTDLG));
	pd.lStructSize = sizeof( pd );
	pd.Flags = PD_RETURNDC;
	pd.hwndOwner = *this;

	// Retrieve the printer DC
	PrintDlg( &pd );
	
	// TODO:
	// Add your own code here. Refer to the tutorial for additional information

	return TRUE;
}

LRESULT CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam)
{
	// Process notification messages sent by child windows
//	switch(((LPNMHDR)lParam)->code)
//	{
 		//Add case statments for each notification message here
//	}

	// Some notifications should return a value when handled
	return CFrame::OnNotify(wParam, lParam);
}

void CMainFrame::SetupToolBar()
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

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//		Add case statements for each messages to be handled here
//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

