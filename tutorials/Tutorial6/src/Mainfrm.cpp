////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class


#include "Mainfrm.h"
#include "resource.h"


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
    // Set m_View as the view window of the frame
    SetView(m_view);

    return CFrame::Create(parent);
}

// Process the messages from the Menu and Toolbar.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    switch (LOWORD(wparam))
    {
    case IDM_FILE_NEW:          OnFileNew();        return TRUE;
    case IDM_FILE_OPEN:         OnFileOpen();       return TRUE;
    case IDM_FILE_SAVE:         OnFileSave();       return TRUE;
    case IDM_FILE_SAVEAS:       OnFileSaveAs();     return TRUE;
    case IDM_FILE_PRINT:        OnFilePrint();      return TRUE;

    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDM_HELP_ABOUT:        return OnHelp();
    case IDM_FILE_EXIT:         OnFileExit();       return TRUE;
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

    // UseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar
    // UseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar
    // UseReBar(FALSE);              // Don't use a ReBar
    // UseStatusBar(FALSE);          // Don't use a StatusBar
    UseThemes(FALSE);             // Don't use themes
    UseToolBar(FALSE);            // Don't use a ToolBar

    // call the base class function
    return CFrame::OnCreate(cs);
}

// Issue a close request to the frame.
void CMainFrame::OnFileExit()
{
    Close();
}

void CMainFrame::OnFileNew()
{
    ::MessageBox(0, _T("File New  ... Implemented later"), _T("Menu"), MB_OK);
}

void CMainFrame::OnFileOpen()
{
    ::MessageBox(0, _T("File Open  ... Implemented later"), _T("Menu"), MB_OK);
}

void CMainFrame::OnFilePrint()
{
    ::MessageBox(0, _T("File Print  ... Implemented later"), _T("Menu"), MB_OK);
}

void CMainFrame::OnFileSave()
{
    ::MessageBox(0, _T("File Save  ... Implemented later"), _T("Menu"), MB_OK);
}

void CMainFrame::OnFileSaveAs()
{
    ::MessageBox(0, _T("File SaveAs  ... Implemented later"), _T("Menu"), MB_OK);
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
