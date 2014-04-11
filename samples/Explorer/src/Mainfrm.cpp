////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"


CMainFrame::CMainFrame()  : m_bShowHidden(FALSE)
{
	// Set m_MainView as the view window of the frame
	SetView(m_LeftPane);

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
	CToolBar* pTB = GetToolBar();
	CRect rc = pTB->GetItemRect(pTB->CommandToIndex(IDM_VIEWMENU));
	pTB->MapWindowPoints(NULL, (LPPOINT)&rc, 2);

	TPMPARAMS tpm;
	tpm.cbSize = sizeof(TPMPARAMS);
	tpm.rcExclude = rc;

	// Load the popup menu
	CMenu TopMenu(IDM_VIEWMENU);
	CMenu* pPopupMenu = TopMenu.GetSubMenu(0);

	// Put a radio check in the currently checked item
	MENUITEMINFO mii = {0};
	for (int i = 3 ; i < 7 ; i++)
	{
		ZeroMemory(&mii, GetSizeofMenuItemInfo());
		mii.cbSize = GetSizeofMenuItemInfo();

		mii.fMask  = MIIM_STATE | MIIM_ID;
		CMenu* pSubMenu = GetFrameMenu()->GetSubMenu(1);
		pSubMenu->GetMenuItemInfo(i, &mii, TRUE);
		if (mii.fState & MFS_CHECKED)
			TopMenu.CheckMenuRadioItem(IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, mii.wID, 0);
	}

	// Start the popup menu
	pPopupMenu->TrackPopupMenuEx(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, rc.left, rc.bottom, this, &tpm);
}

void CMainFrame::OnInitialUpdate()
{
	// Add the right window pane
	int Width = (int) (GetWindowRect().Width() * 0.7);
	DWORD dwDockStyle = DS_DOCKED_RIGHT  | DS_NO_UNDOCK | DS_NO_CAPTION;
	m_RightPane = static_cast<CRightPane*>(m_LeftPane.AddDockedChild(new CRightPane, dwDockStyle, Width));

	// All windows are now created, so populate the treeview
	GetTreeView()->GetRootItems();

	// Uncheck the hidden menu item
	GetFrameMenu()->CheckMenuItem(IDM_SHOW_HIDDEN, MF_UNCHECKED);

	// Place Radio button in view menu
	CMenu* pViewMenu = GetFrameMenu()->GetSubMenu(1);
	pViewMenu->CheckMenuRadioItem(IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_REPORT, 0);

	// Uncomment the following to use a hash bar and disable of auto resizing 
	// m_LeftPane.SetDragAutoResize(FALSE);
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (LOWORD(wParam))
	{
	case IDM_FILE_EXIT:			OnFileExit();		return TRUE;
	case IDM_HELP_ABOUT:		OnHelp();			return TRUE;
	case IDW_VIEW_STATUSBAR:	OnViewStatusBar();	return TRUE;
	case IDW_VIEW_TOOLBAR:		OnViewToolBar();	return TRUE;
	case IDM_VIEW_LARGEICON:	OnViewLargeIcon();	return TRUE;
	case IDM_VIEW_SMALLICON:	OnViewSmallIcon();	return TRUE;
	case IDM_VIEW_LIST:			OnViewList();		return TRUE;
	case IDM_VIEW_REPORT:		OnViewReport();		return TRUE;
	case IDM_SHOW_HIDDEN:		OnShowHidden();		return TRUE;
	case IDM_VIEWMENU:			DoPopupMenu();		return TRUE;	// This Command is recieved if Comctl32.dll version is below 4.7
	}

	return FALSE;

} // CMainFrame::OnCommand(...)

int CMainFrame::OnCreate(LPCREATESTRUCT pcs)
{
	// m_bUseIndicatorStatus = FALSE;	// Don't show keyboard indicators in the StatusBar
	// m_bUseMenuStatus = FALSE;		// Don't show menu descriptions in the StatusBar
	// m_bUseReBar = FALSE;				// Don't use a ReBar
	// m_bUseThemes = FALSE;            // Don't use themes
	// m_bUseToolBar = FALSE;			// Don't use a ToolBar
	
	// call the base OnCreate function
	return CFrame::OnCreate(pcs);
}

void CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
}

void CMainFrame::OnViewLargeIcon()
{
	// Handle the the View submenu
	CMenu* pViewMenu = GetFrameMenu()->GetSubMenu(1);

	GetListView()->ViewLargeIcons();
	pViewMenu->CheckMenuRadioItem(IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_LARGEICON, 0);
} 

void CMainFrame::OnViewList()
{
	// Handle the the View submenu
	CMenu* pViewMenu = GetFrameMenu()->GetSubMenu(1);

	GetListView()->ViewList();
	pViewMenu->CheckMenuRadioItem(IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_LIST, 0);
}

void CMainFrame::OnViewReport()
{
	// Handle the the View submenu
	CMenu* pViewMenu = GetFrameMenu()->GetSubMenu(1);

	GetListView()->ViewReport();
	pViewMenu->CheckMenuRadioItem(IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_REPORT, 0);
}

void CMainFrame::OnViewSmallIcon()
{
	// Handle the the View submenu
	CMenu* pViewMenu = GetFrameMenu()->GetSubMenu(1);

	GetListView()->ViewSmallIcons();
	pViewMenu->CheckMenuRadioItem(IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_SMALLICON, 0);

}

void CMainFrame::OnShowHidden()
{
	// Handle the the View submenu
	CMenu* pViewMenu = GetFrameMenu()->GetSubMenu(1);

	m_bShowHidden = !m_bShowHidden;
	pViewMenu->CheckMenuItem(IDM_SHOW_HIDDEN, (TRUE == m_bShowHidden)? MF_CHECKED : MF_UNCHECKED);

	// Refresh the Listview display
	GetListView()->DoDisplay();
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
			if (((LPNMHDR)lParam)->hwndFrom == GetToolBar()->GetHwnd())
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
		CToolBar* pTB = GetToolBar();
		pTB->SetButtonStyle(IDM_VIEWMENU, BTNS_WHOLEDROPDOWN);
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

