/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"


//////////////////////////////////
// CMainFrame function definitions
//

// Constructor.
CMainFrame::CMainFrame()
{
    m_useProportionalResize = FALSE;
    m_use3DBorder = TRUE;
    m_disableUndocking = FALSE;
    m_disableResize = FALSE;
    m_disableDockLR = FALSE;
    m_disableDockClose = FALSE;
    m_useDynamicResize = TRUE;

    //Set m_View as the view window of the frame
    SetView(m_view);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\Dock"));
}

// Destructor.
CMainFrame::~CMainFrame()
{
}

// Loads a default configuration of dockers.
void CMainFrame::LoadDefaultDockers()
{
    // Note: The  DockIDs are used for saving/restoring the dockers state in the registry

    CDocker* pDockLeft = AddDockedChild(new CDockClasses, DS_DOCKED_LEFT, 200, ID_DOCK_CLASSES1);
    CDocker* pDockRight = AddDockedChild(new CDockClasses, DS_DOCKED_RIGHT, 200, ID_DOCK_CLASSES2);
    CDocker* pDockTop = AddDockedChild(new CDockText, DS_DOCKED_TOP, 100, ID_DOCK_TEXT1);
    CDocker* pDockBottom = AddDockedChild(new CDockText, DS_DOCKED_BOTTOM, 100, ID_DOCK_TEXT2);

    pDockLeft->AddDockedChild(new CDockFiles, DS_DOCKED_BOTTOM, 150, ID_DOCK_FILES1);
    pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_BOTTOM, 150, ID_DOCK_FILES2);
    pDockTop->AddDockedChild(new CDockSimple, DS_DOCKED_RIGHT, 100, ID_DOCK_SIMPLE1);
    pDockBottom->AddDockedChild(new CDockSimple, DS_DOCKED_RIGHT, 100, ID_DOCK_SIMPLE2);

    // Adjust dockstyles as per menu selections
    SetDockStyles();
}

// Adds a new docker. The id specifies the dock type.
CDocker* CMainFrame::NewDockerFromID(int id)
{
    CDocker* pDock = NULL;
    switch (id)
    {
    case ID_DOCK_CLASSES1:
        pDock = new CDockClasses;
        break;
    case ID_DOCK_CLASSES2:
        pDock = new CDockClasses;
        break;
    case ID_DOCK_FILES1:
        pDock = new CDockFiles;
        break;
    case ID_DOCK_FILES2:
        pDock = new CDockFiles;
        break;
    case ID_DOCK_SIMPLE1:
        pDock = new CDockSimple;
        break;
    case ID_DOCK_SIMPLE2:
        pDock = new CDockSimple;
        break;
    case ID_DOCK_TEXT1:
        pDock = new CDockText;
        break;
    case ID_DOCK_TEXT2:
        pDock = new CDockText;
        break;
    default:
        TRACE("Unknown Dock ID\n");
        break;
    }

    return pDock;
}

// Toggles the display of the 3D border.
BOOL CMainFrame::On3DBorder()
{
    m_use3DBorder = !m_use3DBorder;
    SetDockStyles();
    return TRUE;
}

// OnCommand responds to menu and and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_FILE_EXIT:         return OnFileExit();
    case IDM_DOCK_DEFAULT:      return OnDockDefault();
    case IDM_DOCK_CLOSEALL:     return OnDockCloseAll();
    case IDM_PROP_RESIZE:       return OnPropResize();
    case IDM_3DBORDER:          return On3DBorder();
    case IDM_NO_UNDOCK:         return OnNoUndocking();
    case IDM_NO_RESIZE:         return OnNoResize();
    case IDM_NO_DOCK_LR:        return OnNoDockLR();
    case IDM_NO_DOCK_CLOSE:     return OnNoDockClose();
    case IDM_DYNAMIC_RESIZE:    return OnDynamicResize();
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

    // UseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar
    // UseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar
    // UseReBar(FALSE);              // Don't use a ReBar
    // UseStatusBar(FALSE);          // Don't use a StatusBar
    // UseThemes(FALSE);             // Don't use themes
    // UseToolBar(FALSE);            // Don't use a ToolBar

    // call the base class function
    return CDockFrame::OnCreate(cs);
}

