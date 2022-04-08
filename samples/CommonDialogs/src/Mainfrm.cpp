/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]              (MainFrm.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CMainFrame class for the
    CommonDialogs demonstration sample application using the Win32++ Windows
    interface classes. This particular frame class contains features a
    fixed-size form for the display, with no resizing gripper tool at the end
    of the status bar, and provisions for selection of client background color,
    selection of edit box font, use of external serialization files, and MRU
    lists.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"
#include <io.h>

#if defined (_MSC_VER) && (_MSC_VER >= 1920)      // VS2019 or higher
#pragma warning (disable : 26812 )  // allow unscoped enums
#endif

/*============================================================================*/
    CMainFrame::
CMainFrame()                                                                /*

    Construct and initialize the CMainFrame object from the IDD_MAIN_DIALOG
    resource defined in resource.rc. Note that the initial window location and
    size are set here.
*-----------------------------------------------------------------------------*/
    :   m_view(IDD_MAIN_DIALOG), m_maxMRU(0), m_isTextWrap(TRUE),
        m_frameXY(100, 100), m_frameSize(700, 600)
{
}

/*============================================================================*/
    HWND CMainFrame::
Create(HWND parent)                                                         /*

    Create the frame window.
*-----------------------------------------------------------------------------*/
{
      // Set m_view as the view window of the frame
    SetView(m_view);

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
    TCHAR szFileName[_MAX_PATH];
    ::DragQueryFile((HDROP)hDropinfo, 0, szFileName, _MAX_PATH);
    OpenDoc(szFileName);
    ::DragFinish(hDropinfo);
    return TRUE;
}

/*============================================================================*/
    void CMainFrame::
EmptyMRUList()                                                              /*

    Remove all entries from the MRU list and  display the empty list
    on the MRU menu.
*-----------------------------------------------------------------------------*/
{
      // use a separate list to get the entries
    const std::vector<CString>& MRUEntries = GetMRUEntries();
      // then remove items from the MRU list one by one
    std::vector<CString>::const_iterator it;
    for (it = MRUEntries.begin(); it != MRUEntries.end(); ++it)
        RemoveMRUEntry(*it);
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
    m_colorChoice.AddColorChoice(DlgBg,     _T("o  Dialog background"),
        COLOR_LT_BLUE);
    m_colorChoice.AddColorChoice(SBBg,      _T("o  Status bar background"),
        GetStatusBarTheme().clrBkgnd1);
      // richedit controls (these are set differently than the others)
    m_colorChoice.AddColorChoice(REdTxFg,   _T("o  RichEdit text foreground"),
        COLOR_WHITE);
    m_colorChoice.AddColorChoice(REdTxBg,   _T("o  RichEdit text background"),
        COLOR_RED);
    m_colorChoice.AddColorChoice(REdBg,     _T("o  RichEdit background"),
        COLOR_LT_RED);
}

/*============================================================================*/
    void    CMainFrame::
LoadPersistentData()                                                        /*

    Recover app, mainframe, MRU, and view serialized data from previous
    execution. MRU strings that are not valid file paths are discarded.
*-----------------------------------------------------------------------------*/
{
      // determine the availability of the archive file
    if (_taccess(m_archivePath, 0x04) != 0)
    {
        CString msg = _T("Default values are being used on this first\n")
            _T("startup. Your customized settings, colors, and font\n")
            _T("will be restored in future usages.\n");
        ::MessageBox(NULL, msg, _T("Information"), MB_OK |
            MB_ICONINFORMATION | MB_TASKMODAL);
        return;
    }
    try
    {
        CArchive ar(m_archivePath, CArchive::load);
              // deserialize in the same order as serialized
        ar >> *TheApp();  // for the app
        ar >> *this;    // for the mainframe and base classes
        ar >> m_view;   // for the view, including control colors
          // the ar object closes on destruction
    }
    catch(...) // catch all exceptions in trying to load the archive
    {
        CString msg = _T("Previous settings could not be restored.\n")
            _T("Unable to read archived values.\n");
        ::MessageBox(NULL, msg, _T("Exception"), MB_OK | MB_ICONSTOP |
            MB_TASKMODAL);
    }

    Invalidate();   // repaint the client with recovered colors

      // weed out any MRU entries that have disappeared
    ValidateMRU(); // remove invalid file path names
}

/*============================================================================*/
    void CMainFrame::
OnCloseDoc()                                                                /*

    Close the document and reset mainframe parameters accordingly.
*-----------------------------------------------------------------------------*/
{
    m_docDir = m_doc.GetDocDir(); // save for next file open/save
    m_doc.OnCloseDoc();
    SetWindowTitle(_T(""));
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

      // reset the status bar color
    UINT selection = m_colorChoice.GetSelectedColorID();
    if (selection == SBBg)
    {
        COLORREF sb = GetSBBkColor();
        SetSBBkColor(sb);
    }
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
OnCommand(WPARAM wparam, LPARAM lparam)                                     /*

    The framework calls this member method when the user selects an
    item from a menu, when a child control sends a notification message,
    or when an accelerator keystroke is translated. Here, we respond to
    menu selections, toolbar events, scrollbar actions, and accelerator
    keys. The method returns nonzero if it processes the message; otherwise it
    returns zero.
*-----------------------------------------------------------------------------*/
{
    UINT id = LOWORD(wparam);

      // map all MRU file messages to one representative
    if(IDW_FILE_MRU_FILE1 <= id && id < IDW_FILE_MRU_FILE1 + m_maxMRU)
        id = IDW_FILE_MRU_FILE1;

    switch(id)
    {
        case IDM_COLOR_CHOICE:
            OnColorChoice();
            return TRUE;

        case IDM_EDIT_COPY:
            GetRichView().Copy();
            return TRUE;

        case IDM_EDIT_CUT:
            GetRichView().Cut();
            return TRUE;

        case IDM_EDIT_DELETE:
            GetRichView().Clear();
            return TRUE;

        case IDM_EDIT_FIND:
            OnEditFind();
            return TRUE;

        case IDM_EDIT_PASTE:
            GetRichView().Paste();
            return TRUE;

        case IDM_EDIT_REPLACE:
            OnEditReplace();
            return TRUE;

        case IDM_EDIT_REDO:
            GetRichView().Redo();
            return TRUE;

        case IDM_EDIT_UNDO:
            GetRichView().Undo();
            return TRUE;

        case IDM_FILE_CLOSE:
            OnCloseDoc();
            return TRUE;

        case IDM_FILE_EXIT:
            OnCloseDoc();
            Close();
            return TRUE;

        case IDM_FILE_NEW:
            OnNewDoc();
            return TRUE;

        case IDM_FILE_OPEN:
            OnOpenDoc();
            return TRUE;

        case IDM_FILE_PAGESETUP:
            m_view.OnPageSetup();
            return TRUE;

        case IDM_FILE_PRINT:
            m_view.OnPrintDocument(m_doc.GetDocPath());
            return TRUE;

        case IDM_FILE_SAVE:
            m_doc.OnSaveDoc();
            return TRUE;

        case IDM_FILE_SAVEAS:
            OnSaveAs();
            return TRUE;

        case IDM_FONT_CHOICE:
            OnFontChoice();
            return TRUE;

        case IDM_RICHEDWRAP:
            OnWrapText();
            return TRUE;

        case IDW_ABOUT:         // invoked by F1 and Help->About menu item
            OnHelp();
            return TRUE;

        case IDW_VIEW_STATUSBAR:
            OnViewStatusBar(); // toggle status bar
            return TRUE;

        case IDW_VIEW_TOOLBAR:
            OnViewToolBar(); // toggle tool bar
            return TRUE;

        case IDW_FILE_MRU_FILE1:
            OnProcessMRU(wparam, lparam);
            return TRUE;

        default:
            break;
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

    // UseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar
    // UseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar
    // UseReBar(FALSE);              // Don't use a ReBar
    // UseStatusBar(FALSE);          // Don't use a StatusBar
    // UseThemes(FALSE);             // Don't use themes
    // UseToolBar(FALSE);            // Don't use a ToolBar


      // call the base class OnCreate() method with these options
    int rtn = CFrame::OnCreate(rcs);
      // set theme colors, if supported
    if (IsReBarSupported())
        SetThemeColors();  //Set the theme colors
      // establish communications
    m_doc.SetDataPath(&m_view);
      // populate the initial control colors (will be overwritten by
      // deserialized values)
    InitCtlColors();
      // and set the initial flags to show all colors
    CHOOSECOLOR cc = m_colorChoice.GetParameters();
    cc.Flags = CC_FULLOPEN;
    cc.Flags |= CC_ANYCOLOR | CC_RGBINIT | CC_ENABLEHOOK;
      // setup the CColorChoice object
    m_colorChoice.SetParameters(cc);
      // tell CFrame the max MRU size
    SetMRULimit(m_maxMRU);
      // set the initial flags to use the font style,
    CHOOSEFONT cf = m_fontChoice.GetParameters();
    cf.Flags |= CF_USESTYLE;
    cf.lpszStyle = const_cast<LPTSTR>(_T("Regular")); // initial font
    m_fontChoice.SetParameters(cf);
      // set the default font
    CFont f;
    f.CreatePointFont(100, _T("Courier New"));
    m_fontChoice.SetChoiceFont(f);
    m_fontChoice.SetColor(m_colorChoice.GetTableColor(REdTxFg));
    m_view.SetEditFont(f);
    LoadPersistentData();
      // set the default status bar color
    COLORREF sb = GetSBBkColor();
    SetSBBkColor(sb);
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
    m_findReplaceDlg.SetBoxTitle(_T("Find a string..."));
    m_findReplaceDlg.Create(TRUE, _T("Initial Text"), _T(""), FR_DOWN |
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
    m_findReplaceDlg.SetBoxTitle(_T("Find, then Replace"));
    m_findReplaceDlg.Create(FALSE, _T("Initial Text"), _T("Replace Text"),
        FR_DOWN | FR_ENABLEHOOK, *this);
}

/*============================================================================*/
    void CMainFrame::
OnFileOpenMRU(UINT nIndex)                                                  /*

    Open the MRU file at nIndex as the next document.
*-----------------------------------------------------------------------------*/
{
      // get the MRU entry
    CString mru_entry = GetMRUEntry(nIndex);
    OpenDoc(mru_entry);
}

/*============================================================================*/
    void CMainFrame::
OnFontChoice()                                                              /*

        Select the view font typeface, characteristics, and color.
*-----------------------------------------------------------------------------*/
{
    HWND hOwnerWnd = GetApp()->GetMainWnd();
      // open the dialog
    m_fontChoice.SetBoxTitle(_T("Select font for rich edit box"));
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
    CRect framePos = CRect(m_frameXY, m_frameSize);
    SetWindowPos(HWND_TOP, framePos, SWP_SHOWWINDOW);
      // the status bar cannot be recolored if XP themes are on, so
    GetStatusBar().SetWindowTheme(L" ", L" ");  // turn XP themes off
      // enable drag-and-drop file entry mode
    DragAcceptFiles(TRUE);
      // Resize the rich text window within the client window
    CRect rc = m_view.GetClientRect();
    rc.DeflateRect(10, 10);
    GetRichView().SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
      // if there is a MRU item at the top of the list, use it
      // as the name of the document to open
    CString openPath = GetMRUEntry(0);
    CString msg;
    msg.Format(_T("Open previous document?\n    %s"), openPath.c_str());
    if (!openPath.IsEmpty() &&  (::MessageBox(NULL, msg, _T("Question..."),
      MB_YESNO | MB_ICONQUESTION) == IDYES))
        OpenDoc(openPath);

      // Unselect the text
    GetRichView().SetSel(0, 0);
    GetRichView().SetFocus();
    TRACE("Frame created\n");
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
    fd.SetBoxTitle(_T("New document file..."));
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
        msg.Format(_T("That document file\n    '%s'\n")
            _T("already exists."), selected.c_str());
        ::MessageBox(NULL, msg, _T("Error"), MB_OK |
            MB_ICONERROR | MB_TASKMODAL);
    }
    msg = _T("No name was entered, no action was taken.");
    ::MessageBox(NULL, msg, _T("Information"),
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
    fd.SetBoxTitle(_T("Open document file..."));
    fd.SetDefExt(m_docExt);
    CString msg;
    if (fd.DoModal(GetApp()->GetMainWnd()) == IDOK)
    {
        CString selected = fd.GetPathName();
        if (OpenDoc(selected))
            return;
    }

    msg = _T("No valid name was entered, no action was taken.");
    ::MessageBox(NULL, msg, _T("Information"),
        MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
    return;
}

/*============================================================================*/
    BOOL CMainFrame::
OnProcessMRU(WPARAM wparam, LPARAM lparam)                                  /*

    One of the MRU entries has been selected.  Process accordingly.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(lparam);

      // compute the MRU index, where IDW_FILE_MRU_FILE1 is index 0
    UINT nMRUIndex = LOWORD(wparam) - IDW_FILE_MRU_FILE1;
    OnFileOpenMRU(nMRUIndex);
    return TRUE;
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
OnSaveAs()                                                                  /*

    Save the current document into a file named in a file dialog and make
    that file the current document. Return TRUE and set the path if the
    document was saved, FALSE otherwise.
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
    fd.SetBoxTitle(_T("Save document file as"));
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
            msg.Format(_T("Document file\n    '%s'\n is already ")
                _T("open. No action taken"), m_doc.GetDocPath().c_str());
            ::MessageBox(NULL, msg, _T("Information"),
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
            msg.Format(_T("Saved document file\n    '%s'")
                _T(" could not be reopened."), selected.c_str());
            ::MessageBox(NULL, msg, _T("Information"),
                MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
              // reopen the current file at entry
            OpenDoc(current_doc);
            return;
        }
    }
    msg = _T("No name was entered, no action was taken.");
    ::MessageBox(NULL, msg, _T("Information"),
        MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
}

/*============================================================================*/
    void CMainFrame::
OnWrapText()                                                                /*

    Description
*-----------------------------------------------------------------------------*/
{
    m_isTextWrap = !m_isTextWrap;
    GetRichView().SetWrapping(m_isTextWrap);
}

/*============================================================================*/
    BOOL CMainFrame::
OpenDoc(LPCTSTR docPath)                                             /*

    Open the document from the given path. Return TRUE if successful, FALSE
    otherwise.
*-----------------------------------------------------------------------------*/
{
   if (CString(docPath).CompareNoCase(m_doc.GetDocPath()) == 0)
    {
        CString msg;
        msg.Format(_T("Document file\n    '%s'\nis already open."), docPath);
        ::MessageBox(NULL, msg, _T("Information"), MB_OK |
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
        AddMRUEntry(docPath);
        SetWindowTitle(docPath);
        return TRUE;
    }
    return FALSE;
}

/*============================================================================*/
    void CMainFrame::
PreCreate(CREATESTRUCT& cs)                                                 /*

    Set cs members to select window frame parameters desired. This gets
    executed before CView::PreCreate(). Use the serialized position
    and size information to set the frame positioning and size.
*-----------------------------------------------------------------------------*/
{
      // do the base class stuff
    CFrame::PreCreate(cs);
      // specify a title bar and border with a window-menu on the title bar
    cs.style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE
        | WS_MINIMIZEBOX    // adds the minimize box
        | WS_MAXIMIZEBOX    // adds the maximize box
//      | WS_THICKFRAME     // same as WS_SIZEBOX, enables resizing
        ;
    cs.dwExStyle = WS_EX_CLIENTEDGE
        | WS_EX_CONTROLPARENT       // TAB key navigation
//      | WS_EX_CONTEXTHELP     // doesn't work if WS_MINIMIZEBOX
                    // or WS_MAXIMIZEBOX is specified
        ;
}

/*============================================================================*/
    BOOL CMainFrame::
SaveRegistrySettings()                                                      /*

    This member is called in response to a WM_CLOSE message for the
    frame.  The base class saves settings into the registry. In this
    override, the archive member is called to save everything into
    archive files.
*-----------------------------------------------------------------------------*/
{
    try
    {
        CArchive ar(m_archivePath, CArchive::store);
             // serialize in the following order
        ar << *TheApp();   // for the App
        ar << *this;    // for the mainframe and base classes
        ar << m_view;   // for the view, including control colors
    }
    catch (const CWinException &e)  // catch all std::exception events
    {
          // Process the exception and quit
        CString msg;
        msg.Format(_T("%s\n%s\n%s"),
            _T("Program settings could not be saved."),
            e.GetErrorString(), e.what());
        ::MessageBox(NULL, msg, _T("Exception"), MB_OK | MB_ICONSTOP |
            MB_TASKMODAL);
    }
    catch(...)
    {
        CString msg = _T("\nProgram settings could not be saved.\n    ");
        ::MessageBox(NULL, msg, _T("Unregistered Exception"), MB_OK |
            MB_ICONSTOP | MB_TASKMODAL);
    }
    return TRUE;
}

/*============================================================================*/
        void CMainFrame::
Serialize(CArchive &ar)                                                     /*

        Called serialize or deserialize the frame to and from the archive ar,
        depending on the sense of IsStoring().  Leave the archive open for
        further serializations.
*-----------------------------------------------------------------------------*/
{
        // perform loading or storing
    if (ar.IsStoring())
    {     // save current window placement information
        m_frameXY = GetWindowRect().TopLeft();
        ar << m_frameXY;
          // save the base class frame status and tool bar switches:
          // these control the display of the StatusBar and ToolBar
        BOOL showbar = GetStatusBar().IsWindowVisible();
        ar << showbar;
        showbar = GetToolBar().IsWindowVisible();
        ar << showbar;
          // save font parameters
        ar << m_fontChoice;
          // save regular control class colors
        ar << m_colorChoice;
        ar << m_isTextWrap;
          // record the number of MRU entries to write
        size_t i, nMRU = GetMRUSize();
        ar << nMRU;
          // save this many entries (don't use a copied list)
        for (i = 0; i < nMRU; ++i)
            ar << GetMRUEntries()[i];
    }
    else
    {     // recover window frame placement
        ar >> m_frameXY;
        CRect framePos = CRect(m_frameXY, m_frameSize);
        SetWindowPos(HWND_TOP, framePos, SWP_SHOWWINDOW);
          // recover frame status and  tool bar base class switches
        BOOL showbar;
        ar >> showbar;
        ShowStatusBar(showbar);
        ar >> showbar;
        ShowToolBar(showbar);
          // recover font parameters
        ar >> m_fontChoice;
        m_view.SetFont(m_fontChoice.GetChoiceFont(), TRUE);
            // set font elements into view
        m_view.SetEditFont(m_fontChoice.GetChoiceFont());
        m_colorChoice.SetTableColor(REdTxFg, m_fontChoice.GetColor());
          // recover colors
        ar >> m_colorChoice;
          // reset the status bar color
        COLORREF sb = GetSBBkColor();
        SetSBBkColor(sb);
        ar >> m_isTextWrap;
        GetRichView().SetWrapping(m_isTextWrap);
          // read MRU values from archive
        EmptyMRUList();
          // use dummy vector in case an exception occurs
        std::vector<CString> vMRUEntries;
          // extract all the MRU entries that were archived to preserve
          // the archive for subsequent use
        size_t i, nMRU;
        ar >> nMRU; // the number of entries to read in
        CString s;
        for (i = 0; i < nMRU; ++i)
        {
            ar >> s;
            if (i < m_maxMRU)  // keep only those within the limit
                vMRUEntries.push_back(s);
        }
          // all successfully read in, so store them LIFO order into
          // the MRU list for proper display
        std::vector<CString>::reverse_iterator it;
        for (it = vMRUEntries.rbegin(); it != vMRUEntries.rend(); ++it)
        {
            AddMRUEntry(*it);
        }
    }
}

/*============================================================================*/
    BOOL CMainFrame::
SetCheckStatus(UINT id, BOOL bCheck, ControlBars where)                    /*

*-----------------------------------------------------------------------------*/
{
    BOOL ok = FALSE;
    if (where == mainmenu || where == both)
    {
        if (bCheck > 1)
            ok = (GetFrameMenu().EnableMenuItem(id, MF_BYCOMMAND |
                MF_GRAYED) != 0xFFFFFFFF);
        else
            ok = (GetFrameMenu().CheckMenuItem(id, MF_BYCOMMAND |
                (bCheck ? MF_CHECKED : MF_UNCHECKED)) != 0xFFFFFFFF);
    }

    if (!ok)
        ok = TRUE;
    if (where == mainmenu || where == both)
    {
        if (bCheck > 1)
            ok = GetToolBar().SetButtonState(id, TBSTATE_INDETERMINATE);
        else
        {
            GetToolBar().CheckButton(id, bCheck);
            ok = TRUE;
        }
    }
    return ok;
}

/*============================================================================*/
    BOOL CMainFrame::
SetEnableStatus(UINT id, BOOL status, ControlBars which)                   /*

    Set the control having the given id to the enable/disable
    status, on the toolbar, main menu, or both, as indicated by which.
*-----------------------------------------------------------------------------*/
{
    UINT action = MF_BYCOMMAND | (status ? MF_ENABLED : MF_GRAYED);
    if (which == mainmenu)
        return (GetFrameMenu().EnableMenuItem(id, action) != 0xFFFFFFFF);
    else if (which == toolbar)
        return (status ? GetToolBar().EnableButton(id) :
            GetToolBar().DisableButton(id));
      // else must be both
    BOOL rtn = (GetFrameMenu().EnableMenuItem(id, action) != 0xFFFFFFFF);
    rtn |= (status ? GetToolBar().EnableButton(id) :
        GetToolBar().DisableButton(id));
    return rtn;
}

/*============================================================================*/
    void CMainFrame::
SetReBarColors(COLORREF clrBkGnd1, COLORREF clrBkGnd2, COLORREF clrBand1,
    COLORREF clrBand2)                                                      /*

    Set the colors to be used in the rebar theme.
*-----------------------------------------------------------------------------*/
{
    if (IsReBarSupported())
    {
        ReBarTheme rt;
        ZeroMemory(&rt, sizeof(rt));
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
}

/*============================================================================*/
     void CMainFrame::
SetStatusbarMsg(CString status)                                             /*

    Write the status message on the status bar.
*-----------------------------------------------------------------------------*/
{
    SetStatusText(status);
}

/*============================================================================*/
    BOOL CMainFrame::
SetThemeColors()                                                            /*

    Set the colors of each rebar in the frame, i.e., the theme colors.
    These were taken from the Win32++ Themes sample program recommended
    colors for Windows XP, case IDM_BLUE.
*-----------------------------------------------------------------------------*/
{
    SetReBarColors(RGB(150, 190, 245), RGB(196, 215, 250),
        RGB(220, 230, 250), RGB( 70,130,220));

    StatusBarTheme sbt = {TRUE, RGB(150, 190, 245), RGB(196, 215, 250)};
    SetStatusBarTheme(sbt);

    ToolBarTheme tbt = {TRUE, RGB(255, 230, 190), RGB(255, 190, 100),
        RGB(255, 140, 40), RGB(255, 180, 80), RGB(128, 128, 255)};
    SetToolBarTheme(tbt);

    MenuTheme mt = {TRUE, RGB(255, 230, 190), RGB(255, 190, 100),
        RGB(150, 190, 245), RGB(220, 230, 250), RGB(128, 128, 200)};
    SetMenuTheme(mt);
    RecalcLayout();
    return TRUE;
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
    AddMenuIcons(data, RGB(192, 192, 192), IDB_MENUICONS, 0);
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
          // start out with a separator just to give some space at the
          // left of the toolbar
    AddToolBarButton(0);  // Separator
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
SetWindowTitle(LPCTSTR path)                                                /*

    Put the app title and docPath name in the main window text. Limit the
    displayed text length to that of the frame.
*-----------------------------------------------------------------------------*/
{
      // the app title is in resources.rc
    CString docPath = path,
            appTitle = LoadString(IDW_MAIN);
      // compute text lengths
    int applen = appTitle.GetLength();
    int doclen = docPath.GetLength();
      // only count "\\" as one character
    for (int i = 0; i < doclen; )
    {
        i = docPath.Find(_T("\\"), i);
        if (i < 0)
            break;

        --doclen;
        i++;
    }
      // form the caption title
    if (!docPath.IsEmpty())
        appTitle  += _T(":    ") + docPath;;
      // compute maximum characters in the caption from its height in pixels,
      // assumed to be proportional to the font width
    INT height = ::GetSystemMetrics(SM_CYCAPTION);
      // the width = client - 3 buttons - the icon
    int width = m_view.GetClientRect().Width() - ::GetSystemMetrics(SM_CXSMICON)
        - 3 * ::GetSystemMetrics(SM_CXSIZE);
      // estimate the max char length using empirical formula, minus "..."
    int maxlen = (37 * width) / (10 * height) - 3;
      // limit the caption line to that which will fit
    if (appTitle.GetLength() > maxlen)
    {
          // eliminate characters in the middle
        int mid = applen + (maxlen - applen - 1) / 2;
        appTitle.Delete(mid, appTitle.GetLength() - maxlen);
        appTitle.Insert(mid, _T("..."));
    }
    SetWindowText(appTitle);
}

/*============================================================================*/
    void CMainFrame::
UpdateControlUIState()                                                      /*

    Check the status of controls whose enabled/disabled or check/uncheck
    status needs to be changed and make changes as necessary. Caution:
    only use one SetXXXXStatus per control m_Win32Version.
*-----------------------------------------------------------------------------*/
{
      // document status
    BOOL    doc_is_ready = m_doc.IsOpen();

      // determine enabled status of controls (TODO: redo for actual app)
    BOOL    ok_to_cut         = doc_is_ready && GetRichView().IsSelected();
    BOOL    ok_to_copy        = doc_is_ready && GetRichView().IsSelected();
    BOOL    ok_to_delete      = doc_is_ready && GetRichView().IsSelected();
    BOOL    ok_to_find        = doc_is_ready;
    BOOL    ok_to_paste       = doc_is_ready && GetRichView().CanPaste();
    BOOL    ok_to_print       = doc_is_ready;
    BOOL    ok_to_page_setup  = TRUE;
    BOOL    ok_to_redo        = GetRichView().CanRedo();
    BOOL    ok_to_replace     = doc_is_ready;
    BOOL    ok_to_save        = doc_is_ready && GetRichView().GetModify();
    BOOL    ok_to_saveas      = doc_is_ready;
    BOOL    ok_to_undo        = GetRichView().CanUndo();
    BOOL    ok_to_close       = doc_is_ready;

      // set the control button status
    SetEnableStatus(IDM_EDIT_CUT,       ok_to_cut,         both);
    SetEnableStatus(IDM_EDIT_COPY,      ok_to_copy,        both);
    SetEnableStatus(IDM_EDIT_DELETE,    ok_to_delete,      both);
    SetEnableStatus(IDM_EDIT_FIND,      ok_to_find,        both);
    SetEnableStatus(IDM_FILE_PAGESETUP, ok_to_page_setup,  mainmenu);
    SetEnableStatus(IDM_EDIT_PASTE,     ok_to_paste,       both);
    SetEnableStatus(IDM_FILE_PRINT,     ok_to_print,       both);
    SetEnableStatus(IDM_EDIT_REDO,      ok_to_redo,        both);
    SetEnableStatus(IDM_EDIT_REPLACE,   ok_to_replace,     both);
    SetEnableStatus(IDM_FILE_CLOSE,     ok_to_close,       mainmenu);
    SetEnableStatus(IDM_FILE_SAVE,      ok_to_save,        both);
    SetEnableStatus(IDM_FILE_SAVEAS,    ok_to_saveas,      both);
    SetEnableStatus(IDM_EDIT_UNDO,      ok_to_undo,        both);
    SetCheckStatus(IDM_RICHEDWRAP,      m_isTextWrap,        mainmenu);
}

/*============================================================================*/
    void CMainFrame::
UpdateMRUMenu()                                                             /*

    Override the base class method to truncate long MRU list entries using
    a '...' splice at the midpoint, rather than at the beginning, as does
    the base class. This will be called by the framework whenever a menu
    element is changed.
*-----------------------------------------------------------------------------*/
{
      // find in the leftmost submenu (i.e., the one with index 0)
    CMenu fileMenu = GetFrameMenu().GetSubMenu(0);
      // compute the index of the last entry in the MRU list
    int last = static_cast<int>(MIN(GetMRUSize(), m_maxMRU)) - 1;
      // if there is no leftmost submenu, or if there are no entries to
      // post, or if we cannot modify the first entry to indicate an empty
      // MRU list, we cannot proceed
    if (!fileMenu.GetHandle())
    {     // just refresh the frame menu bar and  leave
        DrawMenuBar();
        return;
    }
      // insert the empty MRU list label in the top slot
    fileMenu.ModifyMenu(IDW_FILE_MRU_FILE1, MF_BYCOMMAND,
        IDW_FILE_MRU_FILE1, _T("Recent Files"));
    fileMenu.EnableMenuItem(IDW_FILE_MRU_FILE1, MF_BYCOMMAND | MF_GRAYED);

      // remove all the other MRU Menu entries
    for (int i = IDW_FILE_MRU_FILE2; i <= IDW_FILE_MRU_FILE1 +
        static_cast<int>(m_maxMRU); ++i)
        fileMenu.DeleteMenu(i, MF_BYCOMMAND);
      // if the list is not empty, there's more to do
    if (last >= 0)
    {
          // create the MRU "show" list, which contains only strings
          // of limited length, chars removed at the midpoint, as needed
        int maxlen = WXX_MAX_STRING_SIZE - 10;
        int mid = maxlen / 2;
        CString strMRUShow[16];
        for (int i = 0; i <= last; i++)
        {
            CString s = GetMRUEntries()[i];
            if (s.GetLength() > maxlen)
            {
                  // eliminate middle if too long
                s.Delete(mid, s.GetLength() - maxlen);
                s.Insert(mid, _T("..."));
            }
            // Prefix with its number
            CString v;
            v.Format(_T("%d "), i + 1);
            strMRUShow[i] = v + s;
        }

          // display the MRU items: start by replacing the first item
          // in the the list with the last MRU item
        fileMenu.ModifyMenu(IDW_FILE_MRU_FILE1, MF_BYCOMMAND,
            static_cast<INT_PTR>(last) + IDW_FILE_MRU_FILE1, strMRUShow[last]);
          // now insert the remaining items in reverse order, starting
          // at the next-to-last entry and  pushing all the others
          // down in the menu (entries thus end up in the correct order)
        for (int j = last - 1; j >= 0; last--, j--)
            fileMenu.InsertMenu(IDW_FILE_MRU_FILE1 + last,
                static_cast<INT_PTR>(j) + MF_BYCOMMAND, IDW_FILE_MRU_FILE1,
                strMRUShow[j]);
    }
      // refresh the frame menu bar and  leave
    DrawMenuBar();
}

/*============================================================================*/
    void CMainFrame::
ValidateMRU()                                                               /*
    Validate the that the MRU list entries, if there are any, correspond
    to actual file paths. Remove any that do not.
*-----------------------------------------------------------------------------*/
{
      // get a copy of the MRU list entries
    const std::vector<CString> MRUEntries = GetMRUEntries();
      // check them one by one as a valid file path
    std::vector<CString>::const_iterator it;
    for (it = MRUEntries.begin(); it != MRUEntries.end(); ++it)
    {
          // check whether the current entry exists
        CString s = *it;
        if (_taccess(s, 4) != 0)
        {
              // for the demo, announce removal
            ::MessageBox(NULL, s, _T("Removing invalid MRU entry..."),
                MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
            RemoveMRUEntry(s);
        }
    }
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
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);
        return 0;
    }
}
/*----------------------------------------------------------------------------*/
