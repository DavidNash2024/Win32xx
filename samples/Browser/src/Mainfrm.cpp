////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "resource.h"
#include "mainfrm.h"


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
	// Create the ComboboxEx window
	m_ComboboxEx.Create(GetReBar());

	// Put the window in a new rebar band
	REBARBANDINFO rbbi;
	ZeroMemory(&rbbi, sizeof(REBARBANDINFO));
	rbbi.cbSize		= sizeof(REBARBANDINFO);
	rbbi.fMask      = RBBIM_COLORS | RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_CHILD | RBBIM_TEXT;
	rbbi.cyMinChild = Listbox_Height;
	rbbi.cyMaxChild = Listbox_Height;
	rbbi.cxMinChild = 200;
	rbbi.fStyle     = RBBS_BREAK | RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS;
	rbbi.clrFore    = GetSysColor(COLOR_BTNTEXT);
	rbbi.clrBack    = GetReBarTheme().clrBand1;
	rbbi.hwndChild  = m_ComboboxEx.GetHwnd();
	rbbi.lpText     = const_cast<LPTSTR>(_T("Address"));

	GetReBar().InsertBand(-1, rbbi);
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

	if (TB.IsWindow())
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

BOOL CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
	return TRUE;
}

BOOL CMainFrame::OnHelpAbout()
{
	// Display the help dialog
	OnHelp();
	return TRUE;
}

BOOL CMainFrame::OnBack()
{
	m_View.GoBack();
	return TRUE;
}

BOOL CMainFrame::OnForward()
{
	m_View.GoForward();
	return TRUE;
}

BOOL CMainFrame::OnRefresh()
{
	m_View.Refresh();
	return TRUE;
}

BOOL CMainFrame::OnStop()
{
	m_View.Stop();
	return TRUE;
}

BOOL CMainFrame::OnHome()
{
	m_View.GoHome();
	return TRUE;
}

BOOL CMainFrame::OnEditCut()
{
	if (GetFocus() == *GetComboEdit())
		GetComboEdit()->Cut();
	else
		m_View.ExecWB( OLECMDID_CUT, OLECMDEXECOPT_DODEFAULT, NULL, NULL );

	return TRUE;
}

BOOL CMainFrame::OnEditCopy()
{
	if (GetFocus() == *GetComboEdit())
		GetComboEdit()->Copy();
	else
		m_View.ExecWB( OLECMDID_COPY, OLECMDEXECOPT_DODEFAULT, NULL, NULL );

	return TRUE;
}

BOOL CMainFrame::OnEditPaste()
{
	if (GetFocus() == *GetComboEdit())
		GetComboEdit()->Paste();
	else
		m_View.ExecWB( OLECMDID_PASTE, OLECMDEXECOPT_DODEFAULT, NULL, NULL );

	return TRUE;
}

BOOL CMainFrame::OnEditDelete()
{
#if defined(__GNUC__)
	OLECMDID OLECMDID_DELETE = (OLECMDID)33;
#endif

	if (GetFocus() == *GetComboEdit())
		GetComboEdit()->Clear();
	else
		m_View.ExecWB( OLECMDID_DELETE, OLECMDEXECOPT_DODEFAULT, NULL, NULL );

	return TRUE;
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Respond to menu and and toolbar input
	switch(LOWORD(wParam))
	{
	case IDM_FILE_EXIT:		 return OnFileExit();
	case IDM_HELP_ABOUT:	 return OnHelpAbout();
	case IDM_BACK:			 return OnBack();
	case IDM_FORWARD:		 return OnForward();
	case IDM_REFRESH:		 return OnRefresh();
	case IDM_STOP:			 return OnStop();
	case IDM_HOME:			 return OnHome();
	case IDM_EDIT_CUT:		 return OnEditCut();
	case IDM_EDIT_COPY:		 return OnEditCopy();
	case IDM_EDIT_PASTE:	 return OnEditPaste();
	case IDM_EDIT_DELETE:	 return OnEditDelete();
	case IDW_VIEW_STATUSBAR: return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:	 return OnViewToolBar();
	}

	// Handle notification WM_COMMAND from ComboboxEx
	if(reinterpret_cast<HWND>(lParam) == m_ComboboxEx.GetHwnd())
	{
		switch(HIWORD(wParam))
		{
		case CBN_SELCHANGE:
			// User made selection from list
			{
				// Get text from edit box
				CString str = m_ComboboxEx.GetWindowText();

				// Navigate to web page
				m_View.Navigate(str);
			}
			return TRUE;
		}
	}

	return FALSE;
}

void CMainFrame::OnDocumentBegin(DISPPARAMS* pDispParams)
{
	UNREFERENCED_PARAMETER(pDispParams);
	TRACE("OnDocumentBegin\n");
}

int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::OnCreate is optional.
	// Uncomment the lines below to change frame options.

	// SetUseIndicatorStatus(FALSE);	// Don't show keyboard indicators in the StatusBar
	// SetUseMenuStatus(FALSE);			// Don't show menu descriptions in the StatusBar
	// SetUseReBar(FALSE);				// Don't use a ReBar
	// SetUseThemes(FALSE);				// Don't use themes
	// SetUseToolBar(FALSE);			// Don't use a ToolBar
	
	// Call the base function first
	CFrame::OnCreate(cs);

	// Add the combo box band to the rebar
	if (IsReBarUsed())
	{
		int Height = 22;
		AddComboBoxBand(Height);
	}

	ConnectEvents();
	return 0;
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

	m_View.GoHome();
}

