//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "stdafx.h"
#include "view.h"
#include "resource.h"

CView::CView()
{
}
/*
BOOL CView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Process the messages from the child ToolBar
	UNREFERENCED_PARAMETER(lParam);

	switch(LOWORD(wParam))
	{
	case IDM_TOP:		OnTop();		return TRUE;
	case IDM_LEFT:		OnLeft();		return TRUE;
	case IDM_RIGHT:		OnRight();		return TRUE;
	case IDM_BOTTOM:	OnBottom();		return TRUE;
	}

	return FALSE;
}
*/
/*

void CView::OnBottom()
{
	DWORD dwStyle = m_ToolBar.GetWindowLongPtr(GWL_STYLE);
	dwStyle &= ~(CCS_VERT);
	dwStyle |= CCS_BOTTOM;
	m_ToolBar.SetWindowLongPtr(GWL_STYLE, dwStyle);
	
	dwStyle = m_ReBar.GetWindowLongPtr(GWL_STYLE);
	dwStyle &= ~(CCS_VERT);
	dwStyle |= CCS_BOTTOM;
	m_ReBar.SetWindowLongPtr(GWL_STYLE, dwStyle);
	RecalcLayout();
}

void CView::OnLeft()
{
	DWORD dwStyle = m_ToolBar.GetWindowLongPtr(GWL_STYLE);
	dwStyle &= ~(CCS_BOTTOM);
	dwStyle |= CCS_LEFT;
	m_ToolBar.SetWindowLongPtr(GWL_STYLE, dwStyle);
	
	dwStyle = m_ReBar.GetWindowLongPtr(GWL_STYLE);
	dwStyle &= ~(CCS_BOTTOM);
	dwStyle |= CCS_LEFT;
	m_ReBar.SetWindowLongPtr(GWL_STYLE, dwStyle);	
	RecalcLayout();
}

void CView::OnRight()
{
	DWORD dwStyle = m_ToolBar.GetWindowLongPtr(GWL_STYLE);
	dwStyle |= CCS_RIGHT;
	m_ToolBar.SetWindowLongPtr(GWL_STYLE, dwStyle);
	
	dwStyle = m_ReBar.GetWindowLongPtr(GWL_STYLE);
	dwStyle |= CCS_RIGHT;
	m_ReBar.SetWindowLongPtr(GWL_STYLE, dwStyle);	
	RecalcLayout();
}

void CView::OnTop()
{
	DWORD dwStyle = m_ToolBar.GetWindowLongPtr(GWL_STYLE);
	dwStyle &= ~(CCS_VERT | CCS_BOTTOM);
	m_ToolBar.SetWindowLongPtr(GWL_STYLE, dwStyle);

	dwStyle = m_ReBar.GetWindowLongPtr(GWL_STYLE);
	dwStyle &= ~(CCS_VERT | CCS_BOTTOM);
	m_ReBar.SetWindowLongPtr(GWL_STYLE, dwStyle);
	RecalcLayout();
}
*/

int CView::OnCreate(LPCREATESTRUCT pcs)
{
	UNREFERENCED_PARAMETER(pcs);

	// Create the ReBar
/*	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS 
					 | CCS_NORESIZE | CCS_NOPARENTALIGN | CCS_NODIVIDER | RBS_VARHEIGHT ;
	m_ReBar.CreateEx(0, REBARCLASSNAME, 0, dwStyle, CRect(0,0,0,0), this, 0);

	dwStyle |= TBSTYLE_FLAT;
	m_MenuBar.CreateEx(0, TOOLBARCLASSNAME, 0, dwStyle, CRect(0,0,0,0), &m_ReBar, 0);
	CFrame* pFrame = (CFrame*)GetAncestor();
	m_MenuBar.SetMenu(*pFrame->GetFrameMenu());

	// Fill the REBARBAND structure
	REBARBANDINFO rbbi = {0};
	rbbi.fMask      = RBBIM_CHILD |RBBIM_CHILDSIZE| RBBIM_ID | RBBIM_SIZE | RBBIM_STYLE;
	rbbi.fStyle     = RBBS_BREAK;
	rbbi.hwndChild  = m_MenuBar.GetHwnd();
	rbbi.wID        = IDW_MENUBAR;
	rbbi.cyMinChild = m_MenuBar.GetMaxSize().cy+1;
	m_ReBar.InsertBand(-1, rbbi);
	
	// Create the ToolBar's image list from 4 icons
	m_ToolBarImages.Create(48, 48, ILC_COLOR32 | ILC_MASK, 0, 0);
	m_ToolBarImages.Add(GetApp()->LoadIcon(IDI_TOP));
	m_ToolBarImages.Add(GetApp()->LoadIcon(IDI_LEFT));
	m_ToolBarImages.Add(GetApp()->LoadIcon(IDI_RIGHT));
	m_ToolBarImages.Add(GetApp()->LoadIcon(IDI_BOTTOM));

	// Create the ToolBar
	dwStyle |= TBSTYLE_TOOLTIPS | TBSTYLE_FLAT;
	m_ToolBar.CreateEx(0, TOOLBARCLASSNAME, 0, dwStyle, CRect(0,0,0,0), &m_ReBar, 0);
	m_ToolBar.SetImageList(&m_ToolBarImages);

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
	rbbi.fMask      = RBBIM_CHILD |RBBIM_CHILDSIZE| RBBIM_ID | RBBIM_SIZE | RBBIM_STYLE;
	rbbi.fStyle     = RBBS_BREAK;
	rbbi.hwndChild  = m_ToolBar.GetHwnd();
	rbbi.wID        = IDC_BANDARROWS;
	rbbi.cyMinChild = m_ToolBar.GetMaxSize().cy+1;
	m_ReBar.InsertBand(-1, rbbi);
*/	
	return 0;
}

