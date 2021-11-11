/////////////////////////////
// MiniFrame.h
//

#ifndef MINIFRAME_H
#define MINIFRAME_H

#include "view.h"

enum class TitlebarButton
{
    None,
    System,
    Minimize,
    Maximize,
    Close,
};

struct ButtonRects
{
    CRect close;
    CRect maximize;
    CRect minimize;
    CRect system;
};

// Specify the colors for title bar.
struct TitlebarColors
{
    COLORREF active =       RGB(150, 200, 180);
    COLORREF inactive =     RGB(202, 227, 218);
    COLORREF activeItem =   RGB(33, 33, 33);
    COLORREF inactiveItem = RGB(127, 127, 127);
    COLORREF hover =        RGB(130, 180, 160);
    COLORREF hoverClose =   RGB(204, 0, 0);
    COLORREF topShadow =    RGB(100, 100, 100);
};

///////////////////////////////////////////////
// CMiniFrame manages the application's main window.
class CMiniFrame : public CWnd
{
public:
    CMiniFrame() : m_hoveredButton(TitlebarButton::None),
              m_oldHoveredButton(TitlebarButton::None)
              {}
    virtual ~CMiniFrame() {}

    void DrawMinimizeButton(CDC& dc) const;
    void DrawMaximizeButton(CDC& dc) const;
    void DrawCloseButton(CDC& dc) const;
    void DrawTitleText(CDC& dc) const;
    void DrawWindowIcon(CDC& dc) const;
    void SystemMenu() const;

    ButtonRects    GetButtonRects() const;
    TitlebarButton GetHoveredButton() const;
    CRect    GetShadowRect() const;
    CRect    GetTitlebarRect() const;
    CRect    GetViewRect() const;
    bool     IsActive() const { return (GetForegroundWindow() == *this); }
    bool     IsMaximized() const;

    LRESULT OnActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnEraseBkGnd(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCHitTest(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCCalcSize(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCMouseMove(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCMouseLeave(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnPaint(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnSysCommand(UINT msg, WPARAM wparam, LPARAM lparam);

protected:
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnDestroy();
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView m_view;
    TitlebarButton m_hoveredButton;
    TitlebarButton m_oldHoveredButton;
    TitlebarColors m_colors;
};

#endif // MINIFRAME_H
