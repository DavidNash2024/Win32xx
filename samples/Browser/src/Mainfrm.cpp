////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "resource.h"
#include "mainfrm.h"
#include <sstream>


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	//Set m_View as the view window of the frame
	SetView(m_View);

	// Set CMainFrame as our event sink
	m_EventSink.SetSink(this);

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
	CReBar& RB = GetReBar();
	ReBarTheme RBTheme = RB.GetReBarTheme();

	// Create the ComboboxEx window
	CREATESTRUCT cs = {0};
	cs.lpszClass = _T("COMBOBOXEX32");
	cs.style = WS_VISIBLE | WS_CHILD | CBS_DROPDOWN;
	cs.cy = 100;	// required to display list
	cs.hMenu = (HMENU)IDC_COMBOBOXEX;
	m_ComboboxEx.PreCreate(cs);
	m_ComboboxEx.Create(&GetReBar());

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
	rbbi.lpText     = (LPTSTR)_T("Address");

	RB.InsertBand(-1, rbbi);
}

void CMainFrame::ConnectEvents()
{
	IUnknown* pUnk = GetBrowser().GetAXWindow().GetUnknown();
	if(!pUnk)
		return;

	IConnectionPoint* pcp;
	pcp = GetConnectionPoint(DIID_DWebBrowserEvents2);
	if(!pcp)
		return;

	pcp->Advise(&m_EventSink, &m_eventCookie);
	pcp->Release();
	pUnk->Release();
}

IConnectionPoint* CMainFrame::GetConnectionPoint(REFIID riid)
{
	IUnknown *pUnk = GetBrowser().GetAXWindow().GetUnknown();
	if(!pUnk)
		return NULL;

	IConnectionPointContainer* pcpc;
	IConnectionPoint* pcp = NULL;

	HRESULT hr = pUnk->QueryInterface(IID_IConnectionPointContainer, (void**)&pcpc);
	if(SUCCEEDED(hr))
	{
		pcpc->FindConnectionPoint(riid, &pcp);
		pcpc->Release();
	}

	pUnk->Release();

	return pcp;
}

void CMainFrame::OnBeforeNavigate2(DISPPARAMS* pDispParams)
{
	UNREFERENCED_PARAMETER(pDispParams);
}

void CMainFrame::OnCommandStateChange(DISPPARAMS* pDispParams)
{
	CToolBar& TB = GetToolBar();

	if (GetToolBar().IsWindow())
	{
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
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Respond to menu and and toolbar input

	CWnd* pEdit = m_ComboboxEx.GetEditCtrl();

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
	case IDM_EDIT_CUT:
		if (::GetFocus() == pEdit->GetHwnd())
			pEdit->SendMessage(WM_CUT, 0, 0);
		return TRUE;
	case IDM_EDIT_COPY:
		if (::GetFocus() == pEdit->GetHwnd())
			pEdit->SendMessage(WM_COPY, 0, 0);
		return TRUE;
	case IDM_EDIT_PASTE:
		if (::GetFocus() == pEdit->GetHwnd())
			pEdit->SendMessage(WM_PASTE, 0, 0);
		return TRUE;
	case IDM_EDIT_DELETE:
		if (::GetFocus() == pEdit->GetHwnd())
			pEdit->SendMessage(WM_CLEAR, 0, 0);
		return TRUE;
	case IDW_VIEW_STATUSBAR:
		OnViewStatusBar();
		return TRUE;
	case IDW_VIEW_TOOLBAR:
		OnViewToolBar();
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

	ConnectEvents();
}

void CMainFrame::OnDocumentComplete(DISPPARAMS* pDispParams)
{
	UNREFERENCED_PARAMETER(pDispParams);
	GetStatusBar().SetPartText(0, _T("Done"));
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
	tString szString = _T("NavigateComplete2: ");

	if (pDispParams->rgvarg[0].vt == (VT_BYREF|VT_VARIANT))
	{
		VARIANT vtURL;
		vtURL = *pDispParams->rgvarg[0].pvarVal;
		vtURL.vt = VT_BSTR;

		szString += W2T(vtURL.bstrVal);
		szString += _T("\n");
		TRACE(szString.c_str());
	}

	BSTR bstrUrlName;

	HRESULT hr = GetBrowser().GetIWebBrowser2()->get_LocationURL(&bstrUrlName);
	if (FAILED(hr))
		return;

	// Update the URL in the ComboboxEx edit box.
	m_ComboboxEx.SendMessage(WM_SETTEXT, 0, (LPARAM)(LPCTSTR)W2T(bstrUrlName));
}

void CMainFrame::OnNewWindow2(DISPPARAMS* pDispParams)
{
	UNREFERENCED_PARAMETER(pDispParams);
	TRACE(_T("NewWindow2\n"));
}

LRESULT CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam)
{
	HWND hwnd = m_ComboboxEx.GetHwnd();

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
		{
			szString << _T("Progress = ") << pDispParams->rgvarg[1].lVal ;
		}

		if (pDispParams->rgvarg[0].vt == VT_I4)
		{
			szString << _T(", ProgressMax = ") << pDispParams->rgvarg[0].lVal;
		}

		szString << _T("\n");
		TRACE(szString.str().c_str());
   }
}

void CMainFrame::OnPropertyChange(DISPPARAMS* pDispParams)
{
	tStringStream str;
	if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_BSTR)
		str << _T("Property Change:") << W2T(pDispParams->rgvarg[0].bstrVal);

	str << _T("\n");
	TRACE(str.str().c_str());
}

void CMainFrame::OnStatusTextChange(DISPPARAMS* pDispParams)
{
	LPOLESTR lpStatusText = pDispParams->rgvarg->bstrVal;


	if (GetStatusBar().IsWindow() && lpStatusText)
	{
		if (lstrcmp(W2T(lpStatusText), _T("")))
		{
			GetStatusBar().SetPartText(0, W2T(lpStatusText));
		}
		else
			GetStatusBar().SetPartText(0, _T("Done"));
	}

}

void CMainFrame::OnTitleChange(DISPPARAMS* pDispParams)
{
	TRACE(_T("TitleChange: \n"));
    tString str;

	if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_BSTR)
	{
        str = tString(W2T(pDispParams->rgvarg[0].bstrVal)) + _T(" - ") + LoadString(IDW_MAIN);
		TRACE(W2T(pDispParams->rgvarg[0].bstrVal));
	}
	else
		str = LoadString(IDW_MAIN);

	SetWindowText(str.c_str());
}

void CMainFrame::SetupToolBar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolBarButton( IDM_BACK   );
	AddToolBarButton( IDM_FORWARD );
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_REFRESH );
	AddToolBarButton( IDM_STOP );
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_HOME );

	// Set the image lists for normal, hot and disabled buttons
	int BitsPerPixel = GetDeviceCaps(GetDesktopWindow()->GetDC(), BITSPIXEL);
	if (GetWinVersion() >= 2501 && LoadBitmap(MAKEINTRESOURCE(IDB_TOOLBAR32_NORM)) && BitsPerPixel == 32)
	{
		// Use 32bit bitmaps for XP and above (support for 32bit bitmaps began with XP)
		SetToolBarImages(RGB(0,0,0), IDB_TOOLBAR32_NORM, IDB_TOOLBAR32_HOT, IDB_TOOLBAR32_DIS);
	}
	else
	{
		// Use 24bit bitmaps for Win2000 and below
		SetToolBarImages(RGB(255,0,255), IDB_TOOLBAR24_NORM, IDB_TOOLBAR24_HOT, IDB_TOOLBAR24_DIS);
	}

	std::vector<UINT> IconData;
	if (IsReBarUsed())
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

