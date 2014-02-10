//////////////////////////////////////////////////////
// MDIChildRect.h
//  Declaration of the CViewRect and CMDIChildRect classes

#ifndef MDICHILDRECT_H
#define MDICHILDRECT_H


// Declaration of CViewRect
class CViewRect : public CWnd
{
public:
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual LRESULT OnSize(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnTimer(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	int m_cxClientMax;
	int m_cyClientMax;
};


// Declaration of CMDIChildRect
class CMDIChildRect : public CMDIChild
{
public:
	CMDIChildRect();
	virtual ~CMDIChildRect();

protected:
	virtual void OnInitialUpdate();

private:
	CViewRect m_RectView;

};


#endif  //MDICHILDRECT_H
