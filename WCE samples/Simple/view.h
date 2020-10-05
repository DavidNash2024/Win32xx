/////////////////////////////
// View.h


#ifndef VIEW_H
#define VIEW_H

#include "wxx_wincore.h"


///////////////////////////////////////////////
// CView manages the application's main window.
class CView : public CWnd
{
public:
    CView() {}
    virtual ~CView() {}
    virtual void OnDraw(CDC& dc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);
};

#endif   //VIEW_H
