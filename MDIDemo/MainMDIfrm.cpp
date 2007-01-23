////////////////////////////////////////////////////
// MainMDIfrm.cpp  - definitions for the CMainMDIFrame class


#include "resource.h"
#include "mainMDIfrm.h"
#include "MDIChildView.h"
#include "MDIChildRect.h"
#include "MDIChildMax.h"


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
}

CMainMDIFrame::~CMainMDIFrame()
{
}

void CMainMDIFrame::OnInitialUpdate()
{
	//The frame is now created.
	//Place any additional startup code here.
}

BOOL CMainMDIFrame::OnCommand(UINT nID)
{
	switch (nID)
	{
	case IDM_FILE_NEWVIEW:
		AddMDIChild(new CMDIChildView);
		return 0;
	case IDM_FILE_NEWRECT:
		AddMDIChild(new CMDIChildRect);
		return 0;
	case IDM_FILE_NEWMAX:
		AddMDIChild(new CMDIChildMax);
		return 0;
	case IDM_FILE_CLOSE:          // Close the active MDI window
		::SendMessage(GetActiveChild(), WM_CLOSE, 0, 0);
		break;
	case IDM_FILE_EXIT:
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		break;
	case IDM_HELP_ABOUT:
		OnHelp();
		break;
	}
	return CMDIFrame::OnCommand(nID);
}

LRESULT CMainMDIFrame::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{

//	}

	//Use the frame default message handling for remaining messages
	return CMDIFrame::WndProc(hwnd, uMsg, wParam, lParam);
}

