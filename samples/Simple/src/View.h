/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H

#include "targetver.h"
#include "wxx_wincore.h"


///////////////////////////////////////////////
// CView manages the application's main window.
class CView : public CWnd
{
public:
    CView() {}
    virtual ~CView() {}

protected:
    // Virtual functions that override base class functions
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnDestroy();
    virtual void OnDraw(CDC& dc);
    virtual void OnInitialUpdate();
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView(const CView&);               // Disable copy construction
    CView& operator=(const CView&);    // Disable assignment operator

    // Message handlers
    void OnSize();
};

#endif
