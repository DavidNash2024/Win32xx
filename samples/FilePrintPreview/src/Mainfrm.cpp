/* (11-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                (MainFrm.h) *
********************************************************************************
|
    Implementation of the CMainFrame class for the FilePrintPreview demo
    program.

********************************************************************************

    Acknowledgment. This demo program was adapted from the PrintPreview 
    sample program appearing in the Win32++ framework sample folder, created 
    by  David Nash and published under the permissions granted in that work.
    The adaptation here implements the PrintView window as a separate popup
    window that appears on screen along with the regular program window.
    This work has been developed under the co-authorship of Robert C. 
    Tausworthe and David Nash, and released under the copyright provisions 
    of the Win32++ Interface Classes software, copyright (c) David Nash,
    2005-2018. The former author acknowledges and thanks the latter for his 
    patient direction and inspiration in the development of the classes of 
    these classes.

*******************************************************************************/

#include "stdafx.h"
#include "mainfrm.h"
#include <richedit.h>
#include "resource.h"

  // Registry key for saving and loading screen and initial print preview
  // sizes. The persistent items appear in the registry key
  // "Software\\" + LPCTSTR FRAME_REGISTRY_KEY_NAME + "\\Frame Settings"
static const LPCTSTR FRAME_REGISTRY_KEY_NAME = _T("Win32++\\FilePrintPreview");

/*============================================================================*/
    CMainFrame::
CMainFrame()                                                            /*

    Construct the main frame object for the PrintPreviewClass demo.
*-----------------------------------------------------------------------------*/
    :    m_PrintPreview(IDD_PRINTPREVIEW, 0), m_WrapOption(WRAP_NONE) 
{
    SetPathName(_T(""));
    SetView(m_RichView);

      // Set the registry key name, and load the initial window position
    LoadRegistrySettings(FRAME_REGISTRY_KEY_NAME);
      // Load the settings from the registry with 5 MRU entries
    LoadRegistryMRUSettings(5);
}

/*============================================================================*/
    CMainFrame::
~CMainFrame()                                                           /*

    Destructor.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
    void CMainFrame::
OnClose()                                                               /*

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
OnCommand(WPARAM wParam, LPARAM lParam)                                 /*

    Route all command messages to their proper handlers.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(lParam);

    UINT msg = LOWORD(wParam);
    switch (msg)
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
        case IDM_WRAP_NONE: 
        case IDM_WRAP_WINDOW:
        case IDM_WRAP_PRINTER:
            return OnOptionsWrap((WordWrapType)(msg - IDM_WRAP_NONE));
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

/*============================================================================*/
    int CMainFrame::
