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
	// The default for the following variables is TRUE

	// m_bShowIndicatorStatus = FALSE;	// Statusbar shows indicators
	// m_bShowMenuStatus = FALSE;		// Statusbar shows Toolbar and menu mouse position
	// m_bUseRebar = FALSE;				// Rebars are used to house toolbar and menubar

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
	// Here we override CFrame::Setbuttons to customise the Toolbar

	// A reference to the CToolbar object
	CToolbar& TB = GetToolbar();

	// Set the button size to 24x24 before adding the bitmap
	TB.SetBitmapSize(24, 24);	

	// Set the image lists for normal, hot and disabled buttons
	TB.SetImageList(8, RGB(255,0,255), IDB_TOOLBAR_NORM, IDB_TOOLBAR_HOT, IDB_TOOLBAR_DIS);

	// Set the resource IDs for the toolbar buttons
	TB.SetButtons(ToolbarData);

	// Add some text to the buttons
	TB.SetButtonText(TB.CommandToIndex(IDM_FILE_NEW),   "New");
	TB.SetButtonText(TB.CommandToIndex(IDM_FILE_OPEN),  "Open");
	TB.SetButtonText(TB.CommandToIndex(IDM_FILE_SAVE),  "Save");
	TB.SetButtonText(TB.CommandToIndex(IDM_EDIT_CUT),   "Cut");
	TB.SetButtonText(TB.CommandToIndex(IDM_EDIT_COPY),  "Copy");
	TB.SetButtonText(TB.CommandToIndex(IDM_EDIT_PASTE), "Paste");
	TB.SetButtonText(TB.CommandToIndex(IDM_FILE_PRINT), "Print"); 
	TB.SetButtonText(TB.CommandToIndex(IDM_HELP_ABOUT), "About");

	// Adjust the toolbar button sizes to take account of the new text
	RECT r;
	TB.GetItemRect(0, &r);
	TB.SetButtonSize(r.right - r.left, r.bottom - r.top);
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

