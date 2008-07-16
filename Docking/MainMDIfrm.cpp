////////////////////////////////////////////////////
// MainMDIfrm.cpp  - definitions for the CMainMDIFrame class


#include "resource.h"
#include "mainMDIfrm.h"
#include "MDIChildView.h"


CMainMDIFrame::CMainMDIFrame()
{
	// Define the resource IDs for the toolbar
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

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\MDI Frame"));
}

CMainMDIFrame::~CMainMDIFrame()
{
}

void CMainMDIFrame::OnInitialUpdate()
{
	TRACE(_T("MDI Frame started \n"));
	//The frame is now created.
	//Place any additional startup code here.
}

BOOL CMainMDIFrame::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	switch (LOWORD(wParam))
	{
	case IDM_FILE_NEW:
		AddMDIChild(new CMDIChildView); // CMDIFrame::RemoveMDIChild deletes this pointer
		return TRUE;
	case IDM_FILE_CLOSE:          // Close the active MDI window
		::SendMessage(GetActiveMDIChild(), WM_CLOSE, 0, 0);
		return TRUE;
	case IDM_FILE_EXIT:
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	case IDM_HELP_ABOUT:
		OnHelp();
		return TRUE;
	}
	return FALSE;
}

void CMainMDIFrame::OnCreate()
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// The default for the following variables is TRUE

	// m_bShowIndicatorStatus = FALSE;	// Don't show statusbar indicators
	// m_bShowMenuStatus = FALSE;		// Don't show toolbar or menu status
	// m_bUseRebar = FALSE;				// Don't use rebars
	// m_bUseThemes = FALSE;            // Don't use themes

	// call the base class function
	CMDIFrame::OnCreate();
	m_DockContainer.Create(m_hWnd);
	RecalcLayout();
}

void CMainMDIFrame::RecalcLayout()
{
	if ((!GetView()) || (!GetView()->GetHwnd()))
		return;

	// Resize the status bar
	HWND hStatusbar = GetStatusbar();
	if (hStatusbar)
	{
		::SendMessage(hStatusbar, WM_SIZE, 0, 0);
		::InvalidateRect(hStatusbar, NULL, TRUE);
	}

	// Reposition the text
	SetStatusText();

	// Resize the rebar or toolbar
	if (IsRebarUsed())
	{
		::SendMessage(GetRebar(), WM_SIZE, 0, 0);
		::InvalidateRect(GetRebar(), NULL, TRUE);
	}
	else
		::SendMessage(GetToolbar(), TB_AUTOSIZE, 0, 0);

	// Resize the View window
	CRect rClient = GetViewRect();
	int DockWidth = m_DockContainer.IsDocked()? 100 : 0;

	if ((rClient.bottom - rClient.top) >= 0)
	{
		int x  = rClient.left;
		int y  = rClient.top;
		int cx = rClient.right - rClient.left;
		int cy = rClient.bottom - rClient.top;

		::SetWindowPos(GetView()->GetHwnd(), NULL, x + DockWidth, y, cx - DockWidth, cy, SWP_SHOWWINDOW );
		
		if (m_DockContainer.IsDocked())
			::SetWindowPos(m_DockContainer.GetHwnd(), NULL, x, y, DockWidth, cy, SWP_SHOWWINDOW );
	}

	if (GetRebar().GetRebarTheme().UseThemes && GetRebar().GetRebarTheme().KeepBandsLeft)
		GetRebar().MoveBandsLeft();

	if (IsMenubarUsed())
		SetMenubarBandSize();

	::SendMessage(m_hWnd, USER_REARRANGED, 0, 0);

	// For MDI Frames
	::RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
}


LRESULT CMainMDIFrame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{

//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

