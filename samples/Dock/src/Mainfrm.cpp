////////////////////////////////////////////////////
// Mainfrm.cpp


#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	m_UseProportionalResize = FALSE;
	m_Use3DBorder = TRUE;
	m_DisableUndocking = FALSE;
	m_DisableResize = FALSE;
	m_DisableDockLR = FALSE;
	m_DisableDockClose = FALSE;
	m_UseDynamicResize = TRUE;

	//Set m_View as the view window of the frame
	SetView(m_View);

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
	case IDM_NO_UNDOCK:			OnNoUndocking();	return TRUE;
	case IDM_NO_RESIZE:			OnNoResize();		return TRUE;
	case IDM_NO_DOCK_LR:		OnNoDockLR();		return TRUE;
	case IDM_NO_DOCK_CLOSE:		OnNoDockClose();	return TRUE;
	case IDM_DYNAMIC_RESIZE:	OnDynamicResize();	return TRUE;
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
	CloseAllDockers();
	LoadDefaultDockers();
	SetRedraw(TRUE);	// Re-enable drawing to the frame window
	RedrawWindow(0, 0, RDW_INVALIDATE|RDW_FRAME|RDW_UPDATENOW|RDW_ALLCHILDREN);
}

void CMainFrame::OnPropResize()
{
	m_UseProportionalResize = !m_UseProportionalResize;
	SetDockStyles();
}

void CMainFrame::On3DBorder()
{
	m_Use3DBorder = !m_Use3DBorder;
	SetDockStyles();
}

void CMainFrame::OnNoUndocking()
{
	m_DisableUndocking = !m_DisableUndocking;
	SetDockStyles();
}

void CMainFrame::OnMenuUpdate(UINT nID)
{
	switch(nID)
	{
	case IDM_PROP_RESIZE:
		GetFrameMenu().CheckMenuItem(nID, MF_BYCOMMAND | (m_UseProportionalResize ? MF_CHECKED : MF_UNCHECKED));
		break;
	case IDM_3DBORDER:
		GetFrameMenu().CheckMenuItem(nID, MF_BYCOMMAND | (m_Use3DBorder ? MF_CHECKED : MF_UNCHECKED));
		break;
	case IDM_NO_UNDOCK:
		GetFrameMenu().CheckMenuItem(nID, MF_BYCOMMAND | (m_DisableUndocking ? MF_CHECKED : MF_UNCHECKED));
		break;
	case IDM_NO_RESIZE:
		GetFrameMenu().CheckMenuItem(nID, MF_BYCOMMAND | (m_DisableResize ? MF_CHECKED : MF_UNCHECKED));
		break;
	case IDM_NO_DOCK_LR:
		GetFrameMenu().CheckMenuItem(nID, MF_BYCOMMAND | (m_DisableDockLR ? MF_CHECKED : MF_UNCHECKED));
		break;
	case IDM_NO_DOCK_CLOSE:
		GetFrameMenu().CheckMenuItem(nID, MF_BYCOMMAND | (m_DisableDockClose ? MF_CHECKED : MF_UNCHECKED));
		break;
	case IDM_DYNAMIC_RESIZE:
		GetFrameMenu().CheckMenuItem(nID, MF_BYCOMMAND | (m_UseDynamicResize ? MF_CHECKED : MF_UNCHECKED));
		break;
	}

	CFrame::OnMenuUpdate(nID);
}

void CMainFrame::OnNoResize()
{
	m_DisableResize = !m_DisableResize;
	SetDockStyles();
}

void CMainFrame::OnNoDockLR()
{
	m_DisableDockLR = !m_DisableDockLR;
	SetDockStyles();
}

void CMainFrame::OnNoDockClose()
{
	m_DisableDockClose = !m_DisableDockClose;
	SetDockStyles();
	RedrawWindow();
} 

void CMainFrame::OnDockCloseAll()
{
	CloseAllDockers();
}

