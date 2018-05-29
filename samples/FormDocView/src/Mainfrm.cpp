////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "resource.h"
#include "FormApp.h"
#include "mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame() : m_SdiView(IDD_DIALOG1)
{
    // Constructor for CMainFrame. Its called after CFrame's constructor

    //Set m_SdiView as the view window of the frame
    SetView(m_SdiView);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\SdiDocViewForm"));
}

CMainFrame::~CMainFrame()
{
    // Destructor for CMainFrame.
}

BOOL CMainFrame::LoadRegistrySettings(LPCTSTR szKeyName)
{
    CFrame::LoadRegistrySettings(szKeyName);
    GetDoc().LoadDocRegistry(GetRegistryKeyName().c_str());

    return TRUE;
}

void CMainFrame::OnFileExit()
{
    // Issue a close request to the frame
    PostMessage(WM_CLOSE);
}

void CMainFrame::OnMenuUpdate(UINT nID)
{
    // Update the check state of the various menu items
    switch (nID)
    {
    case ID_CHECK_A:
        OnUpdateCheckA(nID);
        break;
    case ID_CHECK_B:
        OnUpdateCheckB(nID);
        break;
    case ID_CHECK_C:
        OnUpdateCheckC(nID);
        break;
    }

    if ((nID >= ID_RADIO_A) && (nID <= ID_RADIO_C))
        OnUpdateRangeOfIDs(ID_RADIO_A, ID_RADIO_C, nID);

    CFrame::OnMenuUpdate(nID);
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    UINT nID = LOWORD(wParam);

    switch(nID)
    {
    case ID_CHECK_A:         m_SdiView.OnCheckA();  return TRUE;
    case ID_CHECK_B:         m_SdiView.OnCheckB();  return TRUE;
    case ID_CHECK_C:         m_SdiView.OnCheckC();  return TRUE;
    case IDM_FILE_EXIT:      OnFileExit();          return TRUE;
    case IDW_VIEW_STATUSBAR: OnViewStatusBar();     return TRUE;
    case IDW_VIEW_TOOLBAR:   OnViewToolBar();       return TRUE;
    case IDM_HELP_ABOUT:     OnHelp();              return TRUE;

    case ID_RADIO_A:
    case ID_RADIO_B:        // intentionally blank
    case ID_RADIO_C:        m_SdiView.OnRangeOfIDs(ID_RADIO_A, ID_RADIO_C, nID);    return TRUE;

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

void CMainFrame::OnUpdateCheckA(UINT nID)
{
    BOOL bCheck = GetDoc().GetCheckA();
    GetFrameMenu().CheckMenuItem(nID, MF_BYCOMMAND | (bCheck ? MF_CHECKED : MF_UNCHECKED));
}

void CMainFrame::OnUpdateCheckB(UINT nID)
{
    BOOL bCheck = GetDoc().GetCheckB();
    GetFrameMenu().CheckMenuItem(nID, MF_BYCOMMAND | (bCheck ? MF_CHECKED : MF_UNCHECKED));
}

void CMainFrame::OnUpdateCheckC(UINT nID)
{
    BOOL bCheck = GetDoc().GetCheckC();
    GetFrameMenu().CheckMenuItem(nID, MF_BYCOMMAND | (bCheck ? MF_CHECKED : MF_UNCHECKED));
}

void CMainFrame::OnUpdateRangeOfIDs(UINT nIDFirst, UINT nIDLast, UINT nID)
{
    int nFileItem = GetMenuItemPos(GetFrameMenu(), _T("Select"));
    CMenu RadioMenu = GetFrameMenu().GetSubMenu(nFileItem);
    if (GetDoc().GetRadio() == nID)
        RadioMenu.CheckMenuRadioItem(nIDFirst, nIDLast, nID, 0);
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

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//  switch (uMsg)
//  {
//      Add case statements for each messages to be handled here
//  }

    // pass unhandled messages on for default processing
    return WndProcDefault(uMsg, wParam, lParam);
}

