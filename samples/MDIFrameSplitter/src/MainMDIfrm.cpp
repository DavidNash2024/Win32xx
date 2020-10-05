/////////////////////////////
// MainMDIfrm.cpp
//

#include "stdafx.h"
#include "mainMDIfrm.h"
#include "SplitterMDIChild.h"
#include "resource.h"

/////////////////////////////////////
// CMainMDIFrame function definitions
//

// Constructor.
CMainMDIFrame::CMainMDIFrame()
{
    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\MDIFrameSplitter"));
}

// Destructor.
CMainMDIFrame::~CMainMDIFrame()
{
}

// Respond to menu and toolbar input.
BOOL CMainMDIFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UINT id = LOWORD(wparam);
    switch (id)
    {
    case IDM_FILE_NEW:          return OnFileNew();
    case IDM_FILE_CLOSE:        return OnFileClose();
    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDW_MDI_ARRANGE:       return OnMDIIconArrange();
    case IDW_MDI_CASCADE:       return OnMDICascade();
    case IDW_MDI_CLOSEALL:      return OnMDICloseAll();
    case IDW_MDI_TILE:          return OnMDITile();
    case IDM_FILE_EXIT:         return OnFileExit();
    case IDM_HELP_ABOUT:        return OnHelp();
    default:    // Pass to active child...
        {
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
    TRACE("MDI Frame started \n");
    //The frame is now created.
    //Place any additional startup code here.
}

// Close the active MDI child window.
BOOL CMainMDIFrame::OnFileClose()
{
    GetActiveMDIChild()->Close();
    return TRUE;
}

// Issue a close request to the frame. Ends the application.
BOOL CMainMDIFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Adds a new splitter MDI child.
BOOL CMainMDIFrame::OnFileNew()
{
    AddMDIChild(new CSplitterMDIChild);
    return TRUE;
}

// Arrange the MDI child windows in cascade mode.
BOOL CMainMDIFrame::OnMDICascade()
{
    MDICascade();
    return TRUE;
}

// Ask all the MDI child windows to close.
BOOL CMainMDIFrame::OnMDICloseAll()
{
    RemoveAllMDIChildren();
    return TRUE;
}

// Arrange the minimised MDI child windows.
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

// Define the resource IDs and images for toolbar buttons.
void CMainMDIFrame::SetupToolBar()
{
    // Define the resource IDs for the toolbar
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_OPEN, FALSE);   // disabled button
    AddToolBarButton( IDM_FILE_SAVE, FALSE);   // disabled button
    AddToolBarButton( 0 );                     // Separator
    AddToolBarButton( IDM_EDIT_CUT, FALSE);    // disabled button
    AddToolBarButton( IDM_EDIT_COPY, FALSE);   // disabled button
    AddToolBarButton( IDM_EDIT_PASTE, FALSE);  // disabled button
    AddToolBarButton( 0 );                     // Separator
    AddToolBarButton( IDM_FILE_PRINT, FALSE);  // disabled button
    AddToolBarButton( 0 );                     // Separator
    AddToolBarButton( IDM_HELP_ABOUT );
}

// Process the MDI frame's window messages.
LRESULT CMainMDIFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_MOUSEACTIVATE:
        // Called when a MDI child is activated with a mouse click
        {
            // Redraw all MDI children to update docker caption
            std::vector<MDIChildPtr>::const_iterator iter;
            for (iter = GetAllMDIChildren().begin(); iter < GetAllMDIChildren().end(); ++iter)
            {
                (*iter)->RedrawWindow();
            }
        }
        break;
    }

//  pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}

