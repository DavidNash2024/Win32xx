/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"

///////////////////////////////////
// CMainFrame function definitions.
//
CMainFrame::CMainFrame() : m_pDockText(0), m_pDockTree(0), m_pDockList(0)
{
}

CMainFrame::~CMainFrame()
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    SetView(m_mainView);

    // Set the registry key name, and load the initial window position.
    // Use a registry key name like "CompanyName\\Application".
    LoadRegistrySettings(_T("Win32++\\Splitter Sample"));

    return CDockFrame::Create(parent);
}

// Adjusts dockers when the window DPI changes.
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
void CMainFrame::DPIScaleToolBar()
{
    if (GetToolBar().IsWindow())
    {
        // Load the toolbar bitmap.
        CBitmap toolbarImage(IDW_MAIN);

        // Create the image-list
        CBitmap dpiImage = DPIScaleUpBitmap(toolbarImage);
        CSize sz = dpiImage.GetSize();
        m_normalImages.Create(sz.cy, sz.cy, ILC_COLOR32 | ILC_MASK, 0, 0);
        COLORREF mask = RGB(192, 192, 192);
        m_normalImages.Add(dpiImage, mask);

        // Assign the image-list to the toolbar.
        GetToolBar().SetImageList(m_normalImages);
        GetToolBar().SetDisableImageList(0);

        // Adjust the toolbar band height.
        if (GetReBar().IsWindow())
        {
            int band = GetReBar().GetBand(GetToolBar());
            if (band >= 0)
            {
                CSize sizeToolBar = GetToolBar().GetMaxSize();
                GetReBar().ResizeBand(band, sizeToolBar);
            }
        }
    }
}

// Load the default arrangement of the window panes.
void CMainFrame::LoadDefaultWindowPanes()
{
    // Set the styles for splitter panes.
    DWORD style = DS_NO_UNDOCK | DS_NO_CAPTION | DS_CLIENTEDGE;
    SetDockStyle(style);

    CRect viewRect = GetViewRect();

    // Add the bottom pane first. It is a child of the main pane.
    m_pDockTree = static_cast<CDockTree*>(AddDockedChild(new CDockTree, style | DS_DOCKED_BOTTOM, viewRect.Height() / 2, ID_DOCK_TREE));

    // Add the bottom right pane. It is a child of the bottom pane.
    m_pDockList = static_cast<CDockList*>(m_pDockTree->AddDockedChild(new CDockList, style | DS_DOCKED_RIGHT, viewRect.Width() / 2, ID_DOCK_LIST));

    // Add the top right pane. It is a child of the main pane.
    m_pDockText = static_cast<CDockText*>(AddDockedChild(new CDockText, style | DS_DOCKED_RIGHT, viewRect.Width() / 2, ID_DOCK_TEXT));
}

// Adds a new docker. The id specifies the dock type.
// The id is used by LoadDockRegistrySettings to restore the
// previous window pane arrangement.
CDocker* CMainFrame::NewDockerFromID(int id)
{
    CDocker* pDock = NULL;
    switch (id)
    {
    case ID_DOCK_LIST:
        pDock = new CDockList;
        break;
    case ID_DOCK_TREE:
        pDock = new CDockTree;
        break;
    case ID_DOCK_TEXT:
        pDock = new CDockText;
        break;
    default:
        TRACE("Unknown Dock ID\n");
        break;
    }

    return pDock;
}

// Responds to menu and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch(id)
    {
    case IDM_FILE_EXIT:         return OnFileExit();
    case IDM_VIEW_TEXT:         return OnViewText();
    case IDM_VIEW_LIST:         return OnViewList();
    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDM_HELP_ABOUT:        return OnHelp();
    }
    return FALSE;
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
    RecalcDockLayout();
    RecalcLayout();

    // Enable redraw and redraw the frame.
    SetRedraw(TRUE);
    RedrawWindow();
    return 0;
}

// Called from the file exit menu command.
BOOL CMainFrame::OnFileExit()
{
    // Issue a close request to the frame
    Close();
    return TRUE;
}

// Called after the frame window is created.
void CMainFrame::OnInitialUpdate()
{
    if (LoadDockRegistrySettings(GetRegistryKeyName()))
    {
        DWORD style = DS_NO_UNDOCK | DS_NO_CAPTION | DS_CLIENTEDGE;
        SetDockStyle(style);
        m_pDockText = dynamic_cast<CDockText*>(GetDockFromID(ID_DOCK_TEXT));
        m_pDockTree = dynamic_cast<CDockTree*>(GetDockFromID(ID_DOCK_TREE));
        m_pDockList = dynamic_cast<CDockList*>(GetDockFromID(ID_DOCK_LIST));

        if ((m_pDockText == 0) || (m_pDockTree == 0) || (m_pDockList == 0))
        {
            CloseAllDockers();
            LoadDefaultWindowPanes();
        }
    }
    else
    {
        // Load the default arrangement of the window panes.
        LoadDefaultWindowPanes();
    }

    DPIScaleDockers();

    // PreCreate initially set the window as invisible, so show it now.
    ShowWindow(GetInitValues().showCmd);
}

// Hides or shows the ListView window pane.
BOOL CMainFrame::OnViewList()
{
    DWORD style = DS_NO_UNDOCK | DS_NO_CAPTION | DS_CLIENTEDGE;
    if (m_pDockList->IsDocked())
    {
        m_pDockList->Hide();
        GetFrameMenu().CheckMenuItem(IDM_VIEW_LIST, MF_UNCHECKED);
    }
    else
    {
        m_pDockTree->Dock(m_pDockList, style | DS_DOCKED_RIGHT);
        GetFrameMenu().CheckMenuItem(IDM_VIEW_LIST, MF_CHECKED);
    }

    return TRUE;
}

// Hides or shows the text window pane.
BOOL CMainFrame::OnViewText()
{
    DWORD style = DS_NO_UNDOCK | DS_NO_CAPTION | DS_CLIENTEDGE;

    if (m_pDockText->IsDocked())
    {
        m_pDockText->Hide();
        GetFrameMenu().CheckMenuItem(IDM_VIEW_TEXT, MF_UNCHECKED);
    }
    else
    {
        Dock(m_pDockText, style | DS_DOCKED_RIGHT);
        GetFrameMenu().CheckMenuItem(IDM_VIEW_TEXT, MF_CHECKED);
    }

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

// Specifies the images used on menu items.
void CMainFrame::SetupMenuIcons()
{
    // Use the MenuIcons bitmap for images in menu items.
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        AddMenuIcons(data, RGB(192, 192, 192), IDW_MAIN);
    else
        AddMenuIcons(data, RGB(192, 192, 192), IDB_TOOLBAR16);
}

// Set the resource IDs and images for the toolbar buttons.
void CMainFrame::SetupToolBar()
{
    // Define the resource IDs for the toolbar
    AddToolBarButton( IDM_FILE_NEW, FALSE );
    AddToolBarButton( IDM_FILE_OPEN, FALSE );
    AddToolBarButton( IDM_FILE_SAVE, FALSE );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_EDIT_CUT, FALSE );
    AddToolBarButton( IDM_EDIT_COPY, FALSE );
    AddToolBarButton( IDM_EDIT_PASTE, FALSE );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_FILE_PRINT, FALSE );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_HELP_ABOUT );
}

// Process the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
    //  switch (msg)
    //  {
    //  Add case statements for each messages to be handled here.
    //  }

        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}

