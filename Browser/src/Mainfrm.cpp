////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "resource.h"
#include "mainfrm.h"
#include <sstream>

typedef std::basic_stringstream<TCHAR> tStringStream;


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	//Set m_View as the view window of the frame
	SetView(m_View);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\Browser Sample"));
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

void CMainFrame::AddComboBoxBand(int Listbox_Height)
{
	// Get the reference to the rebar object
	CRebar& RB = GetRebar();
	RebarTheme RBTheme = RB.GetRebarTheme();

	// Create the ComboboxEx window
	CREATESTRUCT cs = {0};
	cs.lpszClass = _T("COMBOBOXEX32");
	cs.style = WS_VISIBLE | WS_CHILD | CBS_DROPDOWN;
	cs.cy = 100;	// required to display list
	cs.hMenu = (HMENU)IDC_COMBOBOXEX;
	m_ComboboxEx.PreCreate(cs);
	m_ComboboxEx.Create(GetRebar().GetHwnd());

	// Put the window in a new rebar band
	REBARBANDINFO rbbi = {0};
	rbbi.cbSize		= sizeof(REBARBANDINFO);
	rbbi.fMask      = RBBIM_COLORS | RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_CHILD | RBBIM_TEXT;
	rbbi.cyMinChild = Listbox_Height;
	rbbi.cyMaxChild = Listbox_Height;
	rbbi.cxMinChild = 200;
	rbbi.fStyle     = RBBS_BREAK | RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS;
	rbbi.clrFore    = GetSysColor(COLOR_BTNTEXT);
	rbbi.clrBack    = RBTheme.clrBand1;
	rbbi.hwndChild  = m_ComboboxEx.GetHwnd();
	rbbi.lpText     = _T("Address");

	RB.InsertBand(-1, rbbi);
}

void CMainFrame::OnBeforeNavigate(DISPPARAMS* pDispParams)
{
	UNREFERENCED_PARAMETER(pDispParams);
}

void CMainFrame::OnCommandStateChange(DISPPARAMS* pDispParams)
{
	CToolbar& TB = GetToolbar();

	if ((pDispParams) && (pDispParams->cArgs == 2))
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

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Respond to menu and and toolbar input
	switch(LOWORD(wParam))
	{
	case IDM_FILE_EXIT:
		// End the application
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	case IDM_HELP_ABOUT:
		// Display the help dialog
		OnHelp();
		return TRUE;
	case IDM_BACK:
		m_View.GetIWebBrowser2()->GoBack();
		return TRUE;
	case IDM_FORWARD:
		m_View.GetIWebBrowser2()->GoForward();
		return TRUE;
	case IDM_REFRESH:
		m_View.GetIWebBrowser2()->Refresh();
		return TRUE;
	case IDM_STOP:
		m_View.GetIWebBrowser2()->Stop();
		return TRUE;
	case IDM_HOME:
		m_View.GetIWebBrowser2()->GoHome();
		return TRUE;
	case IDW_VIEW_STATUSBAR:
		OnViewStatusbar();
		return TRUE;
	case IDW_VIEW_TOOLBAR:
		OnViewToolbar();
		return TRUE;
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
			return TRUE;
		}
	}

	return FALSE;
}

void CMainFrame::OnDocumentBegin(DISPPARAMS* pDispParams)
{
	UNREFERENCED_PARAMETER(pDispParams);
	TRACE(_T("OnDocumentBegin\n"));
}

void CMainFrame::OnCreate()
{
	// Call the base function first
	CFrame::OnCreate();
}

void CMainFrame::OnDocumentComplete(DISPPARAMS* pDispParams)
{
	UNREFERENCED_PARAMETER(pDispParams);
	GetStatusbar().SetPartText(0, _T("Done"));
}

void CMainFrame::OnDownloadBegin(DISPPARAMS* pDispParams)
{
	UNREFERENCED_PARAMETER(pDispParams);
}

