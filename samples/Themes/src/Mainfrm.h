/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"
#include "MyCombo.h"
#include "resource.h"


///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame that has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();
    virtual HWND Create(HWND parent = 0);

protected:
    // Virtual functions that override base class functions
    virtual BOOL    LoadRegistrySettings(LPCTSTR keyName);
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual LRESULT OnDpiChanged(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void    OnInitialUpdate();
    virtual void    OnMenuUpdate(UINT id);
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual BOOL    SaveRegistrySettings();
    virtual void    SetupMenuIcons();
    virtual void    SetupToolBar();
    virtual void    SetTheme();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMainFrame(const CMainFrame&);                // Disable copy construction
    CMainFrame& operator = (const CMainFrame&);   // Disable assignment operator

    LRESULT AddCombo();
    BOOL ChooseColor(UINT color);
    void SetMenuBarColors(COLORREF hot1, COLORREF hot2, COLORREF pressed1, COLORREF pressed2, COLORREF outline, COLORREF text);
    void SetReBarColors(COLORREF backGround1, COLORREF backGround2, COLORREF band1, COLORREF band2);
    void SetStatusBarColors(COLORREF bkgnd1, COLORREF bkgnd2, COLORREF text);
    void SetToolBarColors(COLORREF hot1, COLORREF hot2, COLORREF pressed1, COLORREF pressed2, COLORREF outline);
    void ShowArrows(bool isShown);
    void ShowCards(bool isShown);

    // Command handlers
    BOOL OnFileExit();
    BOOL OnUseThemes();
    BOOL OnBandColors();
    BOOL OnFlatStyle();
    BOOL OnLeftBands();
    BOOL OnLockMenuBar();
    BOOL OnRoundBorders();
    BOOL OnShortBands();
    BOOL OnUseLines();
    BOOL OnViewArrows();
    BOOL OnViewCards();

    // Message handlers
    LRESULT OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam);

    // Member variables
    CView m_view;
    CToolBar m_arrows;
    CToolBar m_cards;
    CMyCombo m_comboBoxEx;
    std::vector<UINT> m_bandIDs;
    std::vector<UINT> m_bandStyles;
    std::vector<UINT> m_bandSizes;

    DWORD m_color;
    bool m_useThemes;
    bool m_useBandColors;
    bool m_useFlatStyle;
    bool m_keepBandsLeft;
    bool m_lockMenuBand;
    bool m_useRoundBorders;
    bool m_useShortBands;
    bool m_useLines;
    bool m_showArrows;
    bool m_showCards;
    int  m_selectedItem;      // Currently selected ComboBoxEx item.
};


#endif // MAINFRM_H

