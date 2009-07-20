////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class


#include "resource.h"
#include "mainfrm.h"


CMainFrame::CMainFrame()
{
	// Set m_View as the view window of the frame
	SetView(m_View);
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Process the messages from the Menu and Tool Bar
	switch (LOWORD(wParam))
	{
	case IDM_FILE_NEW:
		m_View.ClearPoints();
		m_PathName = _T("");
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
	case IDM_FILE_PRINT:
		::MessageBox(NULL, _T("File Print Implemented Later"), _T("Menu"), MB_OK);
		return TRUE;
	case IDM_PEN_RED:
		m_View.SetPen(RGB(255,0,0));
		return TRUE;
	case IDM_PEN_BLUE:
		m_View.SetPen(RGB(0,0,255));
		return TRUE;
	case IDM_PEN_GREEN:
		m_View.SetPen(RGB(0,196,0));
		return TRUE;
	case IDM_PEN_BLACK:
		m_View.SetPen(RGB(0,0,0));
		return TRUE;
	case IDM_FILE_EXIT:
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
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
	}

	return FALSE;
}

void CMainFrame::OnFileOpen()
{
	// Fill the OPENFILENAME structure
	TCHAR szFilters[] = _T("Scribble Files (*.dat)\0*.dat\0\0");
	TCHAR szFilePathName[_MAX_PATH] = _T("");
	OPENFILENAME ofn = {0};
	if (GetWinVersion() >= 2500)
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

	// Retrieve the PlotPoint data
	m_View.FileOpen(szFilePathName);
	
	// Save the filename
	m_PathName = szFilePathName;
}

void CMainFrame::OnFileSave()
{
	if (m_PathName == _T(""))
		OnFileSaveAs();
	else
		m_View.FileSave(m_PathName.c_str());
}

void CMainFrame::OnFileSaveAs()
{
	// Fill the OPENFILENAME structure
	TCHAR szFilters[] = _T("Scribble Files (*.dat)\0*.dat\0\0");
	TCHAR szFilePathName[_MAX_PATH] = _T("");
	OPENFILENAME ofn = {0};
	ofn.lStructSize = sizeof(OPENFILENAME);	
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = szFilters;
	ofn.lpstrFile = szFilePathName;
	ofn.lpstrDefExt = _T("dat");
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrTitle = _T("Save File");
	ofn.Flags = OFN_OVERWRITEPROMPT;

	// Open the file save dialog, and open the file
	if (!::GetSaveFileName(&ofn))
		return;

	// Store the PLotPoint data in the file
	m_PathName = szFilePathName;
	
	// Save the file name
	m_View.FileSave(szFilePathName);
}

void CMainFrame::SetupToolbar()
{
	// Define our toolbar
	AddToolbarButton( IDM_FILE_NEW   );
	AddToolbarButton( IDM_FILE_OPEN  );
	AddToolbarButton( IDM_FILE_SAVE  );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_EDIT_CUT,   FALSE );
	AddToolbarButton( IDM_EDIT_COPY,  FALSE );
	AddToolbarButton( IDM_EDIT_PASTE, FALSE );
	AddToolbarButton( IDM_FILE_PRINT );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_PEN_RED    );
	AddToolbarButton( IDM_PEN_BLUE   );
	AddToolbarButton( IDM_PEN_GREEN  );
	AddToolbarButton( IDM_PEN_BLACK  );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_HELP_ABOUT );
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{

//	} // switch (uMsg)

	return WndProcDefault(uMsg, wParam, lParam);
} // LRESULT CMainFrame::WndProc(...)

