////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
    // Constructor for CMainFrame. Its called after CFrame's constructor

    // Set m_View as the view window of the frame
    SetView(m_View);

    // Normally we would use LoadRegistrySettings here, but this
    // is omitted in this sample. We use an ini file instead.

    // Load initial values from the ini file.
    SerializeINI(FALSE);
}

CMainFrame::~CMainFrame()
{
    // Destructor for CMainFrame.
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // OnCommand responds to menu and and toolbar input

    UNREFERENCED_PARAMETER(lParam);

    UINT nID = LOWORD(wParam);
    switch(nID)
    {
    case IDM_FILE_OPEN:         return OnFileOpen();
    case IDM_FILE_SAVE:         return OnFileSave();
    case IDM_FILE_SAVEAS:       return OnFileSave();
    case IDM_FILE_PRINT:        return OnFilePrint();
    case IDM_FILE_EXIT:         return OnFileExit();
    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDM_HELP_ABOUT:        return OnHelp();
    }

    return FALSE;
}

int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
    // OnCreate controls the way the frame is created.
    // Overriding CFrame::OnCreate is optional.
    // The default for the following variables is TRUE

    // SetUseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar
    // SetUseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar
    // SetUseReBar(FALSE);              // Don't use a ReBar
    // SetUseThemes(FALSE);             // Don't use themes
    // SetUseToolBar(FALSE);            // Don't use a ToolBar

    // call the base class function
    return CFrame::OnCreate(cs);
}

void CMainFrame::OnClose()
{
    SerializeINI(true);
    CFrame::OnClose();
}

BOOL CMainFrame::OnFileExit()
{
    // Issue a close request to the frame
    PostMessage(WM_CLOSE);
    return TRUE;
}

void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    TRACE("Frame created\n");
}

BOOL CMainFrame::OnFileOpen()
{
    CFileDialog FileDlg(TRUE);

    // Bring up the file open dialog retrieve the selected filename
    if (FileDlg.DoModal(*this) == IDOK)
    {
        // TODO:
        // Add your own code here. Refer to the tutorial for additional information
    }

    return TRUE;
}

BOOL CMainFrame::OnFileSave()
{
    CFileDialog FileDlg(FALSE);

    // Bring up the file save dialog retrieve the selected filename
    if (FileDlg.DoModal(*this) == IDOK)
    {
        // TODO:
        // Add your own code here. Refer to the tutorial for additional information
    }

    return TRUE;
}

