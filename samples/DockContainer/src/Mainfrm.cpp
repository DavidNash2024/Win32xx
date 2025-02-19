/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "Classes.h"
#include "Files.h"
#include "Output.h"
#include "Text.h"
#include "resource.h"

using namespace std;

//////////////////////////////////
// CMainFrame function definitions
//

// Constructor for CMainFrame.
CMainFrame::CMainFrame() : m_isContainerTabsAtTop(false), m_hideSingleTab(true)
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    // Set m_view as the view window of the frame.
    SetView(m_view);

    // Set the registry key name, and load the initial window position.
    // Use a registry key name like "CompanyName\\Application".
    LoadRegistrySettings(L"Win32++\\DockContainer");

    return CDockFrame::Create(parent);
}

// Hides or shows the tab for a container with a single tab.
void CMainFrame::HideSingleContainerTab(bool hideSingle)
{
    m_hideSingleTab = hideSingle;

    // Set the Tab position for each container
    for (CDocker* docker : GetAllDockers())
    {
        CDockContainer* pContainer = docker->GetContainer();
        if (pContainer && pContainer->IsWindow())
        {
            pContainer->SetHideSingleTab(hideSingle);
        }
    }
}

// Loads the default arrangement of dockers.
void CMainFrame::LoadDefaultDockers()
{
    // Note: The  DockIDs are used for saving/restoring the dockers state in the registry.

    DWORD style = DS_CLIENTEDGE; // The style added to each docker

    // Add the right most dockers.
    CDocker* pDockRight = AddDockedChild(make_unique<CDockClasses>(), DS_DOCKED_RIGHT | style, DpiScaleInt(200), ID_DOCK_CLASSES1);
    pDockRight->AddDockedChild(make_unique<CDockFiles>(), DS_DOCKED_CONTAINER | style, DpiScaleInt(200), ID_DOCK_FILES1);
    pDockRight->AddDockedChild(make_unique<CDockClasses>(), DS_DOCKED_CONTAINER | style, DpiScaleInt(200), ID_DOCK_CLASSES2);
    pDockRight->AddDockedChild(make_unique<CDockFiles>(), DS_DOCKED_CONTAINER | style, DpiScaleInt(200), ID_DOCK_FILES2);

    // Add the bottom dockers.
    CDocker* pDockBottom = AddDockedChild(make_unique<CDockOutput>(), DS_DOCKED_BOTTOM | style, DpiScaleInt(100), ID_DOCK_OUTPUT1);
    pDockBottom->AddDockedChild(make_unique<CDockOutput>(), DS_DOCKED_CONTAINER | style, DpiScaleInt(100), ID_DOCK_OUTPUT2);

    // Add the frame's dockers.
    AddDockedChild(make_unique<CDockText>(), DS_DOCKED_CONTAINER | style, DpiScaleInt(100), ID_DOCK_TEXT1);
    AddDockedChild(make_unique<CDockText>(), DS_DOCKED_CONTAINER | style, DpiScaleInt(100), ID_DOCK_TEXT2);

    SetDockStyle(style);
}

// Adds a new docker. The id specifies its type.
DockPtr CMainFrame::NewDockerFromID(int id)
{
    DockPtr docker;
    switch(id)
    {
    case ID_DOCK_CLASSES1:
        docker = make_unique<CDockClasses>();
        break;
    case ID_DOCK_CLASSES2:
        docker = make_unique<CDockClasses>();
        break;
    case ID_DOCK_FILES1:
        docker = make_unique<CDockFiles>();
        break;
    case ID_DOCK_FILES2:
        docker = make_unique<CDockFiles>();
        break;
    case ID_DOCK_OUTPUT1:
        docker = make_unique<CDockOutput>();
        break;
    case ID_DOCK_OUTPUT2:
        docker = make_unique<CDockOutput>();
        break;
    case ID_DOCK_TEXT1:
        docker = make_unique<CDockText>();
        break;
    case ID_DOCK_TEXT2:
        docker = make_unique<CDockText>();
        break;
    default:
        TRACE("Unknown Dock ID\n");
        break;
    }

    return docker;
}

// OnCommand responds to menu and and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_CONTAINER_TOP:     return OnContainerTabsAtTop();
    case IDM_FILE_EXIT:         return OnFileExit();
    case IDM_DOCK_DEFAULT:      return OnDockDefault();
    case IDM_DOCK_CLOSEALL:     return OnDockCloseAll();
    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDM_HELP_ABOUT:        return OnHelp();
    case IDM_HIDE_SINGLE_TAB:   return OnHideSingleTab();
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

    // UseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar.
    // UseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar.
    // UseOwnerDrawnMenu(FALSE);     // Don't use owner draw for popup menu items.
    // UseReBar(FALSE);              // Don't use a ReBar.
    // UseStatusBar(FALSE);          // Don't use a StatusBar.
    // UseThemes(FALSE);             // Don't use themes.
    // UseToolBar(FALSE);            // Don't use a ToolBar.

    // call the base class function
    return CDockFrame::OnCreate(cs);
}

