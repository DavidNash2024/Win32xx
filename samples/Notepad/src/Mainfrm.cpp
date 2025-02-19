/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"

#ifndef INVALID_FILE_ATTRIBUTES
  #define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif

#ifndef SF_USECODEPAGE
  #define SF_USECODEPAGE    0x0020
#endif

// Encoding IDs
const int ANSI = 0;            // Default for plain text
const int UTF8 = 1;            // Default for rich text
const int UTF16LE = 2;


///////////////////////////////////
// CMainFrame function definitions
//

// Constructor.
CMainFrame::CMainFrame() : m_preview(m_richView),
                           m_encoding(ANSI), m_isToolbarShown(true),
                           m_isWrapped(false), m_isRTF(false), m_oldFocus(nullptr)

{
}

// Adjusts the specified value for the current DPI.
int CMainFrame::AdjustForDPI(int value) const
{
    CClientDC statusDC(GetStatusBar());
    statusDC.SelectObject(GetStatusBar().GetFont());

    // Perform the DPI adjustment calculation.
    const int defaultDPI = 96;
    int xDPI = statusDC.GetDeviceCaps(LOGPIXELSX);
    value = MulDiv(value, xDPI, defaultDPI);

    return value;
}

// Clears the contents of the richedit view.
void CMainFrame::ClearContents()
{
    m_richView.SetWindowText(nullptr);
    m_pathName.Empty();
    SetWindowTitle();
    m_richView.SetFontDefaults();
    m_richView.SetModify(FALSE);

    // Set Rich text or plain text mode.
    UINT mode = m_isRTF ? TM_RICHTEXT : TM_PLAINTEXT;
    VERIFY(m_richView.SetTextMode(mode) == 0);

    SetStatusIndicators();
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    // Set m_richView as the view window for the frame.
    SetView(m_richView);

    // Set the registry key name, and load the initial window position.
    // Use a registry key name like "CompanyName\\Application".
    LoadRegistrySettings(L"Win32++\\Notepad Sample");

    // Load the settings from the registry with 5 MRU entries.
    LoadRegistryMRUSettings(5);

    return CFrame::Create(parent);
}

// Determines the encoding of the specified file.
void CMainFrame::DetermineEncoding(CFile& file)
{
    UINT encoding = ANSI;
    ULONGLONG fileLength = file.GetLength();

    if (fileLength >= 3)
    {
        try
        {
            file.SeekToBegin();
            DWORD testlen = std::min(1024, static_cast<int>(fileLength));
            std::vector<byte> buffer(testlen);
            file.Read(buffer.data(), testlen);

            // look UTF Byte Order Mark (BOM)
            if (buffer[0] == 0xef && buffer[1] == 0xbb && buffer[2] == 0xbf)
                    encoding = UTF8;

            // check for UTF-16 LE with Byte Order Mark (BOM)
            int tests = IS_TEXT_UNICODE_SIGNATURE;
            int textLength = static_cast<int>(testlen);
            if (::IsTextUnicode(buffer.data(), textLength, &tests) != 0)
                encoding = UTF16LE;
        }
        catch (const CFileException& e)
        {
            CString str = CString("Failed to read from ") + e.GetFileName();
            ::MessageBox(nullptr, str, AtoW(e.what()), MB_ICONWARNING);
        }
    }

    file.SeekToBegin();

    SetEncoding(encoding);
}

// Retrieves the width of the part required to contain the specified text.
int CMainFrame::GetTextPartWidth(LPCWSTR text) const
{
    CClientDC statusDC(GetStatusBar());
    statusDC.SelectObject(GetStatusBar().GetFont());
    CSize textSize = statusDC.GetTextExtentPoint32(text);
    int width = textSize.cx;
    const int border = 8;
    width += border;

    return width;
}

