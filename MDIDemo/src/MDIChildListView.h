//////////////////////////////////////////////////////////
// MDIChildListView.h
//  Declaration of the CViewList and CMDIChildListView classes


#ifndef MDICHILDLISTVIEW_H
#define MDICHILDLISTVIEW_H

#include "../../Win32++/mdi.h"
#include "../../Win32++/listview.h"

class CViewList : public CListView
{
public:
	CViewList();
	virtual ~CViewList();
	virtual int  AddItem(LPCTSTR szText, int nImage);
	virtual void InsertItems();
	virtual void OnInitialUpdate();
	virtual void SetColumns();
	virtual BOOL SetSubItem(int nItem, int nSubItem, LPCTSTR szText);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HIMAGELIST m_himlSmall;

};


class CMDIChildListView : public CMDIChild
{
public:
	CMDIChildListView();
	virtual ~CMDIChildListView();

protected:
	virtual void OnCreate();
	virtual void OnInitialUpdate();

private:
	CViewList m_ListView;

};

#endif  //MDICHILDLISTVIEW_H

