/////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"

#ifndef SF_USECODEPAGE
  #define SF_USECODEPAGE	0x0020
#endif


// definitions for the CMainFrame class
CMainFrame::CMainFrame() : m_isWrapped(false)
{
    SetView(m_richView);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\PrintPreview Sample"));

    // Load the settings from the registry with 5 MRU entries
    LoadRegistryMRUSettings(5);
}

CMainFrame::~CMainFrame()
{
}

DWORD CALLBACK CMainFrame::MyStreamInCallback(DWORD cookie, LPBYTE pBuffer, LONG cb, LONG *pcb)
{
    // Required for StreamIn
    if (!cb)
        return (1);

    *pcb = 0;
    if (!::ReadFile((HANDLE)(DWORD_PTR)cookie, pBuffer, cb, (LPDWORD)pcb, NULL))
        ::MessageBox(NULL, _T("ReadFile Failed"), _T(""), MB_OK);

    return 0;
}

DWORD CALLBACK CMainFrame::MyStreamOutCallback(DWORD cookie, LPBYTE pBuffer, LONG cb, LONG *pcb)
{
    // Required for StreamOut
    if (!cb)
        return (1);

    *pcb = 0;
    if (!::WriteFile((HANDLE)(DWORD_PTR)cookie, pBuffer, cb, (LPDWORD)pcb, NULL))
        ::MessageBox(NULL, _T("WriteFile Failed"), _T(""), MB_OK);
    return 0;
}

void CMainFrame::OnClose()
{
    ShowToolBar(TRUE);

    //Check for unsaved text
    SaveModifiedText();

    // Call the base function
    CFrame::OnClose();
}

BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
    switch (id)
    {
    case IDM_FILE_NEW:          return OnFileNew();
    case IDM_FILE_OPEN:         return OnFileOpen();
    case IDM_FILE_SAVE:         return OnFileSave();
    case IDM_FILE_SAVEAS:       return OnFileSaveAs();
    case IDM_FILE_PREVIEW:      return OnFilePreview();
    case IDM_FILE_PRINTSETUP:   return OnFilePrintSetup();
    case IDM_FILE_PRINT:        return OnFilePrint();
    case IDM_FILE_QUICKPRINT:   return OnFileQuickPrint();
    case IDM_EDIT_COPY:         return OnEditCopy();
    case IDM_EDIT_PASTE:        return OnEditPaste();
    case IDM_EDIT_CUT:          return OnEditCut();
    case IDM_EDIT_DELETE:       return OnEditDelete();
    case IDM_EDIT_REDO:         return OnEditRedo();
    case IDM_EDIT_UNDO:         return OnEditUndo();
    case IDM_FILE_EXIT:         return OnFileExit();
    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDM_OPTIONS_WRAP:      return OnOptionsWrap();
    case IDM_OPTIONS_FONT:      return OnOptionsFont();
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

	// Create the PrintPreview dialog. It is initially hidden.  
	m_printPreview.Create(*this);
	
	// Get the name of the default or currently chosen printer
    CPrintDialog printDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
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

BOOL CMainFrame::OnDropFiles(HDROP hDropInfo)
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
    return TRUE;
}

BOOL CMainFrame::OnEditCut()
{
    m_richView.Cut();
    return TRUE;
}

BOOL CMainFrame::OnEditCopy()
{
    m_richView.Copy();
    return TRUE;
}

BOOL CMainFrame::OnEditPaste()
{
    m_richView.PasteSpecial(CF_TEXT);
    return TRUE;
}

BOOL CMainFrame::OnEditDelete()
{
    m_richView.Clear();
    return TRUE;
}

BOOL CMainFrame::OnEditRedo()
{
    m_richView.Redo();
    return TRUE;
}

BOOL CMainFrame::OnEditUndo()
{
    m_richView.Undo();
    return TRUE;
}

BOOL CMainFrame::OnFileExit()
{
    // Issue a close request to the frame
    PostMessage(WM_CLOSE);
    return TRUE;
}

BOOL CMainFrame::OnFileMRU(WPARAM wparam)
{
    UINT mruIndex = LOWORD(wparam) - IDW_FILE_MRU_FILE1;
    CString mruText = GetMRUEntry(mruIndex);

    if (ReadFile(mruText))
        m_pathName = mruText;
    else
        RemoveMRUEntry(mruText);

    SetWindowTitle();
    return TRUE;
}

BOOL CMainFrame::OnFileNew()
{
    //Check for unsaved text
    SaveModifiedText();

    m_richView.SetWindowText(_T(""));
    m_pathName.Empty();
    SetWindowTitle();
    m_richView.SetFontDefaults();
    m_richView.SetModify(FALSE);
    return TRUE;
}

BOOL CMainFrame::OnFilePreview()
{
    // Verify a print preview is possible
    CPrintDialog printDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
    CDC printerDC = printDlg.GetPrinterDC();
    if (printerDC.GetHDC() == 0)
    {
        MessageBox(_T("Print preview requires a printer to copy settings from"), _T("No Printer found"), MB_ICONWARNING);
        return FALSE;
    }

	// Setup the print preview.
	m_printPreview.SetSource(m_richView);   // CPrintPreview calls m_richView::PrintPage
	m_printPreview.UseHalfTone(TRUE);       // Trun of Half tone for text previewing

    // Set the preview's owner (for messages), and number of pages.
    UINT maxPage = m_richView.CollatePages();
    m_printPreview.DoPrintPreview(*this, maxPage);
    
    // Hide the menu and toolbar
    ShowMenu(FALSE);
    ShowToolBar(FALSE);
    
    // Swap views
    SetView(m_printPreview);

    return TRUE;
}

