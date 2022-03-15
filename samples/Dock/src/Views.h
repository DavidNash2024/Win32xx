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
    // Virtual functions that override base class functions
    virtual void    OnDraw(CDC& dc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    // Message handlers
    LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
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
    // Virtual functions that override base class functions
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

protected:
    // Virtual functions that override base class functions
    virtual void    OnAttach();
    virtual void    OnDestroy();  
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    // Message handlers
    LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);

    // Member variables
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

protected:
    // Virtual functions that override base class functions
    virtual void    OnAttach();
    virtual void    OnDestroy();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    // Message handlers
    LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);

    void InsertItems();
    void SetColumns();

    // Member variables
    CImageList m_smallImages;
};


#endif // VIEWS_H