void CMainFrame::OnDownloadComplete(DISPPARAMS* pDispParams)
{
	UNREFERENCED_PARAMETER(pDispParams);
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
	tString szString = _T("NavigateComplete2: ");

	if (pDispParams->rgvarg[0].vt == (VT_BYREF|VT_VARIANT))
	{
		CComVariant vtURL(*pDispParams->rgvarg[0].pvarVal);
		vtURL.ChangeType(VT_BSTR);

		szString += OLE2T(vtURL.bstrVal);
		szString += _T("\n");
		TRACE(szString.c_str());
	}

	BSTR bstrUrlName;

	CView* pView = (CView*)GetView();
	HRESULT hr = pView->GetIWebBrowser2()->get_LocationURL(&bstrUrlName);
	if (FAILED(hr))
		return;

	// Update the URL in the ComboboxEx edit box.
	m_ComboboxEx.SendMessage(WM_SETTEXT, 0, (LPARAM)OLE2T(bstrUrlName));
}

void CMainFrame::OnNewWindow2(DISPPARAMS* pDispParams)
{
	UNREFERENCED_PARAMETER(pDispParams);
	TRACE(_T("NewWindow2\n"));
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
			szString << _T("Progress = ") << pDispParams->rgvarg[1].lVal ;

		if (pDispParams->rgvarg[0].vt == VT_I4)
			szString << _T(", ProgressMax = ") << pDispParams->rgvarg[0].lVal;

		szString << _T("\n");
		TRACE(szString.str().c_str());
   }
}

void CMainFrame::OnPropertyChange(DISPPARAMS* pDispParams)
{
	USES_CONVERSION;
	tStringStream str;
	if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_BSTR)
		str << _T("Property Change:") << OLE2T(pDispParams->rgvarg[0].bstrVal);

	str << _T("\n");
	TRACE(str.str().c_str());
}

void CMainFrame::OnStatusTextChange(DISPPARAMS* pDispParams)
{
	USES_CONVERSION;
	LPOLESTR lpStatusText = pDispParams->rgvarg->bstrVal;


	if (lpStatusText)
	{
		if (lstrcmp(OLE2T(lpStatusText), _T("")))
		{
			GetStatusbar().SetPartText(0, OLE2T(lpStatusText));
		}
		else
			GetStatusbar().SetPartText(0, _T("Done"));
	}
}

void CMainFrame::OnTimer(WPARAM wParam)
{
	UNREFERENCED_PARAMETER(wParam);
	GetStatusbar().SetPartText(0, _T("Done"));
}

void CMainFrame::OnTitleChange(DISPPARAMS* pDispParams)
{
	TRACE(_T("TitleChange: \n"));
	USES_CONVERSION;
	tStringStream str;

	if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_BSTR)
	{
		str << OLE2T(pDispParams->rgvarg[0].bstrVal) << _T(" - ") << LoadString(IDW_MAIN);
		TRACE(OLE2T(pDispParams->rgvarg[0].bstrVal));
	}
	else
		str << LoadString(IDW_MAIN);

	::SetWindowText(m_hWnd, str.str().c_str());
}

void CMainFrame::SetupToolbar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolbarButton( IDM_BACK   );
	AddToolbarButton( IDM_FORWARD );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_REFRESH );
	AddToolbarButton( IDM_STOP );
	AddToolbarButton( 0 );				// Separator
	AddToolbarButton( IDM_HOME );

	// Set the image lists for normal, hot and disabled buttons
	SetToolbarImages(RGB(255,0,255), IDB_TOOLBAR_NORM, IDB_TOOLBAR_HOT, IDB_TOOLBAR_DIS);

	std::vector<UINT> IconData;
	if (IsRebarUsed())
	{
		int Height = 22;
		AddComboBoxBand(Height);

		// Set the icons for popup menu items
		IconData.push_back ( IDM_FILE_NEW  );
		IconData.push_back ( IDM_FILE_OPEN );
		IconData.push_back ( IDM_FILE_SAVE );
		IconData.push_back ( IDM_EDIT_CUT  );
		IconData.push_back ( IDM_EDIT_COPY );
		IconData.push_back ( IDM_EDIT_PASTE);
		IconData.push_back ( IDM_FILE_PRINT);
		IconData.push_back ( IDM_HELP_ABOUT); 
	}
	
	AddMenuIcons(IconData, RGB(192, 192, 192), IDW_MAIN, 0);  
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
		//Additional messages to be handled go here
//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

