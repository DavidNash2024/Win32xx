////////////////////////////////////////////////////
// Mainfrm.cpp


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
	LoadRegistrySettings(_T("Win32++\\Frame"));
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	// OnCommand responds to menu and and toolbar input

	switch(LOWORD(wParam))
	{
	case IDM_FILE_OPEN:
		OnFileOpen();
		return TRUE;
	case IDM_FILE_SAVE:
		// Refer to the tutorial for an example of OnFileSave()
		return TRUE;
	case IDM_FILE_SAVEAS:
		// Refer to the tutorial for an example of OnFileSaveAs()
		return TRUE;
	case IDM_FILE_PRINT:
		OnFilePrint();
		return TRUE;
	case IDM_FILE_EXIT:
		// End the application
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	case IDW_VIEW_STATUSBAR:
		OnViewStatusbar();
		return TRUE;
	case IDW_VIEW_TOOLBAR:
		OnViewToolbar();
		return TRUE;
	case IDM_HELP_ABOUT:
		// Display the help dialog
		OnHelp();
		return TRUE;
	}

	return FALSE;
}

void CMainFrame::OnCreate()
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// The default for the following variables is TRUE

	// m_bShowIndicatorStatus = FALSE;	// Don't show statusbar indicators
	// m_bShowMenuStatus = FALSE;		// Don't show toolbar or menu status
	// m_bUseRebar = FALSE;				// Don't use rebars
	// m_bUseThemes = FALSE;            // Don't use themes
	// m_bUseToolbar = FALSE;			// Don't use a toolbar

	// call the base class function
	CFrame::OnCreate();
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.

	TRACE(_T("Frame created\n"));
}

void CMainFrame::OnFileOpen()
{
	TCHAR szFilePathName[_MAX_PATH] = _T("");
	OPENFILENAME ofn = {0};
	if (GetWinVersion() >= 2500)
	{
		// For Win2000 and above
		ofn.lStructSize = sizeof(OPENFILENAME);
	}
	else
	{
		// For older operating systems
		ofn.lStructSize = OPENFILENAME_SIZE_VERSION_400;
	}

	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFile = szFilePathName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrTitle = _T("Open File");

	// Bring up the dialog, and open the file
	::GetOpenFileName(&ofn);

	// TODO:
	// Add your own code here. Refer to the tutorial for additional information 
}

void CMainFrame::OnFilePrint()
{
	// Bring up a dialog to choose the printer
	PRINTDLG pd = {0};
	pd.lStructSize = sizeof( pd );
	pd.Flags = PD_RETURNDC;

	// Retrieve the printer DC
	PrintDlg( &pd );
	
	// TODO:
	// Add your own code here. Refer to the tutorial for additional information 
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

void CMainFrame::SetupToolbar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolbarButton( IDM_FILE_NEW   );
	AddToolbarButton( IDM_FILE_OPEN  );
	AddToolbarButton( IDM_FILE_SAVE  );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_EDIT_CUT   );
	AddToolbarButton( IDM_EDIT_COPY  );
	AddToolbarButton( IDM_EDIT_PASTE );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_FILE_PRINT );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_HELP_ABOUT );

	// Disable some buttons
	GetToolbar().DisableButton(IDM_EDIT_CUT);
	GetToolbar().DisableButton(IDM_EDIT_COPY);
	GetToolbar().DisableButton(IDM_EDIT_PASTE);   
}

LRESULT CMainFrame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
		//Additional messages to be handled go here
//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