// The stream in callback function. Reads from the file.
DWORD CALLBACK CMainFrame::MyStreamInCallback(DWORD cookie, LPBYTE pBuffer, LONG cb, LONG *pcb)
{
    // Required for StreamIn
    if (!cb)
        return (1);

    HANDLE file = reinterpret_cast<HANDLE>(static_cast<DWORD_PTR>(cookie));
    LPDWORD bytesRead = reinterpret_cast<LPDWORD>(pcb);
    *bytesRead = 0;
    DWORD bytesToRead = static_cast<DWORD>(cb);
    if (!::ReadFile(file, pBuffer, bytesToRead, bytesRead, nullptr))
        ::MessageBox(nullptr, L"ReadFile Failed", L"", MB_OK);

    return 0;
}

// The stream out callback function. Writes to the file.
DWORD CALLBACK CMainFrame::MyStreamOutCallback(DWORD cookie, LPBYTE pBuffer, LONG cb, LONG *pcb)
{
    // Required for StreamOut
    if (!cb)
        return (1);

    HANDLE file = reinterpret_cast<HANDLE>(static_cast<DWORD_PTR>(cookie));
    LPDWORD bytesWritten = reinterpret_cast<LPDWORD>(pcb);
    *bytesWritten = 0;
    DWORD bytesToRead = static_cast<DWORD>(cb);
    if (!::WriteFile(file, pBuffer, bytesToRead, bytesWritten, nullptr))
        ::MessageBox(nullptr, L"WriteFile Failed", L"", MB_OK);
    return 0;
}

// Called when the window is closed.
void CMainFrame::OnClose()
{
    // Close the preview
    if (GetView() == m_preview)
        OnPreviewClose();

    //Check for unsaved text
    SaveModifiedText();

    // Call the base function
    CFrame::OnClose();
}


// Handle commands from the menu and toolbar.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch (id)
    {
    case IDM_FILE_NEW_PLAIN:    return OnFileNewPlain();
    case IDM_FILE_NEW_RICH:     return OnFileNewRich();
    case IDM_FILE_OPEN:         return OnFileOpen();
    case IDM_FILE_SAVE:         return OnFileSave();
    case IDM_FILE_SAVEAS:       return OnFileSaveAs();
    case IDM_FILE_PREVIEW:      return OnFilePreview();
    case IDM_FILE_PRINTNOW:     return OnFilePrintNow();
    case IDM_FILE_PRINT:        return OnFilePrint();
    case IDM_FILE_PRINTSETUP:   return OnFilePrintSetup();
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

// OnCreate controls the way the frame is created.
int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
    // OnCreate controls the way the frame is created.
    // Overriding CFrame::OnCreate is optional.

    // A menu is added if the IDW_MAIN menu resource is defined.
    // Frames have all options enabled by default.
    // Use the following functions to disable options.

    // UseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar.
    // UseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar.
    // UseOwnerDrawnMenu(FALSE);     // Don't use owner draw for popup menu items.
    // UseReBar(FALSE);              // Don't use a ReBar.
    // UseStatusBar(FALSE);          // Don't use a StatusBar.
    // UseThemes(FALSE);             // Don't use themes.
    // UseToolBar(FALSE);            // Don't use a ToolBar.

    // call the base class function
    return CFrame::OnCreate(cs);
}

// Called when the effective dots per inch (dpi) for a window has changed.
// This occurs when:
//  - The window is moved to a new monitor that has a different DPI.
//  - The DPI of the monitor hosting the window changes.
LRESULT CMainFrame::OnDpiChanged(UINT msg, WPARAM wparam, LPARAM lparam)
{
    CFrame::OnDpiChanged(msg, wparam, lparam);
    UpdateToolbar();
    RecalcLayout();
    return 0;
}

