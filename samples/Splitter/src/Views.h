/////////////////////////////
// Views.h
//

#ifndef VIEWS_H
#define VIEWS_H

////////////////////////////////////////
// Declaration of the CViewSimple class.
//
class CViewSimple : public CWnd
{
public:
    CViewSimple() {}
    virtual ~CViewSimple() {}

protected:
    virtual void OnDraw(CDC& dc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);
};

//////////////////////////////////////
// Declaration of the CViewText class.
//
class CViewText : public CWnd
{
public:
    CViewText() {}
    virtual ~CViewText() {}

protected:
    virtual void OnAttach();
    virtual void PreCreate(CREATESTRUCT& cs);
};

//////////////////////////////////////
// Declaration of the CViewTree class.
//
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

//////////////////////////////////////
// Declaration of the CViewList class.
//
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
