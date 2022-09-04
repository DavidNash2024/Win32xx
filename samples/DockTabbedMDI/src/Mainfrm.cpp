/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "Browser.h"
#include "Classes.h"
#include "Files.h"
#include "MyDialog.h"
#include "Output.h"
#include "Rect.h"
#include "Text.h"
#include "resource.h"


//////////////////////////////////
// CMainFrame function definitions
//

// Constructor for CMainFrame.
CMainFrame::CMainFrame() : m_isContainerTabsAtTop(FALSE), m_isHideSingleTab(TRUE),
                            m_isMDITabsAtTop(TRUE), m_pActiveDocker(NULL)
{
}

// Destructor for CMainFrame.
CMainFrame::~CMainFrame()
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    //Set m_MyTabbedMDI as the view window of the frame
    SetView(m_myTabbedMDI);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\TabbedMDI Docking"));

    return CDockFrame::Create(parent);
}

// Hides or shows tabs for containers with a single tab.
void CMainFrame::HideSingleContainerTab(bool hideSingle)
{
    m_isHideSingleTab = hideSingle;
    std::vector<DockPtr>::const_iterator iter;

    // Set the Tab position for each container
    for (iter = GetAllDockChildren().begin(); iter < GetAllDockChildren().end(); ++iter)
    {
        CDockContainer* pContainer = (*iter)->GetContainer();
        if (pContainer && pContainer->IsWindow())
        {
            pContainer->SetHideSingleTab(hideSingle);
        }
    }

}

// Loads the default arrangement of dockers.
void CMainFrame::LoadDefaultDockers()
{
    // Note: The  DockIDs are used for saving/restoring the dockers state in the registry

    DWORD style = DS_CLIENTEDGE; // The style added to each docker

    // Add the parent dockers
    CDocker* pDockRight  = AddDockedChild(new CDockClasses, DS_DOCKED_RIGHT | style, 250, ID_DOCK_CLASSES1);
    CDocker* pDockBottom = AddDockedChild(new CDockText, DS_DOCKED_BOTTOM | style, 100, ID_DOCK_TEXT1);

    // Add the remaining dockers
    pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | style, 250, ID_DOCK_FILES1);
    pDockRight->AddDockedChild(new CDockDialog, DS_DOCKED_CONTAINER | style, 250, ID_DOCK_DIALOG);

    pDockBottom->AddDockedChild(new CDockOutput, DS_DOCKED_CONTAINER | style, 100, ID_DOCK_OUTPUT1);
    pDockBottom->AddDockedChild(new CDockText, DS_DOCKED_CONTAINER | style, 100, ID_DOCK_TEXT2);
    pDockBottom->AddDockedChild(new CDockOutput, DS_DOCKED_CONTAINER | style, 100, ID_DOCK_OUTPUT2);
}

// Loads the default arrangement of MDIs.
void CMainFrame::LoadDefaultMDIs()
{
    // Add some MDI tabs
    m_myTabbedMDI.AddMDIChild(new CViewWeb, _T("Browser"), ID_MDI_WEB);
    m_myTabbedMDI.AddMDIChild(new CViewRect, _T("Rectangles"), ID_MDI_RECT);
    m_myTabbedMDI.AddMDIChild(new CViewText, _T("TextView"), ID_MDI_TEXT);
    m_myTabbedMDI.AddMDIChild(new CViewClasses, _T("Classes"), ID_MDI_CLASSES);
    m_myTabbedMDI.AddMDIChild(new CViewFiles, _T("Files"), ID_MDI_FILES);

    if (m_myTabbedMDI.IsWindow())
        m_myTabbedMDI.SetActiveMDITab(0);
}

// Adds a new docker. The dockID parameter specifies the docker type.
CDocker* CMainFrame::NewDockerFromID(int dockID)
{
    CDocker* pDocker = NULL;
    switch (dockID)
    {
    case ID_DOCK_CLASSES1:
        pDocker = new CDockClasses;
        break;
    case ID_DOCK_CLASSES2:
        pDocker = new CDockClasses;
        break;
    case ID_DOCK_FILES1:
        pDocker = new CDockFiles;
        break;
    case ID_DOCK_FILES2:
        pDocker = new CDockFiles;
        break;
    case ID_DOCK_OUTPUT1:
        pDocker = new CDockOutput;
        break;
    case ID_DOCK_OUTPUT2:
        pDocker = new CDockOutput;
        break;
    case ID_DOCK_TEXT1:
        pDocker = new CDockText;
        break;
    case ID_DOCK_TEXT2:
        pDocker = new CDockText;
        break;
    case ID_DOCK_DIALOG:
        pDocker = new CDockDialog;
        break;
    default:
        TRACE("Unknown Dock ID\n");
        break;
    }

    return pDocker;
}

