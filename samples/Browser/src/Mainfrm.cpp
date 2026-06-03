/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"
#include "UserMessages.h"

constexpr COLORREF black     = RGB(0, 0, 0);
constexpr COLORREF white     = RGB(255, 255, 255);
constexpr COLORREF lightgray = RGB(192, 192, 192);
constexpr COLORREF magenta   = RGB(255, 0, 255);

///////////////////////////////////////
// Definitions for the CMainFrame class
//

// Constructor
CMainFrame::CMainFrame()
{
}

// Adds a ComboBoxEx control to the rebar.
LRESULT CMainFrame::AddComboBoxBand()
{
    // Destroy and re-create the ComboboxEx window.
    m_combo.Destroy();
    m_combo.Create(GetReBar());
    m_combo.RestoreItems();
    GetCBEdit().SetWindowText(m_selectedItem);

    int padding = 2;
    int height = m_combo.GetWindowRect().Height() + DpiScaleInt(padding);

    // Put the window in a new rebar band.
    REBARBANDINFO rbbi{};
    rbbi.cbSize     = sizeof(rbbi);
    rbbi.fMask      = RBBIM_COLORS | RBBIM_CHILDSIZE | RBBIM_STYLE | RBBIM_CHILD | RBBIM_TEXT;
    rbbi.cyMinChild = height;
    rbbi.cyMaxChild = height;
    rbbi.cxMinChild = 200;
    rbbi.fStyle     = RBBS_BREAK | RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS;
    rbbi.clrFore    = GetSysColor(COLOR_BTNTEXT);
    rbbi.clrBack    = GetReBarTheme().clrBand1;
    rbbi.hwndChild  = m_combo.GetHwnd();
    rbbi.lpText     = const_cast<LPWSTR>(L"Address");

    GetReBar().InsertBand(-1, rbbi);
    return 0;
}

// Adds a string to the ComboBoxEx control if it is not already present.
void CMainFrame::AddStringToCombo(LPCWSTR str)
{
    int index = m_combo.FindStringExact(-1, str);
    if (index == CB_ERR)
    {
        // Add the string to the list box if it is not already there.
        COMBOBOXEXITEM item = {};
        item.mask = CBEIF_TEXT;
        item.pszText = const_cast<LPWSTR>(str);
        m_combo.InsertItem(item);
    }
}

// Create the main frame window, and set the browser as the view window.
HWND CMainFrame::Create(HWND parent)
{
    // Set m_view as the view window of the frame.
    SetView(m_browser);

    // Set the registry key name, and load the initial window position.
    // Use a registry key name like "CompanyName\\Application".
    LoadRegistrySettings(L"Win32++\\Browser Sample");

    return CFrame::Create(parent);
}