// Called in response to the EN_DROPFILES notification.
void CMainFrame::OnDropFiles(HDROP dropInfo)
{
    UINT length = ::DragQueryFile(dropInfo, 0, nullptr, 0);
    int bufferLength = static_cast<int>(length);
    if (length > 0)
    {
        CString fileName;
        ::DragQueryFile(dropInfo, 0, fileName.GetBuffer(bufferLength), length + 1);
        fileName.ReleaseBuffer();

        if (ReadFile(fileName))
        {
            m_pathName = fileName;
            SetWindowTitle();
            AddMRUEntry(fileName);
        }
    }

    ::DragFinish(dropInfo);
}

// Delete (cut) the current selection, if any.
BOOL CMainFrame::OnEditCut()
{
    m_richView.Cut();
    return TRUE;
}

// Copy the current selection to the clipboard.
BOOL CMainFrame::OnEditCopy()
{
    m_richView.Copy();
    return TRUE;
}

// Paste plain text or rich text to the document.
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

// Clears the contents of the document.
BOOL CMainFrame::OnEditDelete()
{
    m_richView.Clear();
    return TRUE;
}

// Redoes the next action in the redo queue.
BOOL CMainFrame::OnEditRedo()
{
    m_richView.Redo();
    return TRUE;
}

// Undoes the last operation in the undo queue.
BOOL CMainFrame::OnEditUndo()
{
    m_richView.Undo();
    return TRUE;
}

// Select ANSI encoding.
BOOL CMainFrame::OnEncodeANSI()
{
    SetEncoding(ANSI);
    int menuItem = GetFrameMenu().FindMenuItem(L"&Encoding");
    if (menuItem >= 0)
    {
        CMenu ThemeMenu = GetFrameMenu().GetSubMenu(menuItem);
        ThemeMenu.CheckMenuRadioItem(IDM_ENC_ANSI, IDM_ENC_UTF16, IDM_ENC_ANSI, MF_BYCOMMAND);
    }

    return TRUE;
}

// Select UTF8 encoding.
BOOL CMainFrame::OnEncodeUTF8()
{
    SetEncoding(UTF8);
    int menuItem = GetFrameMenu().FindMenuItem(L"&Encoding");
    if (menuItem >= 0)
    {
        CMenu ThemeMenu = GetFrameMenu().GetSubMenu(menuItem);
        ThemeMenu.CheckMenuRadioItem(IDM_ENC_ANSI, IDM_ENC_UTF16, IDM_ENC_UTF8, MF_BYCOMMAND);
    }

    return TRUE;
}

// Select UTF16 encoding.
BOOL CMainFrame::OnEncodeUTF16()
{
    SetEncoding(UTF16LE);
    int menuItem = GetFrameMenu().FindMenuItem(L"&Encoding");
    if (menuItem >= 0)
    {
        CMenu ThemeMenu = GetFrameMenu().GetSubMenu(menuItem);
        ThemeMenu.CheckMenuRadioItem(IDM_ENC_ANSI, IDM_ENC_UTF16, IDM_ENC_UTF16, MF_BYCOMMAND);
    }

    return TRUE;
}

// Issue a close request to the frame to end the application.
BOOL CMainFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Respond to a MRU selection.
BOOL CMainFrame::OnFileMRU(WPARAM wparam)
{
    UINT mruIndex = static_cast<UINT>(LOWORD(wparam)) - IDW_FILE_MRU_FILE1;
    CString mruText = GetMRUEntry(mruIndex);

    if (ReadFile(mruText))
        m_pathName = mruText;
    else
        RemoveMRUEntry(mruText);

    SetWindowTitle();

    return TRUE;
}

// Create a blank plain text document.
BOOL CMainFrame::OnFileNewPlain()
{
    //Check for unsaved text
    SaveModifiedText();
    m_isRTF = false;
    SetEncoding(ANSI);
    ClearContents();
    return TRUE;
}

// Create a blank rich text document.
BOOL CMainFrame::OnFileNewRich()
{
    //Check for unsaved text
    SaveModifiedText();
    m_isRTF = true;
    SetEncoding(ANSI);
    ClearContents();
    return TRUE;
}

