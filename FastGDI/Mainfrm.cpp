////////////////////////////////////////////////////
// Mainfrm.cpp


#include "resource.h"
#include "mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_MyView as the view window of the frame
	SetView(m_MyView);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\Fast GDI Demo"));

	// Load the settings from the registry with 4 MRU entries
	LoadRegistryMRUSettings(4);
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

void CMainFrame::OnAdjustImage()
{
	if (GetMyView().GetImage())
	{
		// Initiate the Colour Adjust Dialog
		CColourDialog Dialog(IDD_DIALOG1);
		Dialog.DoModal();
	}
	else
		MessageBox(_T("Open a Bitmap file first!"), _T("Error"), MB_OK);
}

void CMainFrame::ModifyBitmap(int cRed, int cGreen, int cBlue)
{
	TintBitmap(GetMyView().GetImage(), cRed, cGreen, cBlue);
	GetMyView().Invalidate();
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	// OnCommand responds to menu and and toolbar input

	switch(LOWORD(wParam))
	{
	case IDM_FILE_NEW:
		{
			CToolbar& TB = GetToolbar();
			TB.DisableButton(IDM_IMAGE_ADJUST);
			EnableMenuItem(GetFrameMenu(), IDM_IMAGE_ADJUST, MF_BYCOMMAND | MF_GRAYED);
			m_MyView.FileOpen(NULL);
		}
		return TRUE;
	case IDM_FILE_OPEN:
		OnFileOpen();
		return TRUE;
	case IDM_FILE_EXIT:
		// End the application
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	case IDM_IMAGE_ADJUST:
		OnAdjustImage();
		return TRUE;
	case IDM_HELP_ABOUT:
		// Display the help dialog
		OnHelp();
		return TRUE;
	case IDW_FILE_MRU_FILE1:
	case IDW_FILE_MRU_FILE2:
	case IDW_FILE_MRU_FILE3:
	case IDW_FILE_MRU_FILE4:
		{
			UINT nMRUIndex = LOWORD(wParam) - IDW_FILE_MRU_FILE1;
			tString tsMRUText = GetMRUEntry(nMRUIndex);
			CToolbar& TB = GetToolbar();

			if (m_MyView.FileOpen(tsMRUText.c_str()))
			{
				m_PathName = tsMRUText;
				TB.EnableButton(IDM_IMAGE_ADJUST);
				EnableMenuItem(GetFrameMenu(), IDM_IMAGE_ADJUST, MF_BYCOMMAND | MF_ENABLED);

				// Adjust the window size
				CRect rcImage = GetMyView().GetImageSize();
				AdjustFrameRect(rcImage);
			}
			else
			{
				RemoveMRUEntry(tsMRUText.c_str());
				TB.DisableButton(IDM_IMAGE_ADJUST);
				EnableMenuItem(GetFrameMenu(), IDM_IMAGE_ADJUST, MF_BYCOMMAND | MF_GRAYED);
			}

			return TRUE;
		}
	}

	return FALSE;
}

void CMainFrame::OnCreate()
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// The default for the following variables is TRUE

	// m_bShowIndicatorStatus = FALSE;	// Don't show statusbar indicators
	// m_bShowMenuStatus = FALSE;		// Don't show toolbar or menu status
	// m_bUseRebar = FALSE;				// Don't use rebars
	// m_bUseThemes = FALSE;            // Don't use themes

	// call the base class function
	CFrame::OnCreate();

	// Disable a the Save toolbar button
	CToolbar& TB = GetToolbar();
	TB.DisableButton(IDM_FILE_SAVE);
	TB.DisableButton(IDM_IMAGE_ADJUST);
	EnableMenuItem(GetFrameMenu(), IDM_IMAGE_ADJUST, MF_BYCOMMAND | MF_GRAYED);
	SetMenuIcons(TB.GetToolbarData(), RGB(192, 192, 192), IDB_TOOLBAR_SML, 0);
}

void CMainFrame::OnFileOpen()
{
	// Fill the OPENFILENAME structure
	TCHAR szFilters[] = _T("Scribble Files (*.bmp)\0*.bmp\0\0");
	TCHAR szFilePathName[_MAX_PATH] = _T("");
	OPENFILENAME ofn = {0};
	if (GetWinVersion() >= 2500)
	{
		// For Win2000 and above
		ofn.lStructSize = sizeof(OPENFILENAME);
	}
	else
	{
		// For older operating systems
		ofn.lStructSize = OPENFILENAME_SIZE_VERSION_400;
	}

	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = szFilters;
	ofn.lpstrFile = szFilePathName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrTitle = _T("Open File");
	ofn.Flags = OFN_FILEMUSTEXIST;

	// Bring up the dialog, and open the file
	if (!::GetOpenFileName(&ofn))
		return;

	// Load the bitmap
	m_MyView.FileOpen(szFilePathName);

	// Save the filename
	m_PathName = szFilePathName;
	AddMRUEntry(szFilePathName);

	CToolbar& TB = GetToolbar();
	TB.EnableButton(IDM_IMAGE_ADJUST);
	EnableMenuItem(GetFrameMenu(), IDM_IMAGE_ADJUST, MF_BYCOMMAND | MF_ENABLED);

	if (GetMyView().GetImage())
	{
		CRect rcImage = GetMyView().GetImageSize();
		AdjustFrameRect(rcImage);
	}
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.

	TRACE(_T("Frame created\n"));
}

void CMainFrame::LoadToolbar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolbarButton( IDM_FILE_NEW   );
	AddToolbarButton( IDM_FILE_OPEN  );
	AddToolbarButton( IDM_FILE_SAVE  );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_IMAGE_ADJUST);
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_HELP_ABOUT );
}

LRESULT CMainFrame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
		//Additional messages to be handled go here
//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

