////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"


CMainFrame::CMainFrame()
{
    SetView(m_MainView);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\Splitter Sample"));
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    UINT nID = LOWORD(wParam);
    switch(nID)
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
    PostMessage(WM_CLOSE);

    return TRUE;;
}

void CMainFrame::OnInitialUpdate()
{
    // Set the styles for splitter panes
    DWORD dwStyle = DS_NO_UNDOCK | DS_NO_CAPTION | DS_CLIENTEDGE;
    SetDockStyle(dwStyle);

    CRect rcView = GetViewRect();

    // Add the bottom pane first. It is a child of the main pane.
    m_pDockTree = static_cast<CDockTree*>(AddDockedChild(new CDockTree, dwStyle|DS_DOCKED_BOTTOM, rcView.Height()/2));

    // Add the bottom right pane. It is a child of the bottom pane 
    m_pDockList = static_cast<CDockList*>(m_pDockTree->AddDockedChild(new CDockList, dwStyle|DS_DOCKED_RIGHT, rcView.Width()/2));

    // Add the top right pane. It is a child of the main pane.
    m_pDockText = static_cast<CDockText*>(AddDockedChild(new CDockText, dwStyle|DS_DOCKED_RIGHT, rcView.Width()/2));

}

BOOL CMainFrame::OnViewList()
{
    DWORD dwStyle = DS_NO_UNDOCK | DS_NO_CAPTION | DS_CLIENTEDGE;
    if (m_pDockList->IsDocked())
    {
        m_pDockList->Hide();
        GetFrameMenu().CheckMenuItem(IDM_VIEW_LIST, MF_UNCHECKED);
    }
    else
    {
        m_pDockTree->Dock(m_pDockList, dwStyle | DS_DOCKED_RIGHT);
        GetFrameMenu().CheckMenuItem(IDM_VIEW_LIST, MF_CHECKED);
    }

    return TRUE;
}

BOOL CMainFrame::OnViewText()
{
    DWORD dwStyle = DS_NO_UNDOCK | DS_NO_CAPTION | DS_CLIENTEDGE;

    if (m_pDockText->IsDocked())
    {
        m_pDockText->Hide();
        GetFrameMenu().CheckMenuItem(IDM_VIEW_TEXT, MF_UNCHECKED);
    }
    else
    {
        Dock(m_pDockText, dwStyle | DS_DOCKED_RIGHT);
        GetFrameMenu().CheckMenuItem(IDM_VIEW_TEXT, MF_CHECKED);
    }

    return TRUE;
}

void CMainFrame::SetupToolBar()
{   
    // Define the resource IDs for the toolbar
    AddToolBarButton( IDM_FILE_NEW   );
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
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//  switch (uMsg)
//  {
//      Add case statements for each messages to be handled here
//  }

    // pass unhandled messages on for default processing
    return WndProcDefault(uMsg, wParam, lParam);
}

