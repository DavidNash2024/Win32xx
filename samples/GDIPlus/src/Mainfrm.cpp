/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"


//////////////////////////////////
// CMainFrame function definitions
//

// Constructor for CMainFrame.
CMainFrame::CMainFrame()
{
}

// Destructor for CMainFrame.
CMainFrame::~CMainFrame()
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    //Set m_view as the view window of the frame
    SetView(m_view);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\GDIPlus"));

    return CFrame::Create(parent);
}

// Assigns menu icons appropriately sized for this window's DPI.
// Overrides the CFrameT::DPIScaleMenuIcons virtual function.
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
        scaledImage.LoadBitmap(IDB_TOOLBAR16);

    // Create the image-list from the scaled image
    CSize sz = scaledImage.GetSize();
    m_menuImages.Create(sz.cy, sz.cy, ILC_COLOR32 | ILC_MASK, 0, 0);
    COLORREF mask = RGB(192, 192, 192);
    m_menuImages.Add(scaledImage, mask);

    // Assign the image-list to the menu items.
    SetMenuImages(m_menuImages);
}

// OnCommand responds to menu and and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch(id)
    {
    case IDM_FILE_EXIT:      return OnFileExit();
    case IDW_VIEW_STATUSBAR: return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:   return OnViewToolBar();
    case IDM_HELP_ABOUT:     return OnHelp();
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

// Called after the window is created.
void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    TRACE("Frame created\n");
}

// Issue a close request to the frame to end the program.
BOOL CMainFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Process notification messages (WM_NOTIFY) sent by child windows.
LRESULT CMainFrame::OnNotify(WPARAM wparam, LPARAM lparam)
{
//  LPNMHDR pHeader = reinterpret_cast<LPNMHDR>(lparam);
//  switch (pHeader->code)
//  {
        //Add case statements for each notification message here
//  }

    // Some notifications should return a value when handled
    return CFrame::OnNotify(wparam, lparam);
}

// Specifies the images for some of the menu items.
void CMainFrame::SetupMenuIcons()
{
    // Set the bitmap used for menu icons
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        SetMenuIcons(data, RGB(192, 192, 192), IDW_MAIN);
    else
        SetMenuIcons(data, RGB(192, 192, 192), IDB_TOOLBAR16);

    // Update the menu icons based on this window's DPI.
    DPIScaleMenuIcons();
}

// Set the resource IDs and images for the toolbar buttons.
void CMainFrame::SetupToolBar()
{
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_OPEN, FALSE);    // disabled button
    AddToolBarButton( IDM_FILE_SAVE, FALSE);    // disabled button

    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_EDIT_CUT,   FALSE );  // disabled button
    AddToolBarButton( IDM_EDIT_COPY,  FALSE );  // disabled button
    AddToolBarButton( IDM_EDIT_PASTE, FALSE );  // disabled button

    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_FILE_PRINT, FALSE);   // disabled button

    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_HELP_ABOUT );
}

