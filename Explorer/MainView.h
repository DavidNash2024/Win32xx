//////////////////////////////////////////////////////
// MainView.h
//  Declaration of the CView class


#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "../Win32++/Splitter.h"
#include "TreeView.h"
#include "ListView.h"

#ifndef BTNS_WHOLEDROPDOWN
  #define BTNS_WHOLEDROPDOWN  0x0080
#endif

class CMainView : public CSplitter
{
public:
	CMainView(int nBarpos = 0, bool bVertical = true);
	virtual ~CMainView(){}
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	CListView& GetListView() {return m_ListView;}
	CTreeView& GetTreeView() {return m_TreeView;}

private:
	CTreeView m_TreeView;
	CListView m_ListView;
};

#endif  //MAINVIEW_H
