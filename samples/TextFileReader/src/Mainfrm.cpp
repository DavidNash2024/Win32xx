/* (22-Oct-2024) [Tab/Indent: 4/4][Line/Box: 80/74]              (MainFrm.cpp) *
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
CMainFrame()                                                            /*

    Constructor for CMainFrame, which is called after CFrame's constructor.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
    HWND CMainFrame::
Create(HWND parent)                                                         /*

    Create the frame window.
*-----------------------------------------------------------------------------*/
{
      // Set m_view as the view window of the frame.
    SetView(m_view);

      // Set the registry key name, and load the initial window position.
      // Use a registry key name like "CompanyName\\Application".
    LoadRegistrySettings(L"Win32++\\TextFileReader");

      // Load the settings from the registry with 5 MRU entries.
    LoadRegistryMRUSettings(5);

    return CFrame::Create(parent);
}

/*============================================================================*/
    BOOL CMainFrame::
OnColorChoice()                                                             /*

        Select the view backbround color.
*-----------------------------------------------------------------------------*/
{
    m_view.OnColorChoice();
    Invalidate();
    UpdateWindow();
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnCommand(WPARAM wparam, LPARAM)                                            /*

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
    UINT nID = LOWORD(wparam);

    switch(nID)
    {
    case IDM_COLOR_CHOICE:      return OnColorChoice();
    case IDM_FILE_CLOSE:        return OnFileClose();
    case IDM_FILE_EXIT:         return OnFileExit();
    case IDM_FILE_OPEN:         return OnFileOpen();
    case IDM_FONT_CHOICE:       return OnFontChoice();
    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDM_LINE_NUMBERING:    return OnLineNumbering();
    case IDW_ABOUT:             return OnHelp();

    case IDW_FILE_MRU_FILE1:
    case IDW_FILE_MRU_FILE2:
    case IDW_FILE_MRU_FILE3:
    case IDW_FILE_MRU_FILE4:
    case IDW_FILE_MRU_FILE5:    return OnFileOpenMRU(wparam);
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
    BOOL CMainFrame::
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
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFileExit()                                                                /*

    Close the application and terminate execution.
*-----------------------------------------------------------------------------*/
{
      // Issue a close request to the frame
    Close();
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFileOpen()                                                                /*

    Respond to the open file toolbar button and menu item by prompting for
    and receiving a string file name, opening the file if valid, and
    initiating its display and scrolling, as appropriate.
*-----------------------------------------------------------------------------*/
{
      // Bring up the dialog, and open the file
    CString filter = LoadString(IDS_FILE_FILTER);
    CFileDialog fd(TRUE, nullptr, nullptr, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
    if (fd.DoModal(*this) != IDOK)
        return FALSE;

      // open the document and read its content
    if (!TheDoc().OpenDoc(fd.GetPathName()))
        return FALSE;

    AddMRUEntry(fd.GetPathName());
    SetWindowText(fd.GetPathName());
    m_view.SetAppSize();
    m_view.SetScrollPosition(CPoint(0, 0));
      // show the document
    Invalidate();
    UpdateWindow();
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFileOpenMRU(WPARAM wparam)                                                  /*

    Open the MRU file at nIndex as the next document.
*-----------------------------------------------------------------------------*/
{
      // get the MRU entry
    UINT index = static_cast<UINT>(LOWORD(wparam)) - IDW_FILE_MRU_FILE1;
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

    return TRUE;
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
    void CMainFrame::                                                       /*

    This function is called after the window is created.
*-----------------------------------------------------------------------------*/
OnInitialUpdate()
{
    GetFrameMenu().CheckMenuItem(IDM_LINE_NUMBERING,
        MF_UNCHECKED);
}

/*============================================================================*/
BOOL CMainFrame::
    OnLineNumbering()                                                       /*

    Toggle the text line numbering.
*-----------------------------------------------------------------------------*/
{
    GetFrameMenu().CheckMenuItem(IDM_LINE_NUMBERING, MF_BYCOMMAND |
        (m_view.ToggleLineNumbers() ? MF_CHECKED : MF_UNCHECKED));
    return TRUE;
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
