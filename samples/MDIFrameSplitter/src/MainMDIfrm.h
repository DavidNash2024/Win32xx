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
    virtual HWND Create(HWND parent = NULL);


protected:
    // Virtual functions that override base class functions.
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual void SetupMenuIcons();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMainMDIFrame(const CMainMDIFrame&);               // Disable copy construction
    CMainMDIFrame& operator=(const CMainMDIFrame&);    // Disable assignment operator

    // Command handlers
    BOOL OnFileClose();
    BOOL OnFileExit();
    BOOL OnFileNew();
    BOOL OnMDICascade();
    BOOL OnMDICloseAll();
    BOOL OnMDIIconArrange();
    BOOL OnMDITile();

    // Message handlers
    LRESULT OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
};

#endif  //MAINMDIFRM_H

