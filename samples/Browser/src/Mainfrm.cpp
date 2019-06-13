////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame() : m_eventCookie(0)
{
    //Set m_View as the view window of the frame
    SetView(m_view);

    // Set CMainFrame as our event sink
    m_eventSink.SetSink(this);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\Browser Sample"));
}

CMainFrame::~CMainFrame()
{
    // Destructor for CMainFrame.
}

void CMainFrame::AddComboBoxBand(int height)
{
    // Create the ComboboxEx window
    m_comboboxEx.Create(GetReBar());

    // Put the window in a new rebar band
    REBARBANDINFO rbbi;
    ZeroMemory(&rbbi, sizeof(rbbi));
    rbbi.cbSize     = sizeof(rbbi);
    rbbi.fMask      = RBBIM_COLORS | RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_CHILD | RBBIM_TEXT;
    rbbi.cyMinChild = height;
    rbbi.cyMaxChild = height;
    rbbi.cxMinChild = 200;
    rbbi.fStyle     = RBBS_BREAK | RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS;
    rbbi.clrFore    = GetSysColor(COLOR_BTNTEXT);
    rbbi.clrBack    = GetReBarTheme().clrBand1;
    rbbi.hwndChild  = m_comboboxEx.GetHwnd();
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

    pcp->Advise(&m_eventSink, &m_eventCookie);
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
    CToolBar& tb = GetToolBar();

    if (tb.IsWindow())
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
                        bEnable ? tb.EnableButton(IDM_FORWARD) : tb.DisableButton(IDM_FORWARD);

                        break;
                    case 2: // Navigate back:
                        bEnable ? tb.EnableButton(IDM_BACK) : tb.DisableButton(IDM_BACK);
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
    m_view.GoBack();
    return TRUE;
}

BOOL CMainFrame::OnForward()
{
	m_view.GoForward();
    return TRUE;
}

BOOL CMainFrame::OnRefresh()
{
	m_view.Refresh();
    return TRUE;
}

BOOL CMainFrame::OnStop()
{
	m_view.Stop();
    return TRUE;
}

BOOL CMainFrame::OnHome()
{
	m_view.GoHome();
    return TRUE;
}

BOOL CMainFrame::OnEditCut()
{
    if (GetFocus() == *GetComboEdit())
        GetComboEdit()->Cut();
    else
		m_view.ExecWB( OLECMDID_CUT, OLECMDEXECOPT_DODEFAULT, NULL, NULL );

    return TRUE;
}

BOOL CMainFrame::OnEditCopy()
{
    if (GetFocus() == *GetComboEdit())
        GetComboEdit()->Copy();
    else
		m_view.ExecWB( OLECMDID_COPY, OLECMDEXECOPT_DODEFAULT, NULL, NULL );

    return TRUE;
}

BOOL CMainFrame::OnEditPaste()
{
    if (GetFocus() == *GetComboEdit())
        GetComboEdit()->Paste();
    else
		m_view.ExecWB( OLECMDID_PASTE, OLECMDEXECOPT_DODEFAULT, NULL, NULL );

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
		m_view.ExecWB( OLECMDID_DELETE, OLECMDEXECOPT_DODEFAULT, NULL, NULL );

    return TRUE;
}

BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    // Respond to menu and and toolbar input
    UINT id = LOWORD(wparam);  
    switch(id)
    {
    case IDM_FILE_EXIT:      return OnFileExit();
	case IDM_FILE_PREVIEW:   OnPrintPreview();     return TRUE;
	case IDM_FILE_PRINT:     OnPrint();            return TRUE;
    case IDM_HELP_ABOUT:     return OnHelpAbout();
    case IDM_BACK:           return OnBack();
    case IDM_FORWARD:        return OnForward();
    case IDM_REFRESH:        return OnRefresh();
    case IDM_STOP:           return OnStop();
    case IDM_HOME:           return OnHome();
    case IDM_EDIT_CUT:       return OnEditCut();
    case IDM_EDIT_COPY:      return OnEditCopy();
    case IDM_EDIT_PASTE:     return OnEditPaste();
    case IDM_EDIT_DELETE:    return OnEditDelete();
    case IDW_VIEW_STATUSBAR: return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:   return OnViewToolBar();
    }

    // Handle notification WM_COMMAND from ComboboxEx
    if(reinterpret_cast<HWND>(lparam) == m_comboboxEx.GetHwnd())
    {
        switch(HIWORD(wparam))
        {
        case CBN_SELCHANGE:
            // User made selection from list
            {
                // Get text from edit box
                CString str = m_comboboxEx.GetWindowText();

                // Navigate to web page
				m_view.Navigate(str);
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

	// A menu is added if the IDW_MAIN menu resource is defined.
	// Frames have all options enabled by default. 
	// Use the following functions to disable options.

    // UseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar
    // UseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar
    // UseReBar(FALSE);              // Don't use a ReBar
	// UseStatusBar(FALSE);          // Don't use a StatusBar
    // UseThemes(FALSE);             // Don't use themes
    // UseToolBar(FALSE);            // Don't use a ToolBar
   
    // Call the base function first
    CFrame::OnCreate(cs);

    // Add the combo box band to the rebar
    if (GetReBar().IsWindow())
    {
        int height = 22;
        AddComboBoxBand(height);
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

	m_view.GoHome();
}

void CMainFrame::OnNavigateComplete2(DISPPARAMS* pDispParams)
{
    CString str = _T("NavigateComplete2: ");

    if (pDispParams->rgvarg[0].vt == (VT_BYREF|VT_VARIANT))
    {
        VARIANT url;
		url = *pDispParams->rgvarg[0].pvarVal;
		url.vt = VT_BSTR;

        str += url.bstrVal;
        TRACE(str);
        VariantClear(&url);
    }

    BSTR urlName;

    HRESULT result = GetBrowser().GetIWebBrowser2()->get_LocationURL(&urlName);
    if (FAILED(result))
        return;

    // Update the URL in the ComboboxEx edit box.
    m_comboboxEx.SetWindowText(WtoT(urlName));
}

void CMainFrame::OnNewWindow2(DISPPARAMS* pDispParams)
{
    UNREFERENCED_PARAMETER(pDispParams);
    TRACE("NewWindow2\n");
}

LRESULT CMainFrame::OnNotify(WPARAM wparam, LPARAM lparam)
{
    switch (((LPNMHDR)lparam)->code)
    {
    case CBEN_ENDEDIT:
        {
            switch (((PNMCBEENDEDIT)lparam)->iWhy)
            {
            case CBENF_RETURN:
                // User hit return in edit box
                {
                    // Get text from edit box
                    CString str = m_comboboxEx.GetWindowText();

                    // Insert text into the list box.
                    COMBOBOXEXITEM item;
                    ZeroMemory(&item, sizeof(item));
					item.mask = CBEIF_TEXT;
					item.pszText = const_cast<LPTSTR>(str.c_str());
                    m_comboboxEx.InsertItem(item);

                    // Navigate to the web page
					m_view.Navigate(str);
                    return FALSE;
                }
            }
        }
    }

    return CFrame::OnNotify(wparam, lparam);
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
        str.Format(_T("Property Change: %s\n"), (LPCTSTR)WtoT(pDispParams->rgvarg[0].bstrVal));

    TRACE(str);
}

void CMainFrame::OnStatusTextChange(DISPPARAMS* pDispParams)
{
    LPOLESTR lpStatusText = pDispParams->rgvarg->bstrVal;

    if (GetStatusBar().IsWindow() && lpStatusText)
    {
        if (lstrcmp(WtoT(lpStatusText), _T("")))
        {
            GetStatusBar().SetPartText(0, WtoT(lpStatusText));
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
        TRACE(WtoT(pDispParams->rgvarg[0].bstrVal));
    }
    else
        str = LoadString(IDW_MAIN);

    SetWindowText(str);
}

void CMainFrame::OnPrintPreview()
{
	GetBrowser().ExecWB(OLECMDID_PRINTPREVIEW, OLECMDEXECOPT_DODEFAULT, NULL, NULL);
}

//The Open() and Navigate() Functions are here
void CMainFrame::OnPrint()
{
	SAFEARRAYBOUND psabBounds[1];
	SAFEARRAY *psaHeadFoot;
	HRESULT hr = S_OK;
	long rgIndices;

	VARIANT vHeadStr;
	VARIANT vFootStr;
	VARIANT vArg;
	ZeroMemory(&vHeadStr, sizeof(vHeadStr));
	ZeroMemory(&vFootStr, sizeof(vFootStr));
	ZeroMemory(&vArg, sizeof(vArg));


	try {
		// Initialize header and footer parameters to send to ExecWB().
		psabBounds[0].lLbound = 0;
		psabBounds[0].cElements = 3;
		psaHeadFoot = SafeArrayCreate(VT_VARIANT, 1, psabBounds);
		if (NULL == psaHeadFoot) 
			throw std::bad_alloc();

		VariantInit(&vHeadStr);
		VariantInit(&vFootStr);

		// Argument 1: Header
		vHeadStr.vt = VT_BSTR;
		vHeadStr.bstrVal = SysAllocString(L"This is my header string.");
		if (vHeadStr.bstrVal == NULL) throw 1001;

		// Argument 2: Footer
		vFootStr.vt = VT_BSTR;
		vFootStr.bstrVal = SysAllocString(L"This is my footer string.");
		if (vFootStr.bstrVal == NULL) throw 1002;

		rgIndices = 0;
		if (FAILED(SafeArrayPutElement(psaHeadFoot, &rgIndices, static_cast<void*>(&vHeadStr))))
			throw CUserException(_T("Failed ot store Header string"));
		rgIndices = 1;
		if (FAILED(SafeArrayPutElement(psaHeadFoot, &rgIndices, static_cast<void*>(&vFootStr))))
			throw CUserException(_T("Failed ot store Footer string"));

		// Issue the print command
		VariantInit(&vArg);
		vArg.vt = VT_ARRAY | VT_BYREF;
		vArg.parray = psaHeadFoot;
		GetBrowser().ExecWB(OLECMDID_PRINT, OLECMDEXECOPT_DONTPROMPTUSER, &vArg, NULL);
		if (FAILED(hr))
			throw CUserException(_T("Print Preview Failed"));
	}
	catch (...) {
		VariantClear(&vHeadStr);
		VariantClear(&vFootStr);
		if (psaHeadFoot)      SafeArrayDestroy(psaHeadFoot);
	}
}


void CMainFrame::SetupMenuIcons()
{
    // Add menu icons from the IDW_MAIN bitmap resource
    
    std::vector<UINT> iconData;     // a vector of Resource IDs
    if (GetReBar().IsWindow())
    {
        // Load the Resource IDs for popup menu items
		iconData.push_back(IDM_FILE_NEW);
		iconData.push_back(IDM_FILE_OPEN);
		iconData.push_back(IDM_FILE_SAVE);
		iconData.push_back(IDM_EDIT_CUT);
		iconData.push_back(IDM_EDIT_COPY);
		iconData.push_back(IDM_EDIT_PASTE);
		iconData.push_back(IDM_FILE_PRINT);
		iconData.push_back(IDM_HELP_ABOUT);
    }

    AddMenuIcons(iconData, RGB(192, 192, 192), IDW_MAIN, 0);

}

void CMainFrame::SetupToolBar()
{
    // Set the Resource IDs for the toolbar buttons
    AddToolBarButton( IDM_BACK   );
    AddToolBarButton( IDM_FORWARD );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_REFRESH );
    AddToolBarButton( IDM_STOP );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_HOME );

    // Set the image lists for normal, hot and disabled buttons
    int bitsPerPixel = GetDesktopWindow().GetDC().GetDeviceCaps(BITSPIXEL);
    if (GetWinVersion() >= 2501 && bitsPerPixel == 32)
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

LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
//  switch (msg)
//  {
        //Additional messages to be handled go here
//  }

    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}

