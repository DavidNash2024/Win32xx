/////////////////////////////////////////
// Mainfrm.h
// Authors: Robert Tausworthe, David Nash
//


#include "stdafx.h"
#include "Mainfrm.h"
#include "App.h"
#include "resource.h"

static const LPCWSTR registryKeyName = L"Win32++\\DDX-DDVDemo";

///////////////////////////////////
// CMainFrame function definitions.
//

// Construct the CMainFrame.
CMainFrame::CMainFrame()
    : m_view(IDD_MAIN_DIALOG)
{
}

// Create the frame window and load the persistent frame and view parameters
// that will have been saved in previous invocations of the program via
// the registry in the key section 'Software\<key name>\Frame Settings'.
HWND CMainFrame::Create(HWND parent)
{
      // Set m_view as the view window of the frame.
    SetView(m_view);

    LoadRegistrySettings(registryKeyName);

    return CFrame::Create(parent);
}

// Load the main frame and document persistent data from the registry
// section marked by keyName.The frame values were automatically
// deposited into the registry by the base class SaveRegistrySettings(),
// which was called by the framework when the program was last terminated.
// The document data is recovered by a CDoc class method.
BOOL CMainFrame::LoadRegistrySettings(LPCWSTR keyName)
{
    // Reload previously saved window placement, toolbar/statusbar
    // switches and view settings from the registry entries in key section
    // 'Software\keyName\Frame Settings'.
    CFrame::LoadRegistrySettings(keyName);

    // Load the saved document entries from the same key.
    TheDoc().LoadDocRegistry(keyName);
    return TRUE;
}

// Toggle the check state of the button with the specified id.
BOOL CMainFrame::OnCheckButton(UINT id)
{
    m_view.SetCheck(id - IDC_CHECK_A);
    return TRUE;
}

// The framework calls this member function as a signal that the CWnd or
// application is to terminate.Retrieve the data in the dialog controls
// and save it if all is well.If an error arises, return with the cursor
// in the offending control so the user can correct it.
void CMainFrame::OnClose()
{
    if (!UpdateDialog(READFROMCONTROL))
    {
        // There is a problem with some of the control data.
        TRACE("*** Verification failed ***\n");
        return;  // return control to user to fix this problem
    }

    // Save the document contents into the registry.
    TheDoc().SaveDocRegistry(GetRegistryKeyName().c_str());
    TRACE("Data saved to registry\n");

    //The base class calls SaveRegistrySettings and DestroyWindow.
    CFrame::OnClose();
}

// The framework calls this member function when the user selects an item
// from a menu, when a child control sends a notification message, or when
// an accelerator keystroke is translated.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
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
        case IDW_ABOUT:            return OnHelp();
        case IDW_VIEW_STATUSBAR:   return OnViewStatusBar();
        case IDW_VIEW_TOOLBAR:     return OnViewToolBar();

        case IDC_CHECK_A:
        case IDC_CHECK_B:
        case IDC_CHECK_C:          return OnCheckButton(id);

        case IDC_RADIO_A:
        case IDC_RADIO_B:
        case IDC_RADIO_C:          return OnRadioButton(id);

        // Indicate not processed here.
        default: return FALSE;
    }
}

// Define the way the frame is created.
int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
    // Uncomment the lines below to change frame options.

    // UseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar.
    // UseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar.
    // UseOwnerDrawnMenu(FALSE);     // Don't use owner draw for popup menu items.
    // UseReBar(FALSE);              // Don't use a ReBar.
    // UseStatusBar(FALSE);          // Don't use a StatusBar.
    // UseThemes(FALSE);             // Don't use themes.
    // UseToolBar(FALSE);            // Don't use a ToolBar.

    // Call the base class function.
    int rtn = CFrame::OnCreate(cs);

    // Show the initial document and status.
    UpdateDialog(SENDTOCONTROL);
    return rtn;
}

