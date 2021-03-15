/* (11-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                (MainFrm.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CMainFrame class for the
    FilePrintPreview demo program using the Win32++ Windows interface classes.

    Programming Notes: The programming style roughly follows that established
    for the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "Mainfrm.h"
#include <richedit.h>
#include "resource.h"

  // Registry key for saving and loading the program's persistent data, which
  // is placed in the registry location
  // "Software\\" + LPCTSTR FRAME_REGISTRY_KEY + "\\Frame Settings"
static const LPCTSTR FRAME_REGISTRY_KEY = _T("Win32++\\FilePrintPreview");
  // set the allowed number of MRU items
static const int MAXMRU = 5;

/*============================================================================*/
    CMainFrame::
CMainFrame()                                                                /*

    Construct the main frame object for the PrintPreviewClass demo.
*-----------------------------------------------------------------------------*/
    :   m_wrapOption(WRAP_NONE)
{
}

/*============================================================================*/
    HWND CMainFrame::
Create(HWND parent)                                                         /*

    Create the frame window.
*-----------------------------------------------------------------------------*/
{
    SetView(m_richView);
    // Set the registry key name, and load persistent data
    LoadRegistrySettings(FRAME_REGISTRY_KEY);
    // Load the MRU settings from the registry: allow MAXMRU entries
    LoadRegistryMRUSettings(MAXMRU);

    return CFrame::Create(parent);
}

/*============================================================================*/
    void CMainFrame::
OnClose()                                                                   /*

    Save the document file, if modified, and shut down the program.
*-----------------------------------------------------------------------------*/
{
    ShowToolBar(TRUE);
      //Check for unsaved text
    SaveModifiedText();
      // Call the base function, which calls SaveRegistrySettings()
    CFrame::OnClose();
}

/*============================================================================*/
    BOOL CMainFrame::
OnCommand(WPARAM wparam, LPARAM lparam)                                     /*

    Route all command messages to their proper handlers.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
      // handle a MRU load request
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
    }
    return FALSE;
}

/*============================================================================*/
    int CMainFrame::
OnCreate(CREATESTRUCT& cs)                                                  /*

    Specify values that control the way the frame is created. Overriding
    CFrame::OnCreate is optional.
*-----------------------------------------------------------------------------*/
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

/*============================================================================*/
    BOOL CMainFrame::
