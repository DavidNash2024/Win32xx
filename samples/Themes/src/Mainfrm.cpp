/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"

#define UWM_ADDCOMBO        (WM_APP + 0x0001)
#define UWM_ARRANGEBANDS    (WM_APP + 0x0002)


///////////////////////////////////
// CMainFrame function definitions.
//

// Constructor for CMainFrame. Its called after CFrame's constructor.
CMainFrame::CMainFrame() : m_color(IDM_BLUE), m_useThemes(true), m_useBandColors(true),
                            m_useFlatStyle(false), m_keepBandsLeft(true), m_lockMenuBand(true),
                            m_useRoundBorders(true), m_useShortBands(true), m_useLines(false),
                            m_showArrows(true), m_showCards(true), m_selectedItem(-1)
{
    // Set m_view as the view window of the frame.
    SetView(m_view);
}

// Destructor for CMainFrame.
CMainFrame::~CMainFrame()
{
}

// Adds a comboBoxEx to the toolbar.
LRESULT CMainFrame::AddCombo()
{
    CToolBar& tb = GetToolBar();

    // Place the ComboBoxEx control over the 'File Save' toolbar button.
    if (tb.CommandToIndex(IDM_FILE_SAVE) >= 0)
    {
        // Convert the 'File Save' button to a separator.
        tb.SetButtonStyle(IDM_FILE_SAVE, TBSTYLE_SEP);

        // Determine the size and position of the ComboBoxEx.
        int comboWidth = DpiScaleInt(100);
        tb.SetButtonWidth(IDM_FILE_SAVE, comboWidth);
        int index = tb.CommandToIndex(IDM_FILE_SAVE);
        CRect rc = tb.GetItemRect(index);

        // Increase the height of the combobox in order to see a dropdown list.
        rc.bottom = DpiScaleInt(100);

        // Create and position the ComboBoxEx window.
        DWORD style = WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_CLIPCHILDREN;
        m_comboBoxEx.CreateEx(0, WC_COMBOBOXEX, 0, style, rc, tb, 0, 0);
        m_comboBoxEx.SetWindowPos(0, rc, SWP_NOACTIVATE);
        m_comboBoxEx.AddItems();
        m_comboBoxEx.SetCurSel(m_selectedItem);
    }

    return 0;
}

// Rearrange the rebar band positions.
void CMainFrame::ArrangeBands()
{
    // Set the band styles and positions.
    std::vector<BandData>::iterator it;

    for (it = m_bandData.begin(); it != m_bandData.end(); ++it)
    {
        int from = GetReBar().IDToIndex((*it).id);
        GetReBar().MoveBand(from, (*it).index);
    }

    GetReBar().MoveBandsLeft();
    RecalcLayout();
}