OnCreate(CREATESTRUCT& cs)                                              /*

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
OnDropFiles(HDROP hDropInfo)                                            /*

    Enable files dropped in the client area to be opened. Any file 
    currently open is closed.
*-----------------------------------------------------------------------------*/
{
    TCHAR szFileName[_MAX_PATH];
    ::DragQueryFile(hDropInfo, 0, szFileName, _MAX_PATH);

    if (ReadFile(szFileName))
    {
        SetPathName(szFileName);
        ReadFile(szFileName);
        SetWindowTitle();
        AddMRUEntry(szFileName);
    }

    ::DragFinish(hDropInfo);
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnEditCopy()                                                            /*

    Copy the current selection (if any) to the clipboard.
*-----------------------------------------------------------------------------*/
{
    m_RichView.Copy();
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnEditCut()                                                             /*

    Cut the current selection (if any) into the clipboard.
*-----------------------------------------------------------------------------*/
{
    m_RichView.Cut();
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnEditDelete()                                                          /*

    Delete the current selection (if any).
*-----------------------------------------------------------------------------*/
{
    m_RichView.Clear();
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnEditPaste()                                                           /*

    Copy the clipboard contents, if any, into the rich edit control at the
    caret, replacing any selected item(s).
*-----------------------------------------------------------------------------*/
{
    m_RichView.PasteSpecial(CF_TEXT);
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnEditRedo()                                                            /*

    Reapply the last edit function to the rich edit control.
*-----------------------------------------------------------------------------*/
{
    m_RichView.Redo();
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnEditUndo()                                                            /*

    Remove the effects of the last edit function to the rich edit control.
*-----------------------------------------------------------------------------*/
{
    m_RichView.Undo();
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFileExit()                                                            /*

    Close the application.
*-----------------------------------------------------------------------------*/
{
      // Issue a close request to the frame
    PostMessage(WM_CLOSE);
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFileMRU(WPARAM wParam)                                                /*

    Load the file whose path appears in the MRU list at the index given
    in the wParam, if one exists. Return TRUE on termination.
*-----------------------------------------------------------------------------*/
{
    UINT nMRUIndex = LOWORD(wParam) - IDW_FILE_MRU_FILE1;
    CString strMRUText = GetMRUEntry(nMRUIndex);
      // skip this if the MRU list is empty
    if (strMRUText.IsEmpty())
        return TRUE;

    if (ReadFile(strMRUText))
        SetPathName(strMRUText);
    else
        RemoveMRUEntry(strMRUText);

    SetWindowTitle();
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFileNew()                                                             /*

    Close any existing open document and create a new empty one.
*-----------------------------------------------------------------------------*/
{
      //Check for unsaved text
    SaveModifiedText();
      // reset the rich view object
    m_RichView.SetWindowText(_T(""));
    SetPathName(_T(""));
    SetWindowTitle();
    m_RichView.SetFontDefaults();
    m_RichView.SetModify(FALSE);
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFileOpen()                                                            /*

    Prompt the user for a document file name and, if valid, load this file
    into the application.
*-----------------------------------------------------------------------------*/
{
      // szFilters is a text string that includes two file name filters:
      // "*.my" for "MyType Files" and "*.*' for "All Files."
    LPCTSTR szFilters =
        _T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");
    CFileDialog FileDlg(TRUE, _T("txt"), NULL, OFN_FILEMUSTEXIST,
         szFilters);

    if (FileDlg.DoModal(*this) == IDOK)
    {
        CString str = FileDlg.GetPathName();
        ReadFile(str);
        SetPathName(str);
        AddMRUEntry(str);
        SetWindowTitle();
    }

    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFilePreview()                                                         /*

    Create the print preview window with the displayed file title in the
    caption window and the document in separate the display window.
*-----------------------------------------------------------------------------*/
{
    return m_PrintPreview.OnPreview(GetPathName()); 
}

/*============================================================================*/
    BOOL CMainFrame::
OnFilePrint(HWND parent)                                                /*

    Print the currently loaded document o the currently selected printer.
*-----------------------------------------------------------------------------*/
{
      // initialize the current print dialog
    CPrintDialog PrintDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
    PRINTDLG pd = PrintDlg.GetParameters();
    pd.nCopies   = 1;
    pd.nFromPage = 0xFFFF;
    pd.nToPage   = 0xFFFF;
    pd.nMinPage  = 1;
    pd.nMaxPage  = 0xFFFF;
    PrintDlg.SetParameters(pd);
    try
    {
          // display the printer dialog box
        if (PrintDlg.DoModal(parent) == IDOK)
        {
            QuickPrint(PrintDlg);
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
OnFilePrintSetup(HWND parent)                                           /*

    Display the printer setup dialog to select a printer and associated
    parameters for printing or previewing the current document.
*-----------------------------------------------------------------------------*/
{
      // Prepare the print dialog
    CPrintDialog PrintDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC | 
        PD_PRINTSETUP);
    PRINTDLG pd = PrintDlg.GetParameters();
    pd.nCopies   = 1;
    pd.nFromPage = 0xFFFF;
    pd.nToPage   = 0xFFFF;
    pd.nMinPage  = 1;
    pd.nMaxPage  = 0xFFFF;
    PrintDlg.SetParameters(pd);
    try
    {
          // Display the print dialog
        PrintDlg.DoModal(parent);
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
OnFileQuickPrint()                                                      /*

    Print the current document using the currently selected printer
    parameters, without bringing up the printer dialog box.
*-----------------------------------------------------------------------------*/
{
      // Acquire the currently selected printer and page settings
    CPrintDialog PrintDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
    QuickPrint(PrintDlg);
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFileSave()                                                            /*

    Save the current document.
*-----------------------------------------------------------------------------*/
{
    if (GetPathName().IsEmpty())
        OnFileSaveAs();
    else
        WriteFile(GetPathName());

    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFileSaveAs()                                                          /*

    Save the current document under a new name and make this the current
    document.
*-----------------------------------------------------------------------------*/
{
      // szFilter is a text string that includes two file name filters:
      // "*.my" for "MyType Files" and "*.*' for "All Files."
    LPCTSTR szFilters(_T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0"));
    CFileDialog FileDlg(FALSE, _T("txt"), NULL, OFN_OVERWRITEPROMPT,
        szFilters);

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

/*============================================================================*/
    void CMainFrame::
OnInitialUpdate()                                                       /*

    Perform initializations necessary for application startup.
*-----------------------------------------------------------------------------*/
{
    DragAcceptFiles(TRUE);
    SetWindowTitle();
      // reopen most recently used file
    OnFileMRU(IDW_FILE_MRU_FILE1);
}

/*============================================================================*/
    void CMainFrame::
OnMenuUpdate(UINT nID)                                                  /*

    Called by the framework to update the menu item with identifier nID.
*-----------------------------------------------------------------------------*/
{
    if (IDM_WRAP_NONE <= nID && nID <= IDM_WRAP_PRINTER)
    {
        UINT active = m_WrapOption + IDM_WRAP_NONE;
        CMenu &cm = GetFrameMenu();
        cm.CheckMenuRadioItem(IDM_WRAP_NONE, IDM_WRAP_PRINTER, 
            active, MF_BYCOMMAND);
    }

    CFrame::OnMenuUpdate(nID);
}

/*============================================================================*/
    LRESULT CMainFrame::
OnNotify(WPARAM wParam, LPARAM lParam)                                  /*

*-----------------------------------------------------------------------------*/
{
    NMHDR* pNMH;
    pNMH = (LPNMHDR) lParam;
    switch (pNMH->code)
    {
        case EN_DROPFILES:
        {
            ENDROPFILES* ENDrop = reinterpret_cast<ENDROPFILES*>(lParam);
            HDROP hDropInfo = (HDROP) ENDrop->hDrop;
            OnDropFiles(hDropInfo);
        }
        return TRUE;
    }
    return CFrame::OnNotify(wParam, lParam);
}

/*============================================================================*/
    BOOL CMainFrame::
OnOptionsFont()                                                         /*

*-----------------------------------------------------------------------------*/
{
    return GetRichView().GetNewFont();
}

/*============================================================================*/
    BOOL CMainFrame::
OnOptionsWrap(WordWrapType option)                                      /*

    Set the rich view word wrap option.
*-----------------------------------------------------------------------------*/
{
      // Note: this is an undocumented feature of the CRichEdit class
    m_RichView.WordWrap(m_WrapOption = option);
    return TRUE;
}

/*============================================================================*/
    void CMainFrame::
PreCreate(CREATESTRUCT& cs)                                             /*

    Customize the CREATESTRUCT structure prior to creation of the frame.
*-----------------------------------------------------------------------------*/
{
    CFrame::PreCreate(cs); // Set the saved frame creation parameters

//  cs.cx = 755;    // you may uncomment this to override saved settings
//  cs.cy = 420;    // but remove resizing in OnInitialUpdate()
}

/*============================================================================*/
    void CMainFrame::
QuickPrint(CPrintDialog& PrintDlg)                                      /*

    Print the document without bringing up the print dialog box using the
    PrintDlg object.
*-----------------------------------------------------------------------------*/
{
	CPrintDialog dlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
	HDC hPrinter = dlg.GetPrinterDC();
	if (hPrinter == 0)
	{
		::MessageBox(NULL, _T("Quick Print requires a printer"),
			_T("No Printer found"), MB_ICONWARNING);

		return;
	}

    GetRichView().PrintPages(PrintDlg);
}

/*============================================================================*/
    BOOL CMainFrame::
ReadFile(LPCTSTR szFileName)                                            /*

    Open the szFileName file as a rich edit view stream for display in
    the main window.
*-----------------------------------------------------------------------------*/
{
      //Check for unsaved text
    SaveModifiedText();
    return GetRichView().ReadFile(szFileName);
}

/*============================================================================*/
    void CMainFrame::
SaveModifiedText()                                                      /*

    If the current document has been changed, save it; otherwise do
    nothing.
*-----------------------------------------------------------------------------*/
{
      //Check for unsaved text
    if (m_RichView.GetModify())
        if (::MessageBox(NULL, _T("Save changes to this document?"), 
            _T("Question..."), MB_YESNO | MB_ICONQUESTION) == IDYES)
            OnFileSave();
}

/*============================================================================*/
    void CMainFrame::
SetupMenuIcons()                                                        /*

    Override to add icons to the main menu items not having toolbar icons.
*-----------------------------------------------------------------------------*/
{
      // print and preview menu bitmaps
    AddMenuIcon(IDM_FILE_PRINTSETUP, GetApp().LoadIcon(IDI_PRINTSETUP));
    AddMenuIcon(IDM_FILE_PREVIEW,    GetApp().LoadIcon(IDI_PRINTPREVIEW));
    AddMenuIcon(IDM_FILE_QUICKPRINT, GetApp().LoadIcon(IDI_QUICKPRINT));
    AddMenuIcon(IDM_FILE_PRINT,      GetApp().LoadIcon(IDI_PRINT));
      // options menu font dialog icon
    AddMenuIcon(IDM_OPTIONS_FONT,    GetApp().LoadIcon(IDI_FONT_OPTION));
      // set options menu radio button unselected icon
    HICON check_box_unselected =     GetApp().LoadIcon(IDI_CHECKBOX_OFF);
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
SetupToolBar()                                                          /*

*-----------------------------------------------------------------------------*/
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
    AddToolBarButton( IDM_HELP_ABOUT, TRUE, NULL, 8);
}

/*============================================================================*/
    void CMainFrame::
SetWindowTitle()                                                        /*

*-----------------------------------------------------------------------------*/
{
    CString Title;

    if (GetPathName().IsEmpty())
        Title = _T("FilePrintPreview Demo");
    else
        Title = GetPathName();

    SetWindowText(Title);
}

/*============================================================================*/
    BOOL CMainFrame::
WriteFile(LPCTSTR szFileName)                                           /*

*-----------------------------------------------------------------------------*/
{
    return GetRichView().WriteFile(szFileName);
}

/*=============================================================================*/
    LRESULT CMainFrame::
WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)                        /*

    The mainframe message pocessing procedure.
*-----------------------------------------------------------------------------*/
{
    switch (uMsg)
    {
        case WM_SYSCOMMAND:
        {
        switch (LOWORD(wParam))
        {
            case SC_CLOSE:
            m_PrintPreview.Destroy();
            break;  // let default process this further
        }
        }
    }

    return WndProcDefault(uMsg, wParam, lParam);
}