// Preview the print job before sending it to a printer.
BOOL CMainFrame::OnFilePreview()
{
    // Verify a print preview is possible
    try
    {
        m_isToolbarShown = GetToolBar().IsWindow() && GetToolBar().IsWindowVisible();

        // Retrieve the device context of the default or currently chosen printer.
        CPrintDialog printDlg;
        CDC printerDC = printDlg.GetPrinterDC();

        // Create the preview window if required
        if (!m_preview.IsWindow())
            m_preview.Create(*this);

        // Set the preview's owner for notification messages, and number of pages.
        int maxPage = m_richView.CollatePages(printerDC);
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
        MessageBox(e.GetText(), L"Print Preview Failed", MB_ICONWARNING);
        SetView(m_richView);
        ShowMenu(GetFrameMenu() != nullptr);
        ShowToolBar(m_isToolbarShown);
    }

    return TRUE;
}

// Select the printer and print the document.
BOOL CMainFrame::OnFilePrint()
{
    try
    {
        m_richView.DoPrint(m_pathName);
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), L"Print Failed", MB_ICONWARNING);
    }

    return TRUE;
}

// Select the printer for use by the application.
BOOL CMainFrame::OnFilePrintSetup()
{
    // Display the print setup dialog.
    CPrintDialog printDlg(PD_PRINTSETUP);
    try
    {
        // Display the print dialog
        if (printDlg.DoModal(*this) == IDOK)
        {
            CString status = L"Printer: " + printDlg.GetDeviceName();
            SetStatusText(status);
        }
    }

    catch (const CWinException& /* e */)
    {
        // No default printer
        MessageBox(L"Unable to display print dialog", L"Print Failed", MB_OK);
    }

    return TRUE;
}

// Print the document without selecting the printer.
BOOL CMainFrame::OnFilePrintNow()
{
    try
    {
        m_richView.QuickPrint(m_pathName);
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), L"Print Failed", MB_ICONWARNING);
    }
    return TRUE;
}

