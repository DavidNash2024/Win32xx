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

void CMainFrame::ModifyBitmap(int cRed, int cGreen, int cBlue, BOOL bGray)
{
	TintBitmap(GetMyView().GetImage(), cRed, cGreen, cBlue);
	if (bGray) 	GrayScaleBitmap(GetMyView().GetImage());
	
	GetMyView().RedrawWindow(0, 0, RDW_NOERASE|RDW_INVALIDATE|RDW_UPDATENOW);
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// OnCommand responds to menu and and toolbar input

	switch(LOWORD(wParam))
	{
	case IDM_FILE_NEW:
		OnFileNew();
		return TRUE;
	case IDM_FILE_OPEN:
		OnFileOpen();
		return TRUE;
	case IDM_FILE_SAVE:
		OnFileSave();
		return TRUE;
	case IDM_FILE_SAVEAS:
		OnFileSaveAs();
		return TRUE;
	case IDM_FILE_EXIT:
		// End the application
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	case IDM_IMAGE_ADJUST:
		OnAdjustImage();
		return TRUE;
	case IDW_VIEW_STATUSBAR:
		OnViewStatusbar();
		return TRUE;
	case IDW_VIEW_TOOLBAR:
		OnViewToolbar();
		return TRUE;
	case IDM_HELP_ABOUT:
		// Display the help dialog
		OnHelp();
		return TRUE;
	case IDW_FILE_MRU_FILE1:
	case IDW_FILE_MRU_FILE2:
	case IDW_FILE_MRU_FILE3:
	case IDW_FILE_MRU_FILE4:
		OnFileOpenMRU(wParam, lParam);
		return TRUE;
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
	// m_bUseToolbar = FALSE;			// Don't use a toolbar

	// call the base class function
	CFrame::OnCreate();
}

void CMainFrame::OnFileNew()
{
	CToolbar& TB = GetToolbar();
	TB.DisableButton(IDM_FILE_SAVEAS);
	TB.DisableButton(IDM_IMAGE_ADJUST);
	m_MyView.FileOpen(NULL);
	m_MyView.Invalidate();

	// Set the caption
	tString ts = _T("FastGDI"); 
	SetWindowText(ts.c_str());
}

void CMainFrame::OnFileOpen()
{
	// Fill the OPENFILENAME structure
	TCHAR szFilters[] = _T("Scribble Files (*.bmp)\0*.bmp\0\0");
	TCHAR szFilePathName[_MAX_PATH] = _T("");
	OPENFILENAME ofn = {0};
	ofn.lStructSize = sizeof(OPENFILENAME);
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

	// Turn on the Toolbar adjust button
	CToolbar& TB = GetToolbar();
	TB.EnableButton(IDM_FILE_SAVEAS);
	TB.EnableButton(IDM_IMAGE_ADJUST);
	EnableMenuItem(GetFrameMenu(), IDM_IMAGE_ADJUST, MF_BYCOMMAND | MF_ENABLED);

	// Resize the frame to match the bitmap
	if (GetMyView().GetImage())
	{
		GetMyView().ShowScrollBar(SB_HORZ, FALSE);
		GetMyView().ShowScrollBar(SB_VERT, FALSE);
		CRect rcImage = GetMyView().GetImageRect();
		AdjustFrameRect(rcImage);
	}
		
	GetMyView().RedrawWindow(0, 0, RDW_NOERASE|RDW_INVALIDATE|RDW_UPDATENOW);

	// Set the caption
	tString ts = _T("FastGDI - ") + m_PathName; 
	SetWindowText(ts.c_str());
}

BOOL CMainFrame::OnFileOpenMRU(WPARAM wParam, LPARAM /*lParam*/)
{
	UINT nMRUIndex = LOWORD(wParam) - IDW_FILE_MRU_FILE1;
	tString tsMRUText = GetMRUEntry(nMRUIndex);
	CToolbar& TB = GetToolbar();

	if (m_MyView.FileOpen(tsMRUText.c_str()))
	{
		m_PathName = tsMRUText;
		TB.EnableButton(IDM_FILE_SAVEAS);
		TB.EnableButton(IDM_IMAGE_ADJUST);

		// Adjust the window size
		CRect rcImage = GetMyView().GetImageRect();
		AdjustFrameRect(rcImage);
	}
	else
	{
		RemoveMRUEntry(tsMRUText.c_str());
		TB.DisableButton(IDM_FILE_SAVEAS);
		TB.DisableButton(IDM_IMAGE_ADJUST);
	}

	// Resize the frame to match the bitmap
	if (GetMyView().GetImage())
	{
		GetMyView().ShowScrollBar(SB_HORZ, FALSE);
		GetMyView().ShowScrollBar(SB_VERT, FALSE);
		CRect rcImage = GetMyView().GetImageRect();
		AdjustFrameRect(rcImage);
	}

	GetMyView().RedrawWindow(0, 0, RDW_NOERASE|RDW_INVALIDATE|RDW_UPDATENOW);

	// Set the caption
	tString ts = _T("FastGDI - ") + m_PathName; 
	SetWindowText(ts.c_str());
	return TRUE;
}

void CMainFrame::OnFileSave()
{
	if (!m_PathName.empty())
	{
		tString ts = m_PathName + _T("  already exists.\nDo you want to replace it?");

		if (IDYES == MessageBox(ts.c_str(), _T("FileSaveAs"), MB_YESNO | MB_ICONWARNING))
			m_MyView.FileSave(m_PathName.c_str());
	}
}

void CMainFrame::OnFileSaveAs()
{
	// Fill the OPENFILENAME structure
	TCHAR szFilters[] = _T("Bitmap Files (*.bmp)\0*.bmp\0\0");
	TCHAR szFilePathName[_MAX_PATH] = _T("");
	OPENFILENAME ofn = {0};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = szFilters;
	ofn.lpstrFile = szFilePathName;
	ofn.lpstrDefExt = _T("bmp");
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrTitle = _T("SaveAs File");
	ofn.Flags = OFN_OVERWRITEPROMPT;

	// Open the file save dialog, and open the file
	if (!::GetSaveFileName(&ofn))
		return;

	// Set the caption
	m_PathName = szFilePathName;
	tString ts = _T("FastGDI - ") + m_PathName; 
	SetWindowText(ts.c_str());

	// Save the file name
	m_MyView.FileSave(szFilePathName);
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.

	TRACE(_T("Frame created\n"));
}

inline void CMainFrame::OnMenuUpdate(UINT nID)
// Called when menu items are about to be displayed
{
	BOOL IsImageLoaded = (BOOL)GetMyView().GetImage();

	switch(nID)
	{
	case IDM_FILE_SAVE:
		EnableMenuItem(GetFrameMenu(), IDM_FILE_SAVE, IsImageLoaded? MF_ENABLED : MF_GRAYED);
		break;
	case IDM_FILE_SAVEAS:
		EnableMenuItem(GetFrameMenu(), IDM_FILE_SAVEAS, IsImageLoaded? MF_ENABLED : MF_GRAYED);
		break;
	case IDM_IMAGE_ADJUST:
		EnableMenuItem(GetFrameMenu(), IDM_IMAGE_ADJUST, IsImageLoaded? MF_ENABLED : MF_GRAYED);
		break;
	}
}

void CMainFrame::SetupToolbar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolbarButton( IDM_FILE_NEW  );
	AddToolbarButton( IDM_FILE_OPEN );
	AddToolbarButton( IDM_FILE_SAVEAS, FALSE );
	
	AddToolbarButton( 0 );	// Separator
	AddToolbarButton( IDM_IMAGE_ADJUST, FALSE );
	
	AddToolbarButton( 0 );	// Separator
	AddToolbarButton( IDM_HELP_ABOUT );

	// Use large toolbar buttons
	SetToolbarImages(RGB(192, 192, 192), IDB_TOOLBAR_BIG, 0, 0);
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

