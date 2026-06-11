/////////////////////////////////////////
// Mainfrm.h
// Authors: Robert Tausworthe, David Nash
//


#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"

// Registry key for saving and loading the program's persistent data.
static const LPCWSTR FRAME_REGISTRY_KEY = L"Win32++\\FilePrintPreview";

// Set the allowed number of MRU items.
static const int MAXMRU = 5;

///////////////////////////////////
// CMainFrame function definitions.
//

// Construct the main frame object for the sample program.
CMainFrame::CMainFrame() : m_wrapOption(WRAP_NONE)
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    // Set the frame's view window.
    SetView(m_richView);

    // Set the registry key name, and load persistent data.
    LoadRegistrySettings(FRAME_REGISTRY_KEY);

    // Load the MRU settings from the registry: allow MAXMRU entries.
    LoadRegistryMRUSettings(MAXMRU);

    return CFrame::Create(parent);
}

// Save the document file, if modified, and shut down the program.
void CMainFrame::OnClose()
{
    ShowToolBar(TRUE);

    //Check for unsaved text
    SaveModifiedText();

    // Call the base function, which calls SaveRegistrySettings()
    CFrame::OnClose();
}

// Route all command messages to their proper handlers.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);

    // Handle a MRU load request.
    if (IDW_FILE_MRU_FILE1 <= id && id < IDW_FILE_MRU_FILE1 + MAXMRU)
        return OnFileMRU(wparam);

    switch (id)
    {
        case IDM_FILE_NEW:          return OnFileNew();
        case IDM_FILE_OPEN:         return OnFileOpen();
        case IDM_FILE_SAVE:         return OnFileSave();
        case IDM_FILE_SAVEAS:       return OnFileSaveAs();
        case IDM_FILE_PRINTSETUP:   return OnFilePrintSetup(*this);
        case IDM_FILE_PREVIEW:      return OnFilePreview();
        case IDM_FILE_PRINT:        return OnFilePrint(*this);
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
        case IDM_OPTIONS_FONT:      return OnOptionsFont();
        case IDM_HELP_ABOUT:        return OnHelp();
        case IDM_WRAP_NONE:
        case IDM_WRAP_WINDOW:
        case IDM_WRAP_PRINTER:
            return OnOptionsWrap((WordWrapType)(id - IDM_WRAP_NONE));

        default:return FALSE;;
    }
}

// Specify values that control the way the frame is created.Overriding
// CFrame::OnCreate is optional.
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

    // Call the base class function.
    return CFrame::OnCreate(cs);
}

//  Enable files dropped in the client area to be opened. Any file
// currently open is closed.
BOOL CMainFrame::OnDropFiles(HDROP dropInfo)
{
    wchar_t filePath[_MAX_PATH];
    ::DragQueryFile(dropInfo, 0, filePath, _MAX_PATH);

    if (ReadFile(filePath))
    {
        SetPath(filePath);
        AddMRUEntry(filePath);
    }
    ::DragFinish(dropInfo);
    return TRUE;
}

// Copy the current selection (if any) to the clipboard.
BOOL CMainFrame::OnEditCopy()
{
    m_richView.Copy();
    return TRUE;
}

// Cut the current selection(if any) into the clipboard.
BOOL CMainFrame::OnEditCut()
{
    m_richView.Cut();
    return TRUE;
}

// Delete the current selection(if any).
BOOL CMainFrame::OnEditDelete()
{
    m_richView.Clear();
    return TRUE;
}

// Copy the clipboard contents, if any, into the rich edit control at the
// caret, replacing any selected item(s).
BOOL CMainFrame::OnEditPaste()
{
    m_richView.PasteSpecial(CF_TEXT);
    return TRUE;
}

// Reapply the last edit function to the rich edit control.
BOOL CMainFrame::OnEditRedo()
{
    m_richView.Redo();
    return TRUE;
}

// Remove the effects of the last edit function to the rich edit control.
BOOL CMainFrame::OnEditUndo()
{
    m_richView.Undo();
    return TRUE;
}

// Close the application.
BOOL CMainFrame::OnFileExit()
{
    // Issue a close request to the frame.
    Close();
    return TRUE;
}

// Load the file whose path appears in the MRU list at the index given
// in the wparam, if one exists and is readable. If the file cannot be loaded,
// remove the MRU entry. Return TRUE on termination.
BOOL CMainFrame::OnFileMRU(WPARAM wparam)
{
    UINT MRUIndex = LOWORD(wparam) - IDW_FILE_MRU_FILE1;
    CString path = GetMRUEntry(MRUIndex);

    // Skip this if the MRU list is empty.
    if (path.IsEmpty())
        return TRUE;

    if (ReadFile(path))
        SetPath(path);
    else
        RemoveMRUEntry(path);

    return TRUE;
}

// Close any existing open document and create a new empty one with no path.
BOOL CMainFrame::OnFileNew()
{
    // Check for unsaved text.
    SaveModifiedText();

    // Reset the rich view object
    m_richView.SetWindowText(L"");
    SetPath(L"");
    m_richView.SetFontDefaults();
    m_richView.SetModify(FALSE);
    return TRUE;
}

