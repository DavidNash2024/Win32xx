////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "TabbedMDIApp.h"
#include "mainfrm.h"
#include "resource.h"


// Definitions for the CMainDockFrame class
CMainDockFrame::CMainDockFrame() : m_pLastActiveDocker(0), m_IsContainerTabsAtTop(FALSE), 
                           m_IsHideSingleTab(TRUE), m_IsMDITabsAtTop(TRUE)
{
	// Constructor for CMainDockFrame. Its called after CFrame's constructor

	//Set m_MyTabbedMDI as the view window of the frame
	SetView(m_MyTabbedMDI);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\TabbedMDI Docking"));
}

CMainDockFrame::~CMainDockFrame()
{
	// Destructor for CMainDockFrame.
}

void CMainDockFrame::LoadDefaultDockers()
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

void CMainDockFrame::LoadDefaultMDIs()
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

BOOL CMainDockFrame::OnFileNew()
// Creates the popup menu when the "New" toolbar button is pressed
{
	// Position the popup menu
	CToolBar& TB = GetToolBar();
	RECT rc = TB.GetItemRect(TB.CommandToIndex(IDM_FILE_NEW));
	TB.ClientToScreen(rc);

	TPMPARAMS tpm;
	tpm.cbSize = sizeof(TPMPARAMS);
	tpm.rcExclude = rc;

	// Load the popup menu
	CMenu TopMenu(IDM_NEWMENU);
	CMenu PopupMenu = TopMenu.GetSubMenu(0);

	// Start the popup menu
	PopupMenu.TrackPopupMenuEx(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, rc.left, rc.bottom, *this, &tpm);
	return TRUE;
}

BOOL CMainDockFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
	return TRUE;
}

BOOL CMainDockFrame::OnFileNewSimple()
{
	m_MyTabbedMDI.AddMDIChild(new CViewWeb, _T("Browser"), ID_MDI_WEB);
	return TRUE;
}

BOOL CMainDockFrame::OnFileNewRect()
{
	m_MyTabbedMDI.AddMDIChild(new CViewRect, _T("Rectangles"), ID_MDI_RECT);
	return TRUE;
}

BOOL CMainDockFrame::OnFileNewList()
{
	m_MyTabbedMDI.AddMDIChild(new CViewFiles, _T("ListView"), ID_MDI_FILES);
	return TRUE;
}

BOOL CMainDockFrame::OnFileNewText()
{
	m_MyTabbedMDI.AddMDIChild(new CViewText, _T("TextView"), ID_MDI_TEXT);
	return TRUE;
}

BOOL CMainDockFrame::OnFileNewTree()
{
	m_MyTabbedMDI.AddMDIChild(new CViewClasses, _T("TreeView"), ID_MDI_CLASSES);
	return TRUE;
}

BOOL CMainDockFrame::OnContainerTabsAtTop()
// Reposition the tabs in the containers
{
	SetContainerTabsAtTop(!m_IsContainerTabsAtTop);
	return TRUE;
}

void CMainDockFrame::SetContainerTabsAtTop(BOOL bTop)
{
	m_IsContainerTabsAtTop = bTop;
	std::vector<DockPtr>::iterator iter;

	// Set the Tab position for each container
	for (iter = GetAllDockChildren().begin(); iter < GetAllDockChildren().end(); ++iter)
	{
		CDockContainer* pContainer = (*iter)->GetContainer();
		if (pContainer && pContainer->IsWindow())
		{
			pContainer->SetTabsAtTop(bTop);
		}
	}

	// Set the menu checkmark
	UINT uCheck = (bTop)? MF_CHECKED : MF_UNCHECKED;
	GetFrameMenu().CheckMenuItem(IDM_CONTAINER_TOP, uCheck);
}

BOOL CMainDockFrame::OnMDITabsAtTop()
// Reposition TabbedMDI's tabs
{
	SetMDITabsAtTop(!m_IsMDITabsAtTop);
	return TRUE;
}

void CMainDockFrame::SetMDITabsAtTop(BOOL bTop)
{
	m_IsMDITabsAtTop = bTop;
	m_MyTabbedMDI.GetTab().SetTabsAtTop(bTop);

	// Set the menu checkmark
	UINT uCheck = (bTop)? MF_CHECKED : MF_UNCHECKED;
	GetFrameMenu().CheckMenuItem(IDM_TABBEDMDI_TOP, uCheck);
}

