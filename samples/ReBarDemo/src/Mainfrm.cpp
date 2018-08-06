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
    SetView(m_view);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\ReBarDemo"));
}

CMainFrame::~CMainFrame()
{
    // Destructor for CMainFrame.
}

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


BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    // OnCommand responds to menu and and toolbar input

    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_FILE_OPEN:         return OnFileOpen();
    case IDM_FILE_SAVE:         return OnFileSave();
    case IDM_FILE_SAVEAS:       return OnFileSave();
    case IDM_FILE_PRINT:        return OnFilePrint();
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
    m_toolBarImages.Add(GetApp().LoadIcon(IDI_TOP));
    m_toolBarImages.Add(GetApp().LoadIcon(IDI_LEFT));
    m_toolBarImages.Add(GetApp().LoadIcon(IDI_RIGHT));
    m_toolBarImages.Add(GetApp().LoadIcon(IDI_BOTTOM));

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

BOOL CMainFrame::OnFileExit()
{
    // Issue a close request to the frame
    PostMessage(WM_CLOSE);
    return TRUE;
}

BOOL CMainFrame::OnFileOpen()
{
    CFileDialog fileDlg(TRUE);

    // Bring up the file open dialog retrieve the selected filename
    if (fileDlg.DoModal(*this) == IDOK)
    {
        // TODO:
        // Add your own code here. Refer to the tutorial for additional information
    }

    return TRUE;
}

BOOL CMainFrame::OnFileSave()
{
    CFileDialog fileDlg(FALSE);

    // Bring up the file save dialog retrieve the selected filename
    if (fileDlg.DoModal(*this) == IDOK)
    {
        // TODO:
        // Add your own code here. Refer to the tutorial for additional information
    }

    return TRUE;
}

BOOL CMainFrame::OnFilePrint()
{
    // Bring up a dialog to choose the printer
    CPrintDialog printdlg;

    try
    {
        INT_PTR result = printdlg.DoModal(*this);

        // Retrieve the printer DC
        // CDC dcPrinter = Printdlg.GetPrinterDC();

        // TODO:
        // Add your own code here. Refer to the tutorial for additional information

        return (result == IDOK);   // boolean expression
    }

    catch (const CWinException& /* e */)
    {
        // No default printer
        MessageBox(_T("Unable to display print dialog"), _T("Print Failed"), MB_OK);
        return FALSE;
    }
}

void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    TRACE("Frame created\n");
}

LRESULT CMainFrame::OnNotify(WPARAM wparam, LPARAM lparam)
// Process notification messages sent by child windows
{
//  switch (((LPNMHDR)lparam)->code)
//  {
//  }

    // Some notifications should return a value when handled
    return CFrame::OnNotify(wparam, lparam);
}

void CMainFrame::RecalcLayout()
// Override CFrame::RecalcLayout to add the positioning of our rebar
{
    // Resize the status bar
    if (GetStatusBar().IsWindow() && GetStatusBar().IsWindowVisible())
    {
        GetStatusBar().SetWindowPos(NULL, 0, 0, 0, 0, SWP_SHOWWINDOW);
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
        GetToolBar().SendMessage(TB_AUTOSIZE, 0, 0);

    // Position the additional rebar at the top, left, right or bottom of the view.
    if (m_reBar.IsWindow())
        SetReBarPos();

    // Position the view window
    CFrame::RecalcLayout();

    // Adjust rebar bands
    if (GetReBar().IsWindow())
    {
        if (GetReBarTheme().UseThemes && GetReBarTheme().BandsLeft)
            GetReBar().MoveBandsLeft();

        if (GetMenuBar().IsWindow())
            SetMenuBarBandSize();
    }
}

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
        m_reBar.SetWindowPos(NULL, 0, rc.top, cxRB, rc.Height(), SWP_SHOWWINDOW);
        m_toolBar.PressButton(IDM_LEFT, TRUE);
        break;
    case CCS_RIGHT:
        m_reBar.SetWindowPos(NULL, rc.Width() - cxRB, rc.top, cxRB, rc.Height(), SWP_SHOWWINDOW);
        m_toolBar.PressButton(IDM_RIGHT, TRUE);
        break;
    case CCS_BOTTOM:
        m_reBar.SetWindowPos(NULL, 0, rc.bottom - cyRB, rc.Width(), cyRB, SWP_SHOWWINDOW);
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

void CMainFrame::SetupToolBar()
{
    // Set the Resource IDs for the toolbar buttons
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_OPEN  );
    AddToolBarButton( IDM_FILE_SAVE  );
    
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_EDIT_CUT,   FALSE );  // disabled button
    AddToolBarButton( IDM_EDIT_COPY,  FALSE );  // disabled button
    AddToolBarButton( IDM_EDIT_PASTE, FALSE );  // disabled button
    
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_FILE_PRINT );
    
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_HELP_ABOUT );
}

// Note: Vertical toolbars require each button to have TBSTATE_WRAP, set for group buttons to work.
//       Horizontal toolbars require the TBSTATE_WRAP removed.
void CMainFrame::SetWrapState(CToolBar* pToolBar, BOOL isWrapped)
{
    for (int i = 0; i < m_toolBar.GetButtonCount(); ++i)
    {
        int id = pToolBar->GetCommandID(i);
        pToolBar->SetButtonState(id, TBSTATE_ENABLED | (isWrapped ? TBSTATE_WRAP : 0));
    }
}

LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
//  switch (msg)
//  {
//      Add case statements for each messages to be handled here
//  }

    // pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}

