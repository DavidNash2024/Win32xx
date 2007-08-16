////////////////////////////////////////////////////
// Mainfrm.cpp


#include "resource.h"
#include "mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_View as the view window of the frame
	SetView(m_View);

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
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// The default for the following variables is TRUE

	// m_bShowIndicatorStatus = FALSE;	// Don't show statusbar indicators
	// m_bShowMenuStatus = FALSE;		// Don't show toolbar or menu status
	// m_bUseRebar = FALSE;				// Don't use rebars

	// call the base class function
	CFrame::OnCreate();
	
	SetTheme();
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.

	TRACE("Frame created");
}

LRESULT CMainFrame::OnNotify(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// Process notification messages sent by child windows
//	switch(((LPNMHDR)lParam)->code)
//	{
 		//Add case statments for each notification message here
//	}

	// Pass any unhandled messages on for default processing
	return 0L;
}

void CMainFrame::SetTheme()
{
	// Set the rebar theme
	CRebar& RB = GetRebar();

	REBARTHEME rt = {0};
	rt.UseThemes= TRUE;
	rt.clrBkGnd1 = RGB(150,190,245);
	rt.clrBkGnd2 = RGB(196,215,250);
	rt.clrBand1  = RGB(220,230,250);
	rt.clrBand2  = RGB( 70,130,220);
	rt.KeepBandsLeft = TRUE;
	rt.LockMenuBand  = TRUE;
	rt.ShortBands    = TRUE;
	rt.RoundBorders  = TRUE;

//	or you could use the following
//	BOOL T = TRUE;
//	BOOL F = FALSE;
//	REBARTHEME rt = {T, RGB(150,190,245), RGB(196,215,250), RGB(220,230,250), RGB( 70,130,220), F, T, T, T, T, F};
	RB.SetTheme(rt);

	// Set the toolbar theme
	CToolbar& TB = GetToolbar();

	TOOLBARTHEME tt = {0};
	tt.UseThemes   = TRUE;
	tt.clrHot1     = RGB(255, 230, 190);
	tt.clrHot2     = RGB(255, 190, 100);
	tt.clrPressed1 = RGB(255, 140, 40);
	tt.clrPressed2 = RGB(255, 180, 80);
	tt.clrOutline  = RGB(192, 128, 255);

//	or you could use the following
//	TOOLBARTHEME tt = {T, RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(192, 128, 255)};
	TB.SetTheme(tt);

	RecalcLayout();
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

