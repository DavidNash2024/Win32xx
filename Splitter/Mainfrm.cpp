////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class


#include "mainfrm.h"
#include "resource.h"


CMainFrame::CMainFrame() : m_MainView(150, false)
{
	SetView(m_MainView);

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
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case IDM_FILE_EXIT:
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		break;
	case IDM_HELP_ABOUT:
		OnHelp();
		break;
	}
	return CFrame::OnCommand(wParam, lParam);
}

LRESULT CMainFrame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		if (::IsWindow(m_MainView.GetHwnd()))
			m_MainView.Reposition();
		break;
	case USER_REARRANGED:
		if (::IsWindow(m_MainView.GetHwnd()))
			m_MainView.Reposition();
		break;
	}
	return CFrame::WndProc(hWnd, uMsg, wParam, lParam);
}
