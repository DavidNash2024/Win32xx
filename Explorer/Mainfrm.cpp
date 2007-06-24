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

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Handle the the View submenu
	HMENU hView = ::GetSubMenu(GetFrameMenu(), 1); 

	switch (LOWORD(wParam))
	{
	case IDM_FILE_EXIT:
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		break;
	case IDM_HELP_ABOUT:
		OnHelp();
		break;
	case IDM_VIEW_LARGEICON:
		GetListView().ViewLargeIcons();
		::CheckMenuRadioItem(hView, IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_LARGEICON, 0);
		break;
	case IDM_VIEW_SMALLICON:
		GetListView().ViewSmallIcons();
		::CheckMenuRadioItem(hView, IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_SMALLICON, 0);
		break;
	case IDM_VIEW_LIST:
		GetListView().ViewList();
		::CheckMenuRadioItem(hView, IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_LIST, 0);
		break;
	case IDM_VIEW_REPORT:
		GetListView().ViewReport();
		::CheckMenuRadioItem(hView, IDM_VIEW_SMALLICON, IDM_VIEW_REPORT, IDM_VIEW_REPORT, 0);
		break;
	case IDM_SHOW_HIDDEN:
		m_bShowHidden = !m_bShowHidden;
		::CheckMenuItem (GetFrameMenu(), IDM_SHOW_HIDDEN, (m_bShowHidden == TRUE)? MF_CHECKED : MF_UNCHECKED);

		// Refresh the Listview display
		GetListView().DoDisplay();
		break;
	case IDM_VIEWMENU:
		// This Command is recieved if Comctl32.dll version is below 5.80
		ViewPopup();
		break;
	} // switch cmd

	return CFrame::OnCommand(wParam, lParam);

} // CMainFrame::OnCommand(...)

void CMainFrame::OnCreate()
{
//	m_bUseRebar = FALSE;

	CFrame::OnCreate();
}

LRESULT CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam)
{
	// Notification from our dropdown button is recieved if Comctl32.dll version
	// is 5.80 or later (IE v5 required).
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

	//Use the default message handling for remaining messages
	return CFrame::OnNotify(wParam, lParam);
}

void CMainFrame::SetButtons(const std::vector<UINT> ToolbarData)
{
	// A reference to the CToolbar object
	CToolbar& TB = GetToolbar();

	// Set the button size to 24x24 before adding the bitmap
	TB.SetBitmapSize(24, 24);

	// Set the image lists for normal, hot and disabled buttons
	TB.SetImageList(9, RGB(192,192,192), IDB_TOOLBAR_NORM, IDB_TOOLBAR_HOT, IDB_TOOLBAR_DIS);

	// Set the resource IDs for the toolbar buttons
	TB.SetButtons(ToolbarData);

	// Adjust the toolbar and rebar size to take account of the larger buttons
	RECT r;
	TB.GetItemRect(0, &r);
	TB.SetButtonSize(r.right - r.left, r.bottom - r.top);

	// Diable the unused buttons
	TB.DisableButton(IDM_FILE_NEW);   
	TB.DisableButton(IDM_FILE_OPEN); 
	TB.DisableButton(IDM_FILE_SAVE);			
	TB.DisableButton(IDM_EDIT_CUT);
	TB.DisableButton(IDM_EDIT_COPY); 
	TB.DisableButton(IDM_EDIT_PASTE);
	TB.DisableButton(IDM_FILE_PRINT);

	// This style requires comctl32.dll version 5.80 or later
	GetToolbar().SetButtonStyle(IDM_VIEWMENU, BTNS_WHOLEDROPDOWN);

	GetMenubar().SetIcons(m_ToolbarData, IDB_TOOLBAR_NORM, RGB(192,192,192));
}

void CMainFrame::ViewPopup()
{
	// Position the popup menu
	RECT rc= {0};
	::SendMessage(GetToolbar().GetHwnd(), TB_GETRECT, IDM_VIEWMENU, (LPARAM) &rc);
	::MapWindowPoints(GetToolbar().GetHwnd(), NULL, (LPPOINT)&rc, 2);

	if (!IsRebarSupported())
	{
		// For Win95 systems without IE 4, we need to calculate rc differently
		GetWindowRect(GetToolbar().GetHwnd(), &rc);
		rc.left = rc.left + 232;
	}
		
	TPMPARAMS tpm;
	tpm.cbSize = sizeof(TPMPARAMS);
	tpm.rcExclude = rc;

	// Load the popup menu
	HMENU hTopMenu = ::LoadMenu(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDM_VIEWMENU));
	HMENU hPopupMenu = GetSubMenu(hTopMenu, 0);

	// Put a radio check in the currently checked item
	// Doesn't work in Win95 without IE 4 (GetMenuItemInfo isn't supported)
	MENUITEMINFO mii = {0};
	for (int i = 3 ; i < 7 ; i++)
	{
		ZeroMemory(&mii, sizeof(MENUITEMINFO));
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

//	}

	// Use the frame default message handling for remaining messages
	return CFrame::WndProc(hWnd, uMsg, wParam, lParam);
}