// Display the file choose dialog and load text from a file.
BOOL CMainFrame::OnFileOpen()
{
    // szFilters is a text string that includes two file name filters:
    // "*.txt" for Plain Text files, "*.rtf" for Rich Text files and "*.*' for "All Files."
    LPCWSTR filters;
    if (m_isRTF)
        filters = L"Rich Text Files (*.rtf)\0*.rtf\0Plain Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    else
        filters = L"Plain Text Files (*.txt)\0*.txt\0Rich Text Files (*.rtf)\0*.rtf\0All Files (*.*)\0*.*\0";

    CFileDialog fileDlg(TRUE, L"txt", nullptr, OFN_FILEMUSTEXIST, filters);

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

// Save the document text to the current file.
BOOL CMainFrame::OnFileSave()
{
    if (m_pathName.IsEmpty())
        OnFileSaveAs();
    else
    {
        DWORD dwAttrib = GetFileAttributes(m_pathName);
        if (dwAttrib != INVALID_FILE_ATTRIBUTES)
        {
            CString str = "This file already exists.\nDo you want to replace it?";
            if (IDYES == MessageBox(str, L"Confirm Save", MB_ICONWARNING | MB_OKCANCEL))
                WriteFile(m_pathName);
        }
        else
            WriteFile(m_pathName);
    }

    return TRUE;
}

// Display the file choose dialog and save text to the file.
BOOL CMainFrame::OnFileSaveAs()
{
    // szFilter is a text string that includes two file name filters:
    // "*.txt" for Plain Text Files, "*.rtf" for Rich Text Files, and "*.*' for All Files.
    LPCWSTR filters;
    if (m_isRTF)
        filters = L"Rich Text Files (*.rtf)\0*.rtf\0Plain Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    else
        filters = L"Plain Text Files (*.txt)\0*.txt\0Rich Text Files (*.rtf)\0*.rtf\0All Files (*.*)\0*.*\0";
    CFileDialog fileDlg(FALSE, L"txt", nullptr, OFN_OVERWRITEPROMPT, filters);

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

// Called after OnCreate.
void CMainFrame::OnInitialUpdate()
{
    DragAcceptFiles(TRUE);
    SetWindowTitle();
    m_richView.SetFocus();
    SetEncoding(ANSI);

    // Retrieve the command line arguments.
    std::vector<CString> args = GetCommandLineArgs();
    // The second argument (if any) contains our file name.
    if (args.size() > 1)
        ReadFile(args[1]);
}

// Updates menu items before they are displayed.
void CMainFrame::OnMenuUpdate(UINT id)
{
    UINT enabled;
    UINT checked;

    switch (id)
    {
    case IDM_OPTIONS_WRAP:
    {
        checked = m_isWrapped ? MF_CHECKED : MF_GRAYED;
        GetFrameMenu().CheckMenuItem(id, checked);
        break;
    }
    case IDM_ENC_UTF8:
    {
        // Only enable UTF-8 for plain text mode.
        enabled = m_isRTF ? MF_GRAYED : MF_ENABLED;
        GetFrameMenu().EnableMenuItem(id, enabled);
        break;
    }
    case IDM_ENC_UTF16:
    {
        // Only enable UTF-16 for plain text mode.
        enabled = m_isRTF? MF_GRAYED : MF_ENABLED;
        GetFrameMenu().EnableMenuItem(id, enabled);
        break;
    }
    case IDM_FILE_SAVE:
    {
        enabled = m_richView.GetModify() ? MF_ENABLED : MF_GRAYED;
        GetFrameMenu().EnableMenuItem(id, enabled);
        break;
    }
    case IDM_EDIT_COPY:
    case IDM_EDIT_CUT:
    case IDM_EDIT_DELETE:
    {
        CHARRANGE range;
        m_richView.GetSel(range);
        enabled = (range.cpMin != range.cpMax)? MF_ENABLED : MF_GRAYED;
        GetFrameMenu().EnableMenuItem(id, enabled);
        break;
    }
    case IDM_EDIT_PASTE:
    {
        enabled = m_richView.CanPaste(CF_TEXT)? MF_ENABLED : MF_GRAYED;
        GetFrameMenu().EnableMenuItem(id, enabled);
        break;
    }
    case IDM_EDIT_REDO:
    {
        enabled = m_richView.CanRedo()? MF_ENABLED : MF_GRAYED;
        GetFrameMenu().EnableMenuItem(id, enabled);
        break;
    }
    case IDM_EDIT_UNDO:
    {
        enabled = m_richView.CanUndo()? MF_ENABLED : MF_GRAYED;
        GetFrameMenu().EnableMenuItem(id, enabled);
        break;
    }
    }

    if ((id >= IDM_ENC_ANSI) && (id <= IDM_ENC_UTF16))
        OnUpdateRangeOfIDs(IDM_ENC_ANSI, IDM_ENC_UTF16, id);

    CFrame::OnMenuUpdate(id);
}

// Respond to notification messages (WM_NOTIFY) from child windows.
LRESULT CMainFrame::OnNotify(WPARAM wparam, LPARAM lparam)
{
    LPNMHDR pHeader = reinterpret_cast<LPNMHDR>(lparam);
    switch (pHeader->code)
    {
    case EN_DROPFILES:
    {
        ENDROPFILES* enDrop = reinterpret_cast<ENDROPFILES*>(lparam);
        HDROP dropInfo = reinterpret_cast<HDROP>(enDrop->hDrop);
        OnDropFiles(dropInfo);
    }
    return TRUE;
    }

    return CFrame::OnNotify(wparam, lparam);
}

// Select a font for the document.
BOOL CMainFrame::OnOptionsFont()
{
    // Retrieve the current character format.
    CHARFORMAT cf{};
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
    m_richView.SetTargetDevice(nullptr, m_isWrapped);
    m_isWrapped = !m_isWrapped;
    return TRUE;
}

// Called when the Print Preview's "Close" button is pressed.
LRESULT CMainFrame::OnPreviewClose()
{
    // Swap the view
    SetView(m_richView);

    // Show the menu and toolbar
    ShowMenu(GetFrameMenu() != nullptr);
    ShowToolBar(m_isToolbarShown);
    UpdateSettings();

    // Restore focus to the window focused before DoPrintPreview was called.
    RestoreFocus();

    return 0;
}

// Called when the Print Preview's "Print Now" button is pressed
LRESULT CMainFrame::OnPreviewPrint()
{
    try
    {
        m_richView.QuickPrint(m_pathName);
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), L"Print Failed", MB_ICONWARNING);
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
            CString status = L"Printer: " + printDlg.GetDeviceName();
            SetStatusText(status);
        }

        // Initiate the print preview.
        CDC printerDC = printDlg.GetPrinterDC();
        int maxPage = m_richView.CollatePages(printerDC);
        m_preview.DoPrintPreview(*this, maxPage);
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);
    }

    return 0;
}