// Go to the previous web page.
BOOL CMainFrame::OnBack()
{
    m_browser.GoBack();
    return TRUE;
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
    case IDM_HELP_ABOUT:     return OnHelp();
    case IDM_HOME:           return OnHome();
    case IDM_REFRESH:        return OnReload();
    case IDM_STOP:           return OnStop();
    case IDW_VIEW_STATUSBAR: return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:   return OnViewToolBar();

    default: break;
    }

    // Handle notification WM_COMMAND from ComboboxEx.
    if (reinterpret_cast<HWND>(lparam) == m_combo.GetHwnd())
    {
        switch (HIWORD(wparam))
        {
        case CBN_SELCHANGE:   // User made selection from list.
        {
            // Get text from edit box.
            CString str = m_combo.GetWindowText();

            // Navigate to web page.
            Navigate(str);
            return TRUE;
        }

        default: break;
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

    // UseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar.
    // UseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar.
    // UseOwnerDrawnMenu(FALSE);     // Don't use owner draw for popup menu items.
    // UseReBar(FALSE);              // Don't use a ReBar.
    // UseStatusBar(FALSE);          // Don't use a StatusBar.
    // UseThemes(FALSE);             // Don't use themes.
    // UseToolBar(FALSE);            // Don't use a ToolBar.

    // Call the base function
    int result = CFrame::OnCreate(cs);

    // Add a ComboBoxEx control to the rebar.
    AddComboBoxBand();

    return result;
}

// Respond to a change in DPI. This function is only called when using
// Per Monitor DPI Awareness, version 2. This is specified by loading
// the appropriate manifest in the resource script (Resource.rc).
LRESULT CMainFrame::OnDpiChanged(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Note: We destroy and recreate the ComboBoxEx control when the DPI changes.
    // This turns out to be simpler and more reliable than modifying the control.

    // Save the current ComboBoxEx edit control text, and the list of items in the
    // ComboBoxEx before the DPI change.
    m_selectedItem = GetCBEdit().GetWindowText();
    m_combo.SaveItems();

    // Delete the rebar band holding the combobox.
    if (GetReBar().IsWindow())
    {
        int band = GetReBar().GetBand(m_combo);
        GetReBar().DeleteBand(band);
    }

    // Call the base class function. This recreates the toolbars.
    CFrame::OnDpiChanged(msg, wparam, lparam);

    // Add the ComboBoxEx control to the rebar after the DPI change.
    AddComboBoxBand();

    return 0;
}

// Cuts the selected text from the ComboBoxEx controlto the clip board.
// Called in response to the menu or accelerator key.
BOOL CMainFrame::OnEditCut()
{
    if (GetFocus() == GetCBEdit().GetHwnd())
        GetCBEdit().Cut();

    return TRUE;
}

// Copies the selected text from the ComboBoxEx control to the clip board.
// Called in response to the menu or accelerator key.
BOOL CMainFrame::OnEditCopy()
{
    if (GetFocus() == GetCBEdit().GetHwnd())
        GetCBEdit().Copy();

    return TRUE;
}

// Pastes text from the clip board to the ComboBoxEx control.
// Called in response to the menu or accelerator key.
BOOL CMainFrame::OnEditPaste()
{
    if (GetFocus() == GetCBEdit().GetHwnd())
        GetCBEdit().Paste();

    return TRUE;
}

// Deletes the selected text or the next character from the ComboBoxEx control.
// Called in response to the menu or accelerator key.
BOOL CMainFrame::OnEditDelete()
{
    if (GetFocus() == GetCBEdit().GetHwnd())
        GetCBEdit().Clear();

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
BOOL CMainFrame::OnHelp()
{
    // Ensure only one dialog displayed even for multiple hits of the F1 button.
    if (!m_aboutDialog.IsWindow())
    {
        m_aboutDialog.DoModal(*this);
    }

    return TRUE;
}

// Limit the minimum size of the window.
LRESULT CMainFrame::OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam)
{
    LPMINMAXINFO lpMMI = (LPMINMAXINFO)lparam;
    const CSize minimumSize(600, 400);
    lpMMI->ptMinTrackSize.x = DpiScaleInt(minimumSize.cx);
    lpMMI->ptMinTrackSize.y = DpiScaleInt(minimumSize.cy);
    return FinalWindowProc(msg, wparam, lparam);
}

// Load the browser's home page.
BOOL CMainFrame::OnHome()
{
    CString str = "https://www.google.com";
    Navigate(str);
    return TRUE;
}

// Called after the frame window is created.
void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    m_browser.StartBrowser();
}

// Called when the frame receives a notification (WM_NOTIFY).
LRESULT CMainFrame::OnNotify(WPARAM wparam, LPARAM lparam)
{
    LPNMHDR pHeader = reinterpret_cast<LPNMHDR>(lparam);
    if (pHeader != 0 && pHeader->hwndFrom == m_combo)
    {
        if (pHeader->code == CBEN_ENDEDIT)
        {
            PNMCBEENDEDIT pEndEdit = reinterpret_cast<PNMCBEENDEDIT>(pHeader);
            if (pEndEdit->iWhy == CBENF_RETURN)   // User hit return in edit box
            {
                // Get text from edit box
                CString str = m_combo.GetWindowText();

                // Navigate to the web page
                Navigate(str);
                return 0;
            }
        }
    }

    return CFrame::OnNotify(wparam, lparam);
}

// Reload the current web page.
BOOL CMainFrame::OnReload()
{
    m_browser.Reload();
    return TRUE;
}

// Stop loading the current web page.
BOOL CMainFrame::OnStop()
{
    m_browser.Stop();
    return TRUE;
}

