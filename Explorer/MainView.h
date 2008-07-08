//////////////////////////////////////////////////////
// MainView.h
//  Declaration of the CView class


#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "../DevWin++/Splitter.h"
#include "MyTreeView.h"
#include "MyListView.h"

#ifndef BTNS_WHOLEDROPDOWN
  #define BTNS_WHOLEDROPDOWN  0x0080
#endif

class CMainView : public CSplitter
{
public:
	CMainView();
	virtual ~CMainView(){}
	HWND GetOldFocus() {return m_hwndOldFocus;}
	CMyListView& GetListView() {return m_ListView;}
	CMyTreeView& GetTreeView() {return m_TreeView;}

protected:
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CMyTreeView m_TreeView;
	CMyListView m_ListView;
	HWND m_hwndOldFocus;

};

#endif  //MAINVIEW_H
