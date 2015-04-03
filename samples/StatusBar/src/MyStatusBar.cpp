#include "stdafx.h"
#include "MyStatusBar.h"


void CMyStatusBar::OnAttach() 
{
	// Start a timer for the progress bar
	SetTimer(101, 100, 0);

	// Create the StatusBar parts
	SetStatusParts();

	// Set the background color (ignored if XP themes are enabled)
	SendMessage(SB_SETBKCOLOR, 0, RGB(125, 230, 255));

	// Create the Hyperlink
	m_Hyperlink.Create(*this);
	m_Hyperlink.SetWindowPos(NULL, CRect(3,3,100, 20), SWP_SHOWWINDOW);

	// Create the ProgressBar
	m_ProgressBar.Create(*this);
	CRect rcPart = GetPartRect(1);
	m_ProgressBar.SetWindowPos(NULL, rcPart, SWP_SHOWWINDOW);

	// Add the icon
	SetPartIcon(2, (HICON)GetApp()->LoadImage(MAKEINTRESOURCE(IDW_MAIN), IMAGE_ICON, 16, 16));

	// Set a Part as owner drawn. The part's drawing is performed in CMainFrame::OnDrawItem.
	SetPartText(3, _T(""), SBT_OWNERDRAW);
}

BOOL CMyStatusBar::OnEraseBkgnd(CDC& dc)
{
	// The background can be set here if XP themes are enabled 
	if (IsXPThemed())
	{
		// Fill the background with a color gradient
		dc.GradientFill(RGB(125, 230, 255), RGB(250, 150, 150), GetClientRect(), TRUE); 
		return TRUE;
	}

	return FALSE;
}

void CMyStatusBar::PreCreate(CREATESTRUCT &cs)
{
	cs.style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CCS_BOTTOM;
	
	// Suppress the gripper unless XPThemes are enabled
	if (IsXPThemed())
		cs.style |= SBARS_SIZEGRIP;
}

void CMyStatusBar::SetStatusParts()
{
	// Create the StatusBar parts
	CRect rcClient = GetClientRect();
	int width = MAX(270, rcClient.right);
	SetPartWidth(0, width - 210);
	SetPartWidth(1, 100);
	SetPartWidth(2, 20);
	SetPartWidth(3, 90); 
}

LRESULT CMyStatusBar::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch(uMsg)
	{
	case WM_TIMER:
		{	
			// Change the Progress Bar indication
			m_ProgressBar.OffsetPos(1);
			if (m_ProgressBar.GetRange(FALSE, 0) == m_ProgressBar.GetPos())
				m_ProgressBar.SetPos(0);

			break;
		}
	case WM_SIZE:
		{
			// Reposition the StatusBar parts
			SetStatusParts();

			// Reposition the Progress Bar
			CRect rcPart = GetPartRect(1);
			m_ProgressBar.SetWindowPos(NULL, rcPart, SWP_SHOWWINDOW);
			break;
		}
	}

	return CStatusBar::WndProc(uMsg, wParam, lParam);
}