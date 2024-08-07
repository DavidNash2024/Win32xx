/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"

#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif

//////////////////////////////////
// CMainFrame function definitions
//

// Constructor for CMainFrame.
CMainFrame::CMainFrame() : m_preview(m_view), m_isToolbarShown(true)
{
    // Set m_view as the view window of the frame.
    SetView(m_view);
}

// Destructor for CMainFrame.
CMainFrame::~CMainFrame()
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    // Normally we would use LoadRegistrySettings here, but this
    // is omitted in this sample. We use an ini file instead.

    // Load initial values from the ini file.
    SerializeINI(FALSE);

    return CFrame::Create(parent);
}

// OnCommand responds to menu and and toolbar input
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

    // UseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar.
    // UseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar.
    // UseOwnerDrawnMenu(FALSE);     // Don't use owner draw for popup menu items.
    // UseReBar(FALSE);              // Don't use a ReBar.
    // UseStatusBar(FALSE);          // Don't use a StatusBar.
    // UseThemes(FALSE);             // Don't use themes.
    // UseToolBar(FALSE);            // Don't use a ToolBar.

    // call the base class function
    return CFrame::OnCreate(cs);
}

// Called when the frame window is asked to close.
void CMainFrame::OnClose()
{
    // Close the preview.
    if (GetView() == m_preview)
        OnPreviewClose();

    SerializeINI(true);

    // Proceed with closing the frame.
    CFrame::OnClose();
}

