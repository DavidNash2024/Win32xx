#include "MainFrm.h"
#include "Resource.h"


CMainFrame::CMainFrame()
{
	// Set the Resource IDs for the toolbar buttons
	m_ToolbarData.clear();
	m_ToolbarData.push_back ( 0 );				// Separator
	m_ToolbarData.push_back ( IDM_BOLD  );
	m_ToolbarData.push_back ( IDM_ITALIC);
	m_ToolbarData.push_back ( IDM_UNDERLINE);

}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Respond to menu and toolbar selections
	switch (LOWORD(wParam))
	{
	// Respond to menu items
	case IDM_EDIT_CUT:
		TRACE(TEXT("Cut ... Menu item"));
		return TRUE;
	case IDM_EDIT_COPY:
		TRACE(TEXT("Copy ... Menu item"));
		return TRUE;
    case IDM_EDIT_PASTE:
		TRACE(TEXT("PASTE ... Menu item"));
		return TRUE;
	case IDM_EDIT_CLEAR:
		TRACE(TEXT("CLEAR ... Menu item"));
		return TRUE;
	case IDM_HELP_ABOUT:
		{
			CDialog HelpDialog(IDW_ABOUT, m_hWnd);
			HelpDialog.DoModal();
		}
		return TRUE;

	// Respond to Toolbar buttons
	case IDM_BOLD:
		TRACE(TEXT("Bold button pressed"));
		return TRUE;
	case IDM_ITALIC:
		TRACE(TEXT("Italic button pressed"));
		return TRUE;
	case IDM_UNDERLINE:
		TRACE(TEXT("Underline button pressed"));
		return TRUE;

	// Respond to the accelerator key
	case IDW_QUIT:
		::SendMessage(m_hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	}

	return FALSE;
}

void CMainFrame::OnInitialUpdate()
{
	// Startup code goes here
	TRACE(TEXT("Frame Created"));

	TRACE(TEXT("Move the trace window up to see menu"));

}

void CMainFrame::OnPaint(HDC hDC)
{
	RECT rt = GetClientRect();
	LPCTSTR szHello = LoadString(IDS_HELLO);
	DrawText(hDC, szHello, lstrlen(szHello), &rt, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
}

LRESULT CMainFrame::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

//	switch (uMsg)
//	{
//
//	}

	// Pass unhandled messages on for default processing
	return WndProcDefault(hwnd, uMsg, wParam, lParam);
}
