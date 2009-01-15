////////////////////////////////////////////////////
// Mainfrm.cpp


#include "resource.h"
#include "ContainerApp.h"
#include "mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_DockView as the view window of the frame
	SetView(m_DockView);

	// Set the Resource IDs for the toolbar buttons
	AddToolbarButton( IDM_FILE_NEW   );
	AddToolbarButton( IDM_FILE_OPEN  );
	AddToolbarButton( IDM_FILE_SAVE  );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_EDIT_CUT   );
	AddToolbarButton( IDM_EDIT_COPY  );
	AddToolbarButton( IDM_EDIT_PASTE );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_FILE_PRINT );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_HELP_ABOUT );

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\Container Docking"));
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
	// Note: The  DockIDs are used for saving/restoring the dockables state in the registry

	DWORD dwStyle = DS_CLIENTEDGE; // The style added to each dockable
	m_DockView.SetDockStyle(dwStyle);
	
	// Add the remaining dockables
//	CDockable* pDockLeft   = m_DockView.AddDockedChild(new CDockClass, DS_DOCKED_LEFT | dwStyle, 200, ID_CLASS1);
//	CDockable* pDockRight  = m_DockView.AddDockedChild(new CDockClass, DS_DOCKED_RIGHT | dwStyle, 200, ID_CLASS2);
	CDockable* pDockRight  = m_DockView.AddDockedChild(new CDockClasses, DS_DOCKED_RIGHT | dwStyle, 200, ID_CONTAINCLASSES);	
//	CDockable* pDockTop    = m_DockView.AddDockedChild(new CDockText, DS_DOCKED_TOP | dwStyle, 100, ID_TEXT1);
	CDockable* pDockBottom = m_DockView.AddDockedChild(new CDockText, DS_DOCKED_BOTTOM | dwStyle, 100, ID_TEXT2);

//	pDockLeft->AddDockedChild(new CDockFiles, DS_DOCKED_BOTTOM | dwStyle, 150, ID_FILES1);
//	pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_BOTTOM | dwStyle, 150, ID_FILES2);
	pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | dwStyle, 200, ID_CONTAINFILES);
	pDockRight->AddDockedChild(new CDockClasses, DS_DOCKED_CONTAINER | dwStyle, 200, ID_CONTAINCLASSES);
	pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | dwStyle, 200, ID_CONTAINFILES);

//	pDockTop->AddDockedChild(new CDockSimple, DS_DOCKED_RIGHT | dwStyle, 100, ID_SIMPLE1);
//	pDockBottom->AddDockedChild(new CDockSimple, DS_DOCKED_RIGHT | dwStyle, 100, ID_SIMPLE2);
}

LRESULT CMainFrame::OnNotify(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// Process notification messages sent by child windows
//	switch(((LPNMHDR)lParam)->code)
//	{
 		//Add case statments for each notification message here
//	}

	// Some notifications should return a value when handled
	return 0L;
}

void CMainFrame::PreCreate(CREATESTRUCT &cs)
{
	cs.style = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	CFrame::PreCreate(cs);
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

