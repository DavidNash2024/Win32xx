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
    // Virtual functions overriding base class functions.
    virtual CDocker* NewDockerFromID(int id);
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual void OnMenuUpdate(UINT id);
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void RecalcDockLayout();
    virtual BOOL SaveRegistrySettings();
    virtual void SetupMenuIcons();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMainFrame(const CMainFrame&);               // Disable copy construction
    CMainFrame& operator=(const CMainFrame&);    // Disable assignment operator

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
