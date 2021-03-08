////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class

#include "targetver.h"
#include "Mainfrm.h"
#include "resource.h"


// Constructor.
CMainFrame::CMainFrame()
{
    // Set m_View as the view window of the frame
    SetView(m_view);
}

// Destructor.
CMainFrame::~CMainFrame()
{
}

// Process the messages from the Menu and Toolbar
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
    case IDM_FILE_EXIT:         OnFileExit();       return TRUE;
    case IDM_PEN_COLOR:         OnPenColor();       return TRUE;
    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDM_HELP_ABOUT:        return OnHelp();
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
    // UseThemes(FALSE);             // Don't use themes
    // UseToolBar(FALSE);            // Don't use a ToolBar

    // call the base class function
    return CFrame::OnCreate(cs);
}


// Issue a close request to the frame.
void CMainFrame::OnFileExit()
{
    PostMessage(WM_CLOSE);
}

void CMainFrame::OnFileNew()
{
    ::MessageBox(NULL, _T("File New  ... Implemented later"), _T("Menu"), MB_OK);
}

void CMainFrame::OnFileOpen()
{
    ::MessageBox(NULL, _T("File Open  ... Implemented later"), _T("Menu"), MB_OK);
}

void CMainFrame::OnFilePrint()
{
    ::MessageBox(NULL, _T("File Print  ... Implemented later"), _T("Menu"), MB_OK);
}

void CMainFrame::OnFileSave()
{
    ::MessageBox(NULL, _T("File Save  ... Implemented later"), _T("Menu"), MB_OK);
}

void CMainFrame::OnFileSaveAs()
{
    ::MessageBox(NULL, _T("File SaveAs  ... Implemented later"), _T("Menu"), MB_OK);
}

// Initiates the Choose Color dialog.
void CMainFrame::OnPenColor()
{
    // array of custom colors, initialized to white
    static COLORREF custColors[16] = {  RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255),
                                        RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255),
                                        RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255),
                                        RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255) };

    CColorDialog colorDlg;
    colorDlg.SetCustomColors(custColors);

    // Initialize the Choose Color dialog
    if (colorDlg.DoModal(*this) == IDOK)
    {
        // Store the custom colors in the static array
        memcpy(custColors, colorDlg.GetCustomColors(), 16*sizeof(COLORREF));

        // Retrieve the chosen color
        m_view.SetPenColor(colorDlg.GetColor());
    }
}

// Configures the Toolbar.
void CMainFrame::SetupToolBar()
{
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_OPEN  );
    AddToolBarButton( IDM_FILE_SAVE  );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_EDIT_CUT,   FALSE );
    AddToolBarButton( IDM_EDIT_COPY,  FALSE );
    AddToolBarButton( IDM_EDIT_PASTE, FALSE );
    AddToolBarButton( IDM_FILE_PRINT );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_PEN_COLOR );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_HELP_ABOUT );

    // Note: By default a single bitmap with a resource ID of IDW_MAIN and
    //       a color mask of RGB(192,192,192) is used for the ToolBar.
    //       The color mask is a color used for transparency.
}

// Handle the frame's messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
//  switch (msg)
//  {

//  } // switch (msg)

    // Use the default message handling for remaining messages.
    return WndProcDefault(msg, wparam, lparam);
}

