/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"

#ifndef SF_USECODEPAGE
  #define SF_USECODEPAGE    0x0020
#endif

//////////////////////////////////
// CMainFrame function definitions
//

// Constructor.
CMainFrame::CMainFrame() : m_isWrapped(false), m_oldFocus(0)
{
}

// Destructor.
CMainFrame::~CMainFrame()
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    SetView(m_richView);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\PrintPreview Sample"));

    // Load the settings from the registry with 5 MRU entries
    LoadRegistryMRUSettings(5);

    return CFrame::Create(parent);
}

// Stream in callback function. Reads from file.
DWORD CALLBACK CMainFrame::MyStreamInCallback(DWORD cookie, LPBYTE pBuffer, LONG cb, LONG *pcb)
{
    // Required for StreamIn
    if (!cb)
        return (1);

    *pcb = 0;
    if (!::ReadFile((HANDLE)(DWORD_PTR)cookie, pBuffer, cb, (LPDWORD)pcb, NULL))
        ::MessageBox(0, _T("ReadFile Failed"), _T(""), MB_OK);

    return 0;
}

// Stream out callback function. Writes to the file.
DWORD CALLBACK CMainFrame::MyStreamOutCallback(DWORD cookie, LPBYTE pBuffer, LONG cb, LONG *pcb)
{
    // Required for StreamOut
    if (!cb)
        return (1);

    *pcb = 0;
    if (!::WriteFile((HANDLE)(DWORD_PTR)cookie, pBuffer, cb, (LPDWORD)pcb, NULL))
        ::MessageBox(0, _T("WriteFile Failed"), _T(""), MB_OK);
    return 0;
}

// Called when the window is closed.
void CMainFrame::OnClose()
{
    //Check for unsaved text
    SaveModifiedText();

    // Call the base function
    CFrame::OnClose();
}

// Handle input from the menu, toolbar or accelerator keys.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch (id)
    {
    case IDM_FILE_NEW:          OnFileNew();            return TRUE;
    case IDM_FILE_OPEN:         OnFileOpen();           return TRUE;
    case IDM_FILE_SAVE:         OnFileSave();           return TRUE;
    case IDM_FILE_SAVEAS:       OnFileSaveAs();         return TRUE;
    case IDM_FILE_PREVIEW:      OnFilePreview();        return TRUE;
    case IDM_FILE_PRINTSETUP:   OnFilePrintSetup();     return TRUE;
    case IDM_FILE_PRINT:        OnFilePrint();          return TRUE;
    case IDM_FILE_QUICKPRINT:   OnFileQuickPrint();     return TRUE;
    case IDM_EDIT_COPY:         OnEditCopy();           return TRUE;
    case IDM_EDIT_PASTE:        OnEditPaste();          return TRUE;
    case IDM_EDIT_CUT:          OnEditCut();            return TRUE;
    case IDM_EDIT_DELETE:       OnEditDelete();         return TRUE;
    case IDM_EDIT_REDO:         OnEditRedo();           return TRUE;
    case IDM_EDIT_UNDO:         OnEditUndo();           return TRUE;
    case IDM_FILE_EXIT:         OnFileExit();           return TRUE;
    case IDW_VIEW_STATUSBAR:    OnViewStatusBar();      return TRUE;
    case IDW_VIEW_TOOLBAR:      OnViewToolBar();        return TRUE;
    case IDM_OPTIONS_WRAP:      OnOptionsWrap();        return TRUE;
    case IDM_OPTIONS_FONT:      OnOptionsFont();        return TRUE;
    case IDM_HELP_ABOUT:        OnHelp();               return TRUE;

    case IDW_FILE_MRU_FILE1:
    case IDW_FILE_MRU_FILE2:
    case IDW_FILE_MRU_FILE3:
    case IDW_FILE_MRU_FILE4:
    case IDW_FILE_MRU_FILE5:    OnFileMRU(wparam);      return TRUE;
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

    // Create the PrintPreview dialog. It is initially hidden.
    m_preview.Create(*this);

    // Get the name of the default or currently chosen printer
    CPrintDialog printDlg;
    if (printDlg.GetDefaults())
    {
        CString status = _T("Printer: ") + printDlg.GetDeviceName();
        SetStatusText(status);
    }
    else
        SetStatusText(_T("No printer found"));

    // call the base class function
    return  CFrame::OnCreate(cs);
}

// Called in response to a EN_DROPFILES notification.
void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
    TCHAR fileName[_MAX_PATH];
    ::DragQueryFile(hDropInfo, 0, fileName, _MAX_PATH);

    if (ReadFile(fileName))
    {
        m_pathName = fileName;
        ReadFile(fileName);
        SetWindowTitle();
        AddMRUEntry(fileName);
    }

    ::DragFinish(hDropInfo);
}

