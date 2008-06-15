////////////////////////////////////////////////////
// Mainfrm.cpp


#include "resource.h"
#include "mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

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

	// Set the name of the registry key
	m_KeyName = _T("Win32++\\Picture Sample");
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
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
	case IDM_FILE_EXIT:
		// End the application
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
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

	// A reference to the CToolbar object
	CToolbar& TB = GetToolbar();

	// Set the image lists for normal, hot and disabled buttons
	SetToolbarImages(TB, 8, RGB(192,192,192), IDB_TOOLBAR_NORM, IDB_TOOLBAR_HOT, IDB_TOOLBAR_DIS);

	// Disable some of the toolbar buttons
	TB.DisableButton(IDM_EDIT_CUT);
	TB.DisableButton(IDM_EDIT_COPY);
	TB.DisableButton(IDM_EDIT_PASTE);
	TB.DisableButton(IDM_FILE_PRINT);

	if (IsRebarUsed())
	{
		CRebar& RB = GetRebar();
		RB.ResizeBand(RB.GetBand(TB.GetHwnd()), TB.GetMaxSize());
	}
}

void CMainFrame::OnFileNew()
{
	if (m_View.m_pPicture)
		m_View.m_pPicture->Release();

	// Set a default title
	::SetWindowText(m_hWnd, LoadString(IDW_MAIN));

	m_View.m_pPicture = NULL;
	::InvalidateRect(m_View.GetHwnd(), NULL, TRUE);
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
		m_View.LoadPictureFile(szFile);
}

void CMainFrame::OnFileSave()
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

void CMainFrame::PreCreate(CREATESTRUCT &cs)
{
	cs.x = CW_USEDEFAULT;
	cs.y = CW_USEDEFAULT;
	cs.cx = 652;
	cs.cy = 595;

	CFrame::PreCreate(cs);
}

LRESULT CMainFrame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
		//Additional messages to be handled go here
//	}

	return WndProcDefault(hWnd, uMsg, wParam, lParam);
}

