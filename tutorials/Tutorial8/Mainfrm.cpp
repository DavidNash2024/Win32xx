////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class


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
	case IDM_FILE_NEW:			OnFileNew();		return TRUE;
	case IDM_FILE_OPEN:			OnFileOpen();		return TRUE;
	case IDM_FILE_SAVE:			OnFileSave();		return TRUE;
	case IDM_FILE_SAVEAS:		OnFileSaveAs();		return TRUE;
	case IDM_FILE_PRINT:		OnFilePrint();		return TRUE;	
	case IDM_FILE_EXIT:			OnFileExit();		return TRUE;
	case IDM_PEN_COLOR:			OnPenColor();		return TRUE;	
	case IDW_VIEW_STATUSBAR:	return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:		return OnViewToolBar();
	case IDM_HELP_ABOUT:		return OnHelp();
	}

	return FALSE;
}

void CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
}

void CMainFrame::OnFileNew()
{
	GetDoc().GetAllPoints().clear();
	m_PathName = _T("");
	GetView().Invalidate();
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

void CMainFrame::OnFileOpen()
{
	CFileDialog FileDlg(TRUE, _T("dat"), 0, OFN_FILEMUSTEXIST, _T("Scribble Files (*.dat)\0*.dat\0\0"));
	FileDlg.SetTitle(_T("Open File"));

	// Bring up the file open dialog retrieve the selected filename
	if (FileDlg.DoModal(*this) == IDOK)
	{
		// Load the file
		LoadFile(FileDlg.GetPathName());
	}
}

void CMainFrame::OnFilePrint()
{
	::MessageBox(NULL, _T("File Print  ... Implemented later"), _T("Menu"), MB_OK);
}

void CMainFrame::OnFileSave()
{
	if (m_PathName == _T(""))
		OnFileSaveAs();
	else
		GetDoc().FileSave(m_PathName);
}

void CMainFrame::OnFileSaveAs()
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
}

void CMainFrame::OnPenColor()
{
	// array of custom colors, initialized to white
	static COLORREF CustColors[16] = {	RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255),
										RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255),
										RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255),
										RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255) };
	
	CColorDialog ColorDlg;
	ColorDlg.SetCustomColors(CustColors);
	
	// Initialize the Choose Color dialog
	if (ColorDlg.DoModal(*this) == IDOK)
	{
		// Store the custom colors in the static array
		memcpy(CustColors, ColorDlg.GetCustomColors(), 16*sizeof(COLORREF));
		
		// Retrieve the chosen color
		m_View.SetPenColor(ColorDlg.GetColor());
	}
}

void CMainFrame::SetupToolBar()
// Configures the ToolBar
{
	// Define our toolbar buttons
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

LRESULT CMainFrame::OnDropFile(WPARAM wParam)
// Called in response to the UWM_DROPFILE user defined message
{
	// wParam is a pointer (LPCTSTR) to the filename
	LPCTSTR szFileName = reinterpret_cast<LPCTSTR>(wParam);
	assert(szFileName);

	// Load the file
	LoadFile(szFileName);
	return 0L;
}

LRESULT CMainFrame::OnGetAllPoints()
// Called in response to the UWM_GETALLPOINTS user defined message
{
	// Get a pointer to the vector of PlotPoints
	std::vector<PlotPoint>* pAllPoints = &GetDoc().GetAllPoints();

	// Cast the pointer to a LRESULT and return it
	return reinterpret_cast<LRESULT>(pAllPoints);
}

LRESULT CMainFrame::OnSendPoint(WPARAM wParam)
// Called in response to the UWM_SENDPOINT user defined message
{
	// wParam is a pointer to the vector of PlotPoints
	PlotPoint* pPP = reinterpret_cast<PlotPoint*>(wParam);

	// Dereference the pointer and store the vector of PlotPoints in CDoc
	GetDoc().StorePoint(*pPP);
	return 0L;
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
// Called to handle the window's messages
{
	switch (uMsg)
	{
	case UWM_DROPFILE:			return OnDropFile(wParam);
	case UWN_GETALLPOINTS:		return OnGetAllPoints();
	case UWM_SENDPOINT:			return OnSendPoint(wParam);
	}

	//Use the default message handling for remaining messages
	return WndProcDefault(uMsg, wParam, lParam);
}

