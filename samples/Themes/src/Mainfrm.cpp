////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame() : m_color(IDM_BLUE), m_useThemes(TRUE), m_useBandColors(TRUE), m_useFlatStyle(FALSE),
                            m_keepBandsLeft(TRUE), m_lockMenuBand(TRUE), m_useRoundBorders(TRUE),
                            m_useShortBands(TRUE), m_useLines(FALSE), m_showArrows(TRUE), m_showCards(TRUE)
{
    // Constructor for CMainFrame. Its called after CFrame's constructor

    //Set m_View as the view window of the frame
    SetView(m_view);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\Themes Sample"));
}

CMainFrame::~CMainFrame()
{
    // Destructor for CMainFrame.
}

void CMainFrame::AddCombo()
{
    // We'll be placing the ComboBoxEx control over the 'File Save' toolbar button
    int comboWidth = 120;
    CToolBar& tb = GetToolBar();
    if (tb.CommandToIndex(IDM_FILE_SAVE) < 0) return;

    tb.SetButtonStyle(IDM_FILE_SAVE, TBSTYLE_SEP);  // Convert the button to a separator
    tb.SetButtonWidth(IDM_FILE_SAVE, comboWidth);

    // Determine the size and position of the ComboBox
    int index = tb.CommandToIndex(IDM_FILE_SAVE);
    CRect rc = tb.GetItemRect(index);

    // Create and position the ComboboxEx window
    m_comboBoxEx.Create(tb);
    m_comboBoxEx.SetWindowPos(NULL, rc, SWP_NOACTIVATE);

    // Set ComboBox Height
    m_comboBoxEx.SetItemHeight(-1, rc.Height() - 6);

    m_comboBoxEx.AddItems();
    RecalcLayout();
}

