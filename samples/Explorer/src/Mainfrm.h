///////////////////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINFRM_H
#define MAINFRM_H


#include "SplitterPanes.h"

#ifndef BTNS_WHOLEDROPDOWN
  #define BTNS_WHOLEDROPDOWN  0x0080
#endif

class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();
    void DoPopupMenu();
    CMyListView& GetListView() const {return static_cast<CMyListView&>(m_rightPane.GetView());}
    CMyTreeView& GetTreeView() const {return static_cast<CMyTreeView&>(m_pLeftPane->GetView());}
    BOOL GetShowHidden() {return m_showHidden;}
    BOOL OnFileExit();  
    BOOL OnViewLargeIcon();
    BOOL OnViewList();
    BOOL OnViewReport();
    BOOL OnViewSmallIcon();
    BOOL OnViewMenu();
    BOOL OnShowHidden();

protected:
    virtual void OnInitialUpdate();
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CLeftPane* m_pLeftPane;
    CRightPane m_rightPane;
    BOOL m_showHidden;
};

#endif  //MAINFRM_H

