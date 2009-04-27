///////////////////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINFRM_H
#define MAINFRM_H


#include "../Win32++/frame.h"
#include "SplitterPanes.h"

#ifndef BTNS_WHOLEDROPDOWN
  #define BTNS_WHOLEDROPDOWN  0x0080
#endif

class CMainFrame : public CFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame();
	CMyListView* GetListView() const {return (CMyListView*)m_RightPane->GetView();}
	CMyTreeView* GetTreeView() const {return (CMyTreeView*)m_LeftPane.GetView();}
	BOOL GetShowHidden() {return m_bShowHidden;}

protected:
	virtual void OnInitialUpdate();
	virtual void OnClose();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void SetupToolbars();
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void DoPopupMenu();
	void SetButtons();

private:
//	CMainView m_MainView;
	CLeftPane m_LeftPane;
	CRightPane* m_RightPane;
	BOOL m_bShowHidden;
};

#endif  //MAINFRM_H

