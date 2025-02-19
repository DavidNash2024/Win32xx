/* (20-Oct-2024) [Tab/Indent: 8/8][Line/Box: 80/74]              (MainFrm.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CMainFrame class for the
    CommonDialogs demonstration sample application using the Win32++ Windows
    interface classes.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"
#include <io.h>


/*============================================================================*/
    CMainFrame::
CMainFrame()                                                                /*

    Construct and initialize the CMainFrame object from the IDD_MAIN_DIALOG
    resource defined in resource.rc. Note that the initial window location and
    size are set here.
*-----------------------------------------------------------------------------*/
    :   m_view(IDD_MAIN_DIALOG), m_isTextWrap(TRUE)
{
}

/*============================================================================*/
    HWND CMainFrame::
        Create(HWND parent)                                                 /*

    Create the frame window.
*-----------------------------------------------------------------------------*/
{
      // Set m_view as the view window of the frame.
    SetView(m_view);

      // Set the registry key name, and load the initial window position.
      // Use a registry key name like "CompanyName\\Application".
    LoadRegistrySettings(L"Win32++\\CommonDialogs Sample");

      // Load the settings from the registry with 5 MRU entries.
    LoadRegistryMRUSettings(5);

    return CFrame::Create(parent);
}

/*============================================================================*/
    BOOL CMainFrame::
DropFiles(LPARAM lparam)                                                    /*

    Open the text document dragged and dropped in the rich edit window.
*-----------------------------------------------------------------------------*/
{
    ENDROPFILES* ENDrop = reinterpret_cast<ENDROPFILES*>(lparam);
    HDROP hDropinfo = (HDROP) ENDrop->hDrop;
    wchar_t szFileName[_MAX_PATH];
    ::DragQueryFile((HDROP)hDropinfo, 0, szFileName, _MAX_PATH);
    OpenDoc(szFileName);
    ::DragFinish(hDropinfo);
    return TRUE;
}

/*============================================================================*/
    void CMainFrame::
InitCtlColors()                                                             /*

    Populate the color table with the initial ctl_color triplets used in the
    list box of the CColorDialog object. These values are displayed in the
    controls on first execution of this program, and are overwritten by
    deserialization in subsequent executions.
*-----------------------------------------------------------------------------*/
{
    m_colorChoice.AddColorChoice(DlgBg,     L"o  Dialog background",
        COLOR_LT_BLUE);
      // richedit controls (these are set differently than the others)
    m_colorChoice.AddColorChoice(REdTxFg,   L"o  RichEdit text foreground",
        COLOR_WHITE);
    m_colorChoice.AddColorChoice(REdTxBg,   L"o  RichEdit text background",
        COLOR_RED);
    m_colorChoice.AddColorChoice(REdBg,     L"o  RichEdit background",
        COLOR_LT_RED);
}

/*============================================================================*/
    void CMainFrame::
OnCloseDoc()                                                                /*

    Close the document and reset mainframe parameters accordingly.
*-----------------------------------------------------------------------------*/
{
    m_docDir = m_doc.GetDocDir(); // save for next file open/save
    m_doc.OnCloseDoc();
    SetWindowTitle(L"");
    GetRichView().Clean();
    m_view.NoDocOpen();
}

/*============================================================================*/
    void CMainFrame::
OnColorChoice()                                                             /*

    Show the CListBoxDlg color dialog box to select the color of a control's
    text foreground, text background, or control background. NOTE: colors
    selected for controls in the view are painted by the OnCtlColor()
    method invoked in the WndProc() loop each time the control is redrawn.
    The staus bar color has no message to redraw it, so it is set here.
*-----------------------------------------------------------------------------*/
{
    if (m_colorChoice.DoModal(*this) != IDOK)
        return;

    UINT selection = m_colorChoice.GetSelectedColorID();
      // reset the client background
    if (selection == DlgBg)
        SetViewBgColor();
      // reset the rich edit control color
    if (selection == REdTxFg || selection == REdTxBg || selection == REdBg)
        SetRichEditColor();

    Invalidate();
    UpdateWindow();
}

/*============================================================================*/
    BOOL CMainFrame::
