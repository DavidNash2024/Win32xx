/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"

///////////////////////////////////
// CMainFrame functions definitions
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
    LoadRegistrySettings(_T("Win32++\\ReBarDemo"));

    return CFrame::Create(parent);
}

// Retrieves the size view rectangle.
CRect CMainFrame::GetViewRect() const
{
    CRect rcClient = GetClientRect();

    if (GetStatusBar().IsWindow() && (GetStatusBar().IsWindowVisible()))
        rcClient = ExcludeChildRect(rcClient, GetStatusBar());

    if (IsReBarSupported() && IsUsingReBar() && GetReBar().IsWindow())
        rcClient = ExcludeChildRect(rcClient, GetReBar());
    else
        if (GetToolBar().IsWindow() && GetToolBar().IsWindowVisible())
            rcClient = ExcludeChildRect(rcClient, GetToolBar());

    if (m_reBar.IsWindow())
        rcClient = ExcludeChildRect(rcClient, m_reBar);

    return rcClient;
}

// Place the rebar at the bottom of the view.
void CMainFrame::OnBottom()
{
    DWORD dwStyle = m_toolBar.GetStyle();
    dwStyle &= ~(CCS_VERT);
    dwStyle |= CCS_BOTTOM;
    SetWrapState(&m_toolBar, FALSE);
    m_toolBar.SetStyle(dwStyle);

    dwStyle = m_reBar.GetStyle();
    dwStyle &= ~(CCS_VERT);
    dwStyle |= CCS_BOTTOM;
    m_reBar.SetStyle(dwStyle);
    RecalcLayout();
}

// Place the rebar on the left of the view.
void CMainFrame::OnLeft()
{
    DWORD dwStyle = m_toolBar.GetStyle();
    dwStyle &= ~(CCS_BOTTOM);
    dwStyle |= CCS_LEFT;
    SetWrapState(&m_toolBar, TRUE);
    m_toolBar.SetStyle(dwStyle);

    dwStyle = m_reBar.GetStyle();
    dwStyle &= ~(CCS_BOTTOM);
    dwStyle |= CCS_LEFT;
    m_reBar.SetStyle(dwStyle);
    RecalcLayout();
}

// Place the rebar on the right of the view.
void CMainFrame::OnRight()
{
    DWORD dwStyle = m_toolBar.GetStyle();
    dwStyle |= CCS_RIGHT;
    SetWrapState(&m_toolBar, TRUE);
    m_toolBar.SetStyle(dwStyle);

    dwStyle = m_reBar.GetStyle();
    dwStyle |= CCS_RIGHT;
    m_reBar.SetStyle(dwStyle);
    RecalcLayout();
}

// Place the rebar at top of the view.
void CMainFrame::OnTop()
{
    DWORD dwStyle = m_toolBar.GetStyle();
    dwStyle &= ~(CCS_VERT | CCS_BOTTOM);
    SetWrapState(&m_toolBar, FALSE);
    m_toolBar.SetStyle(dwStyle);

    dwStyle = m_reBar.GetStyle();
    dwStyle &= ~(CCS_VERT | CCS_BOTTOM);
    m_reBar.SetStyle(dwStyle);
    RecalcLayout();
}

// Respond to menu and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch(id)
    {
    case IDM_FILE_EXIT:         return OnFileExit();
    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDM_HELP_ABOUT:        return OnHelp();

    // Commands from the ToolBar in the additional rebar
    case IDM_TOP:       OnTop();        return TRUE;
    case IDM_LEFT:      OnLeft();       return TRUE;
    case IDM_RIGHT:     OnRight();      return TRUE;
    case IDM_BOTTOM:    OnBottom();     return TRUE;
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
    CFrame::OnCreate(cs);

    DWORD style = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS
                     | CCS_NORESIZE | CCS_NOPARENTALIGN | CCS_NODIVIDER | RBS_VARHEIGHT ;
    m_reBar.CreateEx(0, REBARCLASSNAME, 0, style, CRect(0,0,0,0), *this, 0);

    style |= TBSTYLE_FLAT;

    // Create the ToolBar's image list from 4 icons
    m_toolBarImages.Create(48, 48, ILC_COLOR32 | ILC_MASK, 0, 0);
    m_toolBarImages.AddIcon(IDI_TOP);
    m_toolBarImages.AddIcon(IDI_LEFT);
    m_toolBarImages.AddIcon(IDI_RIGHT);
    m_toolBarImages.AddIcon(IDI_BOTTOM);

    // Create the ToolBar
    style |= TBSTYLE_TOOLTIPS | TBSTYLE_FLAT;
    m_toolBar.CreateEx(0, TOOLBARCLASSNAME, 0, style, CRect(0,0,0,0), m_reBar, 0);
    m_toolBar.SetImageList(m_toolBarImages);

    // Add the ToolBar buttons
    TBBUTTON buttonInfo[] =
    {
        { 0, IDM_TOP,       TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK|TBSTYLE_GROUP, {0}, 0, 0 },
        { 1, IDM_LEFT,      TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK|TBSTYLE_GROUP, {0}, 0, 0 },
        { 2, IDM_RIGHT,     TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK|TBSTYLE_GROUP, {0}, 0, 0 },
        { 3, IDM_BOTTOM,    TBSTATE_ENABLED, TBSTYLE_BUTTON|TBSTYLE_CHECK|TBSTYLE_GROUP, {0}, 0, 0 },
        { 0, 0,             TBSTATE_ENABLED, TBSTYLE_SEP,                                {0}, 0, 0 }
    };   // Last button is a separator
    m_toolBar.AddButtons(5, buttonInfo);

    // Fill the REBARBAND structure
    REBARBANDINFO rbbi;
    ZeroMemory(&rbbi, sizeof(REBARBANDINFO));
    rbbi.fMask      = RBBIM_CHILD |RBBIM_CHILDSIZE| RBBIM_ID | RBBIM_SIZE | RBBIM_STYLE;
    rbbi.fStyle     = RBBS_BREAK;
    rbbi.hwndChild  = m_toolBar.GetHwnd();
    rbbi.wID        = IDC_BANDARROWS;
    rbbi.cyMinChild = m_toolBar.GetMaxSize().cy+1;
    m_reBar.InsertBand(-1, rbbi);

    return 0;
}

