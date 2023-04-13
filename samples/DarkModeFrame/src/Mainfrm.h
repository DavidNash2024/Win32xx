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
    void DPIScaleMenuIcons();

protected:
    // Virtual functions that override base class functions
    virtual void    OnClose();
    virtual BOOL    OnCommand(WPARAM, LPARAM);
    virtual int     OnCreate(CREATESTRUCT& cs);
    LRESULT         OnDPIChanged(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual BOOL    OnHelp();
    virtual void    OnInitialUpdate();
    virtual LRESULT OnSysColorChange(UINT, WPARAM, LPARAM);
    virtual void    SetTheme();
    virtual void    SetupMenuIcons();
    virtual void    SetupToolBar();
    virtual LRESULT WndProc(UINT, WPARAM, LPARAM);

private:
    CMainFrame(const CMainFrame&);                // Disable copy construction
    CMainFrame& operator = (const CMainFrame&);   // Disable assignment operator

    // Command Handlers
    BOOL OnFileExit();
    BOOL OnFileOpen();
    BOOL OnFilePrint();
    BOOL OnFileSave();
    BOOL OnFilePreview();

    // Message handlers
    LRESULT OnPreviewClose();
    LRESULT OnPreviewPrint();
    LRESULT OnPreviewSetup();

    // Member variables
    CView m_view;
    CDarkPreview<CView> m_preview;
    CDarkAbout m_helpDialog;
    bool m_isToolbarShown;
    CImageList m_menuImages;
};

#endif //MAINFRM_H

