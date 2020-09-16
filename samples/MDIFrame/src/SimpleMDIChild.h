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
    virtual void OnDraw(CDC& dc);
    virtual LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

    COLORREF GetColor() {return m_color;}
    void SetColor(COLORREF color) { m_color = color; }

private:
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
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int OnCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CSimpleView m_view;
    CMenu m_menu;
};

#endif  //MDICHILDVIEW_H
