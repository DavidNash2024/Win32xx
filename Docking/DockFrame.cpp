////////////////////////////////////////////////////
// DockFrame.cpp  - definitions for the CDockFrame class


#include "resource.h"
//#include "DockFrame.h"
#include "DockingApp.h"
#include "MDIChildView.h"


CDockFrame::CDockFrame() : m_BarWidth(4)
{
	WORD HashPattern[] = {0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA};
	m_hbm = ::CreateBitmap (8, 8, 1, 1, HashPattern);
	m_hbrDithered = ::CreatePatternBrush (m_hbm);

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

	::DeleteObject(m_hbrDithered);
	::DeleteObject(m_hbm);
}

void CDockFrame::AddDockable(CDockable* pDockable, UINT uDockSide, int DockWidth)
{
	// Create the dockable window
	pDockable->SetDockWidth(DockWidth);
	HWND hDockable = pDockable->Create(m_hWnd);
	m_vDockables.push_back(pDockable);

	// Also create a new splitter bar
	CBar* pBar = new CBar;
	pBar->Create(m_hWnd);
	m_vBars.push_back(pBar);

	// Dock the dockable window
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
		for (UINT u = 0; u < m_vDockables.size(); ++u)
		{
			if (hDockable == m_vDockables[u]->GetHwnd())
			{
				// Re-order m_vDockables, putting this pDockable last  
				m_vDockables.erase(m_vDockables.begin() + u);
				m_vDockables.push_back(pDockable);
				break;
			}
		}

		RecalcLayout();
		
		SetForegroundWindow();
		pDockable->SetFocus();
	}
}

void CDockFrame::DrawHashBar(HWND hBar, POINT Pos)
{
	// draws a hashed bar while the splitter bar is being dragged
	{
		CDockable* pDock = ((CBar*)FromHandle(hBar))->m_pDockable;
		BOOL bVertical = (pDock->GetDockState() == DS_DOCKED_LEFT) || (pDock->GetDockState() == DS_DOCKED_RIGHT);
		
		CDC BarDC = ::GetDC(m_hWnd);
		BarDC.AttachBrush(m_hbrDithered);
		
		CRect rc = pDock->GetWindowRect();
		MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rc, 2);
		int cx = rc.Width();
		int cy = rc.Height();

		if (bVertical)
			::PatBlt (BarDC, Pos.x - m_BarWidth/2, rc.top, m_BarWidth, cy, PATINVERT);
		else
			::PatBlt (BarDC, rc.left, Pos.y - m_BarWidth/2, cx, m_BarWidth, PATINVERT);
		
		BarDC.DetachBrush();
	}
}

CDockable* CDockFrame::GetDockNeighbour(CDockable* pDock)
{
	for (UINT u = 0; u < m_vDockables.size(); ++u)
	{
		if (pDock == m_vDockables[u])
		{
			UINT uDockState = pDock->GetDockState();
			for (UINT v = u+1; v < m_vDockables.size(); ++v)
			{
				if (uDockState == m_vDockables[v]->GetDockState())
					return m_vDockables[v];
			}
		}
	}

	return NULL;
}

UINT CDockFrame::GetDockSide(LPDRAGPOS pdp)
{
	CRect rcWindow = GetView()->GetClientRect();
	MapWindowPoints(GetView()->GetHwnd(), NULL, (LPPOINT)&rcWindow, 2);
	
	CRect rcLeft = rcWindow;
	rcLeft.right = rcLeft.left + 30;
	if (rcLeft.PtInRect(pdp->ptPos))
		return DS_DOCKED_LEFT;

	CRect rcRight = rcWindow;
	rcRight.left = rcRight.right - 30;
	if (rcRight.PtInRect(pdp->ptPos))
		return DS_DOCKED_RIGHT;

	CRect rcTop = rcWindow;
	rcTop.InflateRect(-30, 0);
	rcTop.bottom = rcRight.top + 30;
	if (rcTop.PtInRect(pdp->ptPos))
		return DS_DOCKED_TOP;

	CRect rcBottom = rcWindow;
	rcBottom.InflateRect(-30, 0);
	rcBottom.top = rcRight.bottom - 30;
	if (rcBottom.PtInRect(pdp->ptPos))
		return DS_DOCKED_BOTTOM;

	return 0;
}

