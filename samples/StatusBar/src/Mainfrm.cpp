/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"

#define STATUS_ID 1211

///////////////////////////////////
// CMainFrame function definitions.
//

// Constructor for CMainFrame. Its called after CFrame's constructor.
CMainFrame::CMainFrame()
{
}

// Destructor for CMainFrame.
CMainFrame::~CMainFrame()
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    //Set m_View as the view window of the frame
    SetView(m_view);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\StatusBar Sample"));

    return CFrame::Create(parent);
}


// OnCommand responds to menu and and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_FILE_EXIT:      return OnFileExit();
    case IDW_VIEW_STATUSBAR: return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:   return OnViewToolBar();
    case IDM_HELP_ABOUT:     return OnHelp();
    }

    return FALSE;
}

// OnCreate controls the way the frame is created.
int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
    // Use the following functions to disable options.

       UseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar
       UseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar
    // UseReBar(FALSE);              // Don't use a ReBar
    // UseStatusBar(FALSE);          // Don't use a StatusBar
    // UseThemes(FALSE);             // Don't use themes
    // UseToolBar(FALSE);            // Don't use a ToolBar

    // call the base class function
    CFrame::OnCreate(cs);

    GetStatusBar().SetWindowLongPtr(GWLP_ID, STATUS_ID);

    return 0;
}

// Perform the owner draw for Part 3 in the status bar
LRESULT CMainFrame::OnDrawItem(UINT msg, WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(msg);

    LPDRAWITEMSTRUCT pDrawItem = (LPDRAWITEMSTRUCT)lparam;

    if (pDrawItem->CtlID == STATUS_ID) // Message comes from the status bar
    {
        CRect partRect = pDrawItem->rcItem;
        CDC dc;
        dc.Attach(pDrawItem->hDC);

        // Set the font to italic
        CFont Font = GetStatusBar().GetFont();
        LOGFONT lf = Font.GetLogFont();
        lf.lfItalic = TRUE;
        dc.CreateFontIndirect(lf);

        // Display the gradient background and text
        dc.GradientFill(RGB(230, 180, 0), RGB(240, 210, 90), partRect, TRUE);
        dc.SetTextColor(RGB(10,20,250));
        dc.SetBkMode(TRANSPARENT);
        dc.TextOut(partRect.left, partRect.top,_T("Owner Draw"), 10);

        return TRUE;
    }

    // Allow the frame to perform owner drawing menu items.
    return CFrame::OnDrawItem(msg, wparam, lparam);
}

// Issue a close request to the frame.
BOOL CMainFrame::OnFileExit()
{
    Close();

    return TRUE;
}

// Called after the frame is created.
void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    TRACE("Frame created\n");
}

// Configure the toolbar.
void CMainFrame::SetupToolBar()
{
    // Set the Resource IDs for the toolbar buttons
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_OPEN, FALSE);    // disabled button
    AddToolBarButton( IDM_FILE_SAVE, FALSE);    // disabled button

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_EDIT_CUT,   FALSE );  // disabled button
    AddToolBarButton( IDM_EDIT_COPY,  FALSE );  // disabled button
    AddToolBarButton( IDM_EDIT_PASTE, FALSE );  // disabled button

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_FILE_PRINT, FALSE);   // disabled button

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_HELP_ABOUT );
}

