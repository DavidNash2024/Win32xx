////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class

#include "targetver.h"
#include "mainfrm.h"
#include "resource.h"

CMainFrame::CMainFrame()
{
	//Set m_View as the view window of the frame
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
	case IDM_FILE_NEW:			OnFileNew();		return TRUE;
	case IDM_FILE_OPEN:			OnFileOpen();		return TRUE;
	case IDM_FILE_SAVE:			OnFileSave();		return TRUE;
	case IDM_FILE_SAVEAS:		OnFileSaveAs();		return TRUE;
	case IDM_FILE_PRINT:		OnFilePrint();		return TRUE;
	case IDW_VIEW_STATUSBAR:	OnViewStatusBar();	return TRUE;
	case IDW_VIEW_TOOLBAR:		OnViewToolBar();	return TRUE;
	case IDM_HELP_ABOUT:		OnHelp();			return TRUE;
	case IDM_FILE_EXIT:			OnFileExit();		return TRUE;
	}

	return FALSE;
}

void CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
}

void CMainFrame::OnFileNew()
{
	::MessageBox(NULL, _T("File New  ... Implemented later"), _T("Menu"), MB_OK);
}

void CMainFrame::OnFileOpen()
{
	::MessageBox(NULL, _T("File Open  ... Implemented later"), _T("Menu"), MB_OK);
}

void CMainFrame::OnFilePrint()
{
	::MessageBox(NULL, _T("File Print  ... Implemented later"), _T("Menu"), MB_OK);
}

void CMainFrame::OnFileSave()
{
	::MessageBox(NULL, _T("File Save  ... Implemented later"), _T("Menu"), MB_OK);
}

void CMainFrame::OnFileSaveAs()
{
	::MessageBox(NULL, _T("File SaveAs  ... Implemented later"), _T("Menu"), MB_OK);
}

void CMainFrame::SetupToolBar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolBarButton( IDM_FILE_NEW   );
	AddToolBarButton( IDM_FILE_OPEN  );
	AddToolBarButton( IDM_FILE_SAVE  );
	
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_EDIT_CUT );
	AddToolBarButton( IDM_EDIT_COPY );
	AddToolBarButton( IDM_EDIT_PASTE );
	
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_FILE_PRINT );
	
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

