/////////////////////////////
// MainMDIfrm.h
//

#ifndef MAINMDIFRM_H
#define MAINMDIFRM_H


/////////////////////////////////////
// Declaration of CMainMDIFrame class
//
class CMainMDIFrame : public CMDIFrame
{
public:
    CMainMDIFrame();
    virtual ~CMainMDIFrame();
    virtual HWND Create(HWND parent = 0);
    BOOL OnFileClose();
    BOOL OnFileExit();
    BOOL OnFileNew();
    BOOL OnMDICascade();
    BOOL OnMDICloseAll();
    BOOL OnMDIIconArrange();
    BOOL OnMDITile();

protected:
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);
};

#endif  //MAINMDIFRM_H

