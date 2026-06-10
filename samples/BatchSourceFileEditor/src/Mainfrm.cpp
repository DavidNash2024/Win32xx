/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"

///////////////////////////////////
// CMainFrame function definitions.
//

// Constructor for CMainFrame.
CMainFrame::CMainFrame() : m_preview(m_view), m_isToolbarShown(true)
{
    // Set m_view as the view window of the frame.
    SetView(m_view);

    // Set the registry key name, and load the initial window position.
    // Use a registry key name like "CompanyName\\Application".
    LoadRegistrySettings(L"Win32++\\Batch Editor");

    m_folder = GetStartFolder();
}

// Destructor for CMainFrame.
CMainFrame::~CMainFrame()
{
}

// Returns true if the file's extension is cpp, h, or rc.
bool CMainFrame::IsSourceFile(const CString& fileName) const
{
    int dot = fileName.ReverseFind(L'.');
    CString ext = fileName.Mid(dot + 1, lstrlen(fileName));

    return (ext == L"cpp" || ext == L"h" || ext == L"rc" );
}

// Searches source files the specified folder and any sub folder.
// Checks each source file for lines ending with training spaces.
void CMainFrame::CheckForEndSpace(const CString& folder)
{
    SetStatusText(L"Searching  " + folder);
    CFileFind ff;
    if (ff.FindFirstFile(folder + L"\\*.*"))
    {
        do
        {
            CString fileName = ff.GetFilePath();
            if (ff.IsDirectory() && !ff.IsDots())
            {
                CheckForEndSpace(ff.GetFilePath());
            }
            if (IsSourceFile(fileName))
            {
                // Do something with each file found
                if (HasEndSpace(fileName))
                {
                    m_view.AppendSmallText(fileName + L"\n");
                    m_filesWithEndSpace.push_back(fileName);
                }
            }
        } while (ff.FindNextFile());
    }
}

// Searches source files the specified folder and any sub folder.
// Checks each source file for non-ascii characters.
void CMainFrame::CheckForNonAscii(const CString& folder)
{
    SetStatusText(L"Searching  " + folder);
    CFileFind ff;
    if (ff.FindFirstFile(folder + L"\\*.*"))
    {
        do
        {
            CString fileName = ff.GetFilePath();
            if (ff.IsDirectory() && !ff.IsDots())
            {
                // Recursively search subfolders.
                CheckForNonAscii(ff.GetFilePath());
            }
            if (IsSourceFile(fileName))
            {
                // Do something with each file found.
                if (HasNonAscii(fileName))
                {
                    m_view.AppendSmallText(fileName + L"\n");
                    m_filesWithNonAscii.push_back(fileName);
                }
            }
        } while (ff.FindNextFile());
    }
}

// Searches source files the specified folder and any sub folder.
// Checks each source file for lines ending with training spaces.
void CMainFrame::CheckForTabs(const CString& folder)
{
    GetStatusBar().SetPartText(0, L"Searching  " + folder);
    CFileFind ff;
    if (ff.FindFirstFile(folder + L"\\*.*"))
    {
        do
        {
            CString fileName = ff.GetFilePath();
            if (ff.IsDirectory() && !ff.IsDots())
            {
                CheckForTabs(ff.GetFilePath());
            }
            if (IsSourceFile(fileName))
            {
                // Do something with each file found.
                if (HasTabs(fileName))
                {
                    m_view.AppendSmallText(fileName + L"\n");
                    m_filesWithTabs.push_back(fileName);
                }
            }
        } while (ff.FindNextFile());
    }
}

