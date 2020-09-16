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

    CDoc& GetDoc() {return m_view.GetDoc(); }

    void    OnFileExit();
    void    OnFileOpen();
    void    OnFilePreview();
    void    OnFilePrint();
    void    OnFileSave();
    void    OnPreviewClose();
    void    OnPreviewPrint();
    void    OnPreviewSetup();

    CString ItoT(int i);
    int     TtoI(LPCTSTR string);

    CString GetINIPath();
    void SerializeINI(BOOL isStoring);

protected:
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    OnClose();
    virtual void    OnInitialUpdate();
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual void    SetupMenuIcons();
    virtual void    SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView m_view;
    CPrintPreview<CView> m_preview;
};

#endif //MAINFRM_H

