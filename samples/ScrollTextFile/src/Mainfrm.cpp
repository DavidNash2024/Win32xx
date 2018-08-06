/* (02-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]              (MainFrm.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2017, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of a basic CMainFrame class for the
    ScrollWin demo application using the Win32++ Windows interface classes,
    Copyright (c) 2005-2017 David Nash, under permissions granted therein.

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
    citation of the author's work should be included in such usages.

    Special Conventions:

    Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Network Planning
        and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"

  // latest file compilation date
CString CMainFrame::m_sCompiled_on = __DATE__;

/*============================================================================*/
    CDoc&
TheDoc()                                /*

    Global function to access to the CDoc class.
*-----------------------------------------------------------------------------*/
{
    return TheApp().TheFrame().TheDoc();
}

/*******************************************************************************

    Implementation of the Main Frame class
    
*=============================================================================*/
    CMainFrame::
CMainFrame()                                /*

    Constructor for CMainFrame, which is called after CFrame's constructor.
*-----------------------------------------------------------------------------*/
{
    m_win_y      = 100;
    m_win_x      = 100;
    m_win_width  = 400;
    m_win_height = 400;
    ZeroMemory(&m_Wndpl, sizeof(WINDOWPLACEMENT));
      // Set m_View as the view window of the frame
    SetView(m_View);
}

/*============================================================================*/
    CMainFrame::
~CMainFrame()                                                           /*

     Destructor for CMainFrame.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
    void CMainFrame::
OnColorChoice()                                                     /*

        Select the view backbround color.
*-----------------------------------------------------------------------------*/
{
    m_View.OnColorChoice();
    Invalidate();
    UpdateWindow();
}

/*============================================================================*/
    BOOL CMainFrame::
OnCommand(WPARAM wParam, LPARAM lParam)                 /*

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
    UNREFERENCED_PARAMETER(lParam);

    UINT nID = LOWORD(wParam);

      // map all MRU file messages to one representative
    if(IDW_FILE_MRU_FILE1 <= nID &&
        nID < IDW_FILE_MRU_FILE1 + m_nMaxMRU)
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

        case IDW_ABOUT:
            return OnHelp();
            
        case IDW_FILE_MRU_FILE1:
            OnProcessMRU(wParam, lParam);
            return TRUE;
    }

    return FALSE;
}


/*============================================================================*/
    int CMainFrame::
OnCreate(CREATESTRUCT& cs)                                               /*

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
    int rtn = CFrame::OnCreate(cs);
      // make sure it created the frame
    if (rtn != 0)
        return rtn;

      // determine the availability of the archive file
    if (::_taccess(TheApp().GetArcvPath(), 4) != 0)
    {
        CString msg = _T("Default values are being used on this first\n")
            _T("startup. Your customized settings, colors, and font\n")
            _T("will be restored in future usages.\n");
        ::MessageBox(NULL, msg, _T("Information"), MB_OK |
            MB_ICONINFORMATION | MB_TASKMODAL);
        return rtn;
    }
      // get archved values
    try
    {
        SetMRULimit(m_nMaxMRU);
          // get archived values
        CArchive ar(TheApp().GetArcvPath(), CArchive::load);
              // deserialize in the same order as serialized
        ar >> TheApp(); // for the app
        ar >> *this;    // for the frame
        ar >> m_View;   // for the view
          // deserialization complete, ar closes on destruction
    }
    catch (const CWinException &e)  // catch all std::exception events
    {
          // Process the exception and quit
        CString msg = (CString)"Error reading in frame parameters.\n"
            + e.GetErrorString() + EOLN + e.what();
        ::MessageBox(NULL, msg.c_str(), _T("Exception"),
            MB_OK | MB_ICONSTOP | MB_TASKMODAL);
    }
    catch(...) // catch all other exception events
    {
        CString msg = _T("Previous state not restored.\n");
        ::MessageBox(NULL, msg.c_str(), _T("Unknown Exception"),
            MB_OK | MB_ICONSTOP | MB_TASKMODAL);
    }
    if (m_Wndpl.length isnt 0)
    {
        m_Wndpl.length = sizeof(WINDOWPLACEMENT);
        SetWindowPlacement(m_Wndpl);
    }
    return rtn;
}

/*============================================================================*/
    void CMainFrame::
