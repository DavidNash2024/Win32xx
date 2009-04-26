////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class

#include "resource.h"
#include "mainfrm.h"


CMainFrame::CMainFrame()
{
	//Set m_View as the view window of the frame
	SetView(m_View);
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	// Process the messages from the Menu and Tool Bar
	switch (LOWORD(wParam))
	{
	case IDM_FILE_EXIT:
		// End the application
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	case IDM_HELP_ABOUT:
		// Display the help dialog
		OnHelp();
		return TRUE;
	case IDM_FILE_NEW:
		::MessageBox(NULL, _T("File New  ... Implemented later"), _T("Menu"), MB_OK);
		return TRUE;
	case IDM_FILE_OPEN:
		::MessageBox(NULL, _T("File Open  ... Implemented later"), _T("Menu"), MB_OK);
		return TRUE;
	case IDM_FILE_SAVE:
		::MessageBox(NULL, _T("File Save  ... Implemented later"), _T("Menu"), MB_OK);
		return TRUE;
	case IDM_FILE_SAVEAS:
		::MessageBox(NULL, _T("File SaveAs  ... Implemented later"), _T("Menu"), MB_OK);
		return TRUE;
	case IDM_FILE_PRINT:
		::MessageBox(NULL, _T("File Print  ... Implemented later"), _T("Menu"), MB_OK);
		return TRUE;
	}

	return FALSE;
}

void CMainFrame::LoadToolbar()
{	
	// Define the resource IDs for the toolbar
	AddToolbarButton( IDM_FILE_NEW   );
	AddToolbarButton( IDM_FILE_OPEN  );
	AddToolbarButton( IDM_FILE_SAVE  );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_EDIT_CUT   );
	AddToolbarButton( IDM_EDIT_COPY  );
	AddToolbarButton( IDM_EDIT_PASTE );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_FILE_PRINT );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_HELP_ABOUT );
}

LRESULT CMainFrame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{

//	} // switch (uMsg)

	return WndProcDefault(hWnd, uMsg, wParam, lParam);
} // LRESULT CMainFrame::WndProc(...)