// Copy text to the control with keyboard focus.
BOOL CMainFrame::OnEditCopy()
{
    GetFocus().SendMessage(WM_COPY, 0, 0);
    return TRUE;
}

// Cut text from the control with keyboard focus.
BOOL CMainFrame::OnEditCut()
{
    GetFocus().SendMessage(WM_CUT, 0, 0);
    return TRUE;
}

// Delete text from the control with keyboard focus.
BOOL CMainFrame::
OnEditDelete()
{
    GetFocus().SendMessage(WM_CLEAR, 0, 0);
    return TRUE;
}

// Paste text to the control with keyboard focus.
BOOL CMainFrame::OnEditPaste()
{
    GetFocus().SendMessage(WM_PASTE, 0, 0);
    return TRUE;
}

// Redo the last text operation for the control with keyboard focus.
BOOL CMainFrame::OnEditRedo()
{
    GetFocus().SendMessage(EM_REDO, 0, 0);
    return TRUE;
}

// Undo the last text operation for the control with keyboard focus.
BOOL CMainFrame::OnEditUndo()
{
    GetFocus().SendMessage(EM_UNDO, 0, 0);
    return TRUE;
}

// Close the application.
BOOL CMainFrame::OnFileExit()
{
    // Issue a close request to the frame.
    Close();
    return TRUE;
}


// Display the help about dialog.
BOOL CMainFrame::OnHelp()
{
    // Ensure only one dialog displayed even for multiple hits of the F1 button.
    if (!m_aboutDialog.IsWindow())
    {
        m_aboutDialog.DoModal(*this);
    }

    return TRUE;
}


// This method is called by the framework after the frame window is created.
void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.
}

// This method is called by the framework before the menu items are
// displayed.Add code here to update the check state of menu items.
void CMainFrame::OnMenuUpdate(UINT id)
{
    switch (id)
    {
    case IDC_CHECK_A:
    {
        bool isCheckA = (m_view.GetCheckA() != 0);
        UINT check = isCheckA ? MF_CHECKED : MF_UNCHECKED;
        GetFrameMenu().CheckMenuItem(id, check);
        break;
    }
    case IDC_CHECK_B:
    {
        bool isCheckB = (m_view.GetCheckB() != 0);
        UINT check = isCheckB ? MF_CHECKED : MF_UNCHECKED;
        GetFrameMenu().CheckMenuItem(id, check);
        break;
    }
    case IDC_CHECK_C:
    {
        bool isCheckC = (m_view.GetCheckC() != 0);
        UINT check = isCheckC ? MF_CHECKED : MF_UNCHECKED;
        GetFrameMenu().CheckMenuItem(id, check);
        break;
    }

    default: break;
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

// Select the radio button with the specified id.
BOOL CMainFrame::OnRadioButton(UINT id)
{
    m_view.SetRadio(id - IDC_CHECK_A);
    return TRUE;
}

// Specify the bitmap for the menu icons.
void CMainFrame::SetupMenuIcons()
{
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        SetMenuIcons(data, RGB(192, 192, 192), IDW_MAIN);
    else
        SetMenuIcons(data, RGB(192, 192, 192), IDB_TOOLBAR16);
}

// Assigns images and command IDs to the toolbar buttons.
void CMainFrame::SetupToolBar()
{
    // Note: The toolbar is destroyed and recreated when the DPI changes when
    // using Per Monitor DPI Awareness.
    // This function is called when the toobar is created.

    // Set the resource IDs for the toolbar buttons.
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

// Update data items in memory(bReadFromControl is READFROMCONTROL) or in
// the dialog controls(bReadFromControl is SENDTOCONTROL).The list of
// affected controls is specified in the CView::DoDataExchange() method.
BOOL CMainFrame::UpdateDialog(BOOL bReadFromControl)
{
    return m_view.UpdateDialog(bReadFromControl);
}

// Handle the window's messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
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
        str2 << L"Error: " << e.what();
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