void CDockFrame::OnInitialUpdate()
{
	TRACE(_T("Docking Frame started \n"));
	//The frame is now created.
	//Place any additional startup code here.

	for (int i = 0 ; i < 4 ; ++i)
	{
		AddDockable(new CDockable, DS_DOCKED_LEFT, 45);
		AddDockable(new CDockable, DS_DOCKED_RIGHT, 120);
		AddDockable(new CDockable, DS_DOCKED_BOTTOM, 90);
		AddDockable(new CDockable, DS_DOCKED_TOP, 100);

	}
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
//	GetView()->SetWindowLongPtr(GWL_EXSTYLE, NULL);
//	RecalcLayout();
}

LRESULT CDockFrame::OnNotify(WPARAM /*wParam*/, LPARAM lParam)
{
	static POINT OldPoint = {0};
	LPDRAGPOS pdp = (LPDRAGPOS)lParam;

	switch (((LPNMHDR)lParam)->code)
	{
	case DN_DOCK_START:
		TRACE("Drag Start notification\n");
		break;

	case DN_DOCK_MOVE:
	//	TRACE("Drag Move notification\n");
		{
			UINT uDockSide = GetDockSide((LPDRAGPOS)lParam);

			switch (uDockSide)
			{
			case DS_DOCKED_LEFT:
				TRACE("Could dock Left\n");
				break;
			case DS_DOCKED_RIGHT:
				TRACE("Could dock Right\n");
				break;
			case DS_DOCKED_TOP:
				TRACE("Could dock Top\n");
				break;
			case DS_DOCKED_BOTTOM:
				TRACE("Could dock Bottom\n");
				break;
			}
		}
		break;

	case DN_DOCK_END:
		TRACE("Drag End notification\n");
		{
			UINT DockSide = GetDockSide(pdp);
			HWND hDockable = pdp->hdr.hwndFrom;

			if (DockSide) Dock(hDockable, DockSide);
		}
		break;

	case DN_BAR_START:
		{
			TRACE("Start drag bar notification\n");	
			POINT pt = pdp->ptPos;
			MapWindowPoints(NULL, m_hWnd, &pt, 1);
			DrawHashBar(pdp->hdr.hwndFrom, pt);
			OldPoint = pt;
		}
		break;

	case DN_BAR_MOVE:
		{
			TRACE("bar dragged notification\n");
			CPoint pt = pdp->ptPos;
			MapWindowPoints(NULL, m_hWnd, &pt, 1);

			CDockable* pDock = ((CBar*)FromHandle(pdp->hdr.hwndFrom))->m_pDockable;
			RECT rcDock = pDock->GetWindowRect();
			MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcDock, 2);

			CWnd* pDockNbr = GetDockNeighbour(pDock);
			if (!pDockNbr) 
				pDockNbr = GetView();
			RECT rcDockNbr = pDockNbr->GetWindowRect();
			MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcDockNbr, 2);
			
			CPoint ptmin(min(rcDock.left, rcDockNbr.left), min(rcDock.top, rcDockNbr.top));
			CPoint ptmax(max(rcDock.right, rcDockNbr.right), max(rcDock.bottom, rcDockNbr.bottom));

			if (pt != OldPoint)			
			{
				DrawHashBar(pdp->hdr.hwndFrom, OldPoint);
				DrawHashBar(pdp->hdr.hwndFrom, pt);
				OldPoint = pt;
			} 
		}
		break;

	case DN_BAR_END:
		{
			TRACE("End drag bar notification\n");
			POINT pt = pdp->ptPos;
			MapWindowPoints(NULL, m_hWnd, &pt, 1);

			CDockable* pDock = ((CBar*)FromHandle(pdp->hdr.hwndFrom))->m_pDockable;
			CRect rcDock = pDock->GetWindowRect();
			MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcDock, 2);

			CWnd* pDockNbr = GetDockNeighbour(pDock);
			if (!pDockNbr) 
				pDockNbr = GetView();
			RECT rcDockNbr = pDockNbr->GetWindowRect();
			MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcDockNbr, 2);
			
			CPoint ptmin(min(rcDock.left, rcDockNbr.left), min(rcDock.top, rcDockNbr.top));
			CPoint ptmax(max(rcDock.right, rcDockNbr.right), max(rcDock.bottom, rcDockNbr.bottom));

			DrawHashBar(pdp->hdr.hwndFrom, pt);

			switch (pDock->GetDockState())
			{
			case DS_DOCKED_LEFT:
				{
					// Resize dockable
					pDock->SetDockWidth(pt.x - rcDock.left - m_BarWidth/2);
					
					// Resize neighbouring dockable
					CDockable* pDockNbr = GetDockNeighbour(pDock);
					if (pDockNbr)
					{
						rcDock = pDockNbr->GetWindowRect();
						MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcDock, 2);						
						pDockNbr->SetDockWidth(rcDock.right - pt.x - m_BarWidth/2);
					}
				}
				break;
			case DS_DOCKED_RIGHT:
				{
					// Resize dockable
					pDock->SetDockWidth(rcDock.right - pt.x - m_BarWidth/2);

					// Resize neighbouring dockable
					CDockable* pDockNbr = GetDockNeighbour(pDock);
					if (pDockNbr)
					{
						rcDock = pDockNbr->GetWindowRect();
						MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcDock, 2);
						pDockNbr->SetDockWidth(pt.x - rcDock.left - m_BarWidth/2);
					}
				}
				break;
			case DS_DOCKED_TOP:
				{
					// Resize dockable
					pDock->SetDockWidth(pt.y - rcDock.top - m_BarWidth/2);

					// Resize neighbouring dockable
					CDockable* pDockNbr = GetDockNeighbour(pDock);
					if (pDockNbr)
					{
						rcDock = pDockNbr->GetWindowRect();
						MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcDock, 2);
						pDockNbr->SetDockWidth(rcDock.bottom - pt.y - m_BarWidth/2);
					}
				}
				break;
			case DS_DOCKED_BOTTOM:
				{
					// Resize dockable
					pDock->SetDockWidth(rcDock.bottom - pt.y - m_BarWidth/2);

					// Resize neighbouring dockable
					CDockable* pDockNbr = GetDockNeighbour(pDock);
					if (pDockNbr)
					{
						rcDock = pDockNbr->GetWindowRect();
						MapWindowPoints(NULL, m_hWnd, (LPPOINT)&rcDock, 2);
						pDockNbr->SetDockWidth(pt.y - rcDock.top - m_BarWidth/2);
					}
					break;
				}
			}
			RecalcLayout();
		
		}
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

		int bw = m_BarWidth;	// Width of the splitter bar
		
		HDWP hDwp = BeginDeferWindowPos(2*m_vDockables.size());

		for (UINT i = 0 ; i < m_vDockables.size(); ++i)
		{
			int DockWidth = m_vDockables[i]->GetDockWidth();
			switch (m_vDockables[i]->m_DockState)
			{
			case DS_DOCKED_LEFT:
				{
				DockWidth = min(DockWidth, cx);
				hDwp = ::DeferWindowPos(hDwp, m_vDockables[i]->GetHwnd(), NULL, x, y, DockWidth, cy, SWP_SHOWWINDOW );
				int bw1 = min(bw, cx-DockWidth);
				hDwp = ::DeferWindowPos(hDwp, m_vBars[i]->GetHwnd(), NULL, x + DockWidth, y, bw1, cy, SWP_SHOWWINDOW );
				m_vBars[i]->m_pDockable = m_vDockables[i];
				cx -= (DockWidth +bw1);
				x  += DockWidth +bw1;
				}
				break;

			case DS_DOCKED_RIGHT:
				{
				DockWidth = min(DockWidth, cx);
				hDwp = ::DeferWindowPos(hDwp, m_vDockables[i]->GetHwnd(), NULL, max(x, x + cx - DockWidth), y, DockWidth, cy, SWP_SHOWWINDOW );
				int bw1 = min(bw, cx-DockWidth);
				hDwp = ::DeferWindowPos(hDwp, m_vBars[i]->GetHwnd(), NULL, max(x, x + cx - DockWidth -bw1), y, bw1, cy, SWP_SHOWWINDOW );
				m_vBars[i]->m_pDockable = m_vDockables[i];
				cx -= (DockWidth +bw1);
				}
				break;

			case DS_DOCKED_TOP:
				{
				DockWidth = min(DockWidth, cy);
				hDwp = ::DeferWindowPos(hDwp, m_vDockables[i]->GetHwnd(), NULL, x, y, cx, DockWidth, SWP_SHOWWINDOW );
				int bw1 = min(bw, cy-DockWidth);
				hDwp = ::DeferWindowPos(hDwp, m_vBars[i]->GetHwnd(), NULL, x, y + DockWidth, cx, bw1, SWP_SHOWWINDOW );
				m_vBars[i]->m_pDockable = m_vDockables[i];
				cy -= (DockWidth +bw1);
				y += DockWidth +bw1;
				}
				break;
				
			case DS_DOCKED_BOTTOM:
				{
				DockWidth = min(DockWidth, cy);
				hDwp = ::DeferWindowPos(hDwp, m_vDockables[i]->GetHwnd(), NULL, x, max(y, y + cy - DockWidth), cx, DockWidth, SWP_SHOWWINDOW );
				int bw1 = min(bw, cy-DockWidth);
				hDwp = ::DeferWindowPos(hDwp, m_vBars[i]->GetHwnd(), NULL, x, max(y, y + cy - DockWidth-bw1), cx, min(cy, bw1), SWP_SHOWWINDOW );
				m_vBars[i]->m_pDockable = m_vDockables[i];
				cy -= (DockWidth +bw1);
				}
				break;

			default:
				m_vBars[i]->ShowWindow(SW_HIDE);
				m_vBars[i]->m_pDockable = NULL;
				break;
			}
		}
		hDwp = ::DeferWindowPos(hDwp, GetView()->GetHwnd(), NULL, x, y, cx, cy, SWP_SHOWWINDOW );
		EndDeferWindowPos(hDwp);
	}

	if (GetRebar().GetRebarTheme().UseThemes && GetRebar().GetRebarTheme().KeepBandsLeft)
		GetRebar().MoveBandsLeft();

	if (IsMenubarUsed())
		SetMenubarBandSize();

	::SendMessage(m_hWnd, USER_REARRANGED, 0, 0);
}


