/* [Tab/Indent: 8/8][Line/Box: 80/74]                            (MainFrm.cpp) *
********************************************************************************

    Contents Description: Implementation of the CMainFrame class for a sample
    program demonstrating a custom status bar within the Win32++ Windows
    framework.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"

/*============================================================================*/
    CMainFrame::
CMainFrame()                                                                /*

    Construct mainframe object and the view; load persistent parameters.
*-----------------------------------------------------------------------------*/
{
      //Set m_view as the view window of the frame
    SetView(m_view);
      // Set the registry key name and load the initial window position
    LoadRegistrySettings(_T("Win32++\\StatusBarEx Sample"));
}

/*============================================================================*/
    BOOL CMainFrame::
OnCommand(WPARAM wparam, LPARAM lparam)                                     /*

    Respond to menu and and toolbar messages.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
    switch(id)
    {
        case IDM_FILE_OPEN:      return OnFileOpen();
        case IDM_FILE_SAVE:      return OnFileSave();
        case IDM_FILE_SAVEAS:    return OnFileSave();
        case IDM_FILE_PRINT:     return OnFilePrint();
        case IDM_FILE_EXIT:      return OnFileExit();
        case IDW_VIEW_STATUSBAR: return OnViewStatusBar();
        case IDW_VIEW_TOOLBAR:   return OnViewToolBar();
        case IDM_HELP_ABOUT:     return OnHelp();
    }
    return FALSE;
}

/*============================================================================*/
    int CMainFrame::
OnCreate(CREATESTRUCT& cs)                                                  /*

    Create the mainframe and set the application parameters.
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
    return CFrame::OnCreate(cs);
}

/*============================================================================*/
    BOOL CMainFrame::
OnFileExit()                                                                /*

    Close the application and terminate the mainframe.
*-----------------------------------------------------------------------------*/
{
      // Issue a close request to the frame
    Close();
    return TRUE;
}

/*============================================================================*/
    void CMainFrame::
OnInitialUpdate()                                                           /*

    Perform necessary initializations required to open the frame.
*-----------------------------------------------------------------------------*/
{
    // The frame is now created.
    // Place any additional startup code here.

      // The content of the owner drawn status bar part can be set anywhere
      // according to the needs of the app. We put it here for the demo.
    m_customStatusBar.SetOwnerDrawText(_T(" Owner Draw"));
    TRACE("Frame created\n");
}

/*============================================================================*/
    BOOL CMainFrame::
OnFileOpen()                                                                /*

    Open the application file...
*-----------------------------------------------------------------------------*/
{
      // declare a file-open dialog
    CFileDialog FileDlg(TRUE);
      // bring up the dialog and retrieve the selected filename
    if (FileDlg.DoModal(*this) == IDOK)
    {
        // TODO: Add your own code here. Refer to the tutorial for
        // additional information
    }
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFileSave()                                                                /*

*-----------------------------------------------------------------------------*/
{
      // declare a file-save dialog
    CFileDialog FileDlg(FALSE);
      // bring up the dialog and retrieve the selected filename
    if (FileDlg.DoModal(*this) == IDOK)
    {
        // TODO: Add your own code here. Refer to the tutorial for
        // additional information
    }
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFilePrint()                                                               /*

    Application printout.
*-----------------------------------------------------------------------------*/
{
      // declare up a dialog to choose the printer
    CPrintDialog printdlg;
    try
    {
        INT_PTR result = printdlg.DoModal(*this);
        // Retrieve the printer DC
        CDC dcPrinter = printdlg.GetPrinterDC();

        // TODO: Add your own code here. Refer to the tutorial for
        // additional information

        return (result == IDOK);   // boolean expression
    }

    catch (const CWinException& /* e */)
    {
        // No default printer
        MessageBox(_T("Unable to display print dialog"),
            _T("Print Failed"), MB_OK);
        return FALSE;
    }
}

/*============================================================================*/
    LRESULT CMainFrame::
OnNotify(WPARAM wparam, LPARAM lparam)                                      /*

    Process notification messages sent by child windows.
*-----------------------------------------------------------------------------*/
{
//  switch(((LPNMHDR)lparam)->code)
//  {
//      Add case statements for each notification message here
//  }
      // Some notifications should return a value when handled
    return CFrame::OnNotify(wparam, lparam);
}

/*============================================================================*/
    void CMainFrame::
PreCreate(CREATESTRUCT& cs)                                                 /*

    Adjust the mainframe appearance.
*-----------------------------------------------------------------------------*/

{
    // This function is called before the frame is created.
    // It provides an opportunity to modify the various CREATESTRUCT
    // parameters used in the frame window's creation.

      // Call base clase to set defaults
    CFrame::PreCreate(cs);
      // window size is constant, overwrites registry values
    cs.cx = 640;
    cs.cy = 480;

      // uncomment this for a fixed size window
//  cs.style &= ~WS_THICKFRAME;
}

/*============================================================================*/
    void CMainFrame::
SetStatusIndicators()                                                       /*

    Override the base class method in order to update the custom status bar
    parts.
*-----------------------------------------------------------------------------*/
{
    m_customStatusBar.SetStatusIndicators();
}

/*============================================================================*/
    void CMainFrame::
SetupToolBar()                                                              /*

    Declare the toolbar buttons and set their resource identifiers. Here, the
    buttons are taken from the IDW_MAIN bitmap, in order, all present, and
    without text.
*-----------------------------------------------------------------------------*/
{
    AddToolBarButton(IDM_FILE_NEW);
    AddToolBarButton(IDM_FILE_OPEN);
    AddToolBarButton(IDM_FILE_SAVE);

    AddToolBarButton(0);                       // Separator
    AddToolBarButton(IDM_EDIT_CUT,   FALSE);   // disabled button
    AddToolBarButton(IDM_EDIT_COPY,  FALSE );  // disabled button
    AddToolBarButton(IDM_EDIT_PASTE, FALSE );  // disabled button

    AddToolBarButton(0);                       // Separator
    AddToolBarButton(IDM_FILE_PRINT);

    AddToolBarButton(0);                       // Separator
    AddToolBarButton(IDM_HELP_ABOUT);
}

/*============================================================================*/
    LRESULT CMainFrame::
WndProc(UINT msg, WPARAM wparam, LPARAM lparam)                             /*

    Process the mainframe messages requiring special handling.
*-----------------------------------------------------------------------------*/
{
//  switch (msg)
//  {
//      Add case statements for each message to be handled here
//  }

    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}
/*----------------------------------------------------------------------------*/