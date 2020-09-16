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

    CDoc& GetDoc()      { return m_view.GetDoc(); }

    void    OnFileExit();
    void    OnFileOpen();
    void    OnFilePrint();
    void    OnFileSave();
    void    OnFilePreview();
    void    OnPreviewClose();
    void    OnPreviewPrint();
    void    OnPreviewSetup();

protected:
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    OnInitialUpdate();
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual void    SetupMenuIcons();
    virtual void    SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView m_view;
    CPrintPreview<CView> m_preview;
};

#endif //MAINFRM_H

