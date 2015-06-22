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
	CMyListView& GetListView() const {return static_cast<CMyListView&>(m_RightPane.GetView());}
	CMyTreeView& GetTreeView() const {return static_cast<CMyTreeView&>(m_pLeftPane->GetView());}
	BOOL GetShowHidden() {return m_ShowHidden;}
	BOOL OnFileExit();	
	BOOL OnViewLargeIcon();
	BOOL OnViewList();
	BOOL OnViewReport();
	BOOL OnViewSmallIcon();
	BOOL OnViewMenu();
	BOOL OnShowHidden();

protected:
	virtual void OnInitialUpdate();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int  OnCreate(LPCREATESTRUCT pcs);
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CLeftPane* m_pLeftPane;
	CRightPane m_RightPane;
	BOOL m_ShowHidden;
};

#endif  //MAINFRM_H

