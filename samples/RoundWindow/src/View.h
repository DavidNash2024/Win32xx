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
    virtual ~CView() override = default;
    void PositionWindow();
    void SetRoundRegion();

protected:
    // Virtual functions that override base class functions
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual void OnDestroy() override;
    virtual void OnDraw(CDC& dc) override;
    virtual void OnInitialUpdate() override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual void PreRegisterClass(WNDCLASS& wc) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CView(const CView&) = delete;
    CView& operator=(const CView&) = delete;

    // Command handlers
    BOOL OnColor();
    BOOL OnExit();

    // Message handlers
    LRESULT OnDpiChanged(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnRButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);

    // Member variables
    CBrush m_brush;
    CPoint m_grabPoint;
    CRect m_rect;
    int m_oldDPI;
};


#endif
