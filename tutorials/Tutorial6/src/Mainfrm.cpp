////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class


#include "Mainfrm.h"
#include "resource.h"


// Constructor.
CMainFrame::CMainFrame()
{
    // Set m_view as the view window of the frame.
    SetView(m_view);
}

// Destructor.
CMainFrame::~CMainFrame()
{
}

// Process the messages from the Menu and Toolbar.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    switch (LOWORD(wparam))
    {
    case IDM_FILE_NEW:        return OnFileNew();
    case IDM_FILE_OPEN:       return OnFileOpen();
    case IDM_FILE_SAVE:       return OnFileSave();
    case IDM_FILE_SAVEAS:     return OnFileSaveAs();
    case IDM_FILE_PRINT:      return OnFilePrint();

    case IDW_VIEW_STATUSBAR:  return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:    return OnViewToolBar();
    case IDM_HELP_ABOUT:      return OnHelp();
    case IDM_FILE_EXIT:       return OnFileExit();
    }

    return FALSE;
}

// Called during window creation.
int CMainFrame::OnCreate(CREATESTRUCT& cs)
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
    UseThemes(FALSE);                // Don't use themes.
    UseToolBar(FALSE);               // Don't use a ToolBar.

    // call the base class function
    return CFrame::OnCreate(cs);
}

// Issue a close request to the frame.
BOOL CMainFrame::OnFileExit()
{
    Close();
    return TRUE;
}

BOOL CMainFrame::OnFileNew()
{
    ::MessageBox(NULL, _T("File New  ... Implemented later"), _T("Menu"), MB_OK);
    return TRUE;
}

BOOL CMainFrame::OnFileOpen()
{
    ::MessageBox(NULL, _T("File Open  ... Implemented later"), _T("Menu"), MB_OK);
    return TRUE;
}

BOOL CMainFrame::OnFilePrint()
{
    ::MessageBox(NULL, _T("File Print  ... Implemented later"), _T("Menu"), MB_OK);
    return TRUE;
}

BOOL CMainFrame::OnFileSave()
{
    ::MessageBox(NULL, _T("File Save  ... Implemented later"), _T("Menu"), MB_OK);
    return TRUE;
}

BOOL CMainFrame::OnFileSaveAs()
{
    ::MessageBox(NULL, _T("File SaveAs  ... Implemented later"), _T("Menu"), MB_OK);
    return TRUE;
}


// Handle the frame's mmessages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{

//  switch (msg)
//  {
//  }

    // Use the default message handling for remaining messages.
    return WndProcDefault(msg, wparam, lparam);
}
