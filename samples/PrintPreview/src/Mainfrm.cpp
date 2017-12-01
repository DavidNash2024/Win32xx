/////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"


// definitions for the CMainFrame class
CMainFrame::CMainFrame() : m_PrintPreview(IDD_PRINTPREVIEW), m_IsWrapped(FALSE)
{
    SetView(m_RichView);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\PrintPreview Sample"));

    // Load the settings from the registry with 5 MRU entries
    LoadRegistryMRUSettings(5);
}

CMainFrame::~CMainFrame()
{
}

CRect CMainFrame::GetPageRect()
// Returns a CRect of the entire printable area. Units are measured in twips.
{
    CRect rcPage;

    // Get the device contect of the default or currently chosen printer
    CPrintDialog PrintDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
    CDC dcPrinter = PrintDlg.GetPrinterDC();

    // Get the printer page specifications
    int nHorizRes = dcPrinter.GetDeviceCaps(HORZRES);
    int nVertRes = dcPrinter.GetDeviceCaps(VERTRES);
    int nLogPixelsX = dcPrinter.GetDeviceCaps(LOGPIXELSX);
    int nLogPixelsY = dcPrinter.GetDeviceCaps(LOGPIXELSY);

    int margin = 200;   // 1440 TWIPS = 1 inch.
    int tpi = 1440;     // twips per inch 

    rcPage.left = margin;
    rcPage.top = margin;
    rcPage.right = (nHorizRes / nLogPixelsX) * tpi - margin;
    rcPage.bottom = (nVertRes / nLogPixelsY) * tpi - margin;

    return rcPage;
}

CRect CMainFrame::GetPrintRect()
// Returns the print area within the page. Units are measured in twips.
{
    int margin = 200;

    CRect rcPage = GetPageRect();
    CRect rcPrintArea;

    if (!rcPage.IsRectEmpty())
    {
        rcPrintArea.left = rcPage.left + margin;
        rcPrintArea.top = rcPage.top + margin;
        rcPrintArea.right = rcPage.right - margin;
        rcPrintArea.bottom = rcPage.bottom - margin;
    }

    return rcPrintArea;
}

DWORD CALLBACK CMainFrame::MyStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
    // Required for StreamIn
    if (!cb)
        return (1);

    *pcb = 0;
    if (!::ReadFile((HANDLE)(DWORD_PTR)dwCookie, pbBuff, cb, (LPDWORD)pcb, NULL))
        ::MessageBox(NULL, _T("ReadFile Failed"), _T(""), MB_OK);

    return 0;
}

DWORD CALLBACK CMainFrame::MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
    // Required for StreamOut
    if (!cb)
        return (1);

    *pcb = 0;
    if (!::WriteFile((HANDLE)(DWORD_PTR)dwCookie, pbBuff, cb, (LPDWORD)pcb, NULL))
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

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    UINT nID = LOWORD(wParam);
    switch (nID)
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
    case IDW_FILE_MRU_FILE5:    return OnFileMRU(wParam);
    }

    return FALSE;
}

int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
    // OnCreate controls the way the frame is created.
    // Overriding CFrame::OnCreate is optional.
    // The default for the following variables is TRUE

    // SetUseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar
    // SetUseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar
    // SetUseReBar(FALSE);              // Don't use a ReBar
    // SetUseThemes(FALSE);             // Don't use themes
    // SetUseToolBar(FALSE);            // Don't use a ToolBar


    // Create the PrintPreview dialog. It is initially hidden.  
    m_PrintPreview.Create(*this);

    // call the base class function
    return  CFrame::OnCreate(cs);
}

BOOL CMainFrame::OnDropFiles(HDROP hDropInfo)
{
    TCHAR szFileName[_MAX_PATH];
    ::DragQueryFile(hDropInfo, 0, szFileName, _MAX_PATH);

    if (ReadFile(szFileName))
    {
        m_PathName = szFileName;
        ReadFile(szFileName);
        SetWindowTitle();
        AddMRUEntry(szFileName);
    }

    return TRUE;
}

BOOL CMainFrame::OnEditCut()
{
    m_RichView.Cut();
    return TRUE;
}

