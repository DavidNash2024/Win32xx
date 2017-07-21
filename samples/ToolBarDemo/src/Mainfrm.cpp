////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame() : m_UseBigIcons(FALSE)
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_View as the view window of the frame
	SetView(m_View);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\ToolBarDemo"));
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

LRESULT CMainFrame::OnBeginAdjust(LPNMTOOLBAR pNMTB)
// Called when the user has begun customizing a toolbar. Here we save
// a copy of the ToolBar layout so it can be restored when the user
// selects the reset button.
{
	CToolBar* pToolBar = static_cast<CToolBar*>(GetCWndPtr(pNMTB->hdr.hwndFrom));
	assert (dynamic_cast<CToolBar*> (pToolBar));
	
	int nResetCount = pToolBar->GetButtonCount();
	m_vTBBReset.clear();

	for (int i = 0; i < nResetCount; i++)
	{
		TBBUTTON tbb;
		pToolBar->GetButton(i, tbb);
		m_vTBBReset.push_back(tbb);
	}

    return TRUE;
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// OnCommand responds to menu and and toolbar input

	UNREFERENCED_PARAMETER(lParam);

	UINT nID = LOWORD(wParam);
	switch(nID)
	{
	case IDM_FILE_OPEN:			return OnFileOpen();
	case IDM_FILE_SAVE:			return OnFileSave();
	case IDM_FILE_SAVEAS:		return OnFileSave();
	case IDM_FILE_PRINT:		return OnFilePrint();
	case IDM_FILE_EXIT:			return OnFileExit();
	case IDW_VIEW_STATUSBAR:	return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:		return OnViewToolBar();
	case IDM_TOOLBAR_CUSTOMIZE:	return OnTBCustomize();
	case IDM_TOOLBAR_DEFAULT:	return OnTBDefault();
	case IDM_TOOLBAR_BIGICONS:	return OnTBBigIcons();
	case IDM_HELP_ABOUT:		return OnHelp();
	}

	return FALSE;
}

int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::OnCreate is optional.
	// Uncomment the lines below to change frame options.

	// SetUseIndicatorStatus(FALSE);	// Don't show keyboard indicators in the StatusBar
	// SetUseMenuStatus(FALSE);			// Don't show menu descriptions in the StatusBar
	// SetUseReBar(FALSE);				// Don't use a ReBar
	// SetUseThemes(FALSE);				// Don't use themes

	// call the base class function
	CFrame::OnCreate(cs);

	// Add the CCS_ADJUSTABLE style to the ToolBar
	DWORD dwStyle = GetToolBar().GetStyle();
	GetToolBar().SetStyle(CCS_ADJUSTABLE|dwStyle);

	// Untick the Large Icons menu item
	GetFrameMenu().CheckMenuItem(IDM_TOOLBAR_BIGICONS, MF_BYCOMMAND | MF_UNCHECKED);

	return 0;
}

LRESULT CMainFrame::OnCustHelp(LPNMHDR pNMHDR)
// Called when the help button on the customize dialog is pressed
{
	UNREFERENCED_PARAMETER(pNMHDR);
	MessageBox(_T("Help Button Pressed"), _T("Help"), MB_ICONINFORMATION | MB_OK);

	return 0L;
}

LRESULT CMainFrame::OnEndAdjust(LPNMHDR pNMHDR)
// Called when the user has stopped customizing a toolbar.
{
	UNREFERENCED_PARAMETER(pNMHDR);

	return TRUE;
}

BOOL CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
	return TRUE;
}

BOOL CMainFrame::OnFileOpen()
{
	CFileDialog FileDlg(TRUE);

	// Bring up the file open dialog retrieve the selected filename
	if (FileDlg.DoModal(*this) == IDOK)
	{
		// TODO:
		// Add your own code here. Refer to the tutorial for additional information
	}

	return TRUE;
}

BOOL CMainFrame::OnFileSave()
{
	CFileDialog FileDlg(FALSE);

	// Bring up the file save dialog retrieve the selected filename
	if (FileDlg.DoModal(*this) == IDOK)
	{
		// TODO:
		// Add your own code here. Refer to the tutorial for additional information
	}

	return TRUE;
}

