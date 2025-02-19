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
    CDoc& GetDoc() {return m_view.GetDoc(); }

    // Virtual functions that override public base class functions
    virtual ~CMainFrame() override = default;
    virtual HWND Create(HWND parent = nullptr) override;

protected:
    // Virtual functions that override base class functions
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual void    OnClose() override;
    virtual void    OnInitialUpdate() override;
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam) override;
    virtual void    SetupMenuIcons() override;
    virtual void    SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    // Command handlers
    BOOL OnFileExit();
    BOOL OnFileOpen();
    BOOL OnFilePreview();
    BOOL OnFilePrint();
    BOOL OnFileSave();

    // Message handlers
    LRESULT OnPreviewClose();
    LRESULT OnPreviewPrint();
    LRESULT OnPreviewSetup();

    CString GetINIPath();
    CString ItoW(int i);
    int     WtoI(LPCWSTR string);
    void    SerializeINI(BOOL isStoring);

    // Member variables
    CView m_view;
    CPrintPreview<CView> m_preview;
    bool m_isToolbarShown;
};

#endif //MAINFRM_H

