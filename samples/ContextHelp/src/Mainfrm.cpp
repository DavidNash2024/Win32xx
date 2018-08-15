////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "mainfrm.h"
#include "App.h"
#include "resource.h"



///////////////////////////////////////
// Definitions for the CMainFrame class
CMainFrame::CMainFrame() : m_view(IDD_DIALOG1), m_isChoosing(FALSE)
// Constructor
{
    // Set the modeless dialog as the view window of the frame
    SetView(m_view);

    CString appName = LoadString(IDS_APP_NAME);
    CString groupFolder = LoadString(IDS_GROUP_FOLDER);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(groupFolder + appName);


    ////////////////////////////////////////////////////////
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
        ::MessageBox(NULL, _T("Failed to find ") + LoadString(IDS_HELP_FILE), _T("File not found"), MB_ICONWARNING);
    }

    /////////////////////////////////////////////////
    // generate about box credits (PUT YOUR OWN HERE)

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
}

CMainFrame::~CMainFrame()
{
    // Destructor for CMainFrame.
}

void CMainFrame::ChooseHelpTopic()
// Enables choose topic mode
{
    ::SetCursor(::LoadCursor(NULL, IDC_HELP));
    SetCapture();
    m_isChoosing = TRUE;
}

CString CMainFrame::CreateAppDataFolder(const CString& subfolder)
// Return a string consisting of the APPDATA folder with the specified
// folder appended. Create this folder if it does not exist. Throws a
// CUserException if the folder creation fails.
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

UINT CMainFrame::GetIDFromCursorPos()
// Identifies the window from the cursor position and returns its ID
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
        LRESULT result = SendMessage(WM_NCHITTEST, 0, MAKELPARAM(pt.x, pt.y));

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
    GetDoc().LoadDocRegistry(GetRegistryKeyName().c_str());

    return TRUE;
}

void CMainFrame::NotImplemented() const
// Used for some toolbar buttons
{
    ::MessageBox(NULL, _T("Feature not implemented."),
        _T("Information"), MB_OK | MB_ICONINFORMATION |
        MB_TASKMODAL);
}

BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
// Processes accelerators, toolbar buttons and menu input
{
    UNREFERENCED_PARAMETER(lparam);
    UINT id = LOWORD(wparam);

    switch(id)
    {
    case IDM_FILE_NEW:       NotImplemented();      return TRUE;
    case IDM_FILE_OPEN:      NotImplemented();      return TRUE;
    case IDM_FILE_SAVE:      NotImplemented();      return TRUE;
    case IDM_FILE_SAVEAS:    NotImplemented();      return TRUE;
    case IDM_FILE_PRINT:     NotImplemented();      return TRUE;
    case IDM_FILE_PRINT_PREVIEW: NotImplemented();  return TRUE;
    case IDM_FILE_CLOSE:     NotImplemented();      return TRUE;

    case IDM_EDIT_UNDO:      NotImplemented();      return TRUE;
    case IDM_EDIT_REDO:      NotImplemented();      return TRUE;
    case IDM_EDIT_CUT:       NotImplemented();      return TRUE;
    case IDM_EDIT_COPY:      NotImplemented();      return TRUE;
    case IDM_EDIT_PASTE:     NotImplemented();      return TRUE;
    case IDM_EDIT_FIND:      NotImplemented();      return TRUE;
    case IDM_EDIT_REPLACE:   NotImplemented();      return TRUE;
    case IDM_EDIT_DELETE:    NotImplemented();      return TRUE;

    case IDM_FONT_CHOICE:    NotImplemented();      return TRUE;
    case IDM_COLOR_CHOICE:   NotImplemented();      return TRUE;

    case ID_CHECK_A:         m_view.OnCheckA();     return TRUE;
    case ID_CHECK_B:         m_view.OnCheckB();     return TRUE;
    case ID_CHECK_C:         m_view.OnCheckC();     return TRUE;

    case IDM_FILE_EXIT:      OnFileExit();          return TRUE;
    case IDW_VIEW_STATUSBAR: OnViewStatusBar();     return TRUE;
    case IDW_VIEW_TOOLBAR:   OnViewToolBar();       return TRUE;

    case ID_RADIO_A:
    case ID_RADIO_B:        // intentionally blank
    case ID_RADIO_C:        m_view.OnRangeOfIDs(ID_RADIO_A, ID_RADIO_C, id);   return TRUE;

    case IDM_HELP_ABOUT:    m_appHelp.About(*this); return TRUE;    // Menu item
    case IDM_HELP_CONTENT:  ShowHelpTopic(id);     return TRUE;    // Menu item
    case IDM_HELP_CONTEXT:  ChooseHelpTopic();      return TRUE;    // Toolbar Button
    case IDM_SHIFT_F1:      OnShiftF1();            return TRUE;    // Accelerator
    case IDM_F1:            OnF1();                 return TRUE;    // Accelerator

    }

    return FALSE;
}

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
    int result = CFrame::OnCreate(cs);

    // Add control IDs for various windows
    GetView().SetWindowLongPtr(GWLP_ID, IDD_DIALOG1);
    GetMenuBar().SetWindowLongPtr(GWLP_ID, IDW_MENUBAR);
    GetStatusBar().SetWindowLongPtr(GWLP_ID, IDW_STATUSBAR);

    return result;
}

