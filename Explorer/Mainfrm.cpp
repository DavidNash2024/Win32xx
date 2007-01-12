////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class


#include "resource.h"
#include "mainfrm.h"


CMainFrame::CMainFrame()  : m_MainView(200, true), m_bShowHidden(FALSE)
{
	// Set m_MainView as the view window of the frame
	SetView(m_MainView);

	// Define our toolbar data
	BYTE ToolbarData[][2] =
	{
		{BUTTON,    IDM_FILE_NEW},
		{BUTTON,    IDM_FILE_OPEN},
		{BUTTON,    IDM_FILE_SAVE},
		{SEPARATOR, 0},
		{BUTTON,    IDM_EDIT_CUT},
		{BUTTON,    IDM_EDIT_COPY},
		{BUTTON,    IDM_EDIT_PASTE},
		{BUTTON,    IDM_FILE_PRINT},
		{SEPARATOR, 0},
		{BUTTON,    IDM_VIEW_LARGEICON},
		{BUTTON,    IDM_VIEW_SMALLICON},
		{BUTTON,    IDM_VIEW_LIST},
        {BUTTON,    IDM_VIEW_REPORT},
		{SEPARATOR, 0},
		{BUTTON,    IDM_HELP_ABOUT}
	};

	int nButtons = sizeof(ToolbarData)/(2*sizeof(BYTE));
	SetToolbarData(nButtons, ToolbarData);
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
}

BOOL CMainFrame::OnCommand(UINT nID)
{
	CFrame::OnCommand(nID);

	switch (nID)
	{
	case IDM_VIEW_LARGEICON:
		GetListView().ViewLargeIcons();
		break;
	case IDM_VIEW_SMALLICON:
		GetListView().ViewSmallIcons();
		break;
	case IDM_VIEW_LIST:
		GetListView().ViewList();
		break;
	case IDM_VIEW_REPORT:
		GetListView().ViewReport();
		break;
	case IDM_SHOW_HIDDEN:
		m_bShowHidden = !m_bShowHidden;
		::CheckMenuItem (GetFrameMenu(), IDM_SHOW_HIDDEN, (m_bShowHidden == TRUE)? MF_CHECKED : MF_UNCHECKED);

		// Refresh the Listview display
		GetListView().DoDisplay();
		break;
	} // switch cmd

	return 0;

} // CMainFrame::OnCommand(...)

LRESULT CMainFrame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{

//	}

	// Use the frame default message handling for remaining messages
	return CFrame::WndProc(hWnd, uMsg, wParam, lParam);
}

