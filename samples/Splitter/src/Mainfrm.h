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
// The main window is a frame that has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CDockFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame() override = default;
    virtual HWND Create(HWND parent = nullptr) override;

protected:
    // Virtual functions that override base class functions
    virtual DockPtr NewDockerFromID(int id) override;
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual void OnInitialUpdate() override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual BOOL SaveRegistrySettings() override;
    virtual void SetupMenuIcons() override;
    virtual void SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    // Command handlers
    BOOL OnFileExit();
    BOOL OnViewList();
    BOOL OnViewText();

    // Message handlers
    LRESULT OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam);

    void LoadDefaultWindowPanes();

    // Member variables
    CViewSimple m_mainView;
    CDockText* m_pDockText;
    CDockTree* m_pDockTree;
    CDockList* m_pDockList;
};


#endif //MAINFRM_H

