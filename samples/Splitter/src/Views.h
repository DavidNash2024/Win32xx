/////////////////////////////
// Views.h
//

#ifndef VIEWS_H
#define VIEWS_H


///////////////////////////////////////
// CViewSimple manages a simple window.
class CViewSimple : public CWnd
{
public:
    CViewSimple() {}
    virtual ~CViewSimple() {}

protected:
    // Virtual functions that override base class functions
    virtual void OnDraw(CDC& dc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CViewSimple(const CViewSimple&);               // Disable copy construction
    CViewSimple& operator=(const CViewSimple&);    // Disable assignment operator
};


//////////////////////////////////////////////
// CViewText manages a rich edit control.
// It is used as the view window by CDockText.
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


//////////////////////////////////////////////
// CViewText manages a tree view control.
// It is used as the view window by CDockTree.
class CViewTree : public CTreeView
{
public:
    CViewTree();
    virtual ~CViewTree();
    void SetDPIImages();

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CViewTree(const CViewTree&);                // Disable copy construction
    CViewTree& operator=(const CViewTree&);   // Disable assignment operator

    // Message handlers
    LRESULT OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam);
};

/////////////////////////////////////////////
// CViewList manages a list view control.
// It is used as the view window by CDockList.
class CViewList : public CListView
{
public:
    CViewList();
    virtual ~CViewList();
    void InsertItems();
    void SetColumns();
    void SetDPIColumnWidths();
    void SetDPIImages();

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach();
    LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CViewList(const CViewList&);                // Disable copy construction
    CViewList& operator=(const CViewList&);   // Disable assignment operator

    // Message handlers
    LRESULT OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam);
};

#endif // VIEWS_H
