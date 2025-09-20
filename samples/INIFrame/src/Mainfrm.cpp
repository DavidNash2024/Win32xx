/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"

constexpr COLORREF lightgray = RGB(192, 192, 192);

//////////////////////////////////
// CMainFrame function definitions
//

// Constructor for CMainFrame.
CMainFrame::CMainFrame() : m_preview(m_view), m_isToolbarShown(true)
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    // Set m_view as the view window of the frame.
    SetView(m_view);

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
    fileDlg.SetDefExt(L".cpp");

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
    fileDlg.SetDefExt(L".cpp");

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
        CString status = L"Printer: " + printDlg.GetDeviceName();
        SetStatusText(status);
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), L"Print Preview Failed", MB_ICONWARNING);
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
            m_view.QuickPrint(L"Frame Sample");
        }

    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), L"Print Failed", MB_ICONWARNING);
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
    ShowMenu(GetFrameMenu() != nullptr);
    ShowToolBar(m_isToolbarShown);
    UpdateSettings();

    SetStatusText(LoadString(IDW_READY));

    return 0;
}

// Called when the Print Preview's "Print Now" button is pressed.
LRESULT CMainFrame::OnPreviewPrint()
{
    m_view.QuickPrint(L"Frame Sample");
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
            CString status = L"Printer: " + printDlg.GetDeviceName();
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

// Integer to wchar_t. Returns a CString.
CString CMainFrame::ItoW(int i)
{
    std::basic_stringstream<wchar_t> wss;
    wss << i;
    return CString(wss.str().c_str());
}

// wchar_t to Integer.
int CMainFrame::WtoI(LPCWSTR string)
{
    std::basic_stringstream<wchar_t> wss(string);
    int res;
    wss >> res;
    return res;
}

// Returns the path used for the INI file.
CString CMainFrame::GetINIPath()
{
    CString appDataPath = GetAppDataPath();
    if (!appDataPath.IsEmpty())
    {
        appDataPath += L"\\Win32++\\INIFrame";
        ::SHCreateDirectory(nullptr, appDataPath);

        DWORD attributes = GetFileAttributes(appDataPath);
        if ((attributes == INVALID_FILE_ATTRIBUTES) || !(attributes & FILE_ATTRIBUTE_DIRECTORY))
            throw CFileException(appDataPath, L"Failed to access app directory");
    }
    else
        appDataPath = L".";

    return appDataPath;
}

// Load values to, or restore values from the ini file.
void CMainFrame::SerializeINI(BOOL isStoring)
{
    try
    {
        CString fileName = GetINIPath() + "\\Frame.ini";
        CString key("Frame Settings");

        WINDOWPLACEMENT wndpl{};
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

            ::WritePrivateProfileString(nullptr, nullptr, nullptr, fileName);

            // Write the Frame window's position and show state
            ::WritePrivateProfileString(key, L"Left", ItoW(left), fileName);
            ::WritePrivateProfileString(key, L"Top", ItoW(top), fileName);
            ::WritePrivateProfileString(key, L"Width", ItoW(width), fileName);
            ::WritePrivateProfileString(key, L"Height", ItoW(height), fileName);
            ::WritePrivateProfileString(key, L"ShowCmd", ItoW(showCmd), fileName);

            // Write the StatusBar and ToolBar show state.
            DWORD showStatusBar = GetStatusBar().IsWindow() && GetStatusBar().IsWindowVisible();
            DWORD showToolBar = GetToolBar().IsWindow() && GetToolBar().IsWindowVisible();
            ::WritePrivateProfileString(key, L"StatusBar", ItoW(showStatusBar), fileName);
            ::WritePrivateProfileString(key, L"ToolBar", ItoW(showToolBar), fileName);
        }
        else
        {
            InitValues values;

            UINT failed = 999999;
            CString error("Error: GPPS failed");

            UINT left = ::GetPrivateProfileInt(key, L"Left", failed, fileName);
            UINT top = ::GetPrivateProfileInt(key, L"Top", failed, fileName);
            UINT width = ::GetPrivateProfileInt(key, L"Width", failed, fileName);
            UINT height = ::GetPrivateProfileInt(key, L"Height", failed, fileName);
            UINT showCmd = ::GetPrivateProfileInt(key, L"ShowCmd", failed, fileName);

            if (left != failed && top != failed && width != failed && height != failed && showCmd != failed)
            {
                // Ensure the saved settings are still valid.
                CPoint midpoint(left + width / 2, top + height / 2);
                CPoint midtop( left + width / 2, top);

                HMONITOR monitor = ::MonitorFromPoint(midpoint, MONITOR_DEFAULTTONULL);
                if (monitor == nullptr)
                    throw CUserException();

                MONITORINFO mi{};
                mi.cbSize = sizeof(mi);
                ::GetMonitorInfo(monitor, &mi);
                CRect workArea = mi.rcWork;

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
                UINT showStatus = ::GetPrivateProfileInt(key, L"StatusBar", 0, fileName);
                if (showStatus != failed)
                    values.showStatusBar = showStatus;

                // Set the show state of the tool bar
                UINT showTool = ::GetPrivateProfileInt(key, L"ToolBar", 0, fileName);
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
    if (GetMenuIconHeight() >= 24)
        SetMenuIcons(data, lightgray, IDW_MAIN);
    else
        SetMenuIcons(data, lightgray, IDB_MENUICONS);
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
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    }

    return 0;
}
