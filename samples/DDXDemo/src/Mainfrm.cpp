/* (20-Oct-2024) [Tab/Indent: 8/8][Line/Box: 80/74]              (MainFrm.cpp) *
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

static const LPCWSTR registryKeyName = L"Win32++\\DDX-DDVDemo";

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
Create(HWND parent)                                                          /*

    Create the frame window and load the persistent frame and view parameters
    that will have been saved in previous invocations of the program via
    the registry in the key section 'Software\<key name>\Frame Settings'.
    Also load the document's saved parameters from the
    'Software\<key name>\Document Settings' key.

    Note: the <key name> used here refers to the registerKeyName above.
* ---------------------------------------------------------------------------- - */
{
      // Set m_view as the view window of the frame.
    SetView(m_view);

    LoadRegistrySettings(registryKeyName);

    return CFrame::Create(parent);
}

/*============================================================================*/
    BOOL CMainFrame::
LoadRegistrySettings(LPCWSTR keyName)                                     /*

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
BOOL CMainFrame::
    OnCheckButton(UINT id)                                                  /*

    Toggle the check state of the button with the specified id.
*-----------------------------------------------------------------------------*/
{
    m_view.SetCheck(id - IDC_CHECK_A);
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
OnCommand(WPARAM wparam, LPARAM)                                     /*

    The framework calls this member function when the user selects an item
    from a menu, when a child control sends a notification message, or when
    an accelerator keystroke is translated.
*-----------------------------------------------------------------------------*/
{
    UINT id = LOWORD(wparam);
    switch(id)
    {
        case IDM_EDIT_CUT:         return OnEditCut();
        case IDM_EDIT_COPY:        return OnEditCopy();
        case IDM_EDIT_DELETE:      return OnEditDelete();
        case IDM_EDIT_PASTE:       return OnEditPaste();
        case IDM_EDIT_REDO:        return OnEditRedo();
        case IDM_EDIT_UNDO:        return OnEditUndo();
        case IDM_FILE_EXIT:        return OnFileExit();
        case IDW_ABOUT:            return CFrame::OnHelp();
        case IDW_VIEW_STATUSBAR:   return OnViewStatusBar();
        case IDW_VIEW_TOOLBAR:     return OnViewToolBar();

        case IDC_CHECK_A:
        case IDC_CHECK_B:
        case IDC_CHECK_C:          return OnCheckButton(id);

        case IDC_RADIO_A:
        case IDC_RADIO_B:
        case IDC_RADIO_C:          return OnRadioButton(id);
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

    // UseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar.
    // UseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar.
    // UseOwnerDrawnMenu(FALSE);     // Don't use owner draw for popup menu items.
    // UseReBar(FALSE);              // Don't use a ReBar.
    // UseStatusBar(FALSE);          // Don't use a StatusBar.
    // UseThemes(FALSE);             // Don't use themes.
    // UseToolBar(FALSE);            // Don't use a ToolBar.

      // call the base class function
    int rtn = CFrame::OnCreate(cs);

      // show the initial document and status
    UpdateDialog(SENDTOCONTROL);
    return rtn;
}

/*============================================================================*/
    BOOL CMainFrame::
OnEditCopy()                                                                /*

    Copy text to the control with keyboard focus.
*-----------------------------------------------------------------------------*/
{
    GetFocus().SendMessage(WM_COPY, 0, 0);
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnEditCut()                                                                /*

    Cut text from the control with keyboard focus.
*-----------------------------------------------------------------------------*/
{
    GetFocus().SendMessage(WM_CUT, 0, 0);
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnEditDelete()                                                                /*

    Delete text from the control with keyboard focus.
*-----------------------------------------------------------------------------*/
{
    GetFocus().SendMessage(WM_CLEAR, 0, 0);
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnEditPaste()                                                                /*

    Paste text to the control with keyboard focus.
*-----------------------------------------------------------------------------*/
{
    GetFocus().SendMessage(WM_PASTE, 0, 0);
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnEditRedo()                                                                /*

    Redo the last text operation for the control with keyboard focus.
*-----------------------------------------------------------------------------*/
{
    GetFocus().SendMessage(EM_REDO, 0, 0);
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnEditUndo()                                                                /*

    Undo the last text operation for the control with keyboard focus.
*-----------------------------------------------------------------------------*/
{
    GetFocus().SendMessage(EM_UNDO, 0, 0);
    return TRUE;
}

/*============================================================================*/
    BOOL CMainFrame::
OnFileExit()                                                                /*

    Close the application.
*-----------------------------------------------------------------------------*/
{
      // Issue a close request to the frame
    Close();
    return TRUE;
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
OnMenuUpdate(UINT id)                                                       /*

    This method is called by the framework before the menu items are
    displayed. Add code here to update the check state of menu items.
*-----------------------------------------------------------------------------*/
{
    switch (id)
    {
    case IDC_CHECK_A:
    {
        bool isCheckA = (m_view.GetCheckA() != 0);
        UINT check = isCheckA ? MF_CHECKED : MF_UNCHECKED;
        GetFrameMenu().CheckMenuItem(id, check);
    }
    break;
    case IDC_CHECK_B:
    {
        bool isCheckB = (m_view.GetCheckB() != 0);
        UINT check = isCheckB ? MF_CHECKED : MF_UNCHECKED;
        GetFrameMenu().CheckMenuItem(id, check);
    }
    break;
    case IDC_CHECK_C:
    {
        bool isCheckC = (m_view.GetCheckC() != 0);
        UINT check = isCheckC ? MF_CHECKED : MF_UNCHECKED;
        GetFrameMenu().CheckMenuItem(id, check);
    }
    break;
    }

    if ((id >= IDC_RADIO_A) && (id <= IDC_RADIO_C))
    {
        int radio = id - IDC_RADIO_A;
        if (m_view.GetRadio() == radio)
            GetFrameMenu().CheckMenuRadioItem(IDC_RADIO_A, IDC_RADIO_C,
                id, MF_BYCOMMAND);
    }

    CFrame::OnMenuUpdate(id);
}

/*============================================================================*/
BOOL CMainFrame::
    OnRadioButton(UINT id)                                                        /*

    Select the radio button with the specified id.
*-----------------------------------------------------------------------------*/
{
    m_view.SetRadio(id - IDC_CHECK_A);
    return TRUE;
}

/*============================================================================*/
void CMainFrame::
    SetupMenuIcons()                                                              /*

        Specify the bitmap for the menu icons.
    *-----------------------------------------------------------------------------*/
{
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        SetMenuIcons(data, RGB(192, 192, 192), IDW_MAIN);
    else
        SetMenuIcons(data, RGB(192, 192, 192), IDB_TOOLBAR16);
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
WndProc(UINT msg, WPARAM wparam, LPARAM lparam)                             /*

    Handle the window's messages
------------------------------------------------------------------------------*/
{
    try
    {
        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

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
