/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"
#include "UserMessages.h"

///////////////////////////////////////
// Definitions for the CMainFrame class
//

// Constructor
CMainFrame::CMainFrame() : m_eventCookie(0)
{
}

// Destructor.
CMainFrame::~CMainFrame()
{
}

// Adds a ComboBoxEx control to the rebar.
void CMainFrame::AddComboBoxBand(UINT height)
{
    // Create the ComboboxEx window.
    m_combo.Create(GetReBar());

    // Put the window in a new rebar band.
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
    rbbi.hwndChild  = m_combo.GetHwnd();
    rbbi.lpText     = const_cast<LPTSTR>(_T("Address"));

    GetReBar().InsertBand(-1, rbbi);
}

// Connect to the event sink.
void CMainFrame::ConnectEvents()
{
    IConnectionPoint* pcp = NULL;
    pcp = GetConnectionPoint(DIID_DWebBrowserEvents2);
    if (pcp)
    {
        VERIFY(SUCCEEDED(pcp->Advise(&m_eventSink, &m_eventCookie)));
        pcp->Release();
    }
}

HWND CMainFrame::Create(HWND parent)
{
    //Set m_View as the view window of the frame
    SetView(m_browser);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\Browser Sample"));

    return CFrame::Create(parent);
}

// Disconnect from the event sink.
void CMainFrame::DisconnectEvents()
{
    IConnectionPoint* pcp = NULL;
    pcp = GetConnectionPoint(DIID_DWebBrowserEvents2);
    if (pcp)
    {
        VERIFY(SUCCEEDED(pcp->Unadvise(m_eventCookie)));
        pcp->Release();
    }
}

// Retrieve pointer to IConnectionPoint.
// Call Release on this pointer when it is no longer required.
IConnectionPoint* CMainFrame::GetConnectionPoint(REFIID riid) const
{
    IConnectionPoint* pcp = NULL;
    IUnknown* pUnk = NULL;
    GetIWebBrowser2()->QueryInterface(IID_IUnknown, (void**)&pUnk);
    if (pUnk)
    {
        IConnectionPointContainer* pcpc = NULL;
        VERIFY(SUCCEEDED(pUnk->QueryInterface(IID_IConnectionPointContainer, (void**)&pcpc)));
        if (pcpc)
        {
            VERIFY(SUCCEEDED(pcpc->FindConnectionPoint(riid, &pcp)));
            pcpc->Release();
        }

        pUnk->Release();
    }

    return pcp;
}

// Go to the previous web page.
BOOL CMainFrame::OnBack()
{
    m_browser.GoBack();
    return TRUE;
}

// Called before navigation occurs on either a window or frameset element.
void CMainFrame::OnBeforeNavigate2(DISPPARAMS*)
{
}

// Called when the enabled state of a command changes.
void CMainFrame::OnCommandStateChange(DISPPARAMS* pDispParams)
{
    CToolBar& TB = GetToolBar();

    if ((pDispParams) && (pDispParams->cArgs == 2))
    {
        BOOL isEnabled = pDispParams->rgvarg[0].boolVal;
        LONG command = pDispParams->rgvarg[1].lVal;
        {
            // Update the Forward and Back buttons.
            switch (command)
            {
            case CSC_NAVIGATEFORWARD:
                isEnabled ? TB.EnableButton(IDM_FORWARD) : TB.DisableButton(IDM_FORWARD);
                break;
            case CSC_NAVIGATEBACK:
                isEnabled ? TB.EnableButton(IDM_BACK) : TB.DisableButton(IDM_BACK);
                break;
            }
        }
    }
}

// Called before the frame is destroyed.
void CMainFrame::OnClose()
{
    m_browser.Stop();
    DisconnectEvents();

    // Call the base class function.
    CFrame::OnClose();
}

// Respond to menu, toolbar or accelerator key input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UINT id = LOWORD(wparam);
    switch (id)
    {
    case IDM_BACK:           return OnBack();
    case IDM_EDIT_CUT:       return OnEditCut();
    case IDM_EDIT_COPY:      return OnEditCopy();
    case IDM_EDIT_PASTE:     return OnEditPaste();
    case IDM_EDIT_DELETE:    return OnEditDelete();
    case IDM_FILE_EXIT:      return OnFileExit();
    case IDM_FORWARD:        return OnForward();
    case IDM_HELP_ABOUT:     return OnHelpAbout();
    case IDM_HOME:           return OnHome();
    case IDM_FILE_PRINT:     return OnPrint();
    case IDM_FILE_PREVIEW:   return OnPrintPreview();
    case IDM_REFRESH:        return OnRefresh();
    case IDM_STOP:           return OnStop();
    case IDW_VIEW_STATUSBAR: return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:   return OnViewToolBar();
    }

    // Handle notification WM_COMMAND from ComboboxEx.
    if (reinterpret_cast<HWND>(lparam) == m_combo.GetHwnd())
    {
        switch (HIWORD(wparam))
        {
        case CBN_SELCHANGE:   // User made selection from list.
        {
            // Get text from edit box
            CString str = m_combo.GetWindowText();

            // Navigate to web page
            m_browser.Navigate(str);

            // Set focus to web page
            LONG_PTR hWeb;
            GetIWebBrowser2()->get_HWND(&hWeb);
            ::SetFocus((HWND)hWeb);
        }
        return TRUE;
        }
    }

    return FALSE;
}

