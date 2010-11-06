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
	virtual void OnAbout();
	virtual void OnCreate();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnDestroy();
	virtual void OnInitialUpdate();
	virtual void OnPaint(CDC& dc);
	virtual void OnSize();
	virtual void OnTrayIcon(WPARAM wParam, LPARAM lParam);
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	BOOL m_IsMinimized;
};

#endif
