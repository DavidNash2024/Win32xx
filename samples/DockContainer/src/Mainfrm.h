/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H


// The docker identifiers (dock IDs)
const int ID_DOCK_CLASSES1 = 1;
const int ID_DOCK_CLASSES2 = 2;
const int ID_DOCK_FILES1 = 3;
const int ID_DOCK_FILES2 = 4;
const int ID_DOCK_TEXT1 = 5;
const int ID_DOCK_OUTPUT1 = 6;
const int ID_DOCK_TEXT2 = 7;
const int ID_DOCK_OUTPUT2 = 8;


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
    void LoadDefaultDockers();
    BOOL OnContainerTabsAtTop();
    BOOL OnDockCloseAll();
    BOOL OnDockDefault();
    BOOL OnFileExit();
    BOOL OnHideSingleTab();

protected:
    virtual CDocker* NewDockerFromID(int id);
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual void OnMenuUpdate(UINT id);
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual BOOL SaveRegistrySettings();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    void HideSingleContainerTab(bool hideSingle);
    void SetContainerTabsAtTop(bool isAtTop);

    CDockContainer m_view;
    bool m_isContainerTabsAtTop;
    bool m_hideSingleTab;

};

#endif //MAINFRM_H