BOOL CMainFrame::OnFilePrint()
{
    // Bring up a dialog to choose the printer
    CPrintDialog Printdlg;

    try
    {
        INT_PTR Res = Printdlg.DoModal(*this);

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

LRESULT CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam)
{
    // Process notification messages sent by child windows
//  switch(((LPNMHDR)lParam)->code)
//  {
//      Add case statements for each notification message here
//  }

    // Some notifications should return a value when handled
    return CFrame::OnNotify(wParam, lParam);
}

CString CMainFrame::ItoT(int i)
// Integer to TCHAR. Returns a CString
{
    // tStringStream is a TCHAR std::stringstream
    tStringStream tss;
    tss << i;
    return CString(tss.str().c_str());
}

int CMainFrame::TtoI(LPCTSTR szString)
// TCHAR to Integer.
{
    // tStringStream is a TCHAR std::stringstream
    tStringStream tss(szString);
    int res;
    tss >> res;
    return res;
}

CString CMainFrame::GetINIPath()
// Returns the path used for the INI file.
{
    CString FilePath = GetAppDataPath();

    if (!FilePath.IsEmpty())
    {
        // Create the directory if required
        FilePath += _T("\\Win32++");
        CreateDirectory(FilePath, NULL);
        FilePath += _T("\\INIFrame");
        CreateDirectory(FilePath, NULL);

        // Note: on Win2000 and above we could create the folders in a single step:
        // FilePath += _T("\\Win32++\\INIFrame");
        // SHCreateDirectory(NULL, FilePath);   // supported on Win2000 and above
    }
    else
        FilePath = _T(".");

    return FilePath;
}

void CMainFrame::SerializeINI(BOOL IsStoring) 
// Load values to, or restore values from the ini file
{
    CString FileName = GetINIPath() + _T("\\Frame.ini");
    CString Key("Frame Settings");

    WINDOWPLACEMENT Wndpl;
    ZeroMemory(&Wndpl, sizeof(WINDOWPLACEMENT));
    Wndpl.length = sizeof(WINDOWPLACEMENT);
    
    if (IsStoring)
    {   
        GetWindowPlacement(Wndpl);

        CRect rc = Wndpl.rcNormalPosition;
        UINT top = MAX(rc.top, 0);
        UINT left = MAX(rc.left, 0);
        UINT width = MAX(rc.Width(), 100);
        UINT height = MAX(rc.Height(), 50);
        UINT showCmd = Wndpl.showCmd;

        ::WritePrivateProfileString(NULL, NULL, NULL, FileName);

        // Write the Frame window's position and show state 
        ::WritePrivateProfileString (Key, _T("Left"),       ItoT(left), FileName);
        ::WritePrivateProfileString (Key, _T("Top"),        ItoT(top), FileName);
        ::WritePrivateProfileString (Key, _T("Width"),      ItoT(width), FileName);
        ::WritePrivateProfileString (Key, _T("Height"),     ItoT(height), FileName);
        ::WritePrivateProfileString (Key, _T("ShowCmd"),    ItoT(showCmd), FileName);

        // Write the StatusBar and ToolBar show state.
        DWORD dwShowStatusBar = GetStatusBar().IsWindow() && GetStatusBar().IsWindowVisible();
        DWORD dwShowToolBar = GetToolBar().IsWindow() && GetToolBar().IsWindowVisible();
        ::WritePrivateProfileString (Key, _T("StatusBar"),  ItoT(dwShowStatusBar), FileName);
        ::WritePrivateProfileString (Key, _T("ToolBar"),    ItoT(dwShowToolBar), FileName);
    }
    else 
    {
        InitValues Values;

        UINT failed = 999999;
        CString Error("Error: GPPS failed");
        
        UINT Left = ::GetPrivateProfileInt(Key, _T("Left"), failed, FileName);
        UINT Top = ::GetPrivateProfileInt (Key, _T("Top"), failed, FileName);
        UINT Width = ::GetPrivateProfileInt (Key, _T("Width"), failed, FileName);
        UINT Height = ::GetPrivateProfileInt (Key, _T("Height"), failed, FileName);
        UINT ShowCmd = ::GetPrivateProfileInt (Key, _T("ShowCmd"), failed, FileName);

        if (Left != failed && Top != failed && Width != failed && Height != failed && ShowCmd != failed) 
        {
            Values.rcPos = CRect(Left, Top, Left + Width, Top + Height);
            Values.ShowCmd = ShowCmd;

            // Set the show state of the status bar
            UINT ShowStatus = ::GetPrivateProfileInt (Key, _T("StatusBar"), 0, FileName);
            if (ShowStatus != failed)
                Values.ShowStatusBar = ShowStatus;

            // Set the show state of the tool bar
            UINT ShowTool = ::GetPrivateProfileInt (Key, _T("ToolBar"), 0, FileName);
            if (ShowTool != failed)
                Values.ShowToolBar = ShowTool;
        }
        else
        {
            TRACE("Failed to load ini file settings");
            ;;
        }

        SetInitValues(Values);
    }
}

void CMainFrame::SetupToolBar()
{
    // Set the Resource IDs for the toolbar buttons
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

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//  switch (uMsg)
//  {
//      Add case statements for each messages to be handled here
//  }

    // pass unhandled messages on for default processing
    return WndProcDefault(uMsg, wParam, lParam);
}

