////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"


CMainFrame::CMainFrame()  : m_pLeftPane(0), m_ShowHidden(FALSE)
{
	// Set m_MainView as the view window of the frame
	SetView(m_RightPane);

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
	CToolBar& TB = GetToolBar();
	CRect rc = TB.GetItemRect(TB.CommandToIndex(IDM_VIEWMENU));
	TB.MapWindowPoints(NULL, (LPPOINT)&rc, 2);

	TPMPARAMS tpm;
	tpm.cbSize = sizeof(TPMPARAMS);
	tpm.rcExclude = rc;

	// Load the popup menu
	CMenu TopMenu(IDM_VIEWMENU);
	CMenu PopupMenu = TopMenu.GetSubMenu(0);

	// Put a radio check in the currently checked item
	MENUITEMINFO mii;
	ZeroMemory(&mii, sizeof(MENUITEMINFO));
	for (int i = 3 ; i < 7 ; i++)
	{
		ZeroMemory(&mii, GetSizeofMenuItemInfo());
		mii.cbSize = GetSizeofMenuItemInfo();

		mii.fMask  = MIIM_STATE | MIIM_ID;
		CMenu SubMenu = GetFrameMenu().GetSubMenu(1);
		SubMenu.GetMenuItemInfo(i, mii, TRUE);
		if (mii.fState & MFS_CHECKED)
			TopMenu.CheckMenuRadioItem(IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, mii.wID, 0);
	}

	// Start the popup menu
	PopupMenu.TrackPopupMenuEx(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, rc.left, rc.bottom, *this, &tpm);
}

void CMainFrame::OnInitialUpdate()
{
	// Add the right window pane
	int Width = (int) (GetWindowRect().Width() * 0.3);
	DWORD dwDockStyle = DS_DOCKED_LEFT  | DS_NO_UNDOCK | DS_NO_CAPTION;
	m_pLeftPane = static_cast<CLeftPane*>(m_RightPane.AddDockedChild(new CLeftPane, dwDockStyle, Width));

	// All windows are now created, so populate the treeview
	GetTreeView().GetRootItems();

	// Uncheck the hidden menu item
	GetFrameMenu().CheckMenuItem(IDM_SHOW_HIDDEN, MF_UNCHECKED);

	// Place Radio button in view menu
	CMenu ViewMenu = GetFrameMenu().GetSubMenu(1);
	ViewMenu.CheckMenuRadioItem(IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_REPORT, 0);

	// Uncomment the following to use a hash bar and disable of auto resizing 
	// m_RightPane.SetDragAutoResize(FALSE);
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (LOWORD(wParam))
	{
	case IDM_FILE_EXIT:			return OnFileExit();
	case IDM_HELP_ABOUT:		return OnHelp();
	case IDW_VIEW_STATUSBAR:	return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:		return OnViewToolBar();
	case IDM_VIEW_LARGEICON:	return OnViewLargeIcon();
	case IDM_VIEW_SMALLICON:	return OnViewSmallIcon();
	case IDM_VIEW_LIST:			return OnViewList();
	case IDM_VIEW_REPORT:		return OnViewReport();
	case IDM_SHOW_HIDDEN:		return OnShowHidden();
	case IDM_VIEWMENU:			return OnViewMenu();
	}

	return FALSE;

} // CMainFrame::OnCommand(...)

int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// Uncomment the lines below to change frame options.

	// SetUseIndicatorStatus(FALSE);	// Don't show keyboard indicators in the StatusBar
	// SetUseMenuStatus(FALSE);			// Don't show menu descriptions in the StatusBar
	// SetUseReBar(FALSE);				// Don't use a ReBar
	// SetUseThemes(FALSE);				// Don't use themes
	// SetUseToolBar(FALSE);			// Don't use a ToolBar
	
	// call the base OnCreate function
	return CFrame::OnCreate(cs);
}

BOOL CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
	return TRUE;
}