// Closes all of the dockers.
BOOL CMainFrame::OnDockCloseAll()
{
    CloseAllDockers();
    return TRUE;
}

// Dragging the docker's splitter bar will either dynamically resize the dockers
// during the dragging, or simply display a hashed splitter bar.
BOOL CMainFrame::OnDynamicResize()
{
    std::vector<CDocker*>::const_iterator iter;
    m_useDynamicResize = !m_useDynamicResize;

    for (iter = GetAllDockers().begin(); iter < GetAllDockers().end(); ++iter)
    {
        (*iter)->SetDragAutoResize(m_useDynamicResize);
    }
    return TRUE;
}

// Loads a default arrangement of dockers.
BOOL CMainFrame::OnDockDefault()
{
    SetRedraw(FALSE);   // Suppress drawing to the frame window
    CloseAllDockers();
    LoadDefaultDockers();
    SetRedraw(TRUE);    // Re-enable drawing to the frame window
    RedrawWindow(RDW_INVALIDATE|RDW_FRAME|RDW_UPDATENOW|RDW_ALLCHILDREN);
    return TRUE;
}

// Issues a close request to the frame to end the program.
BOOL CMainFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Called after the window is created.
void CMainFrame::OnInitialUpdate()
{
    SetDockStyle(DS_CLIENTEDGE);

    // Load dock settings
    if (!LoadDockRegistrySettings(GetRegistryKeyName()))
        LoadDefaultDockers();

    // Adjust dockstyles as per menu selections
    SetDockStyles();

    // PreCreate initially set the window as invisible, so show it now.
    ShowWindow(GetInitValues().showCmd);
}

// Updates the menu items before they are displayed.
void CMainFrame::OnMenuUpdate(UINT id)
{
    switch(id)
    {
    case IDM_PROP_RESIZE:
        GetFrameMenu().CheckMenuItem(id, MF_BYCOMMAND | (m_useProportionalResize ? MF_CHECKED : MF_UNCHECKED));
        break;
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
    case IDM_DYNAMIC_RESIZE:
        GetFrameMenu().CheckMenuItem(id, MF_BYCOMMAND | (m_useDynamicResize ? MF_CHECKED : MF_UNCHECKED));
        break;
    }

    CDockFrame::OnMenuUpdate(id);
}

// Toggles the ability to close of dockers.
BOOL CMainFrame::OnNoDockClose()
{
    m_disableDockClose = !m_disableDockClose;
    SetDockStyles();
    RedrawWindow();
    return TRUE;
}

// Toggles the ability to dock left or right.
BOOL CMainFrame::OnNoDockLR()
{
    m_disableDockLR = !m_disableDockLR;
    SetDockStyles();
    return TRUE;
}

// Togggles the ability to resize dockers using the splitter bar.
BOOL CMainFrame::OnNoResize()
{
    m_disableResize = !m_disableResize;
    SetDockStyles();
    return TRUE;
}

// Toggles the ability to undock a docker.
BOOL CMainFrame::OnNoUndocking()
{
    m_disableUndocking = !m_disableUndocking;
    SetDockStyles();
    return TRUE;
}

// Toggles proportional resizing.
BOOL CMainFrame::OnPropResize()
{
    m_useProportionalResize = !m_useProportionalResize;
    SetDockStyles();
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
    std::vector<CDocker*>::const_iterator iter;

    for (iter = GetAllDockers().begin(); iter < GetAllDockers().end(); ++iter)
    {
        DWORD style = (*iter)->GetDockStyle();

        // Filter unwanted styles
        style &= 0xF400F;

        // Add styles selected from the menu
        if (m_useProportionalResize)    style |= DS_NO_FIXED_RESIZE;
        if (m_use3DBorder)              style |= DS_CLIENTEDGE;
        if (m_disableUndocking)         style |= DS_NO_UNDOCK;
        if (m_disableResize)            style |= DS_NO_RESIZE;
        if (m_disableDockLR)            style |= DS_NO_DOCKCHILD_LEFT | DS_NO_DOCKCHILD_RIGHT;
        if (m_disableDockClose)         style |= DS_NO_CLOSE;

        (*iter)->SetDockStyle(style);
    }
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
