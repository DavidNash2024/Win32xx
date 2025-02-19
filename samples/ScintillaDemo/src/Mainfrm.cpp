/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Scintilla.h"
#include "Mainfrm.h"
#include "resource.h"


//////////////////////////////////
// CMainFrame function definitions
//

// Constructor.
CMainFrame::CMainFrame() : m_preview(m_view), m_isToolbarShown(true), m_oldFocus(nullptr)
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

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    // Set m_view as the view window of the frame.
    SetView(m_view);

    // Set the registry key name, and load the initial window position.
    // Use a registry key name like "CompanyName\\Application".
    LoadRegistrySettings(L"Win32++\\ScintillaDemo");

    // Load the settings from the registry with 4 MRU entries.
    LoadRegistryMRUSettings(4);

    return CFrame::Create(parent);
}

// Retrieves the file name component of the file path.
CString CMainFrame::GetFileName()
{
    CString fileName;
    int delimiter = m_pathName.ReverseFind(L'\\');
    if (delimiter >= 0)
        fileName = m_pathName.Mid(delimiter + 1);

    return fileName;
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

// Called when the frame window is asked to close.
void CMainFrame::OnClose()
{
    // Close the preview.
    if (GetView() == m_preview)
        OnPreviewClose();

    // Check for unsaved text.
    if (m_view.GetModify())
    {
        CString str = L"Save file " + m_pathName;
        if (::MessageBox(0, str, L"Save", MB_YESNO | MB_ICONQUESTION) == IDYES)
        {
            if (m_pathName.IsEmpty())
                OnFileSave();
            else
                m_view.SaveFile(m_pathName);
        }
    }

    // Proceed with closing the frame.
    CFrame::OnClose();
}

// OnCommand responds to menu and and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_FILE_NEW:        return OnFileNew();
    case IDM_FILE_OPEN:       return OnFileOpen();
    case IDM_FILE_SAVE:       return OnFileSave();
    case IDM_FILE_SAVEAS:     return OnFileSaveAs();
    case IDM_FILE_PREVIEW:    return OnFilePreview();
    case IDM_FILE_PRINT:      return OnFilePrint();
    case IDM_FILE_EXIT:       return OnFileExit();
    case IDW_VIEW_STATUSBAR:  return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:    return OnViewToolBar();
    case IDM_HELP_ABOUT:      return OnHelp();
    case IDM_EDIT_UNDO:       m_view.Undo(); return TRUE;
    case IDM_EDIT_REDO:       m_view.Redo(); return TRUE;
    case IDM_EDIT_CUT:        m_view.Cut(); return TRUE;
    case IDM_EDIT_COPY:       m_view.Copy(); return TRUE;
    case IDM_EDIT_PASTE:      m_view.Paste(); return TRUE;
    case IDM_EDIT_DELETE:     m_view.Clear(); return TRUE;
    case IDM_EDIT_SELECTALL:  m_view.SelectAll(); return TRUE;
    case IDM_OPTION_FONT:     return OnOptionsFont();
    case IDM_OPTION_BACKGROUND: return OnOptionsBackground();
    case IDM_OPTION_WORDWRAP: return OnOptionWordWrap();

    case IDW_FILE_MRU_FILE1:
    case IDW_FILE_MRU_FILE2:
    case IDW_FILE_MRU_FILE3:
    case IDW_FILE_MRU_FILE4:
    case IDW_FILE_MRU_FILE5:    return OnFileMRU(wparam);
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


// Called when a file is dropped on the view window.
LRESULT CMainFrame::OnDropFiles(UINT, WPARAM wparam, LPARAM)
{
    HDROP hDrop = (HDROP)wparam;
    UINT length = DragQueryFile(hDrop, 0, 0, 0);

    if (length > 0)
    {
        CString pathName;
        DragQueryFile(hDrop, 0, pathName.GetBuffer(length), length + 1);
        pathName.ReleaseBuffer();

        m_view.OpenFile(pathName);
        m_pathName = pathName;

        AddMRUEntry(pathName);
        SetWindowTitle();
    }

    DragFinish(hDrop);
    return 0;
}

// Issue a close request to the frame to end the program.
BOOL CMainFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Create a new document.
BOOL CMainFrame::OnFileNew()
{
    m_view.ClearAll();          // Removes all text from the control.
    m_view.EmptyUndoBuffer();
    m_pathName.Empty();
    SetWindowTitle();
    m_view.SetSavePoint();      // Clears the modified status.
    m_view.SetScrollWidth(1);   // Changes the initial scrollwidth from 1200 to 1.
    return TRUE;
}

// Create the File Open dialog to choose the file to load.
BOOL CMainFrame::OnFileOpen()
{
    CString filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*|";
    CFileDialog fileDlg(TRUE);    // TRUE for file open
    fileDlg.SetFilter(filter);
    fileDlg.SetDefExt(L".txt");

    try
    {
        // Bring up the file open dialog retrieve the selected filename
        if (fileDlg.DoModal(*this) == IDOK)
        {
            CString pathName = fileDlg.GetPathName();
            m_view.OpenFile(pathName);
            AddMRUEntry(pathName);
            m_pathName = pathName;
            SetWindowTitle();
        }
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);
        return FALSE;
    }

    return TRUE;
}

