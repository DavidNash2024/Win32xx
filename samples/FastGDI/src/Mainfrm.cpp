////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "mainfrm.h"
#include "ColourDialog.h"
#include "resource.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
    // Constructor for CMainFrame. Its called after CFrame's constructor

    //Set m_MyView as the view window of the frame
    SetView(m_myView);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\Fast GDI Demo"));

    // Load the settings from the registry with 4 MRU entries
    LoadRegistryMRUSettings(4);
}

CMainFrame::~CMainFrame()
{
    // Destructor for CMainFrame.
}

BOOL CMainFrame::OnAdjustImage()
{
    if (GetMyView().GetImage())
    {
        // Initiate the Colour Adjust Dialog
        CColourDialog Dialog(IDD_DIALOG1, GetMyView().GetImage());
        Dialog.DoModal();
    }
    else
        MessageBox(_T("Open a Bitmap file first!"), _T("Error"), MB_OK);

    return TRUE;
}

void CMainFrame::ModifyBitmap(int cRed, int cGreen, int cBlue, BOOL isGray)
{
    GetMyView().GetImage().TintBitmap(cRed, cGreen, cBlue);
    
    if (isGray)
    {
        GetMyView().GetImage().GrayScaleBitmap();
    }

    GetMyView().RedrawWindow(RDW_NOERASE|RDW_INVALIDATE|RDW_UPDATENOW);
}

BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    // OnCommand responds to menu and and toolbar input

    UINT nID = LOWORD(wparam);
    switch(nID)
    {
    case IDM_FILE_EXIT:         return OnFileExit();
    case IDM_FILE_NEW:          return OnFileNew();
    case IDM_FILE_OPEN:         return OnFileOpen();
    case IDM_FILE_SAVE:         return OnFileSave();
    case IDM_FILE_SAVEAS:       return OnFileSaveAs();
    case IDM_HELP_ABOUT:        return OnHelp();
    case IDM_IMAGE_ADJUST:      return OnAdjustImage();
    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDW_FILE_MRU_FILE1:
    case IDW_FILE_MRU_FILE2:    // Intentionally blank
    case IDW_FILE_MRU_FILE3:
    case IDW_FILE_MRU_FILE4:    return OnFileOpenMRU(wparam, lparam);
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
    PostMessage(WM_CLOSE);
    return TRUE;
}

BOOL CMainFrame::OnFileNew()
{
    CToolBar& TB = GetToolBar();
    TB.DisableButton(IDM_FILE_SAVEAS);
    TB.DisableButton(IDM_IMAGE_ADJUST);
    GetMyView().LoadFileImage(NULL);

    // Set the caption
    SetWindowText(_T("FastGDI"));

    return TRUE;
}

BOOL CMainFrame::LoadFile(CString& fileName)
{
    // Load the bitmap
    BOOL IsFileLoaded = m_myView.LoadFileImage(fileName);

    if (IsFileLoaded)
    {
        // Save the filename
        m_pathName = fileName;
        AddMRUEntry(fileName);

        // Turn on the ToolBar adjust button
        CToolBar& tb = GetToolBar();
		tb.EnableButton(IDM_FILE_SAVEAS);
		tb.EnableButton(IDM_IMAGE_ADJUST);
        GetFrameMenu().EnableMenuItem(IDM_IMAGE_ADJUST, MF_BYCOMMAND | MF_ENABLED);

        // Resize the frame to match the bitmap
        if (GetMyView().GetImage())
        {
            CRect rcImage = GetMyView().GetImageRect();
            AdjustFrameRect(rcImage);
        }

        GetMyView().RedrawWindow(RDW_NOERASE|RDW_INVALIDATE|RDW_UPDATENOW);

        // Set the caption
        CString str = _T("FastGDI - ") + m_pathName;
        SetWindowText(str);
    }

    return IsFileLoaded;
}

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

BOOL CMainFrame::OnFileOpenMRU(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    UINT mruIndex = LOWORD(wparam) - IDW_FILE_MRU_FILE1;
    CString mruText = GetMRUEntry(mruIndex);
    CToolBar& tb = GetToolBar();

    if (m_myView.LoadFileImage(mruText))
    {
        m_pathName = mruText;
		tb.EnableButton(IDM_FILE_SAVEAS);
		tb.EnableButton(IDM_IMAGE_ADJUST);

        // Adjust the window size
        CRect rcImage = GetMyView().GetImageRect();
        AdjustFrameRect(rcImage);
    }
    else
    {
        RemoveMRUEntry(mruText);
		tb.DisableButton(IDM_FILE_SAVEAS);
		tb.DisableButton(IDM_IMAGE_ADJUST);
    }

    // Resize the frame to match the bitmap
    if (GetMyView().GetImage())
    {
        CRect rcImage = GetMyView().GetImageRect();
        AdjustFrameRect(rcImage);
    }

    GetMyView().RedrawWindow(RDW_NOERASE|RDW_INVALIDATE|RDW_UPDATENOW);

    // Set the caption
    CString str = _T("FastGDI - ") + m_pathName;
    SetWindowText(str);
    return TRUE;
}

BOOL CMainFrame::OnFileSave()
{
    SaveFile(m_pathName);
    return TRUE;
}

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

void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    TRACE("Frame created\n");
}

inline void CMainFrame::OnMenuUpdate(UINT id)
// Called when menu items are about to be displayed
{
    bool IsImageLoaded = (GetMyView().GetImage() != 0);

    switch(id)
    {
    case IDM_FILE_SAVE:
        GetFrameMenu().EnableMenuItem(IDM_FILE_SAVE, IsImageLoaded? MF_ENABLED : MF_GRAYED);
        break;
    case IDM_FILE_SAVEAS:
        GetFrameMenu().EnableMenuItem(IDM_FILE_SAVEAS, IsImageLoaded? MF_ENABLED : MF_GRAYED);
        break;
    case IDM_IMAGE_ADJUST:
        GetFrameMenu().EnableMenuItem(IDM_IMAGE_ADJUST, IsImageLoaded? MF_ENABLED : MF_GRAYED);
        break;
    }
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
        m_myView.SaveFileImage(fileName);
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

    AddToolBarButton( 0 );  // Separator
    AddToolBarButton( IDM_IMAGE_ADJUST, FALSE );

    AddToolBarButton( 0 );  // Separator
    AddToolBarButton( IDM_HELP_ABOUT );

    // Use large toolbar buttons
    SetToolBarImages(RGB(192, 192, 192), IDB_TOOLBAR_BIG, 0, 0);
}

LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
//  switch (msg)
//  {
//
//  }

    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}

