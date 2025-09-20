/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H


#include "Dockers.h"

// The docker identifiers (dock IDs)
const int ID_DOCK_CLASSES1 = 1;
const int ID_DOCK_CLASSES2 = 2;
const int ID_DOCK_FILES1 = 3;
const int ID_DOCK_FILES2 = 4;
const int ID_DOCK_SIMPLE1 = 5;
const int ID_DOCK_SIMPLE2 = 6;
const int ID_DOCK_TEXT1 = 7;
const int ID_DOCK_TEXT2 = 8;


///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame which has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CDockFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame() override = default;
    virtual HWND Create(HWND parent = 0) override;

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
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    // Command handlers
    BOOL On3DBorder();
    BOOL OnDockCloseAll();
    BOOL OnDockDefault();
    BOOL OnFileExit();
    BOOL OnNoUndocking();
    BOOL OnNoResize();
    BOOL OnNoDockLR();
    BOOL OnNoDockClose();
    BOOL OnNoDockCaption();
    BOOL OnStylesDefault();

    // Message handles
    LRESULT OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam);

    void LoadDefaultDockers();
    void SetDockStyles();
    void SetDockStylesToDefault();

    // member variables
    CViewSimple m_view;
    bool m_use3DBorder;
    bool m_disableUndocking;
    bool m_disableResize;
    bool m_disableDockLR;
    bool m_disableDockClose;
    bool m_disableDockCaption;
};

#endif //MAINFRM_H
