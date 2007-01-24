////////////////////////////////////////////////////
// Mainfrm.cpp  


#include "resource.h"
#include "mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	//Set m_View as the view window of the frame
	SetView(m_View);

	// Set the Resource IDs for the toolbar buttons
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

BOOL CMainFrame::OnCommand(UINT nID)
{
	switch(nID)
	{
	case IDM_FILE_EXIT:
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		break;
	case IDM_HELP_ABOUT:
		OnHelp();
		break;
	}
	return CFrame::OnCommand(nID);
}

void CMainFrame::OnCreate()
{
	// OnCreate controls the way the frame is created
	// This function is optional

	// m_bUseMenubar = FALSE;
	// m_bUseRebar = FALSE;
	// m_bUseStatusIndicators = FALSE;
	
	CFrame::OnCreate();
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created. 
	// Place any additional startup code here.

	// Disable some of the toolbar buttons
	GetToolbar().SetButtonState(IDM_EDIT_CUT, 0);
	GetToolbar().SetButtonState(IDM_EDIT_COPY, 0);
	GetToolbar().SetButtonState(IDM_EDIT_PASTE, 0);

	TRACE("Frame created");
}

void CMainFrame::SetButtons(std::vector<UINT> ToolbarData)
{
	CToolbar& TB = GetToolbar();
	
	// Call base class SetButtons function
	CFrame::SetButtons(ToolbarData);

	// Add some text to the buttons
	TB.SetButtonText(TB.CommandToIndex(IDM_FILE_NEW),  "NEW");
	TB.SetButtonText(TB.CommandToIndex(IDM_FILE_OPEN), "OPEN");
	TB.SetButtonText(TB.CommandToIndex(IDM_FILE_SAVE), "SAVE");
	TB.SetButtonText(TB.CommandToIndex(IDM_EDIT_CUT),  "CUT");
	TB.SetButtonText(TB.CommandToIndex(IDM_FILE_PRINT),"PRINT");

	// Adjust the toolbar button sizes
	RECT r;
	TB.GetItemRect(0, &r);
	SIZE sizeButton = {r.right - r.left, r.bottom - r.top}; 
	SIZE sizeImage = {16, 15};
	TB.SetSizes(sizeButton, sizeImage); 
}

LRESULT CMainFrame::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
		//Additional messages to be handled go here	
//	} 

	//Use the frame default message handling for remaining messages
	return CFrame::WndProc(hwnd, uMsg, wParam, lParam);
}

