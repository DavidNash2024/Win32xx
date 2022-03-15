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
    virtual ~CMainFrame();
    virtual HWND Create(HWND parent = 0);

protected:
    // Virtual functions that override base class functions
    virtual CDocker* NewDockerFromID(int dockID);
    virtual void    OnClose();
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

