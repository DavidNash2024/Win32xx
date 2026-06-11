/////////////////////////////
// MainMDIfrm.h
//

#ifndef MAINMDIFRM_H
#define MAINMDIFRM_H

#include "AboutDialog.h"

/////////////////////////////////////
// Declaration of CMainMDIFrame class
//
class CMainMDIFrame : public CMDIFrame
{
public:
    CMainMDIFrame() = default;
    virtual ~CMainMDIFrame() override = default;
    virtual HWND Create(HWND parent = nullptr) override;


protected:
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual BOOL OnHelp() override;
    virtual void OnInitialUpdate() override;
    virtual void SetupMenuIcons() override;
    virtual void SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainMDIFrame(const CMainMDIFrame&) = delete;
    CMainMDIFrame& operator=(const CMainMDIFrame&) = delete;

    // Command handlers
    BOOL OnDefaultCommand(WPARAM wparam, LPARAM lparam);
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

    CAboutDialog m_aboutDialog;
};

#endif  //MAINMDIFRM_H

