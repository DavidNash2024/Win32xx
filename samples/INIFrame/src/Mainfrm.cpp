////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"


// Definitions for the CMainFrame class

// Constructor
CMainFrame::CMainFrame()
{
    // Constructor for CMainFrame. Its called after CFrame's constructor

    // Set m_View as the view window of the frame
    SetView(m_view);

    // Normally we would use LoadRegistrySettings here, but this
    // is omitted in this sample. We use an ini file instead.

    // Load initial values from the ini file.
    SerializeINI(FALSE);
}


// Destructor for CMainFrame.
CMainFrame::~CMainFrame()
{
}


// OnCommand responds to menu and and toolbar input
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_FILE_OPEN:         OnFileOpen();       return TRUE;
    case IDM_FILE_SAVE:         OnFileSave();       return TRUE;
    case IDM_FILE_SAVEAS:       OnFileSave();       return TRUE;
    case IDM_FILE_PREVIEW:      OnFilePreview();    return TRUE;
    case IDM_FILE_PRINT:        OnFilePrint();      return TRUE;
    case IDM_FILE_EXIT:         OnFileExit();       return TRUE;
    case IDW_VIEW_STATUSBAR:    OnViewStatusBar();  return TRUE;
    case IDW_VIEW_TOOLBAR:      OnViewToolBar();    return TRUE;
    case IDM_HELP_ABOUT:        OnHelp();           return TRUE;
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


// Called when the window is closed.
void CMainFrame::OnClose()
{
    SerializeINI(true);
    CFrame::OnClose();
}


// Issue a close request to the frame
void CMainFrame::OnFileExit()
{
    PostMessage(WM_CLOSE);
}


// Called after the frame is created.
void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    // Show the menu and toolbar
    ShowMenu(TRUE);
    ShowToolBar(TRUE);
    
    TRACE("Frame created\n");
}


// Bring up the file open dialog to choose a file.
void CMainFrame::OnFileOpen()
{
    CFileDialog fileDlg(TRUE);

    // Bring up the file open dialog retrieve the selected filename
    if (fileDlg.DoModal(*this) == IDOK)
    {
        // TODO:
        // Add your own code here. Refer to the tutorial for additional information
    }

}


// Save the file.
void CMainFrame::OnFileSave()
{
    CFileDialog fileDlg(FALSE);

    // Bring up the file save dialog retrieve the selected filename
    if (fileDlg.DoModal(*this) == IDOK)
    {
        // TODO:
        // Add your own code here. Refer to the tutorial for additional information
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
        if (printerDC.GetHDC() != 0)        // Verify a print preview is possible
        {
            // Create the preview window if required
            if (!m_preview.IsWindow())
                m_preview.Create(*this);

            // Specify the source of the PrintPage function
            m_preview.SetSource(*this);

            // Set the preview's owner (for messages)
            m_preview.DoPrintPreview(*this);

            // Hide the menu and toolbar
            ShowMenu(FALSE);
            ShowToolBar(FALSE);

            // Swap views
            SetView(m_preview);

            // Update status
            CString status = _T("Printer: ") + printDlg.GetDeviceName();
            SetStatusText(status);
        }
        else
        {
            MessageBox(_T("Print preview requires a printer to copy settings from"), _T("No Printer found"), MB_ICONWARNING);
        }
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);
    }
}


