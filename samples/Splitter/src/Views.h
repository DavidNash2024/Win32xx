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
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);
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
    virtual HTREEITEM AddItem(HTREEITEM hParent, LPCTSTR text, int image);

private:
    CImageList m_smallImages;

};

class CViewList : public CListView
{
public:
    CViewList();
    virtual ~CViewList();
    virtual int  AddItem(LPCTSTR text, int image);
    virtual void InsertItems();
    virtual void OnAttach();
    virtual void SetColumns();

private:
    CImageList m_smallImages;

};


#endif // VIEWS_H
