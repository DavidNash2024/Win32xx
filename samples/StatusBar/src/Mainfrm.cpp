////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "resource.h"
#include "mainfrm.h"

#define STATUS_ID 1211

// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_View as the view window of the frame
	SetView(m_View);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\Frame"));
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// OnCommand responds to menu and and toolbar input

	UNREFERENCED_PARAMETER(lParam);

	switch(LOWORD(wParam))
	{
	case IDM_FILE_OPEN:		 OnFileOpen();		return TRUE;
	case IDM_FILE_SAVE:		 OnFileSave();		return TRUE;
	case IDM_FILE_SAVEAS:	 OnFileSave();		return TRUE;
	case IDM_FILE_PRINT:	 OnFilePrint();		return TRUE;
	case IDM_FILE_EXIT:		 OnFileExit();		return TRUE;
	case IDW_VIEW_STATUSBAR: OnViewStatusBar();	return TRUE;
	case IDW_VIEW_TOOLBAR:	 OnViewToolBar();	return TRUE;
	case IDM_HELP_ABOUT:	 OnHelp();			return TRUE;
	}

	return FALSE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT pcs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// The default for the following variables is TRUE

	 m_UseIndicatorStatus = FALSE;		// Don't show keyboard indicators in the StatusBar
	 m_UseMenuStatus = FALSE;			// Don't show menu descriptions in the StatusBar
	// m_UseReBar = FALSE;				// Don't use a ReBar
	// m_UseThemes = FALSE;				// Don't use themes
	// m_UseToolBar = FALSE;			// Don't use a ToolBar

	// call the base class function
	CFrame::OnCreate(pcs);

	GetStatusBar().SetWindowLongPtr(GWLP_ID, STATUS_ID);

	return 0;
}

LRESULT CMainFrame::OnDrawItem(WPARAM wParam, LPARAM lParam)
{
	// Perform the owner draw for Part 3 in the status bar
	LPDRAWITEMSTRUCT pDrawItem = (LPDRAWITEMSTRUCT) lParam;
	
	if (pDrawItem->CtlID == STATUS_ID) // Message comes from the status bar
	{
		CRect rcPart = pDrawItem->rcItem;
		CDC* pDC = CDC::FromHandle(pDrawItem->hDC);
	
		// Set the font to italic
		CFont* pFont = GetStatusBar().GetFont();
		LOGFONT lf = pFont->GetLogFont();
		lf.lfItalic = TRUE;
		pDC->CreateFontIndirect(&lf);
		
		// Display the gradient background and text
		pDC->GradientFill(RGB(230, 180, 0), RGB(240, 210, 90), rcPart, TRUE);
		pDC->SetTextColor(RGB(10,20,250));
		pDC->SetBkMode(TRANSPARENT);
		pDC->TextOut(rcPart.left,rcPart.top,_T("Owner Draw"), 10);
	}

	// Allow the frame to perform owner drawing menu items.
	return CFrame::OnDrawItem(wParam, lParam);
}

void CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.

	TRACE("Frame created\n");
}

void CMainFrame::OnFileOpen()
{
	// Bring up the dialog, and open the file
	CFile File;
	CString str = File.OpenFileDialog(0, 0, 0, 0);

	// TODO:
	// Add your own code here. Refer to the tutorial for additional information 
}

void CMainFrame::OnFileSave()
{
	CFile File;
	CString str = File.SaveFileDialog(0, 0, 0, 0, 0);

	// TODO:
	// Add your own code here. Refer to the tutorial for additional information 
}

void CMainFrame::OnFilePrint()
{
	// Bring up a dialog to choose the printer
	PRINTDLG pd;
	ZeroMemory(&pd, sizeof(PRINTDLG));
	pd.lStructSize = sizeof( pd );
	pd.Flags = PD_RETURNDC;
	pd.hwndOwner = *this;

	// Retrieve the printer DC
	PrintDlg( &pd );
	
	// TODO:
	// Add your own code here. Refer to the tutorial for additional information 
}

LRESULT CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam)
{
	// Process notification messages sent by child windows
//	switch(((LPNMHDR)lParam)->code)
//	{
//		Add case statements for each notification message here
//	}

	// Some notifications should return a value when handled
	return CFrame::OnNotify(wParam, lParam);
}

void CMainFrame::PreCreate(CREATESTRUCT &cs)
{
	// This function is called before the frame is created.
	// It provides an opportunity to modify the various CREATESTRUCT
	// parameters used in the frame window's creation.
	
	// Call base clase to set defaults
	CFrame::PreCreate(cs);
	
	// cs.style &= ~WS_VISIBLE;	// Remove the WS_VISIBLE style. The frame will be initially hidden.
}

void CMainFrame::SetupToolBar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolBarButton( IDM_FILE_NEW   );
	AddToolBarButton( IDM_FILE_OPEN  );
	AddToolBarButton( IDM_FILE_SAVE  );
	
	AddToolBarButton( 0 );						// Separator
	AddToolBarButton( IDM_EDIT_CUT,   FALSE );	// disabled button
	AddToolBarButton( IDM_EDIT_COPY,  FALSE );	// disabled button
	AddToolBarButton( IDM_EDIT_PASTE, FALSE );	// disabled button
	
	AddToolBarButton( 0 );						// Separator
	AddToolBarButton( IDM_FILE_PRINT );
	
	AddToolBarButton( 0 );						// Separator
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

