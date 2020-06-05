////////////////////////////////////////////////////
// MainMDIfrm.cpp  - definitions for the CMainMDIFrame class

#include "stdafx.h"
#include "mainMDIfrm.h"
#include "SplitterMDIChild.h"
#include "resource.h"


CMainMDIFrame::CMainMDIFrame()
{
    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\MDIFrameSplitter"));
}

CMainMDIFrame::~CMainMDIFrame()
{
}

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
    default:    // Pass to active child...
        {
            if (GetActiveMDIChild())
                GetActiveMDIChild()->SendMessage(WM_COMMAND, wparam, lparam);
        }
        break ;
    }
    return FALSE;
}

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

BOOL CMainMDIFrame::OnFilePrint()
{
    // Bring up a dialog to choose the printer
    CPrintDialog printdlg;

    try
    {
        INT_PTR result = printdlg.DoModal(*this);

        // Retrieve the printer DC
        // CDC printerDC = printdlg.GetPrinterDC();

        // TODO:
        // Add your own code here. Refer to the tutorial for additional information

        return (result == IDOK);   // boolean expression
    }

    catch (const CWinException& /* e */)
    {
        // No default printer
        MessageBox(_T("Unable to display print dialog"), _T("Print Failed"), MB_OK);
        return FALSE;
    }
}

void CMainMDIFrame::OnInitialUpdate()
{
    TRACE("MDI Frame started \n");
    //The frame is now created.
    //Place any additional startup code here.
}

BOOL CMainMDIFrame::OnFileClose()
{
    // Close the active MDI window
    GetActiveMDIChild()->SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
    return TRUE;
}

BOOL CMainMDIFrame::OnFileExit()
{
    // Issue a close request to the frame
    PostMessage(WM_CLOSE);
    return TRUE;
}

BOOL CMainMDIFrame::OnFileNew()
{
    AddMDIChild(new CSplitterMDIChild);
    return TRUE;
}

BOOL CMainMDIFrame::OnMDICascade() 
{ 
    MDICascade();
    return TRUE;
}

BOOL CMainMDIFrame::OnMDICloseAll()
{
    RemoveAllMDIChildren();
    return TRUE;
}

BOOL CMainMDIFrame::OnMDIIconArrange()
{
    MDIIconArrange();
    return TRUE;
}

BOOL CMainMDIFrame::OnMDITile()
{ 
    MDITile(); 
    return TRUE; 
}

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
}

LRESULT CMainMDIFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_MOUSEACTIVATE:
        // Called when a MDI child is activated with a mouse click
        {
            // Redraw all MDI children to update docker caption
            std::vector<MDIChildPtr>::const_iterator iter;
            for (iter = GetAllMDIChildren().begin(); iter < GetAllMDIChildren().end(); ++iter)
            {
                (*iter)->RedrawWindow();
            }
        }
        break;
    }

//  pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}

