//////////////////////////////////////////////////////
// MDIChildSimple.h
//  Declaration of the CViewSimple and CMDIChildSimple class

#ifndef MDICHILDVIEW_H
#define MDICHILDVIEW_H


// Declaration of CViewSimple
class CViewSimple : public CWnd
{
public:
    CViewSimple();
    virtual void OnDraw(CDC& dc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);
    
    COLORREF GetColor() {return m_color;}
    void SetColor(COLORREF color) { m_color = color; }

private:
    COLORREF m_color;
};


// Declaration of CMDIChildSimple
class CMDIChildSimple : public CMDIChild
{
public:
    CMDIChildSimple();
    virtual ~CMDIChildSimple();

protected:
    virtual void OnClose();
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CViewSimple m_view;
    CMenu m_menu;
};

#endif  //MDICHILDVIEW_H
