/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H


#include "MyTabbedMDI.h"

enum DockIDs
{
    ID_DOCK_CLASSES1 = 1,
    ID_DOCK_CLASSES2 = 2,
    ID_DOCK_FILES1 = 3,
    ID_DOCK_FILES2 = 4,
    ID_DOCK_OUTPUT1 = 5,
    ID_DOCK_OUTPUT2 = 6,
    ID_DOCK_TEXT1 = 7,
    ID_DOCK_TEXT2 = 8,
    ID_DOCK_BROWSER = 9,
    ID_DOCK_RECT = 10,
    ID_DOCK_DIALOG = 11
};


///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame which has a menubar, toolbar,
// statusbar and view window. It supports docking.
class CMainFrame : public CDockFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();
    virtual HWND Create(HWND parent = 0);
    void HideSingleContainerTab(BOOL hideSingle);
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
    void LoadDefaultDockers();
    void LoadDefaultMDIs();
    void SetContainerTabsAtTop(BOOL atTop);
    void SetMDITabsAtTop(BOOL atTop);

protected:
    virtual CDocker* NewDockerFromID(int dockID);
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual LRESULT OnDockActivated(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void    OnInitialUpdate();
    virtual LRESULT OnInitMenuPopup(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void    OnMenuUpdate(UINT id);
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual BOOL    SaveRegistrySettings();
    virtual void    SetupMenuIcons();
    virtual void    SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMyTabbedMDI m_myTabbedMDI;
    BOOL m_isContainerTabsAtTop;
    BOOL m_isHideSingleTab;
    BOOL m_isMDITabsAtTop;
    CDocker* m_pActiveDocker;
};

#endif //MAINFRM_H

