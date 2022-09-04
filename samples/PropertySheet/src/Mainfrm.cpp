/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"

//////////////////////////////////
// CMainFrame function definitions
//

// Constructor.
CMainFrame::CMainFrame()
{
}

// Destructor.
CMainFrame::~CMainFrame()
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    //Set m_View as the view window of the frame
    SetView(m_view);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\PropertySheet Sample"));

    return CFrame::Create(parent);
}

// Respond to menu and and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch(id)
    {
    case IDM_FILE_EXIT:      return OnFileExit();
    case IDM_MODELESS:       return OnModeless();
    case IDM_MODAL:          return OnModal();
    case IDM_WIZARD:         return OnWizard();
    case IDW_VIEW_STATUSBAR: return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:   return OnViewToolBar();
    case IDM_HELP_ABOUT:     return OnHelp();
    }

    return FALSE;
}

// Called when the window is created.
int CMainFrame::OnCreate(CREATESTRUCT& cs)
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

// Issue a close request to the frame to end the application
BOOL CMainFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Called after the window is created.
// Called after OnCreate.
void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    TRACE("Frame created\n");
}

// Create a modeless property sheet.
BOOL CMainFrame::OnModeless()
{
    // Permit only one Modeless property sheet
    if (!m_modelessPS.IsWindow())
    {
        m_modelessPS.AddPage(new CButtonPage(IDD_BUTTONS, _T("Buttons")));
        m_modelessPS.AddPage(new CComboPage(IDD_COMBOBOXES, _T("Combo Boxes")));
        m_modelessPS.SetTitle(_T("Modeless Property Sheet"));
        m_modelessPS.Create(*this);
    }
    else
        m_modelessPS.SetForegroundWindow();

    return TRUE;
}

// Create a modal property sheet.
BOOL CMainFrame::OnModal()
{
    if (m_modelessPS.IsWindow())
        m_modelessPS.Destroy();

    CMyPropertySheet mps(_T("Modal Property Sheet"), *this);
    mps.AddPage(new CButtonPage(IDD_BUTTONS, _T("Buttons")));
    mps.AddPage(new CComboPage(IDD_COMBOBOXES, _T("Combo Boxes")));
    mps.DoModal();

    return TRUE;
}

// Create a wizard. A wizard displays a series of property sheets.
BOOL CMainFrame::OnWizard()
{
    CMyPropertySheet mps(NULL, *this);
    mps.AddPage(new CButtonPage(IDD_BUTTONS, _T("Buttons")));
    mps.AddPage(new CComboPage(IDD_COMBOBOXES, _T("Combo Boxes")));
    mps.SetWizardMode(TRUE);
    mps.DoModal();
    return TRUE;
}

// Specifies the images used on menu items.
void CMainFrame::SetupMenuIcons()
{
    // Use the MenuIcons bitmap for images in menu items.
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        AddMenuIcons(data, RGB(192, 192, 192), IDW_MAIN);
    else
        AddMenuIcons(data, RGB(192, 192, 192), IDB_TOOLBAR16);
}

// Sets the resource IDs and images for the toolbar buttons.
void CMainFrame::SetupToolBar()
{
    // Set the Resource IDs for the toolbar buttons
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_OPEN  );
    AddToolBarButton( IDM_FILE_SAVE  );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_EDIT_CUT   );
    AddToolBarButton( IDM_EDIT_COPY  );
    AddToolBarButton( IDM_EDIT_PASTE );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_FILE_PRINT );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_HELP_ABOUT );
}

// Process the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {

    //  switch (msg)
    //  {
    //  Add case statements for each messages to be handled here.
    //  }

        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}