// Issue a close request to the frame to end the application.
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

// Overrides CFrame::RecalcLayout to add the positioning of our rebar
void CMainFrame::RecalcLayout()
{
    // Resize the status bar
    if (GetStatusBar().IsWindow() && GetStatusBar().IsWindowVisible())
    {
        GetStatusBar().SetWindowPos(0, 0, 0, 0, 0, SWP_SHOWWINDOW);
        GetStatusBar().Invalidate();
        if (IsUsingMenuStatus())
            GetStatusBar().SetWindowText(GetStatusText());

        SetStatusIndicators();
    }

    // Resize the rebar or toolbar
    if (GetReBar().IsWindow())
    {
        GetReBar().SendMessage(WM_SIZE, 0, 0);
        GetReBar().Invalidate();
    }
    else if (GetToolBar().IsWindow() && GetToolBar().IsWindowVisible())
        GetToolBar().Autosize();

    // Position the additional rebar at the top, left, right or bottom of the view.
    if (m_reBar.IsWindow())
        SetReBarPos();

    // Position the view window.
    if (GetView().IsWindow())
        RecalcViewLayout();

    // Adjust rebar bands
    if (GetReBar().IsWindow())
    {
        if (GetReBarTheme().UseThemes && GetReBarTheme().BandsLeft)
            GetReBar().MoveBandsLeft();

        if (GetMenuBar().IsWindow())
            SetMenuBarBandSize();
    }
}

// Sets the position of the Rebar.
void CMainFrame::SetReBarPos()
{
    int cxRB = 0;
    for (UINT u = 0 ; u < m_reBar.GetRowCount(); ++u)
        cxRB += m_reBar.GetRowHeight(u);

    CRect rc = CFrame::GetViewRect();
    int cyRB = MIN(cxRB, rc.Height());

    DWORD style = m_reBar.GetStyle();
    style &= CCS_VERT | CCS_BOTTOM; // Filter unwanted styles

    switch(style)
    {
    case CCS_LEFT:
        m_reBar.SetWindowPos(0, 0, rc.top, cxRB, rc.Height(), SWP_SHOWWINDOW);
        m_toolBar.PressButton(IDM_LEFT, TRUE);
        break;
    case CCS_RIGHT:
        m_reBar.SetWindowPos(0, rc.Width() - cxRB, rc.top, cxRB, rc.Height(), SWP_SHOWWINDOW);
        m_toolBar.PressButton(IDM_RIGHT, TRUE);
        break;
    case CCS_BOTTOM:
        m_reBar.SetWindowPos(0, 0, rc.bottom - cyRB, rc.Width(), cyRB, SWP_SHOWWINDOW);
        m_toolBar.PressButton(IDM_BOTTOM, TRUE);
        break;
    default:
        m_reBar.SetWindowPos(NULL, 0, rc.top, rc.Width(), cyRB, SWP_SHOWWINDOW);
        m_toolBar.PressButton(IDM_TOP, TRUE);
        break;
    }

    Invalidate();
    m_reBar.RedrawWindow();
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

// Add the TBSTATE_WRAP style to the rebar's toolbar.
// Note: Vertical toolbars require each button to have TBSTATE_WRAP,
//       set for group buttons to work.
//       Horizontal toolbars require the TBSTATE_WRAP removed.
void CMainFrame::SetWrapState(CToolBar* pToolBar, BOOL isWrapped)
{
    for (int i = 0; i < m_toolBar.GetButtonCount(); ++i)
    {
        int id = pToolBar->GetCommandID(i);
        pToolBar->SetButtonState(id, TBSTATE_ENABLED | (isWrapped ? TBSTATE_WRAP : 0));
    }
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