bool CMainFrame::PickFolder(const CString& startFolder)
{
    bool result = false;
    IFileDialog* pfd;
    if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))))
    {
        LPWSTR path;
        DWORD dwOptions;
        if (SUCCEEDED(pfd->GetOptions(&dwOptions)))
        {
            pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
        }

        PIDLIST_ABSOLUTE pidl;
        HRESULT hresult = ::SHParseDisplayName(startFolder, nullptr, &pidl, SFGAO_FOLDER, 0);
        if (SUCCEEDED(hresult))
        {
            IShellItem* psi;
            hresult = ::SHCreateShellItem(nullptr, nullptr, pidl, &psi);
            if (SUCCEEDED(hresult))
            {
                pfd->SetFolder(psi);
            }
            ILFree(pidl);
            psi->Release();
        }

        if (S_OK == pfd->Show(nullptr))
        {
            IShellItem* psi;
            if (SUCCEEDED(pfd->GetResult(&psi)))
            {
                if (SUCCEEDED(psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &path)))
                {
                    m_folder = path;
                    result = true;
                }

                CoTaskMemFree(path);
                psi->Release();
            }
        }
        pfd->Release();
    }

    return result;
}

CString CMainFrame::GetStartFolder()
{
    CString str = GetCommandLineArgs()[0];

    int sep = str.ReverseFind(L'\\');
    if (sep > 0)
        str = str.Left(sep);

    return str;
}

// Returns true if the specified file has lines with trailing spaces.
bool CMainFrame::HasEndSpace(const CString& fileName)
{
    std::ifstream file(WtoA(fileName.c_str()));
    char c1 = 0;
    char c2 = 0;
    while (file.get(c2)) {
        if (c2 == '\n' && c1 == ' ')
            return true;
        c1 = c2;
    }

    return false;
}

// Returns true if the specified file contains tabs.
bool CMainFrame::HasTabs(const CString& fileName)
{
    std::ifstream file(WtoA(fileName.c_str()));
    char c;
    while (file.get(c)) {
        if (c == '\t')  // Check for tab character.
            return true;
    }

    return false;
}

bool CMainFrame::HasNonAscii(const CString& fileName)
{
    std::ifstream file(WtoA(fileName.c_str()));
    char c;
    while (file.get(c)) {
        if (c < 0)  // Check for non-ascii.
            return true;
    }


    return false;
}

// OnCommand responds to menu and and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch(id)
    {
    case IDM_FILE_NEW:        return OnFileNew();
    case IDM_FILE_CHOOSE:     return OnChooseFolder();
    case IDM_FILE_CHECK:      return OnCheckFiles();
    case IDM_FILE_FIX:        return OnFixFiles();
    case IDM_FILE_PREVIEW:    return OnFilePreview();
    case IDM_FILE_PRINT:      return OnFilePrint();
    case IDM_FILE_EXIT:       return OnFileExit();
    case IDW_VIEW_STATUSBAR:  return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:    return OnViewToolBar();
    case IDM_HELP_ABOUT:      return OnHelp();

    default: break;
    }

    return FALSE;
}

// OnCreate controls the way the frame is created.
// Overriding CFrame::OnCreate is optional.
int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
    // A menu is added if the IDW_MAIN menu resource is defined.
    // Frames have all options enabled by default.
    // Use the following functions to disable options.

    // UseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar
    // UseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar
    // UseReBar(FALSE);              // Don't use a ReBar
    // UseStatusBar(FALSE);          // Don't use a StatusBar
    // UseThemes(FALSE);             // Don't use themes
    // UseToolBar(FALSE);            // Don't use a ToolBar

    // Call the base class function.
    return CFrame::OnCreate(cs);
}

// Issue a close request to end the program.
BOOL CMainFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Display the help about dialog.
BOOL CMainFrame::OnHelp()
{
    // Ensure only one dialog displayed even for multiple hits of the F1 button.
    if (!m_aboutDialog.IsWindow())
    {
        m_aboutDialog.DoModal(*this);
    }

    return TRUE;
}

