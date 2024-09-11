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
    virtual ~CMainFrame() override;
    virtual HWND Create(HWND parent = nullptr) override;
    void DpiScaleReBar();

protected:
    // Virtual functions that override base class functions
    virtual CRect   GetViewRect() const override;
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual LRESULT OnDpiChanged(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual void    OnInitialUpdate() override;
    virtual void    SetupMenuIcons() override;
    virtual void    SetupToolBar() override;
    virtual void    RecalcLayout() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wmaram, LPARAM lmaram) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    // Command handlers
    BOOL OnFileExit();
    void OnBottom();
    void OnLeft();
    void OnRight();
    void OnTop();

    void SetReBarPos();
    void SetWrapState(CToolBar* pToolBar, BOOL isWrapped);

    // Member variables
    CReBar m_reBar;
    CToolBar m_toolBar;
    CImageList m_toolBarImages;
    CView m_view;
};

#endif //MAINFRM_H

