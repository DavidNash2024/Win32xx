/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H

#include "wxx_wincore.h"


//////////////////////////////////////////
// CView manages the application's main window.
// The main window is round.
class CView : public CWnd
{
public:
    CView();
    virtual ~CView() {}

protected:
    // Virtual functions that override base class functions
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnDestroy();
    virtual void OnDraw(CDC& dc);
    virtual void OnInitialUpdate();
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual void PreRegisterClass(WNDCLASS& wc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView(const CView&);                // Disable copy construction
    CView& operator = (const CView&);   // Disable assignment operator

    // Command handlers
    BOOL OnColor();
    BOOL OnExit();

    // Message handlers
    LRESULT OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnRButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);

    // Member variables
    CBrush m_brush;
    CPoint m_point;
    CRect m_rect;
};


#endif