// Called after the window is created.
void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    m_view.AppendBigText(L"Instructions\n\n");
    m_view.AppendSmallText(L"This program searches all source files within the selected folder\n");
    m_view.AppendSmallText(L"for lines containing tabs, trailing spaces and non-ascii characters.\n");
    m_view.AppendSmallText(L"Source files in any subfolders are searched as well.\n");
    m_view.AppendSmallText(L"When fixed, tabs are replaced with 4 space characters,\n");
    m_view.AppendSmallText(L"trailing spaces are removed from the end of lines,\n");
    m_view.AppendSmallText(L"and non-ascii characters are removed from lines.\n\n");

    m_view.AppendSmallText(L"Select the Folder button to choose the folder containing the source files.\n");
    m_view.AppendSmallText(L"Select the Search button to list the files which will be modified.\n");
    m_view.AppendSmallText(L"Select the Edit button to fix the listed files.\n\n");

    m_view.AppendBigWarning(L"Warning:\n");
    m_view.AppendSmallWarning(L"This will modify source files in the selected folder and any subfolders.\n");
    m_view.AppendSmallWarning(L"All non-ascii characters will be removed from source files.\n");
    m_view.AppendSmallWarning(L"Make a backup copy of your source files before modifying them.\n");
    m_view.AppendSmallWarning(L"Choose the folder containing the source files carefully.\n\n");
}

BOOL CMainFrame::OnChooseFolder()
{
    if (PickFolder(m_folder))
    {
        m_view.AppendBigText(L"The folder selected is:\n");
        m_view.AppendBigText(m_folder + L"\n");
    }

    return TRUE;
}

// Prepare for a new search.
BOOL CMainFrame::OnFileNew()
{
    m_folder = GetStartFolder();
    m_view.SetWindowText(0);
    m_filesWithEndSpace.clear();
    m_filesWithTabs.clear();
    m_fileContents.clear();
    return TRUE;
}

// Previews a print job before sending it to the printer.
BOOL CMainFrame::OnFilePreview()
{
    try
    {
        m_isToolbarShown = GetToolBar().IsWindow() && GetToolBar().IsWindowVisible();

        // Get the device context of the default or currently chosen printer.
        CPrintDialog printDlg;
        CDC printerDC = printDlg.GetPrinterDC();

        // Create the preview window if required.
        if (!m_preview.IsWindow())
            m_preview.Create(*this);

        // Set the preview's owner (for messages), and number of pages.
        UINT maxPage = m_view.CollatePages();
        m_preview.DoPrintPreview(*this, maxPage);

        // Swap views.
        SetView(m_preview);

        // Hide the menu and toolbar.
        ShowMenu(FALSE);
        ShowToolBar(FALSE);

        // Update status.
        CString status = L"Printer: " + printDlg.GetDeviceName();
        SetStatusText(status);
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), L"Print Preview Failed", MB_ICONWARNING);
        SetView(m_view);
        ShowMenu(GetFrameMenu() != 0);
        ShowToolBar(m_isToolbarShown);
    }

    return TRUE;
}

// Bring up a dialog to choose the printer.
BOOL CMainFrame::OnFilePrint()
{
    try
    {
        m_view.DoPrint(L"Batch Editor");
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), L"Print Failed", MB_ICONWARNING);
    }

    return TRUE;
}

// Called when the Print Preview's "Close" button is pressed.
BOOL CMainFrame::OnPreviewClose()
{
    // Swap the view.
    SetView(m_view);

    // Show the menu and toolbar.
    ShowMenu(GetFrameMenu() != 0);
    ShowToolBar(m_isToolbarShown);

    SetStatusText(LoadString(IDW_READY));

    return TRUE;
}

// Called when the Print Preview's "Print Now" button is pressed.
BOOL CMainFrame::OnPreviewPrint()
{
    m_view.QuickPrint(L"Batch Editor");
    return TRUE;
}

