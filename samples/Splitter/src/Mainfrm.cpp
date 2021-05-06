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

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\Splitter Sample"));

    return CDockFrame::Create(parent);
}

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

BOOL CMainFrame::OnFileExit()
{
    // Issue a close request to the frame
    Close();
    return TRUE;;
}

void CMainFrame::OnInitialUpdate()
{
    // Set the styles for splitter panes
    DWORD style = DS_NO_UNDOCK | DS_NO_CAPTION | DS_CLIENTEDGE;
    SetDockStyle(style);

    CRect viewRect = GetViewRect();

    // Add the bottom pane first. It is a child of the main pane.
    m_pDockTree = static_cast<CDockTree*>(AddDockedChild(new CDockTree, style|DS_DOCKED_BOTTOM, viewRect.Height()/2));

    // Add the bottom right pane. It is a child of the bottom pane
    m_pDockList = static_cast<CDockList*>(m_pDockTree->AddDockedChild(new CDockList, style|DS_DOCKED_RIGHT, viewRect.Width()/2));

    // Add the top right pane. It is a child of the main pane.
    m_pDockText = static_cast<CDockText*>(AddDockedChild(new CDockText, style|DS_DOCKED_RIGHT, viewRect.Width()/2));

}

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

