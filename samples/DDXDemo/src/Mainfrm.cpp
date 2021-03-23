/* (12-Jun-2015) [Tab/Indent: 8/8][Line/Box: 80/74]              (MainFrm.cpp) *
********************************************************************************
|                                                                              |
|                      Author: Robert C. Tausworthe, 2020                      |
|                                                                              |
===============================================================================*

    Contents Description: The DDXDemo CMainFrame class implementation.
    This class controls the appearance and frame-related actions of the
    DDXDemo sample program. This class is a modified version of that found
    in the FormDocView sample distributed with the Win32++ Windows interface
    classes. This class presents a fixed-size frame whose status bar has no
    resizing  gripper in the lower-right-hand corner.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

    Acknowledgement: The author would like to thank and acknowledge the advice,
    critical review, insight, and assistance provided by David Nash in the
    development of this work.

*******************************************************************************/

#include "stdafx.h"
#include "Mainfrm.h"
#include "App.h"
#include "resource.h"

static const LPCTSTR registryKeyName = _T("Win32++\\DDX-DDVDemo");

/*============================================================================*/
    CMainFrame::
CMainFrame()                                                                /*

    Construct the CMainFrame.
*-----------------------------------------------------------------------------*/
    : m_view(IDD_MAIN_DIALOG)
{
}

/*============================================================================*/
    HWND CMainFrame::
Create(HWND parent)                                                         /*

    Create the frame window and load the persistent frame and view parameters
    that will have been saved in previous invocations of the program via
    the registry in the key section 'Software\<key name>\Frame Settings'.
    Also load the document's saved parameters from the
    'Software\<key name>\Document Settings' key.

    Note: the <key name> used here refers to the registerKeyName above.
* ---------------------------------------------------------------------------- - */
{
      // Set m_View as the view window of the frame.
    SetView(m_view);
    LoadRegistrySettings(registryKeyName);

    return CFrame::Create(parent);
}

/*============================================================================*/
    BOOL CMainFrame::
LoadRegistrySettings(LPCTSTR keyName)                                     /*

    Load the main frame and document persistent data from the registry
    section marked by keyName. The frame values were automatically
    deposited into the registry by the base class SaveRegistrySettings(),
    which was called by the framework when the program was last terminated.
    The document data is recovered by a CDoc class method.
*-----------------------------------------------------------------------------*/
{
      // reload previously saved window placement, toolbar/statusbar
      // switches and view settings from the registry entries in key section
      // 'Software\keyName\Frame Settings'
    CFrame::LoadRegistrySettings(keyName);
      // load the saved document entries from the same key
    TheDoc().LoadDocRegistry(keyName);
    return TRUE;
}

/*============================================================================*/
    void CMainFrame::
OnClose()                                                                   /*

    The framework calls this member function as a signal that the CWnd or
    application is to terminate. Retrieve the data in the dialog controls
    and save it if all is well. If an error arises, return with the cursor
    in the offending control so the user can correct it.
*-----------------------------------------------------------------------------*/
{
    if (!UpdateDialog(READFROMCONTROL))
    {     // oops! there is a problem with some of the control data
        TRACE("*** Verification failed ***\n");
        return;  // return control to user to fix this problem
    }
      // save the document contents into the registry
    TheDoc().SaveDocRegistry(GetRegistryKeyName().c_str());
    TRACE("Data saved to registry\n");
      // the base class calls SaveRegistrySettings() and DestroyWindow()
    CFrame::OnClose();
}

/*============================================================================*/
    BOOL CMainFrame::
OnCommand(WPARAM wparam, LPARAM lparam)                                     /*

    The framework calls this member function when the user selects an item
    from a menu, when a child control sends a notification message, or when
    an accelerator keystroke is translated.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
    switch(id)
    {
        case IDM_FILE_EXIT:
            OnFileExit();
            return TRUE;

        case IDW_VIEW_STATUSBAR:
            OnViewStatusBar();
            return TRUE;

        case IDW_VIEW_TOOLBAR:
            OnViewToolBar();
            return TRUE;

        case IDW_ABOUT:
            CFrame::OnHelp();
            return TRUE;

        case WM_CLOSE:
            return TRUE;
    }
      // indicate not processed here
    return FALSE;
}

/*============================================================================*/
    int CMainFrame::
OnCreate(CREATESTRUCT& cs)                                                  /*

    Define the way the frame is created.
*-----------------------------------------------------------------------------*/
{
    // Uncomment the lines below to change frame options.

    // UseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar
    // UseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar
    // UseReBar(FALSE);              // Don't use a ReBar
    // UseStatusBar(FALSE);          // Don't use a StatusBar
    // UseThemes(FALSE);             // Don't use themes
    // UseToolBar(FALSE);            // Don't use a ToolBar

    // call the base class function
    int rtn = CFrame::OnCreate(cs);

    if (IsReBarSupported())
    {
        //Set our theme colors
        SetThemeColors();
    }

      // show the initial document and status
    UpdateDialog(SENDTOCONTROL);
    return rtn;
}