// Called when the Print Preview's "Print Setup" button is pressed.
BOOL CMainFrame::OnPreviewSetup()
{
    // Call the print setup dialog.
    CPrintDialog printDlg(PD_PRINTSETUP);
    try
    {
        // Display the print dialog.
        if (printDlg.DoModal(*this) == IDOK)
        {
            CString status = L"Printer: " + printDlg.GetDeviceName();
            SetStatusText(status);
        }
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);
    }

    // Initiate the print preview.
    UINT maxPage = m_view.CollatePages();
    m_preview.DoPrintPreview(*this, maxPage);

    return TRUE;
}

// Check source files for tabs and trailing white space.
BOOL CMainFrame::OnCheckFiles()
{
    // Clear any previous searches.
    m_filesWithEndSpace.clear();
    m_filesWithNonAscii.clear();
    m_filesWithTabs.clear();

    // Check file for tabs
    m_view.AppendBigText(L"Source files with tabs\n");
    CheckForTabs(m_folder);
    if (m_filesWithTabs.size() == 0)
        m_view.AppendSmallText(L"None\n");
    else
    {
        CString str = L"Files with tabs: " +
        ToCString(m_filesWithTabs.size()) + L"\n";
        m_view.AppendSmallText(str);
    }

    // Check files for trailing white space
    m_view.AppendBigText(L"Source files with trailing white space\n");
    CheckForEndSpace(m_folder);
    if (m_filesWithEndSpace.size() == 0)
        m_view.AppendSmallText(L"None\n");
    else
    {
        CString str = L"Files with trailing white space: " +
            ToCString(m_filesWithEndSpace.size()) + L"\n";
        m_view.AppendSmallText(str);
    }

    m_view.AppendBigText(L"Source files with non-ascii characters\n");
    CheckForNonAscii(m_folder);
    if (m_filesWithNonAscii.size() == 0)
        m_view.AppendSmallText(L"None\n");
    else
    {
        CString str = L"Files with non-ascii characters: " +
            ToCString(m_filesWithNonAscii.size()) + L"\n";
        m_view.AppendSmallText(str);
    }

    SetStatusText(L"Done");

    return TRUE;
}

BOOL CMainFrame::OnFixFiles()
{
    m_view.AppendBigText(L"Fixing source files with tabs\n");

    // Fix tabs.
    for (CString& file : m_filesWithTabs)
    {
        try
        {
            CString text = "Fixing tabs in ";
            text << file << "\n";
            m_view.AppendSmallText(text);
            ReadFile(file);
            for (CStringA& line : m_fileContents)
            {
                line.Replace("\t", "    ");
                line.TrimRight(); // We might end with a tab.
            }
            WriteFile(file);
        }
        catch (const CUserException& e)
        {
            m_view.AppendSmallText(L"  File ignored\n");
            MessageBox(e.GetText() + CString("\n") + file, L"Bad File", MB_OK);
        }
    }

    // Fix trailing white space.
    m_view.AppendBigText(L"Fixing source files with trailing white space\n");

    for (CString& file : m_filesWithEndSpace)
    {
        try
        {
            CString text = "Fix trailing white space in ";
            text << file << "\n";
            m_view.AppendSmallText(text);
            ReadFile(file);
            for (CStringA& line : m_fileContents)
            {
                line.TrimRight();
            }
            WriteFile(file);
        }
        catch (const CUserException& e)
        {
            m_view.AppendSmallText(L"  File ignored\n");
            MessageBox(e.GetText() + file, L"Bad File", MB_OK);
        }
    }

    // Fix non-ascii.
    m_view.AppendBigText(L"Fixing source files with non-ascii characters\n");

    for (CString& file : m_filesWithNonAscii)
    {
        try
        {
            ReadFile(file);

            for (CStringA& line : m_fileContents)
            {
                if (!line.IsEmpty())
                {
                    int maxIndex = line.GetLength() - 1;
                    for (int index = maxIndex; index >= 0; --index)
                    {
                        // We might have removed multiple non-ascii character previously.
                        if (index < line.GetLength())
                        {
                            char c = line.GetAt(index);
                            if (c < 0)
                            {
                                // Remove all of these non-ascii characters from the line.
                                line.Remove(c);
                                CString text = "Removed non-ascii charater(s): ";
                                text << c << "  from: " << file << "\n";
                                m_view.AppendSmallText(text);
                            }
                        }
                    }
                }
            }
            WriteFile(file);
        }
        catch (const CUserException& e)
        {
            m_view.AppendSmallText(L"  File ignored\n");
            MessageBox(e.GetText() + file, L"Bad File", MB_OK);
        }
    }


    m_view.AppendSmallText(L"Done\n");
    m_fileContents.clear();
    m_filesWithTabs.clear();
    m_filesWithEndSpace.clear();
    m_filesWithNonAscii.clear();

    return TRUE;
}

