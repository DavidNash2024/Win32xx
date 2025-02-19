/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "MyTab.h"

///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame that has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CFrame
{
public:
    CMainFrame() = default;
    virtual ~CMainFrame() override = default;
    virtual HWND Create(HWND parent = nullptr) override;

protected:
    // Virtual functions that override base class functions
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual void OnInitialUpdate() override;
    virtual void OnMenuUpdate(UINT id) override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual void SetupMenuIcons() override;
    virtual void SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    // COmmand handlers
    BOOL OnFileExit();
    BOOL OnNewClassesTab();
    BOOL OnNewDialogTab();
    BOOL OnNewFilesTab();
    BOOL OnNewTab();
    BOOL OnShowButtons();
    BOOL OnTabsAtTop();
    BOOL OnTabClose();
    BOOL OnTabFixedWidth();
    BOOL OnTabOwnerDraw();

    CMyTab m_view;
};

#endif // MAINFRM_H
