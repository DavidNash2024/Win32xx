////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class


#include "resource.h"
#include "mainfrm.h"


CMainFrame::CMainFrame()  : m_bShowHidden(FALSE)
{
	// Set m_MainView as the view window of the frame
//	SetView(m_MainView);
	SetView(m_LeftPane);

	// Define our toolbar data
	AddToolbarButton( IDM_FILE_NEW   );
	AddToolbarButton( IDM_FILE_OPEN  );
	AddToolbarButton( IDM_FILE_SAVE  );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_EDIT_CUT   );
	AddToolbarButton( IDM_EDIT_COPY  );
	AddToolbarButton( IDM_EDIT_PASTE );
	AddToolbarButton( IDM_FILE_PRINT );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_VIEWMENU);
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_HELP_ABOUT );

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\Explorer Sample"));
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::DoPopupMenu()
{
	// Creates the popup menu for the "View Menu" toolbar button

	// Position the popup menu
	CToolbar& TB = GetToolbar();
	CRect rc = TB.GetItemRect(TB.CommandToIndex(IDM_VIEWMENU));
	::MapWindowPoints(TB.GetHwnd(), NULL, (LPPOINT)&rc, 2);

	TPMPARAMS tpm;
	tpm.cbSize = sizeof(TPMPARAMS);
	tpm.rcExclude = rc;

	// Load the popup menu
	HMENU hTopMenu = ::LoadMenu(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDM_VIEWMENU));
	HMENU hPopupMenu = GetSubMenu(hTopMenu, 0);

	// Put a radio check in the currently checked item
	MENUITEMINFO mii = {0};
	for (int i = 3 ; i < 7 ; i++)
	{
		ZeroMemory(&mii, sizeof(MENUITEMINFO));

		// Fix for an undocumented bug in the Windows API
		// For Win95 and NT, cbSize needs to be 44
		if ((1400 == GetWinVersion()) || (2400 == GetWinVersion()))
			mii.cbSize = 44;
		else
			mii.cbSize = sizeof(MENUITEMINFO);

		mii.fMask  = MIIM_STATE | MIIM_ID;
		GetMenuItemInfo(GetSubMenu(GetFrameMenu(), 1), i, TRUE,  &mii );
		if (mii.fState & MFS_CHECKED)
			::CheckMenuRadioItem(hTopMenu, IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, mii.wID, 0);
	}

	// Start the popup menu
	::TrackPopupMenuEx(hPopupMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, rc.left, rc.bottom, m_hWnd, &tpm);

	// Release the menu resource
	::DestroyMenu(hTopMenu);
}

void CMainFrame::OnInitialUpdate()
{
	// Add the right window pane
	int Width = (int) (GetWindowRect().Width() * 0.7);
	DWORD dwDockStyle = DS_DOCKED_RIGHT  | DS_NO_UNDOCK | DS_NO_CAPTION;
	m_RightPane = (CRightPane*)m_LeftPane.AddDockedChild(new CRightPane, dwDockStyle, Width);

	// All windows are now created, so populate the treeview
	GetTreeView()->GetRootItems();

	// Uncheck the hidden menu item
	::CheckMenuItem (GetFrameMenu(), IDM_SHOW_HIDDEN, MF_UNCHECKED);

	// Place Radio button in view menu
	HMENU hView = ::GetSubMenu(GetFrameMenu(), 1);
	::CheckMenuRadioItem(hView, IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_REPORT, 0);
}

