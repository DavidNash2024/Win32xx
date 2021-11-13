/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"

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
    COLORREF active = RGB(150, 200, 180);
    COLORREF inactive = RGB(202, 227, 218);
    COLORREF activeItem = RGB(33, 33, 33);
    COLORREF inactiveItem = RGB(191, 191, 191);
    COLORREF hover = RGB(130, 180, 160);
    COLORREF hoverClose = RGB(204, 0, 0);
    COLORREF topShadow = RGB(100, 100, 100);
};

///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame which has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();
    virtual HWND Create(HWND parent = 0);

    void DrawMinimizeButton(CDC& dc) const;
    void DrawMaximizeButton(CDC& dc) const;
    void DrawCloseButton(CDC& dc) const;
    void DrawTitleText(CDC& dc) const;
    void DrawWindowIcon(CDC& dc) const;

    CDoc& GetDoc()      { return m_view.GetDoc(); }
    ButtonRects    GetButtonRects() const;
    TitlebarButton GetHoveredButton() const;
    CRect GetShadowRect() const;
    CRect GetTitlebarRect() const;

    bool  IsActive() const { return (GetForegroundWindow() == *this); }
    bool  IsMaximized() const;

    LRESULT OnActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    BOOL    OnFileExit();
    BOOL    OnFileOpen();
    BOOL    OnFilePrint();
    BOOL    OnFileSave();
    BOOL    OnFilePreview();
    LRESULT OnPreviewClose();
    LRESULT OnPreviewPrint();
    LRESULT OnPreviewSetup();

    LRESULT OnNCCalcSize(UINT, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCHitTest(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCMouseMove(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCMouseLeave(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnPaint(UINT, WPARAM, LPARAM);
    LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnSysCommand(UINT msg, WPARAM wparam, LPARAM lparam);

    void SystemMenu() const;

protected:
    virtual CRect   GetViewRect() const;
    virtual void    OnClose();
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    OnInitialUpdate();
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual void    RecalcLayout();
    virtual void    SetupMenuIcons();
    virtual void    SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView m_view;
    CPrintPreview<CView> m_preview;
    BOOL m_isToolbarShown;
    TitlebarButton m_hoveredButton;
    TitlebarButton m_oldHoveredButton;
    TitlebarColors m_colors;
};

#endif //MAINFRM_H