// Replaces the current docking arrangement with the default.
BOOL CMainFrame::OnDockDefault()
{
    // Suppress redraw to render the docking changes smoothly.
    SetRedraw(FALSE);

    CloseAllDockers();
    LoadDefaultDockers();
    HideSingleContainerTab(m_hideSingleTab);
    SetContainerTabsAtTop(m_isContainerTabsAtTop);

    // Enable redraw and redraw the frame.
    SetRedraw(TRUE);
    RedrawWindow();
    return TRUE;
}

// Close all the frame's dockers.
BOOL CMainFrame::OnDockCloseAll()
{
    CloseAllDockers();
    return TRUE;
}

// Issue a close request to the frame to end the program.
BOOL CMainFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Limit the minimum size of the window.
LRESULT CMainFrame::OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam)
{
    LPMINMAXINFO lpMMI = (LPMINMAXINFO)lparam;
    const CSize minimumSize(500, 350);
    lpMMI->ptMinTrackSize.x = DpiScaleInt(minimumSize.cx);
    lpMMI->ptMinTrackSize.y = DpiScaleInt(minimumSize.cy);
    return FinalWindowProc(msg, wparam, lparam);
}

// Toggle the hiding of tabs for containers with a single tab.
BOOL CMainFrame::OnHideSingleTab()
{
    HideSingleContainerTab(!m_hideSingleTab);
    return TRUE;
}

// Called after the frame's window is created.
void CMainFrame::OnInitialUpdate()
{
    // Load dock settings
    if (!LoadDockRegistrySettings(GetRegistryKeyName()))
        LoadDefaultDockers();

    // Hide the container's tab if it has just one tab
    HideSingleContainerTab(m_hideSingleTab);

    // PreCreate initially set the window as invisible, so show it now.
    ShowWindow( GetInitValues().showCmd );
}

// Called when menu items are about to be displayed.
void CMainFrame::OnMenuUpdate(UINT id)
{
    UINT check;
    switch (id)
    {
    case IDM_CONTAINER_TOP:
        check = (m_isContainerTabsAtTop) ? MF_CHECKED : MF_UNCHECKED;
        GetFrameMenu().CheckMenuItem(id, check);
        break;

    case IDM_HIDE_SINGLE_TAB:
        check = (m_hideSingleTab) ? MF_CHECKED : MF_UNCHECKED;
        GetFrameMenu().CheckMenuItem(id, check);
        break;
    }

    CDockFrame::OnMenuUpdate(id);
}

// Sets the CREATESTRUCT parameters before the window is created.
void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
    // Call base clase to set defaults
    CDockFrame::PreCreate(cs);

    // Hide the window initially by removing the WS_VISIBLE style
    cs.style &= ~WS_VISIBLE;
}

// This function overrides CDocker::RecalcDockLayout to elimate jitter
// when the dockers are resized. The technique used here is is most
// appropriate for a complex arrangement of dockers.  It might not suite
// other docking applications. To support this technique the
// WS_EX_COMPOSITED extended style has been added to some view windows.
void CMainFrame::RecalcDockLayout()
{
    if (GetWinVersion() >= 3000)  // Windows 10 or later.
    {
        if (GetDockAncestor()->IsWindow())
        {
            GetTopmostDocker()->LockWindowUpdate();
            CRect rc = GetTopmostDocker()->GetViewRect();
            GetTopmostDocker()->RecalcDockChildLayout(rc);
            GetTopmostDocker()->UnlockWindowUpdate();
            GetTopmostDocker()->UpdateWindow();
        }
    }
    else
        CDocker::RecalcDockLayout();
}

// Saves the docking arrangement and other settings in the registry.
BOOL CMainFrame::SaveRegistrySettings()
{
    if (CDockFrame::SaveRegistrySettings())
        return SaveDockRegistrySettings(GetRegistryKeyName());
    else
        return FALSE;
}

// Positions the tabs at the top or bottom of all containers.
void CMainFrame::SetContainerTabsAtTop(bool isAtTop)
{
    m_isContainerTabsAtTop = isAtTop;

    // Set the Tab position for each container
    for (CDocker* docker : GetAllDockers())
    {
        CDockContainer* pContainer = docker->GetContainer();
        if (pContainer && pContainer->IsWindow())
        {
            pContainer->SetTabsAtTop(isAtTop);
        }
    }
}

// Specify the icons used in popup menus.
void CMainFrame::SetupMenuIcons()
{
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        SetMenuIcons(data, RGB(192, 192, 192), IDW_MAIN);
    else
        SetMenuIcons(data, RGB(192, 192, 192), IDB_TOOLBAR16);
}

// Set the Resource IDs for the toolbar buttons
void CMainFrame::SetupToolBar()
{
    AddToolBarButton( IDM_FILE_NEW,   FALSE );
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

// Overrides base class function to supress container group undocking.
void CMainFrame::UndockContainerGroup()
{
}

// Process the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_GETMINMAXINFO:   return OnGetMinMaxInfo(msg, wparam, lparam);
        }

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