/*============================================================================*/
    void CMainFrame::
OnFileExit()                                                                /*

    Close the application.
*-----------------------------------------------------------------------------*/
{
      // Issue a close request to the frame
    Close();
}

/*============================================================================*/
    void CMainFrame::
OnInitialUpdate()                                                           /*

    This method is called by the framework after the view is first attached
    to the document, but before the view is initially displayed. Add code
    here to perform any one-time initialization.
*-----------------------------------------------------------------------------*/
{
    // The frame is now created.
    // Place any additional startup code here.
}

/*============================================================================*/
    void CMainFrame::
PreCreate(CREATESTRUCT& cs)                                                 /*

    This method is called by the framework before the creation of the
    window attached to this CWnd object.  Use it to modify the CREATESTRUCT
    structure before the window is created.
*-----------------------------------------------------------------------------*/
{
      // set default window placement
    cs.x  = 100;
    cs.y  = 100;
    cs.cx = 950;
    cs.cy = 675;
      // reset frame values with those from the registry, if they exist
    CFrame::PreCreate(cs);

       // specify a title bar and border with a window-menu on the title bar
    cs.style = WS_VISIBLE   // must be present, or screen is blank
        | WS_OVERLAPPED     // implied, the default 0x00000000L.
        | WS_CAPTION        // redundant, but put in anyway
        | WS_SYSMENU        // adds close box at top right
        | WS_MINIMIZEBOX    // adds minimize box to close box
        | WS_THICKFRAME     // enables resizing
        ;
    cs.dwExStyle = WS_EX_CLIENTEDGE;
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
    BOOL CMainFrame::
SetThemeColors()                                                            /*

    Set the colors of each rebar in the frame, i.e., the theme colors.
    These were taken from the Win32++ Themes sample program recommended
    colors for Windows XP, case IDM_BLUE.
*-----------------------------------------------------------------------------*/
{
    SetReBarColors(RGB(150,190,245), RGB(196,215,250), RGB(220,230,250),
        RGB( 70,130,220));

    StatusBarTheme sbt = {TRUE, RGB(150,190,245), RGB(196,215,250)};
    SetStatusBarTheme (sbt);

    ToolBarTheme tbt = {TRUE, RGB(255, 230, 190), RGB(255, 190, 100),
        RGB(255, 140, 40), RGB(255, 180, 80), RGB(128, 128, 255)};
    SetToolBarTheme(tbt);

    MenuTheme mt = {TRUE, RGB(255, 230, 190), RGB(255, 190, 100),
        RGB(150,190,245), RGB(220,230,250), RGB(128, 128, 200)};
    SetMenuTheme(mt);
    RecalcLayout();
    return TRUE;
}

/*============================================================================*/
    void CMainFrame::
SetupToolBar()                                                              /*

    Add the specific buttons to the tool bar.
*-----------------------------------------------------------------------------*/
{
      // Set the Resource IDs for the toolbar buttons
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_OPEN  );
    AddToolBarButton( IDM_FILE_SAVE  );
    AddToolBarButton( 0 );        // Separator
    AddToolBarButton( IDM_EDIT_CUT   );
    AddToolBarButton( IDM_EDIT_COPY  );
    AddToolBarButton( IDM_EDIT_PASTE );
    AddToolBarButton( 0 );        // Separator
    AddToolBarButton( IDM_FILE_PRINT );
    AddToolBarButton( 0 );        // Separator
    AddToolBarButton( IDW_ABOUT );
}

/*============================================================================*/
    BOOL CMainFrame::
UpdateDialog(BOOL bReadFromControl)                                         /*

    Update data items in memory (bReadFromControl is READFROMCONTROL) or in
    the dialog controls (bReadFromControl is SENDTOCONTROL). The list of
    affected controls is specified in the CView::DoDataExchange() method.
*-----------------------------------------------------------------------------*/
{
    return m_view.UpdateDialog(bReadFromControl);
}

/*============================================================================*/
    LRESULT CMainFrame::
WndProc(UINT msg, WPARAM wparam, LPARAM lparam)                            /*

    All CWnd windows direct their messages here. This function can process
    some of these and redirect the remaining messages to the base class
    default handler.
*-----------------------------------------------------------------------------*/
{
//  switch (msg)
//  {
//      // Add case statements for each messages to be handled here
//  }

      // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}
/*----------------------------------------------------------------------------*/
