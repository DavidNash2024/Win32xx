////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "mainfrm.h"
#include "ColourDialog.h"
#include "resource.h"

///////////////////////////////////////
// Definitions for the CMainFrame class

// Constructor for CMainFrame.
CMainFrame::CMainFrame()
{
    // Constructor for CMainFrame. Its called after CFrame's constructor

    //Set m_MyView as the view window of the frame
    SetView(m_view);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\Fast GDI Demo"));

    // Load the settings from the registry with 4 MRU entries
    LoadRegistryMRUSettings(4);
}


// Destructor for CMainFrame.
CMainFrame::~CMainFrame()
{
}


// Displays the Color Adjust dialog to choose the red, blue and green adjustments.
void CMainFrame::OnAdjustImage()
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


// OnCommand responds to menu and and toolbar input
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_FILE_EXIT:         OnFileExit();        return TRUE;
    case IDM_FILE_NEW:          OnFileNew();         return TRUE;
    case IDM_FILE_OPEN:         OnFileOpen();        return TRUE;
    case IDM_FILE_SAVE:         OnFileSave();        return TRUE;
    case IDM_FILE_SAVEAS:       OnFileSaveAs();      return TRUE;
    case IDM_FILE_PREVIEW:      OnFilePreview();     return TRUE;
    case IDM_FILE_PRINT:        OnFilePrint();       return TRUE;
    case IDM_HELP_ABOUT:        OnHelp();            return TRUE;
    case IDM_IMAGE_ADJUST:      OnAdjustImage();     return TRUE;
    case IDW_VIEW_STATUSBAR:    OnViewStatusBar();   return TRUE;
    case IDW_VIEW_TOOLBAR:      OnViewToolBar();     return TRUE;
    case IDW_FILE_MRU_FILE1:
    case IDW_FILE_MRU_FILE2:    // Intentionally blank
    case IDW_FILE_MRU_FILE3:
    case IDW_FILE_MRU_FILE4:    OnFileOpenMRU(wparam, lparam); return TRUE;
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


// Issue a close request to the frame
void CMainFrame::OnFileExit()
{
    PostMessage(WM_CLOSE);
}


// Clears any selected image.
void CMainFrame::OnFileNew()
{
    CToolBar& TB = GetToolBar();
    TB.DisableButton(IDM_FILE_SAVEAS);
    TB.DisableButton(IDM_FILE_PRINT);
    TB.DisableButton(IDM_IMAGE_ADJUST);
    m_view.LoadFileImage(NULL);

    // Set the caption
    SetWindowText(_T("FastGDI"));
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
void CMainFrame::OnFileOpen()
{
    CFileDialog fileDlg(TRUE, _T("bmp"), NULL, OFN_FILEMUSTEXIST, _T("Bitmap Files (*.bmp)\0*.bmp\0\0"));
    
    if (fileDlg.DoModal(*this) == IDOK)
    {
        CString str = fileDlg.GetPathName();
        LoadFile(str);
    }
}


// Called when a MRU file is selected from the menu.
void CMainFrame::OnFileOpenMRU(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

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
}


// Preview the page before it is printed.
void CMainFrame::OnFilePreview()
{
    try
    {
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
        ShowToolBar(GetToolBar().IsWindow());
    }

}


// Print the bitmap.
void CMainFrame::OnFilePrint()
{
    try
    {
        m_view.Print(m_pathName);
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), _T("Print Preview Failed"), MB_ICONWARNING);
    }

}


// Save the bitmap to the file.
void CMainFrame::OnFileSave()
{
    SaveFile(m_pathName);
}


// Save the bitmap to the specified file.
void CMainFrame::OnFileSaveAs()
{
    CFileDialog FileDlg(FALSE, _T("bmp"), NULL, 0, _T("Bitmap Files (*.bmp)\0*.bmp\0\0"));

    if (FileDlg.DoModal(*this) == IDOK)
    {
        CString strName = FileDlg.GetPathName();        
        SaveFile(strName);
    }
    
}


// Called after the window is created.
void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    // Show the menu and toolbar
    ShowMenu(GetFrameMenu() != 0);
    ShowToolBar(GetToolBar().IsWindow());

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
}


// Called when the Print Preview's "Close" button is pressed.
void CMainFrame::OnPreviewClose()
{
    // Swap the view
    SetView(m_view);

    // Show the menu and toolbar
    ShowMenu(GetFrameMenu() != 0);
    ShowToolBar(GetToolBar().IsWindow());

    SetStatusText(LoadString(IDW_READY));
}


// Called when the Print Preview's "Print Now" button is pressed.
void CMainFrame::OnPreviewPrint()
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
}


// Called when the Print Preview's "Print Setup" button is pressed.
void CMainFrame::OnPreviewSetup()
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
}


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

void CMainFrame::SetupToolBar()
{
    // Set the Resource IDs for the toolbar buttons
    AddToolBarButton( IDM_FILE_NEW  );
    AddToolBarButton( IDM_FILE_OPEN );
    AddToolBarButton( IDM_FILE_SAVEAS, FALSE );
    AddToolBarButton( IDM_FILE_PRINT, FALSE );

    AddToolBarButton( 0 );  // Separator
    AddToolBarButton( IDM_IMAGE_ADJUST, FALSE );

    AddToolBarButton( 0 );  // Separator
    AddToolBarButton( IDM_HELP_ABOUT );

    // Use large toolbar buttons
    SetToolBarImages(RGB(192, 192, 192), IDB_TOOLBAR_BIG, 0, 0);
}


// Handle the frame's messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case UWM_PREVIEWCLOSE:      OnPreviewClose();   break;
    case UWM_PRINTNOW:          OnPreviewPrint();   break;
    case UWM_PRINTSETUP:        OnPreviewSetup();   break;
    }

    return WndProcDefault(msg, wparam, lparam);
}