void CView::OnDraw(CDC* pDC)
// OnDraw is called when part or all of the window needs to be redrawn
{
	UNREFERENCED_PARAMETER(pDC);

/*	CRect rc = GetClientRect();
	CRect rcRB = m_ReBar.GetWindowRect();

	// Calculate the view rect excluding the ToolBar rect
	ClientToScreen(rc);
	if (rc.Width() == rcRB.Width())
	{
		if (rc.top == rcRB.top)	rc.top += rcRB.Height();
		else					rc.bottom -= rcRB.Height(); 
	}
	else
	{
		if (rc.left == rcRB.left)	rc.left += rcRB.Width();
		else						rc.right -= rcRB.Width();
	}

	ScreenToClient(rc);

	// Display some text in our view window
	pDC->DrawText(_T("\nPress the arrows to change the inner toolbar's orientation,\n or choose customize toolbar from the ToolBar menu item to modify the toolbar in the frame."), -1, rc, DT_CENTER | DT_WORDBREAK);
	*/
}

void CView::OnInitialUpdate()
// OnInitialUpdate is called immediately after the window is created
{
	TRACE("View window created\n");
}

inline LRESULT CView::OnNotify(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	LPNMHDR pNMHDR = (LPNMHDR)lParam;
	switch (pNMHDR->code)
	{
	// Pass the ToolBar's ToolTip info up to the frame
	case TTN_GETDISPINFO: return GetParent()->SendMessage(WM_NOTIFY, wParam, lParam);
	}

	return 0L;
}

void CView::PreCreate(CREATESTRUCT &cs)
{
	// Here we set the defaults used by the create function for the view window
	// Preforming this is optional, but doing so allows us to
	// take more precise control over the window we create.

	// Set the extended style
	cs.dwExStyle = WS_EX_CLIENTEDGE;
}

void CView::PreRegisterClass(WNDCLASS &wc)
{
	// Here we set the Window class parameters.
	// Preforming this is optional, but doing so allows us to
	// take more precise control over the type of window we create.

	// Set the Window Class name
	wc.lpszClassName = _T("Win32++ View");

	// Set a background brush to white
	wc.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);

	// Set the default cursor
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);

	// Set the class style (not to be confused with the window styles set in PreCreate)
	wc.style = CS_DBLCLKS;	// Generate left button double click messages
}
/*
void CView::RecalcLayout()
{
	CRect rcView = GetClientRect();
	
	// Position the toolbar at the top, left, right or bottom of the view.
	int nReBarHeight = 0;
	for (UINT u = 0 ; u < m_ReBar.GetRowCount(); ++u)
		nReBarHeight += m_ReBar.GetRowHeight(u);

	int cxRB = nReBarHeight;
	int cyRB = nReBarHeight;
	int cxClient = GetClientRect().Width();
	int cyClient = GetClientRect().Height();

	DWORD dwStyle = m_ReBar.GetWindowLongPtr(GWL_STYLE);
	dwStyle &= CCS_VERT | CCS_BOTTOM; // Filter unwanted styles

	switch(dwStyle)
	{
	case CCS_LEFT:
		m_ReBar.SetWindowPos(NULL, 0, 0, cxRB, cyClient, SWP_SHOWWINDOW);
		SetWrapState(&m_ToolBar, TRUE);
		SetWrapState(&m_MenuBar, TRUE);
		m_ToolBar.PressButton(IDM_LEFT, TRUE);
		break;
	case CCS_RIGHT:
		m_ReBar.SetWindowPos(NULL, cxClient - cxRB, 0, cxRB, cyClient, SWP_SHOWWINDOW);
		SetWrapState(&m_ToolBar, TRUE);
		SetWrapState(&m_MenuBar, TRUE);
		m_ToolBar.PressButton(IDM_RIGHT, TRUE);
		break;
	case CCS_BOTTOM:
		m_ReBar.SetWindowPos(NULL, 0, cyClient - cyRB, cxClient, cyRB, SWP_SHOWWINDOW);
		SetWrapState(&m_ToolBar, FALSE);
		SetWrapState(&m_MenuBar, FALSE);
		m_ToolBar.PressButton(IDM_BOTTOM, TRUE);
		break;
	default:
		m_ReBar.SetWindowPos(NULL, 0, 0, cxClient, cyRB, SWP_SHOWWINDOW);
		SetWrapState(&m_ToolBar, FALSE);
		SetWrapState(&m_MenuBar, FALSE);
		m_ToolBar.PressButton(IDM_TOP, TRUE);
		break;
	}

	Invalidate();
	m_ReBar.RedrawWindow(); 
}

// Note: Vertical toolbars require each button to have TBSTATE_WRAP, set for group buttons to work.
//       Horizontal toolbars require the TBSTATE_WRAP removed.
void CView::SetWrapState(CToolBar* pToolBar, BOOL bWrap)
{
	for (int i = 0; i < m_ToolBar.GetButtonCount(); ++i)
	{
		int nID = pToolBar->GetCommandID(i);
		pToolBar->SetButtonState(nID, TBSTATE_ENABLED | (bWrap? TBSTATE_WRAP : 0));
	}
}
*/
LRESULT CView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
// All window messages for this window pass through WndProc
{
//	switch (uMsg)
//	{
//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}