// Prompt the user for a document file name and, if valid, load this file
// into the application.
BOOL CMainFrame::OnFileOpen()
{
    // Ddefine two-part CFileDialog filter, one for text strings and another
    // for all files.
    LPCWSTR filter = L"Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";
    CFileDialog fileDlg(TRUE, L"txt", nullptr, OFN_FILEMUSTEXIST, filter);

    // Open the dialog and get the path.
    if (fileDlg.DoModal(*this) == IDOK)
    {
        CString path = fileDlg.GetPathName();
        ReadFile(path);
        SetPath(path);
        AddMRUEntry(path);
    }

    return TRUE;
}

// Create the print preview window with the displayed file title in the
// caption window and the document in separate the display window.
BOOL CMainFrame::OnFilePreview()
{
    return m_printPreview.OnPreview(GetPath());
}

// Print the currently loaded document o the currently selected printer.
BOOL CMainFrame::OnFilePrint(HWND parent)
{
    // Initialize the current print dialog.
    CPrintDialog printDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC |
        PD_ENABLEPRINTHOOK);
    PRINTDLG pd = printDlg.GetParameters();
    pd.nCopies   = 1;
    pd.nFromPage = 0xFFFF;
    pd.nToPage   = 0xFFFF;
    pd.nMinPage  = 1;
    pd.nMaxPage  = 0xFFFF;
    printDlg.SetParameters(pd);
    try
    {
        // Display the printer dialog box.
        if (printDlg.DoModal(parent) == IDOK)
        {
            QuickPrint(printDlg);
        }
        else
            return FALSE;
    }
    catch (const CWinException& /* e */)
    {
          // no default printer chosen
        MessageBox(L"Unable to display print dialog",
            L"Print Failed", MB_OK);
        return FALSE;
    }
    return TRUE;
}

// Display the printer setup dialog to select a printer and associated
// parameters for printing or previewing the current document.
BOOL CMainFrame::OnFilePrintSetup(HWND parent)
{
    // Prepare the print dialog.
    CPrintDialog printDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC |
        PD_PRINTSETUP);
    PRINTDLG pd  = printDlg.GetParameters();
    pd.nCopies   = 1;
    pd.nFromPage = 0xFFFF;
    pd.nToPage   = 0xFFFF;
    pd.nMinPage  = 1;
    pd.nMaxPage  = 0xFFFF;
    printDlg.SetParameters(pd);
    try
    {
        // Display the print dialog.
        printDlg.DoModal(parent);
    }
    catch (const CWinException& /* e */)
    {
          // No default printer
        MessageBox(L"Unable to display print dialog",
            L"Print Failed", MB_OK);
        return FALSE;
    }

    return TRUE;
}

// Print the current document using the currently selected printer
// parameters, without bringing up the printer dialog box.
BOOL CMainFrame::OnFileQuickPrint()
{
    // Acquire the currently selected printer and page settings.
    CPrintDialog printDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
    QuickPrint(printDlg);
    return TRUE;
}

// Save the current document.
BOOL CMainFrame::OnFileSave()
{
    if (GetPath().IsEmpty())
        OnFileSaveAs();
    else
        WriteFile(GetPath());

    return TRUE;
}

// Save the current document under a new name and make this the current
// document.
BOOL CMainFrame::OnFileSaveAs()
{
    // Define two-part CFileDialog filter, one for text strings and another
    // for all files.
    LPCWSTR filter = L"Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";
    CFileDialog fileDlg(FALSE, L"txt", nullptr, OFN_OVERWRITEPROMPT, filter);

    // Bring up the dialog and get the file path.
    if (fileDlg.DoModal(*this) == IDOK)
    {
        CString str = fileDlg.GetPathName();
        WriteFile(str);
        SetPath(str);
        AddMRUEntry(str);
    }
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

// Perform initializations necessary for application startup.
void CMainFrame::OnInitialUpdate()
{
    DragAcceptFiles(TRUE);

    // Reopen most recently used file.
    OnFileMRU(IDW_FILE_MRU_FILE1);
}

// Called by the framework to update the menu item with identifier id.
void CMainFrame::OnMenuUpdate(UINT id)
{
    if (IDM_WRAP_NONE <= id && id <= IDM_WRAP_PRINTER)
    {
        UINT active = m_wrapOption + IDM_WRAP_NONE;
        GetFrameMenu().CheckMenuRadioItem(IDM_WRAP_NONE, IDM_WRAP_PRINTER,
            active, MF_BYCOMMAND);
    }

    CFrame::OnMenuUpdate(id);
}

LRESULT CMainFrame::OnNotify(WPARAM wparam, LPARAM lparam)
{
    NMHDR* nmh = (LPNMHDR) lparam;
    switch (nmh->code)
    {
        case EN_DROPFILES:
        {
            ENDROPFILES* ENDrop = reinterpret_cast<ENDROPFILES*>(lparam);
            HDROP dropInfo = (HDROP) ENDrop->hDrop;
            OnDropFiles(dropInfo);
            return TRUE;
        }

        default: return CFrame::OnNotify(wparam, lparam);
    }
}

BOOL CMainFrame::OnOptionsFont()
{
    return GetRichView().GetNewFont();
}

BOOL CMainFrame::OnOptionsWrap(WordWrapType option)
{
    m_richView.WordWrap(m_wrapOption = option);
    return TRUE;
}

// Customize the CREATESTRUCT structure prior to creation of the frame.
void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
    CFrame::PreCreate(cs); // Set the saved frame creation parameters.

//  cs.cx = 755;    // You may uncomment these to override saved settings.
//  cs.cy = 420;
}

