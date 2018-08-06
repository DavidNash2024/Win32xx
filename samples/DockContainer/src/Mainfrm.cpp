////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "mainfrm.h"
#include "Classes.h"
#include "Files.h"
#include "Output.h"
#include "Text.h"
#include "resource.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame() : m_isContainerTabsAtTop(FALSE), m_hideSingleTab(TRUE)
{
    // Constructor for CMainFrame. Its called after CFrame's constructor

    //Set m_View as the view window of the frame
    SetView(m_view);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\DockContainer"));
}

CMainFrame::~CMainFrame()
{
    // Destructor for CMainFrame.
}

void CMainFrame::HideSingleContainerTab(BOOL hideSingle)
{
    m_hideSingleTab = hideSingle;
    std::vector<CDocker*>::const_iterator iter;

    // Set the Tab position for each container
    for (iter = GetAllDockers().begin(); iter < GetAllDockers().end(); ++iter)
    {
        CDockContainer* pContainer = (*iter)->GetContainer();
        if (pContainer && pContainer->IsWindow())
        {
            pContainer->SetHideSingleTab(hideSingle);
        }
    }
}

void CMainFrame::LoadDefaultDockers()
{
    // Note: The  DockIDs are used for saving/restoring the dockers state in the registry

    DWORD style = DS_CLIENTEDGE; // The style added to each docker

                                   // Add the parent dockers
    CDocker* pDockRight = AddDockedChild(new CDockClasses, DS_DOCKED_RIGHT | style, 200, ID_DOCK_CLASSES1);
    CDocker* pDockBottom = AddDockedChild(new CDockText, DS_DOCKED_BOTTOM | style, 100, ID_DOCK_TEXT1);

    // Add the remaining dockers
    pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | style, 200, ID_DOCK_FILES1);
    pDockRight->AddDockedChild(new CDockClasses, DS_DOCKED_CONTAINER | style, 200, ID_DOCK_CLASSES2);
    pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | style, 200, ID_DOCK_FILES2);

    pDockBottom->AddDockedChild(new CDockOutput, DS_DOCKED_CONTAINER | style, 100, ID_DOCK_OUTPUT1);
    pDockBottom->AddDockedChild(new CDockText, DS_DOCKED_CONTAINER | style, 100, ID_DOCK_TEXT2);
    pDockBottom->AddDockedChild(new CDockOutput, DS_DOCKED_CONTAINER | style, 100, ID_DOCK_OUTPUT2);
}

CDocker* CMainFrame::NewDockerFromID(int id)
{
    CDocker* pDock = NULL;
    switch(id)
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
    case ID_DOCK_OUTPUT1:
        pDock = new CDockOutput;
        break;
    case ID_DOCK_OUTPUT2:
        pDock = new CDockOutput;
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

BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    // OnCommand responds to menu and and toolbar input
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

BOOL CMainFrame::OnContainerTabsAtTop()
// Reposition the tabs in the containers
{
    SetContainerTabsAtTop(!m_isContainerTabsAtTop);
    return TRUE;
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
    return CDockFrame::OnCreate(cs);
}

BOOL CMainFrame::OnDockDefault()
{
    SetRedraw(FALSE);
    CloseAllDockers();
    LoadDefaultDockers();
    HideSingleContainerTab(m_hideSingleTab);
    SetContainerTabsAtTop(m_isContainerTabsAtTop);
    SetRedraw(TRUE);
    RedrawWindow(RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
    return TRUE;
}

BOOL CMainFrame::OnDockCloseAll()
{
    CloseAllDockers();
    return TRUE;
}

BOOL CMainFrame::OnFileExit()
{
    // Issue a close request to the frame
    PostMessage(WM_CLOSE);
    return TRUE;
}

BOOL CMainFrame::OnHideSingleTab()
{
    HideSingleContainerTab(!m_hideSingleTab);
    return TRUE;
}

void CMainFrame::OnInitialUpdate()
{
    SetDockStyle(DS_CLIENTEDGE);

    // Load dock settings
    if (!LoadDockRegistrySettings(GetRegistryKeyName()))
        LoadDefaultDockers();

    // Hide the container's tab if it has just one tab
    HideSingleContainerTab(m_hideSingleTab);

    // PreCreate initially set the window as invisible, so show it now.
    ShowWindow( GetInitValues().ShowCmd );
}

void CMainFrame::OnMenuUpdate(UINT id)
// Called when menu items are about to be displayed
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

void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
    // Call the base class function first
    CDockFrame::PreCreate(cs);
    
    // Hide the window initially by removing the WS_VISIBLE style
    cs.style &= ~WS_VISIBLE;
}

BOOL CMainFrame::SaveRegistrySettings()
{
    if (CDockFrame::SaveRegistrySettings())
        return SaveDockRegistrySettings(GetRegistryKeyName());
    else
        return FALSE;
}

void CMainFrame::SetContainerTabsAtTop(BOOL isAtTop)
{
    m_isContainerTabsAtTop = isAtTop;
    std::vector<CDocker*>::const_iterator iter;

    // Set the Tab position for each container
    for (iter = GetAllDockers().begin(); iter < GetAllDockers().end(); ++iter)
    {
        CDockContainer* pContainer = (*iter)->GetContainer();
        if (pContainer && pContainer->IsWindow())
        {
            pContainer->SetTabsAtTop(isAtTop);
        }
    }
}

void CMainFrame::SetupToolBar()
{
    // Set the Resource IDs for the toolbar buttons
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
