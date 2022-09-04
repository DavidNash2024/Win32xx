////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "Mainfrm.h"
#include "ColourDialog.h"
#include "resource.h"

//////////////////////////////////
// CMainFrame function definitions
//

// Constructor for CMainFrame.
CMainFrame::CMainFrame() : m_isToolbarShown(true)
{
}

// Destructor for CMainFrame.
CMainFrame::~CMainFrame()
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    //Set m_MyView as the view window of the frame
    SetView(m_view);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\Fast GDI Demo"));

    // Load the settings from the registry with 4 MRU entries
    LoadRegistryMRUSettings(4);

    return CFrame::Create(parent);
}

// Displays the Color Adjust dialog to choose the red, blue and green adjustments.
BOOL CMainFrame::OnAdjustImage()
{
    if (m_view.GetImage())
    {
        // Initiate the Choose Colour Dialog
        CColourDialog dlg(IDD_DIALOG1, m_view.GetImage());
        if (IDOK == dlg.DoModal())
            ModifyBitmap(dlg.GetRed(), dlg.GetGreen(), dlg.GetBlue(), dlg.IsGray());
    }
    else
        MessageBox(_T("Open a Bitmap file first!"), _T("Error"), MB_OK);

    return TRUE;
}

// Modify the color of the bitmap.
void CMainFrame::ModifyBitmap(int cRed, int cGreen, int cBlue, BOOL isGray)
{
    if (isGray)
        m_view.GetImage().GrayScaleBitmap();
    else
        m_view.GetImage().TintBitmap(cRed, cGreen, cBlue);

    m_view.RedrawWindow(RDW_NOERASE|RDW_INVALIDATE|RDW_UPDATENOW);
}

// Called when the frame window is closed.
void CMainFrame::OnClose()
{
    // Close the preview
    if (GetView() == m_preview)
        OnPreviewClose();

    CFrame::OnClose();
}

// OnCommand responds to menu and and toolbar input
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_FILE_EXIT:       return OnFileExit();
    case IDM_FILE_NEW:        return OnFileNew();
    case IDM_FILE_OPEN:       return OnFileOpen();
    case IDM_FILE_SAVE:       return OnFileSave();
    case IDM_FILE_SAVEAS:     return OnFileSaveAs();
    case IDM_FILE_PREVIEW:    return OnFilePreview();
    case IDM_FILE_PRINT:      return OnFilePrint();
    case IDM_HELP_ABOUT:      return OnHelp();
    case IDM_IMAGE_ADJUST:    return OnAdjustImage();
    case IDW_VIEW_STATUSBAR:  return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:    return OnViewToolBar();
    case IDW_FILE_MRU_FILE1:
    case IDW_FILE_MRU_FILE2:    // Intentionally blank
    case IDW_FILE_MRU_FILE3:
    case IDW_FILE_MRU_FILE4:  return OnFileOpenMRU(wparam, lparam);
    }

    return FALSE;
}

// OnCreate controls the way the frame is created.
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

// Clears any selected image.
BOOL CMainFrame::OnFileNew()
{
    CToolBar& TB = GetToolBar();
    TB.DisableButton(IDM_FILE_SAVEAS);
    TB.DisableButton(IDM_FILE_PRINT);
    TB.DisableButton(IDM_IMAGE_ADJUST);
    m_view.LoadFileImage(0);

    // Set the caption
    SetWindowText(_T("FastGDI"));

    return TRUE;
}

// Load the bitmap from the specified file.
BOOL CMainFrame::LoadFile(CString& fileName)
{
    BOOL IsFileLoaded = m_view.LoadFileImage(fileName);

    if (IsFileLoaded)
    {
        // Save the filename
        m_pathName = fileName;
        AddMRUEntry(fileName);

        // Turn on the ToolBar adjust button
        CToolBar& tb = GetToolBar();
        tb.EnableButton(IDM_FILE_SAVEAS);
        tb.EnableButton(IDM_FILE_PRINT);
        tb.EnableButton(IDM_IMAGE_ADJUST);

        // Resize the frame to match the bitmap
        if (m_view.GetImage())
        {
            CRect rcImage = m_view.GetImageRect();
            AdjustFrameRect(rcImage);
        }

        m_view.RedrawWindow(RDW_NOERASE|RDW_INVALIDATE|RDW_UPDATENOW);

        // Set the caption
        CString str = _T("FastGDI - ") + m_pathName;
        SetWindowText(str);
    }

    return IsFileLoaded;
}

// Displays the File Open dialog, to choose a file to load.
BOOL CMainFrame::OnFileOpen()
{
    CFileDialog fileDlg(TRUE, _T("bmp"), NULL, OFN_FILEMUSTEXIST, _T("Bitmap Files (*.bmp)\0*.bmp\0\0"));

    if (fileDlg.DoModal(*this) == IDOK)
    {
        CString str = fileDlg.GetPathName();
        LoadFile(str);
    }

    return TRUE;
}

