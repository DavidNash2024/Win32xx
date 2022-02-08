/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"


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
    BOOL    OnFileExit();
    BOOL    OnTBBigIcons();
    BOOL    OnTBCustomize();
    BOOL    OnTBDefault();
    void    ResizeToolbarBand() const;
    void    SaveTBDefault();

protected:
    virtual LRESULT OnBeginAdjust(LPNMTOOLBAR pNMTB);
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int OnCreate(CREATESTRUCT& cs);
    virtual LRESULT OnCustHelp(LPNMHDR pNMHDR);
    virtual LRESULT OnEndAdjust(LPNMHDR pNMHDR);
    virtual LRESULT OnGetButtonInfo(LPNMTOOLBAR pNMTB);
    virtual void OnInitialUpdate();
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnQueryDelete(LPNMTOOLBAR pNMTB);
    virtual LRESULT OnQueryInsert(LPNMTOOLBAR pNMTB);
    virtual LRESULT OnReset(LPNMTOOLBAR pNMTB);
    virtual LRESULT OnToolBarChange(LPNMTOOLBAR pNMTB);
    virtual BOOL    OnViewToolBar();
    virtual void    SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView m_view;
    CToolBar m_arrows;
    CToolBar m_cards;
    CImageList m_arrowImages;
    CImageList m_cardImages;
    std::vector<TBBUTTON>   m_defaultButtons;
    std::vector<TBBUTTON>   m_resetButtons;
    bool    m_useBigIcons;
};


#endif //MAINFRM_H

