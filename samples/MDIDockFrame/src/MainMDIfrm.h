/////////////////////////////
// MainMDIfrm.h
//

#ifndef MAINMDIFRM_H
#define MAINMDIFRM_H

/////////////////////////////////////////
// Declaration of the CMainMDIFrame class
//
class CMainMDIFrame : public CMDIDockFrame
{
public:
    CMainMDIFrame();
    virtual ~CMainMDIFrame();

    BOOL OnFileClose();
    BOOL OnFileExit();
    BOOL OnFileNewDocker();
    BOOL OnFileNewMDI();
    BOOL OnFileOpen();
    BOOL OnFileSave();
    BOOL OnFilePrint();
    BOOL OnMDICascade();
    BOOL OnMDICloseAll();
    BOOL OnMDIIconArrange();
    BOOL OnMDITile();

protected:
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual void SetupMenuIcons();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

};

#endif  //MAINMDIFRM_H

