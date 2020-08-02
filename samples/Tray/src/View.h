/////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H


class CView : public CWnd
{
public:
    CView() : m_menu(IDW_MAIN), m_isMinimized(FALSE) {}
    virtual ~CView() {}
    void Minimize();
    void Restore();

protected:
    virtual void    OnAbout();
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual void    OnDestroy();
    virtual void    OnDraw(CDC& dc);    
    virtual void    OnFileExit();
    virtual void    OnInitialUpdate();
    virtual LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnSysCommand(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnTrayIcon(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMenu m_menu;
    BOOL m_isMinimized;
};

#endif
