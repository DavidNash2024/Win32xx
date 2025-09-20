/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"


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
    void SetImageListsForToolBars();

protected:
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual LRESULT OnDpiChanged(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual void    OnInitialUpdate() override;
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam) override;
    virtual BOOL    OnViewToolBar() override;
    virtual void    SetupMenuIcons() override;
    virtual void    SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    void    ResizeToolbarBand() const;
    void    SaveTBDefault();

    // Command handlers
    BOOL    OnFileExit();
    BOOL    OnTBBigIcons();
    BOOL    OnTBCustomize();
    BOOL    OnTBDefault();

    // Message handlers
    LRESULT OnBeginAdjust(LPNMTOOLBAR pNMTB);
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
    std::vector<TBBUTTON>   m_defaultButtons;
    std::vector<TBBUTTON>   m_resetButtons;
    bool    m_useBigIcons;
};


#endif //MAINFRM_H

