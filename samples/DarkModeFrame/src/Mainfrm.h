/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "DarkAbout.h"
#include "DarkPreview.h"
#include "View.h"


///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame which has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    CDoc& GetDoc()      { return m_view.GetDoc(); }
    void SetDarkTheme();

    // Virtual functions that override public base class functions
    virtual ~CMainFrame();
    virtual HWND Create(HWND parent = 0);

protected:
    // Virtual functions that override base class functions
    virtual void    OnClose();
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual BOOL    OnHelp();
    virtual void    OnInitialUpdate();
    virtual LRESULT OnSysColorChange(UINT msg, WPARAM, LPARAM);
    virtual void    SetTheme();
    virtual void    SetupMenuIcons();
    virtual void    SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMainFrame(const CMainFrame&);                // Disable copy construction
    CMainFrame& operator = (const CMainFrame&);   // Disable assignment operator

    // Command Handlers
    BOOL    OnFileExit();
    BOOL    OnFileOpen();
    BOOL    OnFilePrint();
    BOOL    OnFileSave();
    BOOL    OnFilePreview();

    // Message handlers
    LRESULT    OnPreviewClose();
    LRESULT    OnPreviewPrint();
    LRESULT    OnPreviewSetup();

    // Member variables
    CView m_view;
    CDarkPreview<CView> m_preview;
    CDarkAbout m_helpDialog;
    bool m_isToolbarShown;
};

#endif //MAINFRM_H

