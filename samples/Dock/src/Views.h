//////////////////////////////////////////////
// Views.h
//  Declaration of the CViewSimple, CViewText,
//  CViewClasses and CViewFiles classes.

#ifndef VIEWS_H
#define VIEWS_H


//////////////////////////////////////////
// CViewClasses manages a tree view control.
// The tree view control displays classes.
class CViewClasses : public CTreeView
{
public:
    CViewClasses() = default;
    virtual ~CViewClasses() override;
    void SetDPIImages();

protected:
    // Virtual functions that override base class functions.
    virtual void    OnAttach() override;
    virtual void    PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewClasses(const CViewClasses&) = delete;
    CViewClasses& operator=(const CViewClasses&) = delete;

    // Message handlers
    LRESULT OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
};


/////////////////////////////////////////
// CViewFiles manages a list view control.
// The list view control displays files.
class CViewFiles : public CListView
{
public:
    CViewFiles() = default;
    virtual ~CViewFiles() override;
    void SetDPIColumnWidths();
    void SetDPIImages();

protected:
    // Virtual functions that override base class functions.
    virtual void    OnAttach() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewFiles(const CViewFiles&) = delete;
    CViewFiles& operator=(const CViewFiles&) = delete;

    // Message handlers
    LRESULT OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);

    void InsertItems();
    void SetColumns();
};


////////////////////////////////////////
// CViewSimple manages an simple window.
// Text is rendered by OnDraw.
class CViewSimple : public CWnd
{
public:
    CViewSimple() = default;
    virtual ~CViewSimple() override = default;

protected:
    // Virtual functions that override base class functions.
    virtual void    OnDraw(CDC& dc) override;
    virtual void    PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewSimple(const CViewSimple&) = delete;
    CViewSimple& operator=(const CViewSimple&) = delete;

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
    CViewText() = default;
    virtual ~CViewText() override = default;
    void SetDPIFont();

protected:
    // Virtual functions that override base class functions.
    virtual void OnAttach() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewText(const CViewText&) = delete;
    CViewText& operator=(const CViewText&) = delete;

    CFont m_font;
};

#endif // VIEWS_H
