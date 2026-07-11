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
    virtual ~CView() override = default;

protected:
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual void    OnDestroy() override;
    virtual LRESULT OnDpiChanged(UINT, WPARAM, LPARAM lparam) override;
    virtual void    OnDraw(CDC& dc) override;
    virtual void    OnInitialUpdate() override;
    virtual LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual LRESULT OnSysCommand(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual LRESULT OnTrayIcon(UINT msg, WPARAM wparam, LPARAM lparam);
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

    // Member variables
    CTaskDialog m_dialog;
    CMenu m_menu;
    bool m_isMinimized;
};

#endif
