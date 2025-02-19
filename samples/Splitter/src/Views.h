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
    CViewSimple() = default;
    virtual ~CViewSimple() override = default;

protected:
    // Virtual functions that override base class functions
    virtual void OnDraw(CDC& dc) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewSimple(const CViewSimple&) = delete;
    CViewSimple& operator=(const CViewSimple&) = delete;
};


//////////////////////////////////////////////
// CViewText manages a rich edit control.
// It is used as the view window by CDockText.
class CViewText : public CRichEdit
{
public:
    CViewText() = default;
    virtual ~CViewText() override = default;
    void SetDPIFont();

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewText(const CViewText&) = delete;
    CViewText& operator=(const CViewText&) = delete;

    CFont m_font;
};


//////////////////////////////////////////////
// CViewText manages a tree view control.
// It is used as the view window by CDockTree.
class CViewTree : public CTreeView
{
public:
    CViewTree() = default;
    virtual ~CViewTree() override;
    void SetDPIImages();

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewTree(const CViewTree&) = delete;
    CViewTree& operator=(const CViewTree&) = delete;

    // Message handlers
    LRESULT OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam);
};

/////////////////////////////////////////////
// CViewList manages a list view control.
// It is used as the view window by CDockList.
class CViewList : public CListView
{
public:
    CViewList() = default;
    virtual ~CViewList() override;
    void InsertItems();
    void SetColumns();
    void SetDPIColumnWidths();
    void SetDPIImages();

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewList(const CViewList&) = delete;
    CViewList& operator=(const CViewList&) = delete;

    // Message handlers
    LRESULT OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam);
};

#endif // VIEWS_H
