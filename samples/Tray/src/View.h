/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H


////////////////////////////////////////////////////
// CView is the application's main window. It can be
// minimized to the tray.
class CView : public CWnd
{
public:
    CView() : m_menu(IDW_MAIN), m_isMinimized(false) {}
    virtual ~CView() override {}

protected:
    // Virtual functions that override base class functions

    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual void    OnDestroy() override;
    virtual void    OnDraw(CDC& dc) override;
    virtual void    OnInitialUpdate() override;
    virtual void    PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CView(const CView&) = delete;
    CView& operator=(const CView&) = delete;

    BOOL Minimize();
    void Restore();

    // Command handlers
    BOOL    OnAbout();
    BOOL    OnFileExit();

    // Message handlers
    LRESULT OnDpiChanged(UINT, WPARAM, LPARAM lparam);
    LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnSysCommand(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnTrayIcon(UINT msg, WPARAM wparam, LPARAM lparam);

    // Member variables
    CMenu m_menu;
    bool m_isMinimized;
};

#endif
