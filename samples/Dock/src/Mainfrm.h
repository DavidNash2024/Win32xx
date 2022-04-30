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
    virtual ~CMainFrame();
    virtual HWND Create(HWND parent = 0);

protected:
    // Virtual functions overriding base class functions
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
    // Command handlers
    BOOL On3DBorder();
    BOOL OnDockCloseAll();
    BOOL OnDockDefault();
    BOOL OnFileExit();
    BOOL OnPropResize();
    BOOL OnNoUndocking();
    BOOL OnNoResize();
    BOOL OnNoDockLR();
    BOOL OnNoDockClose();

    void LoadDefaultDockers();
    void SetDockStyles();

    // member variables
    CViewSimple m_view;
    bool m_useProportionalResize;
    bool m_use3DBorder;
    bool m_disableUndocking;
    bool m_disableResize;
    bool m_disableDockLR;
    bool m_disableDockClose;
};

#endif //MAINFRM_H
