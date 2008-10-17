////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class


#include "mainfrm.h"
#include "resource.h"


CMainFrame::CMainFrame()
{
	SetView(m_MainView);

	// Define the resource IDs for the toolbar
	m_ToolbarData.clear();
	m_ToolbarData.push_back ( IDM_FILE_NEW   );
	m_ToolbarData.push_back ( IDM_FILE_OPEN  );
	m_ToolbarData.push_back ( IDM_FILE_SAVE  );
	m_ToolbarData.push_back ( 0 );				// Separator
	m_ToolbarData.push_back ( IDM_EDIT_CUT   );
	m_ToolbarData.push_back ( IDM_EDIT_COPY  );
	m_ToolbarData.push_back ( IDM_EDIT_PASTE );
	m_ToolbarData.push_back ( 0 );				// Separator
	m_ToolbarData.push_back ( IDM_FILE_PRINT );
	m_ToolbarData.push_back ( 0 );				// Separator
	m_ToolbarData.push_back ( IDM_HELP_ABOUT );

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\Splitter Sample"));
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	switch(LOWORD(wParam))
	{
	case IDM_FILE_EXIT:
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	case IDM_HELP_ABOUT:
		OnHelp();
		return TRUE;
	}
	return FALSE;
}

void CMainFrame::OnInitialUpdate()
{
	// Set the styles for splitter panes
	DWORD dwStyle = DS_NO_UNDOCK | DS_NO_CAPTION | DS_CLIENTEDGE;
	m_MainView.SetDockStyle(dwStyle);

	CRect rcView = GetViewRect();

	// Add the bottom pane first. It is a child of the main pane.
	CDockable* pDockBottom = m_MainView.AddDockedChild(new CPaneBottomLeft, dwStyle|DS_DOCKED_BOTTOM, rcView.Height()/2);

	// Add the bottom right pane. It is a child of the bottom pane 
	pDockBottom->AddDockedChild(new CPaneBottomRight, dwStyle|DS_DOCKED_RIGHT, rcView.Width()/2);

	// Add the top right pane. It is a child of the main pane.
	m_MainView.AddDockedChild(new CPaneTopRight, dwStyle|DS_DOCKED_RIGHT, rcView.Width()/2);

}

LRESULT CMainFrame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{

//	}

	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}
