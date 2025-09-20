/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "MainContainer.h"


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
    virtual ~CMainFrame() = default;
    virtual HWND Create(HWND parent = nullptr) override;

protected:
    virtual DockPtr NewDockerFromID(int id) override;
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual void OnInitialUpdate() override;
    virtual void OnMenuUpdate(UINT id) override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual void RecalcDockLayout() override;
    virtual BOOL SaveRegistrySettings() override;
    virtual void SetupMenuIcons() override;
    virtual void SetupToolBar() override;
    virtual void UndockContainerGroup() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    // Command handlers
    BOOL OnContainerTabsAtTop();
    BOOL OnDockDefault();
    BOOL OnFileExit();
    BOOL OnHideSingleTab();

    // Message handlers
    LRESULT OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam);

    void HideSingleContainerTab(bool hideSingle);
    void LoadDefaultDockers();
    void SetContainerTabsAtTop(bool isAtTop);

    // Member variables
    CMainContainer m_view;
    bool m_isContainerTabsAtTop;
    bool m_hideSingleTab;
};

#endif //MAINFRM_H