void CMainFrame::OnNavigateComplete2(DISPPARAMS* pDispParams)
{
	CString str = _T("NavigateComplete2: ");

	if (pDispParams->rgvarg[0].vt == (VT_BYREF|VT_VARIANT))
	{
		VARIANT vtURL;
		vtURL = *pDispParams->rgvarg[0].pvarVal;
		vtURL.vt = VT_BSTR;

		str += vtURL.bstrVal;
		TRACE(str);
		VariantClear(&vtURL);
	}

	BSTR bstrUrlName;

	HRESULT hr = GetBrowser().GetIWebBrowser2()->get_LocationURL(&bstrUrlName);
	if (FAILED(hr))
		return;

	// Update the URL in the ComboboxEx edit box.
	m_ComboboxEx.SetWindowText(W2T(bstrUrlName));
	m_View.SetFocus();
}

void CMainFrame::OnNewWindow2(DISPPARAMS* pDispParams)
{
	UNREFERENCED_PARAMETER(pDispParams);
	TRACE("NewWindow2\n");
}

LRESULT CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam)
{
	switch (((LPNMHDR)lParam)->code)
	{
	case CBEN_ENDEDIT:
		{
			switch (((PNMCBEENDEDIT)lParam)->iWhy)
			{
			case CBENF_RETURN:
				// User hit return in edit box
				{
					// Get text from edit box
					CString str = m_ComboboxEx.GetWindowText();

					// Insert text into the list box.
					COMBOBOXEXITEM CBXitem;
					ZeroMemory(&CBXitem, sizeof(COMBOBOXEXITEM));
					CBXitem.mask = CBEIF_TEXT;
					CBXitem.pszText = const_cast<LPTSTR>(str.c_str());
					m_ComboboxEx.InsertItem(CBXitem);

					// Navigate to the web page
					m_View.Navigate(str);
					return FALSE;
				}
			}
		}
	}

	return CFrame::OnNotify(wParam, lParam);
}

void CMainFrame::OnProgressChange(DISPPARAMS* pDispParams)
{
	CString str;

	if (pDispParams->cArgs != 0)
	{
		if (pDispParams->cArgs > 1 && pDispParams->rgvarg[1].vt == VT_I4)
		{
			str.Format(_T("Progress = %d"), pDispParams->rgvarg[1].lVal);
		}

		if (pDispParams->rgvarg[0].vt == VT_I4)
		{
			CString str2;
			str2.Format(_T(", ProgressMax = %d\n"), pDispParams->rgvarg[0].lVal);
			str = str + str2;
		}

		TRACE(str);
   }
}

void CMainFrame::OnPropertyChange(DISPPARAMS* pDispParams)
{
	CString str;
	if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_BSTR)
		str.Format(_T("Property Change: %s\n"), (LPCTSTR)W2T(pDispParams->rgvarg[0].bstrVal));

	TRACE(str);
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
	TRACE("TitleChange: \n");
    CString str;

	if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_BSTR)
	{
        str = CString(pDispParams->rgvarg[0].bstrVal) + _T(" - ") + LoadString(IDW_MAIN);
		TRACE(W2T(pDispParams->rgvarg[0].bstrVal));
	}
	else
		str = LoadString(IDW_MAIN);

	SetWindowText(str);
}

void CMainFrame::SetupMenuIcons()
{
	// Add menu icons from the IDW_MAIN bitmap resource
	
	std::vector<UINT> IconData;		// a vector of Resource IDs
	if (IsReBarUsed())
	{
		// Load the Resource IDs for popup menu items
		IconData.push_back(IDM_FILE_NEW);
		IconData.push_back(IDM_FILE_OPEN);
		IconData.push_back(IDM_FILE_SAVE);
		IconData.push_back(IDM_EDIT_CUT);
		IconData.push_back(IDM_EDIT_COPY);
		IconData.push_back(IDM_EDIT_PASTE);
		IconData.push_back(IDM_FILE_PRINT);
		IconData.push_back(IDM_HELP_ABOUT);
	}

	AddMenuIcons(IconData, RGB(192, 192, 192), IDW_MAIN, 0);

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
    int BitsPerPixel = GetDesktopWindow().GetDC().GetDeviceCaps(BITSPIXEL);
	if (GetWinVersion() >= 2501 && BitsPerPixel == 32)
	{
		// Load the 32bit bitmaps if we can, otherwise load 24bit ones.
		CBitmap bm(IDB_TOOLBAR32_NORM);
		if (bm.GetHandle())
			SetToolBarImages(RGB(0,0,0), IDB_TOOLBAR32_NORM, IDB_TOOLBAR32_HOT, IDB_TOOLBAR32_DIS);
		else
			SetToolBarImages(RGB(255,0,255), IDB_TOOLBAR24_NORM, IDB_TOOLBAR24_HOT, IDB_TOOLBAR24_DIS);
	}
	else
	{
		// Use 24bit bitmaps for Win2000 and below
		SetToolBarImages(RGB(255,0,255), IDB_TOOLBAR24_NORM, IDB_TOOLBAR24_HOT, IDB_TOOLBAR24_DIS);
	}

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

