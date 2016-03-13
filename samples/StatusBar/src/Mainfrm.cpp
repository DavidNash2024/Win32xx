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
	LoadRegistrySettings(_T("Win32++\\StatusBar Demo"));
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
	case IDM_FILE_OPEN:		 return OnFileOpen();
	case IDM_FILE_SAVE:		 return OnFileSave();
	case IDM_FILE_SAVEAS:	 return OnFileSave();
	case IDM_FILE_PRINT:	 return OnFilePrint();
	case IDM_FILE_EXIT:		 return OnFileExit();
	case IDW_VIEW_STATUSBAR: return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:	 return OnViewToolBar();
	case IDM_HELP_ABOUT:	 return OnHelp();
	}

	return FALSE;
}

int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// Uncomment the lines below to change frame options.

	SetUseIndicatorStatus(FALSE);	// Don't show keyboard indicators in the StatusBar
	SetUseMenuStatus(FALSE);			// Don't show menu descriptions in the StatusBar
	// SetUseReBar(FALSE);				// Don't use a ReBar
	// SetUseThemes(FALSE);				// Don't use themes
	// SetUseToolBar(FALSE);			// Don't use a ToolBar

	// call the base class function
	CFrame::OnCreate(cs);

	GetStatusBar().SetWindowLongPtr(GWLP_ID, STATUS_ID);

	return 0;
}

LRESULT CMainFrame::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(uMsg);

	// Perform the owner draw for Part 3 in the status bar
	LPDRAWITEMSTRUCT pDrawItem = (LPDRAWITEMSTRUCT) lParam;
	
	if (pDrawItem->CtlID == STATUS_ID) // Message comes from the status bar
	{
		CRect rcPart = pDrawItem->rcItem;
		CDC dc;
		dc.Attach(pDrawItem->hDC);
	
		// Set the font to italic
		CFont Font = GetStatusBar().GetFont();
		LOGFONT lf = Font.GetLogFont();
		lf.lfItalic = TRUE;
		dc.CreateFontIndirect(&lf);
		
		// Display the gradient background and text
		dc.GradientFill(RGB(230, 180, 0), RGB(240, 210, 90), rcPart, TRUE);
		dc.SetTextColor(RGB(10,20,250));
		dc.SetBkMode(TRANSPARENT);
		dc.TextOut(rcPart.left,rcPart.top,_T("Owner Draw"), 10);

		return TRUE;
	}

	// Allow the frame to perform owner drawing menu items.
	return CFrame::OnDrawItem(uMsg, wParam, lParam);
}

BOOL CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);

	return TRUE;
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.

	TRACE("Frame created\n");
}

BOOL CMainFrame::OnFileOpen()
{
	// Bring up the dialog, and open the file
	CFile File;
	CString str = File.OpenFileDialog(0, 0, 0, 0);

	// TODO:
	// Add your own code here. Refer to the tutorial for additional information

	return TRUE;
}

BOOL CMainFrame::OnFileSave()
{
	CFile File;
	CString str = File.SaveFileDialog(0, 0, 0, 0, 0);

	// TODO:
	// Add your own code here. Refer to the tutorial for additional information

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

	catch (const CResourceException& e)
	{
		// No default printer
		MessageBox(_T("Unable to display print dialog"), _T("Error"), MB_OK);
		return FALSE;
	}
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

void CMainFrame::PreCreate(CREATESTRUCT& cs)
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

