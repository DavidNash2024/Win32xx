////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class

#include "stdafx.h"
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

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch(LOWORD(wParam))
	{
	case IDM_FILE_EXIT:			OnFileExit();		return TRUE;
	case IDM_VIEW_TEXT:			OnViewText();		return TRUE;
	case IDM_VIEW_LIST:			OnViewList();		return TRUE;
	case IDW_VIEW_STATUSBAR:	OnViewStatusBar();	return TRUE;
	case IDW_VIEW_TOOLBAR:		OnViewToolBar();	return TRUE;
	case IDM_HELP_ABOUT:		OnHelp();			return TRUE;
	}
	return FALSE;
}

void CMainFrame::OnFileExit()
{
	// End the application
	Destroy();
}

void CMainFrame::OnInitialUpdate()
{
	// Set the styles for splitter panes
	DWORD dwStyle = DS_NO_UNDOCK | DS_NO_CAPTION | DS_CLIENTEDGE;
	m_MainView.SetDockStyle(dwStyle);

	CRect rcView = GetViewRect();

	// Add the bottom pane first. It is a child of the main pane.
	m_pDockTree = (CDockTree*)m_MainView.AddDockedChild(new CDockTree, dwStyle|DS_DOCKED_BOTTOM, rcView.Height()/2);

	// Add the bottom right pane. It is a child of the bottom pane 
	m_pDockList = (CDockList*)m_pDockTree->AddDockedChild(new CDockList, dwStyle|DS_DOCKED_RIGHT, rcView.Width()/2);

	// Add the top right pane. It is a child of the main pane.
	m_pDockText = (CDockText*)m_MainView.AddDockedChild(new CDockText, dwStyle|DS_DOCKED_RIGHT, rcView.Width()/2);

}

void CMainFrame::OnViewList()
{
	DWORD dwStyle = DS_NO_UNDOCK | DS_NO_CAPTION | DS_CLIENTEDGE;
	if (m_pDockList->IsDocked())
	{
		m_pDockList->Hide();
		GetFrameMenu().CheckMenuItem(IDM_VIEW_LIST, MF_UNCHECKED);
	}
	else
	{
		m_pDockTree->Dock(m_pDockList, dwStyle | DS_DOCKED_RIGHT);
		GetFrameMenu().CheckMenuItem(IDM_VIEW_LIST, MF_CHECKED);
	}
}

void CMainFrame::OnViewText()
{
	DWORD dwStyle = DS_NO_UNDOCK | DS_NO_CAPTION | DS_CLIENTEDGE;

	if (m_pDockText->IsDocked())
	{
		m_pDockText->Hide();
		GetFrameMenu().CheckMenuItem(IDM_VIEW_TEXT, MF_UNCHECKED);
	}
	else
	{
		m_MainView.Dock(m_pDockText, dwStyle | DS_DOCKED_RIGHT);
		GetFrameMenu().CheckMenuItem(IDM_VIEW_TEXT, MF_CHECKED);
	}
}

void CMainFrame::SetupToolBar()
{	
	// Define the resource IDs for the toolbar
	AddToolBarButton( IDM_FILE_NEW   );
	AddToolBarButton( IDM_FILE_OPEN  );
	AddToolBarButton( IDM_FILE_SAVE  );
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_EDIT_CUT   );
	AddToolBarButton( IDM_EDIT_COPY  );
	AddToolBarButton( IDM_EDIT_PASTE );
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_FILE_PRINT );
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_HELP_ABOUT );
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//		Add case statements for each messages to be handled here
//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

