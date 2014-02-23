////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "TabbedMDIApp.h"
#include "mainfrm.h"
#include "resource.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame() : m_pLastActiveDocker(0)
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_DockTabbedMDI as the view window of the frame
	SetView(m_DockTabbedMDI);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\TabbedMDI Docking"));
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

void CMainFrame::LoadDefaultDockers()
{
	// Note: The  DockIDs are used for saving/restoring the dockers state in the registry

	DWORD dwStyle = DS_CLIENTEDGE; // The style added to each docker

	// Add the parent dockers
	CDocker* pDockRight  = m_DockTabbedMDI.AddDockedChild(new CDockClasses, DS_DOCKED_RIGHT | dwStyle, 200, ID_DOCK_CLASSES1);
	CDocker* pDockBottom = m_DockTabbedMDI.AddDockedChild(new CDockText, DS_DOCKED_BOTTOM | dwStyle, 100, ID_DOCK_TEXT1);

	// Add the remaining dockers
	pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | dwStyle, 200, ID_DOCK_FILES1);
	pDockRight->AddDockedChild(new CDockClasses, DS_DOCKED_CONTAINER | dwStyle, 200, ID_DOCK_CLASSES2);
	pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | dwStyle, 200, ID_DOCK_FILES2);

	pDockBottom->AddDockedChild(new CDockOutput, DS_DOCKED_CONTAINER | dwStyle, 100, ID_DOCK_OUTPUT1);
	pDockBottom->AddDockedChild(new CDockText, DS_DOCKED_CONTAINER | dwStyle, 100, ID_DOCK_TEXT2);
	pDockBottom->AddDockedChild(new CDockOutput, DS_DOCKED_CONTAINER | dwStyle, 100, ID_DOCK_OUTPUT2);
}

void CMainFrame::LoadDefaultMDIs()
{
	// Add some MDI tabs
	CTabbedMDI* pTabbedMDI = (CTabbedMDI*)m_DockTabbedMDI.GetView();
	pTabbedMDI->AddMDIChild(new CViewSimple, _T("Simple View"), ID_MDI_SIMPLE);
	pTabbedMDI->AddMDIChild(new CViewRect, _T("Rectangles"), ID_MDI_RECT);
	pTabbedMDI->AddMDIChild(new CViewText, _T("TextView"), ID_MDI_TEXT);
	pTabbedMDI->AddMDIChild(new CViewClasses, _T("Classes"), ID_MDI_CLASSES);
	pTabbedMDI->AddMDIChild(new CViewFiles, _T("Files"), ID_MDI_FILES);

	if (pTabbedMDI->IsWindow())
		pTabbedMDI->SetActiveMDITab(0);
}

void CMainFrame::OnFileNew()
{
	// Creates the popup menu when the "New" toolbar button is pressed

	// Position the popup menu
	CToolBar* pTB = GetToolBar();
	RECT rc = pTB->GetItemRect(pTB->CommandToIndex(IDM_FILE_NEW));
	pTB->ClientToScreen(rc);

	TPMPARAMS tpm;
	tpm.cbSize = sizeof(TPMPARAMS);
	tpm.rcExclude = rc;

	// Load the popup menu
	HMENU hTopMenu = ::LoadMenu(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDM_NEWMENU));
	HMENU hPopupMenu = ::GetSubMenu(hTopMenu, 0);

	// Start the popup menu
	::TrackPopupMenuEx(hPopupMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, rc.left, rc.bottom, m_hWnd, &tpm);

	// Release the menu resource
	::DestroyMenu(hTopMenu);
}

void CMainFrame::OnFileExit()
{
	// The application ends when the frame is destroyed
	Destroy();
}

void CMainFrame::OnFileNewSimple()
{
	CTabbedMDI* pTabbedMDI = (CTabbedMDI*)m_DockTabbedMDI.GetView();
	assert(pTabbedMDI);
	pTabbedMDI->AddMDIChild(new CViewSimple, _T("Simple"), ID_MDI_SIMPLE);
}

void CMainFrame::OnFileNewRect()
{
	CTabbedMDI* pTabbedMDI = (CTabbedMDI*)m_DockTabbedMDI.GetView();
	assert(pTabbedMDI);
	pTabbedMDI->AddMDIChild(new CViewRect, _T("Rectangles"), ID_MDI_RECT);
}

