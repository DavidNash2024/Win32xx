////////////////////////////////////////////////////
// Mainfrm.cpp


#include "resource.h"
#include "mainfrm.h"
#include <string>


#define MAX_ITEMS 15

// AddItems - Uses the CBEM_INSERTITEM message to add items to an
// existing ComboBoxEx control.


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_View as the view window of the frame
	SetView(m_View);

	// Set the Resource IDs for the toolbar buttons
	m_ToolbarData.clear();
	m_ToolbarData.push_back ( IDM_BACK   );
	m_ToolbarData.push_back ( IDM_FORWARD );
	m_ToolbarData.push_back ( 0 );				// Separator
	m_ToolbarData.push_back ( IDM_REFRESH );
	m_ToolbarData.push_back ( IDM_STOP );
	m_ToolbarData.push_back ( 0 );				// Separator
	m_ToolbarData.push_back ( IDM_HOME );
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

void CMainFrame::AddListboxBand(int Listbox_Height)
{
	// Create the ComboboxEx window
	CREATESTRUCT cs = {0};
	cs.lpszClass = TEXT("COMBOBOXEX32");
	cs.style = WS_VISIBLE | WS_CHILD | CBS_DROPDOWN;
	cs.cy = 100;	// required to display list
	m_Combobox.PreCreate(cs);
	m_Combobox.Create(GetRebar().GetHwnd());

	// Put the window in a new rebar band
	REBARBANDINFO rbbi = {0};

	rbbi.cbSize     = sizeof(REBARBANDINFO);
	rbbi.fMask      = RBBIM_COLORS | RBBIM_CHILDSIZE | RBBIM_STYLE |  RBBIM_CHILD | RBBIM_TEXT;
	rbbi.cyMinChild = Listbox_Height;
	rbbi.cyMaxChild = Listbox_Height;
	rbbi.fStyle     = RBBS_BREAK | RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS;
	rbbi.clrFore    = GetSysColor(COLOR_BTNTEXT);
	rbbi.clrBack    = GetSysColor(COLOR_BTNFACE);
	rbbi.hwndChild  = m_Combobox.GetHwnd();
	rbbi.lpText     = TEXT("Address");

	GetRebar().InsertBand(-1, &rbbi);
}

BOOL CMainFrame::OnCommand(UINT nID)
{
	// OnCommand responds to menu and and toolbar input
	switch(nID)
	{
	case IDM_FILE_EXIT:
		// End the application
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		break;
	case IDM_HELP_ABOUT:
		// Display the help dialog
		OnHelp();
		break;
	case IDM_BACK:
		TRACE("Back");
		break;
	case IDM_FORWARD:
		TRACE("FORWARD");
		break;
	case IDM_REFRESH:
		TRACE("REFRESH");
		break;
	case IDM_STOP:
		TRACE("STOP");
		break;
	case IDM_HOME:
		TRACE("HOME");
		break;
	}

	// call the base class function
	return CFrame::OnCommand(nID);
}

void CMainFrame::OnCreate()
{
	CFrame::OnCreate();
	AddListboxBand(22);
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.

	m_View.Navigate(TEXT("www.google.com"));
}

LRESULT CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = m_Combobox.GetHwnd();

	switch (((LPNMHDR)lParam)->code)
	{
	case CBEN_ENDEDIT:
		{
			switch (((PNMCBEENDEDIT)lParam)->iWhy)
			{
			case CBENF_RETURN:
				{
					std::basic_string<TCHAR> tString;

					// Get text from edit box
					::SendMessage(hwnd, WM_GETTEXT, 256, (LPARAM)tString.c_str()); 
					m_View.Navigate(tString.c_str());
					
					// Insert text into the list box.
					COMBOBOXEXITEM CBXitem = {0};
					CBXitem.mask = CBEIF_TEXT;
					CBXitem.pszText = (LPTSTR)tString.c_str();
					::SendMessage(hwnd, CBEM_INSERTITEM, 0, (LPARAM) &CBXitem); 
					
					// Put text in edit box
					::SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)tString.c_str());

					TRACE("CBENF_RETURN");
					return FALSE;
				}
			case CBENF_DROPDOWN:
				{
				//	LRESULT lr = ::SendMessage(hwnd , CB_GETCURSEL, 0, 0);
				//	std::basic_string<TCHAR> tString;

				//	::SendMessage(hwnd, CB_GETLBTEXT, lr, (LPARAM)tString.c_str());
				//	m_View.Navigate(tString.c_str());
					TRACE("CBENF_DROPDOWN");
					break;
				} 
			}
		}
	}
	return CFrame::OnNotify(wParam, lParam);
}

void CMainFrame::SetButtons(const std::vector<UINT> ToolbarData)
{
	// Overriding CFrame::Setbuttons is optional. We do it here to use larger buttons 
	// with seperate imagelists for normal, hot and disabled buttons.

	// A reference to the CToolbar object
	CToolbar& TB = GetToolbar();

	// Set the button size to 24x24 before adding the bitmap
	TB.SetBitmapSize(24, 24);

	// Set the image lists for normal, hot and disabled buttons
	TB.SetImageList(5, RGB(255,0,255), IDB_TOOLBAR_NORM, IDB_TOOLBAR_HOT, IDB_TOOLBAR_DIS);

	// Set the resource IDs for the toolbar buttons
	TB.SetButtons(ToolbarData);

	// Adjust the toolbar and rebar size to take account of the larger buttons
	RECT r;
	TB.GetItemRect(TB.CommandToIndex(IDM_BACK), &r);
	TB.SetButtonSize(r.right - r.left, r.bottom - r.top);

}

LRESULT CMainFrame::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
		//Additional messages to be handled go here
//	}

	//Use the frame default message handling for remaining messages
	return CFrame::WndProc(hwnd, uMsg, wParam, lParam);
}

