/* (02-Aug-2014) [Tab/Indent: 8/8][Line/Box: 80/74]              (MainFrm.cpp) *
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
    :   m_maxMRUEntries(5)
{
      // set screen default position and  size
    m_xWin  = 100;
    m_yWin  = 100;
    m_cxWin = 800;
    m_cyWin = 700;
    ZeroMemory(&m_plWnd, sizeof(WINDOWPLACEMENT));
}

/*============================================================================*/
    HWND CMainFrame::
Create(HWND parent)                                                         /*

    Construct and initiallize the CMainFrame object.
*-----------------------------------------------------------------------------*/
{
    // Set m_view as the view window of the frame
    SetView(m_view);

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
OnCommand(WPARAM wparam, LPARAM lparam)                                     /*

    The framework calls this member function when the user selects an
    item from a menu, when a child control sends a notification message,
     or  when an accelerator keystroke is translated. Here, we respond to
    menu selections, toolbar events, scrollbar actions, and  accelerator
    keys.

    Here, the actions are divided into two groups: those that invoke help
    actions, and  those that program invoke actions.

    The method returns nonzero if it processes the message; otherwise it
    returns zero.
*-----------------------------------------------------------------------------*/
{
        UINT lowParam = LOWORD(wparam);
        // map all MRU file messages to one representative
        if (IDW_FILE_MRU_FILE1 <= lowParam &&
            lowParam < IDW_FILE_MRU_FILE1 + m_maxMRUEntries)
            lowParam = IDW_FILE_MRU_FILE1;

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
            case IDW_FILE_MRU_FILE1: OnProcessMRU(wparam, lparam); return TRUE;
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

    // UseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar
    // UseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar
    // UseReBar(FALSE);              // Don't use a ReBar
    // UseStatusBar(FALSE);          // Don't use a StatusBar
    // UseThemes(FALSE);             // Don't use themes
    // UseToolBar(FALSE);            // Don't use a ToolBar

      // call the base class function
    CFrame::OnCreate(cs);
      // Connect the MRU to the frame's menu
    m_MRU.AssignMenu(GetFrameMenu(), m_maxMRUEntries);
      // get archived values
    try
    {
          // open the application's saved parameter archive
        CArchive ar(m_arcName, CArchive::load);
          // recover the frame saved parameters
        ar >> *this;
    }
    catch (const CException& e)
    {
          // Process the exception and  quit
        CString msg;
        CString what(e.what());
        msg.Format(_T("Error restoring previous parameters.\n%s\n%s"),
            e.GetText(), e.GetErrorString());
        ::MessageBox(NULL, msg.c_str(), what.c_str(),
            MB_OK | MB_ICONSTOP | MB_TASKMODAL);
          // remove the corrupted application archive file
        ::DeleteFile(m_arcName);
        m_view.SetDefaults();
    }
    catch(...)
    {
        CString msg = _T("Error restoring previous parameters.\n");
        ::MessageBox(NULL, msg.c_str(), _T("Exception"),
            MB_OK | MB_ICONSTOP | MB_TASKMODAL);
        m_view.SetDefaults();
    }
      // recover the window placement, if read in ok
    if (m_plWnd.length != 0)
    {
        m_plWnd.length = sizeof(WINDOWPLACEMENT);
        m_plWnd.showCmd = SW_RESTORE;
        SetWindowPlacement(m_plWnd);
    }
      // weed out any MRU entries that have disappeared
    m_MRU.ValidateMRU();
    // Show the gripper in the ToolBar
    GetReBar().ShowGripper(GetReBar().GetBand(GetToolBar()), TRUE);
    return 0;
}

/*============================================================================*/
    void CMainFrame::
OnDestroy()                                                                 /*

    Save the mainframe, MRU, and view status parameters before termination.
*-----------------------------------------------------------------------------*/
{
    try
    {
        CArchive ar(m_arcName, CArchive::store);
          // no serialization on Open() error
        ar << *this;  // for the frame
    }
    catch (const CException& e)
    {
          // Process the exception and  quit
        CString msg;
        CString what(e.what());
        msg.Format(_T("Error while saving program settings:\n%s\n%s"),
            e.GetText(), e.GetErrorString());
        ::MessageBox(NULL, msg.c_str(), what.c_str(),
            MB_OK | MB_ICONSTOP | MB_TASKMODAL);
    }
    catch(...)
    {
        CString msg = _T("Error while saving program settings:\n");
        ::MessageBox(NULL, msg.c_str(), _T("Exception"),
            MB_OK | MB_ICONSTOP | MB_TASKMODAL);
    }
    CFrame::OnDestroy();
}

/*============================================================================*/
    void CMainFrame::
OnFileExit()                                                                /*

    Perform whatever functions are necessary, other than Serialize(), as
    it is invoked in response to the WM_CLOSE message that is sent when
    the frame is close.
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
        ThisDoc().GetDocOpenFileName(_T("Name the file to open..."));
    if (str.IsEmpty())
        return;

    if (ThisDoc().OpenDoc(str))
    {
        m_MRU.AddEntry(str);
        SetWindowTitle(str);
    }
    m_view.Invalidate();
}

/*============================================================================*/
    bool CMainFrame::
OnFileOpenMRU(UINT index)                                                   /*

    Open the MRU file at nIndex as the next document.
*-----------------------------------------------------------------------------*/
{
      // get the MRU entry if there is one (str will be empty if not)
    CString str = m_MRU.GetEntry(index);
    if (str.IsEmpty())
        return false;

    if (ThisDoc().OpenDoc(str))
    {         // now it's ok to add it to the top of the MRU list
        m_MRU.AddEntry(str);
        SetWindowTitle(str);
        m_view.Invalidate();
        return true;
    }
    else // if it could not be opened, remove the entry from the MRU list
        m_MRU.RemoveEntry(str.c_str());

    return false;
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
        TheMRU().AddEntry(docPath);
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
        TheMRU().AddEntry(docPath);
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

        catch (CResourceException&)
        {
            ::MessageBox(NULL, _T("Font creation error."),
                _T("Error"), MB_OK | MB_ICONEXCLAMATION |
                MB_TASKMODAL);
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
    void CMainFrame::
PreCreate(CREATESTRUCT& cs)                                                 /*

    Set cs members to select window frame parameters desired. This gets
    executed before CView::PreCreate(). Use the deserialized position
     and  size information to set the frame positioning and  size.
*-----------------------------------------------------------------------------*/
{
      // do the base class stuff
    CFrame::PreCreate(cs);
    cs.x  = m_xWin;  // set to deserialized or default values
    cs.y  = m_yWin;
    cs.cx = m_cxWin;
    cs.cy = m_cyWin;
       // specify a title bar and  border with a window-menu on the title bar
    cs.style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE
        | WS_MINIMIZEBOX    // adds the minimize box
        | WS_MAXIMIZEBOX    // adds the maximize box
        | WS_THICKFRAME     // same as WS_SIZEBOX, enables resizing
        ;
}

/*============================================================================*/
        void CMainFrame::
Serialize(CArchive &ar)                                                     /*

        Called serialize or deserialize the frame to and  from the archive ar,
        depending on the sense of IsStoring().
*-----------------------------------------------------------------------------*/
{
      // perform loading or storing
    if (ar.IsStoring())
    {
          // each item serialized is written to the archive
          // file as a char stream of the proper length,
          // preceded by that length. In some cases, other forms of
          // data are saved, from with the primary items are then
          // reconstructed.

          // save current window placement information
        ZeroMemory(&m_plWnd, sizeof(WINDOWPLACEMENT));
        m_plWnd.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(m_plWnd);
        ArchiveObject w(&m_plWnd, m_plWnd.length);
        ar << w;
          // save the base class frame status and  tool bar switches:
          // these control the display of the StatusBar and  ToolBar

        BOOL showbar = GetStatusBar().IsWindow() && GetStatusBar().IsWindowVisible();
        ar << showbar;
        showbar = GetToolBar().IsWindow() && GetToolBar().IsWindowVisible();
        ar << showbar;
        // save MRU list and view
        ar << m_MRU;
        ar << m_view;

        CString str = _T("Complete");
        ar << str;

    }
    else    // recovering
    {
          // each item deserialized from the archive is
          // retrieved by first reading its length and  then
          // loading in that number of bytes into the data
          // item saved in the archive, as above. Some items require
          // additional conversion procedures, as shown below.

          // recover window frame placement, but do not invoke
          // SetWindowPlacement(), as the window is not yet created.
        ArchiveObject w(&m_plWnd, sizeof(WINDOWPLACEMENT));
        ar >> w;
          // recover frame status and  tool bar base class switches
        BOOL showbar;
        ar >> showbar;
        ShowStatusBar(showbar);
        ar >> showbar;
        ShowToolBar(showbar);
          // no exception having been raised, set frame parameters
        RECT rc = m_plWnd.rcNormalPosition;
        m_xWin = rc.left;
        m_yWin = rc.top;
        m_cxWin = rc.right  - rc.left;
        m_cyWin = rc.bottom - rc.top;
        // load MRU and view parameters
        ar >> m_MRU;
        ar >> m_view;
        // ensure we've read the entire archive
        CString str;
        ar >> str;
        if (str != _T("Complete"))
            throw CUserException(_T("Invalid archive"));
    }
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
SetWindowTitle(const CString &docPath /* = _T("") */)                       /*

    Set the window title to the application base title plus the document
    file name.
*-----------------------------------------------------------------------------*/
{
    CString s = m_appName + _T(":   ") + docPath;
    SetTitle(s);
}
/*----------------------------------------------------------------------------*/
