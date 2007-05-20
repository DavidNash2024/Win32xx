////////////////////////////////////////////////////
// Mainfrm.cpp


#include "resource.h"
#include "mainfrm.h"


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

void CMainFrame::OnBeforeNavigate(DISPPARAMS* pDispParams)
{
	// Update the URL in the ComboboxEx edit box.
	if (pDispParams->cArgs >= 5 && pDispParams->rgvarg[5].vt == (VT_BYREF|VT_VARIANT))
	{
		CComVariant vtURL(*pDispParams->rgvarg[5].pvarVal);
		vtURL.ChangeType(VT_BSTR);

		HWND hwnd = m_ComboboxEx.GetHwnd();

		USES_CONVERSION;
		::SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)OLE2T(vtURL.bstrVal));		
	}
}

void CMainFrame::OnCommandStateChange(DISPPARAMS* pDispParams)
{
	CToolbar& TB = GetToolbar();

	if ( pDispParams)
	{
		if (pDispParams->cArgs == 2)
		{
			if (pDispParams->rgvarg[1].vt == (VT_I4) && pDispParams->rgvarg[0].vt == (VT_BOOL))
			{
				VARIANT_BOOL bEnable = pDispParams->rgvarg[0].boolVal;
				int nCommand = pDispParams->rgvarg[1].intVal;
				{
					switch (nCommand)
					{
					case 1: // Navigate forward:
						bEnable ? TB.EnableButton(IDM_FORWARD) : TB.DisableButton(IDM_FORWARD);

						break;
					case 2: // Navigate back:
						bEnable ? TB.EnableButton(IDM_BACK) : TB.DisableButton(IDM_BACK);
						break;
					}
				}
			}
		}
	}
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

void CMainFrame::OnDocumentBegin(DISPPARAMS* pDispParams)
{
	TRACE(TEXT("OnDocumentBegin"));
}

void CMainFrame::OnCreate()
{
	CFrame::OnCreate();
	AddListboxBand(22);
}

void CMainFrame::OnDocumentComplete(DISPPARAMS* pDispParams)
{
	m_StatusText = TEXT("Done");
}

void CMainFrame::OnDownloadBegin(DISPPARAMS* pDispParams)
{
}

void CMainFrame::OnDownloadComplete(DISPPARAMS* pDispParams)
{
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.

	m_View.GetIWebBrowser2()->GoHome();
}

void CMainFrame::OnNavigateComplete2(DISPPARAMS* pDispParams)
{
	USES_CONVERSION;
	tString szString = TEXT("NavigateComplete2: ");

	if (pDispParams->rgvarg[0].vt == (VT_BYREF|VT_VARIANT))
	{
		CComVariant vtURL(*pDispParams->rgvarg[0].pvarVal);
		vtURL.ChangeType(VT_BSTR);
	
		szString += OLE2T(vtURL.bstrVal);
		TRACE(szString.c_str());
	}
}

void CMainFrame::OnNewWindow2(DISPPARAMS* pDispParams)
{
	//TRACE(TEXT("NewWindow2"));
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

void CMainFrame::OnProgressChange(DISPPARAMS* pDispParams)
{
	tStringStream szString;

	if (pDispParams->cArgs != 0)
	{
		if (pDispParams->cArgs > 1 && pDispParams->rgvarg[1].vt == VT_I4)
			szString << TEXT("Progress = ") << pDispParams->rgvarg[1].lVal ;

		if (pDispParams->rgvarg[0].vt == VT_I4)
			szString << TEXT(", ProgressMax = ") << pDispParams->rgvarg[0].lVal;

		TRACE(szString.str().c_str());		
   }
}

void CMainFrame::OnPropertyChange(DISPPARAMS* pDispParams)
{
		/*	     if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_BSTR)
		      strEventInfo << OLE2T(pDispParams->rgvarg[0].bstrVal);
		   else
		      strEventInfo << "NULL";

		   strEventInfo << ends;   */
}

void CMainFrame::OnStatusTextChange(DISPPARAMS* pDispParams)
{
	USES_CONVERSION;
	LPOLESTR lpStatusText = pDispParams->rgvarg->bstrVal;
	

	if (lpStatusText)
	{
		if (strcmp(OLE2T(lpStatusText), ""))
		{
			GetStatusbar().SetPaneText(0, OLE2T(lpStatusText));
		}
		else
			GetStatusbar().SetPaneText(0, TEXT("Done"));
	}
}

void CMainFrame::OnTimer(WPARAM wParam)
{
	CFrame::OnTimer(wParam);
	m_StatusText = TEXT("Done");
}

void CMainFrame::OnTitleChange(DISPPARAMS* pDispParams)
{
	TRACE("TitleChange: ");
	USES_CONVERSION;

	if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_BSTR)
	{
		TRACE(OLE2T(pDispParams->rgvarg[0].bstrVal));
		::SetWindowText(m_hWnd, OLE2T(pDispParams->rgvarg[0].bstrVal));
	}
	else
		::SetWindowText(m_hWnd, LoadString(IDW_MAIN));
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