void CMainFrame::OnF1()
// Called when the F1 key is pressed
{
    UINT id = GetIDFromCursorPos();

    if (id != 0)
        m_appHelp.ShowHelpTopic(id);
    else
        m_appHelp.ShowHelpTopic(_T("Introduction"));
}

void CMainFrame::OnFileExit()
{
    // Issue a close request to the frame
    PostMessage(WM_CLOSE);
}

void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

}

void CMainFrame::OnMenuUpdate(UINT id)
{
    // Update the check state of the various menu items
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

LRESULT CMainFrame::OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
// Handle the left mouse button click.
// Note: When the frame has mouse capture, clicks over child windows are processed here too.
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

LRESULT CMainFrame::OnSetCursor(UINT msg, WPARAM wparam, LPARAM lparam)
// Modify the cursor when appropriate
{
    if (m_isChoosing)
    {
        ::SetCursor(::LoadCursor(NULL, IDC_HELP));
        return TRUE;
    }

    return WndProcDefault(msg, wparam, lparam);
}

void CMainFrame::OnShiftF1()
// Called when the F1 key is pressed while SHIFT is held down
{
    ChooseHelpTopic();
}

void CMainFrame::OnUpdateCheckA(UINT id)
{
    BOOL checkA = GetDoc().GetCheckA();
    GetFrameMenu().CheckMenuItem(id, MF_BYCOMMAND | (checkA ? MF_CHECKED : MF_UNCHECKED));
}

void CMainFrame::OnUpdateCheckB(UINT id)
{
    BOOL checkB = GetDoc().GetCheckB();
    GetFrameMenu().CheckMenuItem(id, MF_BYCOMMAND | (checkB ? MF_CHECKED : MF_UNCHECKED));
}

void CMainFrame::OnUpdateCheckC(UINT id)
{
    BOOL checkC = GetDoc().GetCheckC();
    GetFrameMenu().CheckMenuItem(id, MF_BYCOMMAND | (checkC ? MF_CHECKED : MF_UNCHECKED));
}

void CMainFrame::OnUpdateRangeOfIDs(UINT idFirst, UINT idLast, UINT id)
{
    int fileItem = GetMenuItemPos(GetFrameMenu(), _T("Select"));
    CMenu radioMenu = GetFrameMenu().GetSubMenu(fileItem);
    if (GetDoc().GetRadio() == id)
        radioMenu.CheckMenuRadioItem(idFirst, idLast, id, 0);
}

void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
    // Call base class function first
    CFrame::PreCreate(cs);

    cs.style &= ~WS_MAXIMIZEBOX;
    cs.style &= ~WS_MINIMIZEBOX;
    cs.dwExStyle |= WS_EX_CONTEXTHELP;
}

