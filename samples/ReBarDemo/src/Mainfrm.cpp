////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "resource.h"
#include "mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_View as the view window of the frame
	SetView(m_View);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\ReBarDemo"));
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

CRect CMainFrame::GetViewRect() const
{
	CRect rcClient = GetClientRect();

	if (GetStatusBar().IsWindow() && (GetStatusBar().IsWindowVisible()))
		rcClient = ExcludeChildRect(rcClient, GetStatusBar());

	if (IsReBarSupported() && GetUseReBar() && GetReBar().IsWindow())
		rcClient = ExcludeChildRect(rcClient, GetReBar());
	else
		if (GetToolBar().IsWindow() && GetToolBar().IsWindowVisible())
			rcClient = ExcludeChildRect(rcClient, GetToolBar());

	if (m_ReBar.IsWindow())
		rcClient = ExcludeChildRect(rcClient, m_ReBar);

	return rcClient;
}

void CMainFrame::OnBottom()
{
	DWORD dwStyle = m_ToolBar.GetWindowLongPtr(GWL_STYLE);
	dwStyle &= ~(CCS_VERT);
	dwStyle |= CCS_BOTTOM;
	SetWrapState(&m_ToolBar, FALSE);
	m_ToolBar.SetWindowLongPtr(GWL_STYLE, dwStyle);
	
	dwStyle = m_ReBar.GetWindowLongPtr(GWL_STYLE);
	dwStyle &= ~(CCS_VERT);
	dwStyle |= CCS_BOTTOM;
	m_ReBar.SetWindowLongPtr(GWL_STYLE, dwStyle);
	RecalcLayout();
}

void CMainFrame::OnLeft()
{
	DWORD dwStyle = m_ToolBar.GetWindowLongPtr(GWL_STYLE);
	dwStyle &= ~(CCS_BOTTOM);
	dwStyle |= CCS_LEFT;
	SetWrapState(&m_ToolBar, TRUE);
	m_ToolBar.SetWindowLongPtr(GWL_STYLE, dwStyle);
	
	dwStyle = m_ReBar.GetWindowLongPtr(GWL_STYLE);
	dwStyle &= ~(CCS_BOTTOM);
	dwStyle |= CCS_LEFT;
	m_ReBar.SetWindowLongPtr(GWL_STYLE, dwStyle);	
	RecalcLayout();
}

void CMainFrame::OnRight()
{
	DWORD dwStyle = m_ToolBar.GetWindowLongPtr(GWL_STYLE);
	dwStyle |= CCS_RIGHT;
	SetWrapState(&m_ToolBar, TRUE);
	m_ToolBar.SetWindowLongPtr(GWL_STYLE, dwStyle);
	
	dwStyle = m_ReBar.GetWindowLongPtr(GWL_STYLE);
	dwStyle |= CCS_RIGHT;
	m_ReBar.SetWindowLongPtr(GWL_STYLE, dwStyle);	
	RecalcLayout();
}

void CMainFrame::OnTop()
{
	DWORD dwStyle = m_ToolBar.GetWindowLongPtr(GWL_STYLE);
	dwStyle &= ~(CCS_VERT | CCS_BOTTOM);
	SetWrapState(&m_ToolBar, FALSE);
	m_ToolBar.SetWindowLongPtr(GWL_STYLE, dwStyle);

	dwStyle = m_ReBar.GetWindowLongPtr(GWL_STYLE);
	dwStyle &= ~(CCS_VERT | CCS_BOTTOM);
	m_ReBar.SetWindowLongPtr(GWL_STYLE, dwStyle);
	RecalcLayout();
}


BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// OnCommand responds to menu and and toolbar input

	UNREFERENCED_PARAMETER(lParam);

	switch(LOWORD(wParam))
	{
	case IDM_FILE_OPEN:			return OnFileOpen();
	case IDM_FILE_SAVE:			return OnFileSave();
	case IDM_FILE_SAVEAS:		return OnFileSave();
	case IDM_FILE_PRINT:		return OnFilePrint();
	case IDM_FILE_EXIT:			return OnFileExit();
	case IDW_VIEW_STATUSBAR:	return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:		return OnViewToolBar();
	case IDM_HELP_ABOUT:		return OnHelp();
	
	// Commands from the ToolBar in the additional rebar
	case IDM_TOP:		OnTop();		return TRUE;
	case IDM_LEFT:		OnLeft();		return TRUE;
	case IDM_RIGHT:		OnRight();		return TRUE;
	case IDM_BOTTOM:	OnBottom();		return TRUE;
	}

	return FALSE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT pcs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// Uncomment the lines below to change frame options.

	// SetUseIndicatorStatus(FALSE);	// Don't show keyboard indicators in the StatusBar
	// SetUseMenuStatus(FALSE);			// Don't show menu descriptions in the StatusBar
	// SetUseThemes(FALSE);				// Don't use themes
	// SetUseToolBar(FALSE);			// Don't use a ToolBar

	// call the base class function
	CFrame::OnCreate(pcs);

	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS 
					 | CCS_NORESIZE | CCS_NOPARENTALIGN | CCS_NODIVIDER | RBS_VARHEIGHT ;
	m_ReBar.CreateEx(0, REBARCLASSNAME, 0, dwStyle, CRect(0,0,0,0), *this, 0);

	dwStyle |= TBSTYLE_FLAT;

	// Create the ToolBar's image list from 4 icons
	m_ToolBarImages.Create(48, 48, ILC_COLOR32 | ILC_MASK, 0, 0);
	m_ToolBarImages.Add(GetApp().LoadIcon(IDI_TOP));
	m_ToolBarImages.Add(GetApp().LoadIcon(IDI_LEFT));
	m_ToolBarImages.Add(GetApp().LoadIcon(IDI_RIGHT));
	m_ToolBarImages.Add(GetApp().LoadIcon(IDI_BOTTOM));

	// Create the ToolBar
	dwStyle |= TBSTYLE_TOOLTIPS | TBSTYLE_FLAT;
	m_ToolBar.CreateEx(0, TOOLBARCLASSNAME, 0, dwStyle, CRect(0,0,0,0), m_ReBar, 0);
	m_ToolBar.SetImageList(m_ToolBarImages);

	// Add the ToolBar buttons
	TBBUTTON ButtonInfo[] =
	{
		{ 0, IDM_TOP,		TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK|TBSTYLE_GROUP, {0}, 0, 0 },
		{ 1, IDM_LEFT,		TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK|TBSTYLE_GROUP, {0}, 0, 0 },
		{ 2, IDM_RIGHT,		TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK|TBSTYLE_GROUP, {0}, 0, 0 },
		{ 3, IDM_BOTTOM,	TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK|TBSTYLE_GROUP, {0}, 0, 0 },
		{ 0, 0,				TBSTATE_ENABLED, TBSTYLE_SEP,                                {0}, 0, 0 }
	};	 // Last button is a separator 
	m_ToolBar.AddButtons(5, ButtonInfo);

	// Fill the REBARBAND structure
	REBARBANDINFO rbbi;
	ZeroMemory(&rbbi, sizeof(REBARBANDINFO));
	rbbi.fMask      = RBBIM_CHILD |RBBIM_CHILDSIZE| RBBIM_ID | RBBIM_SIZE | RBBIM_STYLE;
	rbbi.fStyle     = RBBS_BREAK;
	rbbi.hwndChild  = m_ToolBar.GetHwnd();
	rbbi.wID        = IDC_BANDARROWS;
	rbbi.cyMinChild = m_ToolBar.GetMaxSize().cy+1;
	m_ReBar.InsertBand(-1, rbbi);

	return 0;
}

BOOL CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
	return TRUE;
}

BOOL CMainFrame::OnFileOpen()
{
	// Bring up the dialog, and open the file
	CFile File;
	CString str = File.OpenFileDialog(0, 0, 0, 0);

	// TODO:
	// Add your own code here. Refer to the tutorial for additional information

	return TRUE;
}

BOOL CMainFrame::OnFileSave()
{
	CFile File;
	CString str = File.SaveFileDialog(0, 0, 0, 0, 0);

	// TODO:
	// Add your own code here. Refer to the tutorial for additional information

	return TRUE;
}

BOOL CMainFrame::OnFilePrint()
{
	// Bring up a dialog to choose the printer
	PRINTDLG pd;
	ZeroMemory(&pd, sizeof(PRINTDLG));
	pd.lStructSize = sizeof( pd );
	pd.Flags = PD_RETURNDC;
	pd.hwndOwner = *this;

	// Retrieve the printer DC
	PrintDlg( &pd );
	
	// TODO:
	// Add your own code here. Refer to the tutorial for additional information

	return TRUE;
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.

	TRACE("Frame created\n");
}

