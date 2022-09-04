/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"

///////////////////////////////////
// CMainFrame function definitions.
//

// Constructor for CMainFrame. Its called after CFrame's constructor
CMainFrame::CMainFrame() : m_useBigIcons(true)
{
}

// Destructor for CMainFrame.
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
    LoadRegistrySettings(_T("Win32++\\ToolBarDemo"));

    return CFrame::Create(parent);
}

// Called when the user has begun customizing a toolbar. Here we save
// a copy of the ToolBar layout so it can be restored when the user
// selects the reset button.
LRESULT CMainFrame::OnBeginAdjust(LPNMTOOLBAR pNMTB)
{
    CToolBar* pToolBar = static_cast<CToolBar*>(GetCWndPtr(pNMTB->hdr.hwndFrom));
    assert (dynamic_cast<CToolBar*> (pToolBar));

    if (pToolBar)
    {
        int nResetCount = pToolBar->GetButtonCount();
        m_resetButtons.clear();

        for (int i = 0; i < nResetCount; i++)
        {
            TBBUTTON tbb;
            pToolBar->GetButton(i, tbb);
            m_resetButtons.push_back(tbb);
        }
    }

    return TRUE;
}

// OnCommand responds to menu and and toolbar input
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch(id)
    {
    case IDM_FILE_EXIT:         return OnFileExit();
    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDM_TOOLBAR_CUSTOMIZE: return OnTBCustomize();
    case IDM_TOOLBAR_DEFAULT:   return OnTBDefault();
    case IDM_TOOLBAR_BIGICONS:  return OnTBBigIcons();
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

    // UseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar
    // UseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar
    // UseReBar(FALSE);              // Don't use a ReBar
    // UseStatusBar(FALSE);          // Don't use a StatusBar
    // UseThemes(FALSE);             // Don't use themes
    // UseToolBar(FALSE);            // Don't use a ToolBar

    // call the base class function
    CFrame::OnCreate(cs);

    // Add the CCS_ADJUSTABLE style to the ToolBar
    DWORD style = GetToolBar().GetStyle();
    GetToolBar().SetStyle(CCS_ADJUSTABLE|style);

    // Tick the Large Icons menu item
    GetFrameMenu().CheckMenuItem(IDM_TOOLBAR_BIGICONS, MF_BYCOMMAND | MF_CHECKED);

    return 0;
}

// Called when the help button on the customize dialog is pressed.
LRESULT CMainFrame::OnCustHelp(LPNMHDR)
{
    MessageBox(_T("Help Button Pressed"), _T("Help"), MB_ICONINFORMATION | MB_OK);

    return 0;
}

// Called when the user has stopped customizing a toolbar.
LRESULT CMainFrame::OnEndAdjust(LPNMHDR)
{
    ResizeToolbarBand();

    return TRUE;
}

// Issue a close request to the frame
BOOL CMainFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Called after the frame window is created.
void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    TRACE("Frame created\n");

    //Store the current ToolBar
    SaveTBDefault();
}

// Process notification messages sent by child windows
LRESULT CMainFrame::OnNotify(WPARAM wparam, LPARAM lparam)
{
    LPNMTOOLBAR pNMTB = (LPNMTOOLBAR)lparam;
    switch(pNMTB->hdr.code)
    {
    case TBN_QUERYDELETE:   return OnQueryDelete(pNMTB);
    case TBN_QUERYINSERT:   return OnQueryInsert(pNMTB);
    case TBN_CUSTHELP:      return OnCustHelp((LPNMHDR)lparam);
    case TBN_GETBUTTONINFO: return OnGetButtonInfo(pNMTB);
    case TBN_BEGINADJUST:   return OnBeginAdjust(pNMTB);
    case TBN_ENDADJUST:     return OnEndAdjust((LPNMHDR)lparam);
    case TBN_TOOLBARCHANGE: return OnToolBarChange(pNMTB);
    case TBN_RESET:         return OnReset(pNMTB);
    }

    // Some notifications should return a value when handled
    return CFrame::OnNotify(wparam, lparam);
}