OnFileClose()                               /*

    Respond to the close toolbar button and main menu item by closing the
    current document, if one was open.
*-----------------------------------------------------------------------------*/
{
    TheDoc().CloseDoc();
    CString s = LoadString(IDW_MAIN);
    SetWindowText(s);
    m_View.SetNewAppSize();
    Invalidate();
    UpdateWindow();
}

/*============================================================================*/
    void CMainFrame::
OnFileExit()                                /*

    Close the application and terminate execution.
*-----------------------------------------------------------------------------*/
{
      // Issue a close request to the frame
    SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
}

/*============================================================================*/
    void CMainFrame::
OnFileOpen()                                                            /*

    Respond to the open file toolbar button and menu item by prompting for
    and receiving a string file name, opening the file if valid, and 
    initiating its display and scrolling, as appropriate.
*-----------------------------------------------------------------------------*/
{
    // Bring up the dialog, and open the file
    CFileDialog fd(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        TheDoc().GetFilter());
    if (!(fd.DoModal(*this) == IDOK))
    {
        ::MessageBox(NULL, _T("No file chosen, no action taken."),
            _T("Information"), MB_OK | MB_ICONINFORMATION |
            MB_TASKMODAL);
        return;
    }
      // open the document and read its content
    if (!TheDoc().OpenDoc(fd.GetPathName()))
    {
        ::MessageBox(NULL, _T("File could not be opened and read."),
            _T("Error"), MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
        return;
    }
    AddMRUEntry(fd.GetPathName());
    SetWindowText(fd.GetPathName());
    m_View.SetNewAppSize();
    m_View.SetScrollPosition(CPoint(0, 0));
      // show the document
    Invalidate();
    UpdateWindow();
}

/*============================================================================*/
    void CMainFrame::
OnFileOpenMRU(UINT nIndex)                      /*

    Open the MRU file at nIndex as the next document.
*-----------------------------------------------------------------------------*/
{
      // get the MRU entry
    CString mru_entry = GetMRUEntry(nIndex);
    if (m_Doc.OpenDoc(mru_entry))
    {
        SetWindowText(mru_entry);
        m_View.SetNewAppSize();
        m_View.SetScrollPosition(CPoint(0, 0));
          // show the document
        Invalidate();
        UpdateWindow();
    }
    else // problem: bad file in MRU
    {
        RemoveMRUEntry(mru_entry);
        ::MessageBox(NULL, _T("File could not be opened and read."), 
              _T("Error"), MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
        OnFileClose();
    }
}

/*============================================================================*/
    BOOL CMainFrame::
OnHelp()                                                /*

    Display the application's help dialog box, which contains the program
    information developed in CApp.
*-----------------------------------------------------------------------------*/
{
    m_AboutDialog.SetAboutBoxInfo(TheApp().GetAboutBoxInfo());
      // Ensure that only one About dialog displays even for multiple
      // activation commands
    if (!m_AboutDialog.IsWindow())
    {
        // Store the window handle that currently has keyboard focus
        HWND hPrevFocus = ::GetFocus();
        if (hPrevFocus == GetMenuBar().GetHwnd())
            hPrevFocus = m_hWnd;

          // make the AboutBox modeless
        m_AboutDialog.Create(*this);

        ::SetFocus(hPrevFocus);
    }
    return TRUE;
}

/*============================================================================*/
    void CMainFrame::
OnInitialUpdate()                                                       /*

    The frame has now been created. Provide any desired main frame
    formatting.
*-----------------------------------------------------------------------------*/
{
    m_nMaxMRU = TheApp().GetMaxMRUSlots();
      // Give keyboard focus to the view window:
    m_View.SetFocus();
    // show initial button status
    TRACE("Frame created\n");
}

/*============================================================================*/
    void CMainFrame::
OnMenuUpdate(UINT nID)                          /*

    This method is called by the framework just before a menu is displayed. 
    It can be augmented to update objects within the main frame or to enable 
    and disable menu items as appropriate.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(nID);
      // Update the check state of the various menu items
    CFrame::OnMenuUpdate(IDW_VIEW_STATUSBAR);
    CFrame::OnMenuUpdate(IDW_VIEW_TOOLBAR);

//  switch (nID)
//  {
//      default:
//          break;
//  }
}

/*============================================================================*/
    LRESULT CMainFrame::
OnNotify(WPARAM wParam, LPARAM lParam)                                  /*

    The framework calls this member method to inform the parent window of a 
    control that an event has occurred in the control or that the control 
    requires some kind of information.
*-----------------------------------------------------------------------------*/
{
      // Process notification messages sent by child windows
//  switch(((LPNMHDR)lParam)->code)
//  {
//      // Add case statements for each notification message here
//  }

    // Some notifications should return a value when handled
    return CFrame::OnNotify(wParam, lParam);
}

/*============================================================================*/
    BOOL CMainFrame::
OnProcessMRU(WPARAM wParam, LPARAM lParam)                              /*

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
    void CMainFrame::
PreCreate(CREATESTRUCT& cs)                                             /*

    Set cs members to select window frame parameters desired. This gets
    executed before CScrollWin::PreCreate(). Use the deserialized position
    and size information to set the frame positioning and size.
*-----------------------------------------------------------------------------*/
{
      // do the base class stuff
    CFrame::PreCreate(cs);
    cs.x  = m_win_x;  // set to default values
    cs.y  = m_win_y;
    cs.cx = m_win_width;
    cs.cy = m_win_height;
       // specify a title bar and  border with a window-menu on the title bar
    cs.style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE
        | WS_MINIMIZEBOX    // adds the minimize box
        | WS_MAXIMIZEBOX    // adds the maximize box
        | WS_THICKFRAME     // same as WS_SIZEBOX, enables resizing
        ;
}

/*============================================================================*/
    BOOL CMainFrame::
PreTranslateMessage(MSG &Msg)                                           /*

    Pretranslates the system message uMsg from the mainframe message loop
    before they are dispatched to theTranslateMessage and DispatchMessage
    Windows functions in the message loop and before they are dispached
    to the PreTranslateMessage function of CWnd. MSG contains the message
    to process. Return a nonzero if the message was translated and should
    not be dispatched; return 0 if the message was not translated and
    should be dispatched.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(Msg);
//  HWND   hwnd = Msg->hwnd;
//  UINT   message  = Msg->message;
//  WPARAM wParam   = Msg->wParam;
//  LPARAM lParam   = Msg->lParam;
//  DWORD  time = Msg->time;
//  CPoint  pt  = Msg->pt;

      // return 0 if the message was NOT handled here
    return 0;
}

/*============================================================================*/
    BOOL CMainFrame::
SaveRegistrySettings()                                                  /*

    This member is called in response to a WM_CLOSE message for the
    frame.  The base class saves settings into the registry. In this
    override, the archive member is called to save everything into
    the archive file
*-----------------------------------------------------------------------------*/
{
    try
    {
          // get archived values
        CArchive ar(TheApp().GetArcvPath(), CArchive::store);
              // deserialize in the same order as serialized
        ar << TheApp(); // for the app
        ar << *this;    // for the frame
        ar << m_View;   // for the view
          // deserialization complete, ar closes on destruction
    }
    catch (const CWinException &e)  // catch all std::exception events
    {
          // Process the exception and quit
        CString msg = (CString)"Error writing in program parameters.\n"
            + e.GetErrorString() + EOLN + e.what();
        ::MessageBox(NULL, msg.c_str(), _T("Exception"),
            MB_OK | MB_ICONSTOP | MB_TASKMODAL);
    }
    catch(...) // catch all other exception events
    {
        CString msg = _T("Previous state not recorded.\n");
        ::MessageBox(NULL, msg.c_str(), _T("Unknown Exception"),
            MB_OK | MB_ICONSTOP | MB_TASKMODAL);
    }
    return TRUE;
}

/*============================================================================*/
        void CMainFrame::
Serialize(CArchive& ar)                                               /*

        Called serialize or deserialize the frame to and from the archive ar,
        depending on the sense of IsStoring().  Leave the archive open for
        further serializations.
*-----------------------------------------------------------------------------*/
{
      // perform loading or storing
    ArchiveObject w(&m_Wndpl, sizeof(WINDOWPLACEMENT));
        if (ar.IsStoring())
        {
                   // save current window placement information
        ZeroMemory(&m_Wndpl, sizeof(WINDOWPLACEMENT));
        m_Wndpl.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(m_Wndpl);
        ar << w;
          // save the base class frame status and  tool bar switches:
          // these control the display of the StatusBar and  ToolBar
        BOOL showbar = GetStatusBar().IsWindowVisible();
        ar << showbar;
        showbar = GetToolBar().IsWindowVisible();
        ar << showbar;
          // save MRU entries
        UINT i, nMRU = GetMRUEntries().size();
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
        UINT i, nMRU;
        std::vector<CString> vMRUEntries = GetMRUEntries();
        std::vector<CString>::const_iterator it;
        for (it = vMRUEntries.begin(); it != vMRUEntries.end(); ++it)
            RemoveMRUEntry(*it);
          // now read from the archive
        ar >> nMRU; // the number of entries to read in
        for (i = 0; i < nMRU; ++i)
        {
            CString s;
            ar >> s;
            if (i < m_nMaxMRU)  // keep only those within the limit
                vMRUEntries.push_back(s);
        }
          // all successfully read in, so store them LIFO order into
          // the MRU list for proper display
        for (i = 0; i < nMRU; ++i)
        {
            CString s = vMRUEntries[nMRU - 1 - i];
            AddMRUEntry(s);
        }
        }
}

/*============================================================================*/
    void CMainFrame::
SetupMenuIcons()                            /*

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
    int nImages = bm.bmWidth / bm.bmHeight;
      // make a vector spanning the number of buttons on the toolbar and 
      // fill it initially with empty IDs
    std::vector<UINT> MenuIDs;
    const int NO_ID = 1;
    MenuIDs.assign(nImages, NO_ID);

      // now fill in just those that are used in the menu using the 
      // indexes of the buttons on IDW_MAIN and the command ID in the form:
      // MenuIDs[index] = nCommandID;
    MenuIDs[ 1] = IDM_FILE_OPEN;
    MenuIDs[ 4] = IDM_FILE_CLOSE;
    MenuIDs[ 5] = IDM_FILE_EXIT;
    MenuIDs[12] = IDM_FONT_CHOICE;
    MenuIDs[13] = IDM_COLOR_CHOICE;
      // replace the current menu image list with this one
    AddMenuIcons(MenuIDs, RGB(255, 0, 255), IDW_MAIN, 0);
}

/*============================================================================*/
    void CMainFrame::
SetupToolBar()                                                          /*

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

/*============================================================================*/
    LRESULT CMainFrame::
WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)                        /*

    This is the main frame message loop. By default, it handles the
    normal housekeeping functions (see Win32++\include\frame.h).
*-----------------------------------------------------------------------------*/
{
    switch (uMsg)
    {
        // Add case statements for each message to be handled here

        case WM_SYSCOMMAND:
        {
        switch (LOWORD(wParam))
        {
            case SC_MINIMIZE:
              // save current window placement data before minimize
            m_Wndpl.length = sizeof(WINDOWPLACEMENT);
            GetWindowPlacement(m_Wndpl);
            break;

            case SC_MAXIMIZE:
              // save current window placement data before maximize
            m_Wndpl.length = sizeof(WINDOWPLACEMENT);
            GetWindowPlacement(m_Wndpl);
            break;

            case SC_RESTORE:
              // restore window parameters before minimize/maximize
            m_Wndpl.length = sizeof(WINDOWPLACEMENT);
              // if restoring from maximized state, restore
              // normal placement indicators
            if (IsZoomed())
            {
                m_Wndpl.flags  = WPF_SETMINPOSITION;
                m_Wndpl.showCmd = SW_SHOWNORMAL;
            }
            return SetWindowPlacement(m_Wndpl);
        }
        }
    }

      // pass unhandled messages on for default processing
    return WndProcDefault(uMsg, wParam, lParam);
}

