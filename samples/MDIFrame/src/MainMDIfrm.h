/////////////////////////////
// MainMDIfrm.h
//

#ifndef MAINMDIFRM_H
#define MAINMDIFRM_H


///////////////////////////////////////////////////////////////
// CMainMDIFrame manages the application's main window.
// The main window is a MDI frame which has a menubar, toolbar,
// statusbar and view window.
class CMainMDIFrame : public CMDIFrame
{
public:
    CMainMDIFrame();
    virtual ~CMainMDIFrame();
    virtual HWND Create(HWND parent = 0);

protected:
    // Virtual functions that override base class functions
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMainMDIFrame(const CMainMDIFrame&);                // Disable copy construction
    CMainMDIFrame& operator = (const CMainMDIFrame&);   // Disable assignment operator

    BOOL OnFileClose();
    BOOL OnFileExit();
    BOOL OnFileNew();
    BOOL OnFileOpen();
    BOOL OnFileSave();
    BOOL OnFilePrint();
    BOOL OnMDICascade();
    BOOL OnMDICloseAll();
    BOOL OnMDIIconArrange();
    BOOL OnMDITile();
};

#endif  //MAINMDIFRM_H

