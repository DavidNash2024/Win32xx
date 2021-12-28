/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"
#include "MyCombo.h"


///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame which has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();
    virtual HWND Create(HWND parent = 0);
    void AddCombo();
    BOOL ChooseColor(UINT color);
    DWORD GetRegDwordFromOpenKey(HKEY hKey, LPCTSTR pName);
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
    void SetReBarColors(COLORREF backGround1, COLORREF backGround2, COLORREF band1, COLORREF band2);
    void ShowArrows(BOOL isShown);
    void ShowCards(BOOL isShown);

protected:
    virtual BOOL LoadRegistrySettings(LPCTSTR keyName);
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual void OnMenuUpdate(UINT id);
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual BOOL SaveRegistrySettings();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView m_view;
    CToolBar m_arrows;
    CToolBar m_cards;
    CMyCombo m_comboBoxEx;
    std::vector<UINT> m_bandIDs;
    std::vector<UINT> m_bandStyles;
    std::vector<UINT> m_bandSizes;
    CImageList m_arrowImages;
    CImageList m_cardImages;

    UINT m_color;
    BOOL m_useThemes;
    BOOL m_useBandColors;
    BOOL m_useFlatStyle;
    BOOL m_keepBandsLeft;
    BOOL m_lockMenuBand;
    BOOL m_useRoundBorders;
    BOOL m_useShortBands;
    BOOL m_useLines;
    BOOL m_showArrows;
    BOOL m_showCards;
};


#endif //MAINFRM_H