void CMainFrame::OnDynamicResize()
{
	// Dragging the docker's splitter bar will either dynamicly resize the dockers
	// during the dragging, or simply display a hashed splitter bar.
	std::vector<CDocker*>::iterator iter;
	m_UseDynamicResize = !m_UseDynamicResize;

	for (iter = GetAllDockers()->begin(); iter < GetAllDockers()->end(); ++iter)
	{
		(*iter)->SetDragAutoResize(m_UseDynamicResize);
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT pcs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// The default for the following variables is TRUE

	// m_UseIndicatorStatus = FALSE;	// Don't show keyboard indicators in the StatusBar
	// m_UseMenuStatus = FALSE;			// Don't show menu descriptions in the StatusBar
	// m_UseReBar = FALSE;				// Don't use a ReBar
	// m_UseThemes = FALSE;				// Don't use themes
	// m_UseToolBar = FALSE;			// Don't use a ToolBar

	// call the base class function
	return CFrame::OnCreate(pcs);
}

void CMainFrame::OnInitialUpdate()
{
	SetDockStyle(DS_CLIENTEDGE);

	// Load dock settings
	if (!LoadDockRegistrySettings(GetRegistryKeyName()))
		LoadDefaultDockers();

	// Adjust dockstyles as per menu selections
	SetDockStyles();

	// PreCreate initially set the window as invisible, so show it now.
	ShowWindow(m_ShowCmd);
}

void CMainFrame::LoadDefaultDockers()
{
	// Note: The  DockIDs are used for saving/restoring the dockers state in the registry

	CDocker* pDockLeft   = AddDockedChild(new CDockClasses, DS_DOCKED_LEFT, 200, ID_DOCK_CLASSES1);
	CDocker* pDockRight  = AddDockedChild(new CDockClasses, DS_DOCKED_RIGHT, 200, ID_DOCK_CLASSES2);
	CDocker* pDockTop    = AddDockedChild(new CDockText, DS_DOCKED_TOP, 100, ID_DOCK_TEXT1);
	CDocker* pDockBottom = AddDockedChild(new CDockText, DS_DOCKED_BOTTOM, 100, ID_DOCK_TEXT2);

	pDockLeft->AddDockedChild(new CDockFiles, DS_DOCKED_BOTTOM, 150, ID_DOCK_FILES1);
	pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_BOTTOM, 150, ID_DOCK_FILES2);
	pDockTop->AddDockedChild(new CDockSimple, DS_DOCKED_RIGHT, 100, ID_DOCK_SIMPLE1);
	pDockBottom->AddDockedChild(new CDockSimple, DS_DOCKED_RIGHT, 100, ID_DOCK_SIMPLE2);

	// Adjust dockstyles as per menu selections
	SetDockStyles();
}

CDocker* CMainFrame::NewDockerFromID(int nID)
{
	CDocker* pDock = NULL;
	switch(nID)
	{
	case ID_DOCK_CLASSES1:
		pDock = new CDockClasses;
		break;
	case ID_DOCK_CLASSES2:
		pDock = new CDockClasses;					
		break;
	case ID_DOCK_FILES1:
		pDock = new CDockFiles;
		break;
	case ID_DOCK_FILES2:
		pDock = new CDockFiles;
		break;
	case ID_DOCK_SIMPLE1:
		pDock = new CDockSimple;
		break;
	case ID_DOCK_SIMPLE2:
		pDock = new CDockSimple;
		break;
	case ID_DOCK_TEXT1:
		pDock = new CDockText;
		break;
	case ID_DOCK_TEXT2:
		pDock = new CDockText;
		break;
	default:
		TRACE("Unknown Dock ID\n");
		break;
	}

	return pDock;
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
		return SaveDockRegistrySettings(GetRegistryKeyName());
	else
		return FALSE;
}

void CMainFrame::SetDockStyles()
{
	std::vector<CDocker*>::iterator iter;

	for (iter = GetAllDockers()->begin(); iter < GetAllDockers()->end(); ++iter)
	{
		DWORD dwStyle = (*iter)->GetDockStyle();
		
		// Filter unwanted styles
		dwStyle &= 0xF400F;
	
		// Add styles selected from the menu
		if (m_UseProportionalResize)	dwStyle |= DS_NO_FIXED_RESIZE;
		if (m_Use3DBorder)				dwStyle |= DS_CLIENTEDGE;
		if (m_DisableUndocking)			dwStyle |= DS_NO_UNDOCK;
		if (m_DisableResize)			dwStyle |= DS_NO_RESIZE;
		if (m_DisableDockLR)			dwStyle |= DS_NO_DOCKCHILD_LEFT | DS_NO_DOCKCHILD_RIGHT;
		if (m_DisableDockClose)			dwStyle |= DS_NO_CLOSE;

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