// Update the radio buttons in the menu.
BOOL CMainFrame::OnUpdateRangeOfIDs(UINT idFirst, UINT idLast, UINT id)
{
    int menuItem = GetFrameMenu().FindMenuItem(L"&Encoding");
    CMenu radioMenu = GetFrameMenu().GetSubMenu(menuItem);
    UINT enc = m_encoding + IDM_ENC_ANSI;
    if (enc == id)
        radioMenu.CheckMenuRadioItem(idFirst, idLast, id, MF_BYCOMMAND);

    return TRUE;
}

// Streams the file into the rich edit control.
BOOL CMainFrame::ReadFile(LPCWSTR fileName)
{
    try
    {
        // Open the file for reading
        CFile file;
        file.Open(fileName, OPEN_EXISTING | CFile::modeRead);

        // Restrict the file size to something a richedit control can handle.
        ULONGLONG fileLength = file.GetLength();
        if (fileLength > 100000000)
        {
            throw CFileException(fileName, L"File is too large!");
        }

        // Use RFT mode if the file has an rtf extension
        CString ext = file.GetFileNameExt();
        ext.MakeLower();
        if (ext == L"rtf")
            OnFileNewRich();
        else
            OnFileNewPlain();

        // set the EDITSTREAM mode
        int stream_mode = m_isRTF? SF_RTF : SF_TEXT;
        // try to determine the file encoding: Note that ANSI and UTF-8 are
        // handled by default, and only UTF-16 LE is accommodated by RichEdit.
        DetermineEncoding(file);
        if (m_encoding == UTF16LE)
            stream_mode |= SF_UNICODE;

        // Skip the BOM for UTF16LE encoding
        if (m_encoding == UTF16LE)
            file.Seek(2, FILE_BEGIN);

        // Skip the BOM for UTF8 encoding
        if ((m_encoding == UTF8) && m_isRTF)
            file.Seek(3, FILE_BEGIN);

        EDITSTREAM es;
        es.dwCookie = reinterpret_cast<DWORD_PTR>(file.GetHandle());
        es.pfnCallback = reinterpret_cast<EDITSTREAMCALLBACK>(
            reinterpret_cast<void*>(MyStreamInCallback));
        m_richView.StreamIn(stream_mode, es);

        //Clear the modified text flag
        m_richView.SetModify(FALSE);

        SetStatusIndicators();
    }

    catch (const CFileException& e)
    {
        CString str = L"Failed to load:  ";
        str += e.GetFilePath();
        str += "\n";
        str += e.GetText();
        ::MessageBox(nullptr, str, AtoW(e.what()), MB_ICONWARNING);
        return FALSE;
    }

    return TRUE;
}

