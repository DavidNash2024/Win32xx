/* (02-Aug-2016) [Tab/Indent: 4/4][Line/Box: 80/74]              (MainFrm.cpp) *
********************************************************************************
|                                                                              |
|                Authors: Robert Tausworthe, David Nash, 2020                  |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CMainFrame class for this sample
    program using the Win32++ Windows interface classes.

     Programming Notes: The programming style roughly follows that established
     got the 1995-1999 Jet Propulsion Laboratory Network Planning and
     Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"
#include "io.h"

/*============================================================================*/
    CMainFrame::
CMainFrame()                                                                /*

    Constructor for CMainFrame, which is called after CFrame's constructor.
*-----------------------------------------------------------------------------*/
{
    m_yWin   = 100;
    m_xWin   = 100;
    m_cxWin  = 400;
    m_cyWin  = 400;
    ZeroMemory(&m_wndPl, sizeof(WINDOWPLACEMENT));
    m_wndPl.length = sizeof(WINDOWPLACEMENT);
}

/*============================================================================*/
    HWND CMainFrame::
Create(HWND parent)                                                         /*

    Create the frame window.
*-----------------------------------------------------------------------------*/
{
      // Set m_View as the view window of the frame
    SetView(m_view);

    return CFrame::Create(parent);
}

/*============================================================================*/
    void CMainFrame::
OnClose()                                                                   /*

    Save the program persistent settings and terminate execution in response to
    the WM_CLOSE message.
*-----------------------------------------------------------------------------*/
{
    SaveSettings();
    CFrame::OnClose();
}

/*============================================================================*/
    void CMainFrame::
OnColorChoice()                                                             /*

        Select the view backbround color.
*-----------------------------------------------------------------------------*/
{
    m_view.OnColorChoice();
    Invalidate();
    UpdateWindow();
}

/*============================================================================*/
    BOOL CMainFrame::
OnCommand(WPARAM wparam, LPARAM lparam)                                     /*

    The framework calls this member function when the user selects an
    item from a menu, when a child control sends a notification message,
    or when an accelerator keystroke is translated. Here, we respond to
    menu selections, toolbar events, scrollbar actions, and accelerator
    keys.

    The low-order word of wParam identifies the command ID of the menu
    or accelerator message. The high-order word is 1 if the message is
    from an accelerator, or 0 if the message is from the menu.

    The method returns nonzero if it processes the message; otherwise it
    returns zero.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(lparam);

    UINT nID = LOWORD(wparam);

      // map all MRU file messages to one representative
    if(IDW_FILE_MRU_FILE1 <= nID &&
        nID < IDW_FILE_MRU_FILE1 + m_maxMRU)
        nID = IDW_FILE_MRU_FILE1;

    switch(nID)
    {
    case IDM_COLOR_CHOICE:
        OnColorChoice();
        return TRUE;

    case IDM_FILE_CLOSE:
        OnFileClose();
        return TRUE;

    case IDM_FILE_EXIT:
        OnFileExit();
        return TRUE;

    case IDM_FILE_OPEN:
        OnFileOpen();
        return TRUE;

    case IDM_FONT_CHOICE:
        OnFontChoice();
        return TRUE;

    case IDW_VIEW_STATUSBAR:
        CFrame::OnViewStatusBar();
        return TRUE;

    case IDW_VIEW_TOOLBAR:
        CFrame::OnViewToolBar();
        return TRUE;

    case IDM_LINE_NUMBERING:
        GetFrameMenu().CheckMenuItem(IDM_LINE_NUMBERING, MF_BYCOMMAND |
            (m_view.ToggleLineNumbers() ? MF_CHECKED : MF_UNCHECKED));
        break;

    case IDW_ABOUT:
        return OnHelp();

    case IDW_FILE_MRU_FILE1:
        OnProcessMRU(wparam, lparam);
        return TRUE;
    }
    return FALSE;
}

/*============================================================================*/
    int CMainFrame::
OnCreate(CREATESTRUCT& cs)                                                  /*

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

      // call the base class function
    CFrame::OnCreate(cs);
      // determine the availability of the archive file
    if (::_taccess(m_arcvPath, 4) != 0)
    {
        CString msg = _T("Default values are being used on this first\n")
            _T("startup. Your customized settings, colors, and font\n")
            _T("will be restored in future usages.\n");
        ::MessageBox(0, msg, _T("Information"), MB_OK |
            MB_ICONINFORMATION | MB_TASKMODAL);
        return 0;
    }
      // get archved values
    try
    {
        SetMRULimit(m_maxMRU);
          // get archived values
        CArchive ar(m_arcvPath, CArchive::load);
        ar >> *TheApp();    // for the app
        ar >> *this;        // for the frame
        ar >> m_view;       // for the view
    }
    catch (const CFileException &e)
    {
        CString text = e.GetText();
        if (!text.IsEmpty())
            text += _T("\n");
        CString msg = (CString)"Error restoring program's previous state.\n" +
            text + e.GetErrorString() + _T("\n") + e.what();
        ::MessageBox(0, msg.c_str(), _T("Exception"),
            MB_OK | MB_ICONSTOP | MB_TASKMODAL);
    }
    catch(...) // catch all other exception events
    {
        CString msg = _T("Program's previous state not restored.\n");
        ::MessageBox(0, msg.c_str(), _T("Unknown Exception"),
            MB_OK | MB_ICONSTOP | MB_TASKMODAL);
    }
    m_wndPl.showCmd = SW_RESTORE;
    SetWindowPlacement(m_wndPl);
    return 0;
}

/*============================================================================*/
    void CMainFrame::
