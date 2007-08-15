////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class


#include "resource.h"
#include "mainfrm.h"


CMainFrame::CMainFrame()  : m_MainView(200, true), m_bShowHidden(FALSE)
{
	// Set m_MainView as the view window of the frame
	SetView(m_MainView);

	// Define our toolbar data
	m_ToolbarData.clear();
	m_ToolbarData.push_back ( IDM_FILE_NEW   );
	m_ToolbarData.push_back ( IDM_FILE_OPEN  );
	m_ToolbarData.push_back ( IDM_FILE_SAVE  );
	m_ToolbarData.push_back ( 0 );				// Separator
	m_ToolbarData.push_back ( IDM_EDIT_CUT   );
	m_ToolbarData.push_back ( IDM_EDIT_COPY  );
	m_ToolbarData.push_back ( IDM_EDIT_PASTE );
	m_ToolbarData.push_back ( IDM_FILE_PRINT );
	m_ToolbarData.push_back ( 0 );				// Separator
	m_ToolbarData.push_back ( IDM_VIEWMENU);
	m_ToolbarData.push_back ( 0 );				// Separator
	m_ToolbarData.push_back ( IDM_HELP_ABOUT );
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::OnInitialUpdate()
{
	// All windows are now created, so populate the treeview
	GetTreeView().GetRootItems();

	// Uncheck the hidden menu item
	::CheckMenuItem (GetFrameMenu(), IDM_SHOW_HIDDEN, MF_UNCHECKED);

	// Place Radio button in view menu
	HMENU hView = ::GetSubMenu(GetFrameMenu(), 1);
	::CheckMenuRadioItem(hView, IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_REPORT, 0);
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
		GetListView().ViewLargeIcons();
		::CheckMenuRadioItem(hView, IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_LARGEICON, 0);
		return TRUE;
	case IDM_VIEW_SMALLICON:
		GetListView().ViewSmallIcons();
		::CheckMenuRadioItem(hView, IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_SMALLICON, 0);
		return TRUE;
	case IDM_VIEW_LIST:
		GetListView().ViewList();
		::CheckMenuRadioItem(hView, IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_LIST, 0);
		return TRUE;
	case IDM_VIEW_REPORT:
		GetListView().ViewReport();
		::CheckMenuRadioItem(hView, IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_REPORT, 0);
		return TRUE;
	case IDM_SHOW_HIDDEN:
		m_bShowHidden = !m_bShowHidden;
		::CheckMenuItem (GetFrameMenu(), IDM_SHOW_HIDDEN, (m_bShowHidden == TRUE)? MF_CHECKED : MF_UNCHECKED);

		// Refresh the Listview display
		GetListView().DoDisplay();
		return TRUE;
	case IDM_VIEWMENU:
		// This Command is recieved if Comctl32.dll version is below 5.80
		ViewPopup();
		return TRUE;
	} // switch cmd

	return FALSE;

} // CMainFrame::OnCommand(...)

void CMainFrame::OnCreate()
{
	// call the base OnCreate function
	CFrame::OnCreate();

	// Use larger buttons
	CToolbar& TB = GetToolbar();
	TB.SetImageList(9, RGB(192,192,192), IDB_TOOLBAR_NORM, IDB_TOOLBAR_HOT, IDB_TOOLBAR_DIS);

	if (IsRebarUsed())
	{
		// Resize the Rebar band
		CRebar& RB = GetRebar();
		RB.ResizeBand(RB.GetBand(TB.GetHwnd()), TB.GetMaxSize());
	}

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

	SetTheme();
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
				ViewPopup();
		}
		break;

	} //switch LPNMHDR

	return 0L;
}

void CMainFrame::SetTheme()
{
	// Set the rebar theme
	CRebar& RB = GetRebar();
	BOOL T = TRUE;
	BOOL F = FALSE;

	REBARTHEME rt = {0};
	rt.UseThemes= TRUE;
	rt.clrBkGnd1 = RGB(150,190,245);
	rt.clrBkGnd2 = RGB(196,215,250);
	rt.clrBand1  = RGB(220,230,250);
	rt.clrBand2  = RGB( 70,130,220);
	rt.KeepBandsLeft = TRUE;
	rt.LockMenuBand  = TRUE;
	rt.ShortBands    = TRUE;
	rt.RoundBorders  = TRUE;

	// or you could use the following
//	REBARTHEME rt = {T, RGB(150,190,245), RGB(196,215,250), RGB(220,230,250), RGB( 70,130,220), F, T, T, T, T, F};
	RB.SetTheme(rt);

	// Set the toolbar theme
	CToolbar& TB = GetToolbar();

	TOOLBARTHEME tt = {0};
	tt.UseThemes   = TRUE;
	tt.clrHot1     = RGB(255, 230, 190);
	tt.clrHot2     = RGB(255, 190, 100);
	tt.clrPressed1 = RGB(255, 140, 40);
	tt.clrPressed2 = RGB(255, 180, 80);
	tt.clrOutline  = RGB(192, 128, 255);

	// or you could use the following
	// TOOLBARTHEME tt = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(192, 128, 255)};
	TB.SetTheme(tt);

	RecalcLayout();
}

void CMainFrame::ViewPopup()
{
	// One of the toolbar buttons brings up a menu to choose the view mode
	// This function creates the popup menu

	// Position the popup menu
	CToolbar& TB = GetToolbar();
	RECT rc = TB.GetItemRect(TB.CommandToIndex(IDM_VIEWMENU));
	::MapWindowPoints(GetToolbar().GetHwnd(), NULL, (LPPOINT)&rc, 2);

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

		// Fix for an undocumented bug in the Win32 API
		// For Win95 and NT, cbSize needs to be 44
		if ((GetWinVersion() == 1400) || (GetWinVersion() == 2400))
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

LRESULT CMainFrame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
		//Additional messages to be handled go here
//	}

	// pass any unhandled messages on for default processing
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

