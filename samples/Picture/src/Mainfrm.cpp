////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "resource.h"
#include "PictureApp.h"
#include "Mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_View as the view window of the frame
	SetView(m_View);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\Picture Sample"));

	// Load the settings from the registry with 4 MRU entries
	LoadRegistryMRUSettings(4);
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
	case IDM_FILE_NEW:			OnFileNew();		return TRUE;
	case IDM_FILE_OPEN:			OnFileOpen();		return TRUE;
	case IDM_FILE_SAVE:			OnFileSaveAs();		return TRUE;
	case IDM_FILE_SAVEAS:		OnFileSaveAs();		return TRUE;
	case IDM_FILE_EXIT:			OnFileExit();		return TRUE;
	case IDW_VIEW_STATUSBAR:	OnViewStatusBar();	return TRUE;
	case IDW_VIEW_TOOLBAR:		OnViewToolBar();	return TRUE;
	case IDM_HELP_ABOUT:		OnHelp();			return TRUE;

	case IDW_FILE_MRU_FILE1:
	case IDW_FILE_MRU_FILE2:
	case IDW_FILE_MRU_FILE3:
	case IDW_FILE_MRU_FILE4:
	case IDW_FILE_MRU_FILE5:	OnFileMRU(wParam);	return TRUE;
	}

	return FALSE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT pcs)
{
	return CFrame::OnCreate(pcs);
}

void CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
}

void CMainFrame::OnFileMRU(WPARAM wParam)
{
	UINT nMRUIndex = LOWORD(wParam) - IDW_FILE_MRU_FILE1;
	CString strMRUText = GetMRUEntry(nMRUIndex);

	if (!m_View.LoadPictureFile(strMRUText))
	{
		RemoveMRUEntry(strMRUText);
		m_View.NewPictureFile();
		MessageBox(_T("Failed to load File"), _T("Error"), MB_OK);
	}
}

void CMainFrame::OnFileNew()
{
	m_View.NewPictureFile();
}

void CMainFrame::OnFileOpen()
{
	TCHAR szFilter[] = _T("Supported Files Types(*.bmp;*.gif;*.jpg;*.ico;*.emf;*.wmf)\0*.bmp;*.gif;*.jpg;*.ico;*.emf;*.wmf\0Bitmaps (*.bmp)\0*.bmp\0GIF Files (*.gif)\0*.gif\0JPEG Files (*.jpg)\0*.jpg\0Icons (*.ico)\0*.ico\0Enhanced Metafiles (*.emf)\0*.emf\0Windows Metafiles (*.wmf)\0*.wmf\0\0");

	CFile File;
	CString str = File.OpenFileDialog(0, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, _T("Open File"), szFilter, this);
	if (!str.IsEmpty())
	{
		m_View.LoadPictureFile(str);
	}
}

LRESULT CMainFrame::OnFileLoaded(LPCTSTR szFile)
{
	SetWindowText(szFile);
	AdjustFrameRect(m_View.GetImageRect());
	AddMRUEntry(szFile);
	return 0L;
}

void CMainFrame::OnFileSaveAs()
{
	if (m_View.GetPicture())
	{
		SHORT Type;
		m_View.GetPicture()->get_Type(&Type);
        CString Filter;
        CString Ext;

		// Note: iPicture doesn't convert between file types
		switch(Type)
		{
		case PICTYPE_BITMAP:
			Filter = _T("Supported Files Type(*.bmp)\0*.bmp;\0Bitmap (*.bmp)\0*.bmp\0\0");
			Ext = _T("bmp");
			break;
		case PICTYPE_METAFILE:
			Filter = _T("Supported Files Type(*.wmf)\0*.bmp;\0Metafile (*.wmf)\0*.wmf\0\0");
			Ext = _T("wmf");
			break;
		case PICTYPE_ICON:
			Filter = _T("Supported Files Type(*.ico)\0*.ico;\0Icon File (*.ico)\0*.ico\0\0");
			Ext = _T("ico");
			break;
		case PICTYPE_ENHMETAFILE:
			Filter = _T("Supported Files Type(*.emf)\0*.emf;\0Enhanced Metafile (*.emf)\0*.emf\0\0");
			Ext = _T("emf");
			break;
		}

		CFile File;
		CString str = File.SaveFileDialog(0, OFN_SHOWHELP | OFN_OVERWRITEPROMPT, _T("Save File"), Filter, Ext, this);

		if (!str.IsEmpty())
		{
			m_View.SavePicture(str);
			AddMRUEntry(str);
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
	switch (uMsg)
	{
	// A message defined in PictureApp.h
	case UWM_FILELOADED: return OnFileLoaded((LPCTSTR) lParam);
	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

