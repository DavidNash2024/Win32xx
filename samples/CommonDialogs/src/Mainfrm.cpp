/* (26-Mar-2025)                                                 (MainFrm.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CMainFrame class for the
    CommonDialogs demonstration sample application using the Win32++ Windows
    interface classes.

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"
#include <io.h>


// Construct and initialize the CMainFrame object from the IDD_MAIN_DIALOG
// resource defined in resource.rc. Note that the initial window location and
// size are set here.
CMainFrame::CMainFrame() : m_view(IDD_MAIN_DIALOG), m_isTextWrap(TRUE)
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
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

// Open the text document dragged and dropped in the rich edit window.
BOOL CMainFrame::DropFiles(LPARAM lparam)
{
    ENDROPFILES* ENDrop = reinterpret_cast<ENDROPFILES*>(lparam);
    HDROP hDropinfo = (HDROP) ENDrop->hDrop;
    wchar_t szFileName[_MAX_PATH];
    ::DragQueryFile((HDROP)hDropinfo, 0, szFileName, _MAX_PATH);
    OpenDoc(szFileName);
    ::DragFinish(hDropinfo);
    return TRUE;
}

// Populate the color table with the initial ctl_color triplets used in the
// list box of the CColorDialog object. These values are displayed in the
// controls on first execution of this program, and are overwritten by
// deserialization in subsequent executions.
void CMainFrame::InitCtlColors()
{
    m_colorChoice.AddColorChoice(DlgBg,   L"o Dialog background",
        COLOR_DK_BLUE);
    m_colorChoice.AddColorChoice(REdTxFg, L"o RichEdit text foreground",
        COLOR_BLACK);
    m_colorChoice.AddColorChoice(REdTxBg, L"o RichEdit text background",
        COLOR_WHITE);
    m_colorChoice.AddColorChoice(REdBg,   L"o RichEdit background",
        COLOR_WHITE);
}

void CMainFrame::OnCloseDoc()
{
    m_docDir = m_doc.GetDocDir();
    m_doc.OnCloseDoc();
    SetWindowTitle(L"");
    GetRichView().Clean();
    m_view.NoDocOpen();
}

// Show the CListBoxDlg color dialog box to select the color of a control's
// text foreground, text background, or control background. NOTE: colors
// selected for controls in the view are painted by the OnCtlColor()
// method invoked in the WndProc() loop each time the control is redrawn.
// The staus bar color has no message to redraw it, so it is set here.
void CMainFrame::OnColorChoice()
{
    if (m_colorChoice.DoModal(*this) != IDOK)
        return;

    UINT selection = m_colorChoice.GetSelectedColorID();
    if (selection == DlgBg)
        SetViewBgColor();
    if (selection == REdTxFg || selection == REdTxBg || selection == REdBg)
        SetRichEditColor();

    Invalidate();
    UpdateWindow();
}

// The framework calls this member method when the user selects an item from a
// menu, when a child control sends a notification message, or when an
// accelerator keystroke is translated. Here, we respond to menu selections,
// toolbar events, scrollbar actions, and accelerator keys. The method returns
// nonzero if it processes the message; otherwise it returns zero.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
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

// This member controls the way the frame is created.
int CMainFrame::OnCreate(CREATESTRUCT& rcs)
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


    int rtn = CFrame::OnCreate(rcs);
    m_doc.SetDataPath(&m_view);

    // Set the color choice parameters.
    InitCtlColors();
    CHOOSECOLOR cc = m_colorChoice.GetParameters();
    cc.Flags = CC_FULLOPEN;
    cc.Flags |= CC_ANYCOLOR | CC_RGBINIT | CC_ENABLEHOOK;
    m_colorChoice.SetParameters(cc);

    // Set the initial flags to use the font style,
    CHOOSEFONT cf = m_fontChoice.GetParameters();
    cf.Flags |= CF_USESTYLE;
    cf.lpszStyle = const_cast<LPWSTR>(L"Regular"); // initial font
    m_fontChoice.SetParameters(cf);

    // Set the default font
    CFont f;
    f.CreatePointFont(100, L"Courier New");
    m_fontChoice.SetChoiceFont(f);
    m_fontChoice.SetColor(m_colorChoice.GetTableColor(REdTxFg));
    m_view.SetEditFont(f);

    SetViewBgColor();
    SetRichEditColor();
    return rtn;
}

// Initiate the find non-modal dialog box.
void CMainFrame::OnEditFind()
{
    m_findReplaceDlg.SetBoxTitle(L"Find a string...");
    m_findReplaceDlg.Create(TRUE, L"Initial Text", L"", FR_DOWN |
        FR_ENABLEHOOK, *this);
}

// Invoke the find-replace dialog.
void CMainFrame::OnEditReplace()
{
    m_findReplaceDlg.SetBoxTitle(L"Find, then Replace");
    m_findReplaceDlg.Create(FALSE, L"Initial Text", L"Replace Text",
        FR_DOWN | FR_ENABLEHOOK, *this);
}

// Close the application.
void CMainFrame::OnExit()
{
    m_docDir = m_doc.GetDocDir(); // save for next file open/save
    m_doc.OnCloseDoc();
    SetWindowTitle(L"");
    GetRichView().Clean();
    m_view.NoDocOpen();
    Close();
}

// Open a file from the Most Recently Used (MRU) list.
BOOL CMainFrame::OnFileMRU(WPARAM wparam)
{
    UINT mruIndex = static_cast<UINT>(LOWORD(wparam)) - IDW_FILE_MRU_FILE1;
    CString mruText = GetMRUEntry(mruIndex);

    if (!OpenDoc(mruText))
        RemoveMRUEntry(mruText);

    return TRUE;
}

// Select the view font typeface, characteristics, and color.
void CMainFrame::OnFontChoice()
{
    HWND hOwnerWnd = GetApp()->GetMainWnd();
    m_fontChoice.SetBoxTitle(L"Select font for rich edit box");
    LOGFONT lf;
    m_fontChoice.GetChoiceFont().GetObject(sizeof(LOGFONT), &lf);
    CHOOSEFONT cf = m_fontChoice.GetParameters();
    cf.Flags |= CF_SCREENFONTS | CF_EFFECTS | CF_INITTOLOGFONTSTRUCT;
    cf.lpLogFont = &lf;
    cf.rgbColors = m_colorChoice.GetTableColor(REdTxFg);
    m_fontChoice.SetParameters(cf);

    // Open the dialog.
    if(m_fontChoice.DoModal(hOwnerWnd))
    {
        m_view.SetEditFont(m_fontChoice.GetChoiceFont());
        m_colorChoice.SetTableColor(REdTxFg, m_fontChoice.GetColor());
        SetRichEditColor();
    }

    Invalidate();
    UpdateWindow();
}

// Overrides CFrame OnHelp() to display a dialog bearing the program's
// application information.
BOOL CMainFrame::OnHelp()
{
    // Ensure only one dialog displays even for multiple activation commands.
    if (!m_aboutBox.IsWindow())
        m_aboutBox.DoModal(*this);

    return TRUE;
}

// The frame has now been created. Provide any desired main frame formatting.
void CMainFrame::OnInitialUpdate()
{
    // Unselect the text.
    GetRichView().SetSel(0, 0);
    GetRichView().SetFocus();

    TRACE("Frame created\n");
}

// Update the enable and check status of menu items.
void CMainFrame::OnMenuUpdate(UINT id)
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

// Prompt the user for a new document file name and, if valid, open a new
// document.The open status and path name are available from the document
// object.
void CMainFrame::OnNewDoc()
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
    while (fd.DoModal(GetApp()->GetMainWnd()) == IDOK)
    {
        CString selected = fd.GetPathName();
        if (::_taccess(selected, 0x04) != 0)
        {
            if (m_doc.MakeNewDoc(selected))
                OpenDoc(selected);
            return;
        }

        // Prompt the user to try again.
        msg.Format(L"That document file\n    '%s'\n"
            L"already exists.", selected.c_str());
        ::MessageBox(nullptr, msg, L"Error", MB_OK | MB_ICONERROR |
            MB_TASKMODAL);
    }
    msg = L"No name was entered, no action was taken.";
    ::MessageBox(nullptr, msg, L"Information", MB_OK | MB_ICONINFORMATION |
        MB_TASKMODAL);
    return;
}

// Process messages that controls send to the parent.
LRESULT CMainFrame::OnNotify(WPARAM wparam, LPARAM lparam)
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

// Display the open file dialog to obtain the path of an existing document
// file.If one is selected, open it.The open status and path name are
// available from the document object.
void CMainFrame::OnOpenDoc()
{
    MyFileDialog fd
    (   TRUE,                   // open file dialog
        m_docExt,               // extension defined by app
        m_docDir,               // a previous directory
        OFN_HIDEREADONLY    |   // flags
        OFN_EXPLORER        |
        OFN_NONETWORKBUTTON |
        OFN_FILEMUSTEXIST   |   // only existing files allowed
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
    ::MessageBox(nullptr, msg, L"Information", MB_OK | MB_ICONINFORMATION |
        MB_TASKMODAL);
    return;
}

// Set the rich edit control colors to those found in the color table.
BOOL CMainFrame::SetRichEditColor()
{
    COLORREF txfg = m_colorChoice.GetTableColor(REdTxFg);
    COLORREF txbg = m_colorChoice.GetTableColor(REdTxBg);
    COLORREF bg   = m_colorChoice.GetTableColor(REdBg);
    m_view.SetRichEditColors(txfg, txbg, bg);
    return TRUE;
}

// Invoke a MyPrintDialog dialog to get printing parameters and then print the
// contents of the rich view control.
void CMainFrame::OnPrint()
{
    m_view.OnPrintDocument(m_doc.GetDocPath());
}

// Save the current document.
void CMainFrame::OnSave()
{
    m_doc.OnSaveDoc();
}

// Save the current document into a file named in a file dialog and make that
// file the current document.
void CMainFrame::OnSaveAs()
{
    if (!m_doc.IsOpen())
        return;

    // Declare the file dialog box.
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
    if (fd.DoModal(GetApp()->GetMainWnd()) == IDOK)
    {
        CString selected = fd.GetPathName();
        if (selected.CompareNoCase(m_doc.GetDocPath()) == 0)
        {
            msg.Format(L"Document file\n    '%s'\n is already "
                L"open. No action taken", m_doc.GetDocPath().c_str());
            ::MessageBox(nullptr, msg, L"Information",
                MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
            return;
        }

        CString current_doc = m_doc.GetDocPath();
        OnCloseDoc();
        CopyFile(current_doc, selected, FALSE);
        if (!OpenDoc(selected))
        {
            msg.Format(L"Saved document file\n    '%s'"
                L" could not be reopened.", selected.c_str());
            ::MessageBox(nullptr, msg, L"Information",
                MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
            OpenDoc(current_doc);
            return;
        }
    }
    msg = "No name was entered, no action was taken.";
    ::MessageBox(nullptr, msg, L"Information",
        MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
}

// Toggle the word wrapping mode in the rich edit control.
void CMainFrame::OnWrapText()
{
    m_isTextWrap = !m_isTextWrap;
    GetRichView().SetWrapping(m_isTextWrap);
}

// Open the document from the given path.Return TRUE if successful, FALSE
// otherwise.
BOOL CMainFrame::OpenDoc(LPCWSTR docPath)
{
   if (CString(docPath).CompareNoCase(m_doc.GetDocPath()) == 0)
    {
        CString msg;
        msg.Format(L"Document file\n    '%s'\nis already open.", docPath);
        ::MessageBox(nullptr, msg, L"Information", MB_OK |
            MB_ICONINFORMATION | MB_TASKMODAL);
        return TRUE;
    }

    if (m_doc.IsOpen())
        OnCloseDoc();

    if (m_doc.OpenDoc(docPath))
    {
        m_docDir = m_doc.GetDocPath();
        SetWindowTitle(docPath);
        AddMRUEntry(docPath);
        return TRUE;
    }
    return FALSE;
}

// Set the colors to be used in the rebar theme.
void CMainFrame::SetReBarColors(COLORREF clrBkGnd1, COLORREF clrBkGnd2,
    COLORREF clrBand1, COLORREF clrBand2)
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

// Assign icons to the dropdown menu items.In particular, add the AboutBox
// identifier to the menu icon list.
void CMainFrame::SetupMenuIcons()
{
    std::vector<UINT> data = GetToolBarData();

    // Specify the bitmap and mask for the menu icons.
    if (GetMenuIconHeight() >= 24)
        SetMenuIcons(data, RGB(255, 0, 255), IDW_MAIN);
    else
        SetMenuIcons(data, RGB(192, 192, 192), IDB_MENUICONS);
}

// Called from the CFrame::CreateToolBar() method to load the toolbar bitmaps,
// to connect the tool bar buttons to resource IDs of the toolbar buttons, and
// to define the order of appearance of the buttons on the toolbar at runtime.
void CMainFrame::SetupToolBar()
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
    AddToolBarButton(0);  // Separator
    AddToolBarButton(IDW_ABOUT,         TRUE, 0, 11);

    // Set the toolbar image list: use defaults for hot and disabled
    SetToolBarImages(RGB(255, 0, 255), IDW_MAIN, 0, 0);
}

// Sets the background color of the dialog used as the view window.
void CMainFrame::    SetViewBgColor()
{
    m_view.SetBgColor(m_colorChoice.GetBrush(DlgBg));
}

// Put the app title and docPath name in the main window text. Limit the
// displayed text length to that of the frame.
void CMainFrame::SetWindowTitle(LPCWSTR path)
{
    CString pathname = path;
    CString title = LoadString(IDW_MAIN);

    if (!pathname.IsEmpty())
        title += " - " + pathname;

    SetWindowText(title);
}

// Called by CApp::OnIdle to update toolbar buttons.
void CMainFrame::UpdateToolbar()
{
    BOOL isSelected = m_doc.IsSelected();
    BOOL canPaste = m_doc.CanPaste();
    BOOL isDirty = m_doc.IsDirty();

    GetToolBar().EnableButton(IDM_FILE_SAVE, isDirty);
    GetToolBar().EnableButton(IDM_EDIT_COPY, isSelected);
    GetToolBar().EnableButton(IDM_EDIT_CUT, isSelected);
    GetToolBar().EnableButton(IDM_EDIT_PASTE, canPaste);
}

// This is the main frame message loop. It handles the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        // Can't use a case statement on this ::RegisterWindowMessage() value
        if (msg == UWM_FINDMSGSTRING)
        {
            m_doc.OnFindReplace(msg, wparam, lparam);
            return TRUE;
        }

        // switch (msg)
        // {
        //    TODO: add message handlers here
        // }

        // Pass unhandled messages on for default processing.
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

