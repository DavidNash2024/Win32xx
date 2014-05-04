////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "resource.h"
#include "mainfrm.h"
#include <shlobj.h>
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
	// Create the ComboboxEx window
	m_ComboboxEx.Create(GetReBar());

	// Put the window in a new rebar band
	REBARBANDINFO rbbi = {0};
	rbbi.cbSize		= sizeof(REBARBANDINFO);
	rbbi.fMask      = RBBIM_COLORS | RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_CHILD | RBBIM_TEXT;
	rbbi.cyMinChild = Listbox_Height;
	rbbi.cyMaxChild = Listbox_Height;
	rbbi.cxMinChild = 200;
	rbbi.fStyle     = RBBS_BREAK | RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS;
	rbbi.clrFore    = GetSysColor(COLOR_BTNTEXT);
	rbbi.clrBack    = GetReBarTheme()->clrBand1;
	rbbi.hwndChild  = m_ComboboxEx.GetHwnd();
	rbbi.lpText     = (LPTSTR)_T("Address");

	GetReBar()->InsertBand(-1, rbbi);
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
	CToolBar* pTB = GetToolBar();

	if (pTB->IsWindow())
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
						bEnable ? pTB->EnableButton(IDM_FORWARD) : pTB->DisableButton(IDM_FORWARD);

						break;
					case 2: // Navigate back:
						bEnable ? pTB->EnableButton(IDM_BACK) : pTB->DisableButton(IDM_BACK);
						break;
					}
				}
			}
		}
	}
}

void CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
}

void CMainFrame::OnHelpAbout()
{
	// Display the help dialog
	OnHelp();
}

void CMainFrame::OnBack()
{
	m_View.GoBack();
}

void CMainFrame::OnForward()
{
	m_View.GoForward();
}

void CMainFrame::OnRefresh()
{
	m_View.Refresh();
}

void CMainFrame::OnStop()
{
	m_View.Stop();
}

void CMainFrame::OnHome()
{
	m_View.GoHome();
}

void CMainFrame::OnEditCut()
{
	if (GetFocus() == GetComboEdit())
		GetComboEdit()->Cut();
	else
		m_View.ExecWB( OLECMDID_CUT, OLECMDEXECOPT_DODEFAULT, NULL, NULL );
}

void CMainFrame::OnEditCopy()
{
	if (GetFocus() == GetComboEdit())
		GetComboEdit()->Copy();
	else
		m_View.ExecWB( OLECMDID_COPY, OLECMDEXECOPT_DODEFAULT, NULL, NULL );
}

void CMainFrame::OnEditPaste()
{
	if (GetFocus() == GetComboEdit())
		GetComboEdit()->Paste();
	else
		m_View.ExecWB( OLECMDID_PASTE, OLECMDEXECOPT_DODEFAULT, NULL, NULL );
}

void CMainFrame::OnEditDelete()
{
#if defined(__GNUC__)
	OLECMDID OLECMDID_DELETE = (OLECMDID)33;
#endif

	if (GetFocus() == GetComboEdit())
		GetComboEdit()->Clear();
	else
		m_View.ExecWB( OLECMDID_DELETE, OLECMDEXECOPT_DODEFAULT, NULL, NULL );
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Respond to menu and and toolbar input
	switch(LOWORD(wParam))
	{
	case IDM_FILE_EXIT:		 OnFileExit();		return TRUE;
	case IDM_HELP_ABOUT:	 OnHelpAbout();		return TRUE;
	case IDM_BACK:			 OnBack();			return TRUE;
	case IDM_FORWARD:		 OnForward();		return TRUE;
	case IDM_REFRESH:		 OnRefresh();		return TRUE;
	case IDM_STOP:			 OnStop();			return TRUE;
	case IDM_HOME:			 OnHome();			return TRUE;
	case IDM_EDIT_CUT:		 OnEditCut();		return TRUE;
	case IDM_EDIT_COPY:		 OnEditCopy();		return TRUE;
	case IDM_EDIT_PASTE:	 OnEditPaste();		return TRUE;
	case IDM_EDIT_DELETE:	 OnEditDelete();	return TRUE;
	case IDW_VIEW_STATUSBAR: OnViewStatusBar(); return TRUE;
	case IDW_VIEW_TOOLBAR:	 OnViewToolBar(); 	return TRUE;
	}

	// Handle notification WM_COMMAND from ComboboxEx
	if((HWND)lParam == m_ComboboxEx.GetHwnd())
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

int CMainFrame::OnCreate(LPCREATESTRUCT pcs)
{
	// m_bUseIndicatorStatus = FALSE;	// Don't show keyboard indicators in the StatusBar
	// m_bUseMenuStatus = FALSE;		// Don't show menu descriptions in the StatusBar
	// m_bUseReBar = FALSE;				// Don't use a ReBar
	// m_bUseThemes = FALSE;            // Don't use themes
	// m_bUseToolBar = FALSE;			// Don't use a ToolBar
	
	// Call the base function first
	CFrame::OnCreate(pcs);

	ConnectEvents();
	return 0;
}

void CMainFrame::OnDocumentComplete(DISPPARAMS* pDispParams)
{
	UNREFERENCED_PARAMETER(pDispParams);
	GetStatusBar()->SetPartText(0, _T("Done"));
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
					COMBOBOXEXITEM CBXitem = {0};
					CBXitem.mask = CBEIF_TEXT;
					CBXitem.pszText = (LPTSTR)str.c_str();
					m_ComboboxEx.InsertItem(&CBXitem);

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

	if (GetStatusBar()->IsWindow() && lpStatusText)
	{
		if (lstrcmp(W2T(lpStatusText), _T("")))
		{
			GetStatusBar()->SetPartText(0, W2T(lpStatusText));
		}
		else
			GetStatusBar()->SetPartText(0, _T("Done"));
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
    int BitsPerPixel = GetDesktopWindow()->GetDC()->GetDeviceCaps(BITSPIXEL);
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

