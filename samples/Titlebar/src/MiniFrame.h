/////////////////////////////
// MiniFrame.h
//

#ifndef MINIFRAME_H
#define MINIFRAME_H

#include "view.h"
#include "resource.h"

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
    COLORREF active =       RGB(255, 255, 155);         // titlebar background
    COLORREF inactive =     RGB(255, 255, 218);         // titlebar background
    COLORREF activeItem =   RGB(33, 33, 33);            // text and max/min buttons
    COLORREF inactiveItem = RGB(191, 191, 191);         // text and max/min buttons
    COLORREF hover =        RGB(220, 220, 160);         // min/max button background
    COLORREF hoverClose =   RGB(220, 0, 0);             // close button background
    COLORREF topShadow =    RGB(100, 100, 100);         // top edge of titlebar
};

///////////////////////////////////////////////
// CMiniFrame manages the application's main window.
class CMiniFrame : public CWnd
{
public:
    CMiniFrame() : m_hoveredButton(TitlebarButton::None),
                   m_oldHoveredButton(TitlebarButton::None),
                   m_aboutDialog(IDW_ABOUT),
                   m_accel(0)
              {}
    virtual ~CMiniFrame() {}

    // Accessors
    ButtonRects    GetButtonRects() const;
    TitlebarButton GetHoveredButton() const;
    CRect    GetShadowRect() const;
    CRect    GetTitlebarRect() const;
    CRect    GetViewRect() const;
    bool     IsActive() const { return (GetForegroundWindow() == *this); }
    bool     IsMaximized() const;

protected:
    // Virtual functions that override base class functions
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    OnDestroy();
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMiniFrame(const CMiniFrame&) = delete;                // Disable copy construction
    CMiniFrame& operator = (const CMiniFrame&) = delete;   // Disable assignment operator

    void DrawBackground(CDC& dc) const;
    void DrawCloseButton(CDC& dc) const;
    void DrawMinimizeButton(CDC& dc) const;
    void DrawMaximizeButton(CDC& dc) const;
    void DrawTitleText(CDC& dc) const;
    void DrawTopShadow(CDC& dc) const;
    void DrawWindowIcon(CDC& dc) const;
    void OnSystemButton() const;
    void RecalcLayout() const;

    // Command handlers
    BOOL OnFileExit();
    BOOL OnHelp();

    // Message handlers
    LRESULT OnActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnEraseBkGnd(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCHitTest(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCCalcSize(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCMouseMove(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCMouseLeave(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCLButtonDblClk(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCRButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnPaint(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnSysCommand(UINT msg, WPARAM wparam, LPARAM lparam);

    // Member variables
    CDialog m_aboutDialog;              // Help about dialog.
    CMenuBar m_menubar;                 // Displays the menu in the titlebar.
    CView m_view;                       // View window.
    CMenu m_menu;                       // The application's menu.
    HACCEL m_accel;                     // The application's accelerator table.
    TitlebarButton m_hoveredButton;     // Current hovered button.
    TitlebarButton m_oldHoveredButton;  // Hovered button when left mouse button pressed.
    TitlebarColors m_colors;            // A struct holding the title bar colors.
};

#endif // MINIFRAME_H
