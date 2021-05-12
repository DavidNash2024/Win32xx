//////////////////////////////////////////////////////
// SplitterMDIChild.h
//  Declaration of the CSimpleView and CSplitterMDIChild class

#ifndef MDICHILDVIEW_H
#define MDICHILDVIEW_H


/////////////////////////////////////////////////////////
// CSimpleView manages a simple window. It is used as the
// view window by CDockSimple.
class CSimpleView : public CWnd
{
public:
    CSimpleView();
    COLORREF GetColor() {return m_color;}
    void SetColor(COLORREF color) { m_color = color; }

protected:
    virtual void OnDraw(CDC& dc);
    virtual LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    COLORREF m_color;
};


//////////////////////////////////////////
// CDockSimple manages a docker that has a
// CSimpleView view.
class CDockSimple : public CDocker
{
public:
    CDockSimple() {SetView(m_view);}
    virtual ~CDockSimple() {}

    CSimpleView& GetSimpleView() { return m_view; }

private:
    CSimpleView m_view;
};


//////////////////////////////////////////////////////
// CSplitterMDIChild manages the MDI child window.
// It uses CDockSimple as its view window. CDockSimple
// as other dockers docked within it.
class CSplitterMDIChild : public CMDIChild
{
public:
    CSplitterMDIChild();
    virtual ~CSplitterMDIChild();

protected:
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual BOOL    OnColor(COLORREF rgb);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    OnInitialUpdate();
    virtual LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CDockSimple m_view;
};

#endif  //MDICHILDVIEW_H
