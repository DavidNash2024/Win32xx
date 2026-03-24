/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "AboutDialog.h"
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
    virtual ~CMainFrame() override = default;
    virtual HWND Create(HWND parent = nullptr) override;

protected:
    virtual BOOL    LoadRegistrySettings(LPCWSTR keyName) override;
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual LRESULT OnDpiChanged(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual BOOL    OnHelp() override;
    virtual void    OnInitialUpdate() override;
    virtual void    OnMenuUpdate(UINT id) override;
    virtual BOOL    SaveRegistrySettings() override;
    virtual void    SetupMenuIcons() override;
    virtual void    SetupToolBar() override;
    virtual void    SetTheme() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    LRESULT AddCombo();
    BOOL ChooseColor(UINT color);
    void SetCaptionColor(COLORREF color);
    void SetCaptionTextColor(COLORREF color);
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
    CAboutDialog m_aboutDialog;
    CView m_view;
    CToolBar m_arrows;
    CToolBar m_cards;
    CMyCombo m_comboBoxEx;

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

