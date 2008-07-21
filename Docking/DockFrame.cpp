////////////////////////////////////////////////////
// DockFrame.cpp  - definitions for the CDockFrame class


#include "resource.h"
#include "DockFrame.h"
#include "MDIChildView.h"


CDockFrame::CDockFrame()
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
	LoadRegistrySettings(_T("Win32++\\Docking Frame"));
}

CDockFrame::~CDockFrame()
{
	std::vector <CDockable*>::iterator v;

	while(m_vDockables.size() > 0)
	{
		v = m_vDockables.begin();
		delete *v;
		m_vDockables.erase(v);
	}

	std::vector <CBar*>::iterator b;
	
	while(m_vBars.size() > 0)
	{
		b = m_vBars.begin();
		delete *b;
		m_vBars.erase(b);
	}
}

void CDockFrame::AddDockable(CDockable* pDockable, UINT uDockSide, int DockWidth)
{
	m_vDockables.push_back(pDockable);
	CBar* pBar = new CBar;
	m_vBars.push_back(pBar);
	pBar->Create(m_hWnd);
	pDockable->SetDockWidth(DockWidth);
	HWND hDockable = pDockable->Create(m_hWnd);
	Dock(hDockable, uDockSide);
}

void CDockFrame::Dock(HWND hDockable, UINT DockState)
{	
	CDockable* pDockable = (CDockable*)FromHandle(hDockable);
	if (pDockable)
	{
		pDockable->SetParent(GetApp()->GetFrame()->GetHwnd());
		DWORD dwStyle = WS_CHILD | WS_VISIBLE;
		pDockable->SetWindowLongPtr(GWL_STYLE, dwStyle);	
		pDockable->SetDockState(DockState);
		RecalcLayout();
		
		SetForegroundWindow();
		pDockable->SetFocus();
	}
	else MessageBox(m_hWnd, _T("Can't dock this type of window"), _T("Error"), MB_OK); 
}

UINT CDockFrame::GetDockSide(LPDRAGPOS pdp)
{
	CRect rcWindow = GetViewRect();
	MapWindowPoints(m_hWnd, NULL, (LPPOINT)&rcWindow, 2);
	
	CRect rcLeft = rcWindow;
	rcLeft.right = rcLeft.left + 30;
	if (rcLeft.PtInRect(pdp->ptPos))
		return DS_DOCK_LEFT;

	CRect rcRight = rcWindow;
	rcRight.left = rcRight.right - 30;
	if (rcRight.PtInRect(pdp->ptPos))
		return DS_DOCK_RIGHT;

	CRect rcTop = rcWindow;
	rcTop.InflateRect(-30, 0);
	rcTop.bottom = rcRight.top + 30;
	if (rcTop.PtInRect(pdp->ptPos))
		return DS_DOCK_TOP;

	CRect rcBottom = rcWindow;
	rcBottom.InflateRect(-30, 0);
	rcBottom.top = rcRight.bottom - 30;
	if (rcBottom.PtInRect(pdp->ptPos))
		return DS_DOCK_BOTTOM;

	return 0;
}

void CDockFrame::OnInitialUpdate()
{
	TRACE(_T("MDI Frame started \n"));
	//The frame is now created.
	//Place any additional startup code here.

	AddDockable(new CDockable, DS_DOCK_LEFT, 45);
	AddDockable(new CDockable, DS_DOCK_RIGHT, 120);
	AddDockable(new CDockable, DS_DOCK_BOTTOM, 90);
}

BOOL CDockFrame::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
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

void CDockFrame::OnCreate()
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
	GetView()->SetWindowLongPtr(GWL_EXSTYLE, NULL);
	RecalcLayout();
}

