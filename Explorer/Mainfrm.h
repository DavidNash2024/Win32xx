///////////////////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINFRM_H
#define MAINFRM_H


#include "..\Win32++\frame.h"
#include "MainView.h"


class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();
	virtual CMainView& GetMainView() {return m_MainView;}
	virtual CListView& GetListView() {return m_MainView.GetListView();}
	virtual CTreeView& GetTreeView() {return m_MainView.GetTreeView();}
	virtual BOOL GetShowHidden() {return m_bShowHidden;}

protected:
	virtual void OnInitialUpdate();
	virtual BOOL OnCommand(UINT nID);
	virtual void OnCreate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void SetButtons(std::vector<UINT> ToolbarData);
	virtual void ViewPopup();
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CMainView m_MainView;
	BOOL m_bShowHidden;
};

#endif  //MAINFRM_H