OnCommand(WPARAM wparam, LPARAM)                                            /*

    The framework calls this member method when the user selects an
    item from a menu, when a child control sends a notification message,
    or when an accelerator keystroke is translated. Here, we respond to
    menu selections, toolbar events, scrollbar actions, and accelerator
    keys. The method returns nonzero if it processes the message; otherwise it
    returns zero.
*-----------------------------------------------------------------------------*/
{
    UINT id = LOWORD(wparam);

    switch(id)
    {
        case IDM_COLOR_CHOICE:    OnColorChoice();       return TRUE;
        case IDM_EDIT_COPY:       GetRichView().Copy();  return TRUE;
        case IDM_EDIT_CUT:        GetRichView().Cut();   return TRUE;
        case IDM_EDIT_DELETE:     GetRichView().Clear(); return TRUE;
        case IDM_EDIT_FIND:       OnEditFind();          return TRUE;
        case IDM_EDIT_PASTE:      GetRichView().Paste(); return TRUE;
        case IDM_EDIT_REPLACE:    OnEditReplace();       return TRUE;
        case IDM_EDIT_REDO:       GetRichView().Redo();  return TRUE;
        case IDM_EDIT_UNDO:       GetRichView().Undo();  return TRUE;
        case IDM_FILE_CLOSE:      OnCloseDoc();          return TRUE;
        case IDM_FILE_EXIT:       OnExit();              return TRUE;
        case IDM_FILE_NEW:        OnNewDoc();            return TRUE;
        case IDM_FILE_OPEN:       OnOpenDoc();           return TRUE;
        case IDM_FILE_PAGESETUP:  m_view.OnPageSetup();  return TRUE;
        case IDM_FILE_PRINT:      OnPrint();             return TRUE;
        case IDM_FILE_SAVE:       OnSave();              return TRUE;
        case IDM_FILE_SAVEAS:     OnSaveAs();            return TRUE;
        case IDM_FONT_CHOICE:     OnFontChoice();        return TRUE;
        case IDM_RICHEDWRAP:      OnWrapText();          return TRUE;
        case IDW_ABOUT:           OnHelp();              return TRUE;
        case IDW_VIEW_STATUSBAR:  OnViewStatusBar();     return TRUE;
        case IDW_VIEW_TOOLBAR:    OnViewToolBar();       return TRUE;

        case IDW_FILE_MRU_FILE1:
        case IDW_FILE_MRU_FILE2:
        case IDW_FILE_MRU_FILE3:
        case IDW_FILE_MRU_FILE4:
        case IDW_FILE_MRU_FILE5:    return OnFileMRU(wparam);
    }
    return FALSE;
}

/*============================================================================*/
    int CMainFrame::
OnCreate(CREATESTRUCT& rcs)                                                 /*

    This member controls the way the frame is created. Overriding
    it is optional.
*-----------------------------------------------------------------------------*/
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


      // call the base class OnCreate() method with these options
    int rtn = CFrame::OnCreate(rcs);
      // establish communications
    m_doc.SetDataPath(&m_view);
      // populate the initial control colors
    InitCtlColors();
      // and set the initial flags to show all colors
    CHOOSECOLOR cc = m_colorChoice.GetParameters();
    cc.Flags = CC_FULLOPEN;
    cc.Flags |= CC_ANYCOLOR | CC_RGBINIT | CC_ENABLEHOOK;
      // setup the CColorChoice object
    m_colorChoice.SetParameters(cc);
      // set the initial flags to use the font style,
    CHOOSEFONT cf = m_fontChoice.GetParameters();
    cf.Flags |= CF_USESTYLE;
    cf.lpszStyle = const_cast<LPWSTR>(L"Regular"); // initial font
    m_fontChoice.SetParameters(cf);
      // set the default font
    CFont f;
    f.CreatePointFont(100, L"Courier New");
    m_fontChoice.SetChoiceFont(f);
    m_fontChoice.SetColor(m_colorChoice.GetTableColor(REdTxFg));
    m_view.SetEditFont(f);
      // set the view background color
    SetViewBgColor();
      // set the edit box colors
    SetRichEditColor();
    return rtn;
}

/*============================================================================*/
    void CMainFrame::
OnEditFind()                                                                /*

    Initiate the find non-modal dialog box and the messages sent to the
    CDoc::OnFindReplace() method by the CMainFrame::WndProc() message loop.
*-----------------------------------------------------------------------------*/
{
    m_findReplaceDlg.SetBoxTitle(L"Find a string...");
    m_findReplaceDlg.Create(TRUE, L"Initial Text", L"", FR_DOWN |
        FR_ENABLEHOOK, *this);
}

/*============================================================================*/
    void CMainFrame::
