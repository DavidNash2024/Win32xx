/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"

using namespace std;

//////////////////////////////////
// CMainFrame function definitions
//

// Constructor.
CMainFrame::CMainFrame()
{
    SetDockStylesToDefault();
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    // Set m_view as the view window of the frame.
    SetView(m_view);

    // Set the registry key name, and load the initial window position.
    // Use a registry key name like "CompanyName\\Application".
    LoadRegistrySettings(L"Win32++\\Dock");

    return CDockFrame::Create(parent);
}

// Loads a default configuration of dockers.
void CMainFrame::LoadDefaultDockers()
{
    // Note: The  DockIDs are used for saving/restoring the dockers state in the registry

    CDocker* pDockLeft = AddDockedChild(make_unique<CDockClasses>(), DS_DOCKED_LEFT, DpiScaleInt(200), ID_DOCK_CLASSES1);
    CDocker* pDockRight = AddDockedChild(make_unique<CDockClasses>(), DS_DOCKED_RIGHT, DpiScaleInt(200), ID_DOCK_CLASSES2);
    CDocker* pDockTop = AddDockedChild(make_unique<CDockText>(), DS_DOCKED_TOP, DpiScaleInt(100), ID_DOCK_TEXT1);
    CDocker* pDockBottom = AddDockedChild(make_unique<CDockText>(), DS_DOCKED_BOTTOM, DpiScaleInt(100), ID_DOCK_TEXT2);

    pDockLeft->AddDockedChild(make_unique<CDockFiles>(), DS_DOCKED_BOTTOM, DpiScaleInt(150), ID_DOCK_FILES1);
    pDockRight->AddDockedChild(make_unique<CDockFiles>(), DS_DOCKED_BOTTOM, DpiScaleInt(150), ID_DOCK_FILES2);
    pDockTop->AddDockedChild(make_unique<CDockSimple>(), DS_DOCKED_RIGHT, DpiScaleInt(100), ID_DOCK_SIMPLE1);
    pDockBottom->AddDockedChild(make_unique<CDockSimple>(), DS_DOCKED_RIGHT, DpiScaleInt(100), ID_DOCK_SIMPLE2);

    // Adjust dockstyles as per menu selections
    SetDockStyles();
}

// Adds a new docker. The id specifies the dock type.
DockPtr CMainFrame::NewDockerFromID(int id)
{
    DockPtr docker;
    switch (id)
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
    case ID_DOCK_SIMPLE1:
        docker = make_unique<CDockSimple>();
        break;
    case ID_DOCK_SIMPLE2:
        docker = make_unique<CDockSimple>();
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

// Toggle the display of a 3D border.
BOOL CMainFrame::On3DBorder()
{
    m_use3DBorder = !m_use3DBorder;
    SetDockStyles();
    return TRUE;
}

// OnCommand responds to menu and and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_FILE_EXIT:         return OnFileExit();
    case IDM_DOCK_DEFAULT:      return OnDockDefault();
    case IDM_STYLES_DEFAULT:    return OnStylesDefault();
    case IDM_DOCK_CLOSEALL:     return OnDockCloseAll();
    case IDM_3DBORDER:          return On3DBorder();
    case IDM_NO_UNDOCK:         return OnNoUndocking();
    case IDM_NO_RESIZE:         return OnNoResize();
    case IDM_NO_DOCK_LR:        return OnNoDockLR();
    case IDM_NO_DOCK_CAPTION:   return OnNoDockCaption();
    case IDM_NO_DOCK_CLOSE:     return OnNoDockClose();
    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDM_HELP_ABOUT:        return OnHelp();
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
    return CDockFrame::OnCreate(cs);
}

// Closes all of the dockers.
BOOL CMainFrame::OnDockCloseAll()
{
    CloseAllDockers();
    return TRUE;
}

// Loads a default arrangement of dockers.
BOOL CMainFrame::OnDockDefault()
{
    // Suppress redraw to render the docking changes smoothly.
    SetRedraw(FALSE);

    CloseAllDockers();
    LoadDefaultDockers();

    // Enable redraw and redraw the frame.
    SetRedraw(TRUE);
    RedrawWindow();
    return TRUE;
}

