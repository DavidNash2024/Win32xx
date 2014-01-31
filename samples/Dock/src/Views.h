/////////////////////////////////////////////////
// Views.h
//  Declaration of the CPaneTopLeft, CPaneBottomLeft
//   and CView classes


#ifndef VIEWS_H
#define VIEWS_H



class CViewSimple : public CWnd
{
public:
	CViewSimple() {}
	virtual ~CViewSimple() {}

protected:
	virtual void OnDraw(CDC* pDC);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


class CViewText : public CWnd
{
public:
	CViewText() {}
	virtual ~CViewText() {}

protected:
	virtual void PreCreate(CREATESTRUCT &cs); 
	virtual void OnInitialUpdate(); 

};

class CViewTree : public CTreeView
{
public:
	CViewTree();
	virtual ~CViewTree();
	virtual HTREEITEM AddItem(HTREEITEM hParent, LPCTSTR sxText, int iImage);

protected:
	virtual void OnDestroy();
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HIMAGELIST m_himlNormal;
};

class CViewList : public CListView
{
public:
	CViewList();
	virtual ~CViewList();
	virtual int  AddItem(LPCTSTR szText, int nImage);
	virtual void InsertItems();
	virtual void OnDestroy();
	virtual void OnInitialUpdate();
	virtual void SetColumns();
	virtual BOOL SetSubItem(int nItem, int nSubItem, LPCTSTR szText);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HIMAGELIST m_himlSmall;
};


#endif // VIEWS_H
