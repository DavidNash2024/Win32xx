/* (20-Oct-2024) [Tab/Indent: 8/8][Line/Box: 80/74]              (MainFrm.cpp) *
********************************************************************************
|                                                                              |
|               Authors: Robert C. Tausworthe, David Nash, 2020                |
|                                                                              |
===============================================================================*

    Contents Description:  Implementation of the CMainFrm class for this
    application using the Win32++ framework.

    Programming Notes: The programming style roughly follows that established
    for the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "Mainfrm.h"
#include "App.h"
#include "resource.h"

/*******************************************************************************

    Static constants initialization                                         */

  // latest main frame file compilation date
const CString CMainFrame::m_compiledOn = __DATE__;


/*============================================================================*/
    CMainFrame::
CMainFrame()                                                                /*

    Construct and initiallize the CMainFrame object.
*-----------------------------------------------------------------------------*/
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
    LoadRegistrySettings(L"Win32++\\TimeDemo");

    return CFrame::Create(parent);
}

/*============================================================================*/
    void CMainFrame::
OnColorChoice()                                                             /*

    Select the view's backbround color.
*-----------------------------------------------------------------------------*/
{
    CColorDialog ColorDlg(m_view.GetBkgndColor(), CC_RGBINIT | CC_ANYCOLOR);
    ColorDlg.SetCustomColors(m_view.GetColorsArray());
    if (ColorDlg.DoModal(m_view) == IDOK)
    {
        m_view.SetBkgndColor(ColorDlg.GetColor());
    }
    m_view.Invalidate();
}

/*============================================================================*/
    BOOL CMainFrame::
OnCommand(WPARAM wparam, LPARAM)                                     /*

    The framework calls this member function when the user selects an
    item from a menu, when a child control sends a notification message,
     or  when an accelerator keystroke is translated. Here, we respond to
    menu selections, toolbar events, scrollbar actions, and  accelerator
    keys.

    The method returns nonzero if it processes the message; otherwise it
    returns zero.
*-----------------------------------------------------------------------------*/
{
        UINT lowParam = LOWORD(wparam);

        switch (lowParam)
        {
            case IDM_FILE_NEW:       OnFileNew();       return TRUE;
            case IDM_FILE_OPEN:      OnFileOpen();      return TRUE;
            case IDM_FILE_SAVE:      OnFileSave();      return TRUE;
            case IDM_FILE_SAVEAS:    OnFileSaveAs();    return TRUE;
            case IDM_FILE_EXIT:      OnFileExit();      return TRUE;
            case IDM_HELP_ABOUT:     OnHelpAbout();     return TRUE;
            case IDM_COLOR_CHOICE:   OnColorChoice();   return TRUE;
            case IDM_FONT_CHOICE:    OnFontChoice();    return TRUE;
            case IDW_VIEW_TOOLBAR:   OnViewToolBar();   return TRUE;
            case IDW_VIEW_STATUSBAR: OnViewStatusBar(); return TRUE;
        }
        return FALSE;
}


/*============================================================================*/
    int CMainFrame::
OnCreate(CREATESTRUCT& cs)                                                  /*

    This method controls the way the frame is created.
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

      // call the base class function
    CFrame::OnCreate(cs);

    return 0;
}

/*============================================================================*/
    void CMainFrame::
OnFileExit()                                                                /*

    Perform whatever functions are necessary, as it is invoked in
    response to the WM_CLOSE message that is sent when the frame is close.
*-----------------------------------------------------------------------------*/
{
      // Issue a close request to the frame
    Close();
}

/*============================================================================*/
    void CMainFrame::
OnFileNew()                                                                 /*

*-----------------------------------------------------------------------------*/
{
      // Refill the document with the initial document and empty the
      // document file name.
    ThisDoc().NewDocument();
    m_view.SetAppSize();
    CString title = LoadString(IDW_MAIN);
    SetTitle(title);
}

/*============================================================================*/
    void CMainFrame::
OnFileOpen()                                                                /*

    Bring up the open file dialog to choose a document file to open.
*-----------------------------------------------------------------------------*/
{
    // Bring up the dialog, and  open the file
    CString str =
        ThisDoc().GetDocOpenFileName(L"Name the file to open...");
    if (str.IsEmpty())
        return;

    if (ThisDoc().OpenDoc(str))
    {
        SetWindowTitle(str);
    }
    m_view.Invalidate();
}

/*============================================================================*/
    void CMainFrame::