// Cuts the selected text to the clipboard.
void CMainFrame::OnEditCut()
{
    m_richView.Cut();
}

// Copies the selected text to the clipboard.
void CMainFrame::OnEditCopy()
{
    m_richView.Copy();
}

// Pastes text from the clipboard.
void CMainFrame::OnEditPaste()
{
    m_richView.PasteSpecial(CF_TEXT);
}

// Deletes the selected text.
void CMainFrame::OnEditDelete()
{
    m_richView.Clear();
}

// Redoes the next action in the redo queue.
void CMainFrame::OnEditRedo()
{
    m_richView.Redo();
}

// Reverses the last editing operation
void CMainFrame::OnEditUndo()
{
    m_richView.Undo();
}

// Issue a close request to the frame.
void CMainFrame::OnFileExit()
{
    Close();
}

// Load a Most Recently Used file from the menu.
void CMainFrame::OnFileMRU(WPARAM wparam)
{
    UINT mruIndex = LOWORD(wparam) - IDW_FILE_MRU_FILE1;
    CString mruText = GetMRUEntry(mruIndex);

    if (ReadFile(mruText))
        m_pathName = mruText;
    else
        RemoveMRUEntry(mruText);

    SetWindowTitle();
}

// Clear the contents of the rich view.
void CMainFrame::OnFileNew()
{
    //Check for unsaved text
    SaveModifiedText();

    m_richView.SetWindowText(_T(""));
    m_pathName.Empty();
    SetWindowTitle();
    m_richView.SetFontDefaults();
    m_richView.SetModify(FALSE);
}

// Initiate the print preview.
void CMainFrame::OnFilePreview()
{
    try
    {
        // Verify a print preview is possible
        CPrintDialog printDlg;
        CDC printerDC = printDlg.GetPrinterDC();
        if (printerDC.GetHDC() == 0)
            throw CResourceException(_T("No printer available"));

        // Setup the print preview.
        m_preview.SetSource(m_richView);   // CPrintPreview calls m_richView::PrintPage

        // Set the preview's owner (for messages), and number of pages.
        UINT maxPage = m_richView.CollatePages();
        m_preview.DoPrintPreview(*this, maxPage);

        // Save the current Focus.
        SaveFocus();

        // Swap views
        SetView(m_preview);
        m_preview.SetFocus();

        // Hide the menu and toolbar
        ShowMenu(FALSE);
        ShowToolBar(FALSE);
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), _T("Print Preview Failed"), MB_ICONWARNING);
        SetView(m_richView);
        ShowMenu(GetFrameMenu() != 0);
        ShowToolBar(GetToolBar().IsWindow());
    }

}

// Create the File Open dialog and select a file to load.
void CMainFrame::OnFileOpen()
{
    // szFilters is a text string that includes two file name filters:
    // "*.txt" for Text Files" and "*.*' for "All Files."
    LPCTSTR filters = _T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");
    CFileDialog fileDlg(TRUE, _T("txt"), NULL, OFN_FILEMUSTEXIST, filters);

    if (fileDlg.DoModal(*this) == IDOK)
    {
        CString str = fileDlg.GetPathName();

        if (ReadFile(str))
        {
            SetPathName(str);
            AddMRUEntry(str);
            SetWindowTitle();
        }
    }
}

// Initiate the print job.
void CMainFrame::OnFilePrint()
{
    try
    {
        m_richView.DoPrint(m_pathName);
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), _T("Print Failed"), MB_ICONWARNING);
    }

}

// Initiates the print setup dialog to choose print options.
void CMainFrame::OnFilePrintSetup()
{
    // Display the print dialog.
    CPrintDialog printDlg;
    try
    {
        // Display the print dialog
        if (printDlg.DoModal(*this) == IDOK)
        {
            CString status = _T("Printer: ") + printDlg.GetDeviceName();
            SetStatusText(status);
        }
    }

    catch (const CWinException& /* e */)
    {
        // No default printer
        MessageBox(_T("Unable to display print dialog"), _T("Print Failed"), MB_OK);
    }

}

// Start a print job without choosing the printer.
void CMainFrame::OnFileQuickPrint()
{
    m_richView.QuickPrint(m_pathName);
}

// Saves the text to the file, overwriting its contents.
void CMainFrame::OnFileSave()
{
    if (m_pathName.IsEmpty())
        OnFileSaveAs();
    else
        WriteFile(m_pathName);
}