// Configures the theme colors based on the user's menu selection.
BOOL CMainFrame::ChooseColor(UINT color)
{
    m_color = color;

    switch (color)
    {
    case IDM_WIN11:  // Recommended for Windows 8, Windows 10 and Windows 11
        SetReBarColors(RGB(235, 237, 250), RGB(235, 237, 250), RGB(220, 225, 240), RGB(220, 225, 240));
        SetMenuBarColors(RGB(180, 250, 255), RGB(140, 190, 255), RGB(240, 250, 255), RGB(120, 170, 220), RGB(127, 127, 255), RGB(0, 0, 0));
        SetStatusBarColors(RGB(235, 237, 250), RGB(235, 237, 250), RGB(0, 0, 0));
        SetToolBarColors(RGB(180, 250, 255), RGB(140, 190, 255), RGB(150, 220, 255), RGB(80, 100, 255), RGB(127, 127, 255));
        SetCaptionColor(RGB(235, 237, 250));
        SetCaptionTextColor(RGB(0, 0, 0));
        break;
    case IDM_WIN7:   // Recommended for Vista and Windows 7
        SetReBarColors(RGB(225, 230, 255), RGB(240, 242, 250), RGB(248, 248, 248), RGB(180, 200, 230));
        SetMenuBarColors(RGB(180, 250, 255), RGB(140, 190, 255), RGB(240, 250, 255), RGB(120, 170, 220), RGB(127, 127, 255), RGB(0, 0, 0));
        SetStatusBarColors(RGB(225, 230, 255), RGB(240, 242, 250), RGB(0, 0, 0) );
        SetToolBarColors(RGB(180, 250, 255), RGB(140, 190, 255), RGB(150, 220, 255), RGB(80, 100, 255), RGB(127, 127, 255));
        SetCaptionColor(RGB(225, 230, 255));
        SetCaptionTextColor(RGB(0, 0, 0));
        break;
    case IDM_BLUE:    // Recommended for Windows XP
        SetReBarColors(RGB(150,190,245), RGB(196,215,250), RGB(220,230,250), RGB( 70,130,220));
        SetMenuBarColors(RGB(255, 230, 190), RGB(255, 190, 100), RGB(150,190,245), RGB(220,230,250), RGB(128, 128, 200), RGB(0, 0, 0));
        SetStatusBarColors(RGB(150,190,245), RGB(196,215,250), RGB(0, 0, 0) );
        SetToolBarColors(RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(128, 128, 255));
        SetCaptionColor(RGB(150, 190, 245));
        SetCaptionTextColor(RGB(0, 0, 0));
        break;
    case IDM_GRAY:    // Gray Theme
        SetReBarColors(RGB(210, 210, 200), RGB(238, 236, 224), RGB(248, 247, 243), RGB(195, 195, 172));
        SetMenuBarColors(RGB(196, 215, 250), RGB(120, 180, 220), RGB(210, 210, 200), RGB(248, 247, 243), RGB(128, 128, 200), RGB(0, 0, 0));
        SetStatusBarColors(RGB(210, 210, 200), RGB(238, 236, 224), RGB(0, 0, 0));
        SetToolBarColors(RGB(192, 210, 238), RGB(192, 210, 238), RGB(152, 181, 226), RGB(152, 181, 226), RGB(49, 106, 197));
        SetCaptionColor(RGB(210, 210, 200));
        SetCaptionTextColor(RGB(0, 0, 0));
        break;
    case IDM_OLIVE:   // Green Theme
        SetReBarColors(RGB(160, 180, 80), RGB(180, 200, 100), RGB(200, 220, 120), RGB(80, 159, 78));
        SetMenuBarColors(RGB(255, 200, 190), RGB(255, 210, 90), RGB(255, 230, 190), RGB(230, 160, 190), RGB(128, 128, 128), RGB(0, 0, 200));
        SetStatusBarColors(RGB(160, 180, 80), RGB(180, 200, 100), RGB(0, 0, 200));
        SetToolBarColors(RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(128, 128, 255));
        SetCaptionColor(RGB(160, 180, 80));
        SetCaptionTextColor(RGB(0, 0, 200));
        break;
    case IDM_GOLD:    // Gold Theme
        SetReBarColors(RGB(230, 180, 0), RGB(240, 210, 90), RGB(255, 240, 150), RGB(180, 140, 50));
        SetMenuBarColors(RGB(196, 215, 250), RGB(120, 180, 220), RGB(240, 210, 90), RGB(255, 240, 150), RGB(128, 128, 128), RGB(0, 0, 0));
        SetStatusBarColors(RGB(230, 180, 0), RGB(240, 210, 90), RGB(0, 0, 0));
        SetToolBarColors(RGB(192, 210, 238), RGB(192, 210, 238), RGB(152, 181, 226), RGB(152, 181, 226), RGB(49, 106, 197));
        SetCaptionColor(RGB(230, 180, 0));
        SetCaptionTextColor(RGB(0, 0, 0));
        break;
    case IDM_OCHRE:   // Orange Theme
        SetReBarColors(RGB(248, 132, 12), RGB(248, 198, 10), RGB(248, 210, 20), RGB(248, 141, 6));
        SetMenuBarColors(RGB(196, 215, 250), RGB(120, 180, 220), RGB(150,190,245), RGB(220,230,250), RGB(128, 128, 128), RGB(0, 0, 0));
        SetStatusBarColors(RGB(248, 132, 12), RGB(248, 198, 10), RGB(0, 0, 0));
        SetToolBarColors(RGB(192, 210, 238), RGB(192, 210, 238), RGB(152, 181, 226), RGB(152, 181, 226), RGB(49, 106, 197));
        SetCaptionColor(RGB(248, 132, 12));
        SetCaptionTextColor(RGB(0, 0, 0));
        break;
    case IDM_PINK:    // Pink Theme
        SetReBarColors(RGB(255, 130, 190), RGB(250, 205, 235), RGB(250, 205, 235), RGB(255, 120, 170));
        SetMenuBarColors(RGB(196, 215, 250), RGB(120, 180, 220), RGB(255, 130, 190), RGB(250, 205, 235), RGB(128, 128, 128), RGB(0, 0, 230));
        SetStatusBarColors(RGB(255, 130, 190), RGB(250, 205, 235), RGB(0, 0, 230));
        SetToolBarColors(RGB(192, 210, 238), RGB(192, 210, 238), RGB(248, 147, 220), RGB(248, 153, 179), RGB(49, 106, 197));
        SetCaptionColor(RGB(255, 130, 190));
        SetCaptionTextColor(RGB(0, 0, 230));
        break;
    case IDM_MAGENTA:  // Magenta Theme
        SetReBarColors(RGB(210, 6, 100), RGB(240, 66, 120), RGB(240, 66, 128), RGB(110, 6, 105));
        SetMenuBarColors(RGB(246, 6, 150), RGB(220, 6, 120), RGB(210,6,145), RGB(220,6,150), RGB(255,128,210), RGB(255, 255, 0));
        SetStatusBarColors(RGB(210, 6, 100), RGB(240, 66, 120), RGB(255, 255, 0));
        SetToolBarColors(RGB(242, 106, 228), RGB(192, 6, 88), RGB(152, 6, 56), RGB(152, 6, 56), RGB(149, 6, 197));
        SetCaptionColor(RGB(210, 6, 100));
        SetCaptionTextColor(RGB(255, 255, 0));
        break;
    case IDM_BLACK:   // Black Theme
        SetReBarColors(RGB(30, 30, 30), RGB(30, 30, 30), RGB(50, 50, 50), RGB(50, 50, 50));
        SetMenuBarColors(RGB(30, 30, 30), RGB(30, 30, 30), RGB(40, 40, 40), RGB(40, 40, 40), RGB(96, 96, 96), RGB(255, 255, 255));
        SetStatusBarColors(RGB(30, 30, 30), RGB(30, 30, 30), RGB(255, 255, 255));
        SetToolBarColors(RGB(49, 106, 197), RGB(64,177,230), RGB(27,65, 160), RGB(64, 177, 230), RGB(49, 106, 197));
        SetCaptionColor(RGB(30, 30, 30));
        SetCaptionTextColor(RGB(255, 255, 255));
        break;
    default:
        m_color = IDM_WIN11;
        SetReBarColors(RGB(235, 237, 250), RGB(235, 237, 250), RGB(220, 225, 240), RGB(220, 225, 240));
        SetMenuBarColors( RGB(180, 250, 255), RGB(140, 190, 255), RGB(240, 250, 255), RGB(120, 170, 220), RGB(127, 127, 255), RGB(0, 0, 0) );
        SetStatusBarColors(RGB(235, 237, 250), RGB(235, 237, 250), RGB(0, 0, 0));
        SetToolBarColors(RGB(180, 250, 255), RGB(140, 190, 255), RGB(150, 220, 255), RGB(80, 100, 255), RGB(127, 127, 255));
        SetCaptionColor(RGB(235, 237, 250));
        SetCaptionTextColor(RGB(0, 0, 0));
        break;
    }

    // Use a dark menu for the black theme.
    (color == IDM_BLACK) ? UseDarkMenu(TRUE) : UseDarkMenu(FALSE);

    RecalcLayout();
    return TRUE;
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    // Set the registry key name, and load the initial window position.
    // Use a registry key name like "CompanyName\\Application".
    LoadRegistrySettings(_T("Win32++\\Themes Sample"));

    return CFrame::Create(parent);
}

