/////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H


#include "wxx_wincore.h"


class CView : public CWnd
{
public:
    CView();
    virtual ~CView() {}

protected:
    virtual BOOL OnColor();
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnDestroy();
    virtual void OnDraw(CDC& dc);
    virtual BOOL OnExit();
    virtual void OnInitialUpdate();
    virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void PreRegisterClass(WNDCLASS& wc);
    virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    CBrush m_Brush;
    CPoint m_Point;
    CRect m_Rect;
};

#endif
