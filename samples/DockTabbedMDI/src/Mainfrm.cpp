////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "TabbedMDIApp.h"
#include "mainfrm.h"
#include "resource.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame() : m_pLastActiveDocker(0), m_bContainerTabsAtTop(FALSE), 
                           m_bHideSingleTab(TRUE), m_bMDITabsAtTop(TRUE)
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_MyTabbedMDI as the view window of the frame
	SetView(m_MyTabbedMDI);

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
	CDocker* pDockRight  = AddDockedChild(new CDockClasses, DS_DOCKED_RIGHT | dwStyle, 250, ID_DOCK_CLASSES1);
	CDocker* pDockBottom = AddDockedChild(new CDockText, DS_DOCKED_BOTTOM | dwStyle, 100, ID_DOCK_TEXT1);

	// Add the remaining dockers
	pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | dwStyle, 250, ID_DOCK_FILES1);
	pDockRight->AddDockedChild(new CDockDialog, DS_DOCKED_CONTAINER | dwStyle, 250, ID_DOCK_DIALOG);

	pDockBottom->AddDockedChild(new CDockOutput, DS_DOCKED_CONTAINER | dwStyle, 100, ID_DOCK_OUTPUT1);
	pDockBottom->AddDockedChild(new CDockText, DS_DOCKED_CONTAINER | dwStyle, 100, ID_DOCK_TEXT2);
	pDockBottom->AddDockedChild(new CDockOutput, DS_DOCKED_CONTAINER | dwStyle, 100, ID_DOCK_OUTPUT2);
}

void CMainFrame::LoadDefaultMDIs()
{
	// Add some MDI tabs
	m_MyTabbedMDI.AddMDIChild(new CViewWeb, _T("Browser"), ID_MDI_WEB);
	m_MyTabbedMDI.AddMDIChild(new CViewRect, _T("Rectangles"), ID_MDI_RECT);
	m_MyTabbedMDI.AddMDIChild(new CViewText, _T("TextView"), ID_MDI_TEXT);
	m_MyTabbedMDI.AddMDIChild(new CViewClasses, _T("Classes"), ID_MDI_CLASSES);
	m_MyTabbedMDI.AddMDIChild(new CViewFiles, _T("Files"), ID_MDI_FILES);

	if (m_MyTabbedMDI.IsWindow())
		m_MyTabbedMDI.SetActiveMDITab(0);
}

void CMainFrame::OnFileNew()
// Creates the popup menu when the "New" toolbar button is pressed
{
	// Position the popup menu
	CToolBar* pTB = GetToolBar();
	RECT rc = pTB->GetItemRect(pTB->CommandToIndex(IDM_FILE_NEW));
	pTB->ClientToScreen(rc);

	TPMPARAMS tpm;
	tpm.cbSize = sizeof(TPMPARAMS);
	tpm.rcExclude = rc;

	// Load the popup menu
	CMenu TopMenu(IDM_NEWMENU);
	CMenu* pPopupMenu = TopMenu.GetSubMenu(0);

	// Start the popup menu
	pPopupMenu->TrackPopupMenuEx(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, rc.left, rc.bottom, this, &tpm);
}

void CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
}

void CMainFrame::OnFileNewSimple()
{
	CTabbedMDI* pTabbedMDI = &m_MyTabbedMDI;
	assert(pTabbedMDI);
	pTabbedMDI->AddMDIChild(new CViewWeb, _T("Browser"), ID_MDI_WEB);
}

void CMainFrame::OnFileNewRect()
{
	CTabbedMDI* pTabbedMDI = &m_MyTabbedMDI;
	assert(pTabbedMDI);
	pTabbedMDI->AddMDIChild(new CViewRect, _T("Rectangles"), ID_MDI_RECT);
}

void CMainFrame::OnFileNewList()
{
	CTabbedMDI* pTabbedMDI = &m_MyTabbedMDI;
	assert(pTabbedMDI);
	pTabbedMDI->AddMDIChild(new CViewFiles, _T("ListView"), ID_MDI_FILES);
}

void CMainFrame::OnFileNewText()
{
	CTabbedMDI* pTabbedMDI =  &m_MyTabbedMDI;
	assert(pTabbedMDI);
	pTabbedMDI->AddMDIChild(new CViewText, _T("TextView"), ID_MDI_TEXT);
}

void CMainFrame::OnFileNewTree()
{
	CTabbedMDI* pTabbedMDI = &m_MyTabbedMDI;
	assert(pTabbedMDI);
	pTabbedMDI->AddMDIChild(new CViewClasses, _T("TreeView"), ID_MDI_CLASSES);
}

