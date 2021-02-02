/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"

///////////////////////////////////
// CMainFrame function definitions.
//
CMainFrame::CMainFrame()
{
    // Constructor for CMainFrame. Its called after CFrame's constructor

    //Set m_View as the view window of the frame
    SetView(m_view);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\Frame"));
}

CMainFrame::~CMainFrame()
{
    // Destructor for CMainFrame.
}

BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    // OnCommand responds to menu and and toolbar input

    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_EDIT_FUNCTION:     return OnEditFunction();
    case IDM_FILE_EXIT:         return OnFileExit();
    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDM_HELP_ABOUT:        return OnHelp();
    }

    return FALSE;
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
    return CFrame::OnCreate(cs);
}

BOOL CMainFrame::OnEditFunction()
{
    if (m_view.GetInput().DoModal(*this) == IDOK)
    {
        CString str = m_view.GetInput().GetFunction();
        m_view.GetCalc().Input(str);
        m_view.RedrawWindow();

        if (m_view.GetCalc().Get_Status() == Calc::st_ERROR)
        {
            MessageBox(_T("Invalid Function Input"), _T("Error"), MB_ICONEXCLAMATION);
        }
        else if (m_view.GetCalc().Get_Status() == Calc::st_OVERFLOW)
        {
            MessageBox(_T("Overflow"), _T("Error"), MB_ICONEXCLAMATION);
        }
    }

    return TRUE;
}

BOOL CMainFrame::OnFileExit()
{
    // Issue a close request to the frame
    Close();
    return TRUE;
}

void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    OnEditFunction();
    TRACE("Frame created\n");
}

void CMainFrame::SetupToolBar()
{
    // Set the Resource IDs for the toolbar buttons
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

LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
//  switch (msg)
//  {
//      Add case statements for each messages to be handled here
//  }

    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}

