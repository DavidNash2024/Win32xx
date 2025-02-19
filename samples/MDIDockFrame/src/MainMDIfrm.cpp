/////////////////////////////
// MainMDIfrm.cpp
//

#include "stdafx.h"
#include "MainMDIfrm.h"
#include "SimpleMDIChild.h"
#include "Files.h"
#include "resource.h"

using namespace std;

/////////////////////////////////////
// CMainMDIFrame function definitions
//

// Create the MDI frame window.
HWND CMainMDIFrame::Create(HWND parent)
{
    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(L"Win32++\\DockMDI Frame");

    return CMDIDockFrame::Create(parent);
}

// Sample code to load data from a file.
BOOL CMainMDIFrame::OnFileOpen()
{
    CFileDialog fileDlg(TRUE);

    // Bring up the file open dialog retrieve the selected filename
    if (fileDlg.DoModal(*this) == IDOK)
    {
        // TODO:
        // Add your own code here. Refer to the tutorial for additional information
    }

    return TRUE;
}

// Sample code to save data in a file.
BOOL CMainMDIFrame::OnFileSave()
{
    CFileDialog fileDlg(FALSE);

    // Bring up the file save dialog retrieve the selected filename
    if (fileDlg.DoModal(*this) == IDOK)
    {
        // TODO:
        // Add your own code here. Refer to the tutorial for additional information
    }

    return TRUE;
}

// Sample code for printing.
BOOL CMainMDIFrame::OnFilePrint()
{
    // Bring up a dialog to choose the printer
    PRINTDLG pd{};
    pd.lStructSize = sizeof( pd );
    pd.Flags = PD_RETURNDC;
    pd.hwndOwner = GetHwnd();

    // Retrieve the printer DC
    PrintDlg( &pd );

    // TODO:
    // Add your own code here. Refer to the tutorial for additional information

    return TRUE;
}

// Called after the frame window is created.
// Called after OnCreate.
void CMainMDIFrame::OnInitialUpdate()
{
    TRACE("MDI Frame started \n");
    //The frame is now created.
    //Place any additional startup code here.

    // Add some Dockers to the MDI Frame
    DWORD dwStyle = DS_CLIENTEDGE; // The style added to each docker
    int dockWidth = DpiScaleInt(150);
    CDocker* pDock1 = AddDockedChild(make_unique<CDockFiles>(), DS_DOCKED_LEFT | dwStyle, dockWidth);
    CDocker* pDock2 = AddDockedChild(make_unique<CDockFiles>(), DS_DOCKED_RIGHT | dwStyle, dockWidth);

    assert (pDock1->GetContainer());
    assert (pDock2->GetContainer());
    pDock1->GetContainer()->SetHideSingleTab(TRUE);
    pDock2->GetContainer()->SetHideSingleTab(TRUE);

    // Add some  MDI children
    AddMDIChild(make_unique<CSimpleMDIChild>());
    AddMDIChild(make_unique<CSimpleMDIChild>());
}

// Process input from the menu and toolbar.
BOOL CMainMDIFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UINT id = LOWORD(wparam);
    switch (id)
    {
    case IDM_FILE_NEWDOCK:      return OnFileNewDocker();
    case IDM_FILE_NEWMDI:       return OnFileNewMDI();
    case IDM_FILE_MDICLOSE:     return OnFileMDIClose();
    case IDM_FILE_OPEN:         return OnFileOpen();
    case IDM_FILE_SAVE:         return OnFileSave();
    case IDM_FILE_PRINT:        return OnFilePrint();
    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDW_MDI_ARRANGE:       return OnMDIIconArrange();
    case IDW_MDI_CASCADE:       return OnMDICascade();
    case IDW_MDI_CLOSEALL:      return OnMDICloseAll();
    case IDW_MDI_TILE:          return OnMDITile();
    case IDM_FILE_EXIT:         return OnFileExit();
    case IDM_HELP_ABOUT:        return OnHelp();
    default:
        {
            // Pass to active child...
            if (GetActiveMDIChild())
                GetActiveMDIChild()->SendMessage(WM_COMMAND, wparam, lparam);
        }
        break ;
    }
    return FALSE;
}

// Called when the window is created.
int CMainMDIFrame::OnCreate(CREATESTRUCT& cs)
{
    // OnCreate controls the way the frame is created.
    // Overriding CFrame::OnCreate is optional.

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
    return CMDIDockFrame::OnCreate(cs);
}

// Closes the active MDI child.
BOOL CMainMDIFrame::OnFileMDIClose()
{
    GetActiveMDIChild()->Close();
    return TRUE;
}

