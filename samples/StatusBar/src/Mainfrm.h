/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"
#include "Hyperlink.h"


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
    virtual void    DrawStatusBar(LPDRAWITEMSTRUCT pDrawItem) const;
    virtual BOOL    DrawStatusBarBkgnd(CDC& dc) const;
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    OnInitialUpdate();
    virtual void    SetStatusIndicators();
    virtual void    SetStatusParts();
    virtual void    SetupMenuIcons();
    virtual void    SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMainFrame(const CMainFrame&);                // Disable copy construction
    CMainFrame& operator = (const CMainFrame&);   // Disable assignment operator

    int AdjustForDPI(int value);
    int GetTextPartWidth(LPCTSTR text) const;

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
