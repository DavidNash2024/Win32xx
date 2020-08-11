//////////////////////////////////////////////////////
// MDIChildMax.h
//  Declaration of the CViewMax and CMDIChildMax classes

#ifndef MDICHILDMAX_H
#define MDICHILDMAX_H

////////////////////////////////////
// Declaration of the CViewMax class
//
class CViewMax : public CWnd
{
public:
    CViewMax() {}
    virtual ~CViewMax() {}

protected:
    virtual void OnDraw(CDC& dc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);
};

////////////////////////////////////////
// Declaration of the CMDIChildMax class
//
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
