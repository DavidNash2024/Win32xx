////////////////////////////////////////////////////
// Mainfrm.cpp


#include "stdafx.h"
#include "TabDemoApp.h"
#include "mainfrm.h"
#include "resource.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	//Set the tab control as the fram's view window
	SetView(m_View);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\Container Demo"));
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
	case IDM_FILE_NEW:
		OnFileNew();
		return TRUE;
	case IDM_FILE_CLOSE:
		OnFileClose();
		return TRUE;
	case IDM_FILE_EXIT:
		// End the application
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	case IDM_NEW_FILES:
		m_View.AddTabPage(new CViewFiles, _T("Files"), IDI_FILEVIEW);
		return TRUE;
	case IDM_NEW_CLASSES:
		m_View.AddTabPage(new CViewClasses, _T("Classes"), IDI_CLASSVIEW);
		return TRUE;
	case IDM_TAB_TOP:
		OnTabsAtTop();
		return TRUE;
	case IDM_TAB_BUTTONS:
		OnShowButtons();
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

void CMainFrame::OnFileClose()
{
	int iTab = m_View.GetCurSel();

	if (iTab >= 0)
		m_View.RemoveTabPage(iTab);
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
	// Add some tabs to the tab control
	m_View.AddTabPage(new CViewClasses, _T("Classes"), IDI_CLASSVIEW);
	m_View.AddTabPage(new CViewFiles, _T("Files"), IDI_FILEVIEW);
	m_View.AddTabPage(new CViewClasses, _T("Classes"), IDI_CLASSVIEW);
	m_View.AddTabPage(new CViewFiles, _T("Files"), IDI_FILEVIEW);
	m_View.SelectPage(0);

	// PreCreate initially set the window as invisible, so show it now.
	ShowWindow();
}

void CMainFrame::OnShowButtons()
{
	BOOL bShow = m_View.GetShowButtons();
	m_View.SetShowButtons(!bShow);
	UINT uCheck = bShow? MF_UNCHECKED : MF_CHECKED;
	::CheckMenuItem(GetFrameMenu(), IDM_TAB_BUTTONS, uCheck);
	m_View.RedrawWindow();
}

void CMainFrame::OnTabsAtTop()
{
	BOOL bTop = m_View.GetTabsAtTop();
	m_View.SetTabsAtTop(!bTop);
	
	UINT uCheck = (bTop)? MF_UNCHECKED : MF_CHECKED;
	::CheckMenuItem(GetFrameMenu(), IDM_TAB_TOP, uCheck);
}

void CMainFrame::PreCreate(CREATESTRUCT &cs)
{
	// Call the base class function first
	CFrame::PreCreate(cs);
	
	// Hide the window initially by removing the WS_VISIBLE style
	cs.style &= ~WS_VISIBLE;
}

void CMainFrame::SetupToolbar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolbarButton( IDM_FILE_NEW          );
	AddToolbarButton( IDM_FILE_OPEN,  FALSE );
	AddToolbarButton( IDM_FILE_SAVE,  FALSE );
	
	AddToolbarButton( 0 );	// Separator
	AddToolbarButton( IDM_EDIT_CUT,   FALSE );
	AddToolbarButton( IDM_EDIT_COPY,  FALSE );
	AddToolbarButton( IDM_EDIT_PASTE, FALSE );
	
	AddToolbarButton( 0 );	// Separator
	AddToolbarButton( IDM_FILE_PRINT, FALSE );
	
	AddToolbarButton( 0 );	// Separator
	AddToolbarButton( IDM_HELP_ABOUT        );

	// Set the Checkmarks in the menu
	::CheckMenuItem(GetFrameMenu(), IDM_TAB_BUTTONS, MF_UNCHECKED);

	// Add some extra icons for menu items
	AddMenuIcon(IDM_NEW_FILES, ::LoadIcon(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDI_FILEVIEW)));
	AddMenuIcon(IDM_NEW_CLASSES, ::LoadIcon(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDI_CLASSVIEW)));
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