CDocker* CMainDockFrame::NewDockerFromID(int idDock)
{
	CDocker* pDocker = NULL;
	switch(idDock)
	{
	case ID_DOCK_CLASSES1:
		pDocker = new CDockClasses;
		break;
	case ID_DOCK_CLASSES2:
		pDocker = new CDockClasses;					
		break;
	case ID_DOCK_FILES1:
		pDocker = new CDockFiles;
		break;
	case ID_DOCK_FILES2:
		pDocker = new CDockFiles;
		break;
	case ID_DOCK_OUTPUT1:
		pDocker = new CDockOutput;
		break;
	case ID_DOCK_OUTPUT2:
		pDocker = new CDockOutput;
		break;
	case ID_DOCK_TEXT1:
		pDocker = new CDockText;
		break;
	case ID_DOCK_TEXT2:
		pDocker = new CDockText;
		break;
	case ID_DOCK_DIALOG:
		pDocker = new CDockDialog;
		break;
	default:
		TRACE("Unknown Dock ID\n");
		break;
	}

	return pDocker;
}

BOOL CMainDockFrame::OnDefaultLayout()
{
	SetRedraw(FALSE);

	CloseAllDockers();
	m_MyTabbedMDI.CloseAllMDIChildren();
	LoadDefaultDockers();
	LoadDefaultMDIs();

	SetContainerTabsAtTop(m_IsContainerTabsAtTop);
	HideSingleContainerTab(m_IsHideSingleTab);
	SetRedraw(TRUE);
	RedrawWindow();
	return TRUE;
}

BOOL CMainDockFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// OnCommand responds to menu and and toolbar input
	switch(LOWORD(wParam))
	{
	case IDM_CLOSE_DOCKERS:		return OnCloseDockers();
	case IDM_CLOSE_MDIS:		return OnCloseMDIs();
	case IDM_CONTAINER_TOP:		return OnContainerTabsAtTop();
	case IDM_DEFAULT_LAYOUT:	return OnDefaultLayout();
	case IDM_FILE_NEW:			return OnFileNew();
	case IDM_FILE_NEWSIMPLE:	return OnFileNewSimple();
	case IDM_FILE_NEWRECT:		return OnFileNewRect();
	case IDM_FILE_NEWTEXT:		return OnFileNewText();
	case IDM_FILE_NEWTREE:		return OnFileNewTree();
	case IDM_FILE_NEWLIST:		return OnFileNewList();
	case IDM_FILE_EXIT:			return OnFileExit();
	case IDM_HELP_ABOUT:		return OnHelp();
	case IDM_HIDE_SINGLE_TAB:	return OnHideSingleTab();
	case IDM_TABBEDMDI_TOP:		return OnMDITabsAtTop();
	case IDW_VIEW_STATUSBAR:	return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:		return OnViewToolBar();
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
			return TRUE;
		}

	default:
		{
			// Pass the command on to the view window with focus
			GetFocus().SendMessage(WM_COMMAND, wParam, lParam);
		}
	}

	return FALSE;
}

int CMainDockFrame::OnCreate(LPCREATESTRUCT pcs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// Uncomment the lines below to change frame options.

	// SetUseIndicatorStatus(FALSE);	// Don't show keyboard indicators in the StatusBar
	// SetUseMenuStatus(FALSE);			// Don't show menu descriptions in the StatusBar
	// SetUseReBar(FALSE);				// Don't use a ReBar
	// SetUseThemes(FALSE);				// Don't use themes
	// SetUseToolBar(FALSE);			// Don't use a ToolBar

	// call the base class function
	return CFrame::OnCreate(pcs);
}

BOOL CMainDockFrame::OnHideSingleTab()
{
	HideSingleContainerTab(!m_IsHideSingleTab);
	return TRUE;
}

void CMainDockFrame::HideSingleContainerTab(BOOL HideSingle)
{
	m_IsHideSingleTab = HideSingle;
	std::vector<DockPtr>::iterator iter;

	// Set the Tab position for each container
	for (iter = GetAllDockChildren().begin(); iter < GetAllDockChildren().end(); ++iter)
	{
		CDockContainer* pContainer = (*iter)->GetContainer();
		if (pContainer && pContainer->IsWindow())
		{
			pContainer->SetHideSingleTab(HideSingle);
		}
	}

	// Set the menu checkmark
	UINT uCheck = (HideSingle)? MF_CHECKED : MF_UNCHECKED;
	GetFrameMenu().CheckMenuItem(IDM_HIDE_SINGLE_TAB, uCheck);
}


