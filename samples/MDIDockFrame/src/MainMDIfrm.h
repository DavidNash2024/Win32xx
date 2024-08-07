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
    CMainMDIFrame();
    virtual ~CMainMDIFrame();
    virtual HWND Create(HWND parent = NULL);

protected:
    // Virtual functions that override base class functions
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual void SetupMenuIcons();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMainMDIFrame(const CMainMDIFrame&);                // Disable copy construction
    CMainMDIFrame& operator = (const CMainMDIFrame&);   // Disable assignment operator

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

