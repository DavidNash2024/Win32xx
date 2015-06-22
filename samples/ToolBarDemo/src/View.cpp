//////////////////////////////////////////////
// View.cpp
//  Definitions for the CView class

#include "stdafx.h"
#include "view.h"
#include "resource.h"

CView::CView()
{
}

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

void CView::OnBottom()
{
	DWORD dwStyle = m_ToolBar.GetWindowLongPtr(GWL_STYLE);

	dwStyle &= ~(CCS_VERT);
	dwStyle |= CCS_BOTTOM;
	m_ToolBar.SetWindowLongPtr(GWL_STYLE, dwStyle);
	RecalcLayout();
}

void CView::OnLeft()
{
	DWORD dwStyle = m_ToolBar.GetWindowLongPtr(GWL_STYLE);

	dwStyle &= ~(CCS_BOTTOM);
	dwStyle |= CCS_LEFT;
	m_ToolBar.SetWindowLongPtr(GWL_STYLE, dwStyle);
	RecalcLayout();
}

void CView::OnRight()
{
	DWORD dwStyle = m_ToolBar.GetWindowLongPtr(GWL_STYLE);

	dwStyle |= CCS_RIGHT;
	m_ToolBar.SetWindowLongPtr(GWL_STYLE, dwStyle);
	RecalcLayout();
}

void CView::OnTop()
{
	DWORD dwStyle = m_ToolBar.GetWindowLongPtr(GWL_STYLE);

	dwStyle &= ~(CCS_VERT | CCS_BOTTOM);
	m_ToolBar.SetWindowLongPtr(GWL_STYLE, dwStyle);
	RecalcLayout();
}

int CView::OnCreate(LPCREATESTRUCT pcs)
{
	UNREFERENCED_PARAMETER(pcs);

	// Create the ToolBar's image list from 4 icons
	m_ToolBarImages.Create(48, 48, ILC_COLOR32 | ILC_MASK, 0, 0);
	m_ToolBarImages.Add(GetApp().LoadIcon(IDI_TOP));
	m_ToolBarImages.Add(GetApp().LoadIcon(IDI_LEFT));
	m_ToolBarImages.Add(GetApp().LoadIcon(IDI_RIGHT));
	m_ToolBarImages.Add(GetApp().LoadIcon(IDI_BOTTOM));

	// Create the ToolBar
	m_ToolBar.Create(*this);
	m_ToolBar.SetImageList(m_ToolBarImages);

	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | TBSTYLE_TOOLTIPS
					| TBSTYLE_FLAT | CCS_NORESIZE | CCS_NOPARENTALIGN | CCS_NODIVIDER ;

	m_ToolBar.SetWindowLongPtr(GWL_STYLE, dwStyle);

	// Add the ToolBar buttons
	TBBUTTON ButtonInfo[] =
	{
		{ 0, IDM_TOP,		TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK|TBSTYLE_GROUP, {0}, 0, 0 },
		{ 1, IDM_LEFT,		TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK|TBSTYLE_GROUP, {0}, 0, 0 },
		{ 2, IDM_RIGHT,		TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK|TBSTYLE_GROUP, {0}, 0, 0 },
		{ 3, IDM_BOTTOM,	TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK|TBSTYLE_GROUP, {0}, 0, 0 }
	};
	m_ToolBar.AddButtons(4, ButtonInfo);

	return 0;
}

void CView::OnDraw(CDC& dc)
// OnDraw is called when part or all of the window needs to be redrawn
{
	CRect rc = GetClientRect();
	CRect rcTB = m_ToolBar.GetWindowRect();

	// Calculate the view rect excluding the ToolBar rect
	ClientToScreen(rc);
	if (rc.Width() == rcTB.Width())
	{
		if (rc.top == rcTB.top)	rc.top += rcTB.Height();
		else					rc.bottom -= rcTB.Height();
	}
	else
	{
		if (rc.left == rcTB.left)	rc.left += rcTB.Width();
		else						rc.right -= rcTB.Width();
	}

	ScreenToClient(rc);

	// Display some text in our view window
	dc.DrawText(_T("\nPress the arrows to change the inner toolbar's orientation,\n or choose customize toolbar from the ToolBar menu item to modify the toolbar in the frame."), -1, rc, DT_CENTER | DT_WORDBREAK);
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
	case TTN_GETDISPINFO: return GetParent().SendMessage(WM_NOTIFY, wParam, lParam);
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

void CView::RecalcLayout()
{
	// Position the toolbar at the top, left, right or bottom of the view.
	int cxTB = m_ToolBar.GetMaxSize().cx;
	int cyTB = m_ToolBar.GetMaxSize().cy;
	int cxClient = GetClientRect().Width();
	int cyClient = GetClientRect().Height();

	DWORD dwStyle = m_ToolBar.GetWindowLongPtr(GWL_STYLE);
	dwStyle &= CCS_VERT | CCS_BOTTOM; // Filter unwanted styles

	switch(dwStyle)
	{
	case CCS_LEFT:
		m_ToolBar.SetWindowPos(NULL, 0, 0, cxTB, cyClient, SWP_SHOWWINDOW);
		SetWrapState(TRUE);
		m_ToolBar.PressButton(IDM_LEFT, TRUE);
		break;
	case CCS_RIGHT:
		m_ToolBar.SetWindowPos(NULL, cxClient - cxTB, 0, cxTB, cyClient, SWP_SHOWWINDOW);
		SetWrapState(TRUE);
		m_ToolBar.PressButton(IDM_RIGHT, TRUE);
		break;
	case CCS_BOTTOM:
		m_ToolBar.SetWindowPos(NULL, 0, cyClient - cyTB, cxClient, cyTB, SWP_SHOWWINDOW);
		SetWrapState(FALSE);
		m_ToolBar.PressButton(IDM_BOTTOM, TRUE);
		break;
	default:
		m_ToolBar.SetWindowPos(NULL, 0, 0, cxClient, cyTB, SWP_SHOWWINDOW);
		SetWrapState(FALSE);
		m_ToolBar.PressButton(IDM_TOP, TRUE);
		break;
	}

	Invalidate();
}

// Note: Vertical toolbars require each button to have TBSTATE_WRAP, set for group buttons to work.
//       Horizontal toolbars require the TBSTATE_WRAP removed.
void CView::SetWrapState(BOOL bWrap)
{
	for (int i = 0; i < m_ToolBar.GetButtonCount(); ++i)
	{
		int nID = m_ToolBar.GetCommandID(i);
		m_ToolBar.SetButtonState(nID, TBSTATE_ENABLED | (bWrap? TBSTATE_WRAP : 0));
	}
}

LRESULT CView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
// All window messages for this window pass through WndProc
{
	switch (uMsg)
	{
	case WM_SIZE:
		RecalcLayout();
		Invalidate();
		break;
	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}



