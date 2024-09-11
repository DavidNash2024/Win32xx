/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"
#include "Hyperlink.h"


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

protected:
    // Virtual functions that override base class functions.
    virtual void    DrawStatusBar(LPDRAWITEMSTRUCT pDrawItem) override;
    virtual BOOL    DrawStatusBarBkgnd(CDC& dc, CStatusBar& statusbar) override;
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual void    OnInitialUpdate() override;
    virtual void    SetStatusIndicators() override;
    virtual void    SetStatusParts() override;
    virtual void    SetupMenuIcons() override;
    virtual void    SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    int GetTextPartWidth(LPCWSTR text) const;

    // Command handlers
    BOOL OnFileExit();

    // Message handlers
    void OnTimer();

    // Member variables
    CView m_view;
    CHyperlink m_hyperlink;
    CProgressBar m_progressBar;
    CString m_colored;
    CString m_cap;
    CString m_num;
    CString m_ovr;
    CString m_scrl;
};


#endif //MAINFRM_H
