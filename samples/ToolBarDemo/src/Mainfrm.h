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

protected:
    // Virtual functions that override base class functions
    virtual LRESULT OnBeginAdjust(LPNMTOOLBAR pNMTB);
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual BOOL    OnViewToolBar();
    virtual void    SetupMenuIcons();
    virtual void    SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMainFrame(const CMainFrame&);                // Disable copy construction
    CMainFrame& operator = (const CMainFrame&);   // Disable assignment operator

    void    ResizeToolbarBand() const;
    void    SaveTBDefault();

    // Command handlers
    BOOL    OnFileExit();
    BOOL    OnTBBigIcons();
    BOOL    OnTBCustomize();
    BOOL    OnTBDefault();

    // Message handlers
    LRESULT OnCustHelp(LPNMHDR pNMHDR);
    LRESULT OnEndAdjust(LPNMHDR pNMHDR);
    LRESULT OnGetButtonInfo(LPNMTOOLBAR pNMTB);
    LRESULT OnQueryDelete(LPNMTOOLBAR pNMTB);
    LRESULT OnQueryInsert(LPNMTOOLBAR pNMTB);
    LRESULT OnReset(LPNMTOOLBAR pNMTB);
    LRESULT OnToolBarChange(LPNMTOOLBAR pNMTB);

    // Member variables
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

