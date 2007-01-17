////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class


#include "view.h"
#include "..\Win32++\frame.h"
#include "resource.h"
#include "mainfrm.h"


CMainFrame::CMainFrame()
{
	// Set m_View as the view window of the frame
	SetView(m_View);

	// Define our toolbar
	BYTE ToolbarData[] =
		{
		 IDM_FILE_NEW,
		 IDM_FILE_OPEN,
		 IDM_FILE_SAVE,
		 0,					// Separator
		 IDM_EDIT_CUT,
		 IDM_EDIT_COPY,
		 IDM_EDIT_PASTE,
		 IDM_FILE_PRINT,
		 0,					// Separator
		 IDM_PEN_RED,
		 IDM_PEN_BLUE,
		 IDM_PEN_GREEN,
		 IDM_PEN_BLACK,
		 0,					// Separator
		 IDM_HELP_ABOUT
		};

	// Calculate the number of button entries
	int nButtons = sizeof(ToolbarData)/(sizeof(BYTE));

	SetToolbarData(nButtons, ToolbarData);
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::OnCommand(UINT nID)
{
	// Process the messages from the Menu and Tool Bar
	switch (nID)
	{
	case IDM_FILE_NEW:
		::MessageBox(NULL, TEXT("File New"), TEXT("Menu"), MB_OK);
		break;
	case IDM_FILE_OPEN:
		::MessageBox(NULL, TEXT("File Open"), TEXT("Menu"), MB_OK);
		break;
	case IDM_FILE_SAVE:
		::MessageBox(NULL, TEXT("File Save"), TEXT("Menu"), MB_OK);
		break;
	case IDM_FILE_SAVEAS:
		::MessageBox(NULL, TEXT("File SaveAs"), TEXT("Menu"), MB_OK);
		break;
	case IDM_FILE_PRINT:
		::MessageBox(NULL, TEXT("File Print"), TEXT("Menu"), MB_OK);
		break;
	case IDM_EDIT_COPY:
		::MessageBox(NULL, TEXT("Edit Copy"), TEXT("Menu"), MB_OK);
		break;
	case IDM_EDIT_PASTE:
		::MessageBox(NULL, TEXT("Edit Paste"), TEXT("Menu"), MB_OK);
		break;
	case IDM_EDIT_CUT:
		::MessageBox(NULL, TEXT("Edit Cut"), TEXT("Menu"), MB_OK);
		break;
	case IDM_EDIT_DELETE:
		::MessageBox(NULL, TEXT("Edit Delete"), TEXT("Menu"), MB_OK);
		break;
	case IDM_EDIT_REDO:
		::MessageBox(NULL, TEXT("Edit Redo"), TEXT("Menu"), MB_OK);
		break;
	case IDM_EDIT_UNDO:
		::MessageBox(NULL, TEXT("Edit Undo"), TEXT("Menu"), MB_OK);
		break;
	case IDM_PEN_RED:
		TRACE("Red pen selected");
		m_View.SetPen(RGB(255,0,0));
		break;
	case IDM_PEN_BLUE:
		TRACE("Blue pen selected");
		m_View.SetPen(RGB(0,0,255));
		break;
	case IDM_PEN_GREEN:
		TRACE("Green pen selected");
		m_View.SetPen(RGB(0,196,0));
		break;
	case IDM_PEN_BLACK:
		TRACE("Black pen selected");
		m_View.SetPen(RGB(0,0,0));
		break;
	}

	return CFrame::OnCommand(nID);
}

LRESULT CMainFrame::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{

//	} // switch (uMsg)

	//Use the frame default message handling for remaining messages
	return CFrame::WndProc(hwnd, uMsg, wParam, lParam);
} // LRESULT CMainFrame::WndProc(...)