// Provides an opportunity to save the document before it is closed.
void CMainFrame::SaveModifiedText()
{
    // Check for unsaved text
    if (m_richView.GetModify())
        if (::MessageBox(nullptr, L"Save changes to this document",
            L"Notepad", MB_YESNO | MB_ICONWARNING) == IDYES)
        {
            OnFileSave();
        }
}

// Set the encoding type.
void CMainFrame::SetEncoding(int encoding)
{
    m_encoding = encoding;

    switch (m_encoding)
    {
    case ANSI:         SetStatusText(L"Encoding: ANSI");            break;
    case UTF8:         SetStatusText(L"Encoding: UTF-8");           break;
    case UTF16LE:      SetStatusText(L"Encoding: UTF-16");          break;
    }
}

// Saves the documents full path name.
void CMainFrame::SetPathName(LPCWSTR filePathName)
{
    m_pathName = filePathName;
}

// Updates the status indicators.
void CMainFrame::SetStatusIndicators()
{
    if (GetStatusBar().IsWindow())
    {
        // Calculate the width of the text indicators
        CClientDC statusDC(GetStatusBar());
        statusDC.SelectObject(GetStatusBar().GetFont());
        CString cap = LoadString(IDW_INDICATOR_CAPS);
        CString num = LoadString(IDW_INDICATOR_NUM);
        CString ovr = LoadString(IDW_INDICATOR_OVR);
        CString ins = LoadString(IDW_INDICATOR_INS);
        CString scrl = LoadString(IDW_INDICATOR_SCRL);
        CString rich = LoadString(IDW_INDICATOR_RICH);
        CString plain =  LoadString(IDW_INDICATOR_PLAIN);
        CString empty;

        m_mode = m_isRTF ? rich : plain;
        m_cap  = (::GetKeyState(VK_CAPITAL) & 0x0001) ? cap : empty;
        m_num  = (::GetKeyState(VK_NUMLOCK) & 0x0001) ? num : empty;
        m_ovr  = (::GetKeyState(VK_INSERT) & 0x0001) ? ovr : ins;

        // Update the indicators.
        // Need member variables for owner drawn text to keep them in scope.
        GetStatusBar().SetPartText(1, m_mode, SBT_OWNERDRAW);
        GetStatusBar().SetPartText(2, m_cap, SBT_OWNERDRAW);
        GetStatusBar().SetPartText(3, m_num, SBT_OWNERDRAW);
        GetStatusBar().SetPartText(4, m_ovr, SBT_OWNERDRAW);
    }
}

// Reposition the statusbar parts. It's called when the statusbar is resized.
void CMainFrame::SetStatusParts()
{
    // Calculate the width of the text indicators
    CClientDC statusDC(GetStatusBar());
    statusDC.SelectObject(GetStatusBar().GetFont());

    // Fill a vector with the status bar part widths.
    std::vector<int> partWidths;
    partWidths.push_back(GetTextPartWidth(LoadString(IDW_INDICATOR_PLAIN)));
    partWidths.push_back(GetTextPartWidth(LoadString(IDW_INDICATOR_CAPS)));
    partWidths.push_back(GetTextPartWidth(LoadString(IDW_INDICATOR_NUM)));
    partWidths.push_back(GetTextPartWidth(LoadString(IDW_INDICATOR_OVR)));

    int sumWidths = 0;
    for (int i : partWidths)
    {
        sumWidths += i;
    }

    const int gripWidth = 20;
    sumWidths += AdjustForDPI(gripWidth);

    // Insert the width for the first status bar part into the vector.
    CRect clientRect = GetClientRect();
    const LONG minWidth = 300;
    int width = std::max(minWidth, clientRect.right);
    auto begin = partWidths.begin();
    partWidths.insert(begin, width - sumWidths);

    // Create or resize the status bar parts.
    int part = 0;
    for (int i : partWidths)
    {
        GetStatusBar().SetPartWidth(part++, i);
    }
}