BOOL CMainFrame::ChooseColor(UINT color)
{
    m_color = color;

    switch (color)
    {
    case IDM_WIN10:     // Recommended for Windows 8 and Windows 10
        {
            SetReBarColors( RGB(235, 237, 250), RGB(235, 237, 250), RGB(220, 225, 240), RGB(220, 225, 240) );

            StatusBarTheme sbt = {m_useThemes, RGB(235, 237, 250), RGB(235, 237, 250)};
            SetStatusBarTheme (sbt);

            ToolBarTheme tbt = {m_useThemes, RGB(180, 250, 255), RGB(140, 190, 255), RGB(150, 220, 255), RGB(80, 100, 255), RGB(127, 127, 255)};
            SetToolBarTheme(tbt);

            MenuTheme mt = {m_useThemes, RGB(180, 250, 255), RGB(140, 190, 255), RGB(240, 250, 255), RGB(120, 170, 220), RGB(127, 127, 255)};
            SetMenuTheme(mt);
        }
        break;
    case IDM_WIN7:  // Recommended for Vista and Windows 7
        {
            SetReBarColors( RGB(225, 230, 255), RGB(240, 242, 250), RGB(248, 248, 248), RGB(180, 200, 230) );

            StatusBarTheme sbt = {m_useThemes, RGB(225, 230, 255), RGB(240, 242, 250)};
            SetStatusBarTheme (sbt);

            ToolBarTheme tbt = {m_useThemes, RGB(180, 250, 255), RGB(140, 190, 255), RGB(150, 220, 255), RGB(80, 100, 255), RGB(127, 127, 255)};
            SetToolBarTheme(tbt);

            MenuTheme mt = {m_useThemes, RGB(180, 250, 255), RGB(140, 190, 255), RGB(240, 250, 255), RGB(120, 170, 220), RGB(127, 127, 255)};
            SetMenuTheme(mt);
        }
        break;
    case IDM_BLUE:      // Recommended for Windows XP
        {
            SetReBarColors( RGB(150,190,245), RGB(196,215,250), RGB(220,230,250), RGB( 70,130,220) );

            StatusBarTheme sbt = {m_useThemes, RGB(150,190,245), RGB(196,215,250)};
            SetStatusBarTheme (sbt);

            ToolBarTheme tbt = {m_useThemes, RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(128, 128, 255)};
            SetToolBarTheme(tbt);

            MenuTheme mt = {m_useThemes, RGB(255, 230, 190), RGB(255, 190, 100), RGB(150,190,245), RGB(220,230,250), RGB(128, 128, 200)};
            SetMenuTheme(mt);
        }
        break;
    case IDM_GREY:  // Grey Theme
        {
            SetReBarColors( RGB(210, 210, 200), RGB(238, 236, 224), RGB(248, 247, 243), RGB(195, 195, 172) );

            StatusBarTheme sbt = {m_useThemes, RGB(210, 210, 200), RGB(238, 236, 224)};
            SetStatusBarTheme (sbt);

            ToolBarTheme tbt = {m_useThemes, RGB(192, 210, 238), RGB(192, 210, 238), RGB(152, 181, 226), RGB(152, 181, 226), RGB(49, 106, 197)};
            SetToolBarTheme(tbt);

            MenuTheme mt = {m_useThemes, RGB(196, 215, 250), RGB( 120, 180, 220), RGB(210, 210, 200), RGB(248, 247, 243), RGB(128, 128, 200)};
            SetMenuTheme(mt);
        }
        break;
    case IDM_OLIVE:
        {
            SetReBarColors( RGB(160, 180, 80), RGB(180, 200, 100), RGB(200, 220, 120), RGB(80, 159, 78) );

            StatusBarTheme sbt = {m_useThemes, RGB(160, 180, 80), RGB(180, 200, 100)};
            SetStatusBarTheme (sbt);

            ToolBarTheme tbt = {m_useThemes, RGB(255, 230, 190), RGB(255, 190, 100), RGB(255, 140, 40), RGB(255, 180, 80), RGB(128, 128, 255)};
            SetToolBarTheme(tbt);

            MenuTheme mt = {m_useThemes, RGB(255, 200, 190), RGB(255, 210, 90), RGB(255, 230, 190), RGB(230, 160, 190), RGB(128, 128, 128)};
            SetMenuTheme(mt);
        }
        break;
    case IDM_GOLD:
        {
            SetReBarColors( RGB(230, 180, 0), RGB(240, 210, 90), RGB(255, 240, 150), RGB(180, 140, 50) );

            StatusBarTheme sbt = {m_useThemes, RGB(230, 180, 0), RGB(240, 210, 90)};
            SetStatusBarTheme (sbt);

            ToolBarTheme tbt = {m_useThemes, RGB(192, 210, 238), RGB(192, 210, 238), RGB(152, 181, 226), RGB(152, 181, 226), RGB(49, 106, 197)};
            SetToolBarTheme(tbt);

            MenuTheme mt = {m_useThemes, RGB(196, 215, 250), RGB( 120, 180, 220), RGB(240, 210, 90), RGB(255, 240, 150), RGB(128, 128, 128)};
            SetMenuTheme(mt);
        }
        break;
    case IDM_OCHRE:
        {
            SetReBarColors( RGB(248, 132, 12), RGB(248, 198, 10), RGB(248, 210, 20), RGB(248, 141, 6) );

            StatusBarTheme sbt = {m_useThemes, RGB(248, 132, 12), RGB(248, 198, 10)};
            SetStatusBarTheme (sbt);

            ToolBarTheme tbt = {m_useThemes, RGB(192, 210, 238), RGB(192, 210, 238), RGB(152, 181, 226), RGB(152, 181, 226), RGB(49, 106, 197)};
            SetToolBarTheme(tbt);

            MenuTheme mt = {m_useThemes, RGB(196, 215, 250), RGB( 120, 180, 220), RGB(150,190,245), RGB(220,230,250), RGB(128, 128, 128)};
            SetMenuTheme(mt);
        }
        break;
    case IDM_PINK:
        {
            SetReBarColors( RGB(255, 130, 190), RGB(250, 205, 235), RGB(250, 205, 235), RGB(255, 120, 170) );

            StatusBarTheme sbt = {m_useThemes, RGB(255, 130, 190), RGB(250, 205, 235)};
            SetStatusBarTheme (sbt);

            ToolBarTheme tbt = {m_useThemes, RGB(192, 210, 238), RGB(192, 210, 238), RGB(248, 147, 220), RGB(248, 153, 179), RGB(49, 106, 197)};
            SetToolBarTheme(tbt);

            MenuTheme mt = {m_useThemes, RGB(196, 215, 250), RGB( 120, 180, 220), RGB(255, 130, 190), RGB(250, 205, 235), RGB(128, 128, 128)};
            SetMenuTheme(mt);
        }
        break;
    case IDM_MAUVE:
        {
            SetReBarColors( RGB(210, 128, 155), RGB(230, 160, 190), RGB(230, 158, 188), RGB(110, 100, 125) );

            StatusBarTheme sbt = {m_useThemes, RGB(210, 128, 155), RGB(230, 160, 190)};
            SetStatusBarTheme (sbt);

            ToolBarTheme tbt = {m_useThemes, RGB(192, 210, 238), RGB(192, 210, 238), RGB(152, 181, 226), RGB(152, 181, 226), RGB(49, 106, 197)};
            SetToolBarTheme(tbt);

            MenuTheme mt = {m_useThemes, RGB(196, 215, 250), RGB( 120, 180, 220), RGB(150,190,245), RGB(220,230,250), RGB(128, 128, 128)};
            SetMenuTheme(mt);
        }
        break;
    case IDM_BLACK:
        {
            SetReBarColors( RGB(7, 17, 27), RGB(57, 67, 77), RGB(118, 128 ,136), RGB(68, 78 ,86) );

            StatusBarTheme sbt = {m_useThemes, RGB(128, 138 ,176), RGB(98, 108 ,136)};
            SetStatusBarTheme (sbt);

            ToolBarTheme tbt = {m_useThemes, RGB(49, 106, 197), RGB(64,177,230), RGB(27,65, 160), RGB(64,177,230), RGB(49, 106, 197)};
            SetToolBarTheme(tbt);

            MenuTheme mt = {m_useThemes, RGB(27,65, 160), RGB(49, 106, 197), RGB(59, 126, 197), RGB(94,187,230), RGB(128, 128, 128)};
            SetMenuTheme(mt);
        }
        break;
    default:
        {
            m_color = IDM_WIN7;
            SetReBarColors( RGB(225, 230, 255), RGB(240, 242, 250), RGB(248, 248, 248), RGB(180, 200, 230) );

            StatusBarTheme sbt = {m_useThemes, RGB(225, 230, 255), RGB(240, 242, 250)};
            SetStatusBarTheme (sbt);

            ToolBarTheme tbt = {m_useThemes, RGB(180, 250, 255), RGB(140, 190, 255), RGB(150, 220, 255), RGB(80, 100, 255), RGB(127, 127, 255)};
            SetToolBarTheme(tbt);

            MenuTheme mt = {m_useThemes, RGB(180, 250, 255), RGB(140, 190, 255), RGB(240, 250, 255), RGB(120, 170, 220), RGB(127, 127, 255)};
            SetMenuTheme(mt);
        }
        break;
    }

    // Check the appropriate menu item
    int fileItem = GetMenuItemPos(GetFrameMenu(), _T("Theme"));
    if (fileItem >= 0)
    {
        CMenu ThemeMenu = GetFrameMenu().GetSubMenu(fileItem);
        ThemeMenu.CheckMenuRadioItem(IDM_BLUE, IDM_WIN7, color, 0);
    }

    RecalcLayout();
    return TRUE;
}

