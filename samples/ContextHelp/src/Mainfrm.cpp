/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"


//////////////////////////////////
// CMainFrame function definitions
//

// Constructor
CMainFrame::CMainFrame() : m_view(IDD_DIALOG1), m_isChoosing(FALSE)
{
}

// Destructor for CMainFrame.
CMainFrame::~CMainFrame()
{
}

// Enables choose topic mode
BOOL CMainFrame::ChooseHelpTopic()
{
    ::SetCursor(::LoadCursor(0, IDC_HELP));
    SetCapture();
    m_isChoosing = TRUE;
    return TRUE;
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    // Set the modeless dialog as the view window of the frame
    SetView(m_view);

    CString appName = LoadString(IDS_APP_NAME);
    CString groupFolder = LoadString(IDS_GROUP_FOLDER);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(groupFolder + appName);

    // Set the path to the chm help file for m_AppHelp
    // generate the help file directory and path strings
    CString helpFilename = LoadString(IDS_HELP_FILE);
    CString helpDir = CreateAppDataFolder(groupFolder + appName);
    CString helpPath = helpDir + _T("\\") + helpFilename;

    if (!::PathFileExists(helpPath))
    {
        // The Help file is not in AppDataPath yet so copy it there
        CString OrigHelpPath = _T("..\\help\\") + helpFilename;

        if (!::CopyFile(OrigHelpPath, helpPath, TRUE))
        {
            // The file copy failed, so set the path to the current folder.
            // The Help file might be located with the program's executable.
            helpPath = helpFilename;
        }
    }

    if (::PathFileExists(helpPath))
    {
        // Specify the help file used by CHelp
        m_appHelp.SetHelpFilePath(helpPath);
    }
    else
    {
        ::MessageBox(0, _T("Failed to find ") + LoadString(IDS_HELP_FILE), _T("File not found"), MB_ICONWARNING);
    }

    // generate the Win32++ version string
    UINT ver = _WIN32XX_VER;
    CString win32Version;
    win32Version.Format(_T("using Win32++ Version %d.%d.%d"), ver / 0x100,
        (ver % 0x100) / 0x10, (ver % 0x10));

    // generate compiler information for the About box
    CString sCompiler;
#ifdef __GNUC__
    sCompiler.Format(_T("Gnu C++ %d.%d.%d"), __GNUC__, __GNUC_MINOR__,
        __GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
    sCompiler.Format(_T("MS C++ %d.%d"), _MSC_VER / 100, _MSC_VER % 100);
#elif defined(__BORLANDC__)
    sCompiler = _T("a Borland compiler");
#else
    sCompiler = _T("(unknown compiler name)");
#endif

    // Set the information used in the Help About dialog
    CString aboutBoxInfo = LoadString(IDW_MAIN);
    aboutBoxInfo += _T("\n") + LoadString(IDS_APP_VERSION);
    aboutBoxInfo += _T("\ncompiled with ") + sCompiler;
    aboutBoxInfo += _T(" on ") + CString(__DATE__);
    aboutBoxInfo += _T("\n") + win32Version;
    m_appHelp.SetCredits(aboutBoxInfo);

    return CFrame::Create(parent);
}

// Return a string consisting of the APPDATA folder with the specified
// folder appended. Create this folder if it does not exist. Throws a
// CUserException if the folder creation fails.
CString CMainFrame::CreateAppDataFolder(const CString& subfolder)
{
    ::SetLastError(0);
    CString app_data_path = GetAppDataPath();

    int from = 0;
    int to = subfolder.GetLength();
    while (from < to)
    {
        int nextbk = subfolder.Find(_T("\\"), from);
        int nextfwd = subfolder.Find(_T("/"), from);
        int next = MAX(nextbk, nextfwd);
        if (next < 0)
            next = to;

        CString add = subfolder.Mid(from, next - from);
        app_data_path += _T("\\") + add;
        ::CreateDirectory(app_data_path, 0);

        if ((::CreateDirectory(app_data_path, 0) == 0) && GetLastError() != ERROR_ALREADY_EXISTS)
        {
            CString msg = app_data_path + _T("\nDirectory creation error.");
            throw CUserException(msg);
        }

        from = ++next;
    }

    return app_data_path;
}

// Identifies the window from the cursor position and returns its ID.
UINT CMainFrame::GetIDFromCursorPos()
{
    UINT id = 0;
    CPoint pt = GetCursorPos();
    HWND hCtrl = WindowFromPoint(pt);

    if (hCtrl == GetToolBar().GetHwnd())
    {
        // Over the toolbar window, so identify the toolbar button

        int button = GetToolBar().HitTest();
        id = GetToolBar().GetCommandID(button);
    }
    else if (hCtrl == GetHwnd())
    {
        // Over the frame window. Check to see if we are over a non-client area spot.

        // The WM_NCHITTEST message returns a value indicating the position of the
        //  cursor hot spot within the non-client area.
        int result = static_cast<int>(SendMessage(WM_NCHITTEST, 0, MAKELPARAM(pt.x, pt.y)));

        id = IDFR_NCFRAME + result; // As defined in resource.h
    }
    else if (hCtrl != 0)
    {
        // The view window (dialog), dialog controls, menubar and statusbar all have control IDs.
        id = ::GetDlgCtrlID(hCtrl);
    }

    return id;
}

BOOL CMainFrame::LoadRegistrySettings(LPCTSTR keyName)
{
    CFrame::LoadRegistrySettings(keyName);
    GetDoc().LoadSettings(GetRegistryKeyName().c_str());

    return TRUE;
}

// Processes accelerators, toolbar buttons and menu input
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch(id)
    {
    case ID_CHECK_A:         return m_view.OnCheckA();
    case ID_CHECK_B:         return m_view.OnCheckB();
    case ID_CHECK_C:         return m_view.OnCheckC();

    case IDM_FILE_EXIT:      return OnFileExit();
    case IDW_VIEW_STATUSBAR: return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:   return OnViewToolBar();

    case ID_RADIO_A:
    case ID_RADIO_B:         // intentionally blank
    case ID_RADIO_C:         return m_view.OnRangeOfIDs(ID_RADIO_A, ID_RADIO_C, id);

    case IDM_HELP_ABOUT:    return m_appHelp.About(*this);    // Menu item
    case IDM_HELP_CONTENT:  return ShowHelpTopic(id);         // Menu item
    case IDM_HELP_CONTEXT:  return ChooseHelpTopic();         // Toolbar Button
    case IDM_SHIFT_F1:      return OnShiftF1();               // Accelerator
    case IDM_F1:            return OnF1();                    // Accelerator

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
    int result = CFrame::OnCreate(cs);

    // Add control IDs for various windows
    GetView().SetWindowLongPtr(GWLP_ID, IDD_DIALOG1);
    GetMenuBar().SetWindowLongPtr(GWLP_ID, IDW_MENUBAR);
    GetStatusBar().SetWindowLongPtr(GWLP_ID, IDW_STATUSBAR);

    return result;
}

// Called when the F1 key is pressed.
BOOL CMainFrame::OnF1()
{
    UINT id = GetIDFromCursorPos();

    if (id != 0)
        m_appHelp.ShowHelpTopic(id);
    else
        m_appHelp.ShowHelpTopic(_T("Introduction"));

    return TRUE;
}

// Issue a close request to the frame.
BOOL CMainFrame::OnFileExit()
{
    Close();
    return TRUE;
}

void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

}