// Called during the window's creation.
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

    // Call the base function
    CFrame::OnCreate(cs);

    ConnectEvents();

    return 0;
}

// Called when a document has been completely loaded and initialized.
void CMainFrame::OnDocumentComplete(DISPPARAMS*)
{
    SetStatusText(_T("Done"));
}

// Called when a navigation operation is beginning.
void CMainFrame::OnDownloadBegin(DISPPARAMS*)
{
}

// Called when a navigation operation finishes, is halted, or fails.
void CMainFrame::OnDownloadComplete(DISPPARAMS*)
{
}

// Deletes the selected text.
// Called in response to the menu or accelerator key.
BOOL CMainFrame::OnEditCut()
{
    if (GetFocus() == GetCBEdit()->GetHwnd())
        GetCBEdit()->Cut();
    else
        m_browser.ExecWB( OLECMDID_CUT, OLECMDEXECOPT_DODEFAULT, NULL, NULL );

    return TRUE;
}

// Copies the selected text to the clip board.
// Called in response to the menu or accelerator key.
BOOL CMainFrame::OnEditCopy()
{
    if (GetFocus() == GetCBEdit()->GetHwnd())
        GetCBEdit()->Copy();
    else
        m_browser.ExecWB( OLECMDID_COPY, OLECMDEXECOPT_DODEFAULT, NULL, NULL );

    return TRUE;
}

// Pastes text from the clip board.
// Called in response to the menu or accelerator key.
BOOL CMainFrame::OnEditPaste()
{
    if (GetFocus() == GetCBEdit()->GetHwnd())
        GetCBEdit()->Paste();
    else
        m_browser.ExecWB( OLECMDID_PASTE, OLECMDEXECOPT_DODEFAULT, NULL, NULL );

    return TRUE;
}

// Deletes the selected text or the next character.
// Called in response to the menu or accelerator key.
BOOL CMainFrame::OnEditDelete()
{
#if defined(__GNUC__)
    OLECMDID OLECMDID_DELETE = (OLECMDID)33;
#endif

    if (GetFocus() == GetCBEdit()->GetHwnd())
        GetCBEdit()->Clear();
    else
        m_browser.ExecWB( OLECMDID_DELETE, OLECMDEXECOPT_DODEFAULT, NULL, NULL );

    return TRUE;
}

// Issue a close request to the frame
BOOL CMainFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Go to the next web page.
BOOL CMainFrame::OnForward()
{
    m_browser.GoForward();
    return TRUE;
}

// Display the help about dialog.
BOOL CMainFrame::OnHelpAbout()
{
    OnHelp();
    return TRUE;
}

// Load the browser's home page.
BOOL CMainFrame::OnHome()
{
    m_browser.GoHome();
    return TRUE;
}

// Called after the frame window is created.
void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    // Suppress Java script errors.
    GetIWebBrowser2()->put_Silent(VARIANT_TRUE);

    // Load the home page
    m_browser.GoHome();
}

// Called when navigation completes on either a window or frameset element.
void CMainFrame::OnNavigateComplete2(DISPPARAMS* pDispParams)
{
    CString str = _T("Navigate Complete");

    if (pDispParams->rgvarg[0].vt == (VT_BYREF | VT_VARIANT))
    {
        VARIANT url = *pDispParams->rgvarg[0].pvarVal;
        url.vt = VT_BSTR;

        str += url.bstrVal;
        str += _T("\n");
        TRACE(str);
        VariantClear(&url);
    }

    str  = m_browser.GetLocationURL();

    // Update the URL in the ComboboxEx edit box.
    m_combo.SetWindowText(CString(str));
}

// Called when a new window is to be created.
void CMainFrame::OnNewWindow2(DISPPARAMS*)
{
    TRACE(_T("NewWindow2\n"));
}