// Specifies the images for menu item IDs matching the toolbar data.
void CMainFrame::SetupMenuIcons()
{
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        SetMenuIcons(data, RGB(192, 192, 192), IDW_MAIN);
    else
        SetMenuIcons(data, RGB(192, 192, 192), IDW_MENUICONS);
}

// Assigns images and command IDs to the toolbar buttons,
void CMainFrame::SetupToolBar()
{
    // Define the resource IDs for the toolbar
    AddToolBarButton(IDM_FILE_NEW_PLAIN);
    AddToolBarButton(IDM_FILE_NEW_RICH);
    AddToolBarButton(IDM_FILE_OPEN);
    AddToolBarButton(IDM_FILE_SAVE);
    AddToolBarButton(IDM_FILE_SAVEAS);
    AddToolBarButton(0);                // Separator
    AddToolBarButton(IDM_EDIT_CUT);
    AddToolBarButton(IDM_EDIT_COPY);
    AddToolBarButton(IDM_EDIT_PASTE);
    AddToolBarButton(0);                // Separator
    AddToolBarButton(IDM_FILE_PRINT);
    AddToolBarButton(0);                // Separator
    AddToolBarButton(IDM_HELP_ABOUT);
}

// Sets the frame's title.
void CMainFrame::SetWindowTitle()
{
    CString title;

    if (m_pathName.IsEmpty())
        title = L"Notepad";
    else
        title = m_pathName + L" - Notepad";

    SetWindowText(title);
}

// Called by CTextApp::OnIdle to update toolbar buttons
void CMainFrame::UpdateToolbar()
{
    CHARRANGE range;
    m_richView.GetSel(range);
    BOOL isSelected = (range.cpMin != range.cpMax);
    BOOL canPaste = m_richView.CanPaste(CF_TEXT);
    BOOL isDirty = m_richView.GetModify();

    GetToolBar().EnableButton(IDM_FILE_SAVE, isDirty);
    GetToolBar().EnableButton(IDM_EDIT_COPY, isSelected);
    GetToolBar().EnableButton(IDM_EDIT_CUT, isSelected);
    GetToolBar().EnableButton(IDM_EDIT_PASTE, canPaste);
}

// Process the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case UWM_PREVIEWCLOSE:      return OnPreviewClose();
        case UWM_PREVIEWPRINT:      return OnPreviewPrint();
        case UWM_PREVIEWSETUP:      return OnPreviewSetup();
        }

        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    }

    return 0;
}

// Streams from the rich edit control to the specified file.
BOOL CMainFrame::WriteFile(LPCWSTR szFileName)
{
    try
    {
        // Open the file for writing
        CFile file;
        file.Open(szFileName, CREATE_ALWAYS);

        // Use Rich Text mode if the file has an rtf extension
        CString ext = file.GetFileNameExt();
        ext.MakeLower();
        m_isRTF = (ext == L"rtf");

        // set the EDITSTREAM mode
        int stream_mode = m_isRTF ? SF_RTF : SF_TEXT;

        if (m_encoding == UTF16LE)
            stream_mode |= SF_UNICODE;

        if (m_encoding == UTF8)
            stream_mode |= (CP_UTF8 << 16) | SF_USECODEPAGE;

        // Write the BOM for UTF16LE_BOM encoding.
        if (m_encoding == UTF16LE)
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
        es.pfnCallback = reinterpret_cast<EDITSTREAMCALLBACK>(
            reinterpret_cast<void*>(MyStreamOutCallback));
        m_richView.StreamOut(stream_mode, es);

        //Clear the modified text flag
        m_richView.SetModify(FALSE);
    }

    catch (const CFileException&)
    {
        CString str = L"Failed to write:  ";
        str += szFileName;
        ::MessageBox(nullptr, str, L"Warning", MB_ICONWARNING);
        return FALSE;
    }

    return TRUE;
}

