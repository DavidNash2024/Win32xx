/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"

//////////////////////////////////
// CMainFrame function definitions
//

// Constructor.
CMainFrame::CMainFrame() : m_isToolbarShown(true)
{
}

// Destructor.
CMainFrame::~CMainFrame()
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    //Set m_View as the view window of the frame
    SetView(m_view);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\Frame"));

    return CFrame::Create(parent);
}

// Called when the frame window is asked to close.
void CMainFrame::OnClose()
{
    // Close the preview.
    if (GetView() == m_preview)
        OnPreviewClose();

    // Proceed with closing the frame.
    CFrame::OnClose();
}

// OnCommand responds to menu and and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_FILE_OPEN:       return OnFileOpen();
    case IDM_FILE_SAVE:       return OnFileSave();
    case IDM_FILE_SAVEAS:     return OnFileSave();
    case IDM_FILE_PREVIEW:    return OnFilePreview();
    case IDM_FILE_PRINT:      return OnFilePrint();
    case IDM_FILE_EXIT:       return OnFileExit();
    case IDW_VIEW_STATUSBAR:  return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:    return OnViewToolBar();
    case IDM_HELP_ABOUT:      return OnHelp();
    }

    return FALSE;
}

// OnCreate controls the way the frame is created.
// Overriding CFrame::OnCreate is optional.
int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
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

// Issue a close request to the frame to end the program.
BOOL CMainFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Called after the window is created.
void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    TRACE("Frame created\n");
}

// Create the File Open dialog to choose the file to load.
BOOL CMainFrame::OnFileOpen()
{
    CString filter = _T("Program Files (*.cpp; *.h)|*.cpp; *.h|All Files (*.*)|*.*||");
    CFileDialog fileDlg(TRUE);    // TRUE for file open
    fileDlg.SetFilter(filter);
    fileDlg.SetDefExt(_T(".cpp"));

    // Bring up the file open dialog retrieve the selected filename
    if (fileDlg.DoModal(*this) == IDOK)
    {
        GetDoc().FileLoad(fileDlg.GetPathName());
    }

    return TRUE;
}

// Create the File Save dialog to choose the file to save.
BOOL CMainFrame::OnFileSave()
{
    CString filter = _T("Program Files (*.cpp; *.h)|*.cpp; *.h|All Files (*.*)|*.*||");
    CFileDialog fileDlg(FALSE);    // FALSE for file save
    fileDlg.SetFilter(filter);
    fileDlg.SetDefExt(_T(".cpp"));

    // Bring up the file save dialog retrieve the selected filename
    if (fileDlg.DoModal(*this) == IDOK)
    {
        GetDoc().FileStore(fileDlg.GetPathName());
    }

    return TRUE;
}

// Previews a print job before sending it to the printer.
BOOL CMainFrame::OnFilePreview()
{
    try
    {
        m_isToolbarShown = GetToolBar().IsWindow() && GetToolBar().IsWindowVisible();

        // Get the device contect of the default or currently chosen printer
        CPrintDialog printDlg;
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
        MessageBox(e.GetText(), _T("Print Preview Failed"), MB_ICONWARNING);
        SetView(m_view);
        ShowMenu(GetFrameMenu() != 0);
        ShowToolBar(m_isToolbarShown);
    }

    return TRUE;
}

// Bring up a dialog to choose the printer.
BOOL CMainFrame::OnFilePrint()
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
        MessageBox(e.GetText(), _T("Print Failed"), MB_ICONWARNING);
    }

    return TRUE;
}

// Process notification messages (WM_NOTIFY) sent by child windows
LRESULT CMainFrame::OnNotify(WPARAM wparam, LPARAM lparam)
{
//  LPNMHDR pHeader = reinterpret_cast<LPNMHDR>(lparam);
//  switch (pHeader->code)
//  {
//      Add case statements for each notification message here
//  }

    // Some notifications should return a value when handled
    return CFrame::OnNotify(wparam, lparam);
}

// Called when the Print Preview's "Close" button is pressed.
LRESULT CMainFrame::OnPreviewClose()
{
    // Swap the view
    SetView(m_view);

    // Show the menu and toolbar
    ShowMenu(GetFrameMenu() != 0);
    ShowToolBar(m_isToolbarShown);

    SetStatusText(LoadString(IDW_READY));

    return 0;
}

// Called when the Print Preview's "Print Now" button is pressed.
LRESULT CMainFrame::OnPreviewPrint()
{
    m_view.QuickPrint(_T("Frame Sample"));
    return 0;
}

// Called when the Print Preview's "Print Setup" button is pressed.
LRESULT CMainFrame::OnPreviewSetup()
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

    return 0;
}

// This function is called before the frame is created.
// It provides an opportunity to modify the various CREATESTRUCT
// parameters used before the frame window is created.
void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
    // The WS_EX_LAYOUTRTL style requires Windows 2000 or above in targetver.h
    // cs.dwExStyle = WS_EX_LAYOUTRTL;      // Set Right-To-Left Window Layout
    // cs.style &= ~WS_VISIBLE; // Remove the WS_VISIBLE style. The frame will be initially hidden.

    // Call base clase to set defaults
    CFrame::PreCreate(cs);
}

// Specifies the images for some of the menu items.
void CMainFrame::SetupMenuIcons()
{
    // Set the bitmap used for menu icons
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        SetMenuIcons(data, RGB(192, 192, 192), IDW_MAIN);
    else
        SetMenuIcons(data, RGB(192, 192, 192), IDB_MENUICONS);
}

// Set the resource IDs and images for the toolbar buttons.
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

// Process the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case UWM_PREVIEWCLOSE:    return OnPreviewClose();
        case UWM_PREVIEWPRINT:    return OnPreviewPrint();
        case UWM_PREVIEWSETUP:    return OnPreviewSetup();
        }

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