BOOL CMainFrame::OnFilePrint()
{
	// Bring up a dialog to choose the printer
	CPrintDialog Printdlg;

	try
	{
		INT_PTR Res = Printdlg.DoModal(*this);

		// Retrieve the printer DC
		// CDC dcPrinter = Printdlg.GetPrinterDC();

		// TODO:
		// Add your own code here. Refer to the tutorial for additional information

		return (Res == IDOK);	// boolean expression
	}

	catch (const CWinException& /* e */)
	{
		// No default printer
		MessageBox(_T("Unable to display print dialog"), _T("Print Failed"), MB_OK);
		return FALSE;
	}
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.

	TRACE("Frame created\n");

	//Store the current ToolBar
	SaveTBDefault();
}

LRESULT CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam)
// Process notification messages sent by child windows
{
	LPNMTOOLBAR pNMTB = (LPNMTOOLBAR)lParam;
	
	switch(pNMTB->hdr.code)
	{
	case TBN_QUERYDELETE:	return OnQueryDelete(pNMTB);
	case TBN_QUERYINSERT:	return OnQueryInsert(pNMTB);
	case TBN_CUSTHELP:		return OnCustHelp((LPNMHDR)lParam);
	case TBN_GETBUTTONINFO: return OnGetButtonInfo(pNMTB);
	case TBN_BEGINADJUST:	return OnBeginAdjust(pNMTB);
	case TBN_ENDADJUST:		return OnEndAdjust((LPNMHDR)lParam);
	case TBN_TOOLBARCHANGE:	return OnToolBarChange(pNMTB);
	case TBN_RESET:			return OnReset(pNMTB); 
	}

	// Some notifications should return a value when handled
	return CFrame::OnNotify(wParam, lParam);
}

LRESULT CMainFrame::OnGetButtonInfo(LPNMTOOLBAR pNMTB)
// Called once for each button during toolbar customization to populate the list
// of available buttons. Return FALSE when all buttons have been added.
{
	// An array of TBBUTTON that contains all possible buttons
	TBBUTTON ButtonInfo[] =
	{
		{ 0, IDM_FILE_NEW,		TBSTATE_ENABLED, 0, {0}, 0, 0 },
		{ 1, IDM_FILE_OPEN,		TBSTATE_ENABLED, 0, {0}, 0, 0 },
		{ 2, IDM_FILE_SAVE,		TBSTATE_ENABLED, 0, {0}, 0, 0 },
		{ 3, IDM_EDIT_CUT,		0,               0, {0}, 0, 0 },
		{ 4, IDM_EDIT_COPY,		0,               0, {0}, 0, 0 },
		{ 5, IDM_EDIT_PASTE,	0,               0, {0}, 0, 0 },
		{ 6, IDM_FILE_PRINT,	TBSTATE_ENABLED, 0, {0}, 0, 0 },
		{ 7, IDM_HELP_ABOUT,	TBSTATE_ENABLED, 0, {0}, 0, 0 }
	};

	// An array of Button text strings (LPCTSTRs).
	// These are displayed in the customize dialog.
	LPCTSTR ButtonText[] = 
	{ 
		_T("New Document"), 
		_T("Open File"), 
		_T("Save File"), 
		_T("Cut"),
		_T("Copy"),
		_T("Paste"),
		_T("Print"),
		_T("Help About") 
	};

	// Pass the next button from the array. There is no need to filter out buttons
	// that are already used. They will be ignored.
	int nButtons = sizeof(ButtonInfo) / sizeof(TBBUTTON);
	
	if (pNMTB->iItem < nButtons)
	{
	    pNMTB->tbButton = ButtonInfo[pNMTB->iItem];
		lstrcpyn(pNMTB->pszText, ButtonText[pNMTB->iItem], pNMTB->cchText);
	    return TRUE;	// Load the next button.
	}

    return FALSE;	// No more buttons.
}

LRESULT CMainFrame::OnQueryDelete(LPNMTOOLBAR pNMTB)
// Called when a button may be deleted from a toolbar while the user is customizing the toolbar.
// Return TRUE to permit button deletion, and FALSE to prevent it.
{
	UNREFERENCED_PARAMETER(pNMTB);

	// Permit all buttons to be deleted
	return TRUE;
}

