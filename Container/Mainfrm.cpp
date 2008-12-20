////////////////////////////////////////////////////
// Mainfrm.cpp


#include "resource.h"
#include "ContainerApp.h"
#include "mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_View as the view window of the frame
	SetView(m_Classes);
//	SetView(m_Files);

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
	LoadRegistrySettings(_T("Win32++\\Tab Demo"));

//	HICON hIcon = LoadIcon(GetTabDemoApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_FILEVIEW));
//	m_View.InsertPage(m_ViewList, _T("FileView"), hIcon);
//	HICON hIcon = LoadIcon(GetTabDemoApp().GetResourceHandle(), MAKEINTRESOURCE(IDI_CLASSVIEW));
//	m_View.InsertPage(m_ViewTree, _T("ClassView"), hIcon);
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

	m_Classes.SetTabText(_T("ClassView"));
	m_Classes.SetTabIcon(IDI_CLASSVIEW);
	m_Files.SetTabText(_T("FileView"));
	m_Files.SetTabIcon(IDI_FILEVIEW);
	m_Classes.AddContainer(&m_Files);	
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.

	TRACE(_T("Frame created\n"));
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