BOOL CMainFrame::SaveRegistrySettings()
{
    CFrame::SaveRegistrySettings();
    GetDoc().SaveDocRegistry(GetRegistryKeyName().c_str());

    return TRUE;
}

void CMainFrame::SetupToolBar()
{
    // Set the Resource IDs for the toolbar buttons: start out with a
    // separator just to give some space at the left of the toolbar
    AddToolBarButton(0);  // Separator

    // Connect button IDs to button icons, show enabled status, and
    // give the explicit image index iImage of each button in the bitmap.
    // Add the toolbar buttons in the order they are to appear at runtime.
    AddToolBarButton(IDM_FILE_NEW,      TRUE, 0, 0);
    AddToolBarButton(IDM_FILE_OPEN,     TRUE, 0, 1);
    AddToolBarButton(IDM_FILE_SAVE,     TRUE, 0, 2);
    AddToolBarButton(IDM_FILE_SAVEAS,   TRUE, 0, 3);
    AddToolBarButton(IDM_FILE_CLOSE,    TRUE, 0, 4);
    AddToolBarButton(0);  // Separator
    AddToolBarButton(IDM_EDIT_CUT,      TRUE, 0, 5);
    AddToolBarButton(IDM_EDIT_COPY,     TRUE, 0, 6);
    AddToolBarButton(IDM_EDIT_PASTE,    TRUE, 0, 7);
    AddToolBarButton(IDM_EDIT_DELETE,   TRUE, 0, 8);
    AddToolBarButton(0);  // Separator
    AddToolBarButton(IDM_FILE_PRINT_PREVIEW, TRUE, 0, 9);
    AddToolBarButton(IDM_FILE_PRINT,    TRUE, 0, 10);
    AddToolBarButton(0);  // Separator
    AddToolBarButton(IDM_EDIT_UNDO,     TRUE, 0, 11);
    AddToolBarButton(IDM_EDIT_REDO,     TRUE, 0, 12);
    AddToolBarButton(0);  // Separator
    AddToolBarButton(IDM_FONT_CHOICE,   TRUE, 0, 13);
    AddToolBarButton(IDM_COLOR_CHOICE,  TRUE, 0, 14);
    AddToolBarButton(0);  // Separator
    AddToolBarButton(IDM_EDIT_FIND,     TRUE, 0, 15);
    AddToolBarButton(IDM_EDIT_REPLACE,  TRUE, 0, 16);
    AddToolBarButton(0);  // Separator
    AddToolBarButton(IDM_HELP_CONTENT,  TRUE, 0, 17);
    AddToolBarButton(IDM_HELP_CONTEXT,  TRUE, 0, 18);
    AddToolBarButton(0);  // Separator

    // Set the toolbar image list: use defaults for hot and disabled
    SetToolBarImages(RGB(255, 0, 255), IDB_TOOLBAR_NORM, 0, 0);
}

LRESULT CMainFrame::OnSysCommand(UINT msg, WPARAM wparam, LPARAM lparam)
// Called for a System Command such as SC_CLOSE, SC_CONTEXTHELP etc.
{
    if (wparam == SC_CONTEXTHELP)
        ChooseHelpTopic();

    // Now call CFrame's OnSysCommand
    return CFrame::OnSysCommand(msg, wparam, lparam);
}

void CMainFrame::ShowHelpTopic(UINT id)
// Display the context help for the specified topic
{
    m_appHelp.ShowHelpTopic(id);
}

LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_SETCURSOR:      return OnSetCursor(msg, wparam, lparam);           // Modify the cursor when appropriate
    case WM_SYSCOMMAND:     return OnSysCommand(msg, wparam, lparam);          // Add SC_CONTEXTHELP support
    case WM_HELP:           return 0;                                          // Suppress default handling on F1 and SHIFT F1
    case WM_LBUTTONDOWN:    return OnLButtonDown(msg, wparam, lparam);         // Handle left mouse click
    }

    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}