LRESULT CMainFrame::OnQueryInsert(LPNMTOOLBAR pNMTB)
// Called when a button may be inserted to the left of the specified button while the user 
//  is customizing a toolbar. Return TRUE to permit button deletion, and FALSE to prevent it.
{
	UNREFERENCED_PARAMETER(pNMTB);

	// Permit all buttons to be inserted
	return TRUE;
}

LRESULT CMainFrame::OnReset(LPNMTOOLBAR pNMTB)
// Called when the user presses the Reset button on teh ToolBar customize dialog.
// Here we restore the Toolbar to the settings saved in OnBeginAdjust.
{
	CToolBar* pToolBar = static_cast<CToolBar*>(GetCWndPtr(pNMTB->hdr.hwndFrom));
	assert (dynamic_cast<CToolBar*> (pToolBar));

	// Remove all current buttons
	int nCount = pToolBar->GetButtonCount();
	for (int i = nCount - 1; i >= 0; i--)
	{
		pToolBar->DeleteButton(i);
	}
	
	// Restore buttons from info stored in m_vTBBReset
	int nResetCount = m_vTBBReset.size();
	for (int j = 0; j < nResetCount; j++)
	{
		TBBUTTON tbb = m_vTBBReset[j];
		pToolBar->InsertButton(j, tbb);
	}

	RecalcLayout();

    return TRUE;
}

LRESULT CMainFrame::OnToolBarChange(LPNMTOOLBAR pNMTB)
// Called when the toolbar has been changed during customization.
{
	UNREFERENCED_PARAMETER(pNMTB);

	// Reposition the toolbar
	RecalcLayout();

	return TRUE;
}

BOOL CMainFrame::OnTBBigIcons()
// Toggle the Image size for the ToolBar by changing Image Lists.
{
	m_UseBigIcons = !m_UseBigIcons;

	GetFrameMenu().CheckMenuItem(IDM_TOOLBAR_BIGICONS, MF_BYCOMMAND | (m_UseBigIcons ? MF_CHECKED : MF_UNCHECKED));

	if (m_UseBigIcons)
	{
		// Set Large Images. 3 Imagelists - Normal, Hot and Disabled
		SetToolBarImages(RGB(192,192,192), IDB_NORMAL, IDB_HOT, IDB_DISABLED);
	}
	else
	{
		// Set Small icons
		SetToolBarImages(RGB(192,192,192), IDW_MAIN, 0, 0);
	}

	RecalcLayout();
	GetToolBar().Invalidate();
	return TRUE;
}

BOOL CMainFrame::OnTBCustomize()
{
	// Customize CFrame's Toolbar
	GetToolBar().Customize();
	return TRUE;
}

BOOL CMainFrame::OnTBDefault()
// Set the Toolbar back to its intial settings.
{
	// Remove all current buttons
	int nCount = GetToolBar().GetButtonCount();
	for (int i = nCount - 1; i >= 0; i--)
	{
		GetToolBar().DeleteButton(i);
	}
	
	// Restore buttons from info stored in m_vTBBDefault
	int nDefaultCount = m_vTBBDefault.size();
	for (int j = 0; j < nDefaultCount; j++)
	{
		TBBUTTON tbb = m_vTBBDefault[j];
		GetToolBar().InsertButton(j, tbb);
	}

	RecalcLayout();
	return TRUE;
}

void CMainFrame::SaveTBDefault()
// Saves the initial Toolbar configuration in a vector of TBBUTTON
{
	int nCount = GetToolBar().GetButtonCount();

	for (int i = 0; i < nCount; i++)
	{
		TBBUTTON tbb;
		GetToolBar().GetButton(i, tbb);
		m_vTBBDefault.push_back(tbb);
	}
}

void CMainFrame::SetupToolBar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolBarButton( IDM_FILE_NEW   );
	AddToolBarButton( IDM_FILE_OPEN  );
	AddToolBarButton( IDM_FILE_SAVE  );
	
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_EDIT_CUT,   FALSE );	// disabled button
	AddToolBarButton( IDM_EDIT_COPY,  FALSE );	// disabled button
	AddToolBarButton( IDM_EDIT_PASTE, FALSE );	// disabled button
	
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_FILE_PRINT );
	
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_HELP_ABOUT );
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//		Add case statements for each messages to be handled here
//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

