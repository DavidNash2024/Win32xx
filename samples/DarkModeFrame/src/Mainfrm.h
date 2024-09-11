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
    void SetDarkMode(bool isDarkMode);
    void SetDarkTheme();

    // Virtual functions that override public base class functions
    virtual ~CMainFrame() override;
    virtual HWND Create(HWND parent = nullptr) override;

protected:
    // Virtual functions that override base class functions
    virtual void    OnClose() override;
    virtual BOOL    OnCommand(WPARAM, LPARAM) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual BOOL    OnHelp() override;
    virtual void    OnInitialUpdate() override;
    virtual LRESULT OnSysColorChange(UINT, WPARAM, LPARAM) override;
    virtual void    SetTheme() override;
    virtual void    SetupMenuIcons() override;
    virtual void    SetupToolBar() override;
    virtual LRESULT WndProc(UINT, WPARAM, LPARAM) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

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
};

#endif //MAINFRM_H

