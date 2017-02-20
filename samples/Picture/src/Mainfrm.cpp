////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "Mainfrm.h"
#include "PictureApp.h"
#include "resource.h"


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

	UINT nID = LOWORD(wParam);
	switch(nID)
	{
	case IDM_FILE_NEW:			return OnFileNew();
	case IDM_FILE_OPEN:			return OnFileOpen();
	case IDM_FILE_SAVE:			return OnFileSaveAs();
	case IDM_FILE_SAVEAS:		return OnFileSaveAs();
	case IDM_FILE_EXIT:			return OnFileExit();
	case IDW_VIEW_STATUSBAR:	return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:		return OnViewToolBar();
	case IDM_HELP_ABOUT:		return OnHelp();

	case IDW_FILE_MRU_FILE1:
	case IDW_FILE_MRU_FILE2:
	case IDW_FILE_MRU_FILE3:
	case IDW_FILE_MRU_FILE4:
	case IDW_FILE_MRU_FILE5:	return OnFileMRU(wParam);
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
	// SetUseToolBar(FALSE);			// Don't use a ToolBar
	
	return CFrame::OnCreate(cs);
}

BOOL CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
	return TRUE;
}

BOOL CMainFrame::OnFileMRU(WPARAM wParam)
{
	UINT nMRUIndex = LOWORD(wParam) - IDW_FILE_MRU_FILE1;
	CString strMRUText = GetMRUEntry(nMRUIndex);

	if (!m_View.LoadPictureFile(strMRUText))
	{
		RemoveMRUEntry(strMRUText);
		m_View.NewPictureFile();
	}

	return TRUE;
}

BOOL CMainFrame::OnFileNew()
{
	m_View.NewPictureFile();
	return TRUE;
}

BOOL CMainFrame::OnFileOpen()
{
	LPCTSTR szFilters = _T("Supported Files Types(*.bmp;*.gif;*.jpg;*.ico;*.emf;*.wmf)\0*.bmp;*.gif;*.jpg;*.ico;*.emf;*.wmf\0Bitmaps (*.bmp)\0*.bmp\0GIF Files (*.gif)\0*.gif\0JPEG Files (*.jpg)\0*.jpg\0Icons (*.ico)\0*.ico\0Enhanced Metafiles (*.emf)\0*.emf\0Windows Metafiles (*.wmf)\0*.wmf\0\0");

	DWORD dwFlags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	CFileDialog FileDlg(TRUE, NULL, NULL, dwFlags, szFilters);
	
	if (FileDlg.DoModal(*this) == IDOK)
	{
		CString str = FileDlg.GetPathName();
		m_View.LoadPictureFile(str);
	}

	return TRUE;
}

LRESULT CMainFrame::OnFileLoaded(LPCTSTR szFile)
{
	SetWindowText(szFile);
	AdjustFrameRect(m_View.GetImageRect());
	AddMRUEntry(szFile);
	return 0L;
}

BOOL CMainFrame::OnFileSaveAs()
{
	if (m_View.GetPicture())
	{
		SHORT Type;
		m_View.GetPicture()->get_Type(&Type);
		LPCTSTR Filter = NULL;
		LPCTSTR Ext    = NULL;

		// Assign the default file extension and filter.
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

		DWORD dwFlags = OFN_OVERWRITEPROMPT;
		CFileDialog FileDlg(FALSE, Ext, NULL, dwFlags, Filter);

		if (FileDlg.DoModal(*this) == IDOK)
		{
			CString str = FileDlg.GetPathName();
			m_View.SavePicture(str);
			AddMRUEntry(str);
		}
	}

	return TRUE;
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
	case WM_SIZE:
		{
			// Remove scrollbars and redraw the view if the frame is maximized
			if (wParam == SIZE_MAXIMIZED)
			{
				GetView().ShowScrollBar(SB_BOTH, FALSE);
				GetView().Invalidate();
			}
			
			break;
		}
	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}