// Called when the frame receives a notification (WM_NOTIFY).
LRESULT CMainFrame::OnNotify(WPARAM wparam, LPARAM lparam)
{
    LPNMHDR pHeader = reinterpret_cast<LPNMHDR>(lparam);

    if (pHeader != 0 && pHeader->hwndFrom == m_combo)
    {
        switch (pHeader->code)
        {
        case CBEN_ENDEDIT:
        {
            PNMCBEENDEDIT pEndEdit = reinterpret_cast<PNMCBEENDEDIT>(pHeader);
            switch (pEndEdit->iWhy)
            {
            case CBENF_RETURN:   // User hit return in edit box
            {
                // Get text from edit box
                CString str = m_combo.GetWindowText();

                // Insert text into the list box.
                COMBOBOXEXITEM item;
                ZeroMemory(&item, sizeof(item));
                item.mask = CBEIF_TEXT;
                item.pszText = const_cast<LPTSTR>(str.c_str());
                m_combo.InsertItem(item);

                // Navigate to the web page
                m_browser.Navigate(str);
                return FALSE;
            }
            }
        }
        }
    }

    return CFrame::OnNotify(wparam, lparam);
}

// Displays the web page as it would look when printed.
BOOL CMainFrame::OnPrintPreview()
{
    m_browser.ExecWB(OLECMDID_PRINTPREVIEW, OLECMDEXECOPT_DODEFAULT, NULL, NULL);
    return TRUE;
}

// Prints the web page to the default printer.
BOOL CMainFrame::OnPrint()
{
    SAFEARRAYBOUND psabBounds[1];
    SAFEARRAY* psaHeadFoot;
    long rgIndices;

    VARIANT vHeadStr;
    VARIANT vFootStr;
    VARIANT vArg;
    ZeroMemory(&vHeadStr, sizeof(vHeadStr));
    ZeroMemory(&vFootStr, sizeof(vFootStr));
    ZeroMemory(&vArg, sizeof(vArg));

    try
    {
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
        if (vHeadStr.bstrVal == NULL)
            throw std::bad_alloc();

        rgIndices = 0;
        if (SUCCEEDED(SafeArrayPutElement(psaHeadFoot, &rgIndices, static_cast<void*>(&vHeadStr))))
        {
            // Argument 2: Footer
            vFootStr.vt = VT_BSTR;
            vFootStr.bstrVal = SysAllocString(L"This is my footer string.");
            if (vFootStr.bstrVal == NULL)
                throw std::bad_alloc();

            rgIndices = 1;
            if (SUCCEEDED(SafeArrayPutElement(psaHeadFoot, &rgIndices, static_cast<void*>(&vFootStr))))
            {
                // Issue the print command
                VariantInit(&vArg);
                vArg.vt = VT_ARRAY | VT_BYREF;
                vArg.parray = psaHeadFoot;
                if (FAILED(m_browser.ExecWB(OLECMDID_PRINT, OLECMDEXECOPT_DONTPROMPTUSER, &vArg, NULL)))
                    throw CUserException(_T("Print Failed"));
            }
        }

        // Calls SysFreeString on each element.
        SafeArrayDestroy(psaHeadFoot);
    }
    catch (...)
    {
        if (psaHeadFoot)
            SafeArrayDestroy(psaHeadFoot);
    }

    return TRUE;
}

// Called when the progress of a download operation is updated on the object.
void CMainFrame::OnProgressChange(DISPPARAMS* pDispParams)
{
    CString str;

    if (pDispParams->cArgs != 0)
    {
        if (pDispParams->cArgs > 1 && pDispParams->rgvarg[1].vt == VT_I4)
        {
            int progress = pDispParams->rgvarg[1].lVal;
            str << _T("Progress = ") << progress;
        }

        if (pDispParams->rgvarg[0].vt == VT_I4)
        {
            int progressMax = pDispParams->rgvarg[0].lVal;
            str << _T(", ProgressMax = ") << progressMax;
        }

        str << _T("\n");
        TRACE(str);
    }
}

// Called when the IWebBrowser2::PutProperty method of the object changes the
// value of a property.
void CMainFrame::OnPropertyChange(DISPPARAMS* pDispParams)
{
    CString str;
    if (pDispParams->cArgs > 0 && pDispParams->rgvarg[0].vt == VT_BSTR)
    {
        str += _T("Property Change:");
        str += pDispParams->rgvarg[0].bstrVal;
    }

    str += _T("\n");
    TRACE(str);
}

// Reload the current web page.
BOOL CMainFrame::OnRefresh()
{
    m_browser.Refresh();
    return TRUE;
}

// Called when the status bar text of the object has changed.
void CMainFrame::OnStatusTextChange(DISPPARAMS* pDispParams)
{
    CString statusText = pDispParams->rgvarg->bstrVal;

    if (statusText != _T(""))
        SetStatusText(statusText);
    else
        SetStatusText(_T("Done"));
}

