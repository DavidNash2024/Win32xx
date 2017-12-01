/////////////////////////////////////////////////
// Views.h
//  Declaration of the CViewSimple, CViewText,
//   CViewTree, and CViewList classes

#ifndef VIEWS_H
#define VIEWS_H



class CViewSimple : public CWnd
{
public:
    CViewSimple() {}
    virtual ~CViewSimple() {}

protected:
    virtual void OnDraw(CDC& dc);
    virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


class CViewText : public CWnd
{
public:
    CViewText() {}
    virtual ~CViewText() {}

protected:
    virtual void OnAttach();
    virtual void PreCreate(CREATESTRUCT& cs); 

};

class CViewTree : public CTreeView
{
public:
    CViewTree();
    virtual ~CViewTree();
    virtual void OnAttach();
    virtual HTREEITEM AddItem(HTREEITEM hParent, LPCTSTR sxText, int iImage);

private:
    CImageList m_imlNormal;

};

class CViewList : public CListView
{
public:
    CViewList();
    virtual ~CViewList();
    virtual int  AddItem(LPCTSTR szText, int nImage);
    virtual void InsertItems();
    virtual void OnAttach();
    virtual void SetColumns();
    virtual BOOL SetSubItem(int nItem, int nSubItem, LPCTSTR szText);

private:
    CImageList m_imlSmall;

};


#endif // VIEWS_H
