////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
    // Constructor for CMainFrame. Its called after CFrame's constructor

    //Set m_View as the view window of the frame
    SetView(m_View);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\PropertySheet Sample"));
}

CMainFrame::~CMainFrame()
{
    // Destructor for CMainFrame.
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // OnCommand responds to menu and and toolbar input

    UNREFERENCED_PARAMETER(lParam);

    UINT nID = LOWORD(wParam);
    switch(nID)
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

BOOL CMainFrame::OnFileExit()
{
    // Issue a close request to the frame
    PostMessage(WM_CLOSE);
    return TRUE;
}

void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    TRACE("Frame created\n");
}

BOOL CMainFrame::OnModeless()
{
    // Permit only one Modeless property sheet
    if (!m_ModelessPS.IsWindow())
    {
        m_ModelessPS.AddPage(new CButtonPage(IDD_BUTTONS, _T("Buttons")));
        m_ModelessPS.AddPage(new CComboPage(IDD_COMBOBOXES, _T("Combo Boxes")));
        m_ModelessPS.SetTitle(_T("Modeless Property Sheet"));
        m_ModelessPS.Create(*this);
    }
    else
        m_ModelessPS.SetForegroundWindow();

    return TRUE;
}

BOOL CMainFrame::OnModal()
{
    if (m_ModelessPS.IsWindow())
        m_ModelessPS.Destroy();

    CMyPropertySheet mps(_T("Modal Property Sheet"), *this);
    mps.AddPage(new CButtonPage(IDD_BUTTONS, _T("Buttons")));
    mps.AddPage(new CComboPage(IDD_COMBOBOXES, _T("Combo Boxes")));
    mps.DoModal();

    return TRUE;
}

BOOL CMainFrame::OnWizard()
{
    CMyPropertySheet mps(NULL, *this);
    mps.AddPage(new CButtonPage(IDD_BUTTONS, _T("Buttons")));
    mps.AddPage(new CComboPage(IDD_COMBOBOXES, _T("Combo Boxes")));
    mps.SetWizardMode(TRUE);
    mps.DoModal();
    return TRUE;
}

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

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//  switch (uMsg)
//  {
        // Add case statements for each messages to be handled here
//  }

    // pass unhandled messages on for default processing
    return WndProcDefault(uMsg, wParam, lParam);
}
