/////////////////////////////
// Rect.h
//

#ifndef RECT_H
#define RECT_H


//////////////////////////////////////////////////////////////
// CViewRect manages a window that displays random rectangles.
// This a view window used by CMyTabbedMDI.
class CViewRect : public CWnd
{
public:
    CViewRect() : m_cxClientMax(0), m_cyClientMax(0) {}
    virtual int OnCreate(CREATESTRUCT& cs);
    virtual void OnDestroy();
    virtual LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnTimer(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    int m_cxClientMax;
    int m_cyClientMax;
};

#endif  //RECT_H
