////////////////////////////////////////////////////
// Mainfrm.cpp


#include "resource.h"
#include "mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_DockView as the view window of the frame
	SetView(m_DockView);

	// Set the Resource IDs for the toolbar buttons
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
	LoadRegistrySettings(_T("Win32++\\Frame"));
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	// OnCommand responds to menu and and toolbar input

	switch(LOWORD(wParam))
	{
	case IDM_FILE_EXIT:
		// End the application
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	case IDM_HELP_ABOUT:
		// Display the help dialog
		OnHelp();
		return TRUE;
	}

	return FALSE;
}

void CMainFrame::OnCreate()
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// The default for the following variables is TRUE

	// m_bShowIndicatorStatus = FALSE;	// Don't show statusbar indicators
	// m_bShowMenuStatus = FALSE;		// Don't show toolbar or menu status
	// m_bUseRebar = FALSE;				// Don't use rebars
	// m_bUseThemes = FALSE;            // Don't use themes

	// call the base class function
	CFrame::OnCreate();
}

void CMainFrame::OnInitialUpdate()
{
	m_DockView.SetDockStyle(DS_CLIENTEDGE);
	CDockable* pDockLeft   = m_DockView.AddDockedChild(new CTextDockable, DS_DOCKED_LEFT|DS_CLIENTEDGE, 100);
	CDockable* pDockRight  = m_DockView.AddDockedChild(new CTextDockable, DS_DOCKED_RIGHT|DS_CLIENTEDGE, 100);
	CDockable* pDockTop    = m_DockView.AddDockedChild(new CTextDockable, DS_DOCKED_TOP|DS_CLIENTEDGE, 100);
	CDockable* pDockBottom = m_DockView.AddDockedChild(new CTextDockable, DS_DOCKED_BOTTOM|DS_CLIENTEDGE, 100);

	pDockLeft->AddDockedChild(new CTextDockable, DS_DOCKED_BOTTOM|DS_CLIENTEDGE, 100);
	pDockRight->AddDockedChild(new CTextDockable, DS_DOCKED_BOTTOM|DS_CLIENTEDGE, 100);
	pDockTop->AddDockedChild(new CTextDockable, DS_DOCKED_RIGHT|DS_CLIENTEDGE, 100);
	pDockBottom->AddDockedChild(new CTextDockable, DS_DOCKED_RIGHT|DS_CLIENTEDGE, 100);
}



