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
	UNREFERENCED_PARAMETER(lParam);

	// Process the messages from the Menu and Tool Bar
	switch (LOWORD(wParam))
	{
	case IDM_FILE_NEW:			return OnFileNew();
	case IDM_FILE_OPEN:			return OnFileOpen();
	case IDM_FILE_SAVE:			return OnFileSave();
	case IDM_FILE_SAVEAS:		return OnFileSaveAs();
	case IDM_FILE_PRINT:		return OnFilePrint();
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
	SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);

	return TRUE;
}

BOOL CMainFrame::OnFileNew()
{
	m_View.ClearPoints();
	m_PathName = _T("");
	return TRUE;
}

BOOL CMainFrame::OnFileOpen()
{
	CFile File;
	CString str = File.OpenFileDialog(0, OFN_FILEMUSTEXIST, _T("Open File"), _T("Scribble Files (*.dat)\0*.dat\0\0"), *this);

	if (!str.IsEmpty())
	{
		// Retrieve the PlotPoint data
		if (m_View.FileOpen(str))
		{
			// Save the filename
			m_PathName = str;
			AddMRUEntry(str);
		}
		else
			m_PathName=_T("");
	}

	return TRUE;
}

BOOL CMainFrame::OnFilePrint()
{
	::MessageBox(NULL, _T("File Print Implemented Later"), _T("Menu"), MB_OK);
	return TRUE;
}

BOOL CMainFrame::OnFileSave()
{
	if (m_PathName == _T(""))
		OnFileSaveAs();
	else
		m_View.FileSave(m_PathName);

	return TRUE;
}

BOOL CMainFrame::OnFileSaveAs()
{
	CFile File;
	CString str = File.SaveFileDialog(0, OFN_OVERWRITEPROMPT, _T("Save File"), _T("Scribble Files (*.dat)\0*.dat\0\0"), _T("dat"), *this);

	// Store the PlotPoint data in the file
	if (!str.IsEmpty())
	{
		m_PathName = str;

		// Save the file name
		m_View.FileSave(str);
		AddMRUEntry(str);
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
//	switch (uMsg)
//	{

//	} // switch (uMsg)

	return WndProcDefault(uMsg, wParam, lParam);
} // LRESULT CMainFrame::WndProc(...)

