////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
    // Constructor for CMainFrame. Its called after CFrame's constructor

    //Set m_View as the view window of the frame
    SetView(m_dxView);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\DirectX"));
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
    case IDM_FILE_OPEN:      return OnFileOpen();
    case IDM_FILE_SAVE:      return OnFileSave();
    case IDM_FILE_SAVEAS:    return OnFileSave();
    case IDM_FILE_PRINT:     return OnFilePrint();
    case IDM_FILE_EXIT:      return OnFileExit();
    case IDW_VIEW_STATUSBAR: return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:   return OnViewToolBar();
    case IDM_HELP_ABOUT:     return OnHelp();
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

void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    TRACE("Frame created\n");
}

BOOL CMainFrame::OnFileExit()
{
    // Issue a close request to the frame
    Close();
    return TRUE;
}

BOOL CMainFrame::OnFileOpen()
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

BOOL CMainFrame::OnFileSave()
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


BOOL CMainFrame::OnFilePrint()
{
    // Bring up a dialog to choose the printer
    PRINTDLG pd;
    ZeroMemory(&pd, sizeof(PRINTDLG));
    pd.lStructSize = sizeof( pd );
    pd.Flags = PD_RETURNDC;
    pd.hwndOwner = *this;

    // Retrieve the printer DC
    PrintDlg( &pd );

    // TODO:
    // Add your own code here. Refer to the tutorial for additional information

    return TRUE;
}

LRESULT CMainFrame::OnNotify(WPARAM wparam, LPARAM lparam)
{
    // Process notification messages sent by child windows
//  switch(((LPNMHDR)lparam)->code)
//  {
        //Add case statments for each notification message here
//  }

    // Some notifications should return a value when handled
    return CFrame::OnNotify(wparam, lparam);
}

void CMainFrame::SetupToolBar()
{
    // Set the Resource IDs for the toolbar buttons
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_OPEN  );
    AddToolBarButton( IDM_FILE_SAVE  );

    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_EDIT_CUT,   FALSE );  // disabled button
    AddToolBarButton( IDM_EDIT_COPY,  FALSE );  // disabled button
    AddToolBarButton( IDM_EDIT_PASTE, FALSE );  // disabled button

    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_FILE_PRINT );

    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_HELP_ABOUT );
}

LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
//  switch (msg)
//  {
//
//  }

    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}

