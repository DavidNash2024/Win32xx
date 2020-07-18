/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]              (MainFrm.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CMainFrame class for the
    CommonDialogs demonstration sample application using the Win32++ Windows
    interface classes, Copyright (c) 2005-2016 David Nash, under permissions
    granted therein.

    This particular frame class contains features a fixed-size form for the
    display, with no resizing gripper tool at the end of the status bar,
    and provisions for selection of client background color, selection of
    edit box font, use of external serialization files, and MRU lists.

        Caveats: The copyright displayed above extends only to the author's
    original contributions to the subject class, and to the alterations,
    additions, deletions, and other treatments of materials that may have
    been extracted from the cited sources.  Unaltered portions of those
    materials retain their original copyright status. The author hereby
    grants permission to any person obtaining a copy of this treatment
    of the subject class and any associated documentation composed by
    the author, to utilize this material, free of charge and without
    restriction or limitation, subject to the following conditions:

        The above copyright notice, as well as that of David Nash
        and Win32++, together with the respective permission
        conditions shall be included in all copies or substantial
        portions of this material so copied, modified, merged,
        published, distributed, or otherwise held by others.

    These materials are provided "as is", without warranty of any kind,
    express or implied, including but not limited to: warranties of
    merchantability, fitness for a particular purpose, and non-infringement.
    In no event shall the authors or copyright holders be liable for any
    claim, damages, or other liability, whether in an action of contract,
    tort or otherwise, arising from, out of, or in connection with, these
    materials, the use thereof, or any other other dealings therewith.
    Citation of the author's work should be included in such usages.

    Special Conventions: 

    Acknowledgement:
        The author would like to thank and acknowledge the advice,
        critical review, insight, and assistance provided by David Nash
        in the development of this work.

    Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
        Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

    Implementation of the CMainFrame class

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"
#include <io.h>
#include "resource.h"

/*******************************************************************************

    Macros and Local (static) default constants                             */

  // the (initial) window dimensions : {left, top, width, height}
static const RECT rcWindowPlacement = {100, 100, 780, 620};

/*******************************************************************************

    Implementation of the Main Frame class

*=============================================================================*/
    CMainFrame::
CMainFrame()                                                                /*

    Construct and initiallize the CMainFrame object from the IDD_FORM_DIALOG
    resource defined in resource.rc.
*-----------------------------------------------------------------------------*/
    : m_View(IDD_FORM_DIALOG)
{
      // set normal cursor to the arrow
    m_hCursor = ::LoadCursor(NULL, IDC_ARROW);
      // set the form initial default location and (fixed) size
    m_win_x   = rcWindowPlacement.left;
    m_win_y   = rcWindowPlacement.top;
    m_width   = rcWindowPlacement.right;
    m_height  = rcWindowPlacement.bottom;
    m_nMaxMRU = 0;
    m_bWrapText = TRUE;
    ZeroMemory(&m_Wndpl, sizeof(WINDOWPLACEMENT));
      // Set m_View as the view window of the frame
    SetView(m_View);
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
    m_ColorChoice.AddColorChoice(DlgBg,      _T("o  Dialog BG"),
        COLOR_LT_BLUE);
    m_ColorChoice.AddColorChoice(SBBg,      _T("o  Status bar BG"),
        GetStatusBarTheme().clrBkgnd1); // use current theme
      // richedit controls (these are set differently than the others)
    m_ColorChoice.AddColorChoice(REdTxFg,   _T("o  RichEdit text FG"),
        COLOR_WHITE);
    m_ColorChoice.AddColorChoice(REdTxBg,   _T("o  RichEdit text BG"),
        COLOR_RED); 
    m_ColorChoice.AddColorChoice(REdBg,     _T("o  RichEdit BG"),
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
    if (_taccess(theApp->GetArcvFile(), 0x04) != 0)
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
        CArchive ar(theApp->GetArcvFile(), CArchive::load);
              // deserialize in the same order as serialized
        ar >> *theApp;  // for the app
        ar >> *this;    // for the mainframe and base classes
        ar >> m_View;   // for the view, including control colors
        Invalidate();   // repaint the client with recovered colors

          // weed out any MRU entries that have disappeared
        ValidateMRU(); // remove invalid file path names
          // the ar object closes on destruction
    }
    catch(...) // catch all exceptions in trying to load the archive
    {
        CString msg = _T("Previous settings could not be restored.\n")
            _T("Unable to read archived values.\n");
        ::MessageBox(NULL, msg, _T("Exception"), MB_OK | MB_ICONSTOP |
            MB_TASKMODAL);
    }
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
    m_ColorChoice.DoModal(GetApp()->GetMainWnd()); 
      // reset the status bar color
    UINT selection = m_ColorChoice.GetSelectedColorID();
    if (selection == SBBg)
    {
        COLORREF sb = GetSBBkColor();
        SetSBBkColor(sb);
    }
      // reset the rich edit control color
    if (selection == REdTxFg || selection == REdTxBg || selection == REdBg)
        OnRichEditColor();
        
    UpdateControlUIState();
    Invalidate();
    UpdateWindow();
}

/*============================================================================*/
    BOOL CMainFrame::
OnCommand(WPARAM wParam, LPARAM lParam)                                     /*

    The framework calls this member method when the user selects an
    item from a menu, when a child control sends a notification message,
    or when an accelerator keystroke is translated. Here, we respond to
    menu selections, toolbar events, scrollbar actions, and accelerator
    keys.

    The low-order word of wParam identifies the command ID of the menu
    or accelerator message. The high-order word is 1 if the message is
    from an accelerator, or 0 if the message is from the menu.

    The command ID may not always be sufficient to determine the action
    to be taken.  For example, the CBN_SELCHANGE message will be received
    when a user changes the current selection in the list box of a combo
    box, and appears in the high-order word of the wParam parameter.
    However, the CBN_SELCHANGE message value itself is 1, and is thus
    indistingishable from an accelerator event on this basis.  For this
    reason, the low-order word will also need to be queried to distinguish
    the actual combobox event.

    The method returns nonzero if it processes the message; otherwise it
    returns zero.
*-----------------------------------------------------------------------------*/
{
    UINT nID = LOWORD(wParam);

      // map all MRU file messages to one representative
    if(IDW_FILE_MRU_FILE1 <= nID && nID < IDW_FILE_MRU_FILE1 + m_nMaxMRU)
        nID = IDW_FILE_MRU_FILE1;

    switch(nID)
    {
    case IDM_FILE_NEW:
        m_Doc.OnNewDoc();
        UpdateControlUIState();
        return TRUE;

    case IDM_FILE_OPEN:
        m_Doc.OnOpenDoc();
        UpdateControlUIState();
        return TRUE;

    case IDM_FILE_SAVE:
        m_Doc.OnSaveDoc();
        UpdateControlUIState();
        return TRUE;

    case IDM_FILE_SAVEAS:
        m_Doc.OnSaveDocAs();
        UpdateControlUIState();
        return TRUE;

    case IDM_FILE_CLOSE:
        m_Doc.OnCloseDoc();
        UpdateControlUIState();
        return TRUE;

    case IDM_FILE_PAGESETUP:
        m_Doc.OnPageSetup();
        return TRUE;

    case IDM_FILE_PREVIEW:
        m_View.OnPrintPreview();
        return TRUE;

    case IDM_FILE_PRINT:
        m_View.OnPrintDocument();
        return TRUE;

    case IDM_FILE_EXIT:
          // Issue a close request to the frame
        SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
        return TRUE;

    case IDM_EDIT_UNDO:
        if (::GetFocus() != GetREView())
            return (::SendMessage(::GetFocus(), EM_UNDO, 0, 0) != 0);

        m_Doc.OnUndo();
        UpdateControlUIState();
        return TRUE;


    case IDM_EDIT_REDO:
        if (::GetFocus() != GetREView())
            return (::SendMessage(::GetFocus(), EM_REDO, 0, 0) != 0);

        m_Doc.OnRedo();
        UpdateControlUIState();
        return TRUE;

    case IDM_EDIT_CUT:
        if (::GetFocus() != GetREView())
            return (::SendMessage(::GetFocus(), WM_CUT, 0, 0) != 0);

        m_Doc.OnCut();
        UpdateControlUIState();
        return TRUE;


    case IDM_EDIT_COPY:
        if (::GetFocus() != GetREView())
            return (::SendMessage(::GetFocus(), WM_COPY, 0, 0) != 0);

        m_Doc.OnCopy();
        UpdateControlUIState();
        return TRUE;

    case IDM_EDIT_PASTE:
        if (::GetFocus() != GetREView())
            return (::SendMessage(::GetFocus(), WM_PASTE, 0, 0) != 0);

        m_Doc.OnPaste();
        UpdateControlUIState();
        return TRUE;

    case IDM_EDIT_DELETE:
        if (::GetFocus() != GetREView())
            return (::SendMessage(::GetFocus(), WM_CLEAR, 0, 0) != 0);

        m_Doc.OnDelete();
        UpdateControlUIState();
        return TRUE;

    case IDM_EDIT_FIND:
        OnEditFind();
        UpdateControlUIState();
        return TRUE;

    case IDM_EDIT_REPLACE:
        OnEditReplace();
        UpdateControlUIState();
        return TRUE;

    case IDW_ABOUT:         // invoked by F1 and Help->About menu item
        OnHelp();
        return TRUE;

    case IDW_VIEW_TOOLBAR:
        OnViewToolBar(); // toggle tool bar
        return TRUE;

    case IDW_VIEW_STATUSBAR:
        OnViewStatusBar(); // toggle status bar
        return TRUE;

    case IDM_RICHEDWRAP:
        OnWrapText();
        return TRUE;
    
    case IDM_COLOR_CHOICE:
        OnColorChoice();
        return TRUE;

    case IDM_FONT_CHOICE:
        OnFontChoice();
        return TRUE;

    case IDW_FILE_MRU_FILE1:
        OnProcessMRU(wParam, lParam);
        return TRUE;


    default:
        break;
    }
    
    return FALSE;
}

/*============================================================================*/
    INT_PTR  CMainFrame::
OnCtlColor(HDC hDC, UINT nCtlColor)                              /*

    This member function is invoked when a child control is about to be drawn. 
    Here it is used to prepare the display context hDC for drawing the dialog
    background using the user-selected color. It is not used to change the 
    colors of a rich edit control. See View::SetRichEditColors() for that process.
*-----------------------------------------------------------------------------*/
{
      // declare default control colors IDs
    UINT fg = DfltClr;
    UINT bk = DfltClr;
    UINT bg = DfltClr;
    if (nCtlColor == WM_CTLCOLORDLG)
            bg = DlgBg;
       // get the display context
    CDC dcCtl(hDC);
    dcCtl.SetTextColor(m_ColorChoice.GetTableColor(fg));
    dcCtl.SetBkColor(m_ColorChoice.GetTableColor(bk));
    m_br = m_ColorChoice.GetBrush(bg);
      // returned brush handle must persist
    return (UINT_PTR)(HBRUSH)m_br;
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


    // TODO: set CREATESTRUCT desired options here

      // set the maximum MRU entries value
    m_nMaxMRU = theAppGlobal.GetMaxMRU();
      
      // call the base class OnCreate() method with these options
    int rtn = CFrame::OnCreate(rcs);

      // set theme colors, if supported
    if (IsReBarSupported())
    {
          //Set our theme colors
        SetThemeColors();
    }
      // establish communications
    m_Doc.Register(this, &m_View);
      // populate the initial control colors (will be overwritten by
      // deserialized values)
    InitCtlColors();
      // and set the initial flags to show all colors
    CHOOSECOLOR cc = m_ColorChoice.GetParameters();
    cc.Flags = CC_FULLOPEN;
    cc.Flags |= CC_ANYCOLOR | CC_RGBINIT | CC_ENABLEHOOK;
      // setup the CColorChoice object
    m_ColorChoice.SetParameters(cc);
      // tell CFrame the max MRU size
    m_nMaxMRU = theAppGlobal.GetMaxMRU();
    SetMRULimit(m_nMaxMRU);
      // set the initial flags to use the font style,
    CHOOSEFONT cf = m_FontChoice.GetParameters();
    cf.Flags |= CF_USESTYLE;
    cf.lpszStyle = const_cast<LPTSTR>(_T("Regular")); // initial font
    m_FontChoice.SetParameters(cf);
      // set the default font
    CFont f;
    f.CreatePointFont(100, _T("Courier New"));
    m_FontChoice.SetChoiceFont(f);
    m_FontChoice.SetColor(m_ColorChoice.GetTableColor(REdTxFg));
    m_View.SetEditFont(f);
    LoadPersistentData();
      // set the default status bar color
    COLORREF sb = GetSBBkColor();
    SetSBBkColor(sb);
      // set the edit box colors
    OnRichEditColor();
    return rtn;
}

/*============================================================================*/
    void CMainFrame::
OnEditFind()                                                                /*

    Initiate the find non-modal dialog box and the messages sent to the
    CDoc::OnFindReplace() method by the CMainFrame::WndProc() message loop.
*-----------------------------------------------------------------------------*/
{
    m_FindRepDialog.SetBoxTitle(_T("Find a string..."));
    m_FindRepDialog.Create(TRUE, _T("Initial Text"), _T(""), FR_DOWN |
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
    m_FindRepDialog.SetBoxTitle(_T("Find, then Replace"));
    m_FindRepDialog.Create(FALSE, _T("Initial Text"), _T("Replace Text"),
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
    m_Doc.OpenDoc(mru_entry);
    UpdateControlUIState();
}

/*============================================================================*/
    void CMainFrame::
OnFontChoice()                                                              /*

        Select the view font typeface, characteristics, and color.
*-----------------------------------------------------------------------------*/
{
    HWND hOwnerWnd = GetApp()->GetMainWnd();
      // open the dialog
    m_FontChoice.SetBoxTitle(_T("Select font for rich edit box"));
    LOGFONT lf;
    m_FontChoice.GetChoiceFont().GetObject(sizeof(LOGFONT), &lf);
    CHOOSEFONT cf = m_FontChoice.GetParameters();
    cf.Flags |= CF_SCREENFONTS | CF_EFFECTS | CF_INITTOLOGFONTSTRUCT; 
    cf.lpLogFont = &lf;     
    cf.rgbColors = m_ColorChoice.GetTableColor(REdTxFg);
    m_FontChoice.SetParameters(cf);
    if(m_FontChoice.DoModal(hOwnerWnd))
    {
          // bring choice elements into this view
        m_View.SetEditFont(m_FontChoice.GetChoiceFont());
        m_ColorChoice.SetTableColor(REdTxFg, m_FontChoice.GetColor());
          // reset the rich edit control color
        OnRichEditColor();
    }
    Invalidate();
    UpdateControlUIState();
    UpdateWindow();
}

/*============================================================================*/
    BOOL CMainFrame::
OnHelp()                                                                    /*

    Overrides CFrame OnHelp().
*-----------------------------------------------------------------------------*/
{
    m_AboutBox.OnHelpAbout();
    return TRUE;
}

/*============================================================================*/
    void CMainFrame::
OnInitialUpdate()                                                           /*

    The frame has now been created. Provide any desired main frame
    formatting.
*-----------------------------------------------------------------------------*/
{
    // TODO: Place any additional startup code here.

      // enable drag-and-drop file entry mode
    DragAcceptFiles(TRUE);
      // if there is a MRU item at the top of the list, use it
      // as the name of the document to open
    CString docfile = GetMRUEntry(0),
        msg;
    msg.Format(_T("Open previous document?\n    %s"), docfile.c_str());
    if (!docfile.IsEmpty() &&  (::MessageBox(NULL, msg, _T("Question..."),
        MB_YESNO | MB_ICONQUESTION) == IDYES))
    {
        m_Doc.OpenDoc(docfile);
    }
    UpdateControlUIState();

      // Resize the text window
    CRect rc = m_View.GetClientRect();
    rc.DeflateRect(10, 10);
    m_View.GetREView().SetWindowPos(NULL, rc, SWP_SHOWWINDOW);

      // Unselect the text
    CHARRANGE r;
    ZeroMemory(&r, sizeof(r));
    m_View.GetREView().SetSel(r);
    m_View.GetREView().SetFocus();

    TRACE("Frame created\n");
}

/*============================================================================*/
    void CMainFrame::
OnMenuUpdate(UINT nID)                                                      /*

    This method is invoked for each item in a top-level menu item the mouse
    selects. Override the base class method to update the frame status
    as well as the usual tool bar menu items.
*-----------------------------------------------------------------------------*/
{
      // Update the check state of the various menu items
    UpdateControlUIState();
    CFrame::OnMenuUpdate(nID);
}

/*============================================================================*/
    LRESULT CMainFrame::
OnNotify(WPARAM wParam, LPARAM lParam)                                      /*

    Process messages that controls send to the main frame, events such
    as mouse clicks, changes in content and  selection, and  control
    background painting by sending a message to the parent.
**-----------------------------------------------------------------------------*/
{
//  NMHDR* pNMH;
//  pNMH = (LPNMHDR) lParam;
//  switch (pNMH->code)
//  {
//      default:
//          break;
//  }

    return CFrame::OnNotify(wParam, lParam);
}

/*============================================================================*/
    BOOL CMainFrame::
OnProcessMRU(WPARAM wParam, LPARAM lParam)                                  /*

    One of the MRU entries has been selected.  Process accordingly.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(lParam);

      // compute the MRU index, where IDW_FILE_MRU_FILE1 is index 0
    UINT nMRUIndex = LOWORD(wParam) - IDW_FILE_MRU_FILE1;
    OnFileOpenMRU(nMRUIndex);
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnRichEditColor()                                                           /*

    Set the rich edit control colors to those found in the color table.
*-----------------------------------------------------------------------------*/
{
    COLORREF txfg = m_ColorChoice.GetTableColor(REdTxFg);
    COLORREF txbg = m_ColorChoice.GetTableColor(REdTxBg);
    COLORREF bg   = m_ColorChoice.GetTableColor(REdBg);
    m_View.SetRichEditColors(txfg, txbg, bg);
    return TRUE;
}

/*============================================================================*/
    void CMainFrame::
OnWrapText()                                                                /*

    Description
*-----------------------------------------------------------------------------*/
{
    m_bWrapText = !m_bWrapText;
    GetREView().SetWrapping(m_bWrapText);
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
    cs.x  = m_win_x;  // set initial window placement
    cs.y  = m_win_y;
    cs.cx = m_width;
    cs.cy = m_height;
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
        CArchive ar(theApp->GetArcvFile(), CArchive::store);
             // serialize in the following order
        ar << theApp;   // for the App
        ar << *this;    // for the mainframe and base classes
        ar << m_View;   // for the view, including control colors
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
    {     // save the base class frame status and tool bar switches:
          // these control the display of the StatusBar and ToolBar
        BOOL showbar = GetStatusBar().IsWindowVisible();
        ar << showbar;
        showbar = GetToolBar().IsWindowVisible();
        ar << showbar;
          // save font parameters
        ar << m_FontChoice;
          // save regular control class colors
        ar << m_ColorChoice;
        ar << m_bWrapText;
          // record the number of MRU entries to write
        size_t i, nMRU = GetMRUSize();
        ar << nMRU;
          // save this many entries (don't use a copied list)
        for (i = 0; i < nMRU; ++i)
        {
            ar << GetMRUEntries()[i];
        }
    }
    else 
    {     // recover frame status and  tool bar base class switches
        BOOL showbar;
        ar >> showbar;
        ShowStatusBar(showbar);
        ar >> showbar;
        ShowToolBar(showbar);
          // recover font parameters
        ar >> m_FontChoice;
        m_View.SetFont(m_FontChoice.GetChoiceFont(), TRUE);
            // set font elements into view
        m_View.SetEditFont(m_FontChoice.GetChoiceFont());
        m_ColorChoice.SetTableColor(REdTxFg, m_FontChoice.GetColor());
          // recover colors
        ar >> m_ColorChoice;
          // reset the status bar color
        COLORREF sb = GetSBBkColor();
        SetSBBkColor(sb);
        ar >> m_bWrapText;
        GetREView().SetWrapping(m_bWrapText);
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
            if (i < m_nMaxMRU)  // keep only those within the limit
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
SetCheckStatus(UINT nID, BOOL bCheck, ControlBars where)                    /*

*-----------------------------------------------------------------------------*/
{
    BOOL ok = FALSE;
    if (where == mainmenu || where == both)
    {
        if (bCheck > 1)
            ok = (GetFrameMenu().EnableMenuItem(nID, MF_BYCOMMAND |
                MF_GRAYED) != 0xFFFFFFFF);
        else
            ok = (GetFrameMenu().CheckMenuItem(nID, MF_BYCOMMAND |
                (bCheck ? MF_CHECKED : MF_UNCHECKED)) != 0xFFFFFFFF);
    }
    
    if (!ok)
        ok = TRUE;
    if (where == mainmenu || where == both)
    {
        if (bCheck > 1)
            ok = GetToolBar().SetButtonState(nID, TBSTATE_INDETERMINATE);
        else
        {
            GetToolBar().CheckButton(nID, bCheck);
            ok = TRUE;
        }
    }
    return ok;
}

/*============================================================================*/
    BOOL CMainFrame::
SetEnableStatus(UINT nID, BOOL status, ControlBars which)                   /*

    Set the control having the given nID to the enable/disable
    status, on the toolbar, main menu, or both, as indicated by which.
*-----------------------------------------------------------------------------*/
{
    UINT action = MF_BYCOMMAND | (status ? MF_ENABLED : MF_GRAYED);
    if (which == mainmenu)
        return (GetFrameMenu().EnableMenuItem(nID, action) != 0xFFFFFFFF);
    else if (which == toolbar)
        return (status ? GetToolBar().EnableButton(nID) :
            GetToolBar().DisableButton(nID));
      // else must be both
    BOOL rtn = (GetFrameMenu().EnableMenuItem(nID, action) != 0xFFFFFFFF);
    rtn |= (status ? GetToolBar().EnableButton(nID) :
        GetToolBar().DisableButton(nID));
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
        ZeroMemory(&rt, sizeof(ReBarTheme));
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
    GetStatusBar().SetPartText(0, status);
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

    Assigns icons to the dropdown menu items.  
*-----------------------------------------------------------------------------*/
{
     // Add IDW_ABOUT to the data
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
SetWindowTitle(const CString& docfile)                                      /*

    Put the app title and docfile name in the main window text. Limit the
    displayed text length to that of the frame.
*-----------------------------------------------------------------------------*/
{
      // the app title is in resources.rc
    CString appTitle = LoadString(IDW_MAIN);
      // compute text lengths
    int applen = appTitle.GetLength();
    int doclen = docfile.GetLength();
      // only count "\\" as one character
    for (int i = 0; i < doclen; )
    {
        i = docfile.Find(_T("\\"), i);
        if (i < 0)
            break;

        --doclen;
        i++;
    }
      // form the caption title
    if (!docfile.IsEmpty())
        appTitle  += _T(":    ") + docfile;;
      // compute maximum characters in the caption and mid point of docfile
    int width = m_View.GetClientRect().Width();
    int maxlen = (10 * width) / 68 - 3;  // empirical value minus "..."
    int mid    = applen + (maxlen - applen - 1) / 2;
      // limit the caption line to that which will fit
    if (appTitle.GetLength() > maxlen)
    {
          // eliminate middle if too long
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
    only use one SetXXXXStatus per control ID.
*-----------------------------------------------------------------------------*/
{
    // TODO: Add and modify code here to implement this member

      // document status
    BOOL    doc_is_ready = m_Doc.IsOpen();

      // determine enabled status of controls (TODO: redo for actual app)
    BOOL    ok_to_cut         = doc_is_ready && GetREView().IsSelected();
    BOOL    ok_to_copy        = doc_is_ready && GetREView().IsSelected();
    BOOL    ok_to_delete      = doc_is_ready && GetREView().IsSelected();
    BOOL    ok_to_find        = doc_is_ready;
    BOOL    ok_to_paste       = doc_is_ready && GetREView().CanPaste();
    BOOL    ok_to_print       = doc_is_ready;
    BOOL    ok_to_preview     = FALSE;   // doc_is_ready; // not supported
    BOOL    ok_to_page_setup  = TRUE;
    BOOL    ok_to_redo        = GetREView().CanRedo();
    BOOL    ok_to_replace     = doc_is_ready;
    BOOL    ok_to_save        = doc_is_ready && GetREView().GetModify();
    BOOL    ok_to_saveas      = doc_is_ready;
    BOOL    ok_to_undo        = GetREView().CanUndo();
    BOOL    ok_to_close       = doc_is_ready;
    
      // set the control button status
    SetEnableStatus(IDM_EDIT_CUT,       ok_to_cut,     both);
    SetEnableStatus(IDM_EDIT_COPY,      ok_to_copy,        both);
    SetEnableStatus(IDM_EDIT_DELETE,    ok_to_delete,      both);
    SetEnableStatus(IDM_EDIT_FIND,      ok_to_find,        both);
    SetEnableStatus(IDM_FILE_PAGESETUP, ok_to_page_setup,  mainmenu);
    SetEnableStatus(IDM_EDIT_PASTE,     ok_to_paste,       both);
    SetEnableStatus(IDM_FILE_PRINT,     ok_to_print,       both);
    SetEnableStatus(IDM_FILE_PREVIEW,       ok_to_preview,     mainmenu);
    SetEnableStatus(IDM_EDIT_REDO,      ok_to_redo,    both);
    SetEnableStatus(IDM_EDIT_REPLACE,   ok_to_replace,     both);
    SetEnableStatus(IDM_FILE_CLOSE,     ok_to_close,       mainmenu);
    SetEnableStatus(IDM_FILE_SAVE,      ok_to_save,        both);
    SetEnableStatus(IDM_FILE_SAVEAS,    ok_to_saveas,      both);
    SetEnableStatus(IDM_EDIT_UNDO,      ok_to_undo,        both);
    SetCheckStatus(IDM_RICHEDWRAP,      m_bWrapText,       mainmenu);
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
    int iLast = static_cast<int>(MIN(GetMRUSize(), m_nMaxMRU)) -  1;
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
        static_cast<int>(m_nMaxMRU); ++i)
        fileMenu.DeleteMenu(i, MF_BYCOMMAND);
      // if the list is not empty, there's more to do
    if (iLast >= 0)
    {
          // create the MRU "show" list, which contains only strings
          // of limited length, chars removed at the midpoint, as needed
        int maxlen = MAX_MENU_STRING - 10;
        int mid    = maxlen / 2;
        CString strMRUShow[16];
        for (int i = 0; i <= iLast; i++)
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
            IDW_FILE_MRU_FILE1 + iLast, strMRUShow[iLast]);
          // now insert the remaining items in reverse order, starting
          // at the next-to-iLast entry and  pushing all the others
          // down in the menu (entries thus end up in the correct order)
        for (int j = iLast - 1 ; j >= 0; iLast--, j--)
            fileMenu.InsertMenu(IDW_FILE_MRU_FILE1 + iLast,
                MF_BYCOMMAND, IDW_FILE_MRU_FILE1 + j,
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
    const std::vector<CString>& MRUEntries = GetMRUEntries();
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
WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)                            /*

    This is the main frame message loop. By default, it handles the
    normal housekeeping functions (see Win32++\include\frame.h).
*-----------------------------------------------------------------------------*/
{
      // can't use a case statement on this ::RegisterWindowMessage() value
    if (uMsg == UWM_FINDMSGSTRING)
    {
        m_Doc.OnFindReplace(uMsg, wParam, lParam);
        return TRUE;
    }

    switch (uMsg)
    {
        case WM_CTLCOLORDLG:
//      case WM_CTLCOLORBTN:        // (none of these used here)
//      case WM_CTLCOLOREDIT:
//      case WM_CTLCOLORLISTBOX:
//      case WM_CTLCOLORSCROLLBAR:
//      case WM_CTLCOLORSTATIC:
            return OnCtlColor(reinterpret_cast<HDC>(wParam), uMsg);

        case IDM_UPDATECONTROLUISTATE:
            UpdateControlUIState();
            return TRUE;
        
        case WM_SYSCOMMAND:
        {
              // else process requests for action
            switch (LOWORD(wParam))
            {
                case SC_CLOSE:
                      // TODO: other cleanup here, as needed
                    m_Doc.OnCloseDoc();
                    break;  // let default process this further
            }
        }
    }
      // pass unhandled messages on for default processing
    return WndProcDefault(uMsg, wParam, lParam);
}