void CMainFrame::OnClose()
{
	// Destroying the TreeView triggers shutdown of the Treeview thread.
	GetTreeView()->DestroyWindow();
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	// Handle the the View submenu
	HMENU hView = ::GetSubMenu(GetFrameMenu(), 1);

	switch (LOWORD(wParam))
	{
	case IDM_FILE_EXIT:
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	case IDM_HELP_ABOUT:
		OnHelp();
		return TRUE;
	case IDM_VIEW_LARGEICON:
		GetListView()->ViewLargeIcons();
		::CheckMenuRadioItem(hView, IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_LARGEICON, 0);
		return TRUE;
	case IDM_VIEW_SMALLICON:
		GetListView()->ViewSmallIcons();
		::CheckMenuRadioItem(hView, IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_SMALLICON, 0);
		return TRUE;
	case IDM_VIEW_LIST:
		GetListView()->ViewList();
		::CheckMenuRadioItem(hView, IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_LIST, 0);
		return TRUE;
	case IDM_VIEW_REPORT:
		GetListView()->ViewReport();
		::CheckMenuRadioItem(hView, IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_REPORT, 0);
		return TRUE;
	case IDM_SHOW_HIDDEN:
		m_bShowHidden = !m_bShowHidden;
		::CheckMenuItem (GetFrameMenu(), IDM_SHOW_HIDDEN, (TRUE == m_bShowHidden)? MF_CHECKED : MF_UNCHECKED);

		// Refresh the Listview display
		GetListView()->DoDisplay();
		return TRUE;
	case IDM_VIEWMENU:
		// This Command is recieved if Comctl32.dll version is below 4.7
		DoPopupMenu();
		return TRUE;
	} // switch cmd

	return FALSE;

} // CMainFrame::OnCommand(...)

void CMainFrame::OnCreate()
{
	// call the base OnCreate function
	CFrame::OnCreate();

	SetButtons();
}

LRESULT CMainFrame::OnNotify(WPARAM /*wParam*/, LPARAM lParam)
{
	// Notification from our dropdown button is recieved if Comctl32.dll version
	// is 4.70 or later (IE v3 required).
    switch(((LPNMHDR)lParam)->code)
	{
 		//Menu for dropdown toolbar button
		case TBN_DROPDOWN:
		{
			if (((LPNMHDR)lParam)->hwndFrom == GetToolbar().GetHwnd())
				DoPopupMenu();
		}

	} //switch LPNMHDR

	return 0L;
}

void CMainFrame::SetButtons()
{
	// Use larger buttons
	CToolbar& TB = GetToolbar();
	SetToolbarImages(TB, 9, RGB(192,192,192), IDB_TOOLBAR_NORM, IDB_TOOLBAR_HOT, IDB_TOOLBAR_DIS);

	// Disable some buttons
	TB.DisableButton(IDM_FILE_NEW);
	TB.DisableButton(IDM_FILE_OPEN);
	TB.DisableButton(IDM_FILE_SAVE);
	TB.DisableButton(IDM_EDIT_CUT);
	TB.DisableButton(IDM_EDIT_COPY);
	TB.DisableButton(IDM_EDIT_PASTE);
	TB.DisableButton(IDM_FILE_PRINT);

	// Configure the ViewMenu button to bring up a menu
	// Setting this style requires comctl32.dll version 4.72 or later
	if (GetComCtlVersion() >= 472)
	{
		TB.SetButtonStyle(IDM_VIEWMENU, BTNS_WHOLEDROPDOWN);
	}

	// Add some text to the buttons
 	TB.SetButtonText(IDM_FILE_NEW,   _T("New"));
	TB.SetButtonText(IDM_FILE_OPEN,  _T("Open"));
	TB.SetButtonText(IDM_FILE_SAVE,  _T("Save"));
	TB.SetButtonText(IDM_EDIT_CUT,   _T("Cut"));
	TB.SetButtonText(IDM_EDIT_COPY,  _T("Copy"));
	TB.SetButtonText(IDM_EDIT_PASTE, _T("Paste"));
	TB.SetButtonText(IDM_FILE_PRINT, _T("Print"));
	TB.SetButtonText(IDM_VIEWMENU,   _T("View Menu"));
	TB.SetButtonText(IDM_HELP_ABOUT, _T("About"));

	if (IsRebarUsed())
	{
		// Resize the Rebar band
		CRebar& RB = GetRebar();
		RB.ResizeBand(RB.GetBand(TB.GetHwnd()), TB.GetMaxSize());
	}
}

LRESULT CMainFrame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		OnClose();
		break;
	}

	// pass any unhandled messages on for default processing
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