// Issue a close request to the frame to end the application.
BOOL CMainMDIFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Create a new MDI child
BOOL CMainMDIFrame::OnFileNewMDI()
{
    AddMDIChild(make_unique<CSimpleMDIChild>());
    return TRUE;
}

// Creates the popup menu when the "New" toolbar button is pressed
BOOL CMainMDIFrame::OnFileNew()
{
    // Position the popup menu
    CToolBar& tb = GetToolBar();
    RECT rc = tb.GetItemRect(tb.CommandToIndex(IDM_FILE_NEW));
    tb.MapWindowPoints(HWND_DESKTOP, (LPPOINT)&rc, 2);

    TPMPARAMS tpm;
    tpm.cbSize = sizeof(tpm);
    tpm.rcExclude = rc;

    // Load the popup menu
    CMenu topMenu(IDM_NEWMENU);
    CMenu popupMenu = topMenu.GetSubMenu(0);

    // Start the popup menu
    popupMenu.TrackPopupMenuEx(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
        rc.left, rc.bottom, *this, &tpm);

    return TRUE;
}

// Create a new docker.
BOOL CMainMDIFrame::OnFileNewDocker()
{
    int dockWidth = DpiScaleInt(150);
    CDocker* pDock = AddDockedChild(make_unique<CDockFiles>(),
        DS_DOCKED_LEFT | DS_CLIENTEDGE, dockWidth);
    pDock->GetContainer()->SetHideSingleTab(TRUE);
    return TRUE;
}

// Arrange the MDI child windows in cascade mode.
BOOL CMainMDIFrame::OnMDICascade()
{
    MDICascade();
    return TRUE;
}

// Close all MDI child windows.
BOOL CMainMDIFrame::OnMDICloseAll()
{
    RemoveAllMDIChildren();
    return TRUE;
}

// Arrange minimised MDI child windows.
BOOL CMainMDIFrame::OnMDIIconArrange()
{
    MDIIconArrange();
    return TRUE;
}

// Arrange the MDI child windows in tile mode.
BOOL CMainMDIFrame::OnMDITile()
{
    MDITile();
    return TRUE;
}

// Process notification messages (WM_NOTIFY) from child windows.
LRESULT CMainMDIFrame::OnNotify(WPARAM wparam, LPARAM lparam)
{
    // Notification from our dropdown button is received if Comctl32.dll version
    // is 4.70 or later (IE v3 required).
    LPNMHDR pHeader = reinterpret_cast<LPNMHDR>(lparam);
    switch (pHeader->code)
    {
        // Menu for dropdown toolbar button
    case TBN_DROPDOWN:
    {
        if (pHeader->hwndFrom == GetToolBar())
            OnFileNew();
    }
    break;

    } //switch LPNMHDR

    return CMDIDockFrame::OnNotify(wparam, lparam);
}

// Configures the images for menu items.
void CMainMDIFrame::SetupMenuIcons()
{
    // Set the bitmap used for menu icons
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        SetMenuIcons(data, RGB(192, 192, 192), IDW_MAIN);
    else
        SetMenuIcons(data, RGB(192, 192, 192), IDB_TOOLBAR16);

    // Add an extra icon for the New Docker menu item
    AddMenuIcon(IDM_FILE_NEWDOCK, IDI_DOCK);
    AddMenuIcon(IDM_FILE_NEWMDI, IDW_MAIN);
}

// Assign resource IDs and images to the toolbar buttons.
void CMainMDIFrame::SetupToolBar()
{
    // Define the resource IDs for the toolbar
    AddToolBarButton( IDM_FILE_NEW);
    AddToolBarButton( IDM_FILE_OPEN  );
    AddToolBarButton( IDM_FILE_SAVE  );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_EDIT_CUT   );
    AddToolBarButton( IDM_EDIT_COPY  );
    AddToolBarButton( IDM_EDIT_PASTE );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_FILE_PRINT );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_HELP_ABOUT );

    SetToolBarImages(RGB(192, 192, 192), IDW_MAIN, IDB_TOOLBAR24_HOT, IDB_TOOLBAR24_DIS);

    // Configure the "New" toolbar button to bring up a menu.
    GetToolBar().SetButtonStyle(IDM_FILE_NEW, BTNS_WHOLEDROPDOWN);
}

// Process the frame's window messages.
LRESULT CMainMDIFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
    //  switch (msg)
    //  {
    //  Add case statements for each messages to be handled here.
    //  }

        //  Pass unhandled messages on for default processing.
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
