/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H

// VS6 requires these macros to be set before including wxx_wincore.h.
#define WINVER            0x0400
#define _WIN32_WINDOWS    0x0400
#define _WIN32_IE         0x0400

#include "wxx_wincore.h"


///////////////////////////////////////////////
// CView manages the application's main window.
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
