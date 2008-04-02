///////////////////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINFRM_H
#define MAINFRM_H


#include "../Win32++/frame.h"
#include "MainView.h"


class CMainFrame : public CFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame();
	CMainView& GetMainView() {return m_MainView;}
	CListView& GetListView() {return m_MainView.GetListView();}
	CTreeView& GetTreeView() {return m_MainView.GetTreeView();}
	BOOL GetShowHidden() {return m_bShowHidden;}

protected:
	virtual void OnInitialUpdate();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void OnSetFocus();
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void DoPopupMenu();
	void SetButtons();

private:
	CMainView m_MainView;
	BOOL m_bShowHidden;
};

#endif  //MAINFRM_H