LRESULT CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam)
// Process notification messages sent by child windows
{
//	switch (((LPNMHDR)lParam)->code)
//	{
//	}

	// Some notifications should return a value when handled
	return CFrame::OnNotify(wParam, lParam);
}

void CMainFrame::RecalcLayout()
// Override CFrame::RecalcLayout to add the positioning of our rebar
{
	// Resize the status bar
	if (GetStatusBar().IsWindow() && GetShowStatusBar())
	{
		GetStatusBar().SetWindowPos(NULL, 0, 0, 0, 0, SWP_SHOWWINDOW);
		GetStatusBar().Invalidate();
		if (GetUseMenuStatus())
			GetStatusBar().SetWindowText(GetStatusText());

		SetStatusIndicators();
	}

	// Resize the rebar or toolbar
	if (IsReBarUsed())
	{
		GetReBar().SendMessage(WM_SIZE, 0L, 0L);
		GetReBar().Invalidate();
	}
	else if (GetUseToolBar() && GetShowToolBar() && GetToolBar().IsWindow())
		GetToolBar().SendMessage(TB_AUTOSIZE, 0L, 0L);

	// Position the additional rebar at the top, left, right or bottom of the view.
	if (m_ReBar.IsWindow())
		SetReBarPos();

	// Position the view window
	CFrame::RecalcLayout();

	// Adjust rebar bands
	if (IsReBarUsed())
	{
		if (GetReBarTheme().UseThemes && GetReBarTheme().BandsLeft)
			GetReBar().MoveBandsLeft();

		if (IsMenuBarUsed())
			SetMenuBarBandSize();
	}
}

void CMainFrame::SetReBarPos()
{
	int cxRB = 0;
	for (UINT u = 0 ; u < m_ReBar.GetRowCount(); ++u)
		cxRB += m_ReBar.GetRowHeight(u);

	CRect rc = CFrame::GetViewRect();
	int cyRB = MIN(cxRB, rc.Height());

	DWORD dwStyle = m_ReBar.GetWindowLongPtr(GWL_STYLE);
	dwStyle &= CCS_VERT | CCS_BOTTOM; // Filter unwanted styles

	switch(dwStyle)
	{
	case CCS_LEFT:
		m_ReBar.SetWindowPos(NULL, 0, rc.top, cxRB, rc.Height(), SWP_SHOWWINDOW);
		m_ToolBar.PressButton(IDM_LEFT, TRUE);
		break;
	case CCS_RIGHT:
		m_ReBar.SetWindowPos(NULL, rc.Width() - cxRB, rc.top, cxRB, rc.Height(), SWP_SHOWWINDOW);
		m_ToolBar.PressButton(IDM_RIGHT, TRUE);
		break;
	case CCS_BOTTOM:
		m_ReBar.SetWindowPos(NULL, 0, rc.bottom - cyRB, rc.Width(), cyRB, SWP_SHOWWINDOW);
		m_ToolBar.PressButton(IDM_BOTTOM, TRUE);
		break;
	default:
		m_ReBar.SetWindowPos(NULL, 0, rc.top, rc.Width(), cyRB, SWP_SHOWWINDOW);	
		m_ToolBar.PressButton(IDM_TOP, TRUE);
		break;
	}

	Invalidate();
	m_ReBar.RedrawWindow(); 	
}

void CMainFrame::SetupToolBar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolBarButton( IDM_FILE_NEW   );
	AddToolBarButton( IDM_FILE_OPEN  );
	AddToolBarButton( IDM_FILE_SAVE  );
	
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_EDIT_CUT,   FALSE );	// disabled button
	AddToolBarButton( IDM_EDIT_COPY,  FALSE );	// disabled button
	AddToolBarButton( IDM_EDIT_PASTE, FALSE );	// disabled button
	
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_FILE_PRINT );
	
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_HELP_ABOUT );
}

// Note: Vertical toolbars require each button to have TBSTATE_WRAP, set for group buttons to work.
//       Horizontal toolbars require the TBSTATE_WRAP removed.
void CMainFrame::SetWrapState(CToolBar* pToolBar, BOOL bWrap)
{
	for (int i = 0; i < m_ToolBar.GetButtonCount(); ++i)
	{
		int nID = pToolBar->GetCommandID(i);
		pToolBar->SetButtonState(nID, TBSTATE_ENABLED | (bWrap? TBSTATE_WRAP : 0));
	}
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