// Close all the dockers.
BOOL CMainFrame::OnCloseDockers()
{
    CloseAllDockers();
    return TRUE;
}

// Close all the MDIs.
BOOL CMainFrame::OnCloseMDIs()
{
    m_myTabbedMDI.CloseAllMDIChildren();
    return TRUE;
}

// Called when the main window is asked to close.
void CMainFrame::OnClose()
{
    SaveRegistrySettings();
    m_myTabbedMDI.CloseAllMDIChildren();
    Destroy();
}

// OnCommand responds to menu and and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UINT id = LOWORD(wparam);
    switch (id)
    {
    case IDM_CLOSE_DOCKERS:     return OnCloseDockers();
    case IDM_CLOSE_MDIS:        return OnCloseMDIs();
    case IDM_CONTAINER_TOP:     return OnContainerTabsAtTop();
    case IDM_DEFAULT_LAYOUT:    return OnDefaultLayout();
    case IDM_FILE_NEW:          return OnFileNew();
    case IDM_FILE_NEWBROWSER:   return OnFileNewBrowser();
    case IDM_FILE_NEWRECT:      return OnFileNewRect();
    case IDM_FILE_NEWTEXT:      return OnFileNewText();
    case IDM_FILE_NEWTREE:      return OnFileNewTree();
    case IDM_FILE_NEWLIST:      return OnFileNewList();
    case IDM_FILE_EXIT:         return OnFileExit();
    case IDM_HELP_ABOUT:        return OnHelp();
    case IDM_HIDE_SINGLE_TAB:   return OnHideSingleTab();
    case IDM_TABBEDMDI_TOP:     return OnMDITabsAtTop();
    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDW_FIRSTCHILD:
    case IDW_FIRSTCHILD + 1:
    case IDW_FIRSTCHILD + 2:
    case IDW_FIRSTCHILD + 3:
    case IDW_FIRSTCHILD + 4:
    case IDW_FIRSTCHILD + 5:
    case IDW_FIRSTCHILD + 6:
    case IDW_FIRSTCHILD + 7:
    case IDW_FIRSTCHILD + 8:
    {
        int tab = LOWORD(wparam) - IDW_FIRSTCHILD;
        m_myTabbedMDI.SetActiveMDITab(tab);
        return TRUE;
    }
    case IDW_FIRSTCHILD + 9:
    {
        m_myTabbedMDI.ShowListDialog();
        return TRUE;
    }

    default:
    {
        // Pass the command on to the view of the active docker
        m_pActiveDocker->GetActiveView()->SendMessage(WM_COMMAND, wparam, lparam);
    }
    }

    return FALSE;
}

// Reposition the tabs in the containers.
BOOL CMainFrame::OnContainerTabsAtTop()
{
    SetContainerTabsAtTop(!m_isContainerTabsAtTop);
    return TRUE;
}

// OnCreate controls the way the frame is created.
// Overriding CFrame::OnCreate is optional.
int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
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
    return CDockFrame::OnCreate(cs);
}

// Replaces the current docking arrangement with the default one.
BOOL CMainFrame::OnDefaultLayout()
{
    SetRedraw(FALSE);

    CloseAllDockers();
    m_myTabbedMDI.CloseAllMDIChildren();
    LoadDefaultDockers();
    LoadDefaultMDIs();

    SetContainerTabsAtTop(m_isContainerTabsAtTop);
    HideSingleContainerTab(m_isHideSingleTab);
    SetRedraw(TRUE);
    RedrawWindow();
    return TRUE;
}

