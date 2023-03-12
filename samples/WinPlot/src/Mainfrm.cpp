/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"

///////////////////////////////////
// CMainFrame function definitions.
//

// Constructor for CMainFrame. Its called after CFrame's constructor.
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
    // Set m_view as the view window of the frame.
    SetView(m_view);

    // Set the registry key name, and load the initial window position.
    // Use a registry key name like "CompanyName\\Application".
    LoadRegistrySettings(_T("Win32++\\WinPlot"));

    return CFrame::Create(parent);
}

// Assigns the appropriately sized menu icons.
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

// OnCommand responds to menu and and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch(id)
    {
    case IDM_EDIT_FUNCTION:     return OnEditFunction();
    case IDM_FILE_EXIT:         return OnFileExit();
    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDM_HELP_ABOUT:        return OnHelp();
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

BOOL CMainFrame::OnEditFunction()
{
    if (m_view.GetInput().DoModal(*this) == IDOK)
    {
        CString str = m_view.GetInput().GetFunction();
        m_view.GetCalc().Input(str);
        m_view.RedrawWindow();

        if (m_view.GetCalc().Get_Status() == Calc::st_ERROR)
        {
            MessageBox(_T("Invalid Function Input"), _T("Error"), MB_ICONEXCLAMATION);
        }
        else if (m_view.GetCalc().Get_Status() == Calc::st_OVERFLOW)
        {
            MessageBox(_T("Overflow"), _T("Error"), MB_ICONEXCLAMATION);
        }
    }

    return TRUE;
}

BOOL CMainFrame::OnFileExit()
{
    // Issue a close request to the frame.
    Close();
    return TRUE;
}

void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    OnEditFunction();
    TRACE("Frame created\n");
}

// Configure the menu icons.
void CMainFrame::SetupMenuIcons()
{
    // Set the bitmap used for menu icons
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        AddMenuIcons(data, RGB(192, 192, 192), IDW_MAIN, 0);
    else
        AddMenuIcons(data, RGB(192, 192, 192), IDB_TOOLBAR16);
}

void CMainFrame::SetupToolBar()
{
    // Set the Resource IDs for the toolbar buttons.
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_OPEN  );
    AddToolBarButton( IDM_FILE_SAVE  );

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_EDIT_CUT,   FALSE );  // disabled button
    AddToolBarButton( IDM_EDIT_COPY,  FALSE );  // disabled button
    AddToolBarButton( IDM_EDIT_PASTE, FALSE );  // disabled button

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_FILE_PRINT );

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_HELP_ABOUT );
}

LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {

    //  switch (msg)
    //  {
    //      Add case statements for each messages to be handled here.
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