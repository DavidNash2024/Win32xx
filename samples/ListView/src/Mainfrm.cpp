/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"

//////////////////////////////////
// CMainFrame function definitions
//

// Constructor.
CMainFrame::CMainFrame()
{
}

// Destructor.
CMainFrame::~CMainFrame()
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    //Set m_View as the view window of the frame
    SetView(m_listView);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\ListView"));

    return CFrame::Create(parent);
}

// OnCommand responds to menu and and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_FILE_EXIT:         OnFileExit();      return TRUE;
    case IDW_VIEW_STATUSBAR:    OnViewStatusBar(); return TRUE;
    case IDW_VIEW_TOOLBAR:      OnViewToolBar();   return TRUE;
    case IDM_HELP_ABOUT:        OnHelp();          return TRUE;
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
    return CFrame::OnCreate(cs);
}

// Issue a close request to the frame to end the program.
void CMainFrame::OnFileExit()
{
    Close();
}

// Called after the window is created.
void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    TRACE("Frame created\n");
}

// Process notification messages (WM_NOTIFY) sent by child windows
LRESULT CMainFrame::OnNotify(WPARAM wparam, LPARAM lparam)
{
//    LONGLONG code = ((LPNMHDR)lparam)->code;   // LONGLONG required by the TDM-GCC 10.3 compiler.
//  switch(code)
//  {
//      Add case statements for each notification message here
//  }

    // Some notifications should return a value when handled
    return CFrame::OnNotify(wparam, lparam);
}

// This function is called before the frame is created.
// It provides an opportunity to modify the various CREATESTRUCT
// parameters used before the frame window is created.
void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
    // The WS_EX_LAYOUTRTL style requires Windows 2000 or above in targetver.h
    // cs.dwExStyle = WS_EX_LAYOUTRTL;      // Set Right-To-Left Window Layout
    // cs.style &= ~WS_VISIBLE; // Remove the WS_VISIBLE style. The frame will be initially hidden.

    // Call base clase to set defaults
    CFrame::PreCreate(cs);
}

// Configure the menu icons.
void CMainFrame::SetupMenuIcons()
{
    // Set the bitmap used for menu icons
    AddMenuIcons(GetToolBarData(), RGB(192, 192, 192), IDB_MENUICONS, 0);
}

// Set the resource IDs and images for the toolbar buttons.
void CMainFrame::SetupToolBar()
{
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_OPEN  );
    AddToolBarButton( IDM_FILE_SAVE  );

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_EDIT_CUT,   FALSE );  // disabled button
    AddToolBarButton( IDM_EDIT_COPY,  FALSE );  // disabled button
    AddToolBarButton( IDM_EDIT_PASTE, FALSE );  // disabled button

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_FILE_PRINT );

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_HELP_ABOUT );
}

// Process the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
    //    switch (msg)
    //    {
    //    }

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

