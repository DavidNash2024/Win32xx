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
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnDestroy();
    virtual void OnDraw(CDC& dc);
    virtual BOOL OnExit();
    virtual void OnInitialUpdate();
	virtual LRESULT OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
	virtual LRESULT OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);
	virtual LRESULT OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam);
	virtual LRESULT OnRButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void PreRegisterClass(WNDCLASS& wc);
	virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CBrush m_brush;
    CPoint m_point;
    CRect m_rect;
};

#endif
