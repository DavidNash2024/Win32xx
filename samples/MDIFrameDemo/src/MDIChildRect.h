//////////////////////////////////////////////////////
// MDIChildRect.h
//  Declaration of the CViewRect and CMDIChildRect classes

#ifndef MDICHILDRECT_H
#define MDICHILDRECT_H


// Declaration of CViewRect
class CViewRect : public CWnd
{
public:
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnDestroy();
    virtual LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnTimer(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    int m_cxClientMax;
    int m_cyClientMax;
};


// Declaration of CMDIChildRect
class CMDIChildRect : public CMDIChild
{
public:
    CMDIChildRect();
    virtual ~CMDIChildRect();

protected:
    virtual int OnCreate(CREATESTRUCT& cs);

private:
    CViewRect m_rectView;
    CMenu m_menu;

};


#endif  //MDICHILDRECT_H
