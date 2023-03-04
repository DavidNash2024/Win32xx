////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "resource.h"
#include "Mainfrm.h"

//////////////////////////////////
// CMainFrame function definitions
//

// Constructor for CMainFrame.
CMainFrame::CMainFrame() : m_sdiView(IDD_DIALOG1)
{
}

// Destructor for CMainFrame.
CMainFrame::~CMainFrame()
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    //Set m_SdiView as the view window of the frame
    SetView(m_sdiView);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\FormDemo"));

    return CFrame::Create(parent);
}

// Assigns the appropriately sized menu icons.
// Called each time the
void CMainFrame::DPIScaleMenuIcons()
{
    // Load the toolbar bitmap.
    CBitmap toolbarImage(IDW_MAIN);

    // Scale the bitmap to the menu item height.
    int menuHeight = GetMenuIconHeight();
    int scale = menuHeight / toolbarImage.GetSize().cy;
    CBitmap scaledImage;
    if (scale > 0)
        scaledImage = ScaleUpBitmap(toolbarImage, scale);
    else
        scaledImage.LoadBitmap(IDB_MENUICONS);

    // Create the image-list from the scaled image
    CSize sz = scaledImage.GetSize();
    m_menuImages.Create(sz.cy, sz.cy, ILC_COLOR32 | ILC_MASK, 0, 0);
    COLORREF mask = RGB(192, 192, 192);
    m_menuImages.Add(scaledImage, mask);

    // Assign the image-list to the menu items.
    SetMenuImages(m_menuImages);
}

// Assigns the appropriately sized toolbar icons.
void CMainFrame::DPIScaleToolBar()
{
    if (GetToolBar().IsWindow())
    {
        // Load the toolbar bitmap.
        CBitmap toolbarImage(IDW_MAIN);

        // Create the image-list
        CBitmap dpiImage = DPIScaleUpBitmap(toolbarImage);
        CSize sz = dpiImage.GetSize();
        m_normalImages.Create(sz.cy, sz.cy, ILC_COLOR32 | ILC_MASK, 0, 0);
        COLORREF mask = RGB(192, 192, 192);
        m_normalImages.Add(dpiImage, mask);

        // Assign the image-list to the toolbar.
        GetToolBar().SetImageList(m_normalImages);
        GetToolBar().SetDisableImageList(0);

        // Adjust the toolbar band height.
        if (GetReBar().IsWindow())
        {
            int band = GetReBar().GetBand(GetToolBar());
            if (band >= 0)
            {
                CSize sizeToolBar = GetToolBar().GetMaxSize();
                GetReBar().ResizeBand(band, sizeToolBar);
            }
        }
    }
}

// Loads the settings from the registry
BOOL CMainFrame::LoadRegistrySettings(LPCTSTR keyName)
{
    CFrame::LoadRegistrySettings(keyName);
    GetDoc().LoadSettings(GetRegistryKeyName().c_str());

    return TRUE;
}

// Called when the effective dots per inch (dpi) for a window has changed.
// This occurs when:
//  - The window is moved to a new monitor that has a different DPI.
//  - The DPI of the monitor hosting the window changes.
LRESULT CMainFrame::OnDPIChanged(UINT msg, WPARAM wparam, LPARAM lparam)
{
    CFrame::OnDPIChanged(msg, wparam, lparam);
    DPIScaleMenuIcons();
    DPIScaleToolBar();
    RecalcLayout();

    return 0;
}

// Close the frame window to end the application.
BOOL CMainFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Update the check state of the various menu items
void CMainFrame::OnMenuUpdate(UINT id)
{
    switch (id)
    {
    case ID_CHECK_A:
        OnUpdateCheckA(id);
        break;
    case ID_CHECK_B:
        OnUpdateCheckB(id);
        break;
    case ID_CHECK_C:
        OnUpdateCheckC(id);
        break;
    }

    if ((id >= ID_RADIO_A) && (id <= ID_RADIO_C))
        OnUpdateRangeOfIDs(ID_RADIO_A, ID_RADIO_C, id);

    CFrame::OnMenuUpdate(id);
}

