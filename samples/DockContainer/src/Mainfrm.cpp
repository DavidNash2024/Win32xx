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


//////////////////////////////////
// CMainFrame function definitions
//

// Constructor for CMainFrame.
CMainFrame::CMainFrame() : m_isContainerTabsAtTop(false), m_hideSingleTab(true)
{
}

// Destructor for CMainFrame.
CMainFrame::~CMainFrame()
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    // Set m_view as the view window of the frame.
    SetView(m_view);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\DockContainer"));

    return CDockFrame::Create(parent);
}

// Adjusts the dockers in response to window DPI changes.
// Required for per-monitor DPI-aware.
void CMainFrame::DPIScaleDockers()
{
    std::vector<CDocker*> v = GetAllDockers();
    std::vector<CDocker*>::iterator it;
    for (it = v.begin(); it != v.end(); ++it)
    {
        if ((*it)->IsWindow())
        {
            // Reset the docker size.
            int size = (*it)->GetDockSize();
            (*it)->SetDockSize(size);

            // Notify the docker that the DPI has changed.
            (*it)->SendMessage(UWM_DPICHANGED, 0, 0);
        }
    }

    RecalcDockLayout();
}

// Assigns the appropriately sized menu icons.
// Required for per-monitor DPI-aware.
void CMainFrame::DPIScaleMenuIcons()
{
    // Load the toolbar bitmap.
    CBitmap toolbarImage(IDW_MAIN);

    // Scale the bitmap to the menu item height.
    int menuHeight = GetMenuIconHeight();
    int scale = menuHeight / toolbarImage.GetSize().cy;
    CBitmap scaledImage;
    if (scale > 0)
        scaledImage = ScaleUpBitmap(toolbarImage, scale);
    else
        scaledImage.LoadBitmap(IDB_TOOLBAR16);

    // Create the image-list from the scaled image
    CSize sz = scaledImage.GetSize();
    m_menuImages.Create(sz.cy, sz.cy, ILC_COLOR32 | ILC_MASK, 0, 0);
    COLORREF mask = RGB(192, 192, 192);
    m_menuImages.Add(scaledImage, mask);

    // Assign the image-list to the menu items.
    SetMenuImages(m_menuImages);
}

// Assigns the appropriately sized toolbar icons.
// Required for per-monitor DPI-aware.
void CMainFrame::DPIScaleToolBar()
{
    if (GetToolBar().IsWindow())
    {
        // Reset the toolbar images.
        SetToolBarImages(RGB(192, 192, 192), IDW_MAIN, 0, 0);
    }
}

// Hides or shows the tab for a container with a single tab.
void CMainFrame::HideSingleContainerTab(bool hideSingle)
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

// Loads the default arrangement of dockers.
void CMainFrame::LoadDefaultDockers()
{
    // Note: The  DockIDs are used for saving/restoring the dockers state in the registry

    DWORD style = DS_CLIENTEDGE; // The style added to each docker

    // Add the right most dockers
    CDocker* pDockRight = AddDockedChild(new CDockClasses, DS_DOCKED_RIGHT | style, DPIScaleInt(200), ID_DOCK_CLASSES1);
    pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | style, DPIScaleInt(200), ID_DOCK_FILES1);
    pDockRight->AddDockedChild(new CDockClasses, DS_DOCKED_CONTAINER | style, DPIScaleInt(200), ID_DOCK_CLASSES2);
    pDockRight->AddDockedChild(new CDockFiles, DS_DOCKED_CONTAINER | style, DPIScaleInt(200), ID_DOCK_FILES2);

    // Add the bottom dockers
    CDocker* pDockBottom = AddDockedChild(new CDockOutput, DS_DOCKED_BOTTOM | style, DPIScaleInt(100), ID_DOCK_OUTPUT1);
    pDockBottom->AddDockedChild(new CDockOutput, DS_DOCKED_CONTAINER | style, DPIScaleInt(100), ID_DOCK_OUTPUT2);

    // Add the frame's dockers
    AddDockedChild(new CDockText, DS_DOCKED_CONTAINER | style, DPIScaleInt(100), ID_DOCK_TEXT1);
    AddDockedChild(new CDockText, DS_DOCKED_CONTAINER | style, DPIScaleInt(100), ID_DOCK_TEXT2);

    DPIScaleDockers();
}

// Adds a new docker. The id specifies its type.
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

// Called when the effective dots per inch (dpi) for a window has changed.
// This occurs when:
//  - The window is moved to a new monitor that has a different DPI.
//  - The DPI of the monitor hosting the window changes.
LRESULT CMainFrame::OnDPIChanged(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Suppress redraw to render the DPI changes smoothly.
    SetRedraw(FALSE);

    CDockFrame::OnDPIChanged(msg, wparam, lparam);
    DPIScaleDockers();
    DPIScaleMenuIcons();
    DPIScaleToolBar();
    RecalcLayout();

    // Enable redraw and redraw the frame.
    SetRedraw(TRUE);
    RedrawWindow();
    return 0;
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

// Toggle the hiding of tabs for containers with a single tab.
BOOL CMainFrame::OnHideSingleTab()
{
    HideSingleContainerTab(!m_hideSingleTab);
    return TRUE;
}

// Called after the frame's window is created.
void CMainFrame::OnInitialUpdate()
{
    SetDockStyle(DS_CLIENTEDGE);

    // Load dock settings
    if (!LoadDockRegistrySettings(GetRegistryKeyName()))
        LoadDefaultDockers();

    // Hide the container's tab if it has just one tab
    HideSingleContainerTab(m_hideSingleTab);

    DPIScaleDockers();

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

// Specify the icons used in popup menus.
void CMainFrame::SetupMenuIcons()
{
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        SetMenuIcons(data, RGB(192, 192, 192), IDW_MAIN);
    else
        SetMenuIcons(data, RGB(192, 192, 192), IDB_TOOLBAR16);

    // Update the menu icons
    DPIScaleMenuIcons();
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
