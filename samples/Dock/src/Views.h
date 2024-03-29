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
    CViewSimple(const CViewSimple&);               // Disable copy construction
    CViewSimple& operator=(const CViewSimple&);    // Disable assignment operator

    // Message handlers
    LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
};


/////////////////////////////////////
// CViewText manages an edit control.
// The edit control displays text.
class CViewText : public CRichEdit
{
public:
    CViewText() {}
    virtual ~CViewText() {}
    void SetDPIFont();

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach();

private:
    CViewText(const CViewText&);                // Disable copy construction
    CViewText& operator=(const CViewText&);   // Disable assignment operator

    CFont m_font;
};


//////////////////////////////////////////
// CViewTree manages a tree view control.
// The tree view control displays classes.
class CViewTree : public CTreeView
{
public:
    CViewTree();
    virtual ~CViewTree();
    void SetDPIImages();

protected:
    // Virtual functions that override base class functions
    virtual void    OnAttach();
    virtual void    OnDestroy();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CViewTree(const CViewTree&);                // Disable copy construction
    CViewTree& operator=(const CViewTree&);   // Disable assignment operator

    // Message handlers
    LRESULT OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
};


/////////////////////////////////////////
// CViewList manages a list view control.
// The list view control displays files.
class CViewList : public CListView
{
public:
    CViewList();
    virtual ~CViewList();
    void SetDPIColumnWidths();
    void SetDPIImages();

protected:
    // Virtual functions that override base class functions
    virtual void    OnAttach();
    virtual void    OnDestroy();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CViewList(const CViewList&);                // Disable copy construction
    CViewList& operator=(const CViewList&);   // Disable assignment operator

    // Message handlers
    LRESULT OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);

    void InsertItems();
    void SetColumns();
};


#endif // VIEWS_H
