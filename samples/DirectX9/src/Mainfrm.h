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

    CDXView& GetDXView() const { return m_dxView; }

    BOOL OnFileExit();

protected:
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    mutable CDXView m_dxView;
};

#endif //MAINFRM_H

