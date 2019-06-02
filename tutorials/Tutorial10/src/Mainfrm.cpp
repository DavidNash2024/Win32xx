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

void CMainFrame::LoadFile(LPCTSTR fileName)
// Called by OnFileOpen and in response to a UWM_DROPFILE message
{
    try
    {
        // Retrieve the PlotPoint data
        GetDoc().FileOpen(fileName);
        m_pathName = fileName;
        GetView().Invalidate();
    }

    catch (const CFileException &e)
    {
        // An exception occurred. Display the relevant information.
        ::MessageBox(NULL, e.GetText(), _T("Failed to Load File"), MB_ICONWARNING);

        m_pathName = _T("");
        GetDoc().GetAllPoints().clear();
    }
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
    case IDM_FILE_PREVIEW:      OnFilePreview();    return TRUE;
    case IDM_PEN_COLOR:         OnPenColor();       return TRUE;
    case IDM_FILE_EXIT:         OnFileExit();       return TRUE;
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

LRESULT CMainFrame::OnDropFile(WPARAM wparam)
// Called in response to the UWM_DROPFILE user defined message
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
        // An exception occurred. Display the relevant information.
        ::MessageBox(NULL, e.GetText(), _T("Failed to Load File"), MB_ICONWARNING);

        GetDoc().GetAllPoints().clear();
    }

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
        // An exception occurred. Display the relevant information.
        ::MessageBox(NULL, e.GetText(), _T("Failed to Load File"), MB_ICONWARNING);

        GetDoc().GetAllPoints().clear();
    }
}

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
        ::MessageBox(NULL, e.GetText(), _T("Failed to Save File"), MB_ICONWARNING);
    }
}

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
        ::MessageBox(NULL, e.GetText(), _T("Failed to Save File"), MB_ICONWARNING);
    }

}

// Preview the page before it is printed.
void CMainFrame::OnFilePreview()
{
    // Get the device contect of the default or currently chosen printer
    CPrintDialog printDlg;
    CDC printerDC = printDlg.GetPrinterDC();
    if (printerDC.GetHDC() != 0)        // Verify a print preview is possible
    {
        // Create the preview window if required
        if (!m_preview.IsWindow())
            m_preview.Create(*this);

        // Specify the source of the PrintPage function
        m_preview.SetSource(m_view);

        // Set the preview's owner (for notification messages)
        m_preview.DoPrintPreview(*this);

        // Hide the menu and toolbar
        ShowMenu(FALSE);
        ShowToolBar(FALSE);

        // Swap views
        SetView(m_preview);
    }
    else
    {
        MessageBox(_T("Print preview requires a printer to copy settings from"), _T("No Printer found"), MB_ICONWARNING);
    }

}

// Sends the bitmap extracted from the View window to a printer of your choice.
// This function provides a useful reference for printing bitmaps in general.
void CMainFrame::OnFilePrint()
{
    try
    {
        // print the view window
        m_view.Print(_T("Scribble Output"));
    }

    catch (const CException& e)
    {
        // Display a message box indicating why printing failed.
        CString message = CString(e.GetText()) + CString("\n") + e.GetErrorString();
        CString type = CString(e.what());
        ::MessageBox(NULL, message, type, MB_ICONWARNING);
    }
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

void CMainFrame::OnPreviewClose()
{
    // Swap the view
    SetView(m_view);

    // Show the menu and toolbar
    ShowMenu(TRUE);
    ShowToolBar(TRUE);
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

    case UWM_PREVIEWCLOSE:    // Preview Close button pressed.
        OnPreviewClose();
        break;

    case UWM_PRINTNOW:        // Preview Print button pressed.
        m_view.QuickPrint(_T("Scribble Output"));
        break;
    }

    //Use the default message handling for remaining messages
    return WndProcDefault(msg, wparam, lparam);
}