// Save the document text to the current file.
BOOL CMainFrame::OnFileSave()
{
    try
    {
        if (m_pathName.IsEmpty())
            OnFileSaveAs();
        else
        {
            DWORD dwAttrib = GetFileAttributes(m_pathName);
            if (dwAttrib != INVALID_FILE_ATTRIBUTES)
            {
                CString str = "This file already exists.\nDo you want to replace it?";
                if (IDYES == MessageBox(str, L"Confirm Save", MB_ICONWARNING | MB_YESNO))
                    m_view.SaveFile(m_pathName);
            }
            else
                m_view.SaveFile(m_pathName);
        }
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);
        return FALSE;
    }


    return TRUE;
}

// Create the File Save dialog to choose the file to save.
BOOL CMainFrame::OnFileSaveAs()
{
    CString filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*|";
    CFileDialog fileDlg(FALSE);    // FALSE for file save
    fileDlg.SetFilter(filter);
    fileDlg.SetDefExt(L".txt");

    try
    {
        // Bring up the file save dialog retrieve the selected filename
        if (fileDlg.DoModal(*this) == IDOK)
        {
            CString pathName = fileDlg.GetPathName();
            m_view.SaveFile(pathName);
            AddMRUEntry(pathName);
            m_pathName = pathName;
            SetWindowTitle();
        }
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);
        return FALSE;
    }

    return TRUE;
}

// Previews a print job before sending it to the printer.
BOOL CMainFrame::OnFilePreview()
{
    try
    {
        m_isToolbarShown = GetToolBar().IsWindow() && GetToolBar().IsWindowVisible();

        // Get the device context of the default or currently chosen printer
        CPrintDialog printDlg;
        CDC printerDC = printDlg.GetPrinterDC();

        // Save the current Focus.
        SaveFocus();

        // Create the preview window if required.
        if (!m_preview.IsWindow())
            m_preview.Create(*this);

        // Set the preview's owner for notification messages.
        UINT maxPage = static_cast<UINT>(m_view.CollatePages());
        m_preview.DoPrintPreview(*this, maxPage);

        // Swap views
        SetView(m_preview);

        // Hide the menu and toolbar
        ShowMenu(FALSE);
        ShowToolBar(FALSE);

        // Update status
        CString status = L"Printer: " + printDlg.GetDeviceName();
        SetStatusText(status);
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), L"Print Preview Failed", MB_ICONWARNING);
        SetView(m_view);
        ShowMenu(GetFrameMenu() != nullptr);
        ShowToolBar(m_isToolbarShown);
    }

    return TRUE;
}

// Bring up a dialog to choose the printer.
BOOL CMainFrame::OnFilePrint()
{
    CPrintDialog printdlg;

    try
    {
        if (IDOK == printdlg.DoModal(*this))
        {
            m_view.QuickPrint(L"Scintilla Demo");
        }

    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), L"Print Failed", MB_ICONWARNING);
    }

    return TRUE;
}

// Limit the minimum size of the window.
LRESULT CMainFrame::OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam)
{
    LPMINMAXINFO lpMMI = (LPMINMAXINFO)lparam;
    const CSize minimumSize(300, 250);
    lpMMI->ptMinTrackSize.x = DpiScaleInt(minimumSize.cx);
    lpMMI->ptMinTrackSize.y = DpiScaleInt(minimumSize.cy);
    return FinalWindowProc(msg, wparam, lparam);
}

// Called after the window is created.
void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    DragAcceptFiles(TRUE);
    TRACE("Frame window created\n");
}

// Respond to a MRU selection.
BOOL CMainFrame::OnFileMRU(WPARAM wparam)
{
    UINT mruIndex = static_cast<UINT>(LOWORD(wparam)) - IDW_FILE_MRU_FILE1;
    CString mruText = GetMRUEntry(mruIndex);

    try
    {
        m_view.OpenFile(mruText);
        m_pathName = mruText;
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);
        RemoveMRUEntry(mruText);
    }

    SetWindowTitle();

    return TRUE;
}

