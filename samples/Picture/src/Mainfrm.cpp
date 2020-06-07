////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "Mainfrm.h"
#include "PictureApp.h"
#include "resource.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
    // Constructor for CMainFrame. Its called after CFrame's constructor

    //Set m_View as the view window of the frame
    SetView(m_view);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\Picture Sample"));

    // Load the settings from the registry with 4 MRU entries
    LoadRegistryMRUSettings(4);
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

BOOL CMainFrame::OnFileExit()
{
    // Issue a close request to the frame
    Close();
    return TRUE;
}

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

BOOL CMainFrame::OnFileNew()
{
    m_view.NewPictureFile();
    return TRUE;
}

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

LRESULT CMainFrame::OnFileLoaded(LPCTSTR fileName)
{
    SetWindowText(fileName);
    AdjustFrameRect(m_view.GetImageRect());
    AddMRUEntry(fileName);
    return 0;
}

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

void CMainFrame::SetupToolBar()
{
    // Set the Resource IDs for the toolbar buttons
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
    SetToolBarImages(RGB(192,192,192), IDB_TOOLBAR_NORM, IDB_TOOLBAR_HOT, IDB_TOOLBAR_DIS);
}

LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    // A message defined in PictureApp.h
    case UWM_FILELOADED: return OnFileLoaded((LPCTSTR) lparam);
    case WM_SIZE:
        {
            // Remove scrollbars and redraw the view if the frame is maximized
            if (wparam == SIZE_MAXIMIZED)
            {
                GetView().ShowScrollBar(SB_BOTH, FALSE);
                GetView().Invalidate();
            }
            
            break;
        }
    }

    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}
