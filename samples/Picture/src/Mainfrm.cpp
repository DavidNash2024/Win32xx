/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "UserMessages.h"
#include "resource.h"

//////////////////////////////////
// CMainFrame function definitions
//

// Constructor.
CMainFrame::CMainFrame()
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
    LoadRegistrySettings(_T("Win32++\\Picture Sample"));

    // Load the settings from the registry with 4 MRU entries
    LoadRegistryMRUSettings(4);

    return CFrame::Create(parent);
}

// OnCommand responds to menu and and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch(id)
    {
    case IDM_FILE_NEW:          return OnFileNew();
    case IDM_FILE_OPEN:         return OnFileOpen();
    case IDM_FILE_SAVE:         return OnFileSaveAs();
    case IDM_FILE_SAVEAS:       return OnFileSaveAs();
    case IDM_FILE_EXIT:         return OnFileExit();
    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDM_HELP_ABOUT:        return OnHelp();

    case IDW_FILE_MRU_FILE1:
    case IDW_FILE_MRU_FILE2:
    case IDW_FILE_MRU_FILE3:
    case IDW_FILE_MRU_FILE4:
    case IDW_FILE_MRU_FILE5:    return OnFileMRU(wparam);
    }

    return FALSE;
}

// Called when the window is created.
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

// Issue a close request to the frame to end the application.
BOOL CMainFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Called when a MRU file selection is made.
BOOL CMainFrame::OnFileMRU(WPARAM wparam)
{
    UINT mruIndex = LOWORD(wparam) - IDW_FILE_MRU_FILE1;
    CString smruText = GetMRUEntry(mruIndex);

    if (!m_view.LoadPictureFile(smruText))
    {
        RemoveMRUEntry(smruText);
        m_view.NewPictureFile();
    }

    return TRUE;
}

// Displays a blank (black) image.
BOOL CMainFrame::OnFileNew()
{
    m_view.NewPictureFile();
    return TRUE;
}

// Displays the file choose dialog an loads an image file.
BOOL CMainFrame::OnFileOpen()
{
    LPCTSTR filters = _T("Supported Files Types(*.bmp;*.gif;*.jpg;*.ico;*.emf;*.wmf)\0*.bmp;*.gif;*.jpg;*.ico;*.emf;*.wmf\0Bitmaps (*.bmp)\0*.bmp\0GIF Files (*.gif)\0*.gif\0JPEG Files (*.jpg)\0*.jpg\0Icons (*.ico)\0*.ico\0Enhanced Metafiles (*.emf)\0*.emf\0Windows Metafiles (*.wmf)\0*.wmf\0\0");

    DWORD flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
    CFileDialog fileDlg(TRUE, NULL, NULL, flags, filters);

    if (fileDlg.DoModal(*this) == IDOK)
    {
        CString str = fileDlg.GetPathName();
        m_view.LoadPictureFile(str);
    }

    return TRUE;
}

// Called when an image has been loaded from a file.
LRESULT CMainFrame::OnFileLoaded(LPCTSTR fileName)
{
    SetWindowText(fileName);
    AdjustFrameRect(m_view.GetImageRect());
    AddMRUEntry(fileName);
    return 0;
}

// Displays the file choose dialog and saves the image to the file.
BOOL CMainFrame::OnFileSaveAs()
{
    if (m_view.GetPicture())
    {
        SHORT Type;
        m_view.GetPicture()->get_Type(&Type);
        LPCTSTR filter = NULL;
        LPCTSTR ext    = NULL;

        // Assign the default file extension and filter.
        // Note: iPicture doesn't convert between file types
        switch(Type)
        {
        case PICTYPE_BITMAP:
            filter = _T("Supported Files Type(*.bmp)\0*.bmp;\0Bitmap (*.bmp)\0*.bmp\0\0");
            ext = _T("bmp");
            break;
        case PICTYPE_METAFILE:
            filter = _T("Supported Files Type(*.wmf)\0*.bmp;\0Metafile (*.wmf)\0*.wmf\0\0");
            ext = _T("wmf");
            break;
        case PICTYPE_ICON:
            filter = _T("Supported Files Type(*.ico)\0*.ico;\0Icon File (*.ico)\0*.ico\0\0");
            ext = _T("ico");
            break;
        case PICTYPE_ENHMETAFILE:
            filter = _T("Supported Files Type(*.emf)\0*.emf;\0Enhanced Metafile (*.emf)\0*.emf\0\0");
            ext = _T("emf");
            break;
        }

        DWORD flags = OFN_OVERWRITEPROMPT;
        CFileDialog fileDlg(FALSE, ext, NULL, flags, filter);

        if (fileDlg.DoModal(*this) == IDOK)
        {
            CString str = fileDlg.GetPathName();
            m_view.SavePicture(str);
            AddMRUEntry(str);
        }
    }

    return TRUE;
}

// Adds images to the popup menu items.
void CMainFrame::SetupMenuIcons()
{
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        SetMenuIcons(data, RGB(192, 192, 192), IDW_MAIN);
    else
        SetMenuIcons(data, RGB(192, 192, 192), IDB_TOOLBAR16);
}

// Sets the resource identifiers and images for the toolbar buttons
void CMainFrame::SetupToolBar()
{
    // Set the resource identifiers for the toolbar buttons
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_OPEN  );
    AddToolBarButton( IDM_FILE_SAVE  );

    AddToolBarButton( 0 );  // Separator
    AddToolBarButton( IDM_EDIT_CUT,   FALSE );
    AddToolBarButton( IDM_EDIT_COPY,  FALSE );
    AddToolBarButton( IDM_EDIT_PASTE, FALSE );

    AddToolBarButton( 0 );  // Separator
    AddToolBarButton( IDM_FILE_PRINT, FALSE );

    AddToolBarButton( 0 );  // Separator
    AddToolBarButton( IDM_HELP_ABOUT );

    // Set the image lists for normal, hot and disabled buttons
    SetToolBarImages(RGB(192,192,192), IDW_MAIN, IDB_TOOLBAR24_HOT, IDB_TOOLBAR24_DIS);
}

// Process the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        // A message defined in PictureApp.h
        case UWM_FILELOADED: return OnFileLoaded((LPCTSTR)lparam);
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