LRESULT CMainFrame::CustomDrawMenuBar(NMHDR* pNMHDR)
// Set the text color for the menubar
{
    LPNMTBCUSTOMDRAW lpNMCustomDraw = (LPNMTBCUSTOMDRAW)pNMHDR;
    
    COLORREF clr;
    if (m_color == IDM_BLACK)
        clr = RGB(255,255,255);
    else
        clr = RGB(0, 0, 0);

    // Set the text color for the custom draw's device context
    CDC drawDC(lpNMCustomDraw->nmcd.hdc);
    drawDC.SetTextColor(clr);

    return CFrame::CustomDrawMenuBar(pNMHDR);
}

DWORD CMainFrame::GetRegDwordFromOpenKey(HKEY hKey, LPCTSTR pName)
{
    DWORD   type;
    DWORD   count = sizeof(DWORD);
    DWORD   value = 0;
    if (ERROR_SUCCESS == RegQueryValueEx(hKey, pName, NULL, &type, (LPBYTE)&value, &count))
      return value;
    else
      return 0;
}

BOOL CMainFrame::LoadRegistrySettings(LPCTSTR keyName)
{
    CFrame::LoadRegistrySettings(keyName);

    HKEY hKey;
    CString fullKeyName = _T("Software\\");
    fullKeyName += keyName;
    fullKeyName += (_T("\\Theme Settings"));

    if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, fullKeyName, 0, KEY_READ, &hKey))
    {
        m_color = GetRegDwordFromOpenKey(hKey, _T("ColorStyle"));
        m_useThemes = GetRegDwordFromOpenKey(hKey, _T("UseThemes")) & 1;
        m_useBandColors = GetRegDwordFromOpenKey(hKey, _T("UseBandColors")) & 1;
        m_useFlatStyle = GetRegDwordFromOpenKey(hKey, _T("UseFlatStyle")) & 1;
        m_keepBandsLeft = GetRegDwordFromOpenKey(hKey, _T("PutBandsLeft")) & 1;
        m_lockMenuBand = GetRegDwordFromOpenKey(hKey, _T("LockMenuBand")) & 1;
        m_useRoundBorders = GetRegDwordFromOpenKey(hKey, _T("UseRoundBorders")) & 1;
        m_useShortBands = GetRegDwordFromOpenKey(hKey, _T("UseShortBands")) & 1;
        m_useLines = GetRegDwordFromOpenKey(hKey, _T("UseLines")) & 1;
        m_showArrows = GetRegDwordFromOpenKey(hKey, _T("ShowArrows")) & 1;
        m_showCards = GetRegDwordFromOpenKey(hKey, _T("ShowCards")) & 1;
        int nBands = GetRegDwordFromOpenKey(hKey, _T("NumBands"));

        // Retrieve the band styles and IDs
        for (int i = 0; i < nBands; ++i)
        {
            TCHAR szSubKey[16];
            wsprintf(szSubKey, _T("Band ID %d\0"), i+1);
            UINT id = GetRegDwordFromOpenKey(hKey, szSubKey);
            m_bandIDs.push_back(id);

            wsprintf(szSubKey, _T("Band Style %d\0"), i+1);
            UINT nStyle = GetRegDwordFromOpenKey(hKey, szSubKey);
            m_bandStyles.push_back(nStyle);

            wsprintf(szSubKey, _T("Band Size %d\0"), i+1);
            UINT nSize = GetRegDwordFromOpenKey(hKey, szSubKey);
            m_bandSizes.push_back(nSize);
        }

        RegCloseKey(hKey);
    }
    else
    {
        // Choose reasonable default values
        m_color = IDM_OLIVE;
        m_useThemes = TRUE;
        m_useBandColors = TRUE;
        m_useFlatStyle = FALSE;
        m_keepBandsLeft = TRUE;
        m_lockMenuBand = TRUE;
        m_useRoundBorders = TRUE;
        m_useShortBands = TRUE;
        m_useLines = FALSE;;
        m_showArrows = TRUE;
        m_showCards = TRUE;
    }

    return TRUE;
}

BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    // OnCommand responds to menu and and toolbar input

    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_FILE_EXIT:      return OnFileExit();
    case IDM_WIN7:           return ChooseColor(IDM_WIN7);
    case IDM_BLUE:           return ChooseColor(IDM_BLUE);
    case IDM_GREY:           return ChooseColor(IDM_GREY);
    case IDM_OLIVE:          return ChooseColor(IDM_OLIVE);
    case IDM_OCHRE:          return ChooseColor(IDM_OCHRE);
    case IDM_MAUVE:          return ChooseColor(IDM_MAUVE);
    case IDM_PINK:           return ChooseColor(IDM_PINK);
    case IDM_GOLD:           return ChooseColor(IDM_GOLD);
    case IDM_BLACK:          return ChooseColor(IDM_BLACK);
    case IDM_WIN10:          return ChooseColor(IDM_WIN10); 
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

int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
    // OnCreate controls the way the frame is created.
    // Overriding CFrame::OnCreate is optional.

    // A menu is added if the IDW_MAIN menu resource is defined.
    // Frames have all options enabled by default. 
    // Use the following functions to disable options.

    // UseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar
    // UseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar
    // UseStatusBar(FALSE);          // Don't use a StatusBar
    // UseToolBar(FALSE);            // Don't use a ToolBar

    // call the base class function
    CFrame::OnCreate(cs);

    if (IsReBarSupported())
    {
        //Set our theme colors
        ChooseColor(m_color);
        
        // Set the band styles and positions
        for (int i = 0; i < GetReBar().GetBandCount(); ++i)
        {
            if (i < static_cast<int>(m_bandStyles.size()))
            {
                // Move the band to the correct position
                int from = GetReBar().IDToIndex(m_bandIDs[i]);
                GetReBar().MoveBand(from, i);

                // Set the band's style
                REBARBANDINFO rbbi;
                ZeroMemory(&rbbi, sizeof(REBARBANDINFO));
                rbbi.fMask = RBBIM_STYLE;
                rbbi.fStyle = m_bandStyles[i];
                GetReBar().SetBandInfo(i, rbbi);
            }

            if (i < static_cast<int>(m_bandSizes.size()))
            {
                // Set the band's size
                REBARBANDINFO rbbi;
                ZeroMemory(&rbbi, sizeof(REBARBANDINFO));
                rbbi.fMask = RBBIM_SIZE;
                rbbi.cx = m_bandSizes[i];
                GetReBar().SetBandInfo(i, rbbi);
            }

        }

        // Set the MenuBar's position and gripper
        int band = GetReBar().GetBand(GetMenuBar());
        GetReBar().ShowGripper(band, !m_lockMenuBand);
        if (m_lockMenuBand)
            GetReBar().MoveBand(band, 0);

        ShowArrows(m_showArrows);
        ShowCards(m_showCards);
    }
    else
        MessageBox(_T("Some Theme features are not supported on this Operating System"), _T("Warning"), MB_ICONWARNING);

    return 0;
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