BOOL CMainFrame::OnEditCopy()
{
    m_RichView.Copy();
    return TRUE;
}

BOOL CMainFrame::OnEditPaste()
{
    m_RichView.PasteSpecial(CF_TEXT);
    return TRUE;
}

BOOL CMainFrame::OnEditDelete()
{
    m_RichView.Clear();
    return TRUE;
}

BOOL CMainFrame::OnEditRedo()
{
    m_RichView.Redo();
    return TRUE;
}

BOOL CMainFrame::OnEditUndo()
{
    m_RichView.Undo();
    return TRUE;
}

BOOL CMainFrame::OnFileExit()
{
    // Issue a close request to the frame
    PostMessage(WM_CLOSE);
    return TRUE;
}

BOOL CMainFrame::OnFileMRU(WPARAM wParam)
{
    UINT nMRUIndex = LOWORD(wParam) - IDW_FILE_MRU_FILE1;
    CString strMRUText = GetMRUEntry(nMRUIndex);

    if (ReadFile(strMRUText))
        m_PathName = strMRUText;
    else
        RemoveMRUEntry(strMRUText);

    SetWindowTitle();
    return TRUE;
}

BOOL CMainFrame::OnFileNew()
{
    //Check for unsaved text
    SaveModifiedText();

    m_RichView.SetWindowText(_T(""));
    m_PathName.Empty();
    SetWindowTitle();
    m_RichView.SetFontDefaults();
    m_RichView.SetModify(FALSE);
    return TRUE;
}

BOOL CMainFrame::OnFilePreview()
{
    // Verify a print preview is possible
    CPrintDialog PrintDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
    CDC dcPrinter = PrintDlg.GetPrinterDC();
    if (dcPrinter.GetHDC() == 0)
    {
        MessageBox(_T("Print preview requires a printer to copy settings from"), _T("No Printer found"), MB_ICONWARNING);
        return FALSE;
    }

    m_PrintPreview.DoPrintPreview(GetPageRect(), GetPrintRect());
    SetView(m_PrintPreview);

    // Supress Frame drawing
    SetRedraw(FALSE);
    
    // Hide the menu and toolbar
    ShowMenu(FALSE);
    ShowToolBar(FALSE);
    
    // Re-enable Frame drawing
    SetRedraw(TRUE);

    RedrawWindow();
    return TRUE;
}