OnDropFiles(HDROP dropInfo)                                                 /*

    Enable files dropped in the client area to be opened. Any file
    currently open is closed.
*-----------------------------------------------------------------------------*/
{
    TCHAR filePath[_MAX_PATH];
    ::DragQueryFile(dropInfo, 0, filePath, _MAX_PATH);

    if (ReadFile(filePath))
    {
        SetPath(filePath);
        AddMRUEntry(filePath);
    }
    ::DragFinish(dropInfo);
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnEditCopy()                                                                /*

    Copy the current selection (if any) to the clipboard.
*-----------------------------------------------------------------------------*/
{
    m_richView.Copy();
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnEditCut()                                                                 /*

    Cut the current selection (if any) into the clipboard.
*-----------------------------------------------------------------------------*/
{
    m_richView.Cut();
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnEditDelete()                                                              /*

    Delete the current selection (if any).
*-----------------------------------------------------------------------------*/
{
    m_richView.Clear();
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnEditPaste()                                                               /*

    Copy the clipboard contents, if any, into the rich edit control at the
    caret, replacing any selected item(s).
*-----------------------------------------------------------------------------*/
{
    m_richView.PasteSpecial(CF_TEXT);
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnEditRedo()                                                                /*

    Reapply the last edit function to the rich edit control.
*-----------------------------------------------------------------------------*/
{
    m_richView.Redo();
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnEditUndo()                                                                /*

    Remove the effects of the last edit function to the rich edit control.
*-----------------------------------------------------------------------------*/
{
    m_richView.Undo();
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFileExit()                                                                /*

    Close the application.
*-----------------------------------------------------------------------------*/
{
      // Issue a close request to the frame
    Close();
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFileMRU(WPARAM wparam)                                                    /*

    Load the file whose path appears in the MRU list at the index given
    in the wparam, if one exists and is readable. If the file cannot be loaded,
    remove the MRU entry. Return TRUE on termination.
*-----------------------------------------------------------------------------*/
{
    UINT MRUIndex = LOWORD(wparam) - IDW_FILE_MRU_FILE1;
    CString path = GetMRUEntry(MRUIndex);
      // skip this if the MRU list is empty
    if (path.IsEmpty())
        return TRUE;

    if (ReadFile(path))
        SetPath(path);
    else
        RemoveMRUEntry(path);
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFileNew()                                                                 /*

    Close any existing open document and create a new empty one with no path.
*-----------------------------------------------------------------------------*/
{
      //Check for unsaved text
    SaveModifiedText();
      // reset the rich view object
    m_richView.SetWindowText(_T(""));
    SetPath(_T(""));
    m_richView.SetFontDefaults();
    m_richView.SetModify(FALSE);
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFileOpen()                                                                /*

    Prompt the user for a document file name and, if valid, load this file
    into the application.
*-----------------------------------------------------------------------------*/
{
      // define two-part CFileDialog filter, one for text strings and another
      // for all files.
    LPCTSTR filter = _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
    CFileDialog fileDlg(TRUE, _T("txt"), NULL, OFN_FILEMUSTEXIST, filter);
      // open the dialog and get the path
    if (fileDlg.DoModal(*this) == IDOK)
    {
        CString path = fileDlg.GetPathName();
        ReadFile(path);
        SetPath(path);
        AddMRUEntry(path);
    }
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFilePreview()                                                             /*

    Create the print preview window with the displayed file title in the
    caption window and the document in separate the display window.
*-----------------------------------------------------------------------------*/
{
    return m_printPreview.OnPreview(GetPath());
}

/*============================================================================*/
    BOOL CMainFrame::
OnFilePrint(HWND parent)                                                    /*

    Print the currently loaded document o the currently selected printer.
*-----------------------------------------------------------------------------*/
{
      // initialize the current print dialog
    CPrintDialog printDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
    PRINTDLG pd = printDlg.GetParameters();
    pd.nCopies   = 1;
    pd.nFromPage = 0xFFFF;
    pd.nToPage   = 0xFFFF;
    pd.nMinPage  = 1;
    pd.nMaxPage  = 0xFFFF;
    printDlg.SetParameters(pd);
    try
    {
          // display the printer dialog box
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
        MessageBox(_T("Unable to display print dialog"),
            _T("Print Failed"), MB_OK);
        return FALSE;
    }
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFilePrintSetup(HWND parent)                                               /*

    Display the printer setup dialog to select a printer and associated
    parameters for printing or previewing the current document.
*-----------------------------------------------------------------------------*/
{
      // Prepare the print dialog
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
          // Display the print dialog
        printDlg.DoModal(parent);
    }
    catch (const CWinException& /* e */)
    {
          // No default printer
        MessageBox(_T("Unable to display print dialog"),
            _T("Print Failed"), MB_OK);
        return FALSE;
    }
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFileQuickPrint()                                                          /*

    Print the current document using the currently selected printer
    parameters, without bringing up the printer dialog box.
*-----------------------------------------------------------------------------*/
{
      // Acquire the currently selected printer and page settings
    CPrintDialog printDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
    QuickPrint(printDlg);
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFileSave()                                                                /*

    Save the current document.
*-----------------------------------------------------------------------------*/
{
    if (GetPath().IsEmpty())
        OnFileSaveAs();
    else
        WriteFile(GetPath());

    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFileSaveAs()                                                              /*

    Save the current document under a new name and make this the current
    document.
*-----------------------------------------------------------------------------*/
{
      // define two-part CFileDialog filter, one for text strings and another
      // for all files.
    LPCTSTR filter = _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
    CFileDialog fileDlg(FALSE, _T("txt"), NULL, OFN_OVERWRITEPROMPT, filter);
      // bring up the dialog and get the file path
    if (fileDlg.DoModal(*this) == IDOK)
    {
        CString str = fileDlg.GetPathName();
        WriteFile(str);
        SetPath(str);
        AddMRUEntry(str);
    }
    return TRUE;
}

/*============================================================================*/
    void CMainFrame::
OnInitialUpdate()                                                           /*

    Perform initializations necessary for application startup.
*-----------------------------------------------------------------------------*/
{
    DragAcceptFiles(TRUE);
      // reopen most recently used file
    OnFileMRU(IDW_FILE_MRU_FILE1);
}

/*============================================================================*/
    void CMainFrame::
OnMenuUpdate(UINT id)                                                       /*

    Called by the framework to update the menu item with identifier id.
*-----------------------------------------------------------------------------*/
{
    if (IDM_WRAP_NONE <= id && id <= IDM_WRAP_PRINTER)
    {
        UINT active = m_wrapOption + IDM_WRAP_NONE;
        GetFrameMenu().CheckMenuRadioItem(IDM_WRAP_NONE, IDM_WRAP_PRINTER,
            active, MF_BYCOMMAND);
    }
    CFrame::OnMenuUpdate(id);
}

/*============================================================================*/
    LRESULT CMainFrame::
OnNotify(WPARAM wparam, LPARAM lparam)                                      /*

*-----------------------------------------------------------------------------*/
{
    NMHDR* nmh = (LPNMHDR) lparam;
    switch (nmh->code)
    {
        case EN_DROPFILES:
        {
            ENDROPFILES* ENDrop = reinterpret_cast<ENDROPFILES*>(lparam);
            HDROP dropInfo = (HDROP) ENDrop->hDrop;
            OnDropFiles(dropInfo);
        }
        return TRUE;
    }
    return CFrame::OnNotify(wparam, lparam);
}

/*============================================================================*/
    BOOL CMainFrame::
OnOptionsFont()                                                             /*

*-----------------------------------------------------------------------------*/
{
    return GetRichView().GetNewFont();
}

/*============================================================================*/
    BOOL CMainFrame::
OnOptionsWrap(WordWrapType option)                                          /*

    Set the rich view word wrap option.
*-----------------------------------------------------------------------------*/
{
      // Note: this is an undocumented feature of the CRichEdit class
    m_richView.WordWrap(m_wrapOption = option);
    return TRUE;
}

/*============================================================================*/
    void CMainFrame::
PreCreate(CREATESTRUCT& cs)                                                 /*

    Customize the CREATESTRUCT structure prior to creation of the frame.
*-----------------------------------------------------------------------------*/
{
    CFrame::PreCreate(cs); // Set the saved frame creation parameters

//  cs.cx = 755;    // you may uncomment this to override saved settings
//  cs.cy = 420;    // but remove resizing in OnInitialUpdate()
}

/*============================================================================*/
    void CMainFrame::
QuickPrint(CPrintDialog& printDlg)                                          /*

    Print the document without bringing up the print dialog box using the
    printDlg object.
*-----------------------------------------------------------------------------*/
{
    CPrintDialog dlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
    HDC hPrinter = dlg.GetPrinterDC();
    if (hPrinter == 0)
    {
        MessageBox(_T("Quick Print requires a printer"),_T("No Printer found"),
            MB_ICONWARNING);
        return;
    }
    GetRichView().PrintPages(printDlg);
}

/*============================================================================*/
    BOOL CMainFrame::
ReadFile(LPCTSTR filePath)                                                  /*

    Open the filePath file as a rich edit view stream for display in
    the main window.
*-----------------------------------------------------------------------------*/
{
      //Check for unsaved text
    SaveModifiedText();
    return GetRichView().ReadFile(filePath);
}

/*============================================================================*/
    void CMainFrame::
SaveModifiedText()                                                          /*

    If the current document has been changed, save it; otherwise do
    nothing.
*-----------------------------------------------------------------------------*/
{
      //Check for unsaved text
    if (m_richView.GetModify())
        if (MessageBox(_T("Save changes to this document?"),
            _T("Question..."), MB_YESNO | MB_ICONQUESTION) == IDYES)
            OnFileSave();
}

/*============================================================================*/
    void CMainFrame::
SetupMenuIcons()                                                            /*

    Add icons to the main menu items not having toolbar icons.
*-----------------------------------------------------------------------------*/
{
      // print and preview menu bitmaps
    AddMenuIcon(IDM_FILE_PRINTSETUP, GetApp()->LoadIcon(IDI_PRINTSETUP));
    AddMenuIcon(IDM_FILE_PREVIEW,    GetApp()->LoadIcon(IDI_PRINTPREVIEW));
    AddMenuIcon(IDM_FILE_QUICKPRINT, GetApp()->LoadIcon(IDI_QUICKPRINT));
    AddMenuIcon(IDM_FILE_PRINT,      GetApp()->LoadIcon(IDI_PRINT));
      // options menu font dialog icon
    AddMenuIcon(IDM_OPTIONS_FONT,    GetApp()->LoadIcon(IDI_FONT_OPTION));
      // set options menu radio button unselected icon
    HICON check_box_unselected =     GetApp()->LoadIcon(IDI_CHECKBOX_OFF);
    AddMenuIcon(IDM_WRAP_NONE,       check_box_unselected);
    AddMenuIcon(IDM_WRAP_WINDOW,     check_box_unselected);
    AddMenuIcon(IDM_WRAP_PRINTER,    check_box_unselected);
      // set toolbar and status bar check boxes unselected icons
    AddMenuIcon(IDW_VIEW_TOOLBAR,    check_box_unselected);
    AddMenuIcon(IDW_VIEW_STATUSBAR,  check_box_unselected);
    CFrame::SetupMenuIcons();
}

/*============================================================================*/
    void CMainFrame::
SetupToolBar()                                                              /*

    Define the resource identifiers for the toolbar, in order.
*-----------------------------------------------------------------------------*/
{
    AddToolBarButton(IDM_FILE_NEW);
    AddToolBarButton(IDM_FILE_OPEN);
    AddToolBarButton(IDM_FILE_SAVE);
    AddToolBarButton(0);              // Separator
    AddToolBarButton(IDM_EDIT_CUT);
    AddToolBarButton(IDM_EDIT_COPY);
    AddToolBarButton(IDM_EDIT_PASTE);
    AddToolBarButton(0);              // Separator
    AddToolBarButton(IDM_FILE_PRINT);
    AddToolBarButton(0 );              // Separator
    AddToolBarButton(IDM_HELP_ABOUT);
}

/*============================================================================*/
    void CMainFrame::
SetPath(LPCTSTR path)                                                       /*

    Set the document path and display it in the window caption if it is non
    null. Put the app title in the caption if not.
*-----------------------------------------------------------------------------*/
{
    m_path = path;
    CString title = (m_path.IsEmpty() ? _T("FilePrintPreview Demo") : m_path.c_str());
    SetWindowText(title);
}

/*============================================================================*/
    BOOL CMainFrame::
WriteFile(LPCTSTR filePath)                                                 /*

    Write the contents of the rich view control into the given file path.
*-----------------------------------------------------------------------------*/
{
    return GetRichView().WriteFile(filePath);
}

/*=============================================================================*/
    LRESULT CMainFrame::
WndProc(UINT msg, WPARAM wparam, LPARAM lparam)                             /*

    The mainframe message pocessing procedure.
*-----------------------------------------------------------------------------*/
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
            }
        }
    }

    return WndProcDefault(msg, wparam, lparam);
}
/*----------------------------------------------------------------------------*/
