//////////////////////////////////////////////
// Views.h
//  Declaration of the CViewSimple, CViewText,
//  CViewTree and CViewList classes.

#ifndef VIEWS_H
#define VIEWS_H


////////////////////////////////////////
// CViewSimple manages an simple window.
// Text is rendered by OnDraw.
class CViewSimple : public CWnd
{
public:
    CViewSimple() {}
    virtual ~CViewSimple() {}

protected:
    virtual void OnDraw(CDC& dc);
    virtual LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);
};


/////////////////////////////////////
// CViewText manages an edit control.
// The edit control displays text.
class CViewText : public CWnd
{
public:
    CViewText() {}
    virtual ~CViewText() {}

protected:
    virtual void OnAttach();
    virtual void PreCreate(CREATESTRUCT& cs);
};


//////////////////////////////////////////
// CViewTree manages a tree view control.
// The tree view control displays classes.
class CViewTree : public CTreeView
{
public:
    CViewTree();
    virtual ~CViewTree();
    virtual HTREEITEM AddItem(HTREEITEM hParent, LPCTSTR text, int image);

protected:
    virtual void OnAttach();
    virtual void OnDestroy();
    virtual LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CImageList m_normalImages;
};


/////////////////////////////////////////
// CViewList manages a list view control.
// The list view control displays files.
class CViewList : public CListView
{
public:
    CViewList();
    virtual ~CViewList();
    virtual int  AddItem(LPCTSTR text, int image);
    virtual void InsertItems();
    virtual void OnAttach();
    virtual void OnDestroy();
    virtual LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void SetColumns();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CImageList m_smallImages;
};


#endif // VIEWS_H