void CMainFrame::OnContainerTabsAtTop()
// Reposition the tabs in the containers
{
	SetContainerTabsAtTop(!m_bContainerTabsAtTop);
}

void CMainFrame::SetContainerTabsAtTop(BOOL bTop)
{
	m_bContainerTabsAtTop = bTop;
	std::vector<DockPtr>::iterator iter;

	// Set the Tab position for each container
	for (iter = GetAllDockChildren()->begin(); iter < GetAllDockChildren()->end(); ++iter)
	{
		CDockContainer* pContainer = (*iter)->GetContainer();
		if (pContainer && pContainer->IsWindow())
		{
			pContainer->SetTabsAtTop(bTop);
		}
	}

	// Set the menu checkmark
	UINT uCheck = (bTop)? MF_CHECKED : MF_UNCHECKED;
	GetFrameMenu()->CheckMenuItem(IDM_CONTAINER_TOP, uCheck);
}

void CMainFrame::OnMDITabsAtTop()
// Reposition TabbedMDI's tabs
{
	SetMDITabsAtTop(!m_bMDITabsAtTop);
}

void CMainFrame::SetMDITabsAtTop(BOOL bTop)
{
	m_bMDITabsAtTop = bTop;
	CTabbedMDI* pTabbedMDI = &m_MyTabbedMDI;
	pTabbedMDI->GetTab()->SetTabsAtTop(bTop);

	// Set the menu checkmark
	UINT uCheck = (bTop)? MF_CHECKED : MF_UNCHECKED;
	GetFrameMenu()->CheckMenuItem(IDM_TABBEDMDI_TOP, uCheck);
}

CDocker* CMainFrame::NewDockerFromID(int idDock)
{
	CDocker* pDock = NULL;
	switch(idDock)
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
	case ID_DOCK_OUTPUT1:
		pDock = new CDockOutput;
		break;
	case ID_DOCK_OUTPUT2:
		pDock = new CDockOutput;
		break;
	case ID_DOCK_TEXT1:
		pDock = new CDockText;
		break;
	case ID_DOCK_TEXT2:
		pDock = new CDockText;
		break;
	case ID_DOCK_DIALOG:
		pDock = new CDockDialog;
		break;
	default:
		TRACE("Unknown Dock ID\n");
		break;
	}

	return pDock;
}

void CMainFrame::OnDefaultLayout()
{
	SetRedraw(FALSE);

	CloseAllDockers();
	m_MyTabbedMDI.CloseAllMDIChildren();
	LoadDefaultDockers();
	LoadDefaultMDIs();

	SetContainerTabsAtTop(m_bContainerTabsAtTop);
	HideSingleContainerTab(m_bHideSingleTab);
	SetRedraw(TRUE);
	RedrawWindow();
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
	case IDM_HIDE_SINGLE_TAB:	OnHideSingleTab();		return TRUE;
	case IDM_TABBEDMDI_TOP:		OnMDITabsAtTop();		return TRUE;
	case IDW_VIEW_STATUSBAR:	OnViewStatusBar();		return TRUE;
	case IDW_VIEW_TOOLBAR:		OnViewToolBar();		return TRUE;
	case IDW_FIRSTCHILD:
	case IDW_FIRSTCHILD +1:
	case IDW_FIRSTCHILD +2:
	case IDW_FIRSTCHILD +3:
	case IDW_FIRSTCHILD +4:
	case IDW_FIRSTCHILD +5:
	case IDW_FIRSTCHILD +6:
	case IDW_FIRSTCHILD +7:
	case IDW_FIRSTCHILD +8:
		{
			int nTab = LOWORD(wParam) - IDW_FIRSTCHILD;
			m_MyTabbedMDI.SetActiveMDITab(nTab);
			return TRUE;
		}
	case IDW_FIRSTCHILD +9:
		{
			m_MyTabbedMDI.ShowListDialog();
		}

	default:
		{
			// Pass the command on to the view window with focus
			GetFocus()->SendMessage(WM_COMMAND, wParam, lParam);
		}
	}

	return FALSE;
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

void CMainFrame::OnHideSingleTab()
{
	HideSingleContainerTab(!m_bHideSingleTab);
}

