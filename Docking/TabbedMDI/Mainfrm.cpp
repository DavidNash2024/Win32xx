////////////////////////////////////////////////////
// Mainfrm.cpp


#include "resource.h"
#include "TabbedMDIApp.h"
#include "mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
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

void CMainFrame::OnFileNew()
{
	// Creates the popup menu when the "New" toolbar button is pressed

	// Position the popup menu
	CToolbar& TB = GetToolbar();
	RECT rc = TB.GetItemRect(TB.CommandToIndex(IDM_FILE_NEW));
	::MapWindowPoints(GetToolbar(), NULL, (LPPOINT)&rc, 2);

	TPMPARAMS tpm;
	tpm.cbSize = sizeof(TPMPARAMS);
	tpm.rcExclude = rc;

	// Load the popup menu
	HMENU hTopMenu = ::LoadMenu(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDM_NEWMENU));
	HMENU hPopupMenu = ::GetSubMenu(hTopMenu, 0);

	// Start the popup menu
	::TrackPopupMenuEx(hPopupMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, rc.left, rc.bottom, m_hWnd, &tpm);

	// Release the menu resource
	::DestroyMenu(hTopMenu);
}

void CMainFrame::OnContainerTabsAtTop()
// Reposition the tabs in the containers
{
	BOOL bTop = FALSE;
	std::vector<CDocker*>::iterator iter;
	
	// Set the Tab position for each container
	for (iter = m_DockTabbedMDI.GetAllDockers().begin(); iter < m_DockTabbedMDI.GetAllDockers().end(); ++iter)
	{
		CContainer* pContainer = (*iter)->GetContainer();
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
	pTabbedMDI->SetActiveTab(0);
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	CTabbedMDI* pTabbedMDI = (CTabbedMDI*)m_DockTabbedMDI.GetView();

	// OnCommand responds to menu and and toolbar input
	switch(LOWORD(wParam))
	{
	case IDM_FILE_NEW:
		OnFileNew();
		return TRUE;
	case IDM_FILE_NEWSIMPLE:
		pTabbedMDI->AddMDIChild(new CViewSimple, _T("Simple View"), ID_MDI_SIMPLE);	
		return TRUE;
	case IDM_FILE_NEWRECT:
		pTabbedMDI->AddMDIChild(new CViewRect, _T("Rectangles"), ID_MDI_RECT);	
		return TRUE;
	case IDM_FILE_NEWTEXT:
		pTabbedMDI->AddMDIChild(new CViewText, _T("TextView"), ID_MDI_TEXT);	
		return TRUE;
	case IDM_FILE_NEWTREE:
		pTabbedMDI->AddMDIChild(new CViewClasses, _T("TreeView"), ID_MDI_CLASSES);	
		return TRUE;
	case IDM_FILE_NEWLIST:
		pTabbedMDI->AddMDIChild(new CViewFiles, _T("ListView"), ID_MDI_FILES);
		return TRUE;
	case IDM_FILE_EXIT:
		// End the application
		::PostMessage(m_hWnd, WM_CLOSE, 0L, 0L);
		return TRUE;
	case IDM_CONTAINER_TOP:
		OnContainerTabsAtTop();
		return TRUE;
	case IDM_TABBEDMDI_TOP:
		OnMDITabsAtTop();
		return TRUE;
	case IDM_LAYOUT_DEFAULT:
		m_DockTabbedMDI.CloseAllDockers();
		::CheckMenuItem(GetFrameMenu(), IDM_CONTAINER_TOP, MF_UNCHECKED);
		LoadDefaultDockers();
		LoadDefaultMDIs();
		return TRUE;
	case IDM_LAYOUT_CLOSE_DOCKERS:
		m_DockTabbedMDI.CloseAllDockers();
		return TRUE;
	case IDM_LAYOUT_CLOSE_MDIS:
		TRACE("IDM_LAYOUT_CLOSEALL_MDIS\n");
		return TRUE;
	case IDW_VIEW_STATUSBAR:
		OnViewStatusbar();
		return TRUE;
	case IDW_VIEW_TOOLBAR:
		OnViewToolbar();
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
	// m_bUseToolbar = FALSE;			// Don't use a toolbar

	// call the base class function
	CFrame::OnCreate();
}

void CMainFrame::OnInitialUpdate()
{
	m_DockTabbedMDI.SetDockStyle(DS_CLIENTEDGE);

	if (0 != GetRegistryKeyName().size())
	{
		try
		{
			// Load dock settings from registry
			m_DockTabbedMDI.LoadRegistrySettings(GetRegistryKeyName());

			// Load MDI child settings from registry
			m_DockTabbedMDI.GetTabbedMDI()->LoadRegistrySettings(GetRegistryKeyName());
		}

		catch (const CWinException &e)
		{
			// Handle possible exceptions from LoadRegistrySettings
			e.MessageBox();
		}
	}

	// Ensure we have some docked/undocked windows
	if (0 == m_DockTabbedMDI.GetAllDockers().size())
		LoadDefaultDockers();

	// Ensure we have some tabbed MDI children
	if (0 == m_DockTabbedMDI.GetTabbedMDI()->GetMDIChildCount())
		LoadDefaultMDIs();

	// PreCreate initially set the window as invisible, so show it now.
	ShowWindow();
}

void CMainFrame::PreCreate(CREATESTRUCT &cs)
{
	// Call the base class function first
	CFrame::PreCreate(cs);
	
	// Hide the window initially by removing the WS_VISIBLE style
	cs.style &= ~WS_VISIBLE;
}

/*
void CMainFrame::SaveTabbedMDIs(tString tsRegistryKeyName)
{
	if (0 != tsRegistryKeyName.size())
	{
		CTabbedMDI* pTabbedMDI = m_DockTabbedMDI.GetTabbedMDI();

		tString tsKeyName = _T("Software\\") + tsRegistryKeyName;
		HKEY hKey = NULL;
		HKEY hKeyMDIChild = NULL;
		if (RegCreateKeyEx(HKEY_CURRENT_USER, tsKeyName.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL))
			throw (CWinException(_T("RegCreateKeyEx Failed")));

		RegDeleteKey(hKey, _T("MDI Children"));
		if (RegCreateKeyEx(hKey, _T("MDI Children"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKeyMDIChild, NULL))
			throw (CWinException(_T("RegCreateKeyEx Failed")));

		for (int i = 0; i < pTabbedMDI->GetMDIChildCount(); ++i)
		{
			TCHAR szNumber[16];
			tString tsSubKey = _T("MDI Child ");
			tsSubKey += _itot(i, szNumber, 10);
			TabPageInfo pdi = m_DockTabbedMDI.GetTabbedMDI()->GetTab().GetTabPageInfo(i);
			RegSetValueEx(hKeyMDIChild, tsSubKey.c_str(), 0, REG_BINARY, (LPBYTE)&pdi, sizeof(TabPageInfo));
		}	

		RegCloseKey(hKeyMDIChild);
		RegCloseKey(hKey);
	}
}

*/

void CMainFrame::SaveRegistrySettings()
{
	CFrame::SaveRegistrySettings();

	// Save the docker settings
	m_DockTabbedMDI.SaveRegistrySettings(GetRegistryKeyName());
	
	// Save the tabbedMDI settings
	m_DockTabbedMDI.GetTabbedMDI()->SaveRegistrySettings(GetRegistryKeyName());
}

void CMainFrame::SetupToolbar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolbarButton( IDM_FILE_NEW   );
	AddToolbarButton( IDM_FILE_OPEN,  FALSE );
	AddToolbarButton( IDM_FILE_SAVE,  FALSE );
	
	AddToolbarButton( 0 );	// Separator
	AddToolbarButton( IDM_EDIT_CUT,   FALSE );
	AddToolbarButton( IDM_EDIT_COPY,  FALSE );
	AddToolbarButton( IDM_EDIT_PASTE, FALSE );
	
	AddToolbarButton( 0 );	// Separator
	AddToolbarButton( IDM_FILE_PRINT, FALSE );
	
	AddToolbarButton( 0 );	// Separator
	AddToolbarButton( IDM_HELP_ABOUT );

	// Remove the checkmark for container tabs at top
	::CheckMenuItem(GetFrameMenu(), IDM_CONTAINER_TOP, MF_UNCHECKED);

	// Add some extra icons for menu items
	AddMenuIcon(IDM_FILE_NEWSIMPLE, ::LoadIcon(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDI_SIMPLE)));
	AddMenuIcon(IDM_FILE_NEWRECT, ::LoadIcon(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDI_RECT)));
	AddMenuIcon(IDM_FILE_NEWTEXT, ::LoadIcon(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDI_TEXT)));
	AddMenuIcon(IDM_FILE_NEWLIST, ::LoadIcon(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDI_FILEVIEW)));
	AddMenuIcon(IDM_FILE_NEWTREE, ::LoadIcon(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDI_CLASSVIEW))); 
}

