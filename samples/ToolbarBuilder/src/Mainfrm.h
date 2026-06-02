/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"
#include "AboutDialog.h"
#include "NewDialog.h"


///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame that has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame() override = default;
    virtual HWND Create(HWND parent = nullptr) override;

    void UpdateToolbar();

protected:
    virtual void    OnClose() override;
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual void    OnInitialUpdate() override;
    virtual BOOL    OnHelp() override;
    virtual void    OnMenuUpdate(UINT) override;
    virtual void    SetupMenuIcons() override;
    virtual void    SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    // Command Handlers
    bool OnChooseBackground();
    bool OnChooseMask();
    bool OnFileExit();
    bool OnFileNewToolbar();
    bool OnFileOpen();
    bool OnFileSave();
    bool OnImageAdd();
    bool OnImageDelete();
    bool OnImageSwap();
    bool OnViewUseMask();

    // Message handlers
    LRESULT OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam);

    // Member variables
    CAboutDialog     m_aboutDialog;
    CView m_view;
    CNewDialog m_newDialog;
    CColorDialog m_colorDialog;
    CString m_fileName;
};

#endif //MAINFRM_H

