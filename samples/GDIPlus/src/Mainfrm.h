/////////////////////////////
// Mainfrm.h
//

#pragma once

#include "AboutDialog.h"
#include "GDIPlusView.h"

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

    BOOL OnFileExit();

protected:
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual BOOL OnHelp() override;
    virtual void OnInitialUpdate() override;
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam) override;
    virtual void SetupMenuIcons() override;
    virtual void SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    CAboutDialog m_aboutDialog;
    CGDIPlusView m_view;
};