void CMainDockFrame::OnInitialUpdate()
{
	SetDockStyle(DS_CLIENTEDGE);

	// Load dock settings
	if (!LoadDockRegistrySettings(GetRegistryKeyName()))
		LoadDefaultDockers();

	// Load MDI child settings
	if (!m_MyTabbedMDI.LoadRegistrySettings(GetRegistryKeyName()))
		LoadDefaultMDIs();

	// Hide the container's tab if it has just one tab
	HideSingleContainerTab(m_IsHideSingleTab);

	// Get a copy of the Frame's menu
	CMenu FrameMenu = GetFrameMenu();

	// Modify the menu
	int nMenuPos = FrameMenu.GetMenuItemCount() -1;
	CMenu WinMenu = m_MyTabbedMDI.GetListMenu();
	FrameMenu.InsertPopupMenu(nMenuPos, MF_BYPOSITION, WinMenu, _T("&Window"));

	// Replace the frame's menu with our modified menu
	SetFrameMenu(FrameMenu);

	// PreCreate initially set the window as invisible, so show it now.
	ShowWindow( GetShowCmd() );
	RedrawWindow(0, 0, RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
}

LRESULT CMainDockFrame::OnInitMenuPopup(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Update the "Window" menu 
	m_MyTabbedMDI.GetListMenu();

	return CFrame::OnInitMenuPopup(uMsg, wParam, lParam);
}

BOOL CMainDockFrame::OnCloseDockers()
{
	CloseAllDockers();
	return TRUE;
}

BOOL CMainDockFrame::OnCloseMDIs()
{
	m_MyTabbedMDI.CloseAllMDIChildren();
	return TRUE;
}

void CMainDockFrame::OnMenuUpdate(UINT nID)
// Called when menu items are about to be displayed
{
	// Only for the Menu IDs we wish to modify
	if (nID >= IDM_EDIT_UNDO && nID <= IDM_EDIT_DELETE)
	{
		CWnd* pWnd = 0;
		CMenu EditMenu = GetFrameMenu().GetSubMenu(1);

		if (m_pLastActiveDocker)
		{
			if (m_pLastActiveDocker == this)
				pWnd = m_MyTabbedMDI.GetActiveMDIChild();
			else if (m_pLastActiveDocker->IsDocked())
				pWnd = m_pLastActiveDocker->GetContainer()->GetActiveView();
		}

		// Enable the Edit menu items for CViewText windows, disable them otherwise
		UINT Flags = (dynamic_cast<CViewText*>(pWnd))? MF_ENABLED : MF_GRAYED;
		EditMenu.EnableMenuItem(nID, MF_BYCOMMAND | Flags);
	}

	CFrame::OnMenuUpdate(nID);
}

void CMainDockFrame::PreCreate(CREATESTRUCT &cs)
{
	// Call the base class function first
	CFrame::PreCreate(cs);

	// Hide the window initially by removing the WS_VISIBLE style
	cs.style &= ~WS_VISIBLE;
}

BOOL CMainDockFrame::SaveRegistrySettings()
{
	CFrame::SaveRegistrySettings();

	// Save the docker settings
	SaveDockRegistrySettings(GetRegistryKeyName());

	// Save the tabbedMDI settings
	m_MyTabbedMDI.SaveRegistrySettings(GetRegistryKeyName());

	return TRUE;
}

void CMainDockFrame::SetupToolBar()
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
	GetFrameMenu().CheckMenuItem(IDM_CONTAINER_TOP, MF_UNCHECKED);

	// Add some extra icons for menu items
	AddMenuIcon(IDM_FILE_NEWSIMPLE, GetApp().LoadIcon(IDI_SIMPLE));
	AddMenuIcon(IDM_FILE_NEWRECT, GetApp().LoadIcon(IDI_RECT));
	AddMenuIcon(IDM_FILE_NEWTEXT, GetApp().LoadIcon(IDI_TEXT));
	AddMenuIcon(IDM_FILE_NEWLIST, GetApp().LoadIcon(IDI_FILEVIEW));
	AddMenuIcon(IDM_FILE_NEWTREE, GetApp().LoadIcon(IDI_CLASSVIEW));
}

LRESULT CMainDockFrame::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(uMsg);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	// Store the active docker before processing the menu events
	m_pLastActiveDocker = GetActiveDocker();

	return MA_ACTIVATE;
}

LRESULT CMainDockFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MOUSEACTIVATE:	return OnMouseActivate(uMsg, wParam, lParam);
	}

	// Always pass unhandled messages on to WndProcDefault
	return WndProcDefault(uMsg, wParam, lParam);
}
