////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class


#include "Mainfrm.h"
#include "resource.h"


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
    // Set m_View as the view window of the frame
    SetView(m_view);

    return CFrame::Create(parent);
}

// Process the messages from the Menu and Tool Bar.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    switch (LOWORD(wparam))
    {
    case IDM_FILE_NEW:          OnFileNew();        return TRUE;
    case IDM_FILE_OPEN:         OnFileOpen();       return TRUE;
    case IDM_FILE_SAVE:         OnFileSave();       return TRUE;
    case IDM_FILE_SAVEAS:       OnFileSaveAs();     return TRUE;
    case IDM_FILE_PRINT:        OnFilePrint();      return TRUE;
    case IDM_FILE_EXIT:         OnFileExit();       return TRUE;
    case IDM_PEN_COLOR:         OnPenColor();       return TRUE;
    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDM_HELP_ABOUT:        return OnHelp();
    }

    return FALSE;
}

// Called during window creation.
int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
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

// Called in response to the UWM_DROPFILE user defined message.
LRESULT CMainFrame::OnDropFile(WPARAM wparam)
{
    try
    {
        // wparam is a pointer (LPCTSTR) to the filename
        LPCTSTR fileName = reinterpret_cast<LPCTSTR>(wparam);
        assert(fileName);

        // Load the file
        LoadFile(fileName);
    }

    catch (const CFileException &e)
    {
        m_pathName = _T("");

        // An exception occurred. Display the relevant information.
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);
    }

    return 0;
}

// Issue a close request to the frame.
void CMainFrame::OnFileExit()
{
    Close();
}

// Create a new scribble screen.
void CMainFrame::OnFileNew()
{
    GetDoc().GetAllPoints().clear();
    m_pathName = _T("");
    GetView().Invalidate();
}

// Called by OnFileOpen and in response to a UWM_DROPFILE message.
void CMainFrame::LoadFile(LPCTSTR fileName)
{
    try
    {
        // Retrieve the PlotPoint data.
        GetDoc().FileOpen(fileName);
        m_pathName = fileName;
        GetView().Invalidate();
    }

    catch (const CFileException &e)
    {
        m_pathName = _T("");

        // An exception occurred. Display the relevant information.
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);
    }
}

// Display the file dialog to choose a file to open.
void CMainFrame::OnFileOpen()
{
    try
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

    catch (const CFileException &e)
    {
        m_pathName = _T("");

        // An exception occurred. Display the relevant information.
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);
    }
}

void CMainFrame::OnFilePrint()
{
    MessageBox(_T("File Print  ... Implemented later"), _T("Menu"), MB_OK);
}

// Save the Plotoint data to a file.
void CMainFrame::OnFileSave()
{
    try
    {
        if (m_pathName == _T(""))
            OnFileSaveAs();
        else
            GetDoc().FileSave(m_pathName);
    }

    catch (const CFileException &e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);
    }
}

// Save the Plotoint data to a specified file.
void CMainFrame::OnFileSaveAs()
{
    try
    {
        CFileDialog fileDlg(FALSE, _T("dat"), 0, OFN_OVERWRITEPROMPT, _T("Scribble Files (*.dat)\0*.dat\0\0"));
        fileDlg.SetTitle(_T("Save File"));

        // Bring up the file open dialog retrieve the selected filename
        if (fileDlg.DoModal(*this) == IDOK)
        {
            CString fileName = fileDlg.GetPathName();

            // Save the file
            GetDoc().FileSave(fileName);
            m_pathName = fileName;
            AddMRUEntry(m_pathName);
        }
    }

    catch (const CFileException &e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);
    }
}

// Display a choose color dialog to choose a color.
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

// Configures the ToolBar.
void CMainFrame::SetupToolBar()
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

// Called to handle the window's messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case UWM_DROPFILE:          OnDropFile(wparam); break;
    }

    // Use the default message handling for remaining messages.
    return WndProcDefault(msg, wparam, lparam);
}

