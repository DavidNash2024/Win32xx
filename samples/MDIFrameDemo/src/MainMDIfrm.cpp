/////////////////////////////
// MainMDIfrm.cpp
//

#include "stdafx.h"
#include "MainMDIfrm.h"
#include "MDIChildSimple.h"
#include "MDIChildRect.h"
#include "MDIChildMax.h"
#include "MDIChildText.h"
#include "MDIChildTreeView.h"
#include "MDIChildListView.h"
#include "resource.h"

/////////////////////////////////////
// CMainMDIFrame function definitions
//

// Constructor.
CMainMDIFrame::CMainMDIFrame()
{
}

// Destructor.
CMainMDIFrame::~CMainMDIFrame()
{
}

// Create the MDI frame window.
HWND CMainMDIFrame::Create(HWND parent)
{
    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\MDIFrameDemo"));

    return CMDIFrame::Create(parent);
}

// Respond to input from the menu and toolbar.
BOOL CMainMDIFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UINT id = LOWORD(wparam);
    switch (id)
    {
    case IDM_FILE_NEW:       return OnFileNew();
    case IDM_FILE_NEWVIEW:   return OnFileNewView();
    case IDM_FILE_NEWRECT:   return OnFileNewRect();
    case IDM_FILE_NEWTEXT:   return OnFileNewText();
    case IDM_FILE_NEWMAX:    return OnFileNewMax();
    case IDM_FILE_NEWTREE:   return OnFileNewTree();
    case IDM_FILE_NEWLIST:   return OnFileNewList();
    case IDM_FILE_CLOSE:     return OnFileClose();
    case IDM_FILE_EXIT:      return OnFileExit();
    case IDM_HELP_ABOUT:     return OnHelp();
    case IDW_VIEW_STATUSBAR: return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:   return OnViewToolBar();
    case IDW_MDI_ARRANGE:    return OnMDIIconArrange();
    case IDW_MDI_CASCADE:    return OnMDICascade();
    case IDW_MDI_CLOSEALL:   return OnMDICloseAll();
    case IDW_MDI_TILE:       return OnMDITile();
    default:    // Pass to active child...
        {
            if (GetActiveMDIChild())
                GetActiveMDIChild()->SendMessage(WM_COMMAND, wparam, lparam);
        }
        break ;

    }

    return FALSE;
}

// Called when the MDI frame window is created.
int CMainMDIFrame::OnCreate(CREATESTRUCT& cs)
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
    return CMDIFrame::OnCreate(cs);
}

// Called after the window is created.
// Called after OnCreate.
void CMainMDIFrame::OnInitialUpdate()
{
    //The frame is now created.
    //Place any additional startup code here.
}

// Creates the popup menu when the "New" toolbar button is pressed
BOOL CMainMDIFrame::OnFileNew()
{
    // Position the popup menu
    CToolBar& tb = GetToolBar();
    RECT rc = tb.GetItemRect(tb.CommandToIndex(IDM_FILE_NEW));
    tb.MapWindowPoints(0, (LPPOINT)&rc, 2);

    TPMPARAMS tpm;
    tpm.cbSize = sizeof(tpm);
    tpm.rcExclude = rc;

    // Load the popup menu
    CMenu topMenu(IDM_NEWMENU);
    CMenu popupMenu = topMenu.GetSubMenu(0);

    // Start the popup menu
    popupMenu.TrackPopupMenuEx(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, rc.left, rc.bottom, *this, &tpm);

    return TRUE;
}

// Ask the active MDI window to close.
BOOL CMainMDIFrame::OnFileClose()
{
    GetActiveMDIChild()->Close();

    return TRUE;
}

// Issue a close request to the frame. This ends the application.
BOOL CMainMDIFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Adds a MDI child with a list view.
BOOL CMainMDIFrame::OnFileNewList()
{
    AddMDIChild(new CMDIChildList); // This pointer is stored in a Shared_Ptr
    return TRUE;
}

// Adds a maximised MDI child.
BOOL CMainMDIFrame::OnFileNewMax()
{
    AddMDIChild(new CMDIChildMax);  // This pointer is stored in a Shared_Ptr
    return TRUE;
}

// Adds a MDI child displaying random rectangles.
BOOL CMainMDIFrame::OnFileNewRect()
{
    AddMDIChild(new CMDIChildRect); // This pointer is stored in a Shared_Ptr
    return TRUE;
}

// Adds a MDI child with a text view.
BOOL CMainMDIFrame::OnFileNewText()
{
    AddMDIChild(new CMDIChildText); // This pointer is stored in a Shared_Ptr
    return TRUE;
}

// Adds a MDI child with a tree view.
BOOL CMainMDIFrame::OnFileNewTree()
{
    AddMDIChild(new CMDIChildTree); // This pointer is stored in a Shared_Ptr
    return TRUE;
}

// Adds a MDI child with a simple view.
BOOL CMainMDIFrame::OnFileNewView()
{
    AddMDIChild(new CMDIChildSimple);   // This pointer is stored in a Shared_Ptr
    return TRUE;
}

// Arrange the MDI children in cascade mode.
BOOL CMainMDIFrame::OnMDICascade()
{
    MDICascade();
    return TRUE;
}

// Ask all the MDI children to close.
BOOL CMainMDIFrame::OnMDICloseAll()
{
    RemoveAllMDIChildren();
    return TRUE;
}

// Arrange all minimised MDI children.
BOOL CMainMDIFrame::OnMDIIconArrange()
{
    MDIIconArrange();
    return TRUE;
}

// Arrange the MDI children in tile mode.
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

    return CMDIFrame::OnNotify(wparam, lparam);
}

// Configure the images for menu items.
void CMainMDIFrame::SetupMenuIcons()
{
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        SetMenuIcons(data, RGB(192, 192, 192), IDW_MAIN, IDB_TOOLBAR24_DIS);
    else
        SetMenuIcons(data, RGB(192, 192, 192), IDB_TOOLBAR16);

    // Add some extra icons for menu items
    AddMenuIcon(IDM_FILE_NEWVIEW, IDI_VIEW);
    AddMenuIcon(IDM_FILE_NEWRECT, IDI_RECT);
    AddMenuIcon(IDM_FILE_NEWMAX, IDI_MAX);
    AddMenuIcon(IDM_FILE_NEWTEXT, IDI_TEXT);
    AddMenuIcon(IDM_FILE_NEWLIST, IDI_FILES);
    AddMenuIcon(IDM_FILE_NEWTREE, IDI_CLASSES);
}

// Define the resource IDs and images for toolbar buttons.
void CMainMDIFrame::SetupToolBar()
{
    // Define the resource IDs for the toolbar
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

    // Use larger buttons with seperate imagelists for normal, hot and disabled buttons.
    SetToolBarImages(RGB(192,192,192), IDW_MAIN, IDB_TOOLBAR24_HOT, IDB_TOOLBAR24_DIS);

    // Configure the "New" toolbar button to bring up a menu
    // Setting this style requires comctl32.dll version 4.72 or later
    if (GetComCtlVersion() >= 472)
    {
        GetToolBar().SetButtonStyle(IDM_FILE_NEW, BTNS_WHOLEDROPDOWN);
    }
}

// Process the MDI frame's window messages.
LRESULT CMainMDIFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {

    //  switch (msg)
    //  {
    //  Add case statements for each messages to be handled here
    //  }

        //  Pass unhandled messages on for default processing.
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

