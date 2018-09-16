/////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"


// definitions for the CMainFrame class
CMainFrame::CMainFrame() : m_isWrapped(FALSE)
{
    SetView(m_richView);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\Notepad Sample"));

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
    case IDM_FILE_PRINT:        return OnFilePrint();
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

BOOL CMainFrame::OnDropFiles(HDROP hDropInfo)
{
    UINT length = DragQueryFile(hDropInfo, 0, 0, 0);
    if (length > 0)
    {
        CString fileName;
        DragQueryFile(hDropInfo, 0, fileName.GetBuffer(length), length + 1);
        fileName.ReleaseBuffer();

        if (ReadFile(fileName))
        {
            m_pathName = fileName;
            ReadFile(fileName);
            SetWindowTitle();
            AddMRUEntry(fileName);
        }
    }

    DragFinish(hDropInfo);
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

BOOL CMainFrame::OnFilePrint()
{
    // Bring up a dialog to choose the printer
    CPrintDialog printDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);

    PRINTDLG pd = printDlg.GetParameters();
    pd.nCopies = 1;
    pd.nFromPage = 0xFFFF;
    pd.nToPage = 0xFFFF;
    pd.nMinPage = 1;
    pd.nMaxPage = 0xFFFF;
    printDlg.SetParameters(pd);

    try
    {
        if (printDlg.DoModal(*this) == IDOK)
        {
            CDC printerDC = printDlg.GetPrinterDC();

            // This code is based on Microsoft's KB article Q129860

            int horizRes   = printerDC.GetDeviceCaps(HORZRES);
            int vertRes    = printerDC.GetDeviceCaps(VERTRES);
            int logPixelsX = printerDC.GetDeviceCaps(LOGPIXELSX);
            int logPixelsY = printerDC.GetDeviceCaps(LOGPIXELSY);
            LONG textLength;   // Length of document.
            LONG textPrinted;  // Amount of document printed.

            // Ensure the printer DC is in MM_TEXT mode.
            printerDC.SetMapMode(MM_TEXT);

            // Rendering to the same DC we are measuring.
            FORMATRANGE fr;
            ZeroMemory(&fr, sizeof(fr));
            fr.hdc = printerDC.GetHDC();
            fr.hdcTarget = printerDC.GetHDC();

            // Set up the page.
            int margin = 200; // 1440 TWIPS = 1 inch.
            fr.rcPage.left = fr.rcPage.top = margin;
            fr.rcPage.right = (horizRes / logPixelsX) * 1440 - margin;
            fr.rcPage.bottom = (vertRes / logPixelsY) * 1440 - margin;

            // Set up margins all around.
            fr.rc.left = fr.rcPage.left;//+ 1440;
            fr.rc.top = fr.rcPage.top;//+ 1440;
            fr.rc.right = fr.rcPage.right;//- 1440;
            fr.rc.bottom = fr.rcPage.bottom;//- 1440;

            // Default the range of text to print as the entire document.
            fr.chrg.cpMin = 0;
            fr.chrg.cpMax = -1;
            m_richView.FormatRange(fr, TRUE);

            // Set up the print job (standard printing stuff here).
            DOCINFO di;
            ZeroMemory(&di, sizeof(di));
            di.cbSize = sizeof(DOCINFO);
            di.lpszDocName = m_pathName;

            // Do not print to file.
            di.lpszOutput = NULL;

            // Start the document.
            printerDC.StartDoc(&di);

            // Find out real size of document in characters.
            textLength = m_richView.GetTextLengthEx(GTL_NUMCHARS);

            do
            {
                // Start the page.
                printerDC.StartPage();

                // Print as much text as can fit on a page. The return value is
                // the index of the first character on the next page. Using TRUE
                // for the wParam parameter causes the text to be printed.
                textPrinted = m_richView.FormatRange(fr, TRUE);
                m_richView.DisplayBand(fr.rc);

                // Print last page.
                printerDC.EndPage();

                // If there is more text to print, adjust the range of characters
                // to start printing at the first character of the next page.
                if (textPrinted < textLength)
                {
                    fr.chrg.cpMin = textPrinted;
                    fr.chrg.cpMax = -1;
                }
            } while (textPrinted < textLength);

            // Tell the control to release cached information.
            m_richView.FormatRange();

            // End the print job
            printerDC.EndDoc();
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
    NMHDR* pNMH = reinterpret_cast<LPNMHDR>(lparam);

    switch (pNMH->code)
    {
    case EN_DROPFILES:
    {
        ENDROPFILES* enDrop = reinterpret_cast<ENDROPFILES*>(lparam);
        HDROP hDropInfo = reinterpret_cast<HDROP>(enDrop->hDrop);
        OnDropFiles(hDropInfo);
    }
    return TRUE;
    }

    return CFrame::OnNotify(wparam, lparam);
}

BOOL CMainFrame::OnOptionsFont()
{
    // Retrieve the current character format
    CHARFORMAT2 cf2;
    ZeroMemory(&cf2, sizeof(cf2));
    cf2.cbSize = sizeof(cf2);
    cf2.dwMask = CFM_COLOR | CFM_FACE | CFM_EFFECTS;
    m_richView.GetDefaultCharFormat(cf2);

    // Fill the LOGFONT struct from CHARFORMAT2
    LOGFONT lf;
    ZeroMemory(&lf, sizeof(lf));
    StrCopy(lf.lfFaceName, cf2.szFaceName, LF_FACESIZE);
    lf.lfHeight = cf2.yHeight / 15;
    lf.lfWeight = (cf2.dwEffects & CFE_BOLD) ? 700 : 400;
    lf.lfItalic = ( BYTE(cf2.dwEffects) & CFE_ITALIC );

    // Display the Choose Font dialog
    CFontDialog logFont(lf, CF_SCREENFONTS | CF_EFFECTS);
    if (logFont.DoModal(*this) == IDOK)
    {
        // Set the Font
        CFont RichFont(logFont.GetLogFont());
        m_richView.SetFont(RichFont, TRUE);

        // Set the font color
        cf2.crTextColor = logFont.GetColor();
        cf2.dwEffects = 0;
        cf2.dwMask = CFM_COLOR;
        m_richView.SetDefaultCharFormat(cf2);
    }

    return TRUE;
}

BOOL CMainFrame::OnOptionsWrap()
{
    m_richView.SetTargetDevice(NULL, m_isWrapped);
    m_isWrapped = !m_isWrapped;
    return TRUE;
}

BOOL CMainFrame::ReadFile(LPCTSTR fileName)
{
    //Check for unsaved text
    SaveModifiedText();

    try
    {
        // Open the file for reading
        CFile file;
        file.Open(fileName, OPEN_EXISTING);

        // Restrict the file size to something a richedit control can handle.
        ULONGLONG fileLength = file.GetLength();
        if (fileLength > 100000000)
        {
            throw CFileException(fileName, _T("File is too large!"));
        }

        EDITSTREAM es;
        es.dwCookie = reinterpret_cast<DWORD_PTR>(file.GetHandle());
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
    CString title;

    if (m_pathName.IsEmpty())
        title = _T("TextEdit");
    else
        title = m_pathName + _T(" - TextEdit");

    SetWindowText(title);
}

LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
//  switch (msg)
//  {
//
//  }

    return WndProcDefault(msg, wparam, lparam);
}

BOOL CMainFrame::WriteFile(LPCTSTR szFileName)
{
    try
    {
        // Open the file for writing
        CFile file;
        file.Open(szFileName, CREATE_ALWAYS);

        EDITSTREAM es;
        es.dwCookie = reinterpret_cast<DWORD_PTR>(file.GetHandle());
        es.dwError = 0;
        es.pfnCallback = reinterpret_cast<EDITSTREAMCALLBACK>(MyStreamOutCallback);
   //     m_richView.StreamOut(SF_TEXT, es);
        
        // Support saving UTF-8 text (without BOM)
        m_richView.StreamOut((CP_UTF8 << 16) | SF_USECODEPAGE | SF_TEXT, es);

        //Clear the modified text flag
        m_richView.SetModify(FALSE);
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