// Print the document without bringing up the print dialog box using the
// printDlg object.
void CMainFrame::QuickPrint(CPrintDialog& printDlg)
{
    CPrintDialog dlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
    HDC hPrinter = dlg.GetPrinterDC();
    if (hPrinter == nullptr)
    {
        MessageBox(L"Quick Print requires a printer",
            L"No Printer found", MB_ICONWARNING);
        return;
    }
    GetRichView().PrintPages(printDlg);
}

// Open the filePath file as a rich edit view stream for display in the main
// window.
BOOL CMainFrame::ReadFile(LPCWSTR filePath)
{
      //Check for unsaved text
    SaveModifiedText();
    return GetRichView().ReadFile(filePath);
}

// If the current document has been changed, save it; otherwise do nothing.
void CMainFrame::SaveModifiedText()
{
    //Check for unsaved text.
    if (m_richView.GetModify())
        if (MessageBox(L"Save changes to this document?",
            L"Question...", MB_YESNO | MB_ICONQUESTION) == IDYES)
            OnFileSave();
}

// Add icons to the main menu items not having toolbar icons.
void CMainFrame::SetupMenuIcons()
{
    // Add the icons from the toolbar.
    CFrame::SetupMenuIcons();

    // Print and preview menu bitmaps.
    AddMenuIcon(IDM_FILE_PREVIEW,    GetApp()->LoadIcon(IDI_PRINTPREVIEW));
    AddMenuIcon(IDM_FILE_QUICKPRINT, GetApp()->LoadIcon(IDI_QUICKPRINT));
    AddMenuIcon(IDM_FILE_PRINT,      GetApp()->LoadIcon(IDI_PRINT));

    // Options menu font dialog icon.
    AddMenuIcon(IDM_OPTIONS_FONT,    GetApp()->LoadIcon(IDI_FONT_OPTION));

    // Set options menu radio button unselected icon.
    HICON check_box_unselected =     GetApp()->LoadIcon(IDI_CHECKBOX_OFF);
    AddMenuIcon(IDM_WRAP_NONE,       check_box_unselected);
    AddMenuIcon(IDM_WRAP_WINDOW,     check_box_unselected);
    AddMenuIcon(IDM_WRAP_PRINTER,    check_box_unselected);

    // Set toolbar and status bar check boxes unselected icons.
    AddMenuIcon(IDW_VIEW_TOOLBAR,    check_box_unselected);
    AddMenuIcon(IDW_VIEW_STATUSBAR,  check_box_unselected);
}

// Assigns images and command IDs to the toolbar buttons.
void CMainFrame::SetupToolBar()
{
    // Note: The toolbar is destroyed and recreated when the DPI changes when
    // using Per Monitor DPI Awareness.
    // This function is called when the toobar is created.

    // Set the resource IDs for the toolbar buttons.
    AddToolBarButton(IDM_FILE_NEW);
    AddToolBarButton(IDM_FILE_OPEN);
    AddToolBarButton(IDM_FILE_SAVE);
    AddToolBarButton(0);                // Separator
    AddToolBarButton(IDM_EDIT_CUT);
    AddToolBarButton(IDM_EDIT_COPY);
    AddToolBarButton(IDM_EDIT_PASTE);
    AddToolBarButton(0);                // Separator
    AddToolBarButton(IDM_FILE_PRINT);
    AddToolBarButton(0 );               // Separator
    AddToolBarButton(IDM_HELP_ABOUT);
}

// Set the document path and display it in the window caption if it is non
// null. Put the app title in the caption if not.
void CMainFrame::SetPath(LPCWSTR path)
{
    m_path = path;
    CString title = (m_path.IsEmpty() ?
        L"FilePrintPreview Sample Program" : m_path.c_str());
    SetWindowText(title);
}

// Write the contents of the rich view control into the given file path.
BOOL CMainFrame::WriteFile(LPCWSTR filePath)
{
    return GetRichView().WriteFile(filePath);
}

// The mainframe message processing procedure.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
            case WM_SYSCOMMAND:
            {
                switch (LOWORD(wparam))
                {
                    case SC_CLOSE:
                    m_printPreview.Destroy();
                    break;  // let default process this further

                    default: break;
                }
            }

            default: return WndProcDefault(msg, wparam, lparam);
        }
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << L"Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    return 0;
}
