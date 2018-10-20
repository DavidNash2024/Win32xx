////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class


#include "mainfrm.h"
#include "resource.h"


CMainFrame::CMainFrame()
{
    // Set m_View as the view window of the frame
    SetView(m_view);
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    // Process the messages from the Menu and Tool Bar

    UNREFERENCED_PARAMETER(lparam);

    switch (LOWORD(wparam))
    {
    case IDM_FILE_NEW:          OnFileNew();        return TRUE;
    case IDM_FILE_OPEN:         OnFileOpen();       return TRUE;
    case IDM_FILE_SAVE:         OnFileSave();       return TRUE;
    case IDM_FILE_SAVEAS:       OnFileSaveAs();     return TRUE;
    case IDM_FILE_PRINT:        OnFilePrint();      return TRUE;
    case IDM_PEN_COLOR:         OnPenColor();       return TRUE;
    case IDM_FILE_EXIT:         OnFileExit();       return TRUE;
    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDM_HELP_ABOUT:        return OnHelp();
    }

    return FALSE;
}

LRESULT CMainFrame::OnDropFile(WPARAM wparam)
// Called in response to the UWM_DROPFILE user defined message
{
    // wparam is a pointer (LPCTSTR) to the filename
    LPCTSTR fileName = reinterpret_cast<LPCTSTR>(wparam);
    assert(fileName);

    // Load the file
    LoadFile(fileName);
    return 0;
}

void CMainFrame::OnFileExit()
{
    // Issue a close request to the frame
    PostMessage(WM_CLOSE);
}

void CMainFrame::OnFileNew()
{
    GetDoc().GetAllPoints().clear();
    m_pathName = _T("");
    GetView().Invalidate();
}

void CMainFrame::LoadFile(LPCTSTR fileName)
// Called by OnFileOpen and in response to a UWM_DROPFILE message
{
    // Retrieve the PlotPoint data
    if (GetDoc().FileOpen(fileName))
    {
        // Save the filename
        m_pathName = fileName;
    }
    else
        m_pathName=_T("");

    GetView().Invalidate();
}

void CMainFrame::OnFileOpen()
{
    CFileDialog fileDlg(TRUE, _T("dat"), 0, OFN_FILEMUSTEXIST, _T("Scribble Files (*.dat)\0*.dat\0\0"));
    fileDlg.SetTitle(_T("Open File"));

    // Bring up the file open dialog retrieve the selected filename
    if (fileDlg.DoModal(*this) == IDOK)
    {
        // Load the file
        LoadFile(fileDlg.GetPathName());
    }
}

void CMainFrame::OnFileSave()
{
    if (m_pathName == _T(""))
        OnFileSaveAs();
    else
        GetDoc().FileSave(m_pathName);
}

void CMainFrame::OnFileSaveAs()
{
    CFileDialog fileDlg(FALSE, _T("dat"), 0, OFN_OVERWRITEPROMPT, _T("Scribble Files (*.dat)\0*.dat\0\0"));
    fileDlg.SetTitle(_T("Save File"));

    // Bring up the file open dialog retrieve the selected filename
    if (fileDlg.DoModal(*this) == IDOK)
    {
        CString fileName = fileDlg.GetPathName();

        // Save the file
        if (GetDoc().FileSave(fileName))
        {
            // Save the file name
            m_pathName = fileName;
            AddMRUEntry(m_pathName);
        }
    }

}

// Sends the bitmap extracted from the View window to a printer of your choice
// This function provides a useful reference for printing bitmaps in general
void CMainFrame::OnFilePrint()
{
    // Pass the print job to CDoc
    GetDoc().Print();
}

void CMainFrame::OnPenColor()
{
    // array of custom colors, initialized to white
    static COLORREF custColors[16] = {  RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255),
                                        RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255),
                                        RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255),
                                        RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255) };
    
    CColorDialog colorDlg;
    colorDlg.SetCustomColors(custColors);
    
    // Initialize the Choose Color dialog
    if (colorDlg.DoModal(*this) == IDOK)
    {
        // Store the custom colors in the static array
        memcpy(custColors, colorDlg.GetCustomColors(), 16*sizeof(COLORREF));
        
        // Retrieve the chosen color
        m_view.SetPenColor(colorDlg.GetColor());
    }
}

void CMainFrame::SetupToolBar()
// Configures the ToolBar
{
    // Define our toolbar buttons
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_OPEN  );
    AddToolBarButton( IDM_FILE_SAVE  );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_EDIT_CUT,   FALSE );
    AddToolBarButton( IDM_EDIT_COPY,  FALSE );
    AddToolBarButton( IDM_EDIT_PASTE, FALSE );
    AddToolBarButton( IDM_FILE_PRINT );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_PEN_COLOR );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_HELP_ABOUT );
	
    // Note: By default a single bitmap with a resource ID of IDW_MAIN and
    //       a color mask of RGB(192,192,192) is used for the ToolBar. 
    //       The color mask is a color used for transparency.	
}

LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
// Called to handle the window's messages
{
    switch (msg)
    {
    case UWM_DROPFILE:      return OnDropFile(wparam);

    }

    //Use the default message handling for remaining messages
    return WndProcDefault(msg, wparam, lparam);
}

