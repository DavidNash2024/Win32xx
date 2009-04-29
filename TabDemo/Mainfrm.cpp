////////////////////////////////////////////////////
// Mainfrm.cpp


#include "resource.h"
#include "TabDemoApp.h"
#include "mainfrm.h"


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
		MessageBox("New", "", MB_OK);
		return TRUE;
	case IDM_FILE_CLOSE:
		MessageBox("Close", "", MB_OK);
		return TRUE;
	case IDM_FILE_EXIT:
		// End the application
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	case IDM_TAB_TOP:
		OnTopTabs();
		return TRUE;
	case IDM_TAB_BUTTONS:
		OnShowButtons();
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

	m_View.SetShowButtons(TRUE);
}

void CMainFrame::OnInitialUpdate()
{
	// Stop window painting
	SetRedraw(FALSE);
	
	// Add some tabs to the tab control
	m_View.AddTabPage(new CViewClasses, _T("Classes"), IDI_CLASSVIEW);
	m_View.AddTabPage(new CViewFiles, _T("Files"), IDI_FILEVIEW);
	m_View.AddTabPage(new CViewClasses, _T("Classes"), IDI_CLASSVIEW);
	m_View.AddTabPage(new CViewFiles, _T("Files"), IDI_FILEVIEW);
	m_View.SelectPage(0);
	
	// Repaint the window
	SetRedraw(TRUE);
	RedrawWindow(0, 0, RDW_INVALIDATE|RDW_ALLCHILDREN|RDW_UPDATENOW);
}

void CMainFrame::OnShowButtons()
{
	BOOL bShow = m_View.GetShowButtons();
	m_View.SetShowButtons(!bShow);
	UINT uCheck = bShow? MF_UNCHECKED : MF_CHECKED;
	::CheckMenuItem(GetFrameMenu(), IDM_TAB_BUTTONS, uCheck);
	m_View.RedrawWindow();
}

void CMainFrame::OnTopTabs()
{
	DWORD dwStyle = m_View.GetWindowLongPtr(GWL_STYLE);
	if (dwStyle & TCS_BOTTOM)
		dwStyle &= ~TCS_BOTTOM;
	else 
		dwStyle |= TCS_BOTTOM;

	m_View.SetWindowLongPtr(GWL_STYLE, dwStyle);
	
	UINT uCheck = (dwStyle & TCS_BOTTOM)? MF_UNCHECKED : MF_CHECKED;
	::CheckMenuItem(GetFrameMenu(), IDM_TAB_TOP, uCheck);
	m_View.RecalcLayout();
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
}

LRESULT CMainFrame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//
//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

