/////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H


class CView : public CWnd
{
public:
	CView() : m_IsMinimized(FALSE) {}
	virtual ~CView() {}
	void Minimize();
	void Restore();

protected:
	virtual void    OnAbout();
	virtual int     OnCreate(CREATESTRUCT& cs);
	virtual BOOL    OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void    OnDestroy();
	virtual void    OnDraw(CDC& dc);	
	virtual void	OnFileExit();
	virtual void    OnInitialUpdate();
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnTrayIcon(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void    PreCreate(CREATESTRUCT& cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	BOOL m_IsMinimized;
};

#endif
