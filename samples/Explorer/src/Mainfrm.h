/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H


#include "SplitterPanes.h"

#ifndef BTNS_WHOLEDROPDOWN
  #define BTNS_WHOLEDROPDOWN  0x0080
#endif


///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame which has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();
    virtual HWND Create(HWND parent = 0);
    void DoPopupMenu();
    CMyListView& GetListView() const {return static_cast<CMyListView&>(m_rightPane.GetView());}
    CMyTreeView& GetTreeView() const {return static_cast<CMyTreeView&>(m_pLeftPane->GetView());}
    bool GetShowHidden() {return m_showHidden;}
    void LoadDefaultWindowPanes();
    void LoadListViewRegistrySettings();
    BOOL OnFileExit();
    BOOL OnViewLargeIcon();
    BOOL OnViewList();
    BOOL OnViewReport();
    BOOL OnViewSmallIcon();
    BOOL OnViewMenu();
    BOOL OnShowHidden();

protected:
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual BOOL SaveRegistrySettings();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CLeftPane* m_pLeftPane;
    CRightPane m_rightPane;
    bool m_showHidden;
};

#endif  //MAINFRM_H

