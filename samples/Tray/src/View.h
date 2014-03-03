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
	virtual int     OnCreate(LPCREATESTRUCT pcs);
	virtual BOOL    OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void    OnDestroy();
	virtual void    OnDraw(CDC* pDC);	
	virtual void	OnFileExit();
	virtual void    OnInitialUpdate();
	virtual LRESULT OnSize();
	virtual LRESULT OnSysCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnTrayIcon(WPARAM wParam, LPARAM lParam);
	virtual void    PreCreate(CREATESTRUCT& cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	BOOL m_IsMinimized;
};

#endif
