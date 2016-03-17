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
	PostMessage(WM_CLOSE);

	return TRUE;
}

BOOL CMainFrame::OnFileNew()
{
	::MessageBox(NULL, _T("File New  ... Implemented later"), _T("Menu"), MB_OK);
	return TRUE;
}

BOOL CMainFrame::OnFileOpen()
{
	::MessageBox(NULL, _T("File Open  ... Implemented later"), _T("Menu"), MB_OK);
	return TRUE;
}

BOOL CMainFrame::OnFilePrint()
{
	::MessageBox(NULL, _T("File Print  ... Implemented later"), _T("Menu"), MB_OK);
	return TRUE;
}

BOOL CMainFrame::OnFileSave()
{
	::MessageBox(NULL, _T("File Save  ... Implemented later"), _T("Menu"), MB_OK);
	return TRUE;
}

BOOL CMainFrame::OnFileSaveAs()
{
	::MessageBox(NULL, _T("File SaveAs  ... Implemented later"), _T("Menu"), MB_OK);
	return TRUE;
}

BOOL CMainFrame::OnPenColor()
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

