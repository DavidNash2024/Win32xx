//////////////////////////////////////////////////////
// SimpleMDIChild.h
//  Declaration of the CSimpleView and CSimpleMDIChild class

#ifndef MDICHILDVIEW_H
#define MDICHILDVIEW_H


/////////////////////////////////////////////////////
// CSimpleView manages CSimpleMDIChild's view window.
class CSimpleView : public CWnd
{
public:
    CSimpleView();

    COLORREF GetColor() { return m_color; }
    void SetColor(COLORREF color) { m_color = color; }

protected:
    // Virtual functions that override base class functions
    virtual void OnDraw(CDC& dc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CSimpleView(const CSimpleView&);                // Disable copy construction
    CSimpleView& operator = (const CSimpleView&);   // Disable assignment operator

    // Message handlers
    LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);

    COLORREF m_color;
};

//////////////////////////////////////////////
// CSimpleMDIChild manages a MDI Child window.
// It uses CSimpleView as its view window.
class CSimpleMDIChild : public CMDIChild
{
public:
    CSimpleMDIChild();
    virtual ~CSimpleMDIChild();

protected:
    // Virtual functions that override base class functions
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CSimpleMDIChild(const CSimpleMDIChild&);                // Disable copy construction
    CSimpleMDIChild& operator = (const CSimpleMDIChild&);   // Disable assignment operator

    CSimpleView m_view;
    CMenu m_menu;
};

#endif  //MDICHILDVIEW_H
