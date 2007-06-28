////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class


#include "resource.h"
#include "mainfrm.h"


CMainFrame::CMainFrame()
{
	// Set m_View as the view window of the frame
	SetView(m_View);

	// Define our toolbar
	m_ToolbarData.clear();
	m_ToolbarData.push_back ( IDM_FILE_NEW   );
	m_ToolbarData.push_back ( IDM_FILE_OPEN  );
	m_ToolbarData.push_back ( IDM_FILE_SAVE  );
	m_ToolbarData.push_back ( 0 );				// Separator
	m_ToolbarData.push_back ( IDM_EDIT_CUT   );
	m_ToolbarData.push_back ( IDM_EDIT_COPY  );
	m_ToolbarData.push_back ( IDM_EDIT_PASTE );
	m_ToolbarData.push_back ( IDM_FILE_PRINT );
	m_ToolbarData.push_back ( 0 );				// Separator
	m_ToolbarData.push_back ( IDM_PEN_RED    );
	m_ToolbarData.push_back ( IDM_PEN_BLUE   );
	m_ToolbarData.push_back ( IDM_PEN_GREEN  );
	m_ToolbarData.push_back ( IDM_PEN_BLACK  );
	m_ToolbarData.push_back ( 0 );				// Separator
	m_ToolbarData.push_back ( IDM_HELP_ABOUT );
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
		::MessageBox(NULL, _T("File New"), _T("Menu"), MB_OK);
		return TRUE;
	case IDM_FILE_OPEN:
		::MessageBox(NULL, _T("File Open"), _T("Menu"), MB_OK);
		return TRUE;
	case IDM_FILE_SAVE:
		::MessageBox(NULL, _T("File Save"), _T("Menu"), MB_OK);
		return TRUE;
	case IDM_FILE_SAVEAS:
		::MessageBox(NULL, _T("File SaveAs"), _T("Menu"), MB_OK);
		return TRUE;
	case IDM_FILE_PRINT:
		::MessageBox(NULL, _T("File Print"), _T("Menu"), MB_OK);
		return TRUE;
	case IDM_EDIT_COPY:
		::MessageBox(NULL, _T("Edit Copy"), _T("Menu"), MB_OK);
		return TRUE;
	case IDM_EDIT_PASTE:
		::MessageBox(NULL, _T("Edit Paste"), _T("Menu"), MB_OK);
		return TRUE;
	case IDM_EDIT_CUT:
		::MessageBox(NULL, _T("Edit Cut"), _T("Menu"), MB_OK);
		return TRUE;
	case IDM_EDIT_DELETE:
		::MessageBox(NULL, _T("Edit Delete"), _T("Menu"), MB_OK);
		return TRUE;
	case IDM_EDIT_REDO:
		::MessageBox(NULL, _T("Edit Redo"), _T("Menu"), MB_OK);
		return TRUE;
	case IDM_EDIT_UNDO:
		::MessageBox(NULL, _T("Edit Undo"), _T("Menu"), MB_OK);
		return TRUE;
	case IDM_PEN_RED:
		TRACE("Red pen selected");
		m_View.SetPen(RGB(255,0,0));
		return TRUE;
	case IDM_PEN_BLUE:
		TRACE("Blue pen selected");
		m_View.SetPen(RGB(0,0,255));
		return TRUE;
	case IDM_PEN_GREEN:
		TRACE("Green pen selected");
		m_View.SetPen(RGB(0,196,0));
		return TRUE;
	case IDM_PEN_BLACK:
		TRACE("Black pen selected");
		m_View.SetPen(RGB(0,0,0));
		return TRUE;
	case IDM_HELP_ABOUT:
		OnHelp();
		return TRUE;
	case IDM_FILE_EXIT:
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	}

	return FALSE;
}

LRESULT CMainFrame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{

//	} // switch (uMsg)

	return CFrame::WndProc(hWnd, uMsg, wParam, lParam);
} // LRESULT CMainFrame::WndProc(...)

