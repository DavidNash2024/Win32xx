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
    COLORREF active = RGB(150, 200, 180);           // titlebar background
    COLORREF inactive = RGB(202, 227, 218);         // titlebar background
    COLORREF activeItem = RGB(33, 33, 33);          // text and max/min buttons
    COLORREF inactiveItem = RGB(160, 160, 160);     // text and max/min buttons
    COLORREF hover = RGB(130, 180, 160);            // min/max button background
    COLORREF hoverClose = RGB(204, 0, 0);           // close button background
    COLORREF topShadow = RGB(100, 100, 100);        // top edge of titlebar
};

///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame that has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame() override = default;
    virtual HWND Create(HWND parent = nullptr) override;

    // Accessors
    CDoc& GetDoc()      { return m_view.GetDoc(); }
    ButtonRects    GetButtonRects() const;
    TitlebarButton GetHoveredButton() const;
    CRect GetShadowRect() const;
    CRect GetTitlebarRect() const;
    bool  IsActive() const { return (::GetForegroundWindow() == *this); }
    bool  IsMaximized() const;

protected:
    // Virtual functions that override base class functions
    virtual CRect   GetViewRect() const override;
    virtual void    OnClose() override;
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual LRESULT OnCustomDraw(LPNMHDR pNMHDR) override;
    virtual void    OnInitialUpdate() override;
    virtual void    RecalcLayout() override;
    virtual void    SetupMenuIcons() override;
    virtual void    SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    void DrawCloseButton(CDC& dc) const;
    void DrawBackground(CDC& dc) const;
    void DrawMinimizeButton(CDC& dc) const;
    void DrawMaximizeButton(CDC& dc) const;
    void DrawTitleText(CDC& dc) const;
    void DrawTopShadow(CDC& dc) const;
    void DrawWindowIcon(CDC& dc) const;

    // Command Handlers
    BOOL OnFileExit();
    BOOL OnFileOpen();
    BOOL OnFilePrint();
    BOOL OnFileSave();
    BOOL OnFilePreview();
    BOOL OnFullMode();
    BOOL OnMiniMode();

    // Message handlers.
    LRESULT OnActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCCalcSize(UINT, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCHitTest(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCMouseMove(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCMouseLeave(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCRButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnPaint(UINT, WPARAM, LPARAM);
    LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnPreviewClose();
    LRESULT OnPreviewPrint();
    LRESULT OnPreviewSetup();

    CView m_view;
    CPrintPreview<CView> m_preview;
    TitlebarButton m_hoveredButton;
    TitlebarButton m_oldHoveredButton;
    TitlebarColors m_colors;
    bool m_isToolbarShown;
    bool m_isMiniFrame;
};

#endif //MAINFRM_H