// Updates menu items before they are displayed.
void CMainFrame::OnMenuUpdate(UINT id)
{
    UINT enabled;
    UINT checked;

    switch (id)
    {
    case IDM_FILE_SAVE:
    {
        enabled = m_view.GetModify()? MF_ENABLED : MF_GRAYED;
        GetFrameMenu().EnableMenuItem(id, enabled);
        break;
    }
    case IDM_EDIT_COPY:
    case IDM_EDIT_CUT:
    case IDM_EDIT_DELETE:
    {
        enabled = (m_view.GetSelectionStart() != m_view.GetSelectionEnd())?
            MF_ENABLED : MF_GRAYED;

        GetFrameMenu().EnableMenuItem(id, enabled);
        break;
    }
    case IDM_EDIT_PASTE:
    {
        enabled = m_view.CanPaste() ? MF_ENABLED : MF_GRAYED;
        GetFrameMenu().EnableMenuItem(id, enabled);
        break;
    }
    case IDM_EDIT_REDO:
    {
        enabled = m_view.CanRedo() ? MF_ENABLED : MF_GRAYED;
        GetFrameMenu().EnableMenuItem(id, enabled);
        break;
    }
    case IDM_EDIT_UNDO:
    {
        enabled = m_view.CanUndo() ? MF_ENABLED : MF_GRAYED;
        GetFrameMenu().EnableMenuItem(id, enabled);
        break;
    }
    case IDM_OPTION_WORDWRAP:
    {
        checked = m_view.GetWrapMode() ? MF_CHECKED : MF_UNCHECKED;
        GetFrameMenu().CheckMenuItem(id, checked);
        break;
    }

    }

    CFrame::OnMenuUpdate(id);
}

// Displays a colour dialog allowing the background color to be chosen.
BOOL CMainFrame::OnOptionsBackground()
{
    // An array of custom colors, initialized to white.
    static COLORREF custColors[16] = { RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255),
                                       RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255),
                                       RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255),
                                       RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255) };

    CColorDialog colorDlg;
    colorDlg.SetCustomColors(custColors);

    // Initialize the Choose Color dialog.
    if (colorDlg.DoModal(*this) == IDOK)
    {
        // Store the custom colors in the static array.
        memcpy(custColors, colorDlg.GetCustomColors(), 16 * sizeof(COLORREF));

        // Assign the chosen color to the background.
        m_view.StyleSetBack(STYLE_DEFAULT, colorDlg.GetColor());
        m_view.StyleClearAll();    // This is required.
    }

    return TRUE;
}

// Displays a font dialog allowing the document's font to be selected.
BOOL CMainFrame::OnOptionsFont()
{
    // Retrieve the current character format.
    CHARFORMAT cf = {};
    cf.cbSize = sizeof(cf);
    cf.dwMask = CFM_COLOR | CFM_FACE | CFM_EFFECTS | CFM_BOLD | CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
    char faceName[LF_FACESIZE] = {};
    m_view.StyleGetFont(STYLE_DEFAULT, faceName);
    StrCopy(cf.szFaceName, AtoW(faceName), LF_FACESIZE);
    cf.yHeight = m_view.StyleGetSize(STYLE_DEFAULT) * 20;
    cf.dwEffects |= m_view.StyleGetBold(STYLE_DEFAULT) ? CFE_BOLD : 0;
    cf.dwEffects |= m_view.StyleGetItalic(STYLE_DEFAULT) ? CFE_ITALIC : 0;
    cf.dwEffects |= m_view.StyleGetUnderLine(STYLE_DEFAULT) ? CFE_UNDERLINE : 0;
    cf.crTextColor = m_view.StyleGetFore(STYLE_DEFAULT);

    // Display the Choose Font dialog, with the current font selected.
    CFontDialog dlg(cf, CF_SCREENFONTS | CF_EFFECTS);

    if (dlg.DoModal(*this) == IDOK)
    {
        // Assign the new font to the rich text document.
        cf = dlg.GetCharFormat();
        m_view.StyleSetFont(STYLE_DEFAULT, WtoA(cf.szFaceName));     // Put this first.
        m_view.StyleSetSize(STYLE_DEFAULT, (cf.yHeight + 10) / 20);  // Put this next.
        m_view.StyleSetFore(STYLE_DEFAULT, cf.crTextColor);
        m_view.StyleSetBold(STYLE_DEFAULT, (cf.dwEffects & CFE_BOLD) != 0);
        m_view.StyleSetItalic(STYLE_DEFAULT, (cf.dwEffects & CFE_ITALIC) != 0);
        m_view.StyleSetUnderLine(STYLE_DEFAULT, (cf.dwEffects & CFE_UNDERLINE) != 0);
        m_view.StyleClearAll();                                      // Put this last.
    }

    return TRUE;
}

