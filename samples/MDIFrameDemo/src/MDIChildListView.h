//////////////////////////////////////////////////////////
// MDIChildListView.h
//  Declaration of the CViewList and CMDIChildListView classes

#ifndef MDICHILDLISTVIEW_H
#define MDICHILDLISTVIEW_H


class CViewList : public CListView
{
public:
	CViewList();
	virtual ~CViewList();
	virtual int  AddItem(LPCTSTR szText, int nImage);
	virtual void OnDestroy();
	virtual void InsertItems();
	virtual void OnInitialUpdate();
	virtual void SetColumns();
	virtual BOOL SetSubItem(int nItem, int nSubItem, LPCTSTR szText);

private:
	CImageList m_imlSmall;

};


class CMDIChildListView : public CMDIChild
{
public:
	CMDIChildListView();
	virtual ~CMDIChildListView();

protected:
	virtual int  OnCreate(CREATESTRUCT& cs);

private:
	CViewList m_ListView;
	CMenu m_Menu;

};

#endif  //MDICHILDLISTVIEW_H