BOOL CMainFrame::OnFileOpen()
{
    // szFilters is a text string that includes two file name filters:
    // "*.my" for "MyType Files" and "*.*' for "All Files."
    LPCTSTR szFilters = _T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");
    CFileDialog FileDlg(TRUE, _T("txt"), NULL, OFN_FILEMUSTEXIST, szFilters);

    if (FileDlg.DoModal(*this) == IDOK)
    {
        CString str = FileDlg.GetPathName();

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
    // Prepare the print dialog
    CPrintDialog PrintDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
    PRINTDLG pd = PrintDlg.GetParameters();
    pd.nCopies = 1;
    pd.nFromPage = 0xFFFF;
    pd.nToPage = 0xFFFF;
    pd.nMinPage = 1;
    pd.nMaxPage = 0xFFFF;
    PrintDlg.SetParameters(pd);

    try
    {
        // Display the print dialog
        if (PrintDlg.DoModal(*this) == IDOK)
        {
            QuickPrint(PrintDlg);
        }
        else
            return FALSE;
    }

    catch (const CWinException& /* e */)
    {
        // No default printer
        MessageBox(_T("Unable to display print dialog"), _T("Print Failed"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

BOOL CMainFrame::OnFilePrintSetup()
{
    // Prepare the print dialog
    CPrintDialog PrintDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC | PD_PRINTSETUP);
    PRINTDLG pd = PrintDlg.GetParameters();
    pd.nCopies = 1;
    pd.nFromPage = 0xFFFF;
    pd.nToPage = 0xFFFF;
    pd.nMinPage = 1;
    pd.nMaxPage = 0xFFFF;
    PrintDlg.SetParameters(pd);

    try
    {
        // Display the print dialog
        if (PrintDlg.DoModal(*this) == IDOK)
        {
            CDC dcPrinter = PrintDlg.GetPrinterDC();
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
    // Acquire the currently selected printer and page settings
    CPrintDialog PrintDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);

    QuickPrint(PrintDlg);
    return TRUE;
}

BOOL CMainFrame::OnFileSave()
{
    if (m_PathName.IsEmpty())
        OnFileSaveAs();
    else
        WriteFile(m_PathName);

    return TRUE;
}

BOOL CMainFrame::OnFileSaveAs()
{
    // szFilter is a text string that includes two file name filters:
    // "*.my" for "MyType Files" and "*.*' for "All Files."
    LPCTSTR szFilters(_T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0"));
    CFileDialog FileDlg(FALSE, _T("txt"), NULL, OFN_OVERWRITEPROMPT, szFilters);

    if (FileDlg.DoModal(*this) == IDOK)
    {
        CString str = FileDlg.GetPathName();
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
}

void CMainFrame::OnMenuUpdate(UINT nID)
{
    if (nID == IDM_OPTIONS_WRAP)
    {
        GetFrameMenu().CheckMenuItem(nID, m_IsWrapped ? MF_CHECKED : MF_UNCHECKED);
    }

    CFrame::OnMenuUpdate(nID);
}

LRESULT CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam)
{
    NMHDR* pNMH;
    pNMH = (LPNMHDR)lParam;
    switch (pNMH->code)
    {
    case EN_DROPFILES:
    {
        ENDROPFILES* ENDrop = reinterpret_cast<ENDROPFILES*>(lParam);
        HDROP hDropInfo = reinterpret_cast<HDROP>(ENDrop->hDrop);
        OnDropFiles(hDropInfo);
    }
    return TRUE;
    }

    return CFrame::OnNotify(wParam, lParam);
}

BOOL CMainFrame::OnOptionsFont()
{
    // Retrieve the current character format
    CHARFORMAT2 cf2;
    ZeroMemory(&cf2, sizeof(cf2));
    cf2.cbSize = sizeof(cf2);
    cf2.dwMask = CFM_COLOR | CFM_FACE | CFM_EFFECTS;
    m_RichView.GetDefaultCharFormat(cf2);

    // Fill the LOGFONT struct from CHARFORMAT2
    LOGFONT lf;
    ZeroMemory(&lf, sizeof(lf));
    lstrcpy(lf.lfFaceName, cf2.szFaceName);
    lf.lfHeight = cf2.yHeight / 15;
    lf.lfWeight = (cf2.dwEffects & CFE_BOLD) ? 700 : 400;
    lf.lfItalic = (BYTE)(cf2.dwEffects & CFE_ITALIC);

    // Display the Choose Font dialog
    CFontDialog LogFont(lf, CF_SCREENFONTS | CF_EFFECTS);
    if (LogFont.DoModal(*this) == IDOK)
    {
        // Set the Font
        CFont RichFont(LogFont.GetLogFont());
        m_RichView.SetFont(RichFont, TRUE);

        // Set the font color
        cf2.crTextColor = LogFont.GetColor();
        cf2.dwEffects = 0;
        cf2.dwMask = CFM_COLOR;
        m_RichView.SetDefaultCharFormat(cf2);
    }

    return TRUE;
}

BOOL CMainFrame::OnOptionsWrap()
{
    m_RichView.SetTargetDevice(NULL, m_IsWrapped);
    m_IsWrapped = !m_IsWrapped;
    return TRUE;
}

void CMainFrame::QuickPrint(CPrintDialog& PrintDlg)
// Print the document without bringing up a print dialog
{
    CDC dcPrinter = PrintDlg.GetPrinterDC();

    // Assign values to the FORMATRANGE struct
    FORMATRANGE fr;
    ZeroMemory(&fr, sizeof(fr));
    fr.hdc = dcPrinter;
    fr.hdcTarget = dcPrinter;

    fr.rcPage = GetPageRect();
    fr.rc = GetPrintRect();

    // Default the range of text to print as the entire document.
    fr.chrg.cpMin = 0;
    fr.chrg.cpMax = -1;

    // Start print job.
    DOCINFO di;
    ZeroMemory(&di, sizeof(di));
    di.cbSize = sizeof(DOCINFO);
    di.lpszDocName = m_PathName;
    di.lpszOutput = NULL;   // Do not print to file.
    dcPrinter.StartDoc(&di);

    LONG lTextLength;   // Length of document.
    LONG lTextPrinted;  // Amount of document printed.

    // Find out real size of document in characters.
    lTextLength = m_RichView.GetTextLengthEx(GTL_NUMCHARS);

    do
    {
        // Start the page.
        dcPrinter.StartPage();

        // Print as much text as can fit on a page. The return value is
        // the index of the first character on the next page. Using TRUE
        // for the wParam parameter causes the text to be printed.
        lTextPrinted = m_RichView.FormatRange(fr, TRUE);
        m_RichView.DisplayBand(fr.rc);

        // Print last page.
        dcPrinter.EndPage();

        // If there is more text to print, adjust the range of characters
        // to start printing at the first character of the next page.
        if (lTextPrinted < lTextLength)
        {
            fr.chrg.cpMin = lTextPrinted;
            fr.chrg.cpMax = -1;
        }
    } while (lTextPrinted < lTextLength);

    // Tell the control to release cached information.
    m_RichView.FormatRange();

    // End the print job
    dcPrinter.EndDoc();
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
            return FALSE;
        }

        EDITSTREAM es;
        es.dwCookie =  reinterpret_cast<DWORD_PTR>(File.GetHandle());
        es.pfnCallback = reinterpret_cast<EDITSTREAMCALLBACK>(MyStreamInCallback);
        m_RichView.StreamIn(SF_TEXT, es);

        //Clear the modified text flag
        m_RichView.SetModify(FALSE);
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
    if (m_RichView.GetModify())
        if (::MessageBox(NULL, _T("Save changes to this document"), _T("TextEdit"), MB_YESNO | MB_ICONWARNING) == IDYES)
            OnFileSave();
}

void CMainFrame::SetPathName(LPCTSTR szFilePathName)
{
    m_PathName = szFilePathName;
}

void CMainFrame::SetupMenuIcons()
{
    CFrame::SetupMenuIcons();
    AddMenuIcon(IDM_FILE_PRINTSETUP,    GetApp().LoadIcon(IDI_PRINTSETUP));
    AddMenuIcon(IDM_FILE_PREVIEW,       GetApp().LoadIcon(IDI_PRINTPREVIEW));
    AddMenuIcon(IDM_FILE_QUICKPRINT,    GetApp().LoadIcon(IDI_QUICKPRINT));
    AddMenuIcon(IDM_FILE_PRINT,         GetApp().LoadIcon(IDI_PRINT));
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

    if (m_PathName.IsEmpty())
        Title = _T("PrintPreview Demo");
    else
        Title = m_PathName + _T(" - PrintPreview Demo");

    SetWindowText(Title);
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case UWM_CHANGEVIEW:
        SetView(m_RichView);

        // Supress Frame drawing
        SetRedraw(FALSE);

        // Show the menu and toolbar
        ShowMenu(TRUE);
        ShowToolBar(TRUE);

        // Re-enable frame drawing
        SetRedraw(TRUE);

        RedrawWindow();
        break;

    case UWM_PRINTNOW:
        {
            CPrintDialog PrintDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
            QuickPrint(PrintDlg);
        }
    }

    return WndProcDefault(uMsg, wParam, lParam);
}

BOOL CMainFrame::WriteFile(LPCTSTR szFileName)
{
    try
    {
        // Open the file for writing
        CFile File;
        File.Open(szFileName, CREATE_ALWAYS);

        EDITSTREAM es;
        es.dwCookie = reinterpret_cast<DWORD_PTR>(File.GetHandle());
        es.dwError = 0;
        es.pfnCallback = reinterpret_cast<EDITSTREAMCALLBACK>(MyStreamOutCallback);
        m_RichView.StreamOut(SF_TEXT, es);

        //Clear the modified text flag
        m_RichView.SetModify(FALSE);
    }

    catch (const CFileException&)
    {
        CString str = _T("Failed to write:  ");
        str += szFileName;
        ::MessageBox(NULL, str, _T("Warning"), MB_ICONWARNING);
        return FALSE;
    }

    return TRUE;
}


