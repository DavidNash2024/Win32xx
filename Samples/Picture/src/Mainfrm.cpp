////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "resource.h"
#include "mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_View as the view window of the frame
	SetView(m_View);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\Picture Sample"));
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
	case IDM_FILE_NEW:
		OnFileNew();
		return TRUE;
	case IDM_FILE_OPEN:
		OnFileOpen();
		return TRUE;
	case IDM_FILE_SAVE:
		OnFileSaveAs();
		return TRUE;
	case IDM_FILE_SAVEAS:
		OnFileSaveAs();
		return TRUE;
	case IDM_FILE_EXIT:
		// End the application
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	case IDW_VIEW_STATUSBAR:
		OnViewStatusBar();
		return TRUE;
	case IDW_VIEW_TOOLBAR:
		OnViewToolBar();
		return TRUE;
	case IDM_HELP_ABOUT:
		// Display the help dialog
		OnHelp();
		return TRUE;
	}

	return FALSE;
}

void CMainFrame::OnCreate()
{
	CFrame::OnCreate();
}

void CMainFrame::OnFileNew()
{
	m_View.NewPictureFile();
}

void CMainFrame::OnFileOpen()
{
	// get file name to open
	TCHAR szFile[MAX_PATH] = _T("");
	OPENFILENAME ofn = {0};
	ofn.lStructSize	= sizeof(OPENFILENAME);
	ofn.Flags		= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrFilter	= _T("Supported Files Types(*.bmp;*.gif;*.jpg;*.ico;*.emf;*.wmf)\0*.bmp;*.gif;*.jpg;*.ico;*.emf;*.wmf\0Bitmaps (*.bmp)\0*.bmp\0GIF Files (*.gif)\0*.gif\0JPEG Files (*.jpg)\0*.jpg\0Icons (*.ico)\0*.ico\0Enhanced Metafiles (*.emf)\0*.emf\0Windows Metafiles (*.wmf)\0*.wmf\0\0");
	ofn.lpstrTitle	= _T("Open Picture File");
	ofn.lpstrFile	= szFile;
	ofn.nMaxFile	= MAX_PATH;

	if (IDOK == ::GetOpenFileName(&ofn))	// OK button hit in file open dialog
	{
		m_View.LoadPictureFile(szFile);
		CRect rcImage = m_View.GetImageRect();
		AdjustFrameRect(rcImage);
	}
}

void CMainFrame::OnFileSaveAs()
{
	if (m_View.GetPicture())
	{
		TCHAR szFile[MAX_STRING_SIZE];
		szFile[0] = '\0';

		OPENFILENAME Ofn = {0};
		::GetSaveFileName(&Ofn);
		Ofn.lStructSize = sizeof(OPENFILENAME);
		Ofn.lpstrFilter = _T("*.bmp\0");
		Ofn.lpstrFile= szFile;
		Ofn.nMaxFile = MAX_STRING_SIZE;
		Ofn.lpstrDefExt = _T("bmp");
		Ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;

		if (GetSaveFileName(&Ofn))
		{
			m_View.SavePicture(szFile);
		}
	}
}

void CMainFrame::SetupToolBar()
{	
	// Set the Resource IDs for the toolbar buttons
	AddToolBarButton( IDM_FILE_NEW   );
	AddToolBarButton( IDM_FILE_OPEN  );
	AddToolBarButton( IDM_FILE_SAVE  );
	
	AddToolBarButton( 0 );	// Separator
	AddToolBarButton( IDM_EDIT_CUT,   FALSE );
	AddToolBarButton( IDM_EDIT_COPY,  FALSE );
	AddToolBarButton( IDM_EDIT_PASTE, FALSE );

	AddToolBarButton( 0 );	// Separator
	AddToolBarButton( IDM_FILE_PRINT, FALSE );
	
	AddToolBarButton( 0 );	// Separator
	AddToolBarButton( IDM_HELP_ABOUT );

	// Set the image lists for normal, hot and disabled buttons
	SetToolBarImages(RGB(192,192,192), IDB_TOOLBAR_NORM, IDB_TOOLBAR_HOT, IDB_TOOLBAR_DIS);
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
		// Add case statements for each messages to be handled here
//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