// Loads the application's settings from the registry when the application starts.
BOOL CMainFrame::LoadRegistrySettings(LPCTSTR keyName)
{
    // Call the base class function.
    if (CFrame::LoadRegistrySettings(keyName))
    {
        CString settingsKeyName;
        settingsKeyName << _T("Software\\") << keyName << _T("\\Theme Settings");
        CRegKey settingsKey;

        int result = settingsKey.Open(HKEY_CURRENT_USER, settingsKeyName);
        if (result == ERROR_SUCCESS)
        {
            DWORD bands = 0;
            result |= settingsKey.QueryDWORDValue(_T("ColorStyle"), m_color);
            result |= settingsKey.QueryBoolValue(_T("UseThemes"), m_useThemes);
            result |= settingsKey.QueryBoolValue(_T("UseBandColors"), m_useBandColors);
            result |= settingsKey.QueryBoolValue(_T("UseFlatStyle"), m_useFlatStyle);
            result |= settingsKey.QueryBoolValue(_T("PutBandsLeft"), m_keepBandsLeft);
            result |= settingsKey.QueryBoolValue(_T("LockMenuBand"), m_lockMenuBand);
            result |= settingsKey.QueryBoolValue(_T("UseRoundBorders"), m_useRoundBorders);
            result |= settingsKey.QueryBoolValue(_T("UseShortBands"), m_useShortBands);
            result |= settingsKey.QueryBoolValue(_T("UseLines"), m_useLines);
            result |= settingsKey.QueryBoolValue(_T("ShowArrows"), m_showArrows);
            result |= settingsKey.QueryBoolValue(_T("ShowCards"), m_showCards);
            result |= settingsKey.QueryDWORDValue(_T("NumBands"), bands);

            // Retrieve the band styles and IDs
            for (UINT i = 0; i < bands; ++i)
            {
                CString bandKeyName;
                DWORD id = 0;
                bandKeyName.Format(_T("Band ID %d\0"), i + 1);
                result |= settingsKey.QueryDWORDValue(bandKeyName, id);
                BandData bandData;
                bandData.index = i;
                bandData.id = id;
                m_bandData.push_back(bandData);
            }
        }

        if (result != ERROR_SUCCESS)
        {
            // Choose reasonable default values
            m_color = IDM_OLIVE;
            m_useThemes = true;
            m_useBandColors = true;
            m_useFlatStyle = false;
            m_keepBandsLeft = true;
            m_lockMenuBand = true;
            m_useRoundBorders = true;
            m_useShortBands = true;
            m_useLines = false;
            m_showArrows = true;
            m_showCards = true;
        }
    }

    return TRUE;
}