// Called once for each button during toolbar customization to populate the list
// of available buttons. Return FALSE when all buttons have been added.
LRESULT CMainFrame::OnGetButtonInfo(LPNMTOOLBAR pNMTB)
{
    // An array of TBBUTTON that contains all possible buttons
    TBBUTTON buttonInfo[] =
    {
        { 0, IDM_FILE_NEW,      TBSTATE_ENABLED, 0, {0}, 0, 0 },
        { 1, IDM_FILE_OPEN,     TBSTATE_ENABLED, 0, {0}, 0, 0 },
        { 2, IDM_FILE_SAVE,     TBSTATE_ENABLED, 0, {0}, 0, 0 },
        { 3, IDM_EDIT_CUT,      0,               0, {0}, 0, 0 },
        { 4, IDM_EDIT_COPY,     0,               0, {0}, 0, 0 },
        { 5, IDM_EDIT_PASTE,    0,               0, {0}, 0, 0 },
        { 6, IDM_FILE_PRINT,    TBSTATE_ENABLED, 0, {0}, 0, 0 },
        { 7, IDM_HELP_ABOUT,    TBSTATE_ENABLED, 0, {0}, 0, 0 }
    };

    // An array of Button text strings (LPCTSTRs).
    // These are displayed in the customize dialog.
    LPCTSTR buttonText[] =
    {
        _T("New Document"),
        _T("Open File"),
        _T("Save File"),
        _T("Cut"),
        _T("Copy"),
        _T("Paste"),
        _T("Print"),
        _T("Help About")
    };

    // Pass the next button from the array. There is no need to filter out buttons
    // that are already used. They will be ignored.
    int buttons = sizeof(buttonInfo) / sizeof(TBBUTTON);

    if (pNMTB->iItem < buttons)
    {
        pNMTB->tbButton = buttonInfo[pNMTB->iItem];
        StrCopy(pNMTB->pszText, buttonText[pNMTB->iItem], pNMTB->cchText);
        return TRUE;    // Load the next button.
    }

    return FALSE;   // No more buttons.
}

// Called when a button may be deleted from a toolbar while the user is customizing the toolbar.
// Return TRUE to permit button deletion, and FALSE to prevent it.
LRESULT CMainFrame::OnQueryDelete(LPNMTOOLBAR)
{
    ResizeToolbarBand();

    // Permit all buttons to be deleted
    return TRUE;
}

// Called when a button may be inserted to the left of the specified button while the user
//  is customizing a toolbar. Return TRUE to permit button deletion, and FALSE to prevent it.
LRESULT CMainFrame::OnQueryInsert(LPNMTOOLBAR)
{
    ResizeToolbarBand();

    // Permit all buttons to be inserted
    return TRUE;
}

// Called when the user presses the Reset button on the ToolBar customize dialog.
// Here we restore the Toolbar to the settings saved in OnBeginAdjust.
LRESULT CMainFrame::OnReset(LPNMTOOLBAR pNMTB)
{
    CToolBar* pToolBar = static_cast<CToolBar*>(GetCWndPtr(pNMTB->hdr.hwndFrom));
    assert (dynamic_cast<CToolBar*> (pToolBar));

    if (pToolBar)
    {
        // Remove all current buttons
        int nCount = pToolBar->GetButtonCount();
        for (int i = nCount - 1; i >= 0; i--)
        {
            pToolBar->DeleteButton(i);
        }

        // Restore buttons from info stored in m_vTBBReset
        int nResetCount = static_cast<int>(m_resetButtons.size());
        for (int j = 0; j < nResetCount; j++)
        {
            TBBUTTON tbb = m_resetButtons[j];
            pToolBar->InsertButton(j, tbb);
        }
    }

    ResizeToolbarBand();
    RecalcLayout();

    return TRUE;
}

// Called when the toolbar has been changed during customization.
LRESULT CMainFrame::OnToolBarChange(LPNMTOOLBAR)
{
    // Reposition the toolbar
    RecalcLayout();

    return TRUE;
}

// Show or hide the toolbars
inline BOOL CMainFrame::OnViewToolBar()
{
    BOOL show = GetToolBar().IsWindow() && !GetToolBar().IsWindowVisible();

    GetReBar().ShowBand(GetReBar().GetBand(GetToolBar()), show);
    GetReBar().ShowBand(GetReBar().GetBand(m_cards), show);
    GetReBar().ShowBand(GetReBar().GetBand(m_arrows), show);
    return TRUE;
}

