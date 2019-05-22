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
    SetView(m_view);

    // Normally we would use LoadRegistrySettings here, but this
    // is omitted in this sample. We use an ini file instead.

    // Load initial values from the ini file.
    SerializeINI(FALSE);
}

CMainFrame::~CMainFrame()
{
    // Destructor for CMainFrame.
}

BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    // OnCommand responds to menu and and toolbar input

    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_FILE_OPEN:         OnFileOpen();       return TRUE;
    case IDM_FILE_SAVE:         OnFileSave();       return TRUE;
    case IDM_FILE_SAVEAS:       OnFileSave();       return TRUE;
    case IDM_FILE_PRINT:        OnFilePrint();      return TRUE;
    case IDM_FILE_EXIT:         OnFileExit();       return TRUE;
    case IDW_VIEW_STATUSBAR:    OnViewStatusBar();  return TRUE;
    case IDW_VIEW_TOOLBAR:      OnViewToolBar();    return TRUE;
    case IDM_HELP_ABOUT:        OnHelp();           return TRUE;
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
    return CFrame::OnCreate(cs);
}

void CMainFrame::OnClose()
{
    SerializeINI(true);
    CFrame::OnClose();
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

    TRACE("Frame created\n");
}

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

void CMainFrame::OnFilePrint()
{
    // Bring up a dialog to choose the printer
    CPrintDialog printdlg;

    try
    {
        printdlg.DoModal(*this);

        // Retrieve the printer DC
        // CDC dcPrinter = Printdlg.GetPrinterDC();

        // TODO:
        // Add your own code here. Refer to the tutorial for additional information

    }

    catch (const CWinException& /* e */)
    {
        // No default printer
        MessageBox(_T("Unable to display print dialog"), _T("Print Failed"), MB_OK);
    }
}

LRESULT CMainFrame::OnNotify(WPARAM wparam, LPARAM lparam)
{
    // Process notification messages sent by child windows
//  switch(((LPNMHDR)lparam)->code)
//  {
//      Add case statements for each notification message here
//  }

    // Some notifications should return a value when handled
    return CFrame::OnNotify(wparam, lparam);
}

CString CMainFrame::ItoT(int i)
// Integer to TCHAR. Returns a CString
{
    // tStringStream is a TCHAR std::stringstream
    tStringStream tss;
    tss << i;
    return CString(tss.str().c_str());
}

int CMainFrame::TtoI(LPCTSTR string)
// TCHAR to Integer.
{
    // tStringStream is a TCHAR std::stringstream
    tStringStream tss(string);
    int res;
    tss >> res;
    return res;
}

CString CMainFrame::GetINIPath()
// Returns the path used for the INI file.
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

void CMainFrame::SerializeINI(BOOL isStoring) 
// Load values to, or restore values from the ini file
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

void CMainFrame::SetupMenuIcons()
{
    // Set the bitmap used for menu icons
    AddMenuIcons(GetToolBarData(), RGB(192, 192, 192), IDB_MENUICONS, 0);
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

LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
//  switch (msg)
//  {
//      Add case statements for each messages to be handled here
//  }

    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}