BOOL CMainFrame::OnViewLargeIcon()
{
	// Handle the the View submenu
	CMenu ViewMenu = GetFrameMenu().GetSubMenu(1);

	GetListView().ViewLargeIcons();
	ViewMenu.CheckMenuRadioItem(IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_LARGEICON, 0);
	return TRUE;
} 

BOOL CMainFrame::OnViewList()
{
	// Handle the the View submenu
	CMenu ViewMenu = GetFrameMenu().GetSubMenu(1);

	GetListView().ViewList();
	ViewMenu.CheckMenuRadioItem(IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_LIST, 0);
	return TRUE;
}

BOOL CMainFrame::OnViewMenu()
{
	// This Command is recieved if Comctl32.dll version is below 4.7
	DoPopupMenu();
	return TRUE;
}

BOOL CMainFrame::OnViewReport()
{
	// Handle the the View submenu
	CMenu ViewMenu = GetFrameMenu().GetSubMenu(1);

	GetListView().ViewReport();
	ViewMenu.CheckMenuRadioItem(IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_REPORT, 0);
	return TRUE;
}

BOOL CMainFrame::OnViewSmallIcon()
{
	// Handle the the View submenu
	CMenu ViewMenu = GetFrameMenu().GetSubMenu(1);

	GetListView().ViewSmallIcons();
	ViewMenu.CheckMenuRadioItem(IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_SMALLICON, 0);
	return TRUE;
}

BOOL CMainFrame::OnShowHidden()
{
	// Handle the the View submenu
	CMenu ViewMenu = GetFrameMenu().GetSubMenu(1);

	m_ShowHidden = !m_ShowHidden;
	ViewMenu.CheckMenuItem(IDM_SHOW_HIDDEN, (TRUE == m_ShowHidden)? MF_CHECKED : MF_UNCHECKED);

	// Refresh the Listview display
	GetListView().DoDisplay();
	return TRUE;
}

LRESULT CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam)
{
	// Notification from our dropdown button is recieved if Comctl32.dll version
	// is 4.70 or later (IE v3 required).
    switch(((LPNMHDR)lParam)->code)
	{
 		//Menu for dropdown toolbar button
		case TBN_DROPDOWN:
		{
			if (((LPNMHDR)lParam)->hwndFrom == GetToolBar())
				DoPopupMenu();
		}

	} //switch LPNMHDR

	return CFrame::OnNotify(wParam, lParam);
}

void CMainFrame::SetupToolBar()
{
	// Define our toolbar data
	AddToolBarButton( IDM_FILE_NEW  , FALSE, _T("New") );
	AddToolBarButton( IDM_FILE_OPEN , FALSE, _T("Open") );
	AddToolBarButton( IDM_FILE_SAVE , FALSE, _T("Save") );
	
	AddToolBarButton( 0 );	// Separator
	AddToolBarButton( IDM_EDIT_CUT  , FALSE, _T("Cut") );
	AddToolBarButton( IDM_EDIT_COPY , FALSE, _T("Copy") );
	AddToolBarButton( IDM_EDIT_PASTE, FALSE, _T("Paste") );
	AddToolBarButton( IDM_FILE_PRINT, FALSE, _T("Print") );
	
	AddToolBarButton( 0 );	// Separator
	AddToolBarButton( IDM_VIEWMENU,   TRUE,  _T("View Menu"));
	
	AddToolBarButton( 0 );	// Separator
	AddToolBarButton( IDM_HELP_ABOUT, TRUE,  _T("About") );

	// Use larger buttons
	SetToolBarImages(RGB(192,192,192), IDB_TOOLBAR_NORM, IDB_TOOLBAR_HOT, IDB_TOOLBAR_DIS);

	// Configure the ViewMenu button to bring up a menu
	// Setting this style requires comctl32.dll version 4.72 or later
	if (GetComCtlVersion() >= 472)
	{
		CToolBar& TB = GetToolBar();
		TB.SetButtonStyle(IDM_VIEWMENU, BTNS_WHOLEDROPDOWN);
	}
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//
//	}

	// pass any unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