// Save the text to a new file.
void CMainFrame::OnFileSaveAs()
{
    // szFilter is a text string that includes two file name filters:
    // "*.txt" for "Text Files" and "*.*' for "All Files."
    LPCTSTR filters(_T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0"));
    CFileDialog fileDlg(FALSE, _T("txt"), NULL, OFN_OVERWRITEPROMPT, filters);

    if (fileDlg.DoModal(*this) == IDOK)
    {
        CString str = fileDlg.GetPathName();
        WriteFile(str);
        SetPathName(str);
        AddMRUEntry(str);
        SetWindowTitle();
    }

}

// Called after the frame is created.
void CMainFrame::OnInitialUpdate()
{
    DragAcceptFiles(TRUE);
    SetWindowTitle();

    // Show the toolbar
    ShowToolBar(GetToolBar().IsWindow());
}

// Update the menu before it is displayed.
void CMainFrame::OnMenuUpdate(UINT id)
{
    switch (id)
    {
    case IDM_OPTIONS_WRAP:
        GetFrameMenu().CheckMenuItem(id, m_isWrapped ? MF_CHECKED : MF_UNCHECKED);
        break;
    case IDM_EDIT_COPY:
    case IDM_EDIT_CUT:
    case IDM_EDIT_DELETE:
    {
        CHARRANGE range;
        m_richView.GetSel(range);
        UINT flag = (range.cpMin != range.cpMax) ? MF_ENABLED : MF_GRAYED;
        GetFrameMenu().EnableMenuItem(IDM_EDIT_COPY, flag);
        GetFrameMenu().EnableMenuItem(IDM_EDIT_CUT, flag);
        GetFrameMenu().EnableMenuItem(IDM_EDIT_DELETE, flag);
        break;
    }
    case IDM_EDIT_PASTE:
    {
        UINT flag = m_richView.CanPaste(CF_TEXT) ? MF_ENABLED : MF_GRAYED;
        GetFrameMenu().EnableMenuItem(IDM_EDIT_PASTE, flag);
        break;
    }
    case IDM_EDIT_REDO:
    {
        UINT flag = m_richView.CanRedo() ? MF_ENABLED : MF_GRAYED;
        GetFrameMenu().EnableMenuItem(IDM_EDIT_REDO, flag);
        break;
    }
    case IDM_EDIT_UNDO:
    {
        UINT flag = m_richView.CanUndo() ? MF_ENABLED : MF_GRAYED;;
        GetFrameMenu().EnableMenuItem(IDM_EDIT_UNDO, flag);
        break;
    }
    }

    CFrame::OnMenuUpdate(id);
}

// Called when a notification (WM_NOTIFY) is received from a child window.
LRESULT CMainFrame::OnNotify(WPARAM wparam, LPARAM lparam)
{
    NMHDR* pNMH;
    pNMH = (LPNMHDR)lparam;
    switch (pNMH->code)
    {
    case EN_DROPFILES:
    {
        ENDROPFILES* ENDrop = reinterpret_cast<ENDROPFILES*>(lparam);
        HDROP hDropInfo = reinterpret_cast<HDROP>(ENDrop->hDrop);
        OnDropFiles(hDropInfo);
    }
    return TRUE;
    }

    return CFrame::OnNotify(wparam, lparam);
}

// Opens a dialog to choose the font.
void CMainFrame::OnOptionsFont()
{
    // Retrieve the current character format
    CHARFORMAT cf;
    ZeroMemory(&cf, sizeof(cf));
    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_COLOR | CFM_FACE | CFM_EFFECTS;
    m_richView.GetDefaultCharFormat(cf);

    // Display the Choose Font dialog
    CFontDialog dlg(cf, CF_SCREENFONTS | CF_EFFECTS);
    if (dlg.DoModal(*this) == IDOK)
    {
        // Set the Font
        cf = dlg.GetCharFormat();
        m_richView.SetDefaultCharFormat(cf);
    }
}

// Toggles the word wrap.
void CMainFrame::OnOptionsWrap()
{
    m_richView.SetTargetDevice(0, m_isWrapped);
    m_isWrapped = !m_isWrapped;
}

// Called when the Print Preview's "Close" button is pressed.
void CMainFrame::OnPreviewClose()
{
    // Swap the view
    SetView(m_richView);

    // Show the menu and toolbar
    ShowMenu(GetFrameMenu() != 0);
    ShowToolBar(GetToolBar().IsWindow());

    // Restore focus to the window focused before DoPrintPreview was called.
    RestoreFocus();
}

// Called when the Print Preview's "Print Now" button is pressed
void CMainFrame::OnPreviewPrint()
{
    m_richView.QuickPrint(m_pathName);
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
    UINT maxPage = m_richView.CollatePages();
    m_preview.DoPrintPreview(*this, maxPage);
}

// Called by CTextApp::OnIdle to update toolbar buttons
void CMainFrame::OnToolbarUpdate()
{
    CHARRANGE range;
    m_richView.GetSel(range);
    BOOL isSelected = (range.cpMin != range.cpMax);
    BOOL canPaste = m_richView.CanPaste(CF_TEXT);
    BOOL isDirty = m_richView.GetModify();

    GetToolBar().EnableButton(IDM_EDIT_COPY, isSelected);
    GetToolBar().EnableButton(IDM_EDIT_CUT, isSelected);
    GetToolBar().EnableButton(IDM_EDIT_PASTE, canPaste);
    GetToolBar().EnableButton(IDM_FILE_SAVE, isDirty);
}

// Loads text from the specified file.
BOOL CMainFrame::ReadFile(LPCTSTR szFileName)
{
    //Check for unsaved text
    SaveModifiedText();

    try
    {
        // Open the file for reading
        CFile File;
        File.Open(szFileName, OPEN_EXISTING);

        // Restrict the file size to something a richedit control can handle.
        ULONGLONG fileLength = File.GetLength();
        if (fileLength > 100000000)
        {
            throw CFileException(szFileName, _T("File is too large!"));
        }

        EDITSTREAM es;
        es.dwCookie =  reinterpret_cast<DWORD_PTR>(File.GetHandle());
        es.pfnCallback = reinterpret_cast<EDITSTREAMCALLBACK>(MyStreamInCallback);
        m_richView.StreamIn(SF_TEXT, es);

        //Clear the modified text flag
        m_richView.SetModify(FALSE);
    }

    catch (const CFileException& e)
    {
        CString str = _T("Failed to load:  ");
        str += e.GetFilePath();
        str += "\n";
        str += e.GetText();
        ::MessageBox(0, str, AtoT(e.what()), MB_ICONWARNING);
        return FALSE;
    }

    return TRUE;
}

// Asks to save the file if the text has been modified.
void CMainFrame::SaveModifiedText()
{
    //Check for unsaved text
    if (m_richView.GetModify())
        if (::MessageBox(0, _T("Save changes to this document"), _T("TextEdit"), MB_YESNO | MB_ICONWARNING) == IDYES)
            OnFileSave();
}

// Set the name of the text file.
void CMainFrame::SetPathName(LPCTSTR filePathName)
{
    m_pathName = filePathName;
}

// Specifies the images used on menu items.
void CMainFrame::SetupMenuIcons()
{
    // Use the MenuIcons bitmap for images in menu items.
    AddMenuIcons(GetToolBarData(), RGB(192, 192, 192), IDW_MENUICONS, 0);

    // Add more images for menu items.
    AddMenuIcon(IDM_FILE_PRINTSETUP, IDI_PRINTSETUP);
    AddMenuIcon(IDM_FILE_PREVIEW,    IDI_PRINTPREVIEW);
    AddMenuIcon(IDM_FILE_QUICKPRINT, IDI_QUICKPRINT);
    AddMenuIcon(IDM_FILE_PRINT,      IDI_PRINT);
}

// Specifies the Toolbar buttons
void CMainFrame::SetupToolBar()
{
    // Define the resource IDs for the toolbar
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_OPEN  );
    AddToolBarButton( IDM_FILE_SAVE  );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_EDIT_CUT   );
    AddToolBarButton( IDM_EDIT_COPY  );
    AddToolBarButton( IDM_EDIT_PASTE );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_FILE_PRINT );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_HELP_ABOUT );
}

