////////////////////////////////////////////////////
// Mainfrm.cpp


#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	m_bProportionalResize = FALSE;
	m_b3DBorder = TRUE;
	m_bUndockable = TRUE;
	m_bUnMoveable = FALSE;
	m_bNoDockLR = FALSE;

	//Set m_DockView as the view window of the frame
	SetView(m_DockView);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\Dock"));
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	// OnCommand responds to menu and and toolbar input

	switch(LOWORD(wParam))
	{
	case IDM_FILE_EXIT:			OnFileExit();		return TRUE;
	case IDM_DOCK_DEFAULT:		OnDockDefault();	return TRUE;
	case IDM_DOCK_CLOSEALL:		OnDockCloseAll();	return TRUE;
	case IDM_PROP_RESIZE:       OnPropResize();		return TRUE;
	case IDM_3DBORDER:			On3DBorder();		return TRUE;
	case IDM_UNDOCKABLE:		OnUndockable();		return TRUE;
	case IDM_NO_RESIZE:			OnNoResize();		return TRUE;
	case IDM_NO_DOCK_LR:		OnNoDockLR();		return TRUE;
	case IDW_VIEW_STATUSBAR:	OnViewStatusBar();	return TRUE;
	case IDW_VIEW_TOOLBAR:		OnViewToolBar();	return TRUE;
	case IDM_HELP_ABOUT:		OnHelp();			return TRUE;
	}

	return FALSE;
}

void CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
}

void CMainFrame::OnDockDefault()
{
	SetRedraw(FALSE);	// Suppress drawing to the frame window
	m_DockView.CloseAllDockers();
	LoadDefaultDockers();
	SetRedraw(TRUE);	// Re-enable drawing to the frame window
	RedrawWindow(0, 0, RDW_INVALIDATE|RDW_FRAME|RDW_UPDATENOW|RDW_ALLCHILDREN);
}

void CMainFrame::OnPropResize()
{
	m_bProportionalResize = !m_bProportionalResize;
	SetDockStyles();
}

void CMainFrame::On3DBorder()
{
	m_b3DBorder = !m_b3DBorder;
	SetDockStyles();
}

void CMainFrame::OnUndockable()
{
	m_bUndockable = !m_bUndockable;
	SetDockStyles();
}

void CMainFrame::OnMenuUpdate(UINT nID)
{
	switch(nID)
	{
	case IDM_PROP_RESIZE:
		GetFrameMenu()->CheckMenuItem(nID, MF_BYCOMMAND | (m_bProportionalResize ? MF_CHECKED : MF_UNCHECKED));
		break;
	case IDM_3DBORDER:
		GetFrameMenu()->CheckMenuItem(nID, MF_BYCOMMAND | (m_b3DBorder ? MF_CHECKED : MF_UNCHECKED));
		break;
	case IDM_UNDOCKABLE:
		GetFrameMenu()->CheckMenuItem(nID, MF_BYCOMMAND | (m_bUndockable ? MF_CHECKED : MF_UNCHECKED));
		break;
	case IDM_NO_RESIZE:
		GetFrameMenu()->CheckMenuItem(nID, MF_BYCOMMAND | (m_bUnMoveable ? MF_CHECKED : MF_UNCHECKED));
		break;
	case IDM_NO_DOCK_LR:
		GetFrameMenu()->CheckMenuItem(nID, MF_BYCOMMAND | (m_bNoDockLR ? MF_CHECKED : MF_UNCHECKED));
		break;
	}	
}

void CMainFrame::OnNoResize()
{
	m_bUnMoveable = !m_bUnMoveable;
	SetDockStyles();
}

void CMainFrame::OnNoDockLR()
{
	m_bNoDockLR = !m_bNoDockLR;
	SetDockStyles();
}

void CMainFrame::OnDockCloseAll()
{
	m_DockView.CloseAllDockers();
}

int CMainFrame::OnCreate(LPCREATESTRUCT pcs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// The default for the following variables is TRUE

	// m_bUseIndicatorStatus = FALSE;	// Don't show keyboard indicators in the StatusBar
	// m_bUseMenuStatus = FALSE;		// Don't show menu descriptions in the StatusBar
	// m_bUseReBar = FALSE;				// Don't use a ReBar
	// m_bUseThemes = FALSE;            // Don't use themes
	// m_bUseToolBar = FALSE;			// Don't use a ToolBar

	// call the base class function
	return CFrame::OnCreate(pcs);
}

