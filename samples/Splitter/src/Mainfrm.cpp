/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"

using namespace std;

///////////////////////////////////
// CMainFrame function definitions.
//
CMainFrame::CMainFrame() : m_pDockText(nullptr), m_pDockTree(nullptr), m_pDockList(nullptr)
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    // Set m_mainView as the view window of the frame.
    SetView(m_mainView);

    // Set the registry key name, and load the initial window position.
    // Use a registry key name like "CompanyName\\Application".
    LoadRegistrySettings(L"Win32++\\Splitter Sample");

    return CDockFrame::Create(parent);
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
DockPtr CMainFrame::NewDockerFromID(int id)
{
    DockPtr docker;
    switch (id)
    {
    case ID_DOCK_LIST:
        docker = make_unique<CDockList>();
        break;
    case ID_DOCK_TREE:
        docker = make_unique<CDockTree>();
        break;
    case ID_DOCK_TEXT:
        docker = make_unique<CDockText>();
        break;
    default:
        TRACE("Unknown Dock ID\n");
        break;
    }

    return docker;
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

// Called from the file exit menu command.
BOOL CMainFrame::OnFileExit()
{
    // Issue a close request to the frame
    Close();
    return TRUE;
}

// Limit the minimum size of the window.
LRESULT CMainFrame::OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam)
{
    LPMINMAXINFO lpMMI = (LPMINMAXINFO)lparam;
    const CSize minimumSize(400, 350);
    lpMMI->ptMinTrackSize.x = DpiScaleInt(minimumSize.cx);
    lpMMI->ptMinTrackSize.y = DpiScaleInt(minimumSize.cy);
    return FinalWindowProc(msg, wparam, lparam);
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

        if ((m_pDockText == nullptr) || (m_pDockTree == nullptr) || (m_pDockList == nullptr))
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
        switch (msg)
        {
        case WM_GETMINMAXINFO:    return OnGetMinMaxInfo(msg, wparam, lparam);
        }

        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

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