// Stop loading the current web page.
BOOL CMainFrame::OnStop()
{
    m_browser.Stop();
    return TRUE;
}

// Called when the title of a document in the object becomes available
// or changes.
void CMainFrame::OnTitleChange(DISPPARAMS* pDispParams)
{
    TRACE(_T("TitleChange: \n"));
    CString str;

    if ((pDispParams->cArgs > 0) && (pDispParams->rgvarg[0].vt == VT_BSTR))
    {
        str = pDispParams->rgvarg[0].bstrVal;
        str += _T(" - Win32++ Browser");
        TRACE(str + _T("\n"));
    }
    else
        str = LoadString(IDW_MAIN);

    SetWindowText(str);
}

// Add menu icons from the IDW_MAIN bitmap resource.
void CMainFrame::SetupMenuIcons()
{
    std::vector<UINT> iconData;     // a vector of Resource IDs

    // Load the Resource IDs for popup menu items.
    iconData.push_back(IDM_FILE_NEW);
    iconData.push_back(IDM_FILE_OPEN);
    iconData.push_back(IDM_FILE_SAVE);
    iconData.push_back(IDM_EDIT_CUT);
    iconData.push_back(IDM_EDIT_COPY);
    iconData.push_back(IDM_EDIT_PASTE);
    iconData.push_back(IDM_FILE_PRINT);
    iconData.push_back(IDM_HELP_ABOUT);

    if (GetMenuIconHeight() >= 24)
        AddMenuIcons(iconData, RGB(192, 192, 192), IDB_MENUICONS24, 0);
    else
        AddMenuIcons(iconData, RGB(192, 192, 192), IDB_MENUICONS16, 0);
}

// Set the Resource IDs for the toolbar buttons.
void CMainFrame::SetupToolBar()
{
    AddToolBarButton(IDM_BACK);
    AddToolBarButton(IDM_FORWARD);
    AddToolBarButton(0);                // Separator
    AddToolBarButton(IDM_REFRESH);
    AddToolBarButton(IDM_STOP);
    AddToolBarButton(0);                // Separator
    AddToolBarButton(IDM_HOME);

    // Set the image lists for normal, hot and disabled buttons.
    int bitsPerPixel = GetDesktopWindow().GetDC().GetDeviceCaps(BITSPIXEL);
    if (GetWinVersion() >= 2501 && bitsPerPixel == 32)
    {
        // Load the 32bit bitmaps if we can, otherwise load 24bit ones.
        CBitmap bm(IDB_TOOLBAR32_NORM);
        if (bm.GetHandle())
            SetToolBarImages(RGB(0, 0, 0), IDB_TOOLBAR32_NORM, IDB_TOOLBAR32_HOT, IDB_TOOLBAR32_DIS);
        else
            SetToolBarImages(RGB(255, 0, 255), IDB_TOOLBAR24_NORM, IDB_TOOLBAR24_HOT, IDB_TOOLBAR24_DIS);
    }
    else
    {
        // Use 24bit bitmaps for Win2000 and below.
        SetToolBarImages(RGB(255, 0, 255), IDB_TOOLBAR24_NORM, IDB_TOOLBAR24_HOT, IDB_TOOLBAR24_DIS);
    }

    // Add the ComboBoxEx control.
    UINT Height = 26;
    AddComboBoxBand(Height);
}

// Process frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case UWM_BEFORENAVIGATE:      OnBeforeNavigate2((DISPPARAMS*)wparam);    break;
        case UWM_COMMANDSTATECHANGE:  OnCommandStateChange((DISPPARAMS*)wparam); break;
        case UWM_DOCUMENTCOMPLETE:    OnDocumentComplete((DISPPARAMS*)wparam);   break;
        case UWM_DOWNLOADBEGIN:       OnDownloadBegin((DISPPARAMS*)wparam);      break;
        case UWM_NAVIGATECOMPLETE2:   OnNavigateComplete2((DISPPARAMS*)wparam);  break;
        case UWM_NEWWINDOW2:          OnNewWindow2((DISPPARAMS*)wparam);         break;
        case UWM_PROGRESSCHANGE:      OnProgressChange((DISPPARAMS*)wparam);     break;
        case UWM_PROPERTYCHANGE:      OnPropertyChange((DISPPARAMS*)wparam);     break;
        case UWM_STATUSTEXTCHANGE:    OnStatusTextChange((DISPPARAMS*)wparam);   break;
        case UWM_TITLECHANGE:         OnTitleChange((DISPPARAMS*)wparam);        break;
        }

        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}