OnFileClose()                                                               /*

    Respond to the close toolbar button and main menu item by closing the
    current document, if one was open.
*-----------------------------------------------------------------------------*/
{
    TheDoc().CloseDoc();
    CString s = LoadString(IDW_MAIN);
    SetWindowText(s);
    m_view.SetAppSize();
    Invalidate();
    UpdateWindow();
}

/*============================================================================*/
    void CMainFrame::
OnFileExit()                                                                /*

    Close the application and terminate execution.
*-----------------------------------------------------------------------------*/
{
      // Issue a close request to the frame
    Close();
}

/*============================================================================*/
    void CMainFrame::
OnFileOpen()                                                                /*

    Respond to the open file toolbar button and menu item by prompting for
    and receiving a string file name, opening the file if valid, and
    initiating its display and scrolling, as appropriate.
*-----------------------------------------------------------------------------*/
{
      // Bring up the dialog, and open the file
    CString filter = LoadString(IDS_FILE_FILTER);
    CFileDialog fd(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
    if (fd.DoModal(*this) != IDOK)
        return;

      // open the document and read its content
    if (!TheDoc().OpenDoc(fd.GetPathName()))
        return;

    AddMRUEntry(fd.GetPathName());
    SetWindowText(fd.GetPathName());
    m_view.SetAppSize();
    m_view.SetScrollPosition(CPoint(0, 0));
      // show the document
    Invalidate();
    UpdateWindow();
}

/*============================================================================*/
    void CMainFrame::
OnFileOpenMRU(UINT index)                                                  /*

    Open the MRU file at nIndex as the next document.
*-----------------------------------------------------------------------------*/
{
      // get the MRU entry
    CString mru_entry = GetMRUEntry(index);
    if (TheDoc().OpenDoc(mru_entry))
    {
        SetWindowText(mru_entry);
        m_view.SetAppSize();
        m_view.SetScrollPosition(CPoint(0, 0));
          // show the document
        Invalidate();
        UpdateWindow();
    }
    else // problem: bad file in MRU
    {
        RemoveMRUEntry(mru_entry);
        OnFileClose();
    }
}

/*============================================================================*/
    BOOL CMainFrame::
OnHelp()                                                                    /*

    Display the application's help dialog box, which contains the program
    information developed in CApp.
*-----------------------------------------------------------------------------*/
{
    m_aboutDialog.DoModal(*this);
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnProcessMRU(WPARAM wparam, LPARAM lparam)                                  /*

    One of the MRU entries has been selected.  Process accordingly.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(lparam);

      // compute the MRU index, where IDW_FILE_MRU_FILE1 is index 0
    UINT MRUIndex = LOWORD(wparam) - IDW_FILE_MRU_FILE1;
    OnFileOpenMRU(MRUIndex);
    return TRUE;
}

/*============================================================================*/
    void CMainFrame::
PreCreate(CREATESTRUCT& cs)                                                 /*

    Set cs members to select window frame parameters desired. This gets
    executed before CScrollWin::PreCreate(). Use the deserialized position
    and size information to set the frame positioning and size.
*-----------------------------------------------------------------------------*/
{
      // do the base class stuff
    CFrame::PreCreate(cs);
    cs.x  = m_xWin;  // set to default values
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
SaveSettings()                                                              /*

    Save the progam's persistent data in preparation for termination.
*-----------------------------------------------------------------------------*/
{
    try
    {
        CArchive ar(m_arcvPath, CArchive::store);
        ar << *TheApp();    // for the app
        ar << *this;        // for the frame
        ar << m_view;       // for the view
    }
    catch (const CFileException &e)
    {
        CString text = e.GetText();
        if (!text.IsEmpty())
            text += _T("\n");
        CString msg = (CString)"Error while saving program's state.\n" + text +
            e.GetErrorString() + _T("\n") + e.what();
        ::MessageBox(0, msg.c_str(), _T("Exception"),
            MB_OK | MB_ICONSTOP | MB_TASKMODAL);
    }
    catch(...) // catch all other exception events
    {
        CString msg = _T("Program's current state not saved.\n");
        ::MessageBox(0, msg.c_str(), _T("Unknown Exception"),
            MB_OK | MB_ICONSTOP | MB_TASKMODAL);
    }
}

/*============================================================================*/
        void CMainFrame::
Serialize(CArchive& ar)                                                     /*

        Called serialize or deserialize the frame to and from the archive ar,
        depending on the sense of IsStoring().  Leave the archive open for
        further serializations.
*-----------------------------------------------------------------------------*/
{
      // perform loading or storing
    ArchiveObject w(&m_wndPl, sizeof(WINDOWPLACEMENT));
    if (ar.IsStoring())
    {
          // save current window placement information
        ZeroMemory(&m_wndPl, sizeof(WINDOWPLACEMENT));
        m_wndPl.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(m_wndPl);
        ar << w;
          // save the base class frame status and  tool bar switches:
          // these control the display of the StatusBar and  ToolBar
        BOOL showbar = GetStatusBar().IsWindowVisible();
        ar << showbar;
        showbar = GetToolBar().IsWindowVisible();
        ar << showbar;
          // save MRU entries
        UINT i, nMRU = static_cast<UINT>(GetMRUEntries().size());
        ar << nMRU;
          // save this many entries (don't use a copied list)
        for (i = 0; i < nMRU; ++i)
        {
            ar << GetMRUEntries()[i];
        }
    }
    else    // recovering
    {
          // recover window frame placement, but do not invoke
          // SetWindowPlacement(), as the window is not yet created.
        ar >> w;
          // recover frame status and  tool bar base class switches
        BOOL showbar;
        ar >> showbar;
        ShowStatusBar(showbar);
        ar >> showbar;
        ShowToolBar(showbar);
          // Read MRU values from archive: use a separate dummy list to
          // work with the entries. First, clear the MRU array (it
          // should already be empty).
        UINT MRU;
        std::vector<CString> MRUEntries;
          // now read from the archive
        ar >> MRU; // the number of entries to read in
        for (UINT i = 0; i < MRU; ++i)
        {
            CString s;
            ar >> s;
            if (i < m_maxMRU)  // keep only those within the limit
                MRUEntries.push_back(s);
        }
          // all successfully read in, so store them LIFO order into
          // the MRU list for proper display
        std::vector<CString>::reverse_iterator it;
        for (it = MRUEntries.rbegin(); it != MRUEntries.rend(); ++it)
        {
            AddMRUEntry(*it);
        }
    }
}

/*============================================================================*/
    void CMainFrame::
SetupMenuIcons()                                                            /*

    Called by the framework to assigns bitmaps to dropdown menu items. By
    default the toolbar button bitmaps are added to menu items by position,
    rather than by command ID. The  base class is overridden here to allow
    images on drop down menu items to be assigned by command ID.  This step
    is necessary when the toolbar buttons are not in the same  order as the
    command IDs.
*-----------------------------------------------------------------------------*/
{
       // determine the number of buttons on the IDW_MAIN toolbar
    CBitmap Bitmap(IDW_MAIN);
    assert (Bitmap.GetHandle());
    BITMAP bm = Bitmap.GetBitmapData();
    int images = bm.bmWidth / bm.bmHeight;
      // make a vector spanning the number of buttons on the toolbar and
      // fill it initially with empty IDs
    std::vector<UINT> menuIDs;
    const int noID = 1;
    menuIDs.assign(images, noID);

      // now fill in just those that are used in the menu using the
      // indexes of the buttons on IDW_MAIN and the command ID in the form:
      // MenuIDs[index] = nCommandID;
    menuIDs[ 1] = IDM_FILE_OPEN;
    menuIDs[ 4] = IDM_FILE_CLOSE;
    menuIDs[ 5] = IDM_FILE_EXIT;
    menuIDs[12] = IDM_FONT_CHOICE;
    menuIDs[13] = IDM_COLOR_CHOICE;
      // replace the current menu image list with this one
    AddMenuIcons(menuIDs, RGB(255, 0, 255), IDW_MAIN, 0);
}

/*============================================================================*/
    void CMainFrame::
SetupToolBar()                                                              /*

    Called from the CFrame::CreateToolBar() function to load the toolbar
    bitmaps, to connect the tool bar buttons to Resource IDs of the
    toolbar buttons, and to define the order of appearance of the buttons
    on the toolbar at runtime.
*-----------------------------------------------------------------------------*/
{
      // Connect button IDs to button icons, show enabled status, and
      // give the explicit image index iImage of each button in the bitmap.
      // Add the toolbar buttons in the order they are to appear at runtime.
    AddToolBarButton(IDM_FILE_OPEN,    TRUE, 0,  1);
    AddToolBarButton(IDM_FILE_CLOSE,   TRUE, 0,  4);
    AddToolBarButton(0);  // Separator
    AddToolBarButton(IDM_FONT_CHOICE,  TRUE, 0, 12);
    AddToolBarButton(IDM_COLOR_CHOICE, TRUE, 0, 13);
    AddToolBarButton(0);  // Separator
      // Set the toolbar image list: use defaults for hot and disabled
    SetToolBarImages(RGB(255, 0, 255), IDB_TOOLBAR, 0, 0);
}
/*----------------------------------------------------------------------------*/
