////////////////////////////////////////////////////
//Mainfrm.cpp
// Definitions for the CMainFrame class

#include "resource.h"
#include "mainfrm.h"


CMainFrame::CMainFrame() : m_hBitmap(NULL)
{
	SetView(m_View);

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
	if (m_hBitmap != NULL)
		::DeleteObject(m_hBitmap);
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	switch(LOWORD(wParam))
	{
	case IDM_FILE_EXIT:
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	case IDM_HELP_ABOUT:
		OnHelp();
		return TRUE;
	}
	return FALSE;
}

void CMainFrame::OnInitialUpdate()
{
	//Set Background
	m_hBitmap = ::LoadBitmap(GetApp()->GetInstanceHandle(), MAKEINTRESOURCE(IDB_BACKGROUND));
	SetBackground(m_hBitmap);
}


LRESULT CMainFrame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//	}

	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}