// Toggles the use of theme colors for rebar bands.
BOOL CMainFrame::OnBandColors()
{
    if (IsReBarSupported())
    {
        m_useBandColors = !m_useBandColors;
        ChooseColor(m_color);

        GetReBar().RedrawWindow(RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
        RecalcLayout();
    }

    return TRUE;
}

// OnCommand responds to menu and and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch(id)
    {
    case IDM_FILE_EXIT:      return OnFileExit();
    case IDM_WIN7:           return ChooseColor(IDM_WIN7);
    case IDM_BLUE:           return ChooseColor(IDM_BLUE);
    case IDM_GRAY:           return ChooseColor(IDM_GRAY);
    case IDM_OLIVE:          return ChooseColor(IDM_OLIVE);
    case IDM_OCHRE:          return ChooseColor(IDM_OCHRE);
    case IDM_MAGENTA:        return ChooseColor(IDM_MAGENTA);
    case IDM_PINK:           return ChooseColor(IDM_PINK);
    case IDM_GOLD:           return ChooseColor(IDM_GOLD);
    case IDM_BLACK:          return ChooseColor(IDM_BLACK);
    case IDM_WIN11:          return ChooseColor(IDM_WIN11);
    case IDM_USE_THEMES:     return OnUseThemes();
    case IDM_BAND_COLORS:    return OnBandColors();
    case IDM_FLAT_STYLE:     return OnFlatStyle();
    case IDM_LEFT_BANDS:     return OnLeftBands();
    case IDM_LOCK_MENUBAR:   return OnLockMenuBar();
    case IDM_ROUND_BORDERS:  return OnRoundBorders();
    case IDM_SHORT_BANDS:    return OnShortBands();
    case IDM_USE_LINES:      return OnUseLines();
    case IDM_VIEW_ARROWS:    return OnViewArrows();
    case IDM_VIEW_CARDS:     return OnViewCards();
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
    // UseStatusBar(FALSE);          // Don't use a StatusBar.
    // UseToolBar(FALSE);            // Don't use a ToolBar.

    // Call the base class function
    CFrame::OnCreate(cs);

    if (IsReBarSupported())
    {
        // Set our theme colors.
        ChooseColor(m_color);

        // Set the MenuBar's position and gripper.
        int band = GetReBar().GetBand(GetMenuBar());
        GetReBar().ShowGripper(band, !m_lockMenuBand);
        if (m_lockMenuBand)
            GetReBar().MoveBand(band, 0);

        // Arrange bands from information loaded from registry.
        ArrangeBands();

        ShowArrows(m_showArrows);
        ShowCards(m_showCards);
    }
    else
        MessageBox(_T("Some Theme features are not supported on this Operating System"), _T("Warning"), MB_ICONWARNING);

    return 0;
}

// Called when the effective dots per inch (dpi) for a window has changed.
// This occurs when:
//  - The window is moved to a new monitor that has a different DPI.
//  - The DPI of the monitor hosting the window changes.
LRESULT CMainFrame::OnDpiChanged(UINT msg, WPARAM wparam, LPARAM lparam)
{
    m_selectedItem = m_comboBoxEx.GetCurSel();

    // Delete the rebar bands holding the cards and arrows toolbars.
    if (GetReBar().IsWindow())
    {
        int band = GetReBar().GetBand(m_arrows);
        GetReBar().DeleteBand(band);
        band = GetReBar().GetBand(m_cards);
        GetReBar().DeleteBand(band);
    }

    // Destroy the cards and arrows toolbars.
    m_arrows.Destroy();
    m_cards.Destroy();

    // Call the base class function. This recreates the toolbars.
    CFrame::OnDpiChanged(msg, wparam, lparam);

    PostMessage(UWM_ARRANGEBANDS);
    return 0;
}

BOOL CMainFrame::OnFileExit()
{
    // Issue a close request to the frame.
    Close();
    return TRUE;
}

// Toggles the flat style display of toolbars.
BOOL CMainFrame::OnFlatStyle()
{
    if (IsReBarSupported())
    {
        m_useFlatStyle = !m_useFlatStyle;
        ReBarTheme rbt = GetReBarTheme();
        rbt.FlatStyle = m_useFlatStyle;
        SetReBarTheme(rbt);

        GetReBar().RedrawWindow(RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
        RecalcLayout();
    }

    return TRUE;
}

// Called before OnDpiChanged when a DPI change occurs.
LRESULT CMainFrame::OnGetDpiScaledSize(UINT, WPARAM, LPARAM)
{
    m_bandData.clear();
    REBARBANDINFO rbbi;
    ZeroMemory(&rbbi, sizeof(rbbi));
    rbbi.fMask = RBBIM_ID | RBBIM_STYLE | RBBIM_SIZE;

    // Store the current band arrangement in m_bandData.
    int bands = GetReBar().GetBandCount();
    for (int i = 0; i < bands; i++)
    {
        GetReBar().GetBandInfo(i, rbbi);
        BandData data;
        data.index = i;
        data.id = rbbi.wID;
        m_bandData.push_back(data);
    }

    return 0;
}

// Limit the minimum size of the window.
LRESULT CMainFrame::OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam)
{
    LPMINMAXINFO lpMMI = (LPMINMAXINFO)lparam;
    const CSize minimumSize(350, 300);
    lpMMI->ptMinTrackSize.x = DpiScaleInt(minimumSize.cx);
    lpMMI->ptMinTrackSize.y = DpiScaleInt(minimumSize.cy);
    return FinalWindowProc(msg, wparam, lparam);
}

// Called when the UWM_ARRANGEBANDS message is posted.
LRESULT CMainFrame::OnArrangeBands(UINT, WPARAM, LPARAM)
{
    ArrangeBands();
    return 0;
}

void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    TRACE("Frame created\n");
}