LRESULT CDockFrame::OnNotify(WPARAM /*wParam*/, LPARAM lParam)
{
	switch (((LPNMHDR)lParam)->code)
	{
	case USER_DRAGSTART:
		TRACE("Drag Start notification\n");
		break;

	case USER_DRAGMOVE:
	//	TRACE("Drag Move notification\n");
		{
			UINT uDockSide = GetDockSide((LPDRAGPOS)lParam);

			switch (uDockSide)
			{
			case DS_DOCK_LEFT:
				TRACE("Could dock Left\n");
				break;
			case DS_DOCK_RIGHT:
				TRACE("Could dock Right\n");
				break;
			case DS_DOCK_TOP:
				TRACE("Could dock Top\n");
				break;
			case DS_DOCK_BOTTOM:
				TRACE("Could dock Bottom\n");
				break;
			}
		}
		break;

	case USER_DRAGEND:
		TRACE("Drag End notification\n");
		{
			LPDRAGPOS pdp = (LPDRAGPOS)lParam;
			UINT DockSide = GetDockSide(pdp);
			HWND hDockable = pdp->hdr.hwndFrom;

			if (DockSide) Dock(hDockable, DockSide);
		}
		break;

	case WM_PARENTNOTIFY:
		TRACE("WM_PARENTNOTIFY\n");
		break;
	}

	return 0L;
}

void CDockFrame::RecalcLayout()
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


	if ((rClient.Height()) >= 0)
	{
		int x  = rClient.left;
		int y  = rClient.top;
		int cx = rClient.Width();
		int cy = rClient.Height();

		for (UINT i = 0 ; i < m_vDockables.size(); ++i)
		{
			int DockWidth = m_vDockables[i]->GetDockWidth();
			switch (m_vDockables[i]->m_DockState)
			{
			case DS_DOCK_LEFT:
				::SetWindowPos(m_vDockables[i]->GetHwnd(), NULL, x, y, DockWidth, cy, SWP_SHOWWINDOW );
				::SetWindowPos(m_vBars[i]->GetHwnd(), NULL, x + DockWidth, y, 5, cy, SWP_SHOWWINDOW );
				cx -= (DockWidth +5);
				x  += DockWidth +5;
				break;

			case DS_DOCK_RIGHT:
				::SetWindowPos(m_vDockables[i]->GetHwnd(), NULL, x + cx - DockWidth, y, DockWidth, cy, SWP_SHOWWINDOW );
				::SetWindowPos(m_vBars[i]->GetHwnd(), NULL, x + cx - DockWidth -5, y, 5, cy, SWP_SHOWWINDOW );
				cx -= (DockWidth +5);
				break;
				
			case DS_DOCK_TOP:
				DockWidth = min(DockWidth, cy);
				::SetWindowPos(m_vDockables[i]->GetHwnd(), NULL, x, y, cx, DockWidth, SWP_SHOWWINDOW );
				::SetWindowPos(m_vBars[i]->GetHwnd(), NULL, x, y + DockWidth, cx, 5, SWP_SHOWWINDOW );
				cy -= (DockWidth +5);
				y += DockWidth +5;
				break;

			case DS_DOCK_BOTTOM:
				DockWidth = min(DockWidth, cy);
				::SetWindowPos(m_vDockables[i]->GetHwnd(), NULL, x, max(y, y + cy - DockWidth), cx, DockWidth, SWP_SHOWWINDOW );
				::SetWindowPos(m_vBars[i]->GetHwnd(), NULL, x, max(y, y + cy - DockWidth-5), cx, 5, SWP_SHOWWINDOW );
				cy -= (DockWidth +5);
				break;

			default:
				break;
			}
		}
		::SetWindowPos(GetView()->GetHwnd(), NULL, x, y, cx, cy, SWP_SHOWWINDOW );		
	}

	if (GetRebar().GetRebarTheme().UseThemes && GetRebar().GetRebarTheme().KeepBandsLeft)
		GetRebar().MoveBandsLeft();

	if (IsMenubarUsed())
		SetMenubarBandSize();

	::SendMessage(m_hWnd, USER_REARRANGED, 0, 0);
}


LRESULT CDockFrame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{

//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

