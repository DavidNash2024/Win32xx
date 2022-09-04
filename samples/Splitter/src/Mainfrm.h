/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "SplitterPanes.h"

// The docker identifiers (dock IDs)
const int ID_DOCK_LIST = 1;
const int ID_DOCK_TREE = 2;
const int ID_DOCK_TEXT = 3;


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
    // Virtual functions that override base class functions
    virtual CDocker* NewDockerFromID(int id);
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual void OnInitialUpdate();
    virtual BOOL SaveRegistrySettings();
    virtual void SetupMenuIcons();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMainFrame(const CMainFrame&);                // Disable copy construction
    CMainFrame& operator = (const CMainFrame&);   // Disable assignment operator

    // Command handlers
    BOOL OnFileExit();
    BOOL OnViewList();
    BOOL OnViewText();

    void LoadDefaultWindowPanes();

    // Member variables
    CViewSimple m_mainView;
    CDockText* m_pDockText;
    CDockTree* m_pDockTree;
    CDockList* m_pDockList;
};


#endif //MAINFRM_H

