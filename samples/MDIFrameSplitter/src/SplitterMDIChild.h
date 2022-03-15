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
    ~CSimpleView() {}
    COLORREF GetColor() {return m_color;}
    void SetColor(COLORREF color) { m_color = color; }

protected:
    // Virtual functions that override base class functions
    virtual void OnDraw(CDC& dc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    // Message handlers
    LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);

    // Member variables
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
    // Virtual functions that override base class functions
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    OnInitialUpdate();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    // Command handlers
    BOOL    OnColor(COLORREF rgb);

    // Message handlers
    LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);

    // Member variables
    CDockSimple m_view;
};

#endif  //MDICHILDVIEW_H