OnEditReplace()                                                             /*

    Invoke the find-replace dialog and initiate the find non-modal dialog
    box and the messages sent to the CDoc::OnFindReplace() method by the
    the CMainFrame::WndProc() message loop.  Note: the replace dialog box
    does not have the direction up-down box that the find dialog box has.
    This is "by design."
*-----------------------------------------------------------------------------*/
{
    m_findReplaceDlg.SetBoxTitle(L"Find, then Replace");
    m_findReplaceDlg.Create(FALSE, L"Initial Text", L"Replace Text",
        FR_DOWN | FR_ENABLEHOOK, *this);
}

/*============================================================================*/
void CMainFrame::
    OnExit()                                                                /*

        Close the application.
    *-----------------------------------------------------------------------------*/
{
    m_docDir = m_doc.GetDocDir(); // save for next file open/save
    m_doc.OnCloseDoc();
    SetWindowTitle(L"");
    GetRichView().Clean();
    m_view.NoDocOpen();
    Close();
}

/*============================================================================*/
    BOOL CMainFrame::
OnFileMRU(WPARAM wparam)                                                    /*

        Open a file from the Most Recently Used (MRU) list.
*-----------------------------------------------------------------------------*/
{
    UINT mruIndex = static_cast<UINT>(LOWORD(wparam)) - IDW_FILE_MRU_FILE1;
    CString mruText = GetMRUEntry(mruIndex);

    if (!OpenDoc(mruText))
        RemoveMRUEntry(mruText);

    return TRUE;
}

/*============================================================================*/
    void CMainFrame::
OnFontChoice()                                                              /*

        Select the view font typeface, characteristics, and color.
*-----------------------------------------------------------------------------*/
{
    HWND hOwnerWnd = GetApp()->GetMainWnd();
      // open the dialog
    m_fontChoice.SetBoxTitle(L"Select font for rich edit box");
    LOGFONT lf;
    m_fontChoice.GetChoiceFont().GetObject(sizeof(LOGFONT), &lf);
    CHOOSEFONT cf = m_fontChoice.GetParameters();
    cf.Flags |= CF_SCREENFONTS | CF_EFFECTS | CF_INITTOLOGFONTSTRUCT;
    cf.lpLogFont = &lf;
    cf.rgbColors = m_colorChoice.GetTableColor(REdTxFg);
    m_fontChoice.SetParameters(cf);
    if(m_fontChoice.DoModal(hOwnerWnd))
    {
          // bring choice elements into this view
        m_view.SetEditFont(m_fontChoice.GetChoiceFont());
        m_colorChoice.SetTableColor(REdTxFg, m_fontChoice.GetColor());
          // reset the rich edit control color
        SetRichEditColor();
    }
    Invalidate();
    UpdateWindow();
}

/*============================================================================*/
    BOOL CMainFrame::
OnHelp()                                                                    /*

    Overrides CFrame OnHelp(). Display a modeless dialog bearing the program's
    application information.
*-----------------------------------------------------------------------------*/
{
      // Ensure only one dialog displays even for multiple activation commands
    if (!m_aboutBox.IsWindow())
        m_aboutBox.Create(*this);  // make the AboutBox modeless

    return TRUE;
}

/*============================================================================*/
    void CMainFrame::
OnInitialUpdate()                                                           /*

    The frame has now been created. Provide any desired main frame
    formatting.
*-----------------------------------------------------------------------------*/
{
      // Unselect the text
    GetRichView().SetSel(0, 0);
    GetRichView().SetFocus();

    TRACE("Frame created\n");
}

/*============================================================================*/
    void CMainFrame::
OnMenuUpdate(UINT id)                                                       /*

    Update the enable and chack status of menu items.
*-----------------------------------------------------------------------------*/
{
    UINT enabled;
    UINT checked;

    switch (id)
    {
    case IDM_FILE_SAVE:
        enabled = m_doc.IsDirty() ? MF_ENABLED : MF_GRAYED;
        GetFrameMenu().EnableMenuItem(id, enabled);
        break;
    case IDM_FILE_SAVEAS:
        enabled = m_doc.IsOpen() ? MF_ENABLED : MF_GRAYED;
        GetFrameMenu().EnableMenuItem(id, enabled);
        break;
    case IDM_RICHEDWRAP:
        checked = m_isTextWrap ? MF_CHECKED : MF_UNCHECKED;
        GetFrameMenu().CheckMenuItem(id, checked);
        break;
    }

    return CFrame::OnMenuUpdate(id);
}