void CMainFrame::OnFileNewList()
{
	CTabbedMDI* pTabbedMDI = (CTabbedMDI*)m_DockTabbedMDI.GetView();
	assert(pTabbedMDI);
	pTabbedMDI->AddMDIChild(new CViewFiles, _T("ListView"), ID_MDI_FILES);
}

void CMainFrame::OnFileNewText()
{
	CTabbedMDI* pTabbedMDI = (CTabbedMDI*)m_DockTabbedMDI.GetView();
	assert(pTabbedMDI);
	pTabbedMDI->AddMDIChild(new CViewText, _T("TextView"), ID_MDI_TEXT);
}

void CMainFrame::OnFileNewTree()
{
	CTabbedMDI* pTabbedMDI = (CTabbedMDI*)m_DockTabbedMDI.GetView();
	assert(pTabbedMDI);
	pTabbedMDI->AddMDIChild(new CViewClasses, _T("TreeView"), ID_MDI_CLASSES);
}

void CMainFrame::OnContainerTabsAtTop()
// Reposition the tabs in the containers
{
	BOOL bTop = FALSE;
	std::vector<DockPtr>::iterator iter;

	// Set the Tab position for each container
	for (iter = m_DockTabbedMDI.GetAllDockers().begin(); iter < m_DockTabbedMDI.GetAllDockers().end(); ++iter)
	{
		CDockContainer* pContainer = (*iter)->GetContainer();
		if (pContainer)
		{
			bTop = pContainer->GetTabsAtTop();
			pContainer->SetTabsAtTop(!bTop);
		}
	}

	// Set the menu checkmark
	UINT uCheck = (bTop)? MF_UNCHECKED : MF_CHECKED;
	::CheckMenuItem(GetFrameMenu(), IDM_CONTAINER_TOP, uCheck);
}

void CMainFrame::OnMDITabsAtTop()
// Reposition TabbedMDI's tabs
{
	CTabbedMDI* pTabbedMDI = m_DockTabbedMDI.GetTabbedMDI();

	BOOL bTop = pTabbedMDI->GetTab().GetTabsAtTop();
	pTabbedMDI->GetTab().SetTabsAtTop(!bTop);

	// Set the menu checkmark
	UINT uCheck = (bTop)? MF_UNCHECKED : MF_CHECKED;
	::CheckMenuItem(GetFrameMenu(), IDM_TABBEDMDI_TOP, uCheck);
}

