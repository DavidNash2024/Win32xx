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
    SetView(m_view);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\Frame"));
}


// Destructor for CMainFrame.
CMainFrame::~CMainFrame()
{
}


// OnCommand responds to menu and and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_FILE_OPEN:         OnFileOpen();      return TRUE;
    case IDM_FILE_SAVE:         OnFileSave();      return TRUE;
    case IDM_FILE_SAVEAS:       OnFileSave();      return TRUE;
    case IDM_FILE_PREVIEW:      OnFilePreview();   return TRUE;
    case IDM_FILE_PRINT:        OnFilePrint();     return TRUE;
    case IDM_FILE_EXIT:         OnFileExit();      return TRUE;
    case IDW_VIEW_STATUSBAR:    OnViewStatusBar(); return TRUE;
    case IDW_VIEW_TOOLBAR:      OnViewToolBar();   return TRUE;
    case IDM_HELP_ABOUT:        OnHelp();          return TRUE;
    }

    return FALSE;
}


// OnCreate controls the way the frame is created.
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


// Called after the window is created.
void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    // Show the menu and toolbar
    ShowMenu(TRUE);
    ShowToolBar(TRUE);

    TRACE("Frame created\n");
}


// Create the File Open dialog to choose the file to load.
void CMainFrame::OnFileOpen()
{
    CFileDialog fileDlg(TRUE);

    // Bring up the file open dialog retrieve the selected filename
    if (fileDlg.DoModal(*this) == IDOK)
    {
        GetDoc().FileLoad(fileDlg.GetPathName());
    }
}


// Create the File Save dialog to choose the file to save.
void CMainFrame::OnFileSave()
{
    CFileDialog fileDlg(FALSE);
    
    // Bring up the file save dialog retrieve the selected filename
    if (fileDlg.DoModal(*this) == IDOK)
    {
        GetDoc().FileStore(fileDlg.GetPathName());
    }
}


// Previews a print job before sending it to the printer.
void CMainFrame::OnFilePreview()
{
    try
    {
        // Get the device contect of the default or currently chosen printer
        CPrintDialog printDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
        CDC printerDC = printDlg.GetPrinterDC();

        // Create the preview window if required
        if (!m_preview.IsWindow())
            m_preview.Create(*this);

        // Specify the source of the PrintPage function
        m_preview.SetSource(m_view);

        // Set the preview's owner (for messages)
        m_preview.DoPrintPreview(*this);

        // Swap views
        SetView(m_preview);

        // Hide the menu and toolbar
        ShowMenu(FALSE);
        ShowToolBar(FALSE);

        // Update status
        CString status = _T("Printer: ") + printDlg.GetDeviceName();
        SetStatusText(status);
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(_T("Print Preview Failed"), e.GetErrorString(), MB_ICONWARNING);
        SetView(m_view);
        ShowMenu(TRUE);
        ShowToolBar(TRUE);      
    }

}


// Bring up a dialog to choose the printer.
void CMainFrame::OnFilePrint()
{
    CPrintDialog printdlg;

    try
    {
        if (IDOK == printdlg.DoModal(*this))
        {
            m_view.QuickPrint(_T("Frame Sample"));
        }

    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);
    }
}


// Process notification messages sent by child windows
LRESULT CMainFrame::OnNotify(WPARAM wparam, LPARAM lparam)
{
//  switch(((LPNMHDR)lparam)->code)
//  {
//      Add case statements for each notification message here
//  }

    // Some notifications should return a value when handled
    return CFrame::OnNotify(wparam, lparam);
}


// Called when the Print Preview's "Close" button is pressed.
void CMainFrame::OnPreviewClose()
{
    // Swap the view
    SetView(m_view);

    // Show the menu and toolbar
    ShowMenu(TRUE);
    ShowToolBar(TRUE);

    SetStatusText(LoadString(IDW_READY));
}


// Called when the Print Preview's "Print Now" button is pressed.
void CMainFrame::OnPreviewPrint()
{
    m_view.QuickPrint(_T("Frame Sample"));
}


// Called when the Print Preview's "Print Setup" button is pressed.
void CMainFrame::OnPreviewSetup()
{
    // Call the print setup dialog.
    CPrintDialog printDlg(PD_PRINTSETUP);
    try
    {
        // Display the print dialog
        if (printDlg.DoModal(*this) == IDOK)
        {
            CString status = _T("Printer: ") + printDlg.GetDeviceName();
            SetStatusText(status);
        }
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);
    }

    // Initiate the print preview.
    m_preview.DoPrintPreview(*this);
}


// Called before window creation to update the window's CREATESTRUCT
void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
    // This function is called before the frame is created.
    // It provides an opportunity to modify the various CREATESTRUCT
    // parameters used in the frame window's creation.

    // The WS_EX_LAYOUTRTL style requires Windows 2000 or above in targetver.h
    // cs.dwExStyle = WS_EX_LAYOUTRTL;      // Set Right-To-Left Window Layout

    // Call base clase to set defaults
    CFrame::PreCreate(cs);

    // cs.style &= ~WS_VISIBLE; // Remove the WS_VISIBLE style. The frame will be initially hidden.
}


// Configure the menu icons.
void CMainFrame::SetupMenuIcons()
{
    // Set the bitmap used for menu icons
    AddMenuIcons(GetToolBarData(), RGB(192, 192, 192), IDB_MENUICONS, 0);
}


// Set the Resource IDs for the toolbar buttons
void CMainFrame::SetupToolBar()
{
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


// Handle the frame's messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case UWM_PREVIEWCLOSE:      OnPreviewClose();   break;
    case UWM_PRINTNOW:          OnPreviewPrint();   break;
    case UWM_PRINTSETUP:        OnPreviewSetup();   break;
    }

    return WndProcDefault(msg, wparam, lparam);
}

