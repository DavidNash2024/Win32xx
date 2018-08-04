/////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H


#include "wxx_wincore.h"


class CView : public CWnd
{
public:
    CView() {}
    virtual ~CView() {}

protected:
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnDestroy();
    virtual void OnDraw(CDC& dc);   
    virtual void OnInitialUpdate();
    virtual void OnSize();
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);
};

#endif