// Updates the check state of the various menu items
void CMainFrame::OnMenuUpdate(UINT id)
{

    switch (id)
    {
    case ID_CHECK_A:    OnUpdateCheckA(id);    break;
    case ID_CHECK_B:    OnUpdateCheckB(id);    break;
    case ID_CHECK_C:    OnUpdateCheckC(id);    break;
    }

    if ((id >= ID_RADIO_A) && (id <= ID_RADIO_C))
        OnUpdateRangeOfIDs(ID_RADIO_A, ID_RADIO_C, id);

    CFrame::OnMenuUpdate(id);
}

// Handle the left mouse button click.
// Note: When the frame has mouse capture, clicks over child windows are processed here too.
LRESULT CMainFrame::OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (GetCapture() == *this)
    {
        UINT id = GetIDFromCursorPos();
        ShowHelpTopic(id);
        ReleaseCapture();
        m_isChoosing = FALSE;
        return 0;
    }

    return WndProcDefault(msg, wparam, lparam);
}

// Modifies the cursor when appropriate.
LRESULT CMainFrame::OnSetCursor(UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (m_isChoosing)
    {
        ::SetCursor(::LoadCursor(0, IDC_HELP));
        return TRUE;
    }

    return WndProcDefault(msg, wparam, lparam);
}

// Called when the F1 key is pressed while SHIFT is held down.
BOOL CMainFrame::OnShiftF1()
{
    ChooseHelpTopic();
    return TRUE;
}

