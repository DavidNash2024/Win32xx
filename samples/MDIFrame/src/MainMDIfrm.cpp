/////////////////////////////
// MainMDIfrm.cpp
//

#include "stdafx.h"
#include "MainMDIfrm.h"
#include "SimpleMDIChild.h"
#include "resource.h"

/////////////////////////////////////
// CMainMDIFrame function definitions
//

// Constructor.
CMainMDIFrame::CMainMDIFrame()
{
}

// Destructor.
CMainMDIFrame::~CMainMDIFrame()
{
}

// Create the MDI frame window.
HWND CMainMDIFrame::Create(HWND parent)
{
    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\MDI Frame"));

    return CMDIFrame::Create(parent);
}

// Respond to input from the menu and toolbar.
BOOL CMainMDIFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UINT id = LOWORD(wparam);
    switch (id)
    {
    case IDM_FILE_NEW:          return OnFileNew();
    case IDM_FILE_CLOSE:        return OnFileClose();
    case IDM_FILE_OPEN:         return OnFileOpen();
    case IDM_FILE_SAVE:         return OnFileSave();
    case IDM_FILE_SAVEAS:       return OnFileSave();
    case IDM_FILE_PRINT:        return OnFilePrint();
    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDW_MDI_ARRANGE:       return OnMDIIconArrange();
    case IDW_MDI_CASCADE:       return OnMDICascade();
    case IDW_MDI_CLOSEALL:      return OnMDICloseAll();
    case IDW_MDI_TILE:          return OnMDITile();
    case IDM_FILE_EXIT:         return OnFileExit();
    case IDM_HELP_ABOUT:        return OnHelp();
    default:
        {
            // Pass to active child...
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

// Close the active MDI child.
BOOL CMainMDIFrame::OnFileClose()
{
    GetActiveMDIChild()->Close();
    return TRUE;
}

// Issue a close request to the frame to end the application.
BOOL CMainMDIFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Create a new MDI child.
BOOL CMainMDIFrame::OnFileNew()
{
    AddMDIChild(new CSimpleMDIChild);
    return TRUE;
}

// Sample code to load data from a file.
BOOL CMainMDIFrame::OnFileOpen()
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

// Sample code to save data to a file.
BOOL CMainMDIFrame::OnFileSave()
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

// Sample code to send output to a printer.
BOOL CMainMDIFrame::OnFilePrint()
{
    // Bring up a dialog to choose the printer
    CPrintDialog printdlg;

    try
    {
        INT_PTR Res = printdlg.DoModal(*this);

        // Retrieve the printer DC
        // CDC dcPrinter = Printdlg.GetPrinterDC();

        // TODO:
        // Add your own code here. Refer to the tutorial for additional information

        return (Res == IDOK);   // boolean expression
    }

    catch (const CWinException& /* e */)
    {
        // No default printer
        MessageBox(_T("Unable to display print dialog"), _T("Print Failed"), MB_OK);
        return FALSE;
    }
}

// Called after the MDI frame window is created.
// Called after OnCreate.
void CMainMDIFrame::OnInitialUpdate()
{
    TRACE("MDI Frame started \n");
    //The frame is now created.
    //Place any additional startup code here.
}

// Arrange the MDI children in cascade mode.
BOOL CMainMDIFrame::OnMDICascade()
{
    MDICascade();
    return TRUE;
}

// Close all MDI children.
BOOL CMainMDIFrame::OnMDICloseAll()
{
    RemoveAllMDIChildren();
    return TRUE;
}

// Arrange minimised MDI children.
BOOL CMainMDIFrame::OnMDIIconArrange()
{
    MDIIconArrange();
    return TRUE;
}

// Arrange MDI children in tile mode.
BOOL CMainMDIFrame::OnMDITile()
{
    MDITile();
    return TRUE;
}

// Configures the images for menu items.
void CMainMDIFrame::SetupMenuIcons()
{
    // Set the bitmap used for menu icons
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        SetMenuIcons(data, RGB(192, 192, 192), IDW_MAIN);
    else
        SetMenuIcons(data, RGB(192, 192, 192), IDB_TOOLBAR16);
}

// Assign resource IDs and images to toolbar buttons.
void CMainMDIFrame::SetupToolBar()
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

    SetToolBarImages(RGB(192, 192, 192), IDW_MAIN, IDB_TOOLBAR24_DIS, IDB_TOOLBAR24_HOT);
}

// Process the MDi frame's window messages.
LRESULT CMainMDIFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
    //  switch (msg)
    //  {
    //  Add case statements for each messages to be handled here.
    //  }

        //  Pass unhandled messages on for default processing.
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

