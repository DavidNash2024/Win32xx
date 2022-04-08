////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class


#include "Mainfrm.h"
#include "resource.h"


// Constructor.
CMainFrame::CMainFrame() : m_isToolbarShown(TRUE)
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

// Called by OnFileOpen and in response to a UWM_DROPFILE message.
void CMainFrame::LoadFile(LPCTSTR fileName)
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
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);

        m_pathName = _T("");
        GetDoc().GetAllPoints().clear();
    }
}

// Called when the window is closed.
void CMainFrame::OnClose()
{
    OnPreviewClose();

    // Ends the application.
    CFrame::OnClose();
}

// Process the messages from the Menu and Tool Bar.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    switch (LOWORD(wparam))
    {
    case IDM_FILE_NEW:        return OnFileNew();
    case IDM_FILE_OPEN:       return OnFileOpen();
    case IDM_FILE_SAVE:       return OnFileSave();
    case IDM_FILE_SAVEAS:     return OnFileSaveAs();
    case IDM_FILE_PRINT:      return OnFilePrint();
    case IDM_FILE_PREVIEW:    return OnFilePreview();
    case IDM_PEN_COLOR:       return OnPenColor();
    case IDM_FILE_EXIT:       return OnFileExit();
    case IDW_VIEW_STATUSBAR:  return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:    return OnViewToolBar();
    case IDM_HELP_ABOUT:      return OnHelp();
    }

    return FALSE;
}

// Called during window creation.
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

// Called in response to the UWM_DROPFILE user defined message
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
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);

        GetDoc().GetAllPoints().clear();
    }

    return 0;
}

// Issue a close request to the frame
BOOL CMainFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Create a new scribble screen
BOOL CMainFrame::OnFileNew()
{
    GetDoc().GetAllPoints().clear();
    m_pathName = _T("");
    GetView().Invalidate();
    return TRUE;
}

// Load the PlotPoint data from the file.
BOOL CMainFrame::OnFileOpen()
{
    try
    {
        CFileDialog fileDlg(TRUE, _T("dat"), 0, OFN_FILEMUSTEXIST, _T("Scribble Files (*.dat)\0*.dat\0\0"));
        fileDlg.SetTitle(_T("Open File"));

        // Bring up the file open dialog retrieve the selected file name.
        if (fileDlg.DoModal(*this) == IDOK)
        {
            // Load the file
            LoadFile(fileDlg.GetPathName());
        }
    }

    catch (const CFileException &e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);

        GetDoc().GetAllPoints().clear();
    }

    return TRUE;
}

// Save the PlotPoint data to the current file.
BOOL CMainFrame::OnFileSave()
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

    return TRUE;
}

// Save the PlotPoint data to a specified file.
BOOL CMainFrame::OnFileSaveAs()
{
    try
    {
        CFileDialog fileDlg(FALSE, _T("dat"), 0, OFN_OVERWRITEPROMPT, _T("Scribble Files (*.dat)\0*.dat\0\0"));
        fileDlg.SetTitle(_T("Save File"));

        // Bring up the file open dialog retrieve the selected file name.
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

    return TRUE;
}

// Preview the page before it is printed.
BOOL CMainFrame::OnFilePreview()
{
    try
    {
        m_isToolbarShown = GetToolBar().IsWindow() && GetToolBar().IsWindowVisible();

        // Get the device contect of the default or currently chosen printer
        CPrintDialog printDlg;
        CDC printerDC = printDlg.GetPrinterDC();

        // Create the preview window if required
        if (!m_preview.IsWindow())
            m_preview.Create(*this);

        // Specify the source of the PrintPage function
        m_preview.SetSource(m_view);

        // Set the preview's owner (for notification messages)
        m_preview.DoPrintPreview(*this);

        // Swap views
        SetView(m_preview);

        // Hide the menu and toolbar
        ShowMenu(FALSE);
        ShowToolBar(FALSE);

        // Update status
        CString status = _T("Printer: ") + printDlg.GetDeviceName();
        SetStatusText(status);
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), _T("Print Preview Failed"), MB_ICONWARNING);
        SetView(m_view);
        ShowMenu(GetFrameMenu() != 0);
        ShowToolBar(m_isToolbarShown);
    }

    return TRUE;
}

// Sends the bitmap extracted from the View window to a printer of your choice.
BOOL CMainFrame::OnFilePrint()
{
    try
    {
        // print the view window
        m_view.Print(_T("Scribble Output"));
    }

    catch (const CException& e)
    {
        // Display a message box indicating why printing failed.
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);
    }

    return TRUE;
}

// Called after the frame is created.
void CMainFrame::OnInitialUpdate()
{
    ShowToolBar(GetToolBar().IsWindow());
}

// Initiates the Choose Color dialog.
BOOL CMainFrame::OnPenColor()
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

    return TRUE;
}

// Called when the Print Preview's "Close" button is pressed.
LRESULT CMainFrame::OnPreviewClose()
{
    // Swap the view
    SetView(m_view);

    // Show the menu and toolbar
    ShowMenu(GetFrameMenu() != 0);
    ShowToolBar(m_isToolbarShown);

    SetStatusText(LoadString(IDW_READY));

    return 0;
}

// Called when the Print Preview's "Print Now" button is pressed.
LRESULT CMainFrame::OnPreviewPrint()
{
    try
    {
        m_view.QuickPrint(_T("Scribble Output"));
    }

    catch (const CException& e)
    {
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);
    }

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
            CString status = _T("Printer: ") + printDlg.GetDeviceName();
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

// Called to handle the window's messages;
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case UWM_DROPFILE:        return OnDropFile(wparam);
    case UWM_PREVIEWCLOSE:    return OnPreviewClose();
    case UWM_PREVIEWPRINT:    return OnPreviewPrint();
    case UWM_PREVIEWSETUP:    return OnPreviewSetup();
    }

    // Use the default message handling for remaining messages.
    return WndProcDefault(msg, wparam, lparam);
}

