/////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"

#ifndef SF_USECODEPAGE
  #define SF_USECODEPAGE    0x0020
#endif


// definitions for the CMainFrame class
CMainFrame::CMainFrame() : m_encoding(ANSI), m_isWrapped(false), m_isRTF(false)
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

void CMainFrame::ClearContents()
{
    m_richView.SendMessage(WM_SETTEXT, 0, 0);
    m_pathName.Empty();
    SetWindowTitle();
    m_richView.SetFontDefaults();
    m_richView.SetModify(FALSE);

    // Set Rich text or plain text mode.
    UINT mode = m_isRTF ? TM_RICHTEXT : TM_PLAINTEXT;
    VERIFY(m_richView.SetTextMode(mode) == 0);

    SetStatusIndicators();
}

void CMainFrame::DetermineEncoding(CFile& file)
{
    Encoding encoding = ANSI;
    ULONGLONG fileLength = file.GetLength();

    if (fileLength >= 3)
    {
        try
        {
            file.SeekToBegin();
            DWORD testlen = MIN(1024, (DWORD)fileLength);
            std::vector<byte> buffer(testlen);
            file.Read(&buffer.front(), testlen);

            // look UTF Byte Order Mark (BOM)
            if (buffer[0] == 0xef && buffer[1] == 0xbb && buffer[2] == 0xbf)
                    encoding = UTF8;

            // check for UTF-16 LE with Byte Order Mark (BOM)
            int tests = IS_TEXT_UNICODE_SIGNATURE;
            if (::IsTextUnicode(&buffer.front(), testlen, &tests) == 1)
                encoding = UTF16LE_BOM;
            else
            {
                // check for UTF-16 LE w/o BOM
                tests = IS_TEXT_UNICODE_STATISTICS;
                if (::IsTextUnicode(&buffer.front(), testlen, &tests) == 1)
                {
                    encoding = UTF16LE;
                }
            }
        }
        catch (const CFileException& e)
        {
            CString str = CString("Failed to read from ") + e.GetFileName();
            ::MessageBox(NULL, str, AtoT(e.what()), MB_ICONWARNING);
        }
    }

    file.SeekToBegin();

    SetEncoding(encoding);
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
    case IDM_FILE_NEW_PLAIN:    return OnFileNewPlain();
    case IDM_FILE_NEW_RICH:     return OnFileNewRich();
    case IDM_FILE_OPEN:         return OnFileOpen();
    case IDM_FILE_SAVE:         return OnFileSave();
    case IDM_FILE_SAVEAS:       return OnFileSaveAs();
	case IDM_FILE_PREVIEW:      return OnFilePreview();
    case IDM_FILE_PRINT:        return OnFilePrint();
    case IDM_EDIT_COPY:         return OnEditCopy();
    case IDM_EDIT_PASTE:        return OnEditPaste();
    case IDM_EDIT_CUT:          return OnEditCut();
    case IDM_EDIT_DELETE:       return OnEditDelete();
    case IDM_EDIT_REDO:         return OnEditRedo();
    case IDM_EDIT_UNDO:         return OnEditUndo();
    case IDM_ENC_ANSI:          return OnEncodeANSI();
    case IDM_ENC_UTF8:          return OnEncodeUTF8();
    case IDM_ENC_UTF16:         return OnEncodeUTF16();
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
    UINT length = DragQueryFile(hDropInfo, 0, 0, 0);
    if (length > 0)
    {
        CString fileName;
        DragQueryFile(hDropInfo, 0, fileName.GetBuffer(length), length + 1);
        fileName.ReleaseBuffer();

        if (ReadFile(fileName))
        {
            m_pathName = fileName;
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
    if (m_isRTF)
        // Paste rich text and plain text.
        m_richView.Paste();
    else
        // Paste plain text only.
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

BOOL CMainFrame::OnEncodeANSI()
{
    SetEncoding(ANSI);
    int menuItem = GetMenuItemPos(GetFrameMenu(), _T("Encoding"));
    if (menuItem >= 0)
    {
        CMenu ThemeMenu = GetFrameMenu().GetSubMenu(menuItem);
        ThemeMenu.CheckMenuRadioItem(IDM_ENC_ANSI, IDM_ENC_UTF16, IDM_ENC_ANSI, 0);
    }

    return TRUE;
}

BOOL CMainFrame::OnEncodeUTF8()
{
    SetEncoding(UTF8);
    int menuItem = GetMenuItemPos(GetFrameMenu(), _T("Encoding"));
    if (menuItem >= 0)
    {
        CMenu ThemeMenu = GetFrameMenu().GetSubMenu(menuItem);
        ThemeMenu.CheckMenuRadioItem(IDM_ENC_ANSI, IDM_ENC_UTF16, IDM_ENC_UTF8, 0);
    }

    return TRUE;
}

BOOL CMainFrame::OnEncodeUTF16()
{
    SetEncoding(UTF16LE);
    int menuItem = GetMenuItemPos(GetFrameMenu(), _T("Encoding"));
    if (menuItem >= 0)
    {
        CMenu ThemeMenu = GetFrameMenu().GetSubMenu(menuItem);
        ThemeMenu.CheckMenuRadioItem(IDM_ENC_ANSI, IDM_ENC_UTF16, IDM_ENC_UTF16, 0);
    }

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

BOOL CMainFrame::OnFileNewPlain()
{
    //Check for unsaved text
    SaveModifiedText();
    m_isRTF = false;
    SetEncoding(ANSI);
    ClearContents();
    return TRUE;
}

BOOL CMainFrame::OnFileNewRich()
{
    //Check for unsaved text
    SaveModifiedText();
    m_isRTF = true;
    SetEncoding(ANSI);
    ClearContents();
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

BOOL CMainFrame::OnFilePrint()
{
	try
	{
		m_richView.DoPrint(m_pathName);
	}

	catch (const CWinException&  /*e*/)
	{
		// No default printer
		MessageBox(_T("Unable to display print dialog"), _T("Print Failed"), MB_OK);
	}

	return TRUE;
}

BOOL CMainFrame::OnFileQuickPrint()
{
	m_richView.QuickPrint(m_pathName);
	return TRUE;
}

BOOL CMainFrame::OnFileOpen()
{
    // szFilters is a text string that includes two file name filters:
    // "*.txt" for Plain Text files, "*.rtf" for Rich Text files and "*.*' for "All Files."
    LPCTSTR filters;
    if (m_isRTF)
        filters = _T("Rich Text Files (*.rtf)\0*.rtf\0Plain Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");
    else
        filters = _T("Plain Text Files (*.txt)\0*.txt\0Rich Text Files (*.rtf)\0*.rtf\0All Files (*.*)\0*.*\0");

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
    // "*.txt" for Plain Text Files, "*.rtf" for Rich Text Files, and "*.*' for All Files.
    LPCTSTR filters;
    if (m_isRTF)
        filters = _T("Rich Text Files (*.rtf)\0*.rtf\0Plain Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");
    else
        filters = _T("Plain Text Files (*.txt)\0*.txt\0Rich Text Files (*.rtf)\0*.rtf\0All Files (*.*)\0*.*\0");
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

    // Select the ANSI radio button
    int menuItem = GetMenuItemPos(GetFrameMenu(), _T("Encoding"));
    if (menuItem >= 0)
    {
        CMenu ThemeMenu = GetFrameMenu().GetSubMenu(menuItem);
        ThemeMenu.CheckMenuRadioItem(IDM_ENC_ANSI, IDM_ENC_UTF16, IDM_ENC_ANSI, 0);
    }

    m_richView.SetFocus();
    SetEncoding(ANSI);

    // Retrieve the command line arguments.
    std::vector<CString> args = GetCommandLineArgs();
    // The second argument (if any) contains our file name.
    if (args.size() > 1)
        ReadFile(args[1]);

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

    if (id == IDM_ENC_UTF16)
    {
        // Disable ANSI for Rich text mode.
        UINT state = m_isRTF ? MF_GRAYED : MF_ENABLED;
        GetFrameMenu().EnableMenuItem(id, state);
    }

    if ((id >= IDM_ENC_ANSI) && (id <= IDM_ENC_UTF16))
        OnUpdateRangeOfIDs(IDM_ENC_ANSI, IDM_ENC_UTF16, id);

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
    // Retrieve the current character format.
    CHARFORMAT cf;
    ZeroMemory(&cf, sizeof(cf));
    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_COLOR | CFM_FACE | CFM_EFFECTS;

    if (m_isRTF)
        m_richView.GetSelectionCharFormat(cf);
    else
        m_richView.GetDefaultCharFormat(cf);

    // Display the Choose Font dialog
    CFontDialog dlg(cf, CF_SCREENFONTS | CF_EFFECTS);

    if (dlg.DoModal(*this) == IDOK)
    {

        // Assign the new font to the rich text document.
        cf = dlg.GetCharFormat();
        if (m_isRTF)
        {
            // Change the selected characters in a RTF document.
            m_richView.SetSelectionCharFormat(cf);
        }
        else
        {
            // Change the entire document for plain text.
            m_richView.SetDefaultCharFormat(cf);
        }
    }

    return TRUE;
}

// Turn word wrap on or off.
BOOL CMainFrame::OnOptionsWrap()
{
    m_richView.SetTargetDevice(NULL, m_isWrapped);
    m_isWrapped = !m_isWrapped;
    return TRUE;
}

// Update the radio buttons in the menu.
void CMainFrame::OnUpdateRangeOfIDs(UINT idFirst, UINT idLast, UINT id)
{
    int menuItem = GetMenuItemPos(GetFrameMenu(), _T("Encoding"));
    CMenu radioMenu = GetFrameMenu().GetSubMenu(menuItem);
    UINT enc = m_encoding + IDM_ENC_ANSI;
    if (enc == id)
        radioMenu.CheckMenuRadioItem(idFirst, idLast, id, 0);
}

// Streams the file into the rich edit control.
BOOL CMainFrame::ReadFile(LPCTSTR fileName)
{
    // Ask to save if file was modified
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

        // Use RFT mode if the file has an rtf extension
        CString ext = file.GetFileNameExt();
        ext.MakeLower();
        m_isRTF = (ext == _T("rtf"));

        // set the EDITSTREAM mode
        int stream_mode = m_isRTF? SF_RTF : SF_TEXT;
        // try to determine the file encoding: Note that ANSI and UTF-8 are
        // handled by default, and only UTF-16 LE is accommodated by RichEdit.
        DetermineEncoding(file);
        if (m_encoding == UTF16LE_BOM || m_encoding == UTF16LE)
            stream_mode |= SF_UNICODE;

        // Skip the BOM for UTF16LE encoding
        if (m_encoding == UTF16LE_BOM)
            file.Seek(2, FILE_BEGIN);

        // Skip the BOM for UTF8 encoding
        if ((m_encoding == UTF8) && m_isRTF)
            file.Seek(3, FILE_BEGIN);

        EDITSTREAM es;
        es.dwCookie = reinterpret_cast<DWORD_PTR>(file.GetHandle());
        es.pfnCallback = reinterpret_cast<EDITSTREAMCALLBACK>(MyStreamInCallback);
        m_richView.StreamIn(stream_mode, es);

        //Clear the modified text flag
        m_richView.SetModify(FALSE);

        SetStatusIndicators();
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

void CMainFrame::SetEncoding(Encoding encoding)
{
    m_encoding = encoding;

    switch (m_encoding)
    {
    case ANSI:         SetStatusText(_T("Encoding: ANSI"));            break;
    case UTF8:         SetStatusText(_T("Encoding: UTF-8"));           break;
    case UTF16LE:      SetStatusText(_T("Encoding: UTF-16"));          break;
    case UTF16LE_BOM:  SetStatusText(_T("Encoding: UTF-16 with BOM")); break;
    }

}

void CMainFrame::SetPathName(LPCTSTR filePathName)
{
    m_pathName = filePathName;
}

// Override CFrame<T>::SetStatusIndicators to indicate insert character status.
inline void CMainFrame::SetStatusIndicators()
{
    if (GetStatusBar().IsWindow() && (IsUsingIndicatorStatus()))
    {
        // Calculate the width of the text indicators
        CClientDC statusDC(GetStatusBar());
        statusDC.SelectObject(GetStatusBar().GetFont());
        CString cap = LoadString(IDW_INDICATOR_CAPS);
        CString num = LoadString(IDW_INDICATOR_NUM);
        CString ins = LoadString(IDW_INDICATOR_OVR);
        CString mode = _T("Plain Text mode     ");
        CSize capSize = statusDC.GetTextExtentPoint32(cap, cap.GetLength());
        CSize numSize = statusDC.GetTextExtentPoint32(num, num.GetLength());
        CSize insSize = statusDC.GetTextExtentPoint32(_T("OVR"), ins.GetLength());
        CSize modeSize = statusDC.GetTextExtentPoint32(mode, mode.GetLength());

        BOOL hasGripper = GetStatusBar().GetStyle() & SBARS_SIZEGRIP;
        int cxGripper = hasGripper ? 20 : 0;
        int cxBorder = 8;

        // Adjust for DPI aware
        int defaultDPI = 96;
        int xDPI = statusDC.GetDeviceCaps(LOGPIXELSX);
        cxGripper = MulDiv(cxGripper, xDPI, defaultDPI);
        capSize.cx += cxBorder;
        numSize.cx += cxBorder;
        insSize.cx += cxBorder;

        // Get the coordinates of the window's client area.
        CRect clientRect = GetClientRect();
        int width = clientRect.right;

        // Create 4 panes
        GetStatusBar().SetPartWidth(0, width - (capSize.cx + numSize.cx + insSize.cx + modeSize.cx + cxGripper));
        GetStatusBar().SetPartWidth(1, modeSize.cx);
        GetStatusBar().SetPartWidth(2, capSize.cx);
        GetStatusBar().SetPartWidth(3, numSize.cx);
        GetStatusBar().SetPartWidth(4, insSize.cx);

        CString status0 = m_isRTF ? LoadString(IDW_INDICATOR_RICH) : LoadString(IDW_INDICATOR_PLAIN);
        CString status1 = (::GetKeyState(VK_CAPITAL) & 0x0001) ? cap : CString("");
        CString status2 = (::GetKeyState(VK_NUMLOCK) & 0x0001) ? num : CString("");
        CString status3 = (::GetKeyState(VK_INSERT) & 0x0001) ? LoadString(IDW_INDICATOR_OVR) : LoadString(IDW_INDICATOR_INS);

        // Only update indicators if the text has changed
        if (status0 != m_oldStatus[0])  GetStatusBar().SetPartText(1, status0);
        if (status1 != m_oldStatus[1])  GetStatusBar().SetPartText(2, status1);
        if (status2 != m_oldStatus[2])  GetStatusBar().SetPartText(3, status2);
        if (status3 != m_oldStatus[3])  GetStatusBar().SetPartText(4, status3);

        m_oldStatus[0] = status0;
        m_oldStatus[1] = status1;
        m_oldStatus[2] = status2;
        m_oldStatus[3] = status3;
    }
}

// Adds images to the popup menu items.
void CMainFrame::SetupMenuIcons()
{
    AddMenuIcons(GetToolBarData(), RGB(192, 192, 192), IDW_MENUICONS, 0);
}

// Assigns images and command IDs to the toolbar buttons,
void CMainFrame::SetupToolBar()
{
    // Define the resource IDs for the toolbar
    AddToolBarButton( IDM_FILE_NEW_PLAIN );
    AddToolBarButton( IDM_FILE_NEW_RICH );
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

// Streams from the rich edit control to the specified file.
BOOL CMainFrame::WriteFile(LPCTSTR szFileName)
{
    try
    {
        // Open the file for writing
        CFile file;
        file.Open(szFileName, CREATE_ALWAYS);

        // Use Rich Text mode if the file has an rtf extension
        CString ext = file.GetFileNameExt();
        ext.MakeLower();
        m_isRTF = (ext == _T("rtf"));

        // set the EDITSTREAM mode
        int stream_mode = m_isRTF ? SF_RTF : SF_TEXT;

        if (m_encoding == UTF16LE_BOM || m_encoding == UTF16LE)
            stream_mode |= SF_UNICODE;

        if (m_encoding == UTF8)
            stream_mode |= (CP_UTF8 << 16) | SF_USECODEPAGE;

        // Write the BOM for UTF16LE_BOM encoding if it had one before.
        if (m_encoding == UTF16LE_BOM)
        {
            byte buffer[2] = { 0xff, 0xfe };
            file.Write(buffer, 2);
        }

        // Write the BOM for UTF encoding
        if (m_encoding == UTF8)
        {
            byte buffer[3] = { 0xef, 0xbb, 0xbf };
            file.Write(buffer, 3);
        }

        EDITSTREAM es;
        es.dwCookie = reinterpret_cast<DWORD_PTR>(file.GetHandle());
        es.dwError = 0;
        es.pfnCallback = reinterpret_cast<EDITSTREAMCALLBACK>(MyStreamOutCallback);
        m_richView.StreamOut(stream_mode, es);

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

