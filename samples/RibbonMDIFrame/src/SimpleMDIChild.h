/////////////////////////////
// SimpleMDIChild.h
//

#ifndef MDICHILDVIEW_H
#define MDICHILDVIEW_H


// User defined window message sent when the Simple MDI Child is created
// WPARAM is a pointer to the view window.
#define UWM_SIMPLECREATED       (WM_APP + 0x0001)


///////////////////////////////////////////////////////////
// CSimpleView manages the view window for CSimpleMDIChild.
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


////////////////////////////////////////////
// CSimpleMDIChild manages a MDI child window.
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
