//////////////////////////////////////////////////////
// SplitterMDIChild.h
//  Declaration of the CSimpleView and CSplitterMDIChild class

#ifndef MDICHILDVIEW_H
#define MDICHILDVIEW_H


// Declaration of CSimpleView
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

// Declaration of the CDockSimple class
class CDockSimple : public CDocker
{
public:
    CDockSimple() {SetView(m_view);}
    virtual ~CDockSimple() {}

    CSimpleView& GetSimpleView() { return m_view; }

private:
    CSimpleView m_view;

};

// Declaration of CSplitterMDIChild
class CSplitterMDIChild : public CMDIChild
{
public:
    CSplitterMDIChild();
    virtual ~CSplitterMDIChild();

protected:
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual void    OnColor(COLORREF rgb);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    OnInitialUpdate();
    virtual LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CDockSimple m_view;
};

#endif  //MDICHILDVIEW_H
