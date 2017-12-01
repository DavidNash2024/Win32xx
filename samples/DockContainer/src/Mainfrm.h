/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H


enum DockIDs
{
    ID_DOCK_CLASSES1 = 1,
    ID_DOCK_CLASSES2 = 2,
    ID_DOCK_FILES1 = 3,
    ID_DOCK_FILES2 = 4,
    ID_DOCK_TEXT1 = 5,
    ID_DOCK_OUTPUT1 = 6,
    ID_DOCK_TEXT2 = 7,
    ID_DOCK_OUTPUT2 = 8,
};

// Declaration of the CMainFrame class
class CMainFrame : public CDockFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();
    void LoadDefaultDockers();
    BOOL OnContainerTabsAtTop();
    BOOL OnDockCloseAll();
    BOOL OnDockDefault();
    BOOL OnFileExit();
    BOOL OnHideSingleTab();
    
protected:
    virtual CDocker* NewDockerFromID(int nID);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual void OnMenuUpdate(UINT nID);
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual BOOL SaveRegistrySettings();
    virtual void SetupToolBar();

private:
    void HideSingleContainerTab(BOOL HideSingle);
    void SetContainerTabsAtTop(BOOL bTop);

    CDockContainer m_View;
    BOOL m_IsContainerTabsAtTop;
    BOOL m_IsHideSingleTab;

};

#endif //MAINFRM_H

