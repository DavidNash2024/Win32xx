//////////////////////////////////////////////////////
// MDIChildMax.h
//  Declaration of the CViewMax and CMDIChildMax classes

#ifndef MDICHILDMAX_H
#define MDICHILDMAX_H


////////////////////////////////////////////////////////
// CViewMax is used as the view window for CMDIChildMax.
class CViewMax : public CWnd
{
public:
    CViewMax() {}
    virtual ~CViewMax() {}

protected:
    virtual void OnDraw(CDC& dc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);
};


////////////////////////////////////////////////////////////
// CMDIChildMax manages a MDI child window that is maximised
// when it is created. It uses CViewMax as its view window.
class CMDIChildMax : public CMDIChild
{
public:
    CMDIChildMax();
    virtual ~CMDIChildMax();

protected:
    virtual int OnCreate(CREATESTRUCT& cs);
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    CViewMax m_maxView;
    CMenu m_menu;
};

#endif  //MDICHILDMAX_H
