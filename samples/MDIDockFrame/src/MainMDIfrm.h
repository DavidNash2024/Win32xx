/////////////////////////////
// MainMDIfrm.h
//

#ifndef MAINMDIFRM_H
#define MAINMDIFRM_H


///////////////////////////////////////////////////////////////
// CMainMDIFrame manages the application's main window.
// The main window is a MDI frame that has a menubar, toolbar,
// statusbar and view window. It supports docking.
class CMainMDIFrame : public CMDIDockFrame
{
public:
    CMainMDIFrame() = default;
    virtual ~CMainMDIFrame() override = default;
    virtual HWND Create(HWND parent = nullptr) override;

protected:
    // Virtual functions that override base class functions
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual void OnInitialUpdate() override;
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam) override;
    virtual void SetupMenuIcons() override;
    virtual void SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainMDIFrame(const CMainMDIFrame&) = delete;
    CMainMDIFrame& operator = (const CMainMDIFrame&) = delete;

    // Command handlers
    BOOL OnFileMDIClose();
    BOOL OnFileExit();
    BOOL OnFileNew();
    BOOL OnFileNewDocker();
    BOOL OnFileNewMDI();
    BOOL OnFileOpen();
    BOOL OnFileSave();
    BOOL OnFilePrint();
    BOOL OnMDICascade();
    BOOL OnMDICloseAll();
    BOOL OnMDIIconArrange();
    BOOL OnMDITile();
};

#endif  //MAINMDIFRM_H