// Issue a close request to the frame.
// OnClose is called when the window is closed.
BOOL CMainFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Create the File Open dialog to choose the file to load.
BOOL CMainFrame::OnFileOpen()
{
    CString filter = "Program Files (*.cpp; *.h)|*.cpp; *.h|All Files (*.*)|*.*|";
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
    CString filter = "Program Files (*.cpp; *.h)|*.cpp; *.h|All Files (*.*)|*.*|";
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

        // Get the device context of the default or currently chosen printer.
        CPrintDialog printDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
        CDC printerDC = printDlg.GetPrinterDC();

        // Create the preview window if required.
        if (!m_preview.IsWindow())
            m_preview.Create(*this);

        // Set the preview's owner for notification messages.
        m_preview.DoPrintPreview(*this);

        // Swap views.
        SetView(m_preview);

        // Hide the menu and toolbar.
        ShowMenu(FALSE);
        ShowToolBar(FALSE);

        // Update status.
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

// Called after the frame is created.
// Place any additional startup code here.
void CMainFrame::OnInitialUpdate()
{
    TRACE("Frame created\n");
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
    ShowMenu(GetFrameMenu() != NULL);
    ShowToolBar(m_isToolbarShown);
    UpdateSettings();

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
    CString appDataPath = GetAppDataPath();
    CString subfolder = _T("Win32++\\INIFrame");
    if (!appDataPath.IsEmpty())
    {
        // Create the directory if required
        int from = 0;
        int to = subfolder.GetLength();
        while (from < to)
        {
            int next = subfolder.Find(_T("\\"), from);
            if (next < 0)
                next = to;

            CString add = subfolder.Mid(from, next - from);
            appDataPath += _T("\\") + add;

            if (!::CreateDirectory(appDataPath, NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
            {
                CString msg = appDataPath + _T("Directory creation error.");
                throw CUserException(msg);
            }

            from = ++next;
        }

        DWORD attributes = GetFileAttributes(appDataPath);
        if ((attributes == INVALID_FILE_ATTRIBUTES) || !(attributes & FILE_ATTRIBUTE_DIRECTORY))
            throw CFileException(appDataPath, _T("Failed to access app directory"));

        // Note: on Win2000 and above we could create the folders in a single step:
        // appDataPath += _T("\\Win32++\\INIFrame");
        // SHCreateDirectory(NULL, FilePath);   // supported on Win2000 and above
    }
    else
        appDataPath = _T(".");

    return appDataPath;
}

// Load values to, or restore values from the ini file.
void CMainFrame::SerializeINI(BOOL isStoring)
{
    try
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
            UINT left = rc.left;
            UINT top = rc.top;
            UINT width = rc.Width();
            UINT height = rc.Height();
            width = std::max(width, 100U);
            height = std::max(height, 50U);
            UINT showCmd = wndpl.showCmd;

            ::WritePrivateProfileString(NULL, NULL, NULL, fileName);

            // Write the Frame window's position and show state
            ::WritePrivateProfileString(key, _T("Left"), ItoT(left), fileName);
            ::WritePrivateProfileString(key, _T("Top"), ItoT(top), fileName);
            ::WritePrivateProfileString(key, _T("Width"), ItoT(width), fileName);
            ::WritePrivateProfileString(key, _T("Height"), ItoT(height), fileName);
            ::WritePrivateProfileString(key, _T("ShowCmd"), ItoT(showCmd), fileName);

            // Write the StatusBar and ToolBar show state.
            DWORD showStatusBar = GetStatusBar().IsWindow() && GetStatusBar().IsWindowVisible();
            DWORD showToolBar = GetToolBar().IsWindow() && GetToolBar().IsWindowVisible();
            ::WritePrivateProfileString(key, _T("StatusBar"), ItoT(showStatusBar), fileName);
            ::WritePrivateProfileString(key, _T("ToolBar"), ItoT(showToolBar), fileName);
        }
        else
        {
            InitValues values;

            UINT failed = 999999;
            CString error("Error: GPPS failed");

            UINT left = ::GetPrivateProfileInt(key, _T("Left"), failed, fileName);
            UINT top = ::GetPrivateProfileInt(key, _T("Top"), failed, fileName);
            UINT width = ::GetPrivateProfileInt(key, _T("Width"), failed, fileName);
            UINT height = ::GetPrivateProfileInt(key, _T("Height"), failed, fileName);
            UINT showCmd = ::GetPrivateProfileInt(key, _T("ShowCmd"), failed, fileName);

            if (left != failed && top != failed && width != failed && height != failed && showCmd != failed)
            {
                // Ensure the saved settings are still valid.
                CPoint midpoint(left + width / 2, top + height / 2);
                CPoint midtop( left + width / 2, top);

#ifdef MONITOR_DEFAULTTONULL

                HMONITOR monitor = ::MonitorFromPoint(midpoint, MONITOR_DEFAULTTONULL);
                if (monitor == NULL)
                    throw CUserException();

                MONITORINFO mi;
                ZeroMemory(&mi, sizeof(mi));
                mi.cbSize = sizeof(mi);
                ::GetMonitorInfo(monitor, &mi);
                CRect workArea = mi.rcWork;

#else
                CRect workArea;
                SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
#endif

                // Check if window is mostly within work area.
                if (!workArea.PtInRect(midpoint))
                    throw CUserException();

                // Check if the caption is within the work area.
                if (!workArea.PtInRect(midtop))
                    throw CUserException();

                if (width <= 0 || height <= 0)
                    throw CUserException();

                values.position = CRect(left, top, left + width, top + height);
                values.showCmd = showCmd;

                // Set the show state of the status bar
                UINT showStatus = ::GetPrivateProfileInt(key, _T("StatusBar"), 0, fileName);
                if (showStatus != failed)
                    values.showStatusBar = showStatus;

                // Set the show state of the tool bar
                UINT showTool = ::GetPrivateProfileInt(key, _T("ToolBar"), 0, fileName);
                if (showTool != failed)
                    values.showToolBar = showTool;
            }
            else
            {
                TRACE("Failed to load ini file settings");
                throw CUserException();
            }

            SetInitValues(values);
        }
    }

    catch (const CException&)
    {
        // Saved values are no longer valid. Use defaults.
        InitValues defaultValues;
        SetInitValues(defaultValues);
    }
}

// Specifies the images for some of the menu items.
void CMainFrame::SetupMenuIcons()
{
    // Set the bitmap used for menu icons
    std::vector<UINT> data = GetToolBarData();
    if ((GetMenuIconHeight() >= 24) && (GetWindowDpi(*this) != 192))
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

// Handle the frame's window messages.
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

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << _T("\n") << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(NULL, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(NULL, str1, _T("Error: std::exception"), MB_ICONERROR);
    }

    return 0;
}