void CMainFrame::HideSingleContainerTab(BOOL bHide)
{
	m_bHideSingleTab = bHide;
	std::vector<DockPtr>::iterator iter;

	// Set the Tab position for each container
	for (iter = GetAllDockChildren()->begin(); iter < GetAllDockChildren()->end(); ++iter)
	{
		CDockContainer* pContainer = (*iter)->GetContainer();
		if (pContainer && pContainer->IsWindow())
		{
			pContainer->SetHideSingleTab(bHide);
		}
	}

	// Set the menu checkmark
	UINT uCheck = (bHide)? MF_CHECKED : MF_UNCHECKED;
	GetFrameMenu()->CheckMenuItem(IDM_HIDE_SINGLE_TAB, uCheck);
}


void CMainFrame::OnInitialUpdate()
{
	SetDockStyle(DS_CLIENTEDGE);

	// Load dock settings
	if (!CDocker::LoadDockRegistrySettings(GetRegistryKeyName()))
		LoadDefaultDockers();

	// Load MDI child settings
	if (!m_MyTabbedMDI.LoadRegistrySettings(GetRegistryKeyName()))
		LoadDefaultMDIs();

	// Hide the container's tab if it has just one tab
	HideSingleContainerTab(m_bHideSingleTab);

	// Add a "Window" menu item, positioned 2nd from the right.
	int nMenuPos = GetFrameMenu()->GetMenuItemCount() -1;
	CMenu* pWinMenu = m_MyTabbedMDI.GetListMenu();
	GetFrameMenu()->InsertMenu(nMenuPos, MF_POPUP|MF_BYPOSITION, (UINT_PTR)pWinMenu->GetHandle(), _T("&Window"));

	// Update the menu
	if (IsReBarUsed())
		GetMenuBar()->SetMenu(*GetFrameMenu());
	else
		DrawMenuBar();

	// PreCreate initially set the window as invisible, so show it now.
	ShowWindow();
	RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
}

LRESULT CMainFrame::OnInitMenuPopup(WPARAM wParam, LPARAM lParam)
{
	// Update the "Window" menu 
	m_MyTabbedMDI.GetListMenu();

	return CFrame::OnInitMenuPopup(wParam, lParam);
}

void CMainFrame::OnCloseDockers()
{
	CloseAllDockers();
}

void CMainFrame::OnCloseMDIs()
{
	m_MyTabbedMDI.CloseAllMDIChildren();
}

void CMainFrame::OnMenuUpdate(UINT nID)
// Called when menu items are about to be displayed
{
	// Only for the Menu IDs we wish to modify
	if (nID >= IDM_EDIT_UNDO && nID <= IDM_EDIT_DELETE)
	{
		CWnd* pWnd = 0;
		CMenu* pEditMenu = GetFrameMenu()->GetSubMenu(1);

		if (m_pLastActiveDocker)
		{
			if (m_pLastActiveDocker == this)
				pWnd = m_MyTabbedMDI.GetActiveMDIChild();
			else if (m_pLastActiveDocker->IsDocked())
				pWnd = m_pLastActiveDocker->GetContainer()->GetActiveView();
		}

		// Enable the Edit menu items for CViewText windows, disable them otherwise
		UINT Flags = (dynamic_cast<CViewText*>(pWnd))? MF_ENABLED : MF_GRAYED;
		pEditMenu->EnableMenuItem(nID, MF_BYCOMMAND | Flags);
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
	CDocker::SaveDockRegistrySettings(GetRegistryKeyName());

	// Save the tabbedMDI settings
	m_MyTabbedMDI.SaveRegistrySettings(GetRegistryKeyName());

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
	GetFrameMenu()->CheckMenuItem(IDM_CONTAINER_TOP, MF_UNCHECKED);

	// Add some extra icons for menu items
	AddMenuIcon(IDM_FILE_NEWSIMPLE, (HICON)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDI_SIMPLE), IMAGE_ICON, 0, 0, LR_SHARED));
	AddMenuIcon(IDM_FILE_NEWRECT, (HICON)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDI_RECT), IMAGE_ICON, 0, 0, LR_SHARED));
	AddMenuIcon(IDM_FILE_NEWTEXT, (HICON)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDI_TEXT), IMAGE_ICON, 0, 0, LR_SHARED));
	AddMenuIcon(IDM_FILE_NEWLIST, (HICON)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDI_FILEVIEW), IMAGE_ICON, 0, 0, LR_SHARED));
	AddMenuIcon(IDM_FILE_NEWTREE, (HICON)LoadImage(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDI_CLASSVIEW), IMAGE_ICON, 0, 0, LR_SHARED));
}

LRESULT CMainFrame::OnMouseActivate(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	// Store the active docker before processing the menu events
	m_pLastActiveDocker = GetActiveDocker();

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
