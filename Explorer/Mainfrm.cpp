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
	m_ToolbarData.push_back ( IDM_VIEW_LARGEICON);
	m_ToolbarData.push_back ( IDM_VIEW_SMALLICON);
	m_ToolbarData.push_back ( IDM_VIEW_LIST  );
	m_ToolbarData.push_back ( IDM_VIEW_REPORT);
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

BOOL CMainFrame::OnCommand(UINT nID)
{
	// Handle the the View submenu
	HMENU hView = ::GetSubMenu(GetFrameMenu(), 1); 

	switch (nID)
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
	} // switch cmd

	return CFrame::OnCommand(nID);

} // CMainFrame::OnCommand(...)

LRESULT CMainFrame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{

//	}

	// Use the frame default message handling for remaining messages
	return CFrame::WndProc(hWnd, uMsg, wParam, lParam);
}