BOOL CMainFrame::OnOptionWordWrap()
{
    int mode = m_view.GetWrapMode();
    int newMode = (mode == 0) ? 1 : 0;
    m_view.SetWrapMode(newMode);

    return TRUE;
}

// Called when the Print Preview's "Close" button is pressed.
LRESULT CMainFrame::OnPreviewClose()
{
    // Swap the view
    SetView(m_view);

    // Show the menu and toolbar
    ShowMenu(GetFrameMenu() != nullptr);
    ShowToolBar(m_isToolbarShown);
    UpdateSettings();

    // Restore focus to the window focused before DoPrintPreview was called.
    RestoreFocus();

    SetStatusText(LoadString(IDW_READY));

    return 0;
}

// Called when the Print Preview's "Print Now" button is pressed.
LRESULT CMainFrame::OnPreviewPrint()
{
    m_view.QuickPrint(L"Scintilla Demo");
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
        UINT maxPage = static_cast<UINT>(m_view.CollatePages());
        m_preview.DoPrintPreview(*this, maxPage);
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);
    }

    return 0;
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
        CString empty;

        m_cap = (::GetKeyState(VK_CAPITAL) & 0x0001) ? cap : empty;
        m_num = (::GetKeyState(VK_NUMLOCK) & 0x0001) ? num : empty;
        m_ovr = (::GetKeyState(VK_INSERT) & 0x0001) ? ovr : ins;

        // Update the indicators.
        // Need member variables for owner drawn text to keep them in scope.
        GetStatusBar().SetPartText(1, m_cap, SBT_OWNERDRAW);
        GetStatusBar().SetPartText(2, m_num, SBT_OWNERDRAW);
        GetStatusBar().SetPartText(3, m_ovr, SBT_OWNERDRAW);
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
        SetMenuIcons(data, RGB(192, 192, 192), IDB_MENUICONS);
}

// Set the resource IDs and images for the toolbar buttons.
void CMainFrame::SetupToolBar()
{
    AddToolBarButton(IDM_FILE_NEW);
    AddToolBarButton(IDM_FILE_OPEN);
    AddToolBarButton(IDM_FILE_SAVE);
    AddToolBarButton(IDM_FILE_SAVEAS);

    AddToolBarButton(0);               // Separator
    AddToolBarButton(IDM_EDIT_CUT);    // disabled button
    AddToolBarButton(IDM_EDIT_COPY);   // disabled button
    AddToolBarButton(IDM_EDIT_PASTE);  // disabled button

    AddToolBarButton(0);               // Separator
    AddToolBarButton(IDM_FILE_PRINT);

    AddToolBarButton(0);               // Separator
    AddToolBarButton(IDM_HELP_ABOUT);
}

// Sets the frame's title.
void CMainFrame::SetWindowTitle()
{
    CString title;

    if (m_pathName.IsEmpty())
        title = L"Scintilla Demo";
    else
        title = m_pathName + L" - Scintilla Demo";

    SetWindowText(title);
}

// Called by CTextApp::OnIdle to update toolbar buttons
void CMainFrame::UpdateToolbar()
{
    BOOL isSelected = (m_view.GetSelectionStart() != m_view.GetSelectionEnd());
    BOOL isDirty = m_view.GetModify();
    BOOL canPaste = m_view.CanPaste();

    GetToolBar().EnableButton(IDM_EDIT_COPY, isSelected);
    GetToolBar().EnableButton(IDM_EDIT_CUT, isSelected);
    GetToolBar().EnableButton(IDM_EDIT_PASTE, canPaste);
    GetToolBar().EnableButton(IDM_FILE_SAVE, isDirty);
}

// Process the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_DROPFILES:        return OnDropFiles(msg, wparam, lparam);
        case WM_GETMINMAXINFO:    return OnGetMinMaxInfo(msg, wparam, lparam);
        case UWM_PREVIEWCLOSE:    return OnPreviewClose();
        case UWM_PREVIEWPRINT:    return OnPreviewPrint();
        case UWM_PREVIEWSETUP:    return OnPreviewSetup();
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
