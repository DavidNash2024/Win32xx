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

    void DpiScaleToolBar();
    void UpdateToolbar();

protected:
    // Virtual functions that override base class functions
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual LRESULT OnDpiChanged(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual void OnMenuUpdate(UINT id) override;
    virtual void SetupMenuIcons() override;
    virtual void SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    // Message handlers
    LRESULT OnFileLoaded(LPCWSTR fileName);
    LRESULT OnWindowPosChanged(UINT msg, WPARAM wparam, LPARAM lparam);

    // Command handlers
    BOOL OnFileExit();
    BOOL OnFileMRU(WPARAM wparam);
    BOOL OnFileNew();
    BOOL OnFileOpen();
    BOOL OnFileSaveAs();

    // Member variables
    CView m_view;
    CRect m_viewRect;
    CPoint m_scrollPos;
    bool m_isDPIChanging;
};

#endif //MAINFRM_H