// Called when a docker is activated. Stores the active docker in preparation for menu input.
// Excludes active docker change for undocked dockers when using the menu.
LRESULT CMainFrame::OnDockActivated(UINT msg, WPARAM wparam, LPARAM lparam)
{
    CPoint pt = GetCursorPos();
    if (WindowFromPoint(pt) != GetMenuBar())
        m_pActiveDocker = GetActiveDocker();

    return CDockFrame::OnDockActivated(msg, wparam, lparam);
}

// Issue a close request to the frame to end the program.
BOOL CMainFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Creates the popup menu when the "New" toolbar button is pressed.
BOOL CMainFrame::OnFileNew()
{
    // Position the popup menu
    CToolBar& tb = GetToolBar();
    RECT rc = tb.GetItemRect(tb.CommandToIndex(IDM_FILE_NEW));
    tb.ClientToScreen(rc);

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

// Adds a MDI with a list-view.
BOOL CMainFrame::OnFileNewList()
{
    m_myTabbedMDI.AddMDIChild(new CViewFiles, _T("ListView"), ID_MDI_FILES);
    return TRUE;
}

// Adds a MDI with a Rectangles view.
BOOL CMainFrame::OnFileNewRect()
{
    m_myTabbedMDI.AddMDIChild(new CViewRect, _T("Rectangles"), ID_MDI_RECT);
    return TRUE;
}

// Adds a MDI with a Browser view.
BOOL CMainFrame::OnFileNewBrowser()
{
    m_myTabbedMDI.AddMDIChild(new CViewWeb, _T("Browser"), ID_MDI_WEB);
    return TRUE;
}

// Adds a MDI with a Text view.
BOOL CMainFrame::OnFileNewText()
{
    m_myTabbedMDI.AddMDIChild(new CViewText, _T("TextView"), ID_MDI_TEXT);
    return TRUE;
}

// Adds a MDI with a tree-view.
BOOL CMainFrame::OnFileNewTree()
{
    m_myTabbedMDI.AddMDIChild(new CViewClasses, _T("TreeView"), ID_MDI_CLASSES);
    return TRUE;
}

// Toggle hiding of tabs for containers with a single tab.
BOOL CMainFrame::OnHideSingleTab()
{
    HideSingleContainerTab(!m_isHideSingleTab);
    return TRUE;
}

// Called after the frame window is created.
void CMainFrame::OnInitialUpdate()
{
    SetDockStyle(DS_CLIENTEDGE);

    // Load dock settings
    if (!LoadDockRegistrySettings(GetRegistryKeyName()))
        LoadDefaultDockers();

    // Load MDI child settings
    if (!m_myTabbedMDI.LoadRegistrySettings(GetRegistryKeyName()))
        LoadDefaultMDIs();

    // Hide the container's tab if it has just one tab
    HideSingleContainerTab(m_isHideSingleTab);

    // Get a copy of the Frame's menu
    CMenu frameMenu = GetFrameMenu();

    // Modify the menu
    int menuPos = frameMenu.GetMenuItemCount() - 1;
    CMenu winMenu = m_myTabbedMDI.GetListMenu();
    frameMenu.InsertPopupMenu(menuPos, MF_BYPOSITION, winMenu, _T("&Window"));

    // Replace the frame's menu with our modified menu
    SetFrameMenu(frameMenu);

    // PreCreate initially set the window as invisible, so show it now.
    ShowWindow(GetInitValues().showCmd);
    RedrawWindow(RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
}

// Updates menu items before they are displayed.
LRESULT CMainFrame::OnInitMenuPopup(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Update the "Window" menu
    m_myTabbedMDI.GetListMenu();

    return CDockFrame::OnInitMenuPopup(msg, wparam, lparam);
}

// Reposition TabbedMDI's tabs.
BOOL CMainFrame::OnMDITabsAtTop()
{
    SetMDITabsAtTop(!m_isMDITabsAtTop);
    return TRUE;
}

// Called when menu items are about to be displayed.
void CMainFrame::OnMenuUpdate(UINT id)
{
    // Only for the Menu IDs we wish to modify
    if (id >= IDM_EDIT_UNDO && id <= IDM_EDIT_DELETE)
    {
        // Get the pointer to the active view
        CWnd* pView = m_pActiveDocker->GetActiveView();

        // Enable the Edit menu items for CViewText views, disable them otherwise
        CMenu editMenu = GetFrameMenu().GetSubMenu(1);
        UINT flags = (dynamic_cast<CViewText*>(pView))? MF_ENABLED : MF_GRAYED;
        editMenu.EnableMenuItem(id, MF_BYCOMMAND | flags);
    }

    UINT check;
    switch (id)
    {
    case IDM_CONTAINER_TOP:
        check = (m_isContainerTabsAtTop) ? MF_CHECKED : MF_UNCHECKED;
        GetFrameMenu().CheckMenuItem(id, check);
        break;

    case IDM_HIDE_SINGLE_TAB:
        check = (m_isHideSingleTab) ? MF_CHECKED : MF_UNCHECKED;
        GetFrameMenu().CheckMenuItem(id, check);
        break;

    case IDM_TABBEDMDI_TOP:
        check = (m_isMDITabsAtTop) ? MF_CHECKED : MF_UNCHECKED;
        GetFrameMenu().CheckMenuItem(id, check);
    }

    CDockFrame::OnMenuUpdate(id);
}

// Sets the CREATESTRUCT parameters before the window is created.
void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
    // Call base class to set defaults
    CDockFrame::PreCreate(cs);

    // Hide the window initially by removing the WS_VISIBLE style
    cs.style &= ~WS_VISIBLE;
}

// Saves the docking arrangement and other settings in the registry.
BOOL CMainFrame::SaveRegistrySettings()
{
    CDockFrame::SaveRegistrySettings();

    // Save the docker settings
    SaveDockRegistrySettings(GetRegistryKeyName());

    // Save the tabbedMDI settings
    m_myTabbedMDI.SaveRegistrySettings(GetRegistryKeyName());

    return TRUE;
}

// Positions the tabs at the top or bottom of the container.
void CMainFrame::SetContainerTabsAtTop(bool atTop)
{
    m_isContainerTabsAtTop = atTop;
    std::vector<DockPtr>::const_iterator iter;

    // Set the Tab position for each container
    for (iter = GetAllDockChildren().begin(); iter < GetAllDockChildren().end(); ++iter)
    {
        CDockContainer* pContainer = (*iter)->GetContainer();
        if (pContainer && pContainer->IsWindow())
        {
            pContainer->SetTabsAtTop(atTop);
        }
    }
}

// Positions the MDI tabs at the top or bottom of the container.
void CMainFrame::SetMDITabsAtTop(bool atTop)
{
    m_isMDITabsAtTop = atTop;
    m_myTabbedMDI.GetTab().SetTabsAtTop(atTop);
}

// Specifies the images for some of the menu items.
void CMainFrame::SetupMenuIcons()
{
    // Load the default set of icons from the toolbar
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        SetMenuIcons(data, RGB(192, 192, 192), IDW_MAIN);
    else
        SetMenuIcons(data, RGB(192, 192, 192), IDB_TOOLBAR16);

    // Add some extra icons for menu items
    AddMenuIcon(IDM_FILE_NEWBROWSER, IDI_GLOBE);
    AddMenuIcon(IDM_FILE_NEWRECT, IDI_RECT);
    AddMenuIcon(IDM_FILE_NEWTEXT, IDI_TEXT);
    AddMenuIcon(IDM_FILE_NEWLIST, IDI_FILEVIEW);
    AddMenuIcon(IDM_FILE_NEWTREE, IDI_CLASSVIEW);
}

// Set the Resource IDs for the toolbar buttons
void CMainFrame::SetupToolBar()
{
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_OPEN,  FALSE );
    AddToolBarButton( IDM_FILE_SAVE,  FALSE );

    AddToolBarButton( 0 );  // Separator
    AddToolBarButton( IDM_EDIT_CUT,   FALSE );
    AddToolBarButton( IDM_EDIT_COPY,  FALSE );
    AddToolBarButton( IDM_EDIT_PASTE, FALSE );

    AddToolBarButton( 0 );  // Separator
    AddToolBarButton( IDM_FILE_PRINT, FALSE );

    AddToolBarButton( 0 );  // Separator
    AddToolBarButton( IDM_HELP_ABOUT );
}

// Process the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
    //  switch (msg)
    //  {
    //  }

        // Always pass unhandled messages on to WndProcDefault.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}