// Updates the menu when check box A is clicked.
void CMainFrame::OnUpdateCheckA(UINT id)
{
    BOOL checkA = GetDoc().GetCheckA();
    GetFrameMenu().CheckMenuItem(id, MF_BYCOMMAND | (checkA ? MF_CHECKED : MF_UNCHECKED));
}

// Updates the menu when check box B is clicked.
void CMainFrame::OnUpdateCheckB(UINT id)
{
    BOOL checkB = GetDoc().GetCheckB();
    GetFrameMenu().CheckMenuItem(id, MF_BYCOMMAND | (checkB ? MF_CHECKED : MF_UNCHECKED));
}

// Updates the menu when check box C is clicked.
void CMainFrame::OnUpdateCheckC(UINT id)
{
    BOOL checkC = GetDoc().GetCheckC();
    GetFrameMenu().CheckMenuItem(id, MF_BYCOMMAND | (checkC ? MF_CHECKED : MF_UNCHECKED));
}

// Updates the menu when a radio button is selected.
void CMainFrame::OnUpdateRangeOfIDs(UINT idFirst, UINT idLast, UINT id)
{
    int fileItem = GetMenuItemPos(GetFrameMenu(), _T("Select"));
    CMenu radioMenu = GetFrameMenu().GetSubMenu(fileItem);
    if (GetDoc().GetRadio() == id)
        radioMenu.CheckMenuRadioItem(idFirst, idLast, id, 0);
}

// Sets the CREATESTRUCT parameters before the window is created.
void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
    // Call base clase to set defaults
    CFrame::PreCreate(cs);

    cs.style &= ~WS_MAXIMIZEBOX;
    cs.style &= ~WS_MINIMIZEBOX;
    cs.dwExStyle |= WS_EX_CONTEXTHELP;
}

// Stores settings in the registry.
BOOL CMainFrame::SaveRegistrySettings()
{
    CFrame::SaveRegistrySettings();
    GetDoc().SaveSettings(GetRegistryKeyName().c_str());

    return TRUE;
}

// Specify the icons used in popup menus.
void CMainFrame::SetupMenuIcons()
{
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        SetMenuIcons(data, RGB(255, 0, 255), IDB_TOOLBAR24);
    else
        SetMenuIcons(data, RGB(192, 192, 192), IDB_TOOLBAR16);
}

// Set the resource IDs and images for the toolbar buttons.
void CMainFrame::SetupToolBar()
{
    // Connect button IDs to button icons.
    AddToolBarButton(IDM_FILE_NEW, FALSE);
    AddToolBarButton(IDM_FILE_OPEN, FALSE);
    AddToolBarButton(IDM_FILE_SAVE, FALSE);
    AddToolBarButton(IDM_FILE_SAVEAS, FALSE);
    AddToolBarButton(0);  // Separator
    AddToolBarButton(IDM_EDIT_CUT, FALSE);
    AddToolBarButton(IDM_EDIT_COPY, FALSE);
    AddToolBarButton(IDM_EDIT_PASTE, FALSE);
    AddToolBarButton(0);  // Separator
    AddToolBarButton(IDM_FILE_PRINT, FALSE);
    AddToolBarButton(0);  // Separator
    AddToolBarButton(IDM_HELP_CONTENT);
    AddToolBarButton(IDM_HELP_CONTEXT);
    AddToolBarButton(0);  // Separator

    // Set the toolbar image list.
    SetToolBarImages(RGB(255, 0, 255), IDB_TOOLBAR24);
}

// Called for a System Command such as SC_CLOSE, SC_CONTEXTHELP etc.
LRESULT CMainFrame::OnSysCommand(UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (wparam == SC_CONTEXTHELP)
        ChooseHelpTopic();

    // Now call CFrame's OnSysCommand
    return CFrame::OnSysCommand(msg, wparam, lparam);
}

// Display the context help for the specified topic.
BOOL CMainFrame::ShowHelpTopic(UINT id)
{
    m_appHelp.ShowHelpTopic(id);
    return TRUE;
}

// Handle the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_SETCURSOR:      return OnSetCursor(msg, wparam, lparam);           // Modify the cursor when appropriate
        case WM_SYSCOMMAND:     return OnSysCommand(msg, wparam, lparam);          // Add SC_CONTEXTHELP support
        case WM_HELP:           return 0;                                          // Suppress default handling on F1 and SHIFT F1
        case WM_LBUTTONDOWN:    return OnLButtonDown(msg, wparam, lparam);         // Handle left mouse click
        }

        // Pass unhandled messages on for default processing.
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