/*============================================================================*/
    void CMainFrame::
OnNewDoc()                                                                  /*

    Prompt the user for a new document file name and, if valid, open a new
    document. The open status and path name are available from the document
    object.
*-----------------------------------------------------------------------------*/
{
    MyFileDialog fd
    (   TRUE,
        m_docExt,
        m_docDir,               // a previous directory
        OFN_HIDEREADONLY    |
        OFN_EXPLORER        |
        OFN_NONETWORKBUTTON |
        OFN_ENABLESIZING,
        m_docFilter
    );
    fd.SetBoxTitle(L"New document file...");
    fd.SetDefExt(m_docFilter);
    CString msg;
      // do not leave without a valid unused file name, unless cancelled
    while (fd.DoModal(GetApp()->GetMainWnd()) == IDOK)
    {
        CString selected = fd.GetPathName();
          // new_path should not exist
        if (::_taccess(selected, 0x04) != 0)
        {
              // make the selected document
            if (m_doc.MakeNewDoc(selected))
                OpenDoc(selected);
            return;
        }
          // prompt the user to try again
        msg.Format(L"That document file\n    '%s'\n"
            L"already exists.", selected.c_str());
        ::MessageBox(nullptr, msg, L"Error", MB_OK |
            MB_ICONERROR | MB_TASKMODAL);
    }
    msg = L"No name was entered, no action was taken.";
    ::MessageBox(nullptr, msg, L"Information",
        MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
    return;
}

/*============================================================================*/
    LRESULT CMainFrame::
OnNotify(WPARAM wparam, LPARAM lparam)                                      /*

    Process messages that controls send to the parent.
*-----------------------------------------------------------------------------*/
{
    NMHDR* pNMH = reinterpret_cast<LPNMHDR>(lparam);
    switch (pNMH->code)
    {
        case EN_DROPFILES:
            DropFiles(lparam);
            return TRUE;
    }
    return CFrame::OnNotify(wparam, lparam);
}

/*============================================================================*/
    void CMainFrame::
OnOpenDoc()                                                            /*

    Display the open file dialog to obtain the path of an existing document
    file. If one is selected, open it. The open status and path name are
    available from the document object.
*-----------------------------------------------------------------------------*/
{
    MyFileDialog fd
    (   TRUE,                   // open file dialog
        m_docExt,               // extension defined by app
        m_docDir,               // a previous directory
        OFN_HIDEREADONLY    |   // flags
        OFN_EXPLORER        |
        OFN_NONETWORKBUTTON |
        OFN_FILEMUSTEXIST   |  // only existing files allowed
        OFN_PATHMUSTEXIST   |
        OFN_ENABLEHOOK      |
        OFN_ENABLESIZING,
        m_docFilter
    );
    fd.SetBoxTitle(L"Open document file...");
    fd.SetDefExt(m_docExt);
    CString msg;
    if (fd.DoModal(GetApp()->GetMainWnd()) == IDOK)
    {
        CString selected = fd.GetPathName();
        if (OpenDoc(selected))
            return;
    }

    msg = "No valid name was entered, no action was taken.";
    ::MessageBox(nullptr, msg, L"Information",
        MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
    return;
}

/*============================================================================*/
    BOOL CMainFrame::
SetRichEditColor()                                                           /*

    Set the rich edit control colors to those found in the color table.
*-----------------------------------------------------------------------------*/
{
    COLORREF txfg = m_colorChoice.GetTableColor(REdTxFg);
    COLORREF txbg = m_colorChoice.GetTableColor(REdTxBg);
    COLORREF bg   = m_colorChoice.GetTableColor(REdBg);
    m_view.SetRichEditColors(txfg, txbg, bg);
    return TRUE;
}

/*============================================================================*/
void CMainFrame::
    OnPrint()                                                                /*

    Invoke a MyPrintDialog dialog to get printing parameters and then print
    the contents of the rich view control.
*-----------------------------------------------------------------------------*/
{
    m_view.OnPrintDocument(m_doc.GetDocPath());
}

/*============================================================================*/
void CMainFrame::
    OnSave()                                                                  /*

    Save the current document.
*-----------------------------------------------------------------------------*/
{
    m_doc.OnSaveDoc();
}

/*============================================================================*/
    void CMainFrame::
OnSaveAs()                                                                  /*

    Save the current document into a file named in a file dialog and make
    that file the current document.
*-----------------------------------------------------------------------------*/
{
    if (!m_doc.IsOpen())
        return;

      // declare the file dialog box
    MyFileDialog fd
    (   FALSE,
        m_docExt,               // extension defined by app
        m_docDir,               // current directory
        OFN_HIDEREADONLY |
        OFN_OVERWRITEPROMPT |
        OFN_EXPLORER |
        OFN_ENABLEHOOK |
        OFN_NONETWORKBUTTON,
        m_docFilter             // filter defined by app
     );
    fd.SetBoxTitle(L"Save document file as");
    CString msg;
      // query user for the save-as file path name
    if (fd.DoModal(GetApp()->GetMainWnd()) == IDOK)
    {    // At this point, a file path has been chosen that is
         // not empty and if it already exists has been approved by the
         // user to be overwritten. Fetch the path from the dialog.
        CString selected = fd.GetPathName();
          // check if the input path is the one already open
        if (selected.CompareNoCase(m_doc.GetDocPath()) == 0)
        {     // the named paths are the same
            msg.Format(L"Document file\n    '%s'\n is already "
                L"open. No action taken", m_doc.GetDocPath().c_str());
            ::MessageBox(nullptr, msg, L"Information",
                MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
            return;
        }
          // the selected path is ok: save and close the current document
        CString current_doc = m_doc.GetDocPath();
        OnCloseDoc();
          // copy it to the selected path
        CopyFile(current_doc, selected, FALSE);
          // open it for operations
        if (!OpenDoc(selected))
        {
            msg.Format(L"Saved document file\n    '%s'"
                L" could not be reopened.", selected.c_str());
            ::MessageBox(nullptr, msg, L"Information",
                MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
              // reopen the current file at entry
            OpenDoc(current_doc);
            return;
        }
    }
    msg = "No name was entered, no action was taken.";
    ::MessageBox(nullptr, msg, L"Information",
        MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
}

/*============================================================================*/
    void CMainFrame::
OnWrapText()                                                                /*

    Toggle the word wrapping mode in the rich edit control.
*-----------------------------------------------------------------------------*/
{
    m_isTextWrap = !m_isTextWrap;
    GetRichView().SetWrapping(m_isTextWrap);
}

/*============================================================================*/
    BOOL CMainFrame::
OpenDoc(LPCWSTR docPath)                                             /*

    Open the document from the given path. Return TRUE if successful, FALSE
    otherwise.
*-----------------------------------------------------------------------------*/
{
   if (CString(docPath).CompareNoCase(m_doc.GetDocPath()) == 0)
    {
        CString msg;
        msg.Format(L"Document file\n    '%s'\nis already open.", docPath);
        ::MessageBox(nullptr, msg, L"Information", MB_OK |
            MB_ICONINFORMATION | MB_TASKMODAL);
          // not deemed a failure, as the file is open, as specified
        return TRUE;
    }
        // if there is currently a document open, close it
    if (m_doc.IsOpen())
        OnCloseDoc();
      // open the document based on this name
    if (m_doc.OpenDoc(docPath))
    {
        m_docDir = m_doc.GetDocPath();
        SetWindowTitle(docPath);
        AddMRUEntry(docPath);
        return TRUE;
    }
    return FALSE;
}

/*============================================================================*/
    void CMainFrame::
SetReBarColors(COLORREF clrBkGnd1, COLORREF clrBkGnd2, COLORREF clrBand1,
    COLORREF clrBand2)                                                      /*

    Set the colors to be used in the rebar theme.
*-----------------------------------------------------------------------------*/
{
    ReBarTheme rt{};
    rt.UseThemes    = TRUE;
    rt.clrBkgnd1    = clrBkGnd1;
    rt.clrBkgnd2    = clrBkGnd2;
    rt.clrBand1     = clrBand1;
    rt.clrBand2     = clrBand2;
    rt.FlatStyle    = FALSE;
    rt.BandsLeft    = TRUE;
    rt.LockMenuBand = TRUE;
    rt.RoundBorders = TRUE;
    rt.ShortBands   = TRUE;
    rt.UseLines     = TRUE;

    SetReBarTheme(rt);
}

/*============================================================================*/
    void CMainFrame::
SetupMenuIcons()                                                             /*

    Assign icons to the dropdown menu items. In particular, add the AboutBox
    identifier to the menu icon list.
*-----------------------------------------------------------------------------*/
{
     // Add IDW_ABOUT to the menu data
    std::vector<UINT> data = GetToolBarData();
    data.push_back(IDW_ABOUT);

    // Specify the bitmap and mask for the menu icons.
    if (GetMenuIconHeight() >= 24)
        SetMenuIcons(data, RGB(255, 0, 255), IDW_MAIN);
    else
        SetMenuIcons(data, RGB(192, 192, 192), IDB_MENUICONS);
}

/*============================================================================*/
    void CMainFrame::
SetupToolBar()                                                              /*

    Called from the CFrame::CreateToolBar() method to load the toolbar
    bitmaps, to connect the tool bar buttons to resource IDs of the
    toolbar buttons, and to define the order of appearance of the buttons
    on the toolbar at runtime.
*-----------------------------------------------------------------------------*/
{
      // Connect button IDs to button icons, show enabled status, and
      // give the explicit image index iImage of each button in the bitmap.
      // Add the toolbar buttons in the order they are to appear at runtime.
    AddToolBarButton(IDM_FILE_NEW,      TRUE, 0, 0);
    AddToolBarButton(IDM_FILE_OPEN,     TRUE, 0, 1);
    AddToolBarButton(IDM_FILE_SAVE,     TRUE, 0, 2);
    AddToolBarButton(0);  // Separator
    AddToolBarButton(IDM_EDIT_CUT,      TRUE, 0, 3);
    AddToolBarButton(IDM_EDIT_COPY,     TRUE, 0, 4);
    AddToolBarButton(IDM_EDIT_PASTE,    TRUE, 0, 5);
    AddToolBarButton(0);  // Separator
    AddToolBarButton( IDM_EDIT_FIND,    TRUE, 0, 6);
    AddToolBarButton( IDM_EDIT_REPLACE, TRUE, 0, 7);
    AddToolBarButton(0);  // Separator
    AddToolBarButton(IDM_FILE_PRINT,    TRUE, 0, 8);
    AddToolBarButton(0);  // Separator
    AddToolBarButton(IDM_COLOR_CHOICE,  TRUE, 0, 9);
    AddToolBarButton(IDM_FONT_CHOICE,   TRUE, 0, 10);
      // Set the toolbar image list: use defaults for hot and disabled
    SetToolBarImages(RGB(255, 0, 255), IDW_MAIN, 0, 0);
}

/*============================================================================*/
void CMainFrame::
    SetViewBgColor()                                                        /*

    Sets the background color of the dialog used as the view window.
*-----------------------------------------------------------------------------*/
{
    m_view.SetBgColor(m_colorChoice.GetBrush(DlgBg));
}

/*============================================================================*/
    void CMainFrame::
SetWindowTitle(LPCWSTR path)                                                /*

    Put the app title and docPath name in the main window text. Limit the
    displayed text length to that of the frame.
*-----------------------------------------------------------------------------*/
{
    CString pathname = path;
    CString title = LoadString(IDW_MAIN);

    if (!pathname.IsEmpty())
        title += " - " + pathname;

    SetWindowText(title);
}

/*============================================================================*/
    void CMainFrame::
UpdateToolbar()                                                             /*

Called by CApp::OnIdle to update toolbar buttons
*-----------------------------------------------------------------------------*/
{
    BOOL isSelected = m_doc.IsSelected();
    BOOL canPaste = m_doc.CanPaste();
    BOOL isDirty = m_doc.IsDirty();

    GetToolBar().EnableButton(IDM_FILE_SAVE, isDirty);
    GetToolBar().EnableButton(IDM_EDIT_COPY, isSelected);
    GetToolBar().EnableButton(IDM_EDIT_CUT, isSelected);
    GetToolBar().EnableButton(IDM_EDIT_PASTE, canPaste);
}

/*============================================================================*/
    LRESULT CMainFrame::
WndProc(UINT msg, WPARAM wparam, LPARAM lparam)                            /*

    This is the main frame message loop. By default, it handles the
    normal housekeeping functions (see Win32++\include\frame.h).
*-----------------------------------------------------------------------------*/
{
    try
    {
          // can't use a case statement on this ::RegisterWindowMessage() value
        if (msg == UWM_FINDMSGSTRING)
        {
            m_doc.OnFindReplace(msg, wparam, lparam);
            return TRUE;
        }

        //switch (msg)
        //{
        //    TODO: add message handlers here
        //}

          // pass unhandled messages on for default processing
        return WndProcDefault(msg, wparam, lparam);
    }
      // Catch all CException types.
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
/*----------------------------------------------------------------------------*/
