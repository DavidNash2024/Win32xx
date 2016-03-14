////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class

#include "targetver.h"
#include "mainfrm.h"
#include "resource.h"


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

	UNREFERENCED_PARAMETER(lParam);

	switch (LOWORD(wParam))
	{
	case IDM_FILE_NEW:			return OnFileNew();
	case IDM_FILE_OPEN:			return OnFileOpen();
	case IDM_FILE_SAVE:			return OnFileSave();
	case IDM_FILE_SAVEAS:		return OnFileSaveAs();
	case IDM_PEN_COLOR:			return OnPenColor();
	case IDW_VIEW_STATUSBAR:	return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:		return OnViewToolBar();
	case IDM_HELP_ABOUT:		return OnHelp();
	case IDM_FILE_EXIT:			return OnFileExit();
	}

	return FALSE;
}

BOOL CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);

	return TRUE;
}

BOOL CMainFrame::OnFileNew()
{
	GetDoc().GetPoints().clear();
	m_PathName = _T("");
	GetView().Invalidate();
	return TRUE;
}

void CMainFrame::LoadFile(LPCTSTR str)
// Called by OnFileOpen and in response to a UWM_DROPFILE message
{
	// Retrieve the PlotPoint data
	if (GetDoc().FileOpen(str))
	{
		// Save the filename
		m_PathName = str;
	}
	else
		m_PathName=_T("");

	GetView().Invalidate();
}

BOOL CMainFrame::OnFileOpen()
{
	CFileDialog FileDlg(TRUE, _T("dat"), 0, OFN_FILEMUSTEXIST, _T("Scribble Files (*.dat)\0*.dat\0\0"));
	FileDlg.SetTitle(_T("Open File"));

	// Bring up the file open dialog retrieve the selected filename
	if (FileDlg.DoModal(*this) == IDOK)
	{
		// Load the file
		LoadFile(FileDlg.GetPathName());
	}

	return TRUE;
}

BOOL CMainFrame::OnFileSave()
{
	if (m_PathName == _T(""))
		OnFileSaveAs();
	else
		GetDoc().FileSave(m_PathName);

	return TRUE;
}

BOOL CMainFrame::OnFileSaveAs()
{
	CFileDialog FileDlg(FALSE, _T("dat"), 0, OFN_OVERWRITEPROMPT, _T("Scribble Files (*.dat)\0*.dat\0\0"));
	FileDlg.SetTitle(_T("Save File"));

	// Bring up the file open dialog retrieve the selected filename
	if (FileDlg.DoModal(*this) == IDOK)
	{
		CString str = FileDlg.GetPathName();

		// Save the file
		if (GetDoc().FileSave(str))
		{
			// Save the file name
			m_PathName = str;
			AddMRUEntry(m_PathName);
		}
	}

	return TRUE;
}

BOOL CMainFrame::OnPenColor()
{
	static COLORREF CustColors[16] = {0};	// array of custom colors
	CHOOSECOLOR cc = {0};					// Structure used by ChooseColor

	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	cc.rgbResult = m_View.GetPenColor();
	cc.lpCustColors = CustColors;
	cc.hwndOwner = *this;
	
	// Initiate the Choose Color dialog
	if (ChooseColor(&cc)==TRUE) 
	{
		m_View.SetPenColor(cc.rgbResult);
	}

	return TRUE;
}

void CMainFrame::SetupToolBar()
{
	// Define our toolbar
	AddToolBarButton( IDM_FILE_NEW   );
	AddToolBarButton( IDM_FILE_OPEN  );
	AddToolBarButton( IDM_FILE_SAVE  );
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_EDIT_CUT,   FALSE );
	AddToolBarButton( IDM_EDIT_COPY,  FALSE );
	AddToolBarButton( IDM_EDIT_PASTE, FALSE );
	AddToolBarButton( IDM_FILE_PRINT );
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_PEN_COLOR );
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_HELP_ABOUT );
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case UWM_DROPFILE:
	{
		LPCTSTR szFileName = (LPCTSTR)wParam;
		assert(szFileName);
		LoadFile(szFileName);
		break;
	}

	} // switch (uMsg)

	return WndProcDefault(uMsg, wParam, lParam);
} // LRESULT CMainFrame::WndProc(...)