LRESULT CDockFrame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SETCURSOR:
	//	TRACE("WM_SETCURSOR from CDockFrame\n");
		break;
	}

	// pass unhandled messages on for default processing
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

void CDockFrame::CBar::SendNotify(UINT nMessageID)
{	
	// Send a splitter bar notification to the frame
	DRAGPOS DragPos;
	DragPos.hdr.code = nMessageID;
	DragPos.hdr.hwndFrom = m_hWnd;
	GetCursorPos(&DragPos.ptPos);
	DragPos.ptPos.x += 1;
	SendMessage(GetApp()->GetFrame()->GetHwnd(), WM_NOTIFY, 0, (LPARAM)&DragPos);
}

LRESULT CDockFrame::CBar::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_SETCURSOR:
		{
			if (m_pDockable)
			{
				UINT uSide = m_pDockable->m_DockState;
				if ((uSide == DS_DOCKED_LEFT) || (uSide == DS_DOCKED_RIGHT))	
					SetCursor(LoadCursor(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDC_SPLITH)));
				else				
					SetCursor(LoadCursor(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDC_SPLITV)));
				
				return TRUE;
			}
		}			
		break;

	case WM_LBUTTONDOWN:
		SendNotify(DN_BAR_START);
		SetCapture();
		m_IsCaptured = TRUE;
		break;
	
	case WM_LBUTTONUP:
		if (m_IsCaptured)
		{
			SendNotify(DN_BAR_END);
			ReleaseCapture();
			m_IsCaptured = FALSE;
		}
		break;

	case WM_MOUSEMOVE:
		if (m_IsCaptured)
		{
			SendNotify(DN_BAR_MOVE);
		}
		break;
	}

	// pass unhandled messages on for default processing
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