// Issues a close request to the frame to end the program.
BOOL CMainFrame::OnFileExit()
{
    Close();
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

// Called after the window is created.
void CMainFrame::OnInitialUpdate()
{
    // Attempt to load  dock settings from the registry.
    if (!LoadDockRegistrySettings(GetRegistryKeyName()))
    {
        // Load the default dock arrangement if loading from the registry fails.
        LoadDefaultDockers();
    }
    else
    {
        // Set menu options from the frame's current dock style.
        DWORD style = GetDockStyle();

        m_use3DBorder         = (style & DS_CLIENTEDGE) != 0;
        m_disableUndocking    = (style & DS_NO_UNDOCK) != 0;
        m_disableResize       = (style & DS_NO_RESIZE) != 0;
        m_disableDockLR       = (style & DS_NO_DOCKCHILD_LEFT) != 0;
        m_disableDockClose    = (style & DS_NO_CLOSE) != 0;
        m_disableDockCaption  = (style & DS_NO_CAPTION) != 0;
    }

    // PreCreate initially set the window as invisible, so show it now.
    ShowWindow(GetInitValues().showCmd);
}

// Updates the menu items before they are displayed.
void CMainFrame::OnMenuUpdate(UINT id)
{
    switch(id)
    {
    case IDM_3DBORDER:
        GetFrameMenu().CheckMenuItem(id, MF_BYCOMMAND | (m_use3DBorder ? MF_CHECKED : MF_UNCHECKED));
        break;
    case IDM_NO_UNDOCK:
        GetFrameMenu().CheckMenuItem(id, MF_BYCOMMAND | (m_disableUndocking ? MF_CHECKED : MF_UNCHECKED));
        break;
    case IDM_NO_RESIZE:
        GetFrameMenu().CheckMenuItem(id, MF_BYCOMMAND | (m_disableResize ? MF_CHECKED : MF_UNCHECKED));
        break;
    case IDM_NO_DOCK_LR:
        GetFrameMenu().CheckMenuItem(id, MF_BYCOMMAND | (m_disableDockLR ? MF_CHECKED : MF_UNCHECKED));
        break;
    case IDM_NO_DOCK_CLOSE:
        GetFrameMenu().CheckMenuItem(id, MF_BYCOMMAND | (m_disableDockClose ? MF_CHECKED : MF_UNCHECKED));
        break;
    case IDM_NO_DOCK_CAPTION:
        GetFrameMenu().CheckMenuItem(id, MF_BYCOMMAND | (m_disableDockCaption ? MF_CHECKED : MF_UNCHECKED));
        break;
    }

    CDockFrame::OnMenuUpdate(id);
}

// Toggle the docking caption.
BOOL CMainFrame::OnNoDockCaption()
{
    m_disableDockCaption = !m_disableDockCaption;
    SetDockStyles();
    RedrawWindow();
    return TRUE;
}

// Toggle the ability to close of dockers.
BOOL CMainFrame::OnNoDockClose()
{
    m_disableDockClose = !m_disableDockClose;
    SetDockStyles();
    RedrawWindow();
    return TRUE;
}

// Toggle the ability to dock left or right.
BOOL CMainFrame::OnNoDockLR()
{
    m_disableDockLR = !m_disableDockLR;
    SetDockStyles();
    return TRUE;
}

// Toggle the ability to resize dockers using the splitter bar.
BOOL CMainFrame::OnNoResize()
{
    m_disableResize = !m_disableResize;
    SetDockStyles();
    return TRUE;
}

// Toggle the ability to undock a docker.
BOOL CMainFrame::OnNoUndocking()
{
    m_disableUndocking = !m_disableUndocking;
    SetDockStyles();
    return TRUE;
}

BOOL CMainFrame::OnStylesDefault()
{
    SetDockStylesToDefault();
    RecalcDockLayout();
    return TRUE;
}

// Specify the CREATESTRUCT parameters before the window is created.
void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
    // Call base class to set defaults
    CDockFrame::PreCreate(cs);

    // Hide the window initially by removing the WS_VISIBLE style
    cs.style &= ~WS_VISIBLE;
}

// This function overrides CDocker::RecalcDockLayout to elimate jitter
// when the dockers are resized. The technique used here is is most
// appropriate for a complex arrangement of dockers. It might not suite
// other docking applications. To support this technique the
// WS_EX_COMPOSITED extended style has been added to each docker.
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

// Save the docking configuration in the registry.
BOOL CMainFrame::SaveRegistrySettings()
{
    if (CDockFrame::SaveRegistrySettings())
        return SaveDockRegistrySettings(GetRegistryKeyName());
    else
        return FALSE;
}

// Sets the style for all the dockers.
void CMainFrame::SetDockStyles()
{
    for (CDocker* docker : GetAllDockers())
    {
        DWORD style = docker->GetDockStyle();

        // Filter unwanted styles
        style &= 0xF000F;

        // Add styles selected from the menu
        if (m_use3DBorder)              style |= DS_CLIENTEDGE;
        if (m_disableUndocking)         style |= DS_NO_UNDOCK;
        if (m_disableResize)            style |= DS_NO_RESIZE;
        if (m_disableDockLR)            style |= DS_NO_DOCKCHILD_LEFT | DS_NO_DOCKCHILD_RIGHT;
        if (m_disableDockClose)         style |= DS_NO_CLOSE;
        if (m_disableDockCaption)       style |= DS_NO_CAPTION;

        docker->SetDockStyle(style);
    }
}

void CMainFrame::SetDockStylesToDefault()
{
    m_use3DBorder = true;
    m_disableUndocking = false;
    m_disableResize = false;
    m_disableDockLR = false;
    m_disableDockClose = false;
    m_disableDockCaption = false;

    SetDockStyles();
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

// Sets the Resource IDs for the toolbar buttons
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

// Process the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_GETMINMAXINFO:    return OnGetMinMaxInfo(msg, wparam, lparam);
        }

        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception.
        CString str;
        str << e.GetText() << L'\n' << e.GetErrorString();
        ::MessageBox(nullptr, str, L"An exception occurred", MB_ICONERROR);
        return 0;
    }
}
