#ifndef VIEW_H
#define VIEW_H

#include "wxx_wincore.h"


class CView : public CWnd
{
public:
    CView() {}
    virtual ~CView() {}
    virtual void OnDraw(CDC& dc);
    virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif   //VIEW_H
