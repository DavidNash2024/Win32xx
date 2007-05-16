////////////////////////////////////////////////////
// Mainfrm.cpp


#include "resource.h"
#include "mainfrm.h"
#include <string>


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
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
	cs.hMenu = (HMENU)IDC_COMBOBOXEX;
	m_ComboboxEx.PreCreate(cs);
	m_ComboboxEx.Create(GetRebar().GetHwnd());

	// Put the window in a new rebar band
	REBARBANDINFO rbbi = {0};

	rbbi.cbSize     = sizeof(REBARBANDINFO);
	rbbi.fMask      = RBBIM_COLORS | RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_CHILD | RBBIM_TEXT;
	rbbi.cyMinChild = Listbox_Height;
	rbbi.cyMaxChild = Listbox_Height;
	rbbi.fStyle     = RBBS_BREAK | RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS;
	rbbi.clrFore    = GetSysColor(COLOR_BTNTEXT);
	rbbi.clrBack    = GetSysColor(COLOR_BTNFACE);
	rbbi.hwndChild  = m_ComboboxEx.GetHwnd();
	rbbi.lpText     = TEXT("Address");

	GetRebar().InsertBand(-1, &rbbi);
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Respond to menu and and toolbar input
	switch(LOWORD(wParam))
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
		m_View.GetIWebBrowser2()->GoBack();
		break;
	case IDM_FORWARD:
		m_View.GetIWebBrowser2()->GoForward();
		break;
	case IDM_REFRESH:
		m_View.GetIWebBrowser2()->Refresh();
		break;
	case IDM_STOP:
		m_View.GetIWebBrowser2()->Stop();
		break;
	case IDM_HOME:
		m_View.GetIWebBrowser2()->GoHome();
		break;
	}

	// Handle notification WM_COMMAND from ComboboxEx
	if((HWND)lParam == m_ComboboxEx.GetHwnd())
	{
		switch(HIWORD(wParam))
		{
		case CBN_SELCHANGE:
			// User made selection from list
			{
				TCHAR szString[256];
		
				// Get text from edit box
				::SendMessage(m_ComboboxEx.GetHwnd(), WM_GETTEXT, 256, (LPARAM)szString);
			
				// Navigate to web page
				m_View.Navigate(szString);

				// Set focus to web page
				LONG_PTR hWeb;
				m_View.GetIWebBrowser2()->get_HWND(&hWeb);
				::SetFocus((HWND)hWeb);
			}
			break;
		}
	}

	// call the base class function
	return CFrame::OnCommand(wParam, lParam);
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

	m_View.GetIWebBrowser2()->GoHome();
}

LRESULT CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = m_ComboboxEx.GetHwnd();
	USES_CONVERSION;

	switch (((LPNMHDR)lParam)->code)
	{
	case CBEN_ENDEDIT:
		{
			switch (((PNMCBEENDEDIT)lParam)->iWhy)
			{
			case CBENF_RETURN:
				// User hit return in edit box
				{
					TCHAR szString[256];

					// Get text from edit box
					::SendMessage(hwnd, WM_GETTEXT, 256, (LPARAM)szString);
						
					// Insert text into the list box.
					COMBOBOXEXITEM CBXitem = {0};
					CBXitem.mask = CBEIF_TEXT;
					CBXitem.pszText = szString;
					::SendMessage(hwnd, CBEM_INSERTITEM, 0, (LPARAM) &CBXitem);
					
					// Navigate to the web page
					m_View.Navigate(szString);
					
					return FALSE;
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

//	}

	//Use the frame default message handling for remaining messages
	return CFrame::WndProc(hwnd, uMsg, wParam, lParam);
}

