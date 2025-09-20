////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "resource.h"
#include "Mainfrm.h"

constexpr COLORREF lightgray = RGB(192, 192, 192);

//////////////////////////////////
// CMainFrame function definitions
//

// Constructor for CMainFrame.
CMainFrame::CMainFrame() : m_formView(IDD_DIALOG1)
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    // Set m_formView as the view window of the frame.
    SetView(m_formView);

    // Set the registry key name, and load the initial window position.
    // Use a registry key name like "CompanyName\\Application".
    LoadRegistrySettings(L"Win32++\\FormDemo");

    return CFrame::Create(parent);
}

// Loads the settings from the registry
BOOL CMainFrame::LoadRegistrySettings(LPCWSTR keyName)
{
    CFrame::LoadRegistrySettings(keyName);
    GetDoc().LoadSettings(GetRegistryKeyName().c_str());

    return TRUE;
}

// Respond to the toolbar and menu.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch(id)
    {
    case ID_CHECK_A:         return m_formView.OnCheckA();
    case ID_CHECK_B:         return m_formView.OnCheckB();
    case ID_CHECK_C:         return m_formView.OnCheckC();
    case IDM_FILE_EXIT:      return OnFileExit();
    case IDW_VIEW_STATUSBAR: return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:   return OnViewToolBar();
    case IDM_HELP_ABOUT:     return OnHelp();

    case ID_RADIO_A:
    case ID_RADIO_B:        // intentionally blank
    case ID_RADIO_C:        return m_formView.OnRangeOfIDs(ID_RADIO_A, ID_RADIO_C, id);

    case IDM_EDIT_COPY:
    {
        GetFocus().SendMessage(WM_COPY);
        return TRUE;
    }

    case IDM_EDIT_PASTE:
    {
        GetFocus().SendMessage(WM_PASTE);
        return TRUE;
    }
    }

  return FALSE;
}

// OnCreate controls the way the frame is created.
// Overriding CFrame::OnCreate is optional.
int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
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

    // call the base class function
    return CFrame::OnCreate(cs);
}

// Called after the frame window is created.
void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.
}

// Close the frame window to end the application.
BOOL CMainFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Update the check state of the various menu items
void CMainFrame::OnMenuUpdate(UINT id)
{
    switch (id)
    {
    case ID_CHECK_A:
        OnUpdateCheckA(id);
        break;
    case ID_CHECK_B:
        OnUpdateCheckB(id);
        break;
    case ID_CHECK_C:
        OnUpdateCheckC(id);
        break;
    }

    if ((id >= ID_RADIO_A) && (id <= ID_RADIO_C))
        OnUpdateRangeOfIDs(ID_RADIO_A, ID_RADIO_C, id);

    CFrame::OnMenuUpdate(id);
}

// Updates the Check A menu item.
void CMainFrame::OnUpdateCheckA(UINT id)
{
    bool isCheckA = (GetDoc().GetCheckA() != 0);
    UINT check = isCheckA ? MF_CHECKED : MF_UNCHECKED;
    GetFrameMenu().CheckMenuItem(id, check);
}

// Updates the Check B menu item.
void CMainFrame::OnUpdateCheckB(UINT id)
{
    bool isCheckB = (GetDoc().GetCheckB() != 0);
    UINT check = MF_BYCOMMAND | (isCheckB ? MF_CHECKED : MF_UNCHECKED);
    GetFrameMenu().CheckMenuItem(id, check);
}

// Updates the Check C menu item.
void CMainFrame::OnUpdateCheckC(UINT id)
{
    bool isCheckC = (GetDoc().GetCheckC() != 0);
    UINT check = MF_BYCOMMAND | (isCheckC ? MF_CHECKED : MF_UNCHECKED);
    GetFrameMenu().CheckMenuItem(id, check);
}

// Updates the radio button menu selection.
void CMainFrame::OnUpdateRangeOfIDs(UINT idFirst, UINT idLast, UINT id)
{
    if (GetDoc().GetRadio() == id)
        GetFrameMenu().CheckMenuRadioItem(idFirst, idLast, id, MF_BYCOMMAND);
}

// Save the settings in the registry.
BOOL CMainFrame::SaveRegistrySettings()
{
    CFrame::SaveRegistrySettings();
    GetDoc().SaveSettings(GetRegistryKeyName().c_str());

    return TRUE;
}

// Specifies the images for some of the menu items.
void CMainFrame::SetupMenuIcons()
{
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        SetMenuIcons(data, lightgray, IDW_MAIN);
    else
        SetMenuIcons(data, lightgray, IDB_MENUICONS);
}

// Configure the toolbar.
void CMainFrame::SetupToolBar()
{
    // Set the Resource IDs for the toolbar buttons
    AddToolBarButton( IDM_FILE_NEW, FALSE );
    AddToolBarButton( IDM_FILE_OPEN, FALSE );
    AddToolBarButton( IDM_FILE_SAVE, FALSE );
    AddToolBarButton( 0 );        // Separator
    AddToolBarButton( IDM_EDIT_CUT, FALSE );
    AddToolBarButton( IDM_EDIT_COPY, FALSE );
    AddToolBarButton( IDM_EDIT_PASTE, FALSE );
    AddToolBarButton( 0 );        // Separator
    AddToolBarButton( IDM_FILE_PRINT, FALSE);
    AddToolBarButton( 0 );        // Separator
    AddToolBarButton( IDM_HELP_ABOUT );
}

// Process the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
    //  switch (msg)
    //  {
    //  Add case statements for each messages to be handled here
    //  }

        // pass unhandled messages on for default processing
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << "Error: " << e.what();
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

