////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "mainfrm.h"
#include "Classes.h"
#include "Files.h"
#include "MyDialog.h"
#include "resource.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
    //Set the tab control as the fram's view window
    SetView(m_view);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\Container Demo"));
}

CMainFrame::~CMainFrame()
{
    // Destructor for CMainFrame.
}

BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    // OnCommand responds to menu and and toolbar input

    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_CLOSE_TAB:         return OnTabClose();
    case IDM_FILE_EXIT:         return OnFileExit();
    case IDM_NEW_FILES:         return OnNewFilesTab();
    case IDM_NEW_CLASSES:       return OnNewClassesTab();
    case IDM_NEW_DIALOG:        return OnNewDialogTab();
    case IDM_NEW_TAB:           return OnNewTab();
    case IDM_TAB_FIXED:         return OnTabFixedWidth();
    case IDM_TAB_DRAW:          return OnTabOwnerDraw();
    case IDM_TAB_TOP:           return OnTabsAtTop();
    case IDM_TAB_BUTTONS:       return OnShowButtons();
    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDM_HELP_ABOUT:        return OnHelp();
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
    CFrame::OnCreate(cs);
    
    // Turn on the tab's owner draw and fixed width tabs
    m_view.SetFixedWidth(TRUE);
    m_view.SetOwnerDraw(TRUE);
    m_view.SetShowButtons(TRUE);

    return 0;
}

BOOL CMainFrame::OnFileExit()
{
    // Issue a close request to the frame
    Close();

    return TRUE;
}

void CMainFrame::OnInitialUpdate()
{   
    // Add some tabs to the tab control
    m_view.AddTabPage(new CViewClasses, _T("Classes"), IDI_CLASSVIEW);
    m_view.AddTabPage(new CViewFiles, _T("Files"), IDI_FILEVIEW);
    m_view.AddTabPage(new CViewClasses, _T("Classes"), IDI_CLASSVIEW);
    m_view.AddTabPage(new CViewFiles, _T("Files"), IDI_FILEVIEW);
    m_view.AddTabPage(new CViewDialog(IDD_MYDIALOG), _T("Dialog"), IDI_DIALOGVIEW);

    m_view.SelectPage(0);

    // PreCreate initially set the window as invisible, so show it now.
    ShowWindow();
}

void CMainFrame::OnMenuUpdate(UINT id)
{
    // Update the check state of the various menu items
    
    BOOL isOwnerDraw  = m_view.GetStyle() & TCS_OWNERDRAWFIXED;
    BOOL isFixed = m_view.GetStyle() & TCS_FIXEDWIDTH;

    switch (id) 
    {

    case IDM_TAB_TOP:
        {
            BOOL isAtTop = m_view.GetTabsAtTop();
            UINT check = (isAtTop)? MF_CHECKED : MF_UNCHECKED;
            GetFrameMenu().CheckMenuItem(IDM_TAB_TOP, check);
        }
        break;
    case IDM_TAB_BUTTONS:
        {
            BOOL isShown = m_view.GetShowButtons();
            UINT check = (isShown)? MF_CHECKED : MF_UNCHECKED;
            GetFrameMenu().CheckMenuItem(IDM_TAB_BUTTONS, check);
            UINT enable = (isOwnerDraw && isFixed)? MF_ENABLED : MF_GRAYED;
            GetFrameMenu().EnableMenuItem(IDM_TAB_BUTTONS, enable);
        }
        break;
    case IDM_TAB_DRAW:
        {
            UINT check = (isOwnerDraw)? MF_CHECKED : MF_UNCHECKED;
            GetFrameMenu().CheckMenuItem(IDM_TAB_DRAW, check);
        }
        break;
    case IDM_TAB_FIXED:
        {
            UINT check = (isFixed)? MF_CHECKED : MF_UNCHECKED;
            GetFrameMenu().CheckMenuItem(IDM_TAB_FIXED, check);
        }
        break;
    }

    CFrame::OnMenuUpdate(id);
}

BOOL CMainFrame::OnNewFilesTab()
{
    m_view.AddTabPage(new CViewFiles, _T("Files"), IDI_FILEVIEW);
    return TRUE;
}