// Respond to the toolbar and menu.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch(id)
    {
    case ID_CHECK_A:         return m_sdiView.OnCheckA();
    case ID_CHECK_B:         return m_sdiView.OnCheckB();
    case ID_CHECK_C:         return m_sdiView.OnCheckC();
    case IDM_FILE_EXIT:      return OnFileExit();
    case IDW_VIEW_STATUSBAR: return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:   return OnViewToolBar();
    case IDM_HELP_ABOUT:     return OnHelp();

    case ID_RADIO_A:
    case ID_RADIO_B:        // intentionally blank
    case ID_RADIO_C:        return m_sdiView.OnRangeOfIDs(ID_RADIO_A, ID_RADIO_C, id);

    case IDM_EDIT_COPY:
    {
        GetFocus().SendMessage(WM_COPY);
        return TRUE;
    }

    case IDM_EDIT_PASTE:
    {
        GetFocus().SendMessage(WM_PASTE);
        return TRUE;
    }
    }

  return FALSE;
}

// OnCreate controls the way the frame is created.
// Overriding CFrame::OnCreate is optional.
int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
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
    return CFrame::OnCreate(cs);
}

// Called after the frame window is created.
void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.
}

// Updates the Check A menu item.
void CMainFrame::OnUpdateCheckA(UINT id)
{
    bool isCheckA = (GetDoc().GetCheckA() != 0);
    GetFrameMenu().CheckMenuItem(id, MF_BYCOMMAND | (isCheckA ? MF_CHECKED : MF_UNCHECKED));
}

// Updates the Check B menu item.
void CMainFrame::OnUpdateCheckB(UINT id)
{
    bool isCheckB = (GetDoc().GetCheckB() != 0);
    GetFrameMenu().CheckMenuItem(id, MF_BYCOMMAND | (isCheckB ? MF_CHECKED : MF_UNCHECKED));
}

// Updates the Check C menu item.
void CMainFrame::OnUpdateCheckC(UINT id)
{
    bool isCheckC = (GetDoc().GetCheckC() != 0);
    GetFrameMenu().CheckMenuItem(id, MF_BYCOMMAND | (isCheckC ? MF_CHECKED : MF_UNCHECKED));
}

// Updates the radio button menu selection.
void CMainFrame::OnUpdateRangeOfIDs(UINT idFirst, UINT idLast, UINT id)
{
    int fileItem = GetFrameMenu().FindMenuItem(_T("&Select"));
    CMenu radioMenu = GetFrameMenu().GetSubMenu(fileItem);
    if (GetDoc().GetRadio() == id)
        radioMenu.CheckMenuRadioItem(idFirst, idLast, id, MF_BYCOMMAND);
}

// Save the settings in the registry.
BOOL CMainFrame::SaveRegistrySettings()
{
    CFrame::SaveRegistrySettings();
    GetDoc().SaveSettings(GetRegistryKeyName().c_str());

    return TRUE;
}

// Specifies the images for some of the menu items.
void CMainFrame::SetupMenuIcons()
{
    // Call the base class function first.
    CFrame::SetupMenuIcons();

    // Update the menu icons
    DPIScaleMenuIcons();
}

// Configure the toolbar.
void CMainFrame::SetupToolBar()
{
    // Set the Resource IDs for the toolbar buttons
    AddToolBarButton( IDM_FILE_NEW, FALSE );
    AddToolBarButton( IDM_FILE_OPEN, FALSE );
    AddToolBarButton( IDM_FILE_SAVE, FALSE );
    AddToolBarButton( 0 );        // Separator
    AddToolBarButton( IDM_EDIT_CUT, FALSE );
    AddToolBarButton( IDM_EDIT_COPY, FALSE );
    AddToolBarButton( IDM_EDIT_PASTE, FALSE );
    AddToolBarButton( 0 );        // Separator
    AddToolBarButton( IDM_FILE_PRINT, FALSE);
    AddToolBarButton( 0 );        // Separator
    AddToolBarButton( IDM_HELP_ABOUT );

    DPIScaleToolBar();
}

// Process the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
    //  switch (msg)
    //  {
    //  Add case statements for each messages to be handled here
    //  }

        // pass unhandled messages on for default processing
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