// Fills m_fileContents with the contents of the specified file.
// Throws a CUserException if text lines don't end with CRLF.
void CMainFrame::ReadFile(const CString& fileName)
{
    CFile file(fileName, OPEN_EXISTING);
    ULONGLONG fileSize = file.GetLength();

    if (fileSize >= UINT_MAX -2)
        throw CUserException(L"File too large");

    // Fill the local buffer with the file's contents.
    std::vector<char> buffer;
    buffer.assign(static_cast<UINT>(fileSize +1), 0);
    file.Read(buffer.data(), static_cast<UINT>(fileSize));
    file.Close();

    std::vector<char>::iterator it;
    for (it = buffer.begin(); it != buffer.end(); ++it)
    {
        // Check for line ending in \r\n, throw exception if not found.
        // Replace \r\n with \0\0
        if ((*it) == '\n')
        {
            if (it == buffer.begin() || *(it - 1) != '\r')
                throw CUserException(L"Not a Windows text file");
            *(it - 1) = '\0';
            *it = '\0';
        }
    }

    // Assign each line of text to a CStringA, stored in m_fileContents.
    m_fileContents.clear();
    size_t pos = 0;
    LPCSTR s = buffer.data();
    while (pos <= fileSize)
    {
        // Assign string, stripping out carriage return.
        CStringA str = s + pos;
        m_fileContents.push_back(str);
        pos += str.GetLength() + size_t(2);
    }
}

// Writes the contents of m_fileContents to the specified file.
void CMainFrame::WriteFile(const CString& fileName)
{
    CFile file(fileName, CREATE_ALWAYS);  // Truncates existing file to zero.

    // Write each line of text to the file.
    for (auto it = m_fileContents.begin(); it != m_fileContents.end(); ++it)
    {
        file.Write((*it).c_str(), (*it).GetLength());
        if (it + 1 != m_fileContents.end())
            file.Write("\r\n", 2);
    }

    file.Close();
}

// Configure the menu icons.
void CMainFrame::SetupMenuIcons()
{
    // Set the bitmap used for menu icons.
    AddMenuIcons(GetToolBarData(), RGB(192, 192, 192), IDB_MENUICONS, 0);
}

// Assigns images and command IDs to the toolbar buttons.
void CMainFrame::SetupToolBar()
{
    // Note: The toolbar is destroyed and recreated when the DPI changes when
    // using Per Monitor DPI Awareness.
    // This function is called when the toobar is created.

    // Set the resource IDs for the toolbar buttons.
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_CHOOSE );
    AddToolBarButton( IDM_FILE_CHECK);
    AddToolBarButton( IDM_FILE_FIX );

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_FILE_PRINT );

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_HELP_ABOUT );
}

// Handle the frame's messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case UWM_PREVIEWCLOSE:      OnPreviewClose();   break;
    case UWM_PREVIEWPRINT:      OnPreviewPrint();   break;
    case UWM_PREVIEWSETUP:      OnPreviewSetup();   break;

    default: return WndProcDefault(msg, wparam, lparam);
    }

    return 0;
}