OnFileSave()                                                                /*

    Save the current document.
*-----------------------------------------------------------------------------*/
{
    if (ThisDoc().SaveDoc())
    {
        CString docPath = ThisDoc().GetDocPath();
        SetWindowTitle(docPath);
    }
}

/*============================================================================*/
    void CMainFrame::
OnFileSaveAs()                                                              /*

    Engage the file save dialog to obtain the file name to be used in
    saving the current document and  to retain that new document as the
    current one.
*-----------------------------------------------------------------------------*/
{
    if (ThisDoc().SaveDocAs())
    {
        CString docPath = ThisDoc().GetDocPath();
        SetWindowTitle(docPath);
    }
}

/*============================================================================*/
    void CMainFrame::
OnFontChoice()                                                              /*

        Select the app font typeface, characteristics, and  color. The font
        background color is always the same as the client area background.
*-----------------------------------------------------------------------------*/
{
    LOGFONT lf = m_view.GetTextFont().GetLogFont();
    DWORD dwFlags = CF_EFFECTS | CF_SCREENFONTS;

    CFontDialog FontDlg(lf, dwFlags);
    FontDlg.SetColor(m_view.GetTextColor());

    if (FontDlg.DoModal(m_view) == IDOK)
    {
        lf = FontDlg.GetLogFont();
        CFont f;
        try
        {
            f.CreateFontIndirect(lf);
            m_view.SetTextFont(f);
            m_view.SetTextColor(FontDlg.GetColor());
        }

        catch (const CResourceException&)
        {
            ::MessageBox(nullptr, L"Font creation error.", L"Error",
                MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
        }
    }

    m_view.SetAppSize(TRUE);  // document extents will change
}

/*============================================================================*/
    BOOL CMainFrame::
OnHelpAbout()                                                               /*

    Display the application's help dialog box, which contains the app
    name, copyright information, and  date of most recent compilation.
*-----------------------------------------------------------------------------*/
{
      // Ensure that only one About dialog displays even for multiple
      // activation commands
    if (!m_aboutDialog.IsWindow())
    {
        m_aboutDialog.DoModal(*this);
    }
    return TRUE;
}

/*============================================================================*/
    void CMainFrame::
OnInitialUpdate()                                                           /*

    The frame has now been created. Provide any desired main frame
    formatting.
*-----------------------------------------------------------------------------*/
{
      // Initialize view scrolling
    m_view.SetAppSize();

    TRACE("Frame created\n");
}

/*============================================================================*/
    void CMainFrame::
SetupMenuIcons()                                                            /*

Called from the CFrame::OnCreate() function to load the menu icons.
*-----------------------------------------------------------------------------*/
{
      // Specify the bitmap and mask for the menu icons.
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        SetMenuIcons(data, RGB(255, 0, 255), IDW_MAIN);
    else
        SetMenuIcons(data, RGB(255, 0, 255), IDB_MENUICONS);
}

/*============================================================================*/
    void CMainFrame::
SetupToolBar()                                                              /*

    Called from the CFrame::CreateToolBar() function to load the toolbar
    bitmaps, to connect the tool bar buttons to Resource IDs of the
    toolbar buttons, and  to define the order of appearance of the buttons
    on the toolbar at runtime.
*-----------------------------------------------------------------------------*/
{
      // Connect button IDs to button icons, show enabled status,  and
      // give the explicit image index iImage of each button in the bitmap.
      // Add the toolbar buttons in the order they are to appear at runtime.
    AddToolBarButton(IDM_FILE_NEW);
    AddToolBarButton(IDM_FILE_OPEN);
    AddToolBarButton(IDM_FILE_SAVE);
    AddToolBarButton(0);  // Separator
    AddToolBarButton(IDM_FONT_CHOICE);
    AddToolBarButton(IDM_COLOR_CHOICE);
    AddToolBarButton(0);  // Separator
    AddToolBarButton(IDM_HELP_ABOUT);
    // Specify the toolbar bitmap and color mask.
    SetToolBarImages(RGB(255, 0, 255), IDW_MAIN, 0, 0);
}

/*============================================================================*/
    void CMainFrame::
SetWindowTitle(const CString &docPath /* = L"" */)                          /*

    Set the window title to the application base title plus the document
    file name.
*-----------------------------------------------------------------------------*/
{
    CString s = m_appName + L":   " + docPath;
    SetTitle(s);
}

/*============================================================================*/
    LRESULT CMainFrame::
WndProc(UINT msg, WPARAM wparam, LPARAM lparam)                             /*

    Handle the window's messages.
*-----------------------------------------------------------------------------*/

{
    try
    {
        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L"\n" << e.GetErrorString();
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