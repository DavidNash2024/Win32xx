/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H


#include "MyTabbedMDI.h"

// The docker identifiers (dock IDs)
const int ID_DOCK_CLASSES1 = 1;
const int ID_DOCK_CLASSES2 = 2;
const int ID_DOCK_FILES1 = 3;
const int ID_DOCK_FILES2 = 4;
const int ID_DOCK_OUTPUT1 = 5;
const int ID_DOCK_OUTPUT2 = 6;
const int ID_DOCK_TEXT1 = 7;
const int ID_DOCK_TEXT2 = 8;
const int ID_DOCK_BROWSER = 9;
const int ID_DOCK_RECT = 10;
const int ID_DOCK_DIALOG = 11;



///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame which has a menubar, toolbar,
// statusbar and view window. It supports docking.
class CMainFrame : public CDockFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame() override = default;
    virtual HWND Create(HWND parent = nullptr) override;

protected:
    virtual DockPtr NewDockerFromID(int dockID) override;
    virtual void    OnClose() override;
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual LRESULT OnDockActivated(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual void    OnInitialUpdate() override;
    virtual LRESULT OnInitMenuPopup(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual void    OnMenuUpdate(UINT id) override;
    virtual void    PreCreate(CREATESTRUCT& cs) override;
    virtual void    RecalcDockLayout() override;
    virtual BOOL    SaveRegistrySettings() override;
    virtual void    SetupMenuIcons() override;
    virtual void    SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    // Command handlers
    BOOL OnCloseMDIs();
    BOOL OnCloseDockers();
    BOOL OnContainerTabsAtTop();
    BOOL OnDefaultLayout();
    BOOL OnFileNew();
    BOOL OnFileExit();
    BOOL OnFileNewBrowser();
    BOOL OnFileNewRect();
    BOOL OnFileNewList();
    BOOL OnFileNewText();
    BOOL OnFileNewTree();
    BOOL OnHideSingleTab();
    BOOL OnMDITabsAtTop();

    // Message handlers
    LRESULT OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam);

    void HideSingleContainerTab(bool hideSingle);
    void LoadDefaultDockers();
    void LoadDefaultMDIs();
    void SetContainerTabsAtTop(bool atTop);
    void SetMDITabsAtTop(bool atTop);

    // Member variables
    CMyTabbedMDI m_myTabbedMDI;
    bool m_isContainerTabsAtTop;
    bool m_isHideSingleTab;
    bool m_isMDITabsAtTop;
    CDocker* m_pActiveDocker;
};

#endif //MAINFRM_H