// Bring up a dialog to choose the printer.
void CMainFrame::OnFilePrint()
{
    CPrintDialog printdlg;

    try
    {
        printdlg.DoModal(*this);

        // Retrieve the printer DC
        // CDC dcPrinter = Printdlg.GetPrinterDC();

        // TODO:
        // Add your own code here. Refer to the tutorial for additional information

    }

    catch (const CWinException& e)
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
    // TODO:
    // Add your own code here. Refer to the tutorial for additional information.
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


// Print the page to the specified device context.
void CMainFrame::PrintPage(CDC&, UINT)
{
    // This function is called by m_preview.
    // Code to render the printed page goes here.
}


// Integer to TCHAR. Returns a CString.
CString CMainFrame::ItoT(int i)
{
    // tStringStream is a TCHAR std::stringstream
    tStringStream tss;
    tss << i;
    return CString(tss.str().c_str());
}


// TCHAR to Integer.
int CMainFrame::TtoI(LPCTSTR string)
{
    // tStringStream is a TCHAR std::stringstream
    tStringStream tss(string);
    int res;
    tss >> res;
    return res;
}


// Returns the path used for the INI file.
CString CMainFrame::GetINIPath()
{
    CString filePath = GetAppDataPath();

    if (!filePath.IsEmpty())
    {
        // Create the directory if required
        filePath += _T("\\Win32++");
        CreateDirectory(filePath, NULL);
        filePath += _T("\\INIFrame");
        CreateDirectory(filePath, NULL);

        // Note: on Win2000 and above we could create the folders in a single step:
        // FilePath += _T("\\Win32++\\INIFrame");
        // SHCreateDirectory(NULL, FilePath);   // supported on Win2000 and above
    }
    else
        filePath = _T(".");

    return filePath;
}


// Load values to, or restore values from the ini file.
void CMainFrame::SerializeINI(BOOL isStoring) 
{
    CString fileName = GetINIPath() + _T("\\Frame.ini");
    CString key("Frame Settings");

    WINDOWPLACEMENT wndpl;
    ZeroMemory(&wndpl, sizeof(wndpl));
    wndpl.length = sizeof(wndpl);
    
    if (isStoring)
    {   
        GetWindowPlacement(wndpl);

        CRect rc = wndpl.rcNormalPosition;
        UINT top = MAX(rc.top, 0);
        UINT left = MAX(rc.left, 0);
        UINT width = MAX(rc.Width(), 100);
        UINT height = MAX(rc.Height(), 50);
        UINT showCmd = wndpl.showCmd;

        ::WritePrivateProfileString(NULL, NULL, NULL, fileName);

        // Write the Frame window's position and show state 
        ::WritePrivateProfileString (key, _T("Left"),       ItoT(left), fileName);
        ::WritePrivateProfileString (key, _T("Top"),        ItoT(top), fileName);
        ::WritePrivateProfileString (key, _T("Width"),      ItoT(width), fileName);
        ::WritePrivateProfileString (key, _T("Height"),     ItoT(height), fileName);
        ::WritePrivateProfileString (key, _T("ShowCmd"),    ItoT(showCmd), fileName);

        // Write the StatusBar and ToolBar show state.
        DWORD showStatusBar = GetStatusBar().IsWindow() && GetStatusBar().IsWindowVisible();
        DWORD showToolBar = GetToolBar().IsWindow() && GetToolBar().IsWindowVisible();
        ::WritePrivateProfileString (key, _T("StatusBar"),  ItoT(showStatusBar), fileName);
        ::WritePrivateProfileString (key, _T("ToolBar"),    ItoT(showToolBar), fileName);
    }
    else 
    {
        InitValues values;

        UINT failed = 999999;
        CString error("Error: GPPS failed");
        
        UINT left = ::GetPrivateProfileInt(key, _T("Left"), failed, fileName);
        UINT top = ::GetPrivateProfileInt (key, _T("Top"), failed, fileName);
        UINT width = ::GetPrivateProfileInt (key, _T("Width"), failed, fileName);
        UINT height = ::GetPrivateProfileInt (key, _T("Height"), failed, fileName);
        UINT showCmd = ::GetPrivateProfileInt (key, _T("ShowCmd"), failed, fileName);

        if (left != failed && top != failed && width != failed && height != failed && showCmd != failed) 
        {
            values.position = CRect(left, top, left + width, top + height);
            values.showCmd = showCmd;

            // Set the show state of the status bar
            UINT showStatus = ::GetPrivateProfileInt (key, _T("StatusBar"), 0, fileName);
            if (showStatus != failed)
                values.showStatusBar = showStatus;

            // Set the show state of the tool bar
            UINT showTool = ::GetPrivateProfileInt (key, _T("ToolBar"), 0, fileName);
            if (showTool != failed)
                values.showToolBar = showTool;
        }
        else
        {
            TRACE("Failed to load ini file settings");
            ;;
        }

        SetInitValues(values);
    }
}


// Configure the menu icons.
void CMainFrame::SetupMenuIcons()
{
    // Set the bitmap used for menu icons
    AddMenuIcons(GetToolBarData(), RGB(192, 192, 192), IDB_MENUICONS, 0);
}


// Set the Resource IDs for the toolbar buttons.
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