void CMainFrame::OnDefaultLayout()
{
	SetRedraw(FALSE);

	m_DockTabbedMDI.CloseAllDockers();
	m_DockTabbedMDI.GetTabbedMDI()->CloseAllMDIChildren();
	LoadDefaultDockers();
	LoadDefaultMDIs();

	SetRedraw(TRUE);
	RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// OnCommand responds to menu and and toolbar input
	switch(LOWORD(wParam))
	{
	case IDM_CLOSE_DOCKERS:		OnCloseDockers();		return TRUE;
	case IDM_CLOSE_MDIS:		OnCloseMDIs();			return TRUE;
	case IDM_CONTAINER_TOP:		OnContainerTabsAtTop();	return TRUE;
	case IDM_DEFAULT_LAYOUT:	OnDefaultLayout();		return TRUE;
	case IDM_FILE_NEW:			OnFileNew();			return TRUE;
	case IDM_FILE_NEWSIMPLE:	OnFileNewSimple();		return TRUE;
	case IDM_FILE_NEWRECT:		OnFileNewRect();		return TRUE;
	case IDM_FILE_NEWTEXT:		OnFileNewText();		return TRUE;
	case IDM_FILE_NEWTREE:		OnFileNewTree();		return TRUE;
	case IDM_FILE_NEWLIST:		OnFileNewList();		return TRUE;
	case IDM_FILE_EXIT:			OnFileExit();			return TRUE;
	case IDM_HELP_ABOUT:		OnHelp();				return TRUE;
	case IDM_TABBEDMDI_TOP:		OnMDITabsAtTop();		return TRUE;
	case IDW_VIEW_STATUSBAR:	OnViewStatusBar();		return TRUE;
	case IDW_VIEW_TOOLBAR:		OnViewToolBar();		return TRUE;

	default:
		// Pass the command on to the view window of the last active docker
		{
			if (m_pLastActiveDocker == &m_DockTabbedMDI)
				m_DockTabbedMDI.GetTabbedMDI()->GetActiveMDIChild()->SendMessage(WM_COMMAND, wParam, lParam);
			else if (m_pLastActiveDocker->IsDocked())
				m_pLastActiveDocker->GetContainer()->GetActiveView()->SendMessage(WM_COMMAND, wParam, lParam);
		}
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
	// m_bUseReBar = FALSE;				// Don't use rebars
	// m_bUseThemes = FALSE;            // Don't use themes
	// m_bUseToolBar = FALSE;			// Don't use a toolbar

	// call the base class function
	CFrame::OnCreate();
}

void CMainFrame::OnInitialUpdate()
{
	m_DockTabbedMDI.SetDockStyle(DS_CLIENTEDGE|DS_FIXED_RESIZE);

	// Load dock settings
	if (!m_DockTabbedMDI.LoadRegistrySettings(GetRegistryKeyName()))
		LoadDefaultDockers();

	// Load MDI child settings
	if (!m_DockTabbedMDI.GetTabbedMDI()->LoadRegistrySettings(GetRegistryKeyName()))
		LoadDefaultMDIs();

	// PreCreate initially set the window as invisible, so show it now.
	ShowWindow();
}

void CMainFrame::OnCloseDockers()
{
	m_DockTabbedMDI.CloseAllDockers();
}

void CMainFrame::OnCloseMDIs()
{
	m_DockTabbedMDI.GetTabbedMDI()->CloseAllMDIChildren();
}

void CMainFrame::OnMenuUpdate(UINT nID)
// Called when menu items are about to be displayed
{
	// Only for the Menu IDs we wish to modify
	if (nID >= IDM_EDIT_UNDO && nID <= IDM_EDIT_DELETE)
	{
		CWnd* pWnd = 0;
		CMenu Menu;
		Menu.Attach(GetFrameMenu());
		CMenu* pEditMenu = Menu.GetSubMenu(1);

		if (m_pLastActiveDocker)
		{
			if (m_pLastActiveDocker == &m_DockTabbedMDI)
				pWnd = m_DockTabbedMDI.GetTabbedMDI()->GetActiveMDIChild();
			else if (m_pLastActiveDocker->IsDocked())
				pWnd = m_pLastActiveDocker->GetContainer()->GetActiveView();
		}

		// Enable the Edit menu items for CViewText windows, disable them otherwise
		UINT Flags = (dynamic_cast<CViewText*>(pWnd))? MF_ENABLED : MF_GRAYED;
		pEditMenu->EnableMenuItem(nID, MF_BYCOMMAND | Flags);
		Menu.Detach();
	}
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
	CFrame::SaveRegistrySettings();

	// Save the docker settings
	m_DockTabbedMDI.SaveRegistrySettings(GetRegistryKeyName());

	// Save the tabbedMDI settings
	m_DockTabbedMDI.GetTabbedMDI()->SaveRegistrySettings(GetRegistryKeyName());

	return TRUE;
}

void CMainFrame::SetupToolBar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolBarButton( IDM_FILE_NEW   );
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

	// Remove the checkmark for container tabs at top
	::CheckMenuItem(GetFrameMenu(), IDM_CONTAINER_TOP, MF_UNCHECKED);

	// Add some extra icons for menu items
	AddMenuIcon(IDM_FILE_NEWSIMPLE, (HICON)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDI_SIMPLE), IMAGE_ICON, 0, 0, LR_SHARED));
	AddMenuIcon(IDM_FILE_NEWRECT, (HICON)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDI_RECT), IMAGE_ICON, 0, 0, LR_SHARED));
	AddMenuIcon(IDM_FILE_NEWTEXT, (HICON)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDI_TEXT), IMAGE_ICON, 0, 0, LR_SHARED));
	AddMenuIcon(IDM_FILE_NEWLIST, (HICON)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDI_FILEVIEW), IMAGE_ICON, 0, 0, LR_SHARED));
	AddMenuIcon(IDM_FILE_NEWTREE, (HICON)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDI_CLASSVIEW), IMAGE_ICON, 0, 0, LR_SHARED));
}

LRESULT CMainFrame::OnMouseActivate(WPARAM wParam, LPARAM lParam)
{
	// Store the active docker before processing the menu events
	m_pLastActiveDocker = m_DockTabbedMDI.GetActiveDocker();

	return MA_ACTIVATE;
}


LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MOUSEACTIVATE:	return OnMouseActivate(wParam, lParam);
	}

	// Always pass unhandled messages on to WndProcDefault
	return WndProcDefault(uMsg, wParam, lParam);
}