BOOL CMainFrame::OnNewClassesTab()
{
    m_view.AddTabPage(new CViewClasses, _T("Classes"), IDI_CLASSVIEW);
    return TRUE;
}

BOOL CMainFrame::OnNewDialogTab()
{
    m_view.AddTabPage(new CViewDialog(IDD_MYDIALOG), _T("Dialog"), IDI_DIALOGVIEW);
    return TRUE;
}

BOOL CMainFrame::OnNewTab()
{
    // Creates the popup menu when the "New" toolbar button is pressed

    // Position the popup menu
    CToolBar& tb = GetToolBar();
    RECT rc = tb.GetItemRect(tb.CommandToIndex(IDM_NEW_TAB));
    tb.MapWindowPoints(NULL, (LPPOINT)&rc, 2);

    TPMPARAMS tpm;
    tpm.cbSize = sizeof(TPMPARAMS);
    tpm.rcExclude = rc;

    // Load the popup menu
    CMenu topMenu(IDM_NEWMENU);
    CMenu popupMenu = topMenu.GetSubMenu(0);

    // Start the popup menu
    popupMenu.TrackPopupMenuEx(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, rc.left, rc.bottom, *this, &tpm);
    return TRUE;
}

BOOL CMainFrame::OnShowButtons()
{
    BOOL isShown = m_view.GetShowButtons();
    m_view.SetShowButtons(!isShown);
    m_view.RedrawWindow();
    return TRUE;
}

BOOL CMainFrame::OnTabClose()
{
    int tab = m_view.GetCurSel();

    if (tab >= 0)
        m_view.RemoveTabPage(tab);

    return TRUE;
}

BOOL CMainFrame::OnTabFixedWidth()
{
    BOOL isFixed = m_view.GetStyle() & TCS_FIXEDWIDTH;
    m_view.SetFixedWidth(!isFixed);
    return TRUE;
}

BOOL CMainFrame::OnTabOwnerDraw()
{
    BOOL isOwnerDraw = m_view.GetStyle() & TCS_OWNERDRAWFIXED;
    m_view.SetOwnerDraw(!isOwnerDraw);
    return TRUE;
}

BOOL CMainFrame::OnTabsAtTop()
{
    BOOL isAtTop = m_view.GetTabsAtTop();
    m_view.SetTabsAtTop(!isAtTop);
    return TRUE;
}

void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
    // Call the base class function first
    CFrame::PreCreate(cs);
    
    // Hide the window initially by removing the WS_VISIBLE style
    cs.style &= ~WS_VISIBLE;
}

void CMainFrame::SetupMenuIcons()
{
    // Load default set of menu icons from the toolbar
    CFrame::SetupMenuIcons();

    // Add some extra icons for menu items
    AddMenuIcon(IDM_NEW_FILES,   IDI_FILEVIEW);
    AddMenuIcon(IDM_NEW_CLASSES, IDI_CLASSVIEW);
    AddMenuIcon(IDM_NEW_DIALOG,  IDI_DIALOGVIEW);
}

void CMainFrame::SetupToolBar()
{
    // Set the Resource IDs for the toolbar buttons
    AddToolBarButton( IDM_NEW_TAB           );
    AddToolBarButton( IDM_FILE_OPEN,  FALSE );
    AddToolBarButton( IDM_FILE_SAVE,  FALSE );
    
    AddToolBarButton( 0 );  // Separator
    AddToolBarButton( IDM_EDIT_CUT,   FALSE );
    AddToolBarButton( IDM_EDIT_COPY,  FALSE );
    AddToolBarButton( IDM_EDIT_PASTE, FALSE );
    
    AddToolBarButton( 0 );  // Separator
    AddToolBarButton( IDM_FILE_PRINT, FALSE );
    
    AddToolBarButton( 0 );  // Separator
    AddToolBarButton( IDM_HELP_ABOUT        );

    // Set the Checkmarks in the menu
    GetFrameMenu().CheckMenuItem(IDM_TAB_BUTTONS, MF_UNCHECKED);
}

LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
//  switch (msg)
//  {
//      Add case statements for each messages to be handled here
//  }

    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}