void CMainFrame::OnInitialUpdate()
{
	m_DockView.SetDockStyle(DS_CLIENTEDGE);

	// Load dock settings
	if (!m_DockView.LoadRegistrySettings(GetRegistryKeyName()))
		LoadDefaultDockers();

	// PreCreate initially set the window as invisible, so show it now.
	ShowWindow();
}

void CMainFrame::LoadDefaultDockers()
{
	// Note: The  DockIDs are used for saving/restoring the dockers state in the registry

	DWORD dwStyle = DS_CLIENTEDGE | DS_FIXED_RESIZE; // The style added to each docker

	CDocker* pDockLeft   = m_DockView.AddDockedChild(new CDockClasses, DS_DOCKED_LEFT | dwStyle, 200, ID_DOCK_CLASSES1);
	CDocker* pDockRight  = m_DockView.AddDockedChild(new CDockClasses, DS_DOCKED_RIGHT | dwStyle, 200, ID_DOCK_CLASSES2);
	CDocker* pDockTop    = m_DockView.AddDockedChild(new CDockText, DS_DOCKED_TOP | dwStyle, 100, ID_DOCK_TEXT1);
	CDocker* pDockBottom = m_DockView.AddDockedChild(new CDockText, DS_DOCKED_BOTTOM | dwStyle, 100, ID_DOCK_TEXT2);

	pDockLeft->AddDockedChild(new CDockFiles, DS_DOCKED_BOTTOM | dwStyle, 150, ID_DOCK_FILES1);
	pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_BOTTOM | dwStyle, 150, ID_DOCK_FILES2);
	pDockTop->AddDockedChild(new CDockSimple, DS_DOCKED_RIGHT | dwStyle, 100, ID_DOCK_SIMPLE1);
	pDockBottom->AddDockedChild(new CDockSimple, DS_DOCKED_RIGHT | dwStyle, 100, ID_DOCK_SIMPLE2);
}

void CMainFrame::PreCreate(CREATESTRUCT &cs)
{
	// Call the base class function first
	CFrame::PreCreate(cs);
	
	// Hide the window initially by removing the WS_VISIBLE style
	cs.style &= ~WS_VISIBLE;
}

BOOL CMainFrame::SaveRegistrySettings()
{
	if (CFrame::SaveRegistrySettings())
		return m_DockView.SaveRegistrySettings(GetRegistryKeyName());
	else
		return FALSE;
}

void CMainFrame::SetDockStyles()
{
	std::vector<DockPtr> AllDockers = m_DockView.GetAllDockers();
	std::vector<DockPtr>::iterator iter;

	for (iter = AllDockers.begin(); iter < AllDockers.end(); ++iter)
	{
		DWORD dwStyle = (*iter)->GetDockStyle();
		
		// Filter unwanted styles
		dwStyle &= 0xF400F;
	
		// Add styles selected from the menu
		if (!m_bProportionalResize) dwStyle |= DS_FIXED_RESIZE;
		if (m_b3DBorder)			dwStyle |= DS_CLIENTEDGE;
		if (!m_bUndockable)			dwStyle |= DS_NO_UNDOCK;
		if (m_bUnMoveable)			dwStyle |= DS_NO_RESIZE;
		if (m_bNoDockLR)			dwStyle |= DS_NO_DOCKCHILD_LEFT;// | DS_NO_DOCKCHILD_RIGHT;

		(*iter)->SetDockStyle(dwStyle);
	}
}

void CMainFrame::SetupToolBar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolBarButton( IDM_FILE_NEW,   FALSE );
	AddToolBarButton( IDM_FILE_OPEN,  FALSE );
	AddToolBarButton( IDM_FILE_SAVE,  FALSE );
	
	AddToolBarButton( 0 );	// Separator
	AddToolBarButton( IDM_EDIT_CUT,   FALSE );
	AddToolBarButton( IDM_EDIT_COPY,  FALSE );
	AddToolBarButton( IDM_EDIT_PASTE, FALSE );
	
	AddToolBarButton( 0 );	// Separator
	AddToolBarButton( IDM_FILE_PRINT, FALSE );
	
	AddToolBarButton( 0 );	// Separator
	AddToolBarButton( IDM_HELP_ABOUT );
}
