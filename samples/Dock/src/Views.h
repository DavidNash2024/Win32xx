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
    CViewClasses();
    virtual ~CViewClasses();
    void SetDPIImages();

protected:
    // Virtual functions that override base class functions.
    virtual void    OnAttach();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CViewClasses(const CViewClasses&);              // Disable copy construction
    CViewClasses& operator=(const CViewClasses&);   // Disable assignment operator

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
    CViewFiles();
    virtual ~CViewFiles();
    void SetDPIColumnWidths();
    void SetDPIImages();

protected:
    // Virtual functions that override base class functions.
    virtual void    OnAttach();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CViewFiles(const CViewFiles&);              // Disable copy construction
    CViewFiles& operator=(const CViewFiles&);   // Disable assignment operator

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
    CViewSimple() {}
    virtual ~CViewSimple() {}

protected:
    // Virtual functions that override base class functions.
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
    // Virtual functions that override base class functions.
    virtual void OnAttach();

private:
    CViewText(const CViewText&);              // Disable copy construction
    CViewText& operator=(const CViewText&);   // Disable assignment operator

    CFont m_font;
};

#endif // VIEWS_H