BOOL CMainFrame::OnFileOpen()
{
    // szFilters is a text string that includes two file name filters:
    // "*.my" for "MyType Files" and "*.*' for "All Files."
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

    return TRUE;
}

BOOL CMainFrame::OnFilePrint()
{
    try
    {
        m_richView.DoPrint(m_pathName);
    }

    catch (const CWinException&  /*e*/ )
    {
        // No default printer
        MessageBox(_T("Unable to display print dialog"), _T("Print Failed"), MB_OK);
    }

    return TRUE;
}

BOOL CMainFrame::OnFilePrintSetup()
{
    // Prepare the print dialog
    CPrintDialog printDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC | PD_PRINTSETUP);
    PRINTDLG pd = printDlg.GetParameters();
    pd.nCopies = 1;
    pd.nFromPage = 0xFFFF;
    pd.nToPage = 0xFFFF;
    pd.nMinPage = 1;
    pd.nMaxPage = 0xFFFF;
    printDlg.SetParameters(pd);

    try
    {
        // Display the print dialog
        if (printDlg.DoModal(*this) == IDOK)
        {
            CDC dcPrinter = printDlg.GetPrinterDC();
            CString status = _T("Printer: ") + printDlg.GetDeviceName();
            SetStatusText(status);
        }
    }

    catch (const CWinException& /* e */)
    {
        // No default printer
        MessageBox(_T("Unable to display print dialog"), _T("Print Failed"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

BOOL CMainFrame::OnFileQuickPrint()
{
    m_richView.QuickPrint(m_pathName);
    return TRUE;
}

BOOL CMainFrame::OnFileSave()
{
    if (m_pathName.IsEmpty())
        OnFileSaveAs();
    else
        WriteFile(m_pathName);

    return TRUE;
}

BOOL CMainFrame::OnFileSaveAs()
{
    // szFilter is a text string that includes two file name filters:
    // "*.my" for "MyType Files" and "*.*' for "All Files."
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

    return TRUE;
}

void CMainFrame::OnInitialUpdate()
{
    DragAcceptFiles(TRUE);
    SetWindowTitle();

	// Show the menu and toolbar
	ShowMenu(TRUE);
	ShowToolBar(TRUE);
}

void CMainFrame::OnMenuUpdate(UINT id)
{
    if (id == IDM_OPTIONS_WRAP)
    {
        GetFrameMenu().CheckMenuItem(id, m_isWrapped ? MF_CHECKED : MF_UNCHECKED);
    }

    CFrame::OnMenuUpdate(id);
}

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

BOOL CMainFrame::OnOptionsFont()
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

    return TRUE;
}

BOOL CMainFrame::OnOptionsWrap()
{
    m_richView.SetTargetDevice(NULL, m_isWrapped);
    m_isWrapped = !m_isWrapped;
    return TRUE;
}

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
        ::MessageBox(NULL, str, AtoT(e.what()), MB_ICONWARNING);
        return FALSE;
    }

    return TRUE;
}

void CMainFrame::SaveModifiedText()
{
    //Check for unsaved text
    if (m_richView.GetModify())
        if (::MessageBox(NULL, _T("Save changes to this document"), _T("TextEdit"), MB_YESNO | MB_ICONWARNING) == IDYES)
            OnFileSave();
}

void CMainFrame::SetPathName(LPCTSTR filePathName)
{
    m_pathName = filePathName;
}

void CMainFrame::SetupMenuIcons()
{
    CFrame::SetupMenuIcons();
    AddMenuIcon(IDM_FILE_PRINTSETUP,    GetApp()->LoadIcon(IDI_PRINTSETUP));
    AddMenuIcon(IDM_FILE_PREVIEW,       GetApp()->LoadIcon(IDI_PRINTPREVIEW));
    AddMenuIcon(IDM_FILE_QUICKPRINT,    GetApp()->LoadIcon(IDI_QUICKPRINT));
    AddMenuIcon(IDM_FILE_PRINT,         GetApp()->LoadIcon(IDI_PRINT));
}

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

void CMainFrame::SetWindowTitle()
{
    CString Title;

    if (m_pathName.IsEmpty())
        Title = _T("PrintPreview Demo");
    else
        Title = m_pathName + _T(" - PrintPreview Demo");

    SetWindowText(Title);
}

LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case UWM_PREVIEWCLOSE:
        // Swap the view
        SetView(m_richView);

        // Show the menu and toolbar
        ShowMenu(TRUE);
        ShowToolBar(TRUE);

        break;

    case UWM_PRINTNOW:
        m_richView.QuickPrint(m_pathName);
        break;
    }

    return WndProcDefault(msg, wparam, lparam);
}

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
        ::MessageBox(NULL, str, _T("Warning"), MB_ICONWARNING);
        return FALSE;
    }

    return TRUE;
}


