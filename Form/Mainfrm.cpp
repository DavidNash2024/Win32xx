////////////////////////////////////////////////////
// Mainfrm.cpp


#include "resource.h"
#include "mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame() : m_MyDialog(IDD_DIALOG1)
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_MyDialog as the view window of the frame
	SetView(m_MyDialog);

	// Set the Resource IDs for the toolbar buttons
	m_ToolbarData.clear();
	m_ToolbarData.push_back ( IDM_FILE_NEW   );
	m_ToolbarData.push_back ( IDM_FILE_OPEN  );
	m_ToolbarData.push_back ( IDM_FILE_SAVE  );
	m_ToolbarData.push_back ( 0 );				// Separator
	m_ToolbarData.push_back ( IDM_EDIT_CUT   );
	m_ToolbarData.push_back ( IDM_EDIT_COPY  );
	m_ToolbarData.push_back ( IDM_EDIT_PASTE );
	m_ToolbarData.push_back ( 0 );				// Separator
	m_ToolbarData.push_back ( IDM_FILE_PRINT );
	m_ToolbarData.push_back ( 0 );				// Separator
	m_ToolbarData.push_back ( IDM_HELP_ABOUT );
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
	// Set the icon
	SetIconLarge(IDW_MAIN);
	SetIconSmall(IDW_MAIN);

	// Set the accelerator table and HWND for translated messages
	GetApp()->SetAccelerators(IDW_MAIN, GetHwnd());

	if (IsRebarSupported() && m_bUseRebar)
	{
		// Create the rebar
		GetRebar().Create(m_hWnd);

		// Create the menu inside rebar
		GetMenubar().Create(GetRebar().GetHwnd());
		GetMenubar().SetMenu(GetFrameMenu());
		AddMenubarBand();

		// Create the toolbar inside rebar
		GetToolbar().Create(GetRebar().GetHwnd());

		// Load the toolbar buttons
		SetButtons(m_ToolbarData);
		
		AddToolbarBand();
	}
	else
	{
		// Create the toolbar without a rebar
		GetToolbar().Create(m_hWnd);

		// Load the toolbar buttons
		SetButtons(m_ToolbarData);
	}

	if (!IsMenubarUsed())
		::SetMenu(m_hWnd, GetFrameMenu());

	// Create the status bar
	GetStatusbar().Create(m_hWnd);

	// Create the view window
	m_MyDialog.SetParent(m_hWnd);
	m_MyDialog.DoModeless();

	// Reposition the child windows
	RecalcLayout();

	// Start timer for Status updates
	if (m_bShowIndicatorStatus || m_bShowMenuStatus)
		::SetTimer(m_hWnd, ID_STATUS_TIMER, 200, NULL);
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.

	TRACE("Frame created");
}

void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
	cs.cx = 500;
	cs.cy = 420;
	CFrame::PreCreate(cs);
}

LRESULT CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam)
{
	// Process notification messages sent by child windows
//	switch(((LPNMHDR)lParam)->code)
//	{
 		//Add case statments for each notification message here
//	}

	// Pass any unhandled messages on for default processing
	return 0L;
}

void CMainFrame::SetButtons(const std::vector<UINT> ToolbarData)
{
	// Overriding CFrame::Setbuttons is optional. We do it here to use larger buttons
	// with seperate imagelists for normal, hot and disabled buttons.

	// A reference to the CToolbar object
	CToolbar& TB = GetToolbar();

	// Set the button size to 24x24 before adding the bitmap
	TB.SetBitmapSize(24, 24);

	// Set the image lists for normal, hot and disabled buttons
	TB.SetImageList(8, RGB(192,192,192), IDB_TOOLBAR_NORM, IDB_TOOLBAR_HOT, IDB_TOOLBAR_DIS);

	// Set the resource IDs for the toolbar buttons
	TB.SetButtons(ToolbarData);

	// Add some text to the buttons
// 	TB.SetButtonText(IDM_FILE_NEW,   _T("New"));
//	TB.SetButtonText(IDM_FILE_OPEN,  _T("Open"));
//	TB.SetButtonText(IDM_FILE_SAVE,  _T("Save"));
//	TB.SetButtonText(IDM_EDIT_CUT,   _T("Cut"));
//	TB.SetButtonText(IDM_EDIT_COPY,  _T("Copy"));
//	TB.SetButtonText(IDM_EDIT_PASTE, _T("Paste"));
//	TB.SetButtonText(IDM_FILE_PRINT, _T("Print"));
//	TB.SetButtonText(IDM_HELP_ABOUT, _T("About"));

	// Adjust the toolbar and rebar size to take account of the larger buttons
	RECT r;
	TB.GetItemRect(TB.CommandToIndex(IDM_HELP_ABOUT), &r);
	TB.SetButtonSize(r.right - r.left, r.bottom - r.top);

	// Disable some of the toolbar buttons
	TB.DisableButton(IDM_EDIT_CUT);
	TB.DisableButton(IDM_EDIT_COPY);
	TB.DisableButton(IDM_EDIT_PASTE);

	// Set the icons for popup menu items
	GetMenubar().SetIcons(m_ToolbarData, IDW_MAIN, RGB(192, 192, 192));
}

LRESULT CMainFrame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
		//Additional messages to be handled go here
//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(hWnd, uMsg, wParam, lParam);	
}