// Toggle the Image size for the ToolBar by changing Image Lists.
BOOL CMainFrame::OnTBBigIcons()
{
    m_useBigIcons = !m_useBigIcons;

    GetFrameMenu().CheckMenuItem(IDM_TOOLBAR_BIGICONS, MF_BYCOMMAND | (m_useBigIcons ? MF_CHECKED : MF_UNCHECKED));

    if (m_useBigIcons)
    {
        // Set Large Images. 3 Imagelists - Normal, Hot and Disabled
        SetToolBarImages(RGB(192,192,192), IDW_MAIN, IDB_HOT, IDB_DISABLED);
        SetTBImageList(m_arrows, m_arrowImages, IDB_ARROWS, RGB(255, 0, 255));
        SetTBImageList(m_cards, m_cardImages, IDB_CARDS, RGB(255, 0, 255));
    }
    else
    {
        // Set Small icons
        SetToolBarImages(RGB(192,192,192), IDB_SMALL, 0, 0);
        SetTBImageList(m_arrows, m_arrowImages, IDB_SMALLARROWS, RGB(255, 0, 255));
        SetTBImageList(m_cards, m_cardImages, IDB_SMALLCARDS, RGB(255, 0, 255));
    }

    RecalcLayout();
    GetToolBar().Invalidate();
    m_arrows.Invalidate();
    m_cards.Invalidate();
    return TRUE;
}

// Customize the toolbar.
BOOL CMainFrame::OnTBCustomize()
{
    GetToolBar().Customize();
    return TRUE;
}

// Set the Toolbar back to its intial settings.
BOOL CMainFrame::OnTBDefault()
{
    // Remove all current buttons
    int count = GetToolBar().GetButtonCount();
    for (int i = count - 1; i >= 0; i--)
    {
        GetToolBar().DeleteButton(i);
    }

    // Restore buttons from info stored in m_vTBBDefault
    int nDefaultCount = static_cast<int>(m_defaultButtons.size());
    for (int j = 0; j < nDefaultCount; j++)
    {
        TBBUTTON tbb = m_defaultButtons[j];
        GetToolBar().InsertButton(j, tbb);
    }

    ResizeToolbarBand();
    RecalcLayout();
    return TRUE;
}

// Readjust the toolbar band size after customization.
void CMainFrame::ResizeToolbarBand() const
{
    CSize size = GetToolBar().GetMaxSize();
    int band = GetReBar().GetBand(GetToolBar());
    GetReBar().ResizeBand(band, size);
}

// Saves the initial Toolbar configuration in a vector of TBBUTTON.
void CMainFrame::SaveTBDefault()
{
    int nCount = GetToolBar().GetButtonCount();

    for (int i = 0; i < nCount; i++)
    {
        TBBUTTON tbb;
        GetToolBar().GetButton(i, tbb);
        m_defaultButtons.push_back(tbb);
    }
}

// Configure the menu icons.
void CMainFrame::SetupMenuIcons()
{
    // Set the bitmap used for menu icons
    std::vector<UINT> data = GetToolBarData();
    if (GetMenuIconHeight() >= 24)
        AddMenuIcons(data, RGB(192, 192, 192), IDW_MAIN, 0);
    else
        AddMenuIcons(data, RGB(192, 192, 192), IDB_SMALL);
}

// Configure the Toolbar.
void CMainFrame::SetupToolBar()
{
    // Set the Resource IDs for the toolbar buttons
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_OPEN, FALSE);    // disabled button
    AddToolBarButton( IDM_FILE_SAVE, FALSE);    // disabled button

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_EDIT_CUT,   FALSE );  // disabled button
    AddToolBarButton( IDM_EDIT_COPY,  FALSE );  // disabled button
    AddToolBarButton( IDM_EDIT_PASTE, FALSE );  // disabled button

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_FILE_PRINT, FALSE);   // disabled button

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_HELP_ABOUT );

    // Add the two other toolbars if we can use rebars (Need Win95 and IE 4 or better)
    if (IsReBarSupported())
    {
        // Add the Arrows toolbar
        AddToolBarBand(m_arrows, 0, IDC_ARROWS);
        m_arrows.AddButton(IDM_ARROW_LEFT);
        m_arrows.AddButton(IDM_ARROW_RIGHT);

        // Add the Cards toolbar
        AddToolBarBand(m_cards, 0, IDB_CARDS);
        m_cards.AddButton(IDM_CARD_CLUB);
        m_cards.AddButton(IDM_CARD_DIAMOND);
        m_cards.AddButton(IDM_CARD_HEART);
        m_cards.AddButton(IDM_CARD_SPADE);

        // Set the images for the additional toolbars
        SetTBImageList(m_arrows, m_arrowImages, IDB_ARROWS, RGB(255,0,255));
        SetTBImageList(m_cards, m_cardImages, IDB_CARDS, RGB(255,0,255));
    }
}

// Process the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        return WndProcDefault(msg, wparam, lparam);
    }

    catch (const CException& e)
    {
        // Display the exception.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);
        return 0;
    }
}