// Sets the caption of the frame.
void CMainFrame::SetWindowTitle()
{
    CString Title;

    if (m_pathName.IsEmpty())
        Title = _T("PrintPreview Demo");
    else
        Title = m_pathName + _T(" - PrintPreview Demo");

    SetWindowText(Title);
}

// Process the window's messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case UWM_PREVIEWCLOSE:      OnPreviewClose();   break;
        case UWM_PRINTNOW:          OnPreviewPrint();   break;
        case UWM_PRINTSETUP:        OnPreviewSetup();   break;
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

// Write the text to the specified file.
BOOL CMainFrame::WriteFile(LPCTSTR fileName)
{
    try
    {
        // Open the file for writing
        CFile file;
        file.Open(fileName, CREATE_ALWAYS);

        EDITSTREAM es;
        es.dwCookie = reinterpret_cast<DWORD_PTR>(file.GetHandle());
        es.dwError = 0;
        es.pfnCallback = reinterpret_cast<EDITSTREAMCALLBACK>(MyStreamOutCallback);

        // Support saving UTF-8 text (without BOM)
        m_richView.StreamOut((CP_UTF8 << 16) | SF_USECODEPAGE | SF_TEXT, es);

        //Clear the modified text flag
        m_richView.SetModify(FALSE);
    }

    catch (const CFileException&)
    {
        CString str = _T("Failed to write:  ");
        str += fileName;
        ::MessageBox(0, str, _T("Warning"), MB_ICONWARNING);
        return FALSE;
    }

    return TRUE;
}


