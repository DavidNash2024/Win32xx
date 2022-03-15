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
    virtual ~CSimpleView() {}
    COLORREF GetColor() { return m_color; }
    void SetColor(COLORREF color) { m_color = color; }

protected:
    // Virtual functions that override base class functions
    virtual void OnDraw(CDC& dc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    // Message handlers
    virtual LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);

    // Member variables
    COLORREF m_color;
};

//////////////////////////////////////////
// CSimpleMDIChild manages a MDI child.
// It uses CSimpleView as the view window.
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
    CSimpleView m_view;
    CMenu m_menu;
};

#endif  //MDICHILDVIEW_H