// Called when a MRU file is selected from the menu.
BOOL CMainFrame::OnFileOpenMRU(WPARAM wparam, LPARAM)
{
    UINT mruIndex = LOWORD(wparam) - IDW_FILE_MRU_FILE1;
    CString mruText = GetMRUEntry(mruIndex);
    CToolBar& tb = GetToolBar();

    if (m_view.LoadFileImage(mruText))
    {
        m_pathName = mruText;
        tb.EnableButton(IDM_FILE_SAVEAS);
        tb.EnableButton(IDM_FILE_PRINT);
        tb.EnableButton(IDM_IMAGE_ADJUST);

        // Adjust the window size
        CRect rcImage = m_view.GetImageRect();
        AdjustFrameRect(rcImage);
    }
    else
    {
        RemoveMRUEntry(mruText);
        tb.DisableButton(IDM_FILE_SAVEAS);
        tb.DisableButton(IDM_IMAGE_ADJUST);
    }

    // Resize the frame to match the bitmap
    if (m_view.GetImage())
    {
        CRect rcImage = m_view.GetImageRect();
        AdjustFrameRect(rcImage);
    }

    m_view.RedrawWindow(RDW_NOERASE|RDW_INVALIDATE|RDW_UPDATENOW);

    // Set the caption
    CString str = _T("FastGDI - ") + m_pathName;
    SetWindowText(str);

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

// Print the bitmap.
BOOL CMainFrame::OnFilePrint()
{
    try
    {
        m_view.Print(m_pathName);
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), _T("Print Failed"), MB_ICONWARNING);
    }

    return TRUE;
}

// Save the bitmap to the file.
BOOL CMainFrame::OnFileSave()
{
    SaveFile(m_pathName);
    return TRUE;
}

// Save the bitmap to the specified file.
BOOL CMainFrame::OnFileSaveAs()
{
    CFileDialog FileDlg(FALSE, _T("bmp"), NULL, 0, _T("Bitmap Files (*.bmp)\0*.bmp\0\0"));

    if (FileDlg.DoModal(*this) == IDOK)
    {
        CString strName = FileDlg.GetPathName();
        SaveFile(strName);
    }

    return TRUE;
}

// Called after the window is created.
void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.


    TRACE("Frame created\n");
}

// Called when menu items are about to be displayed.
inline void CMainFrame::OnMenuUpdate(UINT id)
{
    bool IsImageLoaded = (m_view.GetImage() != 0);

    switch(id)
    {
    case IDM_FILE_SAVE:
        GetFrameMenu().EnableMenuItem(IDM_FILE_SAVE, IsImageLoaded? MF_ENABLED : MF_GRAYED);
        break;
    case IDM_FILE_SAVEAS:
        GetFrameMenu().EnableMenuItem(IDM_FILE_SAVEAS, IsImageLoaded? MF_ENABLED : MF_GRAYED);
        break;
    case IDM_IMAGE_ADJUST:
        GetFrameMenu().EnableMenuItem(IDM_IMAGE_ADJUST, IsImageLoaded ? MF_ENABLED : MF_GRAYED);
        break;
    case IDM_FILE_PREVIEW:
        GetFrameMenu().EnableMenuItem(IDM_FILE_PREVIEW, IsImageLoaded ? MF_ENABLED : MF_GRAYED);
        break;
    case IDM_FILE_PRINT:
        GetFrameMenu().EnableMenuItem(IDM_FILE_PRINT, IsImageLoaded ? MF_ENABLED : MF_GRAYED);
        break;
    }

    CFrame::OnMenuUpdate(id);
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
        m_view.QuickPrint(m_pathName);
    }

    catch (const CUserException& e)
    {
        // An exception occurred. Display the relevant information.
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

// Saves the current bitmap to the specified file.
void CMainFrame::SaveFile(CString& fileName)
{
    bool DoSave = TRUE;

    try
    {
        CFile File;
        File.Open(fileName, OPEN_EXISTING);

        if (IDYES != MessageBox(_T("File already exists. Do you wish to overwrite it?"), _T("Saving file ") + fileName, MB_YESNO | MB_ICONWARNING))
            DoSave = FALSE;
    }

    catch (const CFileException&)
    {
        // Discard exception
    }

    if (DoSave)
    {
        m_pathName = fileName;

        // Set the caption
        CString Title = _T("FastGDI - ") + m_pathName;
        SetWindowText(Title);

        // Save the file
        m_view.SaveFileImage(fileName);
        AddMRUEntry(fileName);
        TRACE("File Saved\n");
    }
}

// Specifies the images for some of the menu items.
void CMainFrame::SetupMenuIcons()
{
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        SetMenuIcons(data, RGB(192, 192, 192), IDW_MAIN);
    else
        SetMenuIcons(data, RGB(192, 192, 192), IDB_TOOLBAR16);
}

// Set the resource IDs and images for the toolbar buttons.
void CMainFrame::SetupToolBar()
{
    AddToolBarButton( IDM_FILE_NEW  );
    AddToolBarButton( IDM_FILE_OPEN );
    AddToolBarButton( IDM_FILE_SAVEAS, FALSE );
    AddToolBarButton( IDM_FILE_PRINT, FALSE );

    AddToolBarButton( 0 );  // Separator
    AddToolBarButton( IDM_IMAGE_ADJUST, FALSE );

    AddToolBarButton( 0 );  // Separator
    AddToolBarButton( IDM_HELP_ABOUT );
}

// Process the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case UWM_PREVIEWCLOSE:    return OnPreviewClose();
        case UWM_PREVIEWPRINT:    return OnPreviewPrint();
        case UWM_PREVIEWSETUP:    return OnPreviewSetup();
        }

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