// Navigate to the specified web page.
void CMainFrame::Navigate(LPCWSTR str)
{
    m_browser.Navigate(str);
}

// Add menu icons from the IDW_MAIN bitmap resource.
void CMainFrame::SetupMenuIcons()
{
    std::vector<UINT> iconData;     // a vector of Resource IDs

    // Load the Resource IDs for popup menu items.
    iconData.push_back(IDM_EDIT_CUT);
    iconData.push_back(IDM_EDIT_COPY);
    iconData.push_back(IDM_EDIT_PASTE);
    iconData.push_back(IDM_HELP_ABOUT);

    if (GetMenuIconHeight() >= 24)
        AddMenuIcons(iconData, lightgray, IDB_MENUICONS24, 0);
    else
        AddMenuIcons(iconData, lightgray, IDB_MENUICONS16, 0);
}

// Assigns images and command IDs to the toolbar buttons.
void CMainFrame::SetupToolBar()
{
    // Note: The toolbar is destroyed and recreated when the DPI changes when
    // using Per Monitor DPI Awareness.
    // This function is called when the toobar is created.

    // Set the resource IDs for the toolbar buttons.
    AddToolBarButton(IDM_BACK, FALSE);    // Initially disabled
    AddToolBarButton(IDM_FORWARD, FALSE); // Initially disabled
    AddToolBarButton(0);                  // Separator
    AddToolBarButton(IDM_REFRESH);
    AddToolBarButton(IDM_STOP);
    AddToolBarButton(0);                  // Separator
    AddToolBarButton(IDM_HOME);

    // Load the 32bit bitmaps.
    SetToolBarImages(black, IDB_TOOLBAR32_NORM, IDB_TOOLBAR32_HOT, IDB_TOOLBAR32_DIS);
}

// Called when the browser's history changes, to enable or disable the Back
// and Forward buttons.
LRESULT CMainFrame::OnHistoryChanged(UINT, WPARAM, LPARAM)
{
    BOOL canGoBack = m_browser.CanGoBack();
    BOOL canGoForward = m_browser.CanGoForward();

    GetToolBar().EnableButton(IDM_BACK, canGoBack);
    GetToolBar().EnableButton(IDM_FORWARD, canGoForward);

    TRACE("History changed. CanGoBack: " + ToCString(canGoBack));
    TRACE(", CanGoForward: " + ToCString(canGoForward) + "\n");
    return 0;
}

// Called when the browser completes navigation to a new page.
LRESULT CMainFrame::OnNavigationCompleted(UINT, WPARAM wparam, LPARAM)
{
    bool success = (wparam != 0);
    if (success)
        TRACE("Navigation completed successfully.\n");
    else
        TRACE("Navigation failed.\n");

    return 0;
}

// Called when the browser starts navigation to a new page.
LRESULT CMainFrame::OnNavigationStarted(UINT, WPARAM, LPARAM)
{
    TRACE("Navigation started.\n");

    return 0;
}

// Called when the browser's source changes, to update the ComboBoxEx control.
LRESULT CMainFrame::OnSourceChanged(UINT, WPARAM, LPARAM lparam)
{
    // Get the new source from lparam, and add it to the ComboBoxEx control.
    CString str = reinterpret_cast<LPCWSTR>(lparam);
    AddStringToCombo(str);
    m_combo.SetWindowText(str);

    // Unselect any text in the ComboBoxEx edit control.
    GetCBEdit().SetSel(-1, -1);

    TRACE("Source changed to: " + str + "\n");

    return 0;
}

// Override WndProc to handle user-defined messages from the browser.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_GETMINMAXINFO:        return OnGetMinMaxInfo(msg, wparam, lparam);
        case UWM_HISTORYCHANGED:      return OnHistoryChanged(msg, wparam, lparam);
        case UWM_NAVIGATIONCOMPLETED: return OnNavigationCompleted(msg, wparam, lparam);
        case UWM_NAVIGATIONSTARTED:   return OnNavigationStarted(msg, wparam, lparam);
        case UWM_SOURCECHANGED:       return OnSourceChanged(msg, wparam, lparam);

        default: break;

        } // switch msg


        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << L"Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    }

    return 0;
}
