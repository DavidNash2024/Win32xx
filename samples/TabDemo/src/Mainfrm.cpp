/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "Classes.h"
#include "Files.h"
#include "MyDialog.h"
#include "resource.h"

///////////////////////////////////
// CMainFrame function definitions.
//

// Constructor for CMainFrame.
CMainFrame::CMainFrame()
{
}

// Destructor for CMainFrame.
CMainFrame::~CMainFrame()
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    // Set the tab control as the frame's view window
    SetView(m_view);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\Tab Demo"));

    return CFrame::Create(parent);
}

// OnCommand responds to menu and and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
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

// Called during the frame's creation.
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

// Issue a close request to the frame.
BOOL CMainFrame::OnFileExit()
{
    Close();

    return TRUE;
}

// Called after the frame is created.
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

// Update the check state of the various menu items.
void CMainFrame::OnMenuUpdate(UINT id)
{
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

// Add a Files tab.
BOOL CMainFrame::OnNewFilesTab()
{
    m_view.AddTabPage(new CViewFiles, _T("Files"), IDI_FILEVIEW);
    return TRUE;
}

// Add a Classes tab.
BOOL CMainFrame::OnNewClassesTab()
{
    m_view.AddTabPage(new CViewClasses, _T("Classes"), IDI_CLASSVIEW);
    return TRUE;
}

// Add a Dialog tab.
BOOL CMainFrame::OnNewDialogTab()
{
    m_view.AddTabPage(new CViewDialog(IDD_MYDIALOG), _T("Dialog"), IDI_DIALOGVIEW);
    return TRUE;
}

// Creates the popup menu when the "New" toolbar button is pressed.
BOOL CMainFrame::OnNewTab()
{
    // Position the popup menu
    CToolBar& tb = GetToolBar();
    RECT rc = tb.GetItemRect(tb.CommandToIndex(IDM_NEW_TAB));
    tb.MapWindowPoints(0, (LPPOINT)&rc, 2);

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

// Show or hide the tab control buttons.
BOOL CMainFrame::OnShowButtons()
{
    BOOL isShown = m_view.GetShowButtons();
    m_view.SetShowButtons(!isShown);
    m_view.RedrawWindow();
    return TRUE;
}

// Called when a tab is closed.
BOOL CMainFrame::OnTabClose()
{
    int tab = m_view.GetCurSel();

    if (tab >= 0)
        m_view.RemoveTabPage(tab);

    return TRUE;
}

// Enable or disable fixed tab width.
BOOL CMainFrame::OnTabFixedWidth()
{
    BOOL isFixed = m_view.GetStyle() & TCS_FIXEDWIDTH;
    m_view.SetFixedWidth(!isFixed);
    return TRUE;
}

// Enable or disable tab owner draw.
BOOL CMainFrame::OnTabOwnerDraw()
{
    BOOL isOwnerDraw = m_view.GetStyle() & TCS_OWNERDRAWFIXED;
    m_view.SetOwnerDraw(!isOwnerDraw);
    return TRUE;
}

// Position the tabs at the top or bottom.
BOOL CMainFrame::OnTabsAtTop()
{
    BOOL isAtTop = m_view.GetTabsAtTop();
    m_view.SetTabsAtTop(!isAtTop);
    return TRUE;
}

// Set the CREATESTRUCT parameters before the window is created.
void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
    // Call the base class function first
    CFrame::PreCreate(cs);

    // Hide the window initially by removing the WS_VISIBLE style
    cs.style &= ~WS_VISIBLE;
}

// Specifies the images used on menu items.
void CMainFrame::SetupMenuIcons()
{
    // Use the MenuIcons bitmap for images in menu items.
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        AddMenuIcons(data, RGB(192, 192, 192), IDW_MAIN);
    else
        AddMenuIcons(data, RGB(192, 192, 192), IDB_TOOLBAR16);

    // Add some extra icons for menu items
    AddMenuIcon(IDM_NEW_FILES,   IDI_FILEVIEW);
    AddMenuIcon(IDM_NEW_CLASSES, IDI_CLASSVIEW);
    AddMenuIcon(IDM_NEW_DIALOG,  IDI_DIALOGVIEW);
}

// Configure the toolbar.
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

// Process the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        return WndProcDefault(msg, wparam, lparam);
    }

    catch (const CException& e)
    {
        // Display the exception.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);
        return 0;
    }
}
