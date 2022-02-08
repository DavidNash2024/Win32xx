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
    virtual ~CView() {}
    BOOL Minimize();
    void Restore();

protected:
    virtual BOOL    OnAbout();
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual void    OnDestroy();
    virtual void    OnDraw(CDC& dc);
    virtual BOOL    OnFileExit();
    virtual void    OnInitialUpdate();
    virtual LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnSysCommand(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnTrayIcon(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMenu m_menu;
    bool m_isMinimized;
};

#endif
