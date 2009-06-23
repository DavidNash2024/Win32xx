////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class


#include "mainfrm.h"
#include "resource.h"


CMainFrame::CMainFrame()
{
	SetView(m_MainView);

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
	case IDW_VIEW_STATUSBAR:
		OnViewStatusbar();
		return TRUE;
	case IDW_VIEW_TOOLBAR:
		OnViewToolbar();
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
	CDocker* pDockBottom = m_MainView.AddDockedChild(new CPaneBottomLeft, dwStyle|DS_DOCKED_BOTTOM, rcView.Height()/2);

	// Add the bottom right pane. It is a child of the bottom pane 
	pDockBottom->AddDockedChild(new CPaneBottomRight, dwStyle|DS_DOCKED_RIGHT, rcView.Width()/2);

	// Add the top right pane. It is a child of the main pane.
	m_MainView.AddDockedChild(new CPaneTopRight, dwStyle|DS_DOCKED_RIGHT, rcView.Width()/2);

}

void CMainFrame::SetupToolbar()
{	
	// Define the resource IDs for the toolbar
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
}

LRESULT CMainFrame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//		Add case statements for each messages to be handled here
//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

