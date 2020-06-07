////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "resource.h"
#include "mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame() : m_sdiView(IDD_DIALOG1)
{
    // Constructor for CMainFrame. Its called after CFrame's constructor

    //Set m_SdiView as the view window of the frame
    SetView(m_sdiView);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\SdiDocViewForm"));
}

CMainFrame::~CMainFrame()
{
    // Destructor for CMainFrame.
}

BOOL CMainFrame::LoadRegistrySettings(LPCTSTR keyName)
{
    CFrame::LoadRegistrySettings(keyName);
    GetDoc().LoadDocRegistry(GetRegistryKeyName().c_str());

    return TRUE;
}

void CMainFrame::OnFileExit()
{
    // Issue a close request to the frame
    Close();
}

void CMainFrame::OnMenuUpdate(UINT id)
{
    // Update the check state of the various menu items
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

BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);
    UINT id = LOWORD(wparam);

    switch(id)
    {
    case ID_CHECK_A:         m_sdiView.OnCheckA();  return TRUE;
    case ID_CHECK_B:         m_sdiView.OnCheckB();  return TRUE;
    case ID_CHECK_C:         m_sdiView.OnCheckC();  return TRUE;
    case IDM_FILE_EXIT:      OnFileExit();          return TRUE;
    case IDW_VIEW_STATUSBAR: OnViewStatusBar();     return TRUE;
    case IDW_VIEW_TOOLBAR:   OnViewToolBar();       return TRUE;
    case IDM_HELP_ABOUT:     OnHelp();              return TRUE;

    case ID_RADIO_A:
    case ID_RADIO_B:        // intentionally blank
    case ID_RADIO_C:        m_sdiView.OnRangeOfIDs(ID_RADIO_A, ID_RADIO_C, id);    return TRUE;

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

    // call the base class function
    return CFrame::OnCreate(cs);
}

void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.
}

void CMainFrame::OnUpdateCheckA(UINT id)
{
    BOOL isCheckA = GetDoc().GetCheckA();
    GetFrameMenu().CheckMenuItem(id, MF_BYCOMMAND | (isCheckA ? MF_CHECKED : MF_UNCHECKED));
}

void CMainFrame::OnUpdateCheckB(UINT id)
{
    BOOL isCheckB = GetDoc().GetCheckB();
    GetFrameMenu().CheckMenuItem(id, MF_BYCOMMAND | (isCheckB ? MF_CHECKED : MF_UNCHECKED));
}

void CMainFrame::OnUpdateCheckC(UINT id)
{
    BOOL isCheckC = GetDoc().GetCheckC();
    GetFrameMenu().CheckMenuItem(id, MF_BYCOMMAND | (isCheckC ? MF_CHECKED : MF_UNCHECKED));
}

void CMainFrame::OnUpdateRangeOfIDs(UINT idFirst, UINT idLast, UINT id)
{
    int fileItem = GetMenuItemPos(GetFrameMenu(), _T("Select"));
    CMenu radioMenu = GetFrameMenu().GetSubMenu(fileItem);
    if (GetDoc().GetRadio() == id)
        radioMenu.CheckMenuRadioItem(idFirst, idLast, id, 0);
}

void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
    CFrame::PreCreate(cs);
//  cs.cx = 500;
//  cs.cy = 420;
}

BOOL CMainFrame::SaveRegistrySettings()
{
    CFrame::SaveRegistrySettings();
    GetDoc().SaveDocRegistry(GetRegistryKeyName().c_str());

    return TRUE;
}

void CMainFrame::SetupMenuIcons()
{
    // Set the bitmap used for menu icons
    AddMenuIcons(GetToolBarData(), RGB(192, 192, 192), IDB_MENUICONS, 0);
}

void CMainFrame::SetupToolBar()
{
    // Set the Resource IDs for the toolbar buttons
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_OPEN  );
    AddToolBarButton( IDM_FILE_SAVE  );
    AddToolBarButton( 0 );        // Separator
    AddToolBarButton( IDM_EDIT_CUT   );
    AddToolBarButton( IDM_EDIT_COPY  );
    AddToolBarButton( IDM_EDIT_PASTE );
    AddToolBarButton( 0 );        // Separator
    AddToolBarButton( IDM_FILE_PRINT );
    AddToolBarButton( 0 );        // Separator
    AddToolBarButton( IDM_HELP_ABOUT );
}

LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
//  switch (uMsg)
//  {
//      Add case statements for each messages to be handled here
//  }

    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}