// Toggles the positioning of bands left.
BOOL CMainFrame::OnLeftBands()
{
    if (IsReBarSupported())
    {
        m_keepBandsLeft = !m_keepBandsLeft;
        ReBarTheme rbt = GetReBarTheme();
        rbt.BandsLeft = m_keepBandsLeft;
        SetReBarTheme(rbt);

        GetReBar().RedrawWindow(RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
        RecalcLayout();
    }

    return TRUE;
}

// Toggles locking the menubar in the first row of the rebar.
BOOL CMainFrame::OnLockMenuBar()
{
    if (IsReBarSupported())
    {
        m_lockMenuBand = !m_lockMenuBand;
        ReBarTheme rbt = GetReBarTheme();
        rbt.LockMenuBand = m_lockMenuBand;
        SetReBarTheme(rbt);
        GetReBar().MoveBand(GetReBar().GetBand(GetMenuBar()), 0);   // Move the MenuBar to band 0
        GetReBar().ShowGripper(GetReBar().GetBand(GetMenuBar()), !m_lockMenuBand);

        GetReBar().RedrawWindow(RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
        RecalcLayout();
    }

    return TRUE;
}

// Called just before the menu is displayed.
void CMainFrame::OnMenuUpdate(UINT id)
{
    // Update the check buttons before displaying the menu.
    switch (id)
    {
    case IDM_USE_THEMES:
        GetFrameMenu().CheckMenuItem(id, m_useThemes ? MF_CHECKED : MF_UNCHECKED);
        break;
    case IDM_BAND_COLORS:
        GetFrameMenu().CheckMenuItem(id, m_useBandColors ? MF_CHECKED : MF_UNCHECKED);
        break;
    case IDM_FLAT_STYLE:
        GetFrameMenu().CheckMenuItem(id, m_useFlatStyle ? MF_CHECKED : MF_UNCHECKED);
        break;
    case IDM_LEFT_BANDS:
        GetFrameMenu().CheckMenuItem(id, m_keepBandsLeft ? MF_CHECKED : MF_UNCHECKED);
        break;
    case IDM_LOCK_MENUBAR:
        GetFrameMenu().CheckMenuItem(id, m_lockMenuBand ? MF_CHECKED : MF_UNCHECKED);
        break;
    case IDM_ROUND_BORDERS:
        GetFrameMenu().CheckMenuItem(id, m_useRoundBorders ? MF_CHECKED : MF_UNCHECKED);
        break;
    case IDM_SHORT_BANDS:
        GetFrameMenu().CheckMenuItem(id, m_useShortBands ? MF_CHECKED : MF_UNCHECKED);
        break;
    case IDM_USE_LINES:
        GetFrameMenu().CheckMenuItem(id, m_useLines ? MF_CHECKED : MF_UNCHECKED);
        break;
    case IDM_VIEW_ARROWS:
        GetFrameMenu().CheckMenuItem(id, m_showArrows ? MF_CHECKED : MF_UNCHECKED);
        break;
    case IDM_VIEW_CARDS:
        GetFrameMenu().CheckMenuItem(id, m_showCards ? MF_CHECKED : MF_UNCHECKED);
        break;
    }

    // Check the color menu item and make it a radio item.
    GetFrameMenu().CheckMenuRadioItem(IDM_WIN11, IDM_BLACK, m_color, MF_BYCOMMAND);

    // Call the base class member function.
    CFrame::OnMenuUpdate(id);
}

// Toggle round borders for toolbars in the rebar.
BOOL CMainFrame::OnRoundBorders()
{
    if (IsReBarSupported())
    {
        m_useRoundBorders = !m_useRoundBorders;
        ReBarTheme rbt = GetReBarTheme();
        rbt.RoundBorders = m_useRoundBorders;
        SetReBarTheme(rbt);

        GetReBar().RedrawWindow(RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
        RecalcLayout();
    }

    return TRUE;
}

// Toggle short toolbars in the rebar.
BOOL CMainFrame::OnShortBands()
{
    if (IsReBarSupported())
    {
        m_useShortBands = !m_useShortBands;
        ReBarTheme rbt = GetReBarTheme();
        rbt.ShortBands = m_useShortBands;
        SetReBarTheme(rbt);

        GetReBar().RedrawWindow(RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
        RecalcLayout();
    }

    return TRUE;
}

// Toggles the display of lines betweeen rebar rows.
BOOL CMainFrame::OnUseLines()
{
    if (IsReBarSupported())
    {
        m_useLines = !m_useLines;
        ReBarTheme rbt = GetReBarTheme();
        rbt.UseLines = m_useLines;
        SetReBarTheme(rbt);

        GetReBar().RedrawWindow(RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
        RecalcLayout();
    }

    return TRUE;
}

// Toggles the use of themes.
BOOL CMainFrame::OnUseThemes()
{
    if (IsReBarSupported())
    {
        m_useThemes = !m_useThemes;

        ReBarTheme rbt = GetReBarTheme();
        rbt.UseThemes = m_useThemes;
        SetReBarTheme(rbt);

        MenuTheme mt = GetMenuBarTheme();
        mt.UseThemes = m_useThemes;
        SetMenuTheme(mt);

        StatusBarTheme sbt = GetStatusBarTheme();
        sbt.UseThemes = m_useThemes;
        SetStatusBarTheme(sbt);

        ToolBarTheme tbt = GetToolBarTheme();
        tbt.UseThemes = m_useThemes;
        SetToolBarTheme(tbt);

        int band = GetReBar().GetBand(GetMenuBar());
        GetReBar().ShowGripper(band, !m_useThemes);

        GetReBar().RedrawWindow(RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
        RecalcLayout();
    }

    return TRUE;
}

// Toggles the display of the arrows toolbar.
BOOL CMainFrame::OnViewArrows()
{
    if (IsReBarSupported())
    {
        m_showArrows = !m_showArrows;
        ShowArrows(m_showArrows);
    }

    return TRUE;
}

// Toggles the display of the cards toolbar.
BOOL CMainFrame::OnViewCards()
{
    if (IsReBarSupported())
    {
        m_showCards = !m_showCards;
        ShowCards(m_showCards);
    }

    return TRUE;
}

// Saves the application's settings in the registry when the application closes.
BOOL CMainFrame::SaveRegistrySettings()
{
    if (IsReBarSupported())
    {
        // Call the base class function
        if (CFrame::SaveRegistrySettings())
        {
            CRegKey settingsKey;
            CString settingsKeyName;
            settingsKeyName << _T("Software\\") << GetRegistryKeyName() << _T("\\Theme Settings");
            int bands = GetReBar().GetBandCount();

            settingsKey.Create(HKEY_CURRENT_USER, settingsKeyName);
            settingsKey.Open(HKEY_CURRENT_USER, settingsKeyName);

            // Save the theme settings
            settingsKey.SetDWORDValue(_T("ColorStyle"), m_color);
            settingsKey.SetBoolValue(_T("UseThemes"), m_useThemes);
            settingsKey.SetBoolValue(_T("UseBandColors"), m_useBandColors);
            settingsKey.SetBoolValue(_T("UseFlatStyle"), m_useFlatStyle);
            settingsKey.SetBoolValue(_T("PutBandsLeft"), m_keepBandsLeft);
            settingsKey.SetBoolValue(_T("LockMenuBand"), m_lockMenuBand);
            settingsKey.SetBoolValue(_T("UseRoundBorders"), m_useRoundBorders);
            settingsKey.SetBoolValue(_T("UseShortBands"), m_useShortBands);
            settingsKey.SetBoolValue(_T("UseLines"), m_useLines);
            settingsKey.SetBoolValue(_T("ShowArrows"), m_showArrows);
            settingsKey.SetBoolValue(_T("ShowCards"), m_showCards);
            settingsKey.SetDWORDValue(_T("NumBands"), bands);

            // Save the rebar band settings
            REBARBANDINFO rbbi;
            ZeroMemory(&rbbi, sizeof(rbbi));
            rbbi.fMask = RBBIM_ID | RBBIM_STYLE | RBBIM_SIZE;

            for (int i = 0; i < bands; i++)
            {
                GetReBar().GetBandInfo(i, rbbi);
                UINT id = rbbi.wID;
                CString bandKeyName;
                bandKeyName.Format(_T("Band ID %d\0"), i + 1);
                settingsKey.SetDWORDValue(bandKeyName, id);
            }
        }
    }

    return TRUE;
}

// This function uses the DwmSetWindowAttribute Windows API function to set
// the caption color. The DWMWA_CAPTION_COLOR option requires Windows 11.
void CMainFrame::SetCaptionColor(COLORREF color)
{
    HMODULE dwmapi = ::LoadLibrary(_T("Dwmapi.dll"));
    if (dwmapi != 0)
    {
        typedef UINT WINAPI DWMSETWINDOWATTRIBUE(HWND, DWORD, LPCVOID, DWORD);
        DWMSETWINDOWATTRIBUE* pDwmSetWindowAttribute = reinterpret_cast<DWMSETWINDOWATTRIBUE*>(
            reinterpret_cast<void*>(::GetProcAddress(dwmapi, "DwmSetWindowAttribute")));

        const int DWMWA_CAPTION_COLOR = 35;
        pDwmSetWindowAttribute(*this, DWMWA_CAPTION_COLOR, &color, sizeof(color));
        ::FreeLibrary(dwmapi);
    }
}

// This function uses the DwmSetWindowAttribute Windows API function to set
// the caption text color. The DWMWA_CAPTION_COLOR option requires Windows 11.
void CMainFrame::SetCaptionTextColor(COLORREF color)
{
    HMODULE dwmapi = ::LoadLibrary(_T("Dwmapi.dll"));
    if (dwmapi != 0)
    {
        typedef UINT WINAPI DWMSETWINDOWATTRIBUE(HWND, DWORD, LPCVOID, DWORD);
        DWMSETWINDOWATTRIBUE* pDwmSetWindowAttribute = reinterpret_cast<DWMSETWINDOWATTRIBUE*>(
            reinterpret_cast<void*>(::GetProcAddress(dwmapi, "DwmSetWindowAttribute")));

        const int DWMWA_TEXT_COLOR = 36;
        pDwmSetWindowAttribute(*this, DWMWA_TEXT_COLOR, &color, sizeof(color));
        ::FreeLibrary(dwmapi);
    }
}

// Sets the menubar theme colors.
void CMainFrame::SetMenuBarColors(COLORREF hot1, COLORREF hot2, COLORREF pressed1, COLORREF pressed2, COLORREF outline, COLORREF text)
{
    MenuTheme mt;

    mt.UseThemes = TRUE;        // TRUE if menubar themes are used
    mt.clrHot1 = hot1;          // Color 1 for menu bar. Color of selected menu item
    mt.clrHot2 = hot2;          // Color 2 for menu bar. Color of checkbox background
    mt.clrPressed1 = pressed1;  // Color 1 for pressed menu bar and side bar
    mt.clrPressed2 = pressed2;  // Color 2 for pressed menu bar and side bar
    mt.clrOutline = outline;    // Color for border outline
    mt.clrText = text;          // Color for the menu bar text

    SetMenuTheme(mt);
}


// Sets the rebar theme colors and settings.
void CMainFrame::SetReBarColors(COLORREF backGround1, COLORREF backGround2, COLORREF band1, COLORREF band2)
{
    if (IsReBarSupported())
    {
        ReBarTheme rt;
        rt.UseThemes = m_useThemes;          // TRUE if rebar themes are used
        rt.clrBkgnd1 = backGround1;          // Color 1 for rebar background
        rt.clrBkgnd2 = backGround2;          // Color 2 for rebar background
        rt.clrBand1  = band1;                // Color 1 for rebar band background. Use 0 if not required
        rt.clrBand2  = band2;                // Color 2 for rebar band background. Use 0 if not required
        rt.FlatStyle = m_useFlatStyle;       // Bands are rendered with flat rather than raised style
        rt.BandsLeft = m_keepBandsLeft;      // Position bands left on rearrange
        rt.LockMenuBand = m_lockMenuBand;    // Lock MenuBar's band in dedicated top row, without gripper
        rt.RoundBorders = m_useRoundBorders; // Use rounded band borders
        rt.ShortBands = m_useShortBands;     // Allows bands to be shorter than maximum available width
        rt.UseLines = m_useLines;            // Displays horizontal lines between bands

        if (!m_useBandColors)
        {
            rt.clrBand1 = 0;
            rt.clrBand2 = 0;
        }

        SetReBarTheme(rt);
    }
}

// Sets the statusbar theme colors.
void CMainFrame::SetStatusBarColors(COLORREF bkgnd1, COLORREF bkgnd2, COLORREF text)
{
    StatusBarTheme sbt;

    sbt.UseThemes = TRUE;     // TRUE if statusbar themes are used
    sbt.clrBkgnd1 = bkgnd1;   // Color 1 for statusbar background
    sbt.clrBkgnd2 = bkgnd2;   // Color 2 for statusbar background
    sbt.clrText = text;       // Text Color

    SetStatusBarTheme(sbt);
}

// Sets the toolbar theme colors.
void CMainFrame::SetToolBarColors(COLORREF hot1, COLORREF hot2, COLORREF pressed1, COLORREF pressed2, COLORREF outline)
{
    ToolBarTheme tbt;

    tbt.UseThemes = TRUE;        // TRUE if toolbar themes are used
    tbt.clrHot1 = hot1;          // Color 1 for hot button
    tbt.clrHot2 = hot2;          // Color 2 for hot button
    tbt.clrPressed1 = pressed1;  // Color 1 for pressed button
    tbt.clrPressed2 = pressed2;  // Color 2 for pressed button
    tbt.clrOutline = outline;    // Color for border outline

    SetToolBarTheme(tbt);
}

// We select the theme from the menu, so we override the default
// behavior of SetTheme.
void CMainFrame::SetTheme()
{
}

// Specifies the bitmap resources used for menu items.
void CMainFrame::SetupMenuIcons()
{
    std::vector<UINT> data = GetToolBarData();
    if ((GetMenuIconHeight() >= 24) && (GetWindowDpi(*this) != 192))
        SetMenuIcons(data, RGB(255, 0, 255), IDB_TOOLBAR_NORM, IDB_TOOLBAR_DIS);
    else
        SetMenuIcons(data, RGB(192, 192, 192), IDB_TOOLBAR_SML);
}

// Configures the toolbars.
void CMainFrame::SetupToolBar()
{
    // Set the Resource IDs for the first toolbar buttons
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_OPEN  );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_FILE_SAVE  );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_EDIT_CUT   );
    AddToolBarButton( IDM_EDIT_COPY  );
    AddToolBarButton( IDM_EDIT_PASTE );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_FILE_PRINT );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_HELP_ABOUT );

    // Set the three image lists for the first toolbar
    SetToolBarImages(RGB(255, 0, 255), IDB_TOOLBAR_NORM, IDB_TOOLBAR_HOT, IDB_TOOLBAR_DIS);

    // Add the two other toolbars if we can use rebars (Need Win95 and IE 4 or better)
    if (IsReBarSupported())
    {
        // Add the Arrows toolbar.
        AddToolBarBand(m_arrows, 0, IDC_ARROWS);
        m_arrows.AddButton(IDM_ARROW_LEFT);
        m_arrows.AddButton(IDM_ARROW_RIGHT);

        // Add the Cards toolbar.
        AddToolBarBand(m_cards, 0, IDB_CARDS);
        m_cards.AddButton(IDM_CARD_CLUB);
        m_cards.AddButton(IDM_CARD_DIAMOND);
        m_cards.AddButton(IDM_CARD_HEART);
        m_cards.AddButton(IDM_CARD_SPADE);

        // Set the three image lists for the arrows and cards toolbars.
        SetToolBarImages(m_arrows, RGB(255, 0, 255), IDB_ARROWS, 0, 0);
        SetToolBarImages(m_cards, RGB(255, 0, 255), IDB_CARDS, 0, 0);
    }

    // Use PostMessage to add the combo late to fix drawing issues
    // that arise when the display scale is changed.
    PostMessage(UWM_ADDCOMBO);
}

// Displays or hides the arrows toolbar.
void CMainFrame::ShowArrows(bool isShown)
{
    if (IsReBarSupported())
    {
        GetReBar().ShowBand(GetReBar().GetBand(m_arrows), isShown);

        if (GetReBarTheme().UseThemes && GetReBarTheme().BandsLeft)
            GetReBar().MoveBandsLeft();
    }
}

// Displays or hides the cards toolbar.
void CMainFrame::ShowCards(bool isShown)
{
    if (IsReBarSupported())
    {
        GetReBar().ShowBand(GetReBar().GetBand(m_cards), isShown);

        if (GetReBarTheme().UseThemes && GetReBarTheme().BandsLeft)
            GetReBar().MoveBandsLeft();
    }
}

// Handle the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case UWM_ADDCOMBO:         return AddCombo();
        case UWM_ARRANGEBANDS:     return OnArrangeBands(msg, wparam, lparam);
        case WM_GETDPISCALEDSIZE:  return OnGetDpiScaledSize(msg, wparam, lparam);
        case WM_GETMINMAXINFO:     return OnGetMinMaxInfo(msg, wparam, lparam);
        }

        //  pass unhandled messages on for default processing
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


