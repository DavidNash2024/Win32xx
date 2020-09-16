/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H


#include "Dockers.h"

enum DockIDs
{
    ID_DOCK_CLASSES1 = 1,
    ID_DOCK_CLASSES2 = 2,
    ID_DOCK_FILES1 = 3,
    ID_DOCK_FILES2 = 4,
    ID_DOCK_SIMPLE1 = 5,
    ID_DOCK_SIMPLE2 = 6,
    ID_DOCK_TEXT1 = 7,
    ID_DOCK_TEXT2 = 8
};


///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame which has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CDockFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();

    void LoadDefaultDockers();
    BOOL On3DBorder();
    BOOL OnDockCloseAll();
    BOOL OnDockDefault();
    BOOL OnDynamicResize();
    BOOL OnFileExit();
    BOOL OnPropResize();
    BOOL OnNoUndocking();
    BOOL OnNoResize();
    BOOL OnNoDockLR();
    BOOL OnNoDockClose();
    void SetDockStyles();

protected:
    virtual CDocker* NewDockerFromID(int id);
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual void OnMenuUpdate(UINT id);
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual BOOL SaveRegistrySettings();
    virtual void SetupToolBar();

private:
    CViewSimple m_view;
    BOOL m_useProportionalResize;
    BOOL m_use3DBorder;
    BOOL m_disableUndocking;
    BOOL m_disableResize;
    BOOL m_disableDockLR;
    BOOL m_disableDockClose;
    BOOL m_useDynamicResize;
};

#endif //MAINFRM_H
