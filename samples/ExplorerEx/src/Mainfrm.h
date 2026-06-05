/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H


#include "AboutDialog.h"
#include "LeftPane.h"
#include "RightPane.h"


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

    // Accessors
    CMyListView& GetListView() const { return static_cast<CMyListView&>(m_rightPane.GetView()); }
    CMyTreeView& GetTreeView() const { return static_cast<CMyTreeView&>(m_pLeftPane->GetView()); }
    bool GetShowHidden() { return m_showHidden; }

protected:
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual BOOL    OnHelp() override;
    virtual void    OnInitialUpdate() override;
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam) override;
    virtual BOOL    SaveRegistrySettings() override;
    virtual void    SetupMenuIcons() override;
    virtual void    SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    // Command handlers
    BOOL OnFileExit();
    BOOL OnViewLargeIcon();
    BOOL OnViewList();
    BOOL OnViewReport();
    BOOL OnViewSmallIcon();
    BOOL OnViewMenu();
    BOOL OnShowHidden();

    // Message handlers
    LRESULT OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam);

    void DoPopupMenu();
    void LoadDefaultWindowPanes();
    void LoadListViewRegistrySettings();

    // Member variables
    CAboutDialog m_aboutDialog;
    CDocker* m_pLeftPane;
    CRightPane m_rightPane;
    bool m_showHidden;
};

#endif  //MAINFRM_H