LRESULT CMainFrame::OnNotify(WPARAM wparam, LPARAM lparam)
{
    // Process notification messages sent by child windows
//  switch(((LPNMHDR)lparam)->code)
//  {
        //Add case statments for each notification message here
//  }

    // Pass any unhandled messages on for default processing
    return CFrame::OnNotify(wparam, lparam);
}

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

        GetReBar().RedrawWindow(RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
        RecalcLayout();
    }

    return TRUE;
}

BOOL CMainFrame::OnBandColors()
{
    if (IsReBarSupported())
    {
        m_useBandColors = !m_useBandColors;
        ChooseColor(m_color);

        GetReBar().RedrawWindow(RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
        RecalcLayout();
    }

    return TRUE;
}

BOOL CMainFrame::OnFlatStyle()
{
    if (IsReBarSupported())
    {
        m_useFlatStyle = !m_useFlatStyle;
        ReBarTheme rbt = GetReBarTheme();
        rbt.FlatStyle = m_useFlatStyle;
        SetReBarTheme(rbt);

        GetReBar().RedrawWindow(RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
        RecalcLayout();
    }

    return TRUE;
}

BOOL CMainFrame::OnLeftBands()
{
    if (IsReBarSupported())
    {
        m_keepBandsLeft = !m_keepBandsLeft;
        ReBarTheme rbt = GetReBarTheme();
        rbt.BandsLeft = m_keepBandsLeft;
        SetReBarTheme(rbt);

        GetReBar().RedrawWindow(RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
        RecalcLayout();
    }

    return TRUE;
}

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

        GetReBar().RedrawWindow(RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
        RecalcLayout();
    }

    return TRUE;
}

void CMainFrame::OnMenuUpdate(UINT id)
// Called just before the menu is displayed
{
    // Update the check buttons before displaying the menu
    switch(id)
    {
    case IDM_USE_THEMES:
        GetFrameMenu().CheckMenuItem(id, m_useThemes? MF_CHECKED : MF_UNCHECKED);
        break;
    case IDM_BAND_COLORS:
        GetFrameMenu().CheckMenuItem(id, m_useBandColors? MF_CHECKED : MF_UNCHECKED);
        break;
    case IDM_FLAT_STYLE:
        GetFrameMenu().CheckMenuItem(id, m_useFlatStyle? MF_CHECKED : MF_UNCHECKED);
        break;
    case IDM_LEFT_BANDS:
        GetFrameMenu().CheckMenuItem(id, m_keepBandsLeft? MF_CHECKED : MF_UNCHECKED);
        break;
    case IDM_LOCK_MENUBAR:
        GetFrameMenu().CheckMenuItem(id, m_lockMenuBand? MF_CHECKED : MF_UNCHECKED);
        break;
    case IDM_ROUND_BORDERS:
        GetFrameMenu().CheckMenuItem(id, m_useRoundBorders? MF_CHECKED : MF_UNCHECKED);
        break;  
    case IDM_SHORT_BANDS:
        GetFrameMenu().CheckMenuItem(id, m_useShortBands? MF_CHECKED : MF_UNCHECKED);
        break;  
    case IDM_USE_LINES:
        GetFrameMenu().CheckMenuItem(id, m_useLines? MF_CHECKED : MF_UNCHECKED);
        break;
    case IDM_VIEW_ARROWS:
        GetFrameMenu().CheckMenuItem(id, m_showArrows? MF_CHECKED : MF_UNCHECKED);
        break;
    case IDM_VIEW_CARDS:
        GetFrameMenu().CheckMenuItem(id, m_showCards? MF_CHECKED : MF_UNCHECKED);
        break;
    }

    // Check the color menu item and make it a radio item 
    GetFrameMenu().CheckMenuRadioItem(IDM_WIN10, IDM_BLACK, m_color, 0);

    // Call the base class member function
    CFrame::OnMenuUpdate(id);
}

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

BOOL CMainFrame::OnUseLines()
{
    if (IsReBarSupported())
    {
        m_useLines = !m_useLines;
        ReBarTheme rbt = GetReBarTheme();
        rbt.UseLines = m_useLines;
        SetReBarTheme(rbt);

        GetReBar().RedrawWindow(RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ALLCHILDREN);
        RecalcLayout();
    }

    return TRUE;
}

BOOL CMainFrame::OnViewArrows()
{
    if (IsReBarSupported())
    {
        m_showArrows = !m_showArrows;
        ShowArrows(m_showArrows);
    }

    return TRUE;
}

BOOL CMainFrame::OnViewCards()
{
    if (IsReBarSupported())
    {
        m_showCards = !m_showCards;
        ShowCards(m_showCards);
    }

    return TRUE;
}

BOOL CMainFrame::SaveRegistrySettings()
{
    if (IsReBarSupported())
    {
        CFrame::SaveRegistrySettings();

        HKEY hKey;
        CString strKeyName = GetRegistryKeyName();
        CString strKey = _T("Software\\");
        strKey += strKeyName + (_T("\\Theme Settings"));
        int bands = GetReBar().GetBandCount();

        RegCreateKeyEx(HKEY_CURRENT_USER, strKey, 0, NULL,
        REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);

        // Save the theme settings
        RegSetValueEx(hKey, _T("ColorStyle"), 0, REG_DWORD, (LPBYTE)&m_color, sizeof(DWORD));
        RegSetValueEx(hKey, _T("UseThemes"), 0, REG_DWORD, (LPBYTE)&m_useThemes, sizeof(DWORD));
        RegSetValueEx(hKey, _T("UseBandColors"), 0, REG_DWORD, (LPBYTE)&m_useBandColors, sizeof(DWORD));
        RegSetValueEx(hKey, _T("UseFlatStyle"), 0, REG_DWORD, (LPBYTE)&m_useFlatStyle, sizeof(DWORD));
        RegSetValueEx(hKey, _T("PutBandsLeft"), 0, REG_DWORD, (LPBYTE)&m_keepBandsLeft, sizeof(DWORD));
        RegSetValueEx(hKey, _T("LockMenuBand"), 0, REG_DWORD, (LPBYTE)&m_lockMenuBand, sizeof(DWORD));
        RegSetValueEx(hKey, _T("UseRoundBorders"), 0, REG_DWORD, (LPBYTE)&m_useRoundBorders, sizeof(DWORD));
        RegSetValueEx(hKey, _T("UseShortBands"), 0, REG_DWORD, (LPBYTE)&m_useShortBands, sizeof(DWORD));
        RegSetValueEx(hKey, _T("UseLines"), 0, REG_DWORD, (LPBYTE)&m_useLines, sizeof(DWORD));
        RegSetValueEx(hKey, _T("ShowArrows"), 0, REG_DWORD, (LPBYTE)&m_showArrows, sizeof(DWORD));
        RegSetValueEx(hKey, _T("ShowCards"), 0, REG_DWORD, (LPBYTE)&m_showCards, sizeof(DWORD));
        RegSetValueEx(hKey, _T("NumBands"), 0, REG_DWORD, (LPBYTE)&bands, sizeof(DWORD));

        // Save the rebar band settings
        REBARBANDINFO rbbi;
        ZeroMemory(&rbbi, sizeof(REBARBANDINFO));
        rbbi.fMask = RBBIM_ID|RBBIM_STYLE|RBBIM_SIZE;

        for (int i = 0; i < bands; i++)
        {
            GetReBar().GetBandInfo(i, rbbi);
            UINT id = rbbi.wID;
            UINT style = rbbi.fStyle;
            UINT size = rbbi.cx;

            TCHAR szSubKey[16];
            wsprintf(szSubKey, _T("Band ID %d\0"), i+1);
            RegSetValueEx(hKey, szSubKey, 0, REG_DWORD, (LPBYTE)&id, sizeof(DWORD));
            wsprintf(szSubKey, _T("Band Style %d\0"), i+1);
            RegSetValueEx(hKey, szSubKey, 0, REG_DWORD, (LPBYTE)&style, sizeof(DWORD));
            wsprintf(szSubKey, _T("Band Size %d\0"), i+1);
            RegSetValueEx(hKey, szSubKey, 0, REG_DWORD, (LPBYTE)&size, sizeof(DWORD));
        }

        RegCloseKey(hKey);
    }

    return TRUE;
}

void CMainFrame::SetReBarColors(COLORREF backGround1, COLORREF backGround2, COLORREF band1, COLORREF band2)
{
    if (IsReBarSupported())
    {
        ReBarTheme rt;
        ZeroMemory(&rt, sizeof(ReBarTheme));
        rt.UseThemes = m_useThemes;
        rt.clrBkgnd1 = backGround1;
        rt.clrBkgnd2 = backGround2;
        rt.clrBand1  = band1;
        rt.clrBand2  = band2;
        rt.FlatStyle = m_useFlatStyle;
        rt.BandsLeft = m_keepBandsLeft;
        rt.LockMenuBand = m_lockMenuBand;
        rt.RoundBorders = m_useRoundBorders;
        rt.ShortBands = m_useShortBands;
        rt.UseLines = m_useLines;

        if (!m_useBandColors)
        {
            rt.clrBand1 = 0;
            rt.clrBand2 = 0;
        }

        SetReBarTheme(rt);
    }
}

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
        
        // Set the button images
        SetTBImageList(m_arrows, m_arrowImages, IDB_ARROWS, RGB(255,0,255));
        SetTBImageList(m_cards, m_cardImages, IDB_CARDS, RGB(255,0,255));
    }

    AddCombo();
}

void CMainFrame::ShowArrows(BOOL isShown)
{
    if (IsReBarSupported())
    {
        GetReBar().ShowBand(GetReBar().GetBand(m_arrows), isShown);

        if (GetReBarTheme().UseThemes && GetReBarTheme().BandsLeft)
            GetReBar().MoveBandsLeft();
    }
}

void CMainFrame::ShowCards(BOOL isShown)
{
    if (IsReBarSupported())
    {
        GetReBar().ShowBand(GetReBar().GetBand(m_cards), isShown);

        if (GetReBarTheme().UseThemes && GetReBarTheme().BandsLeft)
            GetReBar().MoveBandsLeft();
    }
}

LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
//  switch (msg)
//  {
//      Add case statements for each messages to be handled here
//  }

//  pass unhandled messages on for default processing
    return WndProcDefault(msg, wparam, lparam);
}


